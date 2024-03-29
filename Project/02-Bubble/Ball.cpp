#include "Ball.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Game.h"

#define bitsSize 0.03125

void Ball::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, BALL_COLOR color, BALL_SIZE size) {
	this->color = color;
	this->size = size;
	status = true;

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
		boxSize = glm::vec2(48, 40);
		posUp = 2 * 8;
		break;
	case Ball::BIG:
		quadSize = glm::ivec2(32, 32);
		sizeInSpriteSheet = glm::vec2(bitsSize * 4, bitsSize * 4);
		offset = 6;
		boxSize = glm::vec2(32, 32);
		posUp = 8 * 8;
		break;
	case Ball::MEDIUM:
		quadSize = glm::ivec2(16, 16);
		sizeInSpriteSheet = glm::vec2(bitsSize * 2, bitsSize * 2);
		offset = 10;
		boxSize = glm::vec2(16, 16);
		posUp = 13 * 8;
		break;
	case Ball::SMALL:
		quadSize = glm::ivec2(8, 8);
		sizeInSpriteSheet = glm::vec2(bitsSize * 1, bitsSize * 1);
		offset = 12;
		boxSize = glm::vec2(8, 8);
		posUp = 19 * 8;
		break;
	default:
		quadSize = glm::ivec2(48, 40);
		sizeInSpriteSheet = glm::vec2(bitsSize * 6, bitsSize * 5);
		offset = 0;
		boxSize = glm::vec2(8 * 6 / 2, 8 * 5 / 2);
		posUp = 2 * 8;
		break;
	}

	sprite = Sprite::createSprite(quadSize, sizeInSpriteSheet, &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	switch (color)
	{
	case Ball::RED:
		sprite->setAnimationSpeed(NORMAL, 8);
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * offset, bitsSize * 0));

		sprite->setAnimationSpeed(BLINKING, 15);
		sprite->addKeyframe(BLINKING, glm::vec2(bitsSize * 26, bitsSize * 26));
		sprite->addKeyframe(BLINKING, glm::vec2(bitsSize * offset, bitsSize * 0));

		sprite->setAnimationSpeed(EXPLODING, 15);
		switch (size)
		{
		case Ball::HUGE:
			sprite->addKeyframe(EXPLODING, glm::vec2(bitsSize * 14, bitsSize * 0));
			sprite->addKeyframe(EXPLODING, glm::vec2(bitsSize * 20, bitsSize * 0));
			sprite->addKeyframe(EXPLODING, glm::vec2(bitsSize * 26, bitsSize * 0));
			sprite->addKeyframe(EXPLODING, glm::vec2(bitsSize * 24, bitsSize * 9));
			sprite->addKeyframe(EXPLODING, glm::vec2(bitsSize * 25, bitsSize * 25));
			break;
		case Ball::BIG:
			sprite->addKeyframe(EXPLODING, glm::vec2(bitsSize * 13, bitsSize * 5));
			sprite->addKeyframe(EXPLODING, glm::vec2(bitsSize * 17, bitsSize * 5));
			sprite->addKeyframe(EXPLODING, glm::vec2(bitsSize * 21, bitsSize * 5));
			sprite->addKeyframe(EXPLODING, glm::vec2(bitsSize * 25, bitsSize * 5));
			sprite->addKeyframe(EXPLODING, glm::vec2(bitsSize * 25, bitsSize * 25));
			break;
		case Ball::MEDIUM:
			sprite->addKeyframe(EXPLODING, glm::vec2(bitsSize * 13, bitsSize * 9));
			sprite->addKeyframe(EXPLODING, glm::vec2(bitsSize * 15, bitsSize * 9));
			sprite->addKeyframe(EXPLODING, glm::vec2(bitsSize * 17, bitsSize * 9));
			sprite->addKeyframe(EXPLODING, glm::vec2(bitsSize * 19, bitsSize * 9));
			sprite->addKeyframe(EXPLODING, glm::vec2(bitsSize * 25, bitsSize * 25));
			break;
		case Ball::SMALL:
			sprite->addKeyframe(EXPLODING, glm::vec2(bitsSize * 21, bitsSize * 10));
			sprite->addKeyframe(EXPLODING, glm::vec2(bitsSize * 22, bitsSize * 10));
			sprite->addKeyframe(EXPLODING, glm::vec2(bitsSize * 23, bitsSize * 10));
			sprite->addKeyframe(EXPLODING, glm::vec2(bitsSize * 25, bitsSize * 25));
			break;
		default:
			break;
		}
		break;
	case Ball::BLUE:
		sprite->setAnimationSpeed(NORMAL, 8);
		sprite->setAnimationSpeed(NORMAL, 8);
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * offset, bitsSize * 5));

		sprite->setAnimationSpeed(BLINKING, 4);
		sprite->addKeyframe(BLINKING, glm::vec2(bitsSize * offset, bitsSize * 0));
		sprite->addKeyframe(BLINKING, glm::vec2(bitsSize * 26, bitsSize * 26));

		sprite->setAnimationSpeed(EXPLODING, 8);
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * offset, bitsSize * 5));
		break;
	case Ball::GREEN:
		sprite->setAnimationSpeed(NORMAL, 8);
		sprite->setAnimationSpeed(NORMAL, 8);
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * offset, bitsSize * 10));

		sprite->setAnimationSpeed(BLINKING, 4);
		sprite->addKeyframe(BLINKING, glm::vec2(bitsSize * offset, bitsSize * 0));
		sprite->addKeyframe(BLINKING, glm::vec2(bitsSize * 26, bitsSize * 26));

		sprite->setAnimationSpeed(EXPLODING, 8);
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * offset, bitsSize * 10));
		break;
	default:
		sprite->setAnimationSpeed(NORMAL, 8);
		sprite->setAnimationSpeed(NORMAL, 8);
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * offset, bitsSize * 0));

		sprite->setAnimationSpeed(BLINKING, 4);
		sprite->addKeyframe(BLINKING, glm::vec2(bitsSize* offset, bitsSize * 0));
		sprite->addKeyframe(BLINKING, glm::vec2(bitsSize * 26, bitsSize * 26));

		sprite->setAnimationSpeed(EXPLODING, 8);
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * offset, bitsSize * 0));
		break;
	}

	sprite->changeAnimation(NORMAL);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));

}

void Ball::update(int deltaTime, int timeStopped) {
	if (!status) {
		if ((deadCounter < 4*5 && size != SMALL) || deadCounter < 4*4) {
			sprite->update(deltaTime);
		}
		deadCounter++;
		return;
	}
	if (timeStopped == 0)
		sprite->changeAnimation(NORMAL);
	else if (timeStopped == 60)
		sprite->changeAnimation(BLINKING);
	
	sprite->update(deltaTime);

	// X algo
	if (timeStopped == 0) {
		posBall.x += velocityX;

		if (map->collisionMoveLeft(posBall, boxSize) ||
			map->collisionMoveRight(posBall, boxSize))
		{
			posBall.x -= velocityX;
			velocityX = -velocityX;
		}

		// Y algo

		posBall.y += velocityY;

		if (energy >= 0) {
			if (velocityY < 0)
				velocityY = 0;
			gravityCounter += gravity;
			if (gravityCounter > gravityFactor) {
				gravityCounter = 0;
				velocityY += 1;
			}
			if (velocityY > maxGravity) {
				velocityY = maxGravity;
			}
		}
		else {
			tmpSpeed = energy / 4;
			if (tmpSpeed < -maxGravity)
				tmpSpeed = -maxGravity;
			if (tmpSpeed > -1)
				tmpSpeed = -1;
			velocityY = tmpSpeed;
			energy -= velocityY;//posUp - posBall.y;
		}

		if (map->collisionMoveDownPlayer(posBall, boxSize))
		{
			while (map->collisionMoveDownPlayer(posBall, boxSize))
				posBall.y -= 1;
			velocityY = 0;
			energy = posUp - posBall.y;
			if (energy > -10)
				energy = -20;
		}

		if (map->collisionMoveUpPlayer(posBall, boxSize))
		{
			while (map->collisionMoveUpPlayer(posBall, boxSize))
				posBall.y += 1;
			velocityY = 0;
			energy = 0;
		}
	}

	sprite->setPosition(glm::vec2(float(posBall.x), float(posBall.y)));
}

void Ball::render()
{
	if (!status &&
		(deadCounter < 4 * 5 && size != SMALL) ||
		deadCounter < 4 * 4)
	{
		sprite->render();
	}
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

glm::vec2 Ball::getPosition()
{
	return posBall;
}


bool Ball::isHitByHarpoon(Harpoon& harpoon) {
	glm::vec2 harpoonPos = harpoon.getPosition();
	glm::ivec2 screenSize = glm::ivec2(48*8,30 * 8 ); // Assuming you have a method to get the screen size

	// Assuming harpoon width is negligible, check if the ball's X position intersects the harpoon's X position
	bool intersectsInX = posBall.x < harpoonPos.x && posBall.x + boxSize.x > harpoonPos.x;

	// Check if the ball is below the top of the harpoon (which we assume extends to the top of the screen)
	bool intersectsInY = posBall.y < screenSize.y && posBall.y + boxSize.y > harpoonPos.y;

	// If both X and Y intersect, the ball is hit by the harpoon
	return intersectsInX && intersectsInY;
}


glm::vec2 Ball::getSize()
{
	return boxSize;
}

Ball::BALL_COLOR Ball::getColor() {
	return color;
}
Ball::BALL_SIZE Ball::getType() {
	return size;
}

void Ball::setDirection(int dir) {
	velocityX = dir;
}

int Ball::getDirection() {
	return velocityX;
}

bool Ball::getStatus() {
	return status;
}

void Ball::kill() {
	status = false;
	sprite->changeAnimation(EXPLODING);

}

void Ball::addForce(int force) {
	energy = force;
}