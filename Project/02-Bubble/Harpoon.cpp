// Harpoon.cpp
#include "Harpoon.h"
#include <GL/glew.h>

void Harpoon::init(const glm::vec2& initPosition, ShaderProgram& shaderProgram) {
    alive = true;
    posHarpoon = initPosition;

    spritesheet.loadFromFile("images/harpoon.png", TEXTURE_PIXEL_FORMAT_RGBA); // Aseg�rate de tener esta textura
    sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.0, 1.0), &spritesheet, &shaderProgram); // Ajusta el tama�o seg�n necesites
    sprite->setPosition(glm::vec2(float(posHarpoon.x), float(posHarpoon.y)));
}

void Harpoon::update(int deltaTime) {
    if (alive) {
        int newYPosition = int(posHarpoon.y); // Prepara una variable int para la nueva posici�n Y.
        posHarpoon.y -= float(deltaTime) * 0.2f; // Ajusta la velocidad seg�n sea necesario.

        // Llama a collisionMoveUp pasando la direcci�n de newYPosition como tercer argumento.
        if (map->collisionMoveUp(glm::ivec2(int(posHarpoon.x), int(posHarpoon.y)), glm::ivec2(16, 16), &newYPosition)) {
            alive = false; // Si hay colisi�n, marca el arp�n como no activo.
            posHarpoon.y = float(newYPosition); // Ajusta la posici�n Y del arp�n seg�n la colisi�n.
        }

        sprite->setPosition(posHarpoon); // Actualiza la posici�n del sprite.
    }
}


void Harpoon::render() {
    if (alive) {
        sprite->render();
    }
}

void Harpoon::setTileMap(TileMap* tileMap) {
    map = tileMap;
}

bool Harpoon::isAlive() const {
    return alive;
}

void Harpoon::kill() {
    alive = false;
}

