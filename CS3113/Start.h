#include "Scene.h"

#ifndef START_H
#define START_H

class Start : public Scene {
private:
    Texture2D textureBG;

public:

    Start();
    Start(Vector2 origin, const char *bgHexCode);
    ~Start();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif