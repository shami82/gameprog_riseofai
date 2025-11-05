#include "Level1.h"

Level1::Level1()                                      : Scene { {0.0f}, nullptr   } {}
Level1::Level1(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

Level1::~Level1() { shutdown(); }

void Level1::initialise()
{
   mGameState.nextSceneID = 1;

   // mGameState.bgm = LoadMusicStream("assets/game/04 - Silent Forest.wav");
   // SetMusicVolume(mGameState.bgm, 0.33f);
   // PlayMusicStream(gState.bgm);

   // mGameState.jumpSound = LoadSound("assets/game/Dirt Jump.wav");

   textureZorp = LoadTexture("assets/zorpsheet.PNG");
   textureFallingBlock = LoadTexture("assets/plan1fall.png");
   textureBG = LoadTexture("assets/spacebg.PNG");

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

   // Vector2 zorpPos = {
   //    mOrigin.x + (2 * TILE_DIMENSION) + (TILE_DIMENSION / 2.0f),
   //    mOrigin.y + (11 * TILE_DIMENSION) + (TILE_DIMENSION / 2.0f)
   // };

   Vector2 zorpScale = { TILE_DIMENSION, TILE_DIMENSION * (17.0f / 16.0f) };

   mGameState.zorp = new Entity(
      {mOrigin.x - 300.0f, mOrigin.y - 300.0f}, // position
      // {mOrigin.x + TILE_DIMENSION / 2.0f - 2 * TILE_DIMENSION, mOrigin.y + TILE_DIMENSION - 5 * TILE_DIMENSION},
      zorpScale,                                // scale
      textureZorp,                              // texture file address
      ATLAS,                                    // single image or atlas?
      { 3, 4 },                                 // atlas dimensions
      zorpAnimationAtlas,                       // actual atlas
      PLAYER                                    // entity type
   );

   mGameState.zorp->setJumpingPower(550.0f);
   mGameState.zorp->setColliderDimensions({
      mGameState.zorp->getScale().x / 1.5f,
      mGameState.zorp->getScale().y / 1.1f
   });
   mGameState.zorp->setSpeed(200);
   mGameState.zorp->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

   /*
      ----------- CAMERA -----------
   */
   mGameState.camera = { 0 };                                    // zero initialize
   mGameState.camera.target = mGameState.zorp->getPosition();    // camera follows player
   mGameState.camera.offset = mOrigin;                           // camera offset to center of screen
   mGameState.camera.rotation = 0.0f;                            // no rotation
   mGameState.camera.zoom = 1.5f;                                // zoom more
}

void Level1::update(float deltaTime)
{
   // UpdateMusicStream(mGameState.bgm);

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

   panCamera(&mGameState.camera, &currentPlayerPosition);

   if (mGameState.zorp->getPosition().y > END_GAME_THRESHOLD){ // LOSE CONDITION
      mGameState.nextSceneID = 1; // CHANGE THIS TO THE LOSE SCREEN
   }

}

void Level1::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));

   mGameState.bg->render();
   mGameState.zorp->render();
   mGameState.zorp->displayCollider();
   mGameState.map->render();

}

void Level1::shutdown()
{
   delete mGameState.bg;
   delete mGameState.zorp;
   delete mGameState.map;

   // UnloadMusicStream(mGameState.bgm);
   // UnloadSound(mGameState.jumpSound);
}