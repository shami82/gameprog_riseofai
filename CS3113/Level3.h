#include "Level2.h"

#ifndef LEVEL3_H
#define LEVEL3_H

class Level3 : public Scene {
private:
    static constexpr int LEVEL_WIDTH = 14,
                         LEVEL_HEIGHT = 16;

    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = { // 14 x 14
        1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, // falling platform col 11, 8,9
        1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, // this will have 2-4 flyers that go up and down col 4&10
        1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, // falling platform col 9,10
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1,
        1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1
    };
    
    Texture2D textureZorp;
    Texture2D textureFallingBlock;
    Texture2D textureBG;
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

    Level3();
    Level3(Vector2 origin, const char *bgHexCode);
    ~Level3();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif