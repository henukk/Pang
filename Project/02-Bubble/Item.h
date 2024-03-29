// Item.h
#ifndef _ITEM_INCLUDE
#define _ITEM_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"

class Item
{
public:
    enum ITEM_TYPE {
        NONE, STOP_WATCH, INVENCIBLE, DINAMITE
    };

    Item();
    ~Item();

    void init(ITEM_TYPE type, glm::vec2 initPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();
    void setTileMap(TileMap* tileMap);
    bool isAlive();
    ITEM_TYPE checkCollider(glm::vec2 playerPos, glm::vec2 playerSize);
    void kill();
private:
    enum ITEM_STATUS {
        NORMAL, BLINKING, DEAD
    };

private:
    ITEM_TYPE itemType;
    int cyclesCounter;
    TileMap* map;
    Texture spritesheet;
    Sprite* sprite;
    glm::vec2 boxSize;
    glm::ivec2 pos;
    bool alive;
};

#endif // _ITEM_INCLUDE
