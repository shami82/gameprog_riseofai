#include "Entity.h"
#include "Map.h"

#ifndef SCENE_H
#define SCENE_H

struct GameState
{
    Entity *zorp;
    Entity *bg;
    Entity *flyer;
    Map *map;

    Music bgm;
    Sound jumpSound;

    Camera2D camera;

    int nextSceneID;
};

// struct GameAudio{ // for music and sfx
//     Music bgm;
//     Sound winSound;
//     Sound loseSound;
// };
// GameAudio gState;

class Scene 
{
protected: // similar to private but children and friend classes can use them
    GameState mGameState;
    Vector2 mOrigin;
    const char *mBGColourHexCode = "#000000";
    
public:
    Scene();
    Scene(Vector2 origin, const char *bgHexCode); // if you want assets instead, change from hex to assets for bg

    virtual void initialise() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;
    
    GameState   getState()           const { return mGameState; }
    Vector2     getOrigin()          const { return mOrigin;    }
    const char* getBGColourHexCode() const { return mBGColourHexCode; }

};

#endif