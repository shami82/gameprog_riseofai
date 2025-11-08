#include "Lose.h"

#ifndef LEVEL1_H
#define LEVEL1_H

class Level1 : public Scene {
private:
    static constexpr int LEVEL_WIDTH = 14,
                         LEVEL_HEIGHT = 12;

    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // this row has the falling platform blocks (col 6-9)
        1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
        1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
        1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
        1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1,
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1
    };
    
    Texture2D textureZorp;
    Texture2D textureFallingBlock;
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

    Level1();
    Level1(Vector2 origin, const char *bgHexCode);
    ~Level1();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif