#include "Entity.h"
#include "Map.h"

#ifndef SCENE_H
#define SCENE_H

struct GameState
{
    Entity *zorp;
    Entity *bg;
    Entity *flyer;
    Entity *flyer2;
    Entity *wanderer;
    Entity *rocket;
    Map *map;
    std::vector<Entity*> fallingBlocks = {};
    std::vector<bool> blockWillFall = {};
    std::vector<float> blockFallTimers = {};

    Music bgm;
    Sound jumpSound;

    Camera2D camera;

    int nextSceneID;
};

class Scene 
{
protected: // similar to private but children and friend classes can use them
    GameState mGameState;
    Vector2 mOrigin;
    const char *mBGColourHexCode = "#000000";
    static int lives;
    
public:
    Scene();
    Scene(Vector2 origin, const char *bgHexCode); // if you want assets instead, change from hex to assets for bg

    virtual void initialise() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;

    static int getLives()                  { return lives; }
    static void setLives(int val)          { lives = val; }
    static void resetLives()               { lives = 3; }
    
    GameState   getState()           const { return mGameState; }
    Vector2     getOrigin()          const { return mOrigin;    }
    const char* getBGColourHexCode() const { return mBGColourHexCode; }

};

#endif