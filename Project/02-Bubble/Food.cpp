#include "Food.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#define bitsSize 0.125
#define liveCycle 300;
#define blinkCycle 60;

Food::Food() {
}

Food::~Food() {

}

vector<int> points = { 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 12000, 14000, 16000, 18000, 20000, 22000, 24000, 26000, 28000, 30000, 33000, 36000, 39000, 42000, 45000, 48000, 50000 };

void Food::init(int id, glm::vec2 initPos, ShaderProgram& shaderProgram) {
	this->id = id;
	this->cyclesCounter = 0;
	pos = initPos;

	glm::ivec2 quadSize;
	glm::vec2 sizeInSpriteSheet;
	int x, y;

	spritesheet.loadFromFile("images/Food.png", TEXTURE_PIXEL_FORMAT_RGBA);
	if (id < 20) {
		quadSize = glm::ivec2(16, 16);
		sizeInSpriteSheet = glm::vec2(bitsSize, bitsSize);
		boxSize = glm::vec2(16, 16);
		x = id % 8;
		y = id / 8;
	}
	else if (id < 27) {
		quadSize = glm::ivec2(32, 16);
		sizeInSpriteSheet = glm::vec2(bitsSize * 2, bitsSize);
		boxSize = glm::vec2(16, 16);
		x = (20 + (id - 20) * 2) % 8;
		y = (20 + (id - 20) * 2) / 8;
		if(pos.x == 360)
			pos.x -= 16;
	}
	else {
		id = 27;
		quadSize = glm::ivec2(32, 32);
		sizeInSpriteSheet = glm::vec2(bitsSize * 2, bitsSize * 2);
		boxSize = glm::vec2(32, 29);
		x = 2;
		y = 4;
		if (pos.x == 360)
			pos.x -= 16;
	}

	sprite = Sprite::createSprite(quadSize, sizeInSpriteSheet, &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(NORMAL, 8);
	sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * x, bitsSize * y));

	sprite->setAnimationSpeed(BLINKING, 15);
	sprite->addKeyframe(BLINKING, glm::vec2(bitsSize * x, bitsSize * y));
	sprite->addKeyframe(BLINKING, glm::vec2(bitsSize * 5, bitsSize * 5));

	sprite->setAnimationSpeed(DEAD, 1);
	sprite->addKeyframe(DEAD, glm::vec2(bitsSize * 5, bitsSize * 5));


	sprite->changeAnimation(NORMAL);
	sprite->setPosition(glm::vec2(float(pos.x), float(pos.y)));

	alive = true;

}

void Food::update(int deltaTime) {
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

void Food::render() {
	sprite->render();
}

void Food::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Food::kill() {
	sprite->changeAnimation(DEAD);
	sprite->update(0);
	sprite->setPosition(glm::vec2(float(pos.x), float(pos.y)));
	alive = false;
}

bool Food::isAlive() {
	return alive;
}


int Food::checkCollider(glm::vec2 playerPos, glm::vec2 playerSize) {

	// Comprueba si hay colisi�n (esto es una comprobaci�n de AABB - Axis Aligned Bounding Box)
	bool collisionX = playerPos.x + playerSize.x >= pos.x &&
		pos.x + boxSize.x >= playerPos.x;
	bool collisionY = playerPos.y + playerSize.y >= pos.y &&
		pos.y + boxSize.y >= playerPos.y;

	// Si hay colisi�n en ambas dimensiones, entonces hay una colisi�n
	if (collisionX && collisionY) {
		return points[id];
	}
	return -1;
}