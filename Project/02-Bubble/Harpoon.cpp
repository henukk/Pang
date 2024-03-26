// Harpoon.cpp
#include "Harpoon.h"
#include <GL/glew.h>


void Harpoon::init(const glm::vec2& tileMapPos, ShaderProgram& shaderProgram) {
    alive = true;
    length = 16.0f; // Inicia con la longitud de la punta del arpón
    posHarpoon = posHarpoonIni = tileMapPos;
    //shaderProgam = shaderProgram;

    spritesheet.loadFromFile("./images/Punxa.png", TEXTURE_PIXEL_FORMAT_RGBA);
    
    tipSprite = Sprite::createSprite(glm::ivec2(9, 12), glm::vec2(1, 1), &spritesheet, &shaderProgram); // Tamaño y coords para la punta
    tipSprite->setNumberAnimations(0);

    spritesheet2.loadFromFile("./images/Cua.png", TEXTURE_PIXEL_FORMAT_RGBA);
    bodySprite = Sprite::createSprite(glm::ivec2(9, 16), glm::vec2(1, 1), &spritesheet2, &shaderProgram); // Tamaño y coords para el cuerpo
    bodySprite->setNumberAnimations(0);

    tileMapDispl = tileMapPos;
    tipSprite->setPosition(glm::vec2(float(tileMapDispl.x + posHarpoon.x), float(tileMapDispl.y + posHarpoon.y)));
}



void Harpoon::update(int deltaTime) {

    tipSprite->update(deltaTime);
    if (alive) {
        
        posHarpoon.y -= 2; 

        // Llama a collisionMoveUp pasando la dirección de newYPosition como tercer argumento.
        if (map->collisionMoveUp(glm::ivec2(int(posHarpoon.x), int(posHarpoon.y) ), glm::ivec2(9, 12))) {
            posHarpoon.y += 2;
            alive = false; // Si hay colisión, marca el arpón como no activo.
            
        }

        tipSprite->setPosition(glm::vec2(float(tileMapDispl.x + posHarpoon.x), float(tileMapDispl.y + posHarpoon.y)));
        bodySprite->setPosition(glm::vec2(float(tileMapDispl.x + posHarpoon.x), float(tileMapDispl.y + posHarpoon.y + 12)));

        
    }
}



void Harpoon::render() {

    if (alive) {
        // Dibuja la punta del arpón
        tipSprite->setPosition(posHarpoon);
        tipSprite->render();

        // Dibuja el cuerpo del arpón repitiendo el sprite del cuerpo según la longitud actual
        float bodySegmentHeight = 16;
        int numSegments = (posHarpoonIni.y - posHarpoon.y +176 - 16) / 16;  //static_cast<int>(length / bodySegmentHeight);
        for (int i = 0; i < numSegments; ++i) {
            bodySprite->setPosition(glm::vec2(float( posHarpoon.x), float(posHarpoon.y + 12 + i * 16)));
            bodySprite->render();
        }
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



void Harpoon::setPosition(const glm::vec2& pos)
{
    posHarpoon.x = pos.x;
    posHarpoon.y = pos.y;
    tipSprite->setPosition(glm::vec2(float(posHarpoon.x), float( posHarpoon.y )));
    bodySprite->setPosition(glm::vec2(float( posHarpoon.x), float( posHarpoon.y + 12 )));
}


void Harpoon::shoot(const glm::vec2& pos)
{
    posPlayer = pos;
    alive = true;
}

bool Harpoon::shooting() {
    return alive;
}

glm::vec2 Harpoon::getPosition() {
    return posHarpoon;
}

int Harpoon::getSize() {
    return length;
}



