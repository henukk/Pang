#include "Ball.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>

#define bitsSize 0.0625

void Ball::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, BALL_COLOR color, BALL_SIZE size) {
    this->color = color;
    this->size = size;

    spritesheet.loadFromFile("images/Balloons.png", TEXTURE_PIXEL_FORMAT_RGBA);
    
	glm::ivec2 quadSize;
	glm::vec2 sizeInSpriteSheet;
	int offset = 0;


	switch (size)
	{
	case Ball::HUGE:
		quadSize = glm::ivec2(48, 40);
		sizeInSpriteSheet = glm::vec2(bitsSize * 6, bitsSize * 5);
		offset = 0;
		break;
	case Ball::BIG:
		quadSize = glm::ivec2(32, 32);
		sizeInSpriteSheet = glm::vec2(bitsSize * 4, bitsSize * 4);
		offset = 6;
		break;
	case Ball::MEDIUM:
		quadSize = glm::ivec2(16, 16);
		sizeInSpriteSheet = glm::vec2(bitsSize * 2, bitsSize * 2);
		offset = 10;
		break;
	case Ball::SMALL:
		quadSize = glm::ivec2(8, 8);
		sizeInSpriteSheet = glm::vec2(bitsSize * 1, bitsSize * 1);
		offset = 12;
		break;
	default:
		quadSize = glm::ivec2(48, 40);
		sizeInSpriteSheet = glm::vec2(bitsSize * 6, bitsSize * 5);
		offset = 0;
		break;
	}

	sprite = Sprite::createSprite(quadSize, sizeInSpriteSheet, &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);
	
	switch (color)
	{
	case Ball::RED:
		sprite->setAnimationSpeed(NORMAL, 8);
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * offset, bitsSize * 0));

		sprite->setAnimationSpeed(EXPLODING, 8);
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * offset, bitsSize * 0));
		break;
	case Ball::BLUE:
		sprite->setAnimationSpeed(NORMAL, 8);
		sprite->setAnimationSpeed(NORMAL, 8);
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * offset, bitsSize * 5));

		sprite->setAnimationSpeed(EXPLODING, 8);
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * offset, bitsSize * 5));
		break;
	case Ball::GREEN:
		sprite->setAnimationSpeed(NORMAL, 8);
		sprite->setAnimationSpeed(NORMAL, 8);
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * offset, bitsSize * 10));

		sprite->setAnimationSpeed(EXPLODING, 8);
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * offset, bitsSize * 10));
		break;
	default:
		sprite->setAnimationSpeed(NORMAL, 8);
		sprite->setAnimationSpeed(NORMAL, 8);
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * offset, bitsSize * 0));

		sprite->setAnimationSpeed(EXPLODING, 8);
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * offset, bitsSize * 0));
		break;
	}

    sprite->changeAnimation(NORMAL);
    tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));

}



void Ball::update(int deltaTime)
{
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(posBall.x), float(posBall.y)));
}

void Ball::render()
{
	sprite->render();
}

void Ball::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Ball::setPosition(const glm::vec2& pos)
{
	posBall = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}