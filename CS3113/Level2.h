#include "Level1.h"

#ifndef LEVEL2_H
#define LEVEL2_H

class Level2 : public Scene {
private:
    static constexpr int LEVEL_WIDTH = 14,
                         LEVEL_HEIGHT = 12;

    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, // this will have 2-4 flyers that go up and down col 4&10
        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0,
        1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0,
        1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0,
        1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
        1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1
    };
    
    Texture2D textureZorp;
    Texture2D textureBG;
    Texture2D textureFlyer;
    Texture2D textureWanderer;
    Texture2D textureHeart;
    Texture2D textureRocketStation;
    Texture2D textureRocketMov1;
    Texture2D textureRocketMov2;

    bool rocketReached;
    float rocketMovTimer;
    int rocketCurrent;

public:
    static constexpr float TILE_DIMENSION       = 60.0f,
                        ACCELERATION_OF_GRAVITY = 981.0f,
                        END_GAME_THRESHOLD      = 990.0f;

    Level2();
    Level2(Vector2 origin, const char *bgHexCode);
    ~Level2();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif