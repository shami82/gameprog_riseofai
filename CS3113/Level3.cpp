#include "Level3.h"

Level3::Level3()                                      : Scene { {0.0f}, nullptr   } {}
Level3::Level3(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

Level3::~Level3() { shutdown(); }

void Level3::initialise()
{
    mGameState.fallingBlocks.clear();
    mGameState.blockWillFall.clear();
    mGameState.blockFallTimers.clear();

    mGameState.nextSceneID = -1; // "don't switch scenes yet"-state

    mGameState.bgm = LoadMusicStream("assets/the_search.mp3");
    SetMusicVolume(mGameState.bgm, 0.7f);
    PlayMusicStream(mGameState.bgm);
    mGameState.lvlCompleteSound = LoadSound("assets/lvlcomplete.wav");
    SetSoundVolume(mGameState.lvlCompleteSound, 1.2f);
    mGameState.jumpSound = LoadSound("assets/jump.wav");
    SetSoundVolume(mGameState.jumpSound, 0.08f);

    textureZorp = LoadTexture("assets/zorpsheet.PNG");
    textureFallingBlock = LoadTexture("assets/plan3fall.png");
    textureBG = LoadTexture("assets/spacebg.PNG");
    textureFlyer = LoadTexture("assets/flyer.PNG");
    textureHeart = LoadTexture("assets/heart.PNG");
    textureRocketStation = LoadTexture("assets/rocketstation.PNG");
    textureRocketMov1 = LoadTexture("assets/rocketmov1.PNG");
    textureRocketMov2 = LoadTexture("assets/rocketmov2.PNG");

    /*
        ----------- BACKGROUND -----------
    */
    mGameState.bg = new Entity(
        mOrigin,                                        // position
        { 990.0f, 720.0f },                             // size
        textureBG,                                      // texture file address
        NONE                                            // type
    );

    /*
        ----------- MAP -----------
    */
    mGameState.map = new Map(
        LEVEL_WIDTH, LEVEL_HEIGHT,   // map grid cols & rows
        (unsigned int *) mLevelData, // grid data
        "assets/plan3reg.PNG",       // texture filepath
        TILE_DIMENSION,              // tile size
        1, 1,                        // texture cols & rows
        mOrigin                      // in-game origin
    );

    /*
        ----------- ZORP :D -----------
    */
    std::map<Direction, std::vector<int>> zorpAnimationAtlas = {
        {UP,    { 0,  1,  2,  3 }}, // idle
        {RIGHT, { 4,  5,  6,  7 }},
        {LEFT,  { 8,  9, 10, 11 }},
        {DOWN,  { 0,  1,  2,  3 }} // idle
    };

    Vector2 zorpScale = { TILE_DIMENSION, TILE_DIMENSION * (17.0f / 16.0f) };

    mGameState.zorp = new Entity(
        {TILE_DIMENSION * 2.75f, TILE_DIMENSION * 10.0f}, // col, row
        zorpScale,                                // scale
        textureZorp,                              // texture file address
        ATLAS,                                    // single image or atlas?
        { 3, 4 },                                 // atlas dimensions
        zorpAnimationAtlas,                       // actual atlas
        PLAYER                                    // entity type
    );

    mGameState.zorp->setJumpingPower(500.0f);
    mGameState.zorp->setColliderDimensions({
        mGameState.zorp->getScale().x * 0.65f,
        mGameState.zorp->getScale().y
    });
    mGameState.zorp->setSpeed(165);
    mGameState.zorp->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    mGameState.zorp->setDirection(UP);

    /*
        ----------- FLYER -----------
    */

    mGameState.flyer = new Entity(
        {TILE_DIMENSION * 11.75f, TILE_DIMENSION * 10.0f}, // starting position
        { TILE_DIMENSION, TILE_DIMENSION },    // scale
        textureFlyer,                          // texture
        NONE                                    // entity type
    );

    mGameState.flyer->setSpeed(200);
    mGameState.flyer->setAcceleration({0.0f, 0.0f});
    mGameState.flyer->setDirection(UP);

    /*
    ----------- FALLING BLOCKS -----------
    */
    for (int col = 9; col <= 10; col++){ // new entity per block
        Vector2 blockpos = { TILE_DIMENSION * ((float)col + 0.75f), TILE_DIMENSION * 7.5f }; // row 9, cols 9-10

        Entity* block = new Entity(
            blockpos,                              // starting position
            { TILE_DIMENSION, TILE_DIMENSION },    // scale
            textureFallingBlock,                   // texture
            BLOCK                                  //entity type
        );

        block->setAcceleration({0.0f, 0.0f}); // start stationary
        block->setSpeed(0);
        block->setColliderDimensions({
            TILE_DIMENSION,
            TILE_DIMENSION * 5.5f // detecting 4 blocks below
        });

        mGameState.fallingBlocks.push_back(block);
        mGameState.blockWillFall.push_back(false);
        mGameState.blockFallTimers.push_back(0.0f);
    }

    for (int col = 8; col <= 11; col++){ // new entity per block (SKIP COL 10)
        if (col == 10) continue; // skipping col 10
        Vector2 blockpos = {0.0f, 0.0f};
        if (col == 11)
            blockpos = { TILE_DIMENSION * ((float)col + 0.75f), TILE_DIMENSION * 1.5f }; // row 2, cols 8,9,11
        else
            blockpos = { TILE_DIMENSION * ((float)col + 0.75f), TILE_DIMENSION * 0.5f };
        Entity* block = new Entity(
            blockpos,                              // starting position
            { TILE_DIMENSION, TILE_DIMENSION },    // scale
            textureFallingBlock,                   // texture
            BLOCK                                  //entity type
        );

        block->setAcceleration({0.0f, 0.0f}); // start stationary
        block->setSpeed(0);
        block->setColliderDimensions({
            TILE_DIMENSION,
            TILE_DIMENSION * 5.5f // detecting 4 blocks below
        });

        mGameState.fallingBlocks.push_back(block);
        mGameState.blockWillFall.push_back(false);
        mGameState.blockFallTimers.push_back(0.0f);
    }

    /*
        ----------- ROCKET (THE GOAL) -----------
    */

    mGameState.rocket = new Entity(
        {TILE_DIMENSION * 3.5f, TILE_DIMENSION * -1.0f}, // starting position
        { TILE_DIMENSION * (28.0f/16.0f), TILE_DIMENSION * (32.0f/16.0f)},    // scale
        textureRocketStation,                          // texture
        NONE                                    // entity type
    );

    // init for animating later
    rocketReached = false;
    rocketMovTimer = 0.0f;
    rocketCurrent = 0;

    /*
        ----------- CAMERA -----------
    */
    mGameState.camera = { 0 };                                    // zero initialize
    mGameState.camera.target = mGameState.zorp->getPosition();    // camera follows player
    mGameState.camera.offset = {mOrigin.x, mOrigin.y + 100};      // camera offset to center of screen
    mGameState.camera.rotation = 0.0f;                            // no rotation
    mGameState.camera.zoom = 1.25f;                               // zoom more
}

void Level3::update(float deltaTime)
{
    if (IsKeyPressed(KEY_N)){ // for skipping level
        mGameState.nextSceneID = 5;
        return;
    }
    UpdateMusicStream(mGameState.bgm);

    mGameState.flyer->update(
        deltaTime,      // delta time / fixed timestep
        nullptr,        // player
        mGameState.map, // map
        nullptr,        // collidable entities
        0               // col. entity count
    );

    if (mGameState.flyer){ // FLYER MOVEMENTS
        static float startY = mGameState.flyer->getPosition().y;

        // top and bottom limits
        static float flyerTopY = startY - 150.0f;
        static float flyerBottomY = startY + 150.0f;

        static bool movingUp = true; // current direction
        Vector2 pos = mGameState.flyer->getPosition();

        if (movingUp){
            pos.y -= mGameState.flyer->getSpeed() * deltaTime;
            if (pos.y <= flyerTopY) movingUp = false; // go down
        } 
        else{
            pos.y += mGameState.flyer->getSpeed() * deltaTime;
            if (pos.y >= flyerBottomY) movingUp = true; // go up
        }

        mGameState.flyer->setPosition(pos);
    }

    // flyer collide with player on -> lose
    if (mGameState.flyer->isColliding(mGameState.zorp)){
        if (lives > 1){ // lose a life restart level
            lives--;
            initialise();
            return;
        } 
        else{
            mGameState.nextSceneID = 1;
            return;
        }
    }

    for (size_t i = 0; i < mGameState.fallingBlocks.size(); i++){ // falling blocks movement
        Entity* block = mGameState.fallingBlocks[i];
        if (!block->isActive()) continue; // skip inactive blocks

        // falls if player is within 4 tiles under it
        if (!mGameState.blockWillFall[i] && block->isColliding(mGameState.zorp)){
            mGameState.blockWillFall[i] = true;
            mGameState.blockFallTimers[i] = 0.0f;
        }
        if (mGameState.blockWillFall[i]){
            mGameState.blockFallTimers[i] += deltaTime;

            if(mGameState.blockFallTimers[i] >= 0.5f && block->getAcceleration().y == 0.0f){
                // reset collider to normal block size for second time collisions
                block->setColliderDimensions({ TILE_DIMENSION, TILE_DIMENSION });
                // update to let it fall now
                block->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY - 450.0f });
            }
        }

        block->update(deltaTime, nullptr, mGameState.map, nullptr, 0);

        if (block->isCollidingBottom()){ // deactivate the block once its hit the bottom (platform)
            block->setAcceleration({0.0f, 0.0f});
            block->setVelocity({0.0f, 0.0f});
        }
    }

    std::vector<Entity*> activeBlocks;
    for (Entity* block : mGameState.fallingBlocks){
        if (block && block->isActive()) activeBlocks.push_back(block);
    }
    mGameState.zorp->update(deltaTime, nullptr, mGameState.map, activeBlocks);

    // block collide with player on -> lose
    for (Entity* block : activeBlocks){
        if (block->isColliding(mGameState.zorp) && block->getVelocity().y > 0){
            if (lives > 1){ // lose a life restart level
                lives--;
                initialise();
                return;
            } 
            else{
                mGameState.nextSceneID = 1;
                return;
            }
        }
    }

    Vector2 currentPlayerPosition = { mGameState.zorp->getPosition().x, mOrigin.y };

    if (mGameState.zorp->getPosition().y > END_GAME_THRESHOLD){ // falling off screen
        if (lives > 1){ // lose a life restart level
            lives--;
            initialise();
            return;
        } 
        else{
            mGameState.nextSceneID = 1;
            return;
        }
    }

    // reaching the rocket to win
    if (!rocketReached && mGameState.zorp->isColliding(mGameState.rocket)){
        rocketReached = true; // trigger animation
        mGameState.zorp->deactivate(); // hide player
        mGameState.zorp->setColliderDimensions({0.0f,0.0f});
        PlaySound(mGameState.lvlCompleteSound);
    }

    if (rocketReached){// animate rocket moving up and switching
        Vector2 pos = mGameState.rocket->getPosition();
        pos.y -= 150.0f * deltaTime; // rocket speed
        mGameState.rocket->setPosition(pos);

        rocketMovTimer += deltaTime;
        if (rocketMovTimer >= 0.25f){
            rocketMovTimer = 0.0f;
            rocketCurrent = 1 - rocketCurrent;
        }

        // set texture based on the frame its in
        float heightFix = 0.0f;
        if (rocketCurrent == 0){
            mGameState.rocket->setScale({ TILE_DIMENSION * (28.0f/16.0f), TILE_DIMENSION * (39.0f/16.0f)});
            mGameState.rocket->setTexture(textureRocketMov1);
        }
        else{
            mGameState.rocket->setScale({ TILE_DIMENSION * (28.0f/16.0f), TILE_DIMENSION * (39.0f/16.0f)});
            mGameState.rocket->setTexture(textureRocketMov2);
        }

        // next lvl when out of frame
        if (pos.y + mGameState.rocket->getScale().y < -400.0f){
            mGameState.nextSceneID = 5;
        }
    }

    // for panning the camera to follow zorp
    Vector2 zorpPos = mGameState.zorp->getPosition();
    mGameState.camera.target = zorpPos;

}

void Level3::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    BeginMode2D(mGameState.camera);

    mGameState.bg->render();
    mGameState.map->render();
    mGameState.zorp->render();
    // mGameState.zorp->displayCollider();
    mGameState.flyer->render();
    // mGameState.flyer->displayCollider();
    mGameState.rocket->render();
    // mGameState.rocket->displayCollider();
    for (Entity* block : mGameState.fallingBlocks){
        if (block && block->isActive()){
            block->render();
            // block->displayCollider();
        }
    }
    EndMode2D();

    // rendering the hearts for lives
    const float padding = 20.0f;
    for (int i = 0; i < lives; i++){
        Rectangle textureArea = { 
            0, 0, 
            (float)textureHeart.width, (float)textureHeart.height 
        };
        Rectangle destinationArea = {
            padding + i * (TILE_DIMENSION + padding),
            padding,
            TILE_DIMENSION,
            TILE_DIMENSION
        };
        Vector2 origin = { 0.0f, 0.0f }; // to be top left

        DrawTexturePro(
            textureHeart,
            textureArea,
            destinationArea,
            origin,
            0.0f,
            WHITE
        );
    }

}

void Level3::shutdown()
{
    delete mGameState.bg;
    delete mGameState.map;
    delete mGameState.zorp;
    delete mGameState.flyer;
    // delete mGameState.rocket;
    UnloadTexture(textureRocketStation);
    UnloadTexture(textureRocketMov1);
    UnloadTexture(textureRocketMov2);

    for (Entity* block : mGameState.fallingBlocks){
        delete block;
    }
    mGameState.fallingBlocks.clear();
    mGameState.blockWillFall.clear();
    mGameState.blockFallTimers.clear();

    UnloadTexture(textureHeart);

    UnloadMusicStream(mGameState.bgm);
    UnloadSound(mGameState.lvlCompleteSound);
    UnloadSound(mGameState.jumpSound);
}