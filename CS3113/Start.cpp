#include "Start.h"

Start::Start() : Scene({0.0f}, nullptr) {}
Start::Start(Vector2 origin, const char *bgHexCode) : Scene(origin, bgHexCode) {}
Start::~Start() { shutdown(); }

void Start::initialise()
{
    textureBG = LoadTexture("assets/startscreen.PNG");
    mGameState.nextSceneID = -1;

    textureZorp = LoadTexture("assets/zorpsheet.PNG");
    /*
      ----------- ZORP :D -----------
   */
   std::map<Direction, std::vector<int>> zorpAnimationAtlas = {
      {UP,    { 0,  1,  2,  3 }}, // idle
      {RIGHT, { 4,  5,  6,  7 }},
      {LEFT,  { 8,  9, 10, 11 }},
      {DOWN,  { 0,  1,  2,  3 }} // idle
   };

   mGameState.zorp = new Entity( // dummy info for initialization
      {0,0},
      {0,0},                                // scale
      textureZorp,                              // texture file address
      ATLAS,                                    // single image or atlas?
      { 3, 4 },                                 // atlas dimensions
      zorpAnimationAtlas,                       // actual atlas
      PLAYER                                    // entity type
   );

    mGameState.bgm = LoadMusicStream("assets/void.mp3");
    SetMusicVolume(mGameState.bgm, 0.50f);
    PlayMusicStream(mGameState.bgm);

    mGameState.bg = new Entity(
        mOrigin,                                        // position
        { 990.0f, 720.0f },                             // size
        textureBG,                                      // texture file address
        NONE                                            // type
    );
}

void Start::update(float deltaTime)
{
    UpdateMusicStream(mGameState.bgm);
    if (IsKeyPressed(KEY_ENTER)){ mGameState.nextSceneID = 2; } // go to lvl 1
}

void Start::render()
{
    ClearBackground(WHITE);

    mGameState.bg->render();

    // instr to start
    const char* msg = "Press [Enter] to Start";
    int fontSize = 30;
    int textWidth = MeasureText(msg, fontSize);
    DrawText(msg,
             static_cast<int>(mOrigin.x - textWidth / 2),
             static_cast<int>(mOrigin.y + 85.0f),
             fontSize,
             WHITE);
}

void Start::shutdown()
{
    delete mGameState.bg;
    delete mGameState.zorp;

    UnloadMusicStream(mGameState.bgm);
}
