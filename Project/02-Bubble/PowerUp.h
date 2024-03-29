// Food.h
#ifndef _POWERUP_INCLUDE
#define _POWERUP_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"


class PowerUp
{
private:
    enum POWERUP_TYPE {
        DYNAMITE, STOP, INVENCIBLE
    };

public:
    PowerUp();
    ~PowerUp();

    void init(int id, glm::vec2 initPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();
    void setTileMap(TileMap* tileMap);
    bool isAlive();
    int checkCollider(glm::vec2 playerPos, glm::vec2 playerSize);

    bool isTypeInvencible() const;
    bool isTypeStop() const;
    bool isTypeDynamite() const;
    void kill();

private:
    int id, cyclesCounter;
    TileMap* map;
    Texture spritesheet;
    Sprite* sprite;
    glm::vec2 boxSize;
    glm::ivec2 pos;
    bool alive;
    POWERUP_TYPE type;
};

#endif // _POWERUP_INCLUDE
