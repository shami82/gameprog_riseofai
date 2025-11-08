#include "Lose.h"

Lose::Lose() : Scene({0.0f}, nullptr) {}
Lose::Lose(Vector2 origin, const char *bgHexCode) : Scene(origin, bgHexCode) {}
Lose::~Lose() { shutdown(); }

void Lose::initialise()
{
    textureBG = LoadTexture("assets/losescreen.PNG");
    mGameState.nextSceneID = -1;
    resetLives(); // make the lives go back to 3 instead of 1/0

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

    mGameState.loseSound = LoadSound("assets/lose.wav");
    SetSoundVolume(mGameState.loseSound, 0.8f);
    PlaySound(mGameState.loseSound);

    mGameState.bg = new Entity(
        mOrigin,                                        // position
        { 990.0f, 720.0f },                             // size
        textureBG,                                      // texture file address
        NONE                                            // type
    );
}

void Lose::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER)){ mGameState.nextSceneID = 0; } // restart at home
}

void Lose::render()
{
    ClearBackground(WHITE);

    mGameState.bg->render();

    // instr to restart
    const char* msg = "Press [Enter] to Restart";
    int fontSize = 30;
    int textWidth = MeasureText(msg, fontSize);
    DrawText(msg,
             static_cast<int>(mOrigin.x - textWidth / 2),
             static_cast<int>(mOrigin.y + 85.0f),
             fontSize,
             WHITE);
}

void Lose::shutdown()
{
    delete mGameState.bg;

    UnloadSound(mGameState.loseSound);
}
