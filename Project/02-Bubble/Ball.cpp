#include "Ball.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>


void Ball::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, BALL_COLOR color, BALL_SIZE size) {
    this->color = color;
    this->size = size;

    spritesheet.loadFromFile("images/Ballons.png", TEXTURE_PIXEL_FORMAT_RGBA);

}
