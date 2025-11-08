#include "End.h"

End::End() : Scene({0.0f}, nullptr) {}
End::End(Vector2 origin, const char *bgHexCode) : Scene(origin, bgHexCode) {}
End::~End() { shutdown(); }

void End::initialise()
{
    mGameState.nextSceneID = 0;

    mGameState.bgm = LoadMusicStream("assets/win.wav");
    SetMusicVolume(mGameState.bgm, 0.50f);
    PlayMusicStream(mGameState.bgm);

    textureBG = LoadTexture("assets/endscreen.PNG");
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

    mGameState.bg = new Entity(
        mOrigin,                                        // position
        { 990.0f, 720.0f },                             // size
        textureBG,                                      // texture file address
        NONE                                            // type
    );
}

void End::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER)){ mGameState.nextSceneID = 2; } // go to lvl 1
}

void End::render()
{
    ClearBackground(WHITE);

    mGameState.bg->render();

    // // instr to start
    // const char* msg = "Press [Enter] to Start";
    // int fontSize = 30;
    // int textWidth = MeasureText(msg, fontSize);
    // DrawText(msg,
    //          static_cast<int>(mOrigin.x - textWidth / 2),
    //          static_cast<int>(mOrigin.y + 85.0f),
    //          fontSize,
    //          WHITE);
}

void End::shutdown()
{
    delete mGameState.bg;
    delete mGameState.zorp;

    UnloadMusicStream(mGameState.bgm);
}
