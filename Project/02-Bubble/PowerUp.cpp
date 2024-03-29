#include "PowerUp.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#define bitsSize 0.125

PowerUp::PowerUp() {

}

PowerUp::~PowerUp() {

}


void PowerUp::init(int id, glm::vec2 initPos, ShaderProgram& shaderProgram) {
	this->id = id;
	this->cyclesCounter = 0;
	pos = initPos;

	glm::ivec2 quadSize;
	glm::vec2 sizeInSpriteSheet;
	int x, y;

	spritesheet.loadFromFile("images/PowerUps.png", TEXTURE_PIXEL_FORMAT_RGBA);
	quadSize = glm::ivec2(16, 16);
	sizeInSpriteSheet = glm::vec2(0.125, 0.5);
	boxSize = glm::vec2(16, 16);
	

	sprite = Sprite::createSprite(quadSize, sizeInSpriteSheet, &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);
	if (id == 1) {

		x = id % 8;
		y = id / 8;
	}
	else if (id == 2) {

		x = (20 + (id - 20) * 2) % 8;
		y = (20 + (id - 20) * 2) / 8;
		if (pos.x == 360)
			pos.x -= 16;
	}
	else {

		x = 2;
		y = 4;
		if (pos.x == 360)
			pos.x -= 16;
	}
	sprite->setAnimationSpeed(DYNAMITE, 8);
	sprite->addKeyframe(DYNAMITE, glm::vec2(0.125,0.5));

	sprite->setAnimationSpeed(STOP, 8);
	sprite->addKeyframe(STOP, glm::vec2(0.0, 0.5));

	sprite->setAnimationSpeed(INVENCIBLE, 8);
	sprite->addKeyframe(INVENCIBLE, glm::vec2(0.0, 0.0));
	sprite->addKeyframe(INVENCIBLE, glm::vec2(0.166, 0.0));
	sprite->addKeyframe(INVENCIBLE, glm::vec2(0.33, 0.0));
	sprite->addKeyframe(INVENCIBLE, glm::vec2(0.5, 0.0));
	sprite->addKeyframe(INVENCIBLE, glm::vec2(0.666, 0.0));
	sprite->addKeyframe(INVENCIBLE, glm::vec2(0.833, 0.0));


	alive = true;

}

void PowerUp::update(int deltaTime) {
	sprite->update(deltaTime);

	cyclesCounter++;

	pos.y += 1;
	if (map->collisionMoveDown(pos, boxSize))
	{
		while (map->collisionMoveDown(pos, boxSize))
			pos.y -= 1;
	}

	sprite->setPosition(glm::vec2(float(pos.x), float(pos.y)));
}

void PowerUp::render() {
	sprite->render();
}

void PowerUp::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void PowerUp::kill() {
	sprite->update(0);
	sprite->setPosition(glm::vec2(float(pos.x), float(pos.y)));
	alive = false;
}

bool PowerUp::isAlive() {
	return alive;
}


int PowerUp::checkCollider(glm::vec2 playerPos, glm::vec2 playerSize) {

	// Comprueba si hay colisi�n (esto es una comprobaci�n de AABB - Axis Aligned Bounding Box)
	bool collisionX = playerPos.x + playerSize.x >= pos.x &&
		pos.x + boxSize.x >= playerPos.x;
	bool collisionY = playerPos.y + playerSize.y >= pos.y &&
		pos.y + boxSize.y >= playerPos.y;

	// Si hay colisi�n en ambas dimensiones, entonces hay una colisi�n
	
	return -1;
}

bool PowerUp::isTypeInvencible() const {
	return type == INVENCIBLE;
}

bool PowerUp::isTypeStop() const {
	return type == STOP;
}


bool PowerUp::isTypeDynamite() const {
	return type == DYNAMITE;
}
