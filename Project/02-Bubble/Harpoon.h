// Harpoon.h
#ifndef HARPOON_H
#define HARPOON_H

#include <glm/glm.hpp>
#include "Sprite.h"
#include "TileMap.h"
#include <vector>

class Harpoon {
public:
    void init(const glm::vec2 &initPosition, ShaderProgram &shaderProgram);
    void update(int deltaTime);
    void render();
    void setTileMap(TileMap *tileMap);
    bool isAlive() const;
    void kill();
    void setPosition(const glm::vec2& pos);
    void shoot(const glm::vec2& pos);
    bool shooting();
private:

    glm::vec2 tileMapDispl, posHarpoonIni, posHarpoon, posPlayer;
    float length; // Longitud actual del arpón
    bool alive;
    Sprite *tipSprite; // Sprite para la punta del arpón
    Sprite *bodySprite; // Sprite para el cuerpo del arpón
    Texture spritesheet, spritesheet2;
    TileMap *map;

};


#endif // HARPOON_H
