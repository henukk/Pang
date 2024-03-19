#include "Ball.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>


void Ball::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, BALL_COLOR color, BALL_SIZE size) {
    this->color = color;
    this->size = size;

    spritesheet.loadFromFile("images/Ballons.png", TEXTURE_PIXEL_FORMAT_RGBA);
    
    glm::ivec2 quadSize, sizeInSpriteSheet;

    sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 0.25), &spritesheet, &shaderProgram);



}
