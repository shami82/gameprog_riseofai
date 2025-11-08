#include "Level2.h"

Level2::Level2()                                      : Scene { {0.0f}, nullptr   } {}
Level2::Level2(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

Level2::~Level2() { shutdown(); }

void Level2::initialise()
{
   mGameState.nextSceneID = -1; // "don't switch scenes yet"-state

   mGameState.bgm = LoadMusicStream("assets/the_search.mp3");
   SetMusicVolume(mGameState.bgm, 0.7f);
   PlayMusicStream(mGameState.bgm);
   mGameState.lvlCompleteSound = LoadSound("assets/lvlcomplete.wav");
   SetSoundVolume(mGameState.lvlCompleteSound, 1.2f);
   mGameState.jumpSound = LoadSound("assets/jump.wav");
   SetSoundVolume(mGameState.jumpSound, 0.1f);

   textureZorp = LoadTexture("assets/zorpsheet.PNG");
   textureBG = LoadTexture("assets/spacebg.PNG");
   textureFlyer = LoadTexture("assets/flyer.PNG");
   textureHeart = LoadTexture("assets/heart.PNG");
   textureRocketStation = LoadTexture("assets/rocketstation.PNG");
   textureRocketMov1 = LoadTexture("assets/rocketmov1.PNG");
   textureRocketMov2 = LoadTexture("assets/rocketmov2.PNG");

   /*
      ----------- BACKGROUND -----------
   */
   mGameState.bg = new Entity(
      mOrigin,                                        // position
      { 990.0f, 720.0f },                             // size
      textureBG,                                      // texture file address
      NONE                                            // type
   );

   /*
      ----------- MAP -----------
   */
   mGameState.map = new Map(
      LEVEL_WIDTH, LEVEL_HEIGHT,   // map grid cols & rows
      (unsigned int *) mLevelData, // grid data
      "assets/plan2reg.PNG",       // texture filepath
      TILE_DIMENSION,              // tile size
      1, 1,                        // texture cols & rows
      mOrigin                      // in-game origin
   );

   /*
      ----------- ZORP :D -----------
   */
   std::map<Direction, std::vector<int>> zorpAnimationAtlas = {
      {UP,    { 0,  1,  2,  3 }}, // idle
      {RIGHT, { 4,  5,  6,  7 }},
      {LEFT,  { 8,  9, 10, 11 }},
      {DOWN,  { 0,  1,  2,  3 }} // idle
   };

   Vector2 zorpScale = { TILE_DIMENSION, TILE_DIMENSION * (17.0f / 16.0f) };

   mGameState.zorp = new Entity(
      {TILE_DIMENSION * 2.75f, TILE_DIMENSION * 10.0f}, // col, row
      zorpScale,                                // scale
      textureZorp,                              // texture file address
      ATLAS,                                    // single image or atlas?
      { 3, 4 },                                 // atlas dimensions
      zorpAnimationAtlas,                       // actual atlas
      PLAYER                                    // entity type
   );

   mGameState.zorp->setJumpingPower(500.0f);
   mGameState.zorp->setColliderDimensions({
      mGameState.zorp->getScale().x * 0.65f,
      mGameState.zorp->getScale().y
   });
   mGameState.zorp->setSpeed(165);
   mGameState.zorp->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

   /*
      ----------- FLYER -----------
   */

   mGameState.flyer = new Entity(
      {TILE_DIMENSION * 4.75f, TILE_DIMENSION * 8.75f}, // starting position
      { TILE_DIMENSION, TILE_DIMENSION },    // scale
      textureFlyer,                          // texture
      NONE                                    // entity type
   );

   mGameState.flyer->setSpeed(100);
   mGameState.flyer->setAcceleration({0.0f, 0.0f});
   mGameState.flyer->setDirection(UP);

   /*
      ----------- FLYER2 -----------
   */

   mGameState.flyer2 = new Entity(
      {TILE_DIMENSION * 10.75f, TILE_DIMENSION * 8.75f}, // starting position
      { TILE_DIMENSION, TILE_DIMENSION },    // scale
      textureFlyer,                          // texture
      NONE                                    // entity type
   );

   mGameState.flyer2->setSpeed(100);
   mGameState.flyer2->setAcceleration({0.0f, 0.0f});
   mGameState.flyer2->setDirection(DOWN);

   /*
      ----------- ROCKET (THE GOAL) -----------
   */

   mGameState.rocket = new Entity(
      {TILE_DIMENSION * 14.25f, TILE_DIMENSION * 10.0f}, // starting position
      { TILE_DIMENSION * (28.0f/16.0f), TILE_DIMENSION * (32.0f/16.0f)},    // scale
      textureRocketStation,                          // texture
      NONE                                    // entity type
   );

   // init for animating later
   rocketReached = false;
   rocketMovTimer = 0.0f;
   rocketCurrent = 0;

   /*
      ----------- CAMERA -----------
   */
   mGameState.camera = { 0 };                                    // zero initialize
   mGameState.camera.target = mGameState.zorp->getPosition();    // camera follows player
   mGameState.camera.offset = {mOrigin.x, mOrigin.y + 100};      // camera offset to center of screen
   mGameState.camera.rotation = 0.0f;                            // no rotation
   mGameState.camera.zoom = 1.25f;                               // zoom more
}

void Level2::update(float deltaTime)
{
   UpdateMusicStream(mGameState.bgm);
   mGameState.zorp->update(
      deltaTime,      // delta time / fixed timestep
      nullptr,        // player
      mGameState.map, // map
      nullptr,        // collidable entities
      0               // col. entity count
   );

   mGameState.flyer->update(
      deltaTime,      // delta time / fixed timestep
      nullptr,        // player
      mGameState.map, // map
      nullptr,        // collidable entities
      0               // col. entity count
   );

   if (mGameState.flyer){ // FLYER MOVEMENTS
      static float startY = mGameState.flyer->getPosition().y;

      // top and bottom limits
      static float flyerTopY = startY - 180.0f;
      static float flyerBottomY = startY + 180.0f;

      static bool movingUp = true; // current direction
      Vector2 pos = mGameState.flyer->getPosition();

      if (movingUp){
         pos.y -= mGameState.flyer->getSpeed() * deltaTime;
         if (pos.y <= flyerTopY) movingUp = false; // go down
      } 
      else{
         pos.y += mGameState.flyer->getSpeed() * deltaTime;
         if (pos.y >= flyerBottomY) movingUp = true; // go up
      }

      mGameState.flyer->setPosition(pos);
   }

   // flyer collide with player on -> lose
   if (mGameState.flyer->isColliding(mGameState.zorp)){
      if (lives > 1){ // lose a life restart level
         lives--;
         initialise();
         return;
      } 
      else{
         mGameState.nextSceneID = 1; // TODO: UPDATE TO LOSE SCREEN
         return;
      }
   }

   mGameState.flyer2->update(
      deltaTime,      // delta time / fixed timestep
      nullptr,        // player
      mGameState.map, // map
      nullptr,        // collidable entities
      0               // col. entity count
   );

   if (mGameState.flyer2){ // FLYER2 MOVEMENTS
      static float startY = mGameState.flyer2->getPosition().y;

      // top and bottom limits
      static float flyer2TopY = startY - 180.0f;
      static float flyer2BottomY = startY + 180.0f;

      static bool movingUp = true; // current direction
      Vector2 pos = mGameState.flyer2->getPosition();

      if (movingUp){
         pos.y -= mGameState.flyer2->getSpeed() * deltaTime;
         if (pos.y <= flyer2TopY) movingUp = false; // go down
      } 
      else{
         pos.y += mGameState.flyer2->getSpeed() * deltaTime;
         if (pos.y >= flyer2BottomY) movingUp = true; // go up
      }

      mGameState.flyer2->setPosition(pos);
   }

   // flyer2 collide with player on -> lose
   if (mGameState.flyer2->isColliding(mGameState.zorp)){
      if (lives > 1){ // lose a life restart level
         lives--;
         initialise();
         return;
      } 
      else{
         mGameState.nextSceneID = 1; // TODO: UPDATE TO LOSE SCREEN
         return;
      }
   }

   Vector2 currentPlayerPosition = { mGameState.zorp->getPosition().x, mOrigin.y };

   // TODO: FIX WIN AND LOSE CONDITION FOR THIS
   if (mGameState.zorp->getPosition().y > END_GAME_THRESHOLD){ // falling off screen
      if (lives > 1){ // lose a life restart level
         lives--;
         initialise();
         return;
      } 
      else{
         mGameState.nextSceneID = 1; // TODO: UPDATE TO LOSE SCREEN
         return;
      }
   }

   // reaching the rocket to win
   if (!rocketReached && mGameState.zorp->isColliding(mGameState.rocket)){
      rocketReached = true; // trigger animation
      mGameState.zorp->deactivate(); // hide player
      mGameState.zorp->setColliderDimensions({0.0f,0.0f});
      PlaySound(mGameState.lvlCompleteSound);
   }

   if (rocketReached){// animate rocket moving up and switching
      Vector2 pos = mGameState.rocket->getPosition();
      pos.y -= 150.0f * deltaTime; // rocket speed
      mGameState.rocket->setPosition(pos);

      rocketMovTimer += deltaTime;
      if (rocketMovTimer >= 0.25f){
         rocketMovTimer = 0.0f;
         rocketCurrent = 1 - rocketCurrent;
      }

      // set texture based on the frame its in
      float heightFix = 0.0f;
      if (rocketCurrent == 0){
         mGameState.rocket->setScale({ TILE_DIMENSION * (28.0f/16.0f), TILE_DIMENSION * (39.0f/16.0f)});
         mGameState.rocket->setTexture(textureRocketMov1);
      }
      else{
         mGameState.rocket->setScale({ TILE_DIMENSION * (28.0f/16.0f), TILE_DIMENSION * (39.0f/16.0f)});
         mGameState.rocket->setTexture(textureRocketMov2);
      }

      // next lvl when out of frame
      if (pos.y + mGameState.rocket->getScale().y < -250.0f){
         mGameState.nextSceneID = 4; // TODO: UPDATE WIN SCREEN
      }
   }

   // for panning the camera to follow zorp
   Vector2 zorpPos = mGameState.zorp->getPosition();
   mGameState.camera.target = zorpPos;

}

void Level2::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));
   BeginMode2D(mGameState.camera);

   mGameState.bg->render();
   mGameState.map->render();
   mGameState.zorp->render();
   mGameState.zorp->displayCollider();
   mGameState.flyer->render();
   mGameState.flyer->displayCollider();
   mGameState.flyer2->render();
   mGameState.flyer2->displayCollider();
   mGameState.rocket->render();
   mGameState.rocket->displayCollider();
   for (Entity* block : mGameState.fallingBlocks){
      if (block && block->isActive()){
         block->render();
         block->displayCollider();
      }
   }
   EndMode2D();

   // rendering the hearts for lives
   const float padding = 20.0f;
   for (int i = 0; i < lives; i++){
      Rectangle textureArea = { 
         0, 0, 
         (float)textureHeart.width, (float)textureHeart.height 
      };
      Rectangle destinationArea = {
         padding + i * (TILE_DIMENSION + padding),
         padding,
         TILE_DIMENSION,
         TILE_DIMENSION
      };
      Vector2 origin = { 0.0f, 0.0f }; // to be top left

      DrawTexturePro(
         textureHeart,
         textureArea,
         destinationArea,
         origin,
         0.0f,
         WHITE
      );
   }

}

void Level2::shutdown()
{
   delete mGameState.bg;
   delete mGameState.map;
   delete mGameState.zorp;
   delete mGameState.flyer;
   delete mGameState.flyer2;
   // delete mGameState.rocket;
   UnloadTexture(textureRocketStation);
   UnloadTexture(textureRocketMov1);
   UnloadTexture(textureRocketMov2);

   for (Entity* block : mGameState.fallingBlocks){
      delete block;
   }
   mGameState.fallingBlocks.clear();
   mGameState.blockWillFall.clear();

   UnloadTexture(textureHeart);
   UnloadTexture(textureRocketStation);
   UnloadTexture(textureRocketMov1);
   UnloadTexture(textureRocketMov2);

   UnloadMusicStream(mGameState.bgm);
   UnloadSound(mGameState.lvlCompleteSound);
   UnloadSound(mGameState.jumpSound);
}