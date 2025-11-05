#include "Start.h"

Start::Start() : Scene({0.0f}, nullptr) {}
Start::Start(Vector2 origin, const char *bgHexCode) : Scene(origin, bgHexCode) {}
Start::~Start() { shutdown(); }

void Start::initialise()
{
    textureBG = LoadTexture("assets/startscreen.PNG");
    mGameState.nextSceneID = 0;

    mGameState.bg = new Entity(
        mOrigin,                                        // position
        { 990.0f, 720.0f },                             // size
        textureBG,                                      // texture file address
        NONE                                            // type
    );
}

void Start::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER)){ mGameState.nextSceneID = 1; }
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
}
