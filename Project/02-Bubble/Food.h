// Food.h
#ifndef _FOOD_INCLUDE
#define _FOOD_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"

class Food
{
public:
    Food();
    ~Food();

    void init(int id, int* playerPoints, glm::vec2 initPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();
    void setTileMap(TileMap* tileMap);
    bool isAlive();
private:
    void kill();


    enum BALL_STATUS {
        NORMAL, BLINKING
    };

private:
    int id, cyclesCounter;
    int* playerPoints;
    TileMap* map;
    Texture spritesheet;
    Sprite* sprite;
    glm::vec2 boxSize;
    glm::ivec2 pos;
    bool alive;
};

#endif // _FOOD_INCLUDE
