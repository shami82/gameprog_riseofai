#include "Level3.h"

#ifndef END_H
#define END_H

class End : public Scene {
private:
    Texture2D textureBG;
    Texture2D textureZorp;

public:

    End();
    End(Vector2 origin, const char *bgHexCode);
    ~End();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif