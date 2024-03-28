#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Harpoon.h"

class Ball
{
public:
    enum BALL_COLOR {
        RED, BLUE, GREEN
    };
    enum BALL_SIZE {
        HUGE, BIG, MEDIUM, SMALL, NONE
    };

    enum BALL_STATUS {
        NORMAL, EXPLODING
    };

    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, BALL_COLOR color, BALL_SIZE size);
    void update(int deltaTime);
    void render();

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    glm::vec2 getPosition();
    bool isHitByHarpoon(Harpoon& harpoon);
    glm::vec2 getSize();

    BALL_COLOR getColor();
    BALL_SIZE getType();
    void setDirection(int dir);
    int getDirection();

    bool getStatus();
    void kill();
    void addForce(int force);

private:
    BALL_COLOR color;
    BALL_SIZE size;
    glm::vec2 boxSize;

    Texture spritesheet;
    Sprite* sprite;

    TileMap* map;

    glm::ivec2 tileMapDispl, posBall;

    int posUp;
    int velocityX = 1;
    int gravityCounter = 0;
    int gravityFactor = 9;
    int gravity = 1;

    int maxGravity = 2;

    int velocityY = 0;
    int energy = 0;
    int tmpSpeed;


    int deadCounter = 0;
    bool status;
};

#endif // _BALL_INCLUDE
