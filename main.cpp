#include "CS3113/End.h"

// Global Constants
constexpr int SCREEN_WIDTH     = 990, //990
              SCREEN_HEIGHT    = 720, //720
              FPS              = 120,
              NUMBER_OF_LEVELS = 6; //6

constexpr Vector2 ORIGIN       = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
            
constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus   = RUNNING;
float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f;

Scene *gCurrentScene = nullptr;
std::vector<Scene*> gLevels = {};

Start *gStart   = nullptr;
Lose *gLose     = nullptr;
Level1 *gLevel1 = nullptr;
Level2 *gLevel2 = nullptr;
Level3 *gLevel3 = nullptr;
End *gEnd       = nullptr;


// Function Declarations
void switchToScene(Scene *scene);
void initialise();
void processInput();
void update();
void render();
void shutdown();

void switchToScene(Scene *scene)
{   
    gCurrentScene = scene;
    gCurrentScene->initialise();
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Meet Me There");
    InitAudioDevice();

    gStart = new Start(ORIGIN, "#000000");
    gLose = new Lose(ORIGIN, "#000000");
    gLevel1 = new Level1(ORIGIN, "#0D171f");
    gLevel2 = new Level2(ORIGIN, "#0D171f");
    gLevel3 = new Level3(ORIGIN, "#0D171f");
    gEnd = new End(ORIGIN, "#0D171f");

    gLevels.push_back(gStart);
    gLevels.push_back(gLose);
    gLevels.push_back(gLevel1);
    gLevels.push_back(gLevel2);
    gLevels.push_back(gLevel3);
    gLevels.push_back(gEnd);

    switchToScene(gLevels[0]); // SWITCH AROUND TO FOCUS ON LEVELS (SHOULD BE gLevels[0])

    SetTargetFPS(FPS);
}

void processInput() 
{
    gCurrentScene->getState().zorp->resetMovement();

    if      (IsKeyDown(KEY_A)) gCurrentScene->getState().zorp->moveLeft();
    else if (IsKeyDown(KEY_D)) gCurrentScene->getState().zorp->moveRight();

    if (IsKeyPressed(KEY_W) && 
        gCurrentScene->getState().zorp->isCollidingBottom())
    {
        gCurrentScene->getState().zorp->jump();
        // PlaySound(gCurrentScene->getState().jumpSound);
    }

    if (GetLength(gCurrentScene->getState().zorp->getMovement()) > 1.0f) 
        gCurrentScene->getState().zorp->normaliseMovement();

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()){
        gAppStatus = TERMINATED;
    }

}

void update() 
{
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }
}

void render()
{
    BeginDrawing();

    // only use camera when level, not startscreen
    if (gCurrentScene->getState().camera.target.x != 0 || gCurrentScene->getState().camera.target.y != 0){
        BeginMode2D(gCurrentScene->getState().camera);
        gCurrentScene->render();
        EndMode2D();
    } 
    else{
        gCurrentScene->render();
    }

    // gCurrentScene->render();

    EndDrawing();
}

void shutdown() 
{
    delete gStart;
    delete gLose;
    delete gLevel1;
    delete gLevel2;
    delete gLevel3;
    delete gEnd;

    for (int i = 0; i < NUMBER_OF_LEVELS; i++) gLevels[i] = nullptr;

    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();

        if (gCurrentScene->getState().nextSceneID > 0)
        {
            int id = gCurrentScene->getState().nextSceneID;
            switchToScene(gLevels[id]);
        }

        render();
    }

    shutdown();

    return 0;
}