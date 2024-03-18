#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Ball
{
public:
    enum BALL_COLOR {
        RED, BLUE, GREEN
    };
    enum BALL_SIZE {
        HUGE, BIG, MEDIUM, SMALL
    };

    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, BALL_COLOR color, BALL_SIZE size);
    void update(int deltaTime);
    void render();

private:
    BALL_COLOR color;
    BALL_SIZE size;

    Texture spritesheet;
    Sprite* sprite;
};

#endif // _BALL_INCLUDE
