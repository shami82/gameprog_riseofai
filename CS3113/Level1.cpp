#include "Level1.h"

Level1::Level1()                                      : Scene { {0.0f}, nullptr   } {}
Level1::Level1(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

Level1::~Level1() { shutdown(); }

Texture2D textureZorp;
// Texture2D textureMap;
Texture2D textureFallingBlock;

void Level1::initialise()
{
   mGameState.nextSceneID = 1;

   // mGameState.bgm = LoadMusicStream("assets/game/04 - Silent Forest.wav");
   // SetMusicVolume(mGameState.bgm, 0.33f);
   // PlayMusicStream(gState.bgm);

   // mGameState.jumpSound = LoadSound("assets/game/Dirt Jump.wav");

   textureZorp = LoadTexture("assets/zorpsheet.PNG");
   // Texture2D textureMap = LoadTexture("assets/plan1reg.PNG");
   textureFallingBlock = LoadTexture("assets/plan1fall.png");

   /*
      ----------- MAP -----------
   */
   mGameState.map = new Map(
      LEVEL_WIDTH, LEVEL_HEIGHT,   // map grid cols & rows
      (unsigned int *) mLevelData, // grid data
      "assets/plan1reg.PNG",       // texture filepath
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

   Vector2 mapBottomLeft = {
      mOrigin.x - (LEVEL_WIDTH * TILE_DIMENSION) / 2,
      mOrigin.y - (LEVEL_HEIGHT * TILE_DIMENSION) / 2
   };

   int spawnRow = 11; // 11th row
   int spawnCol = 2;  // 2nd column

   float map_startx = mOrigin.x - (LEVEL_WIDTH * TILE_DIMENSION) / 2.0f;
   float map_starty = mOrigin.y - (LEVEL_HEIGHT * TILE_DIMENSION) / 2.0f;

   Vector2 zorpPos = {
      map_startx + (spawnCol * TILE_DIMENSION) + (TILE_DIMENSION / 2.0f),
      map_starty + (spawnRow * TILE_DIMENSION) + (TILE_DIMENSION / 2.0f)
   };

   Vector2 zorpScale = { TILE_DIMENSION, TILE_DIMENSION * (17.0f / 16.0f) };

   mGameState.zorp = new Entity(
      {mOrigin.x, mOrigin.y - 80.0f},           // position
      zorpScale,                                // scale
      textureZorp,                              // texture file address
      ATLAS,                                    // single image or atlas?
      { 3, 4 },                                 // atlas dimensions
      zorpAnimationAtlas,                       // actual atlas
      PLAYER                                    // entity type
   );

   mGameState.zorp->setJumpingPower(550.0f);
   mGameState.zorp->setColliderDimensions({
      zorpScale.x * 0.9f,
      zorpScale.y * 0.9f
   });
   mGameState.zorp->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

   //DEBUGGING
   DrawRectangle(zorpPos.x - 5, zorpPos.y - 5, 10, 10, RED);

   /*
      ----------- FALLING BLOCK -----------
   */
   for (int i = 0; i < 4; i++){
      Vector2 blockPos = {
         mOrigin.x - 60.0f + (6 + i) * TILE_DIMENSION,
         mOrigin.y - 260.0f
      };

      Entity* fallingblock = new Entity(
         blockPos,                           // position
         { TILE_DIMENSION, TILE_DIMENSION },    // scale
         textureFallingBlock,             // texture file address
         BLOCK                                  // entity type
      );

      fallingblock->setAcceleration({0.0f, 0.0f});
      fallingblock->activate();

      mGameState.enemies.push_back(fallingblock); // added to gamestate
   }

   /*
      ----------- CAMERA -----------
   */
   // mGameState.camera = { 0 };                                    // zero initialize
   // mGameState.camera.target = mGameState.zorp->getPosition();    // camera follows player
   // mGameState.camera.offset = mOrigin;                           // camera offset to center of screen
   // mGameState.camera.rotation = 0.0f;                            // no rotation
   // mGameState.camera.zoom = 1.0f;                                // default zoom
}

void Level1::update(float deltaTime)
{
   UpdateMusicStream(mGameState.bgm);

   mGameState.zorp->update(
      deltaTime,      // delta time / fixed timestep
      nullptr,        // player
      mGameState.map, // map
      nullptr,        // collidable entities
      0               // col. entity count
   );

   Vector2 currentPlayerPosition = { mGameState.zorp->getPosition().x, mOrigin.y };

   // TODO: FIX WIN AND LOSE CONDITION FOR THIS
   if (mGameState.zorp->getPosition().y > 800.0f) mGameState.nextSceneID = 1;

   // panCamera(&mGameState.camera, &currentPlayerPosition);

   if (mGameState.zorp->getPosition().y > END_GAME_THRESHOLD){ // LOSE CONDITION
      mGameState.nextSceneID = 1; // CHANGE THIS TO THE LOSE SCREEN
   }

   for (Entity* block : mGameState.enemies){ // logic for falling blocks
      if (!block->isActive()) continue;

      Vector2 blockPos = block->getPosition();
      Vector2 blockVel = block->getVelocity();
      Vector2 blockAcc = block->getAcceleration();

      // fall if zorp is below it
      float blockLeft = blockPos.x - block->getScale().x / 2;
      float blockRight = blockPos.x + block->getScale().x / 2;
      float playerLeft = currentPlayerPosition.x - mGameState.zorp->getScale().x / 2;
      float playerRight = currentPlayerPosition.x + mGameState.zorp->getScale().x / 2;

      bool playerUnderBlock = (playerRight > blockLeft) && (playerLeft < blockRight) && (currentPlayerPosition.y > blockPos.y);

      if (playerUnderBlock && blockAcc.y == 0.0f){ // block start falling
         block->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY - 200.0f});
      }

      // udpate the velocity and position
      blockPos.x += blockVel.x * deltaTime;
      blockPos.y += blockVel.y * deltaTime;
      block->setPosition(blockPos);

      if (block->isCollidingBottom()){ // collision with map/platform -> stop falling
         if (block->checkCollisionWithMap(mGameState.map)){
            block->setVelocity({0.0f, 0.0f});
         }
         // TODO: fix the lose condition
         else if (block->isColliding(mGameState.zorp)){ // LOSE CONDITION
            mGameState.nextSceneID = 1;
         }
      }
   }
}

void Level1::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));

   mGameState.map->render();
   mGameState.zorp->render();

}

void Level1::shutdown()
{
   delete mGameState.zorp;
   delete mGameState.map;
   clearEnemies();

   // UnloadMusicStream(mGameState.bgm);
   // UnloadSound(mGameState.jumpSound);
}