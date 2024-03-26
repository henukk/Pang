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
		boxSize = glm::vec2(48, 40);
		break;
	case Ball::BIG:
		quadSize = glm::ivec2(32, 32);
		sizeInSpriteSheet = glm::vec2(bitsSize * 4, bitsSize * 4);
		offset = 6;
		boxSize = glm::vec2(32, 32);
		break;
	case Ball::MEDIUM:
		quadSize = glm::ivec2(16, 16);
		sizeInSpriteSheet = glm::vec2(bitsSize * 2, bitsSize * 2);
		offset = 10;
		boxSize = glm::vec2(16, 16);
		break;
	case Ball::SMALL:
		quadSize = glm::ivec2(8, 8);
		sizeInSpriteSheet = glm::vec2(bitsSize * 1, bitsSize * 1);
		offset = 12;
		boxSize = glm::vec2(8, 8);
		break;
	default:
		quadSize = glm::ivec2(48, 40);
		sizeInSpriteSheet = glm::vec2(bitsSize * 6, bitsSize * 5);
		offset = 0;
		boxSize = glm::vec2(8 * 6 / 2, 8 * 5 / 2);
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
	const int speedX = 1;

	const int gravity = 1;
	const int gravityFactor = 8;
	const int jumpForce = 5;
	const int maxGravity = 5;

	static int gravityCounter = 0;
	static int jumpCounter = 0;

	static int velocityX = speedX;
	static int velocityY = 0;
	static bool onGround = false;

	sprite->update(deltaTime);

	posBall.x += velocityX;
	
	if (map->collisionMoveLeft(posBall, boxSize) || map->collisionMoveRight(posBall, boxSize))
	{
		posBall.x -= velocityX;
		velocityX = -velocityX;
	}
	if (map->collisionMoveUp(posBall, boxSize))
	{
		velocityY = 0; // Detener el movimiento hacia arriba
	}

	if (!onGround)
	{
		gravityCounter += gravity;
		if (gravityCounter > gravityFactor) {
			gravityCounter = 0;
			velocityY += 1;
		}
		if (velocityY > maxGravity) {
			velocityY = maxGravity;
		}
	}

	posBall.y += velocityY;

	// Detección de colisión y rebote vertical
	
	if (map->collisionMoveDown(posBall, boxSize))
	{
		gravityCounter = 0;
		posBall.y -= velocityY;
		onGround = true; // La bola está en el suelo
		velocityY = -jumpForce; // Resetear la velocidad vertical para el rebote
	}
	else
	{
		onGround = false;
	}

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

glm::vec2 Ball::getPosition()
{
	return posBall;
}


bool Ball::isHitByHarpoon(Harpoon& harpoon) {
	// Asumiendo que tienes métodos para obtener la posición y el tamaño del sprite del arpón
	glm::vec2 harpoonPos = harpoon.getPosition();
	int harpoonSize = harpoon.getSize();

	// Comprobar si las cajas de colisión se solapan
	return !(posBall.x + boxSize.x < harpoonPos.x || posBall.x > harpoonPos.x + 8 ||
		posBall.y + boxSize.y < harpoonPos.y || posBall.y > harpoonPos.y + harpoonSize);
}

glm::vec2 Ball::getSize()
{
	return boxSize;
}
