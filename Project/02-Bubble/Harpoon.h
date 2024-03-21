// Harpoon.h
#ifndef HARPOON_H
#define HARPOON_H

#include <glm/glm.hpp>
#include "Sprite.h"
#include "TileMap.h"
#include <vector>

class Harpoon {
public:
    void init(const glm::vec2& initPosition, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();
    void setTileMap(TileMap* tileMap);
    bool isAlive() const;
    void kill();

private:
    glm::vec2 posHarpoon;
    bool alive;
    Sprite* sprite;
    Texture spritesheet;
    TileMap* map;
};

#endif // HARPOON_H
