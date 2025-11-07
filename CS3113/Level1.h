#include "Start.h"

#ifndef LEVEL1_H
#define LEVEL1_H

constexpr int LEVEL_WIDTH = 14,
            //   LEVEL_HEIGHT = 14;
              LEVEL_HEIGHT = 12;

class Level1 : public Scene {
private:
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

    // unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
    //     1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
    //     1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
    //     1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, // this will have 2-4 flyers that go up and down col 4&10
    //     1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
    //     1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
    //     1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
    //     1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1,
    //     1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1,
    //     1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1,
    //     1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1,
    //     1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1,
    //     1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1
    // };

    // unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = { // 14 x 14
    //     1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
    //     1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    //     1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, // falling platform col 11, 8,9
    //     1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
    //     1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, // this will have 2-4 flyers that go up and down col 4&10
    //     1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1,
    //     1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1,
    //     1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1,
    //     1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, // falling platform col 9,10
    //     1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
    //     1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
    //     1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1,
    //     1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1,
    //     1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1
    // };
    
    Texture2D textureZorp;
    Texture2D textureFallingBlock;
    Texture2D textureBG;
    Texture2D textureFlyer;
    Texture2D textureWanderer;
    Texture2D textureHeart;

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