#include "Food.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#define bitsSize 0.125

Food::Food() {
}

Food::~Food() {

}

vector<int> points = { 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 12000, 14000, 16000, 18000, 20000, 22000, 24000, 26000, 28000, 30000, 33000, 36000, 39000, 42000, 45000, 48000, 50000 };

void Food::init(int id, int* playerPoints, glm::vec2 initPos, ShaderProgram& shaderProgram) {
	this->id = id;
	this->cyclesCounter = 0;
	pos = initPos;
	this->playerPoints = playerPoints;

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
		quadSize = glm::ivec2(16, 8);
		sizeInSpriteSheet = glm::vec2(bitsSize * 2, bitsSize);
		boxSize = glm::vec2(16, 16);
		x = (4 + (id - 19) * 2) % 8;
		y = (4 + (id - 19) * 2) / 8;
	}
	else {
		quadSize = glm::ivec2(16, 16);
		sizeInSpriteSheet = glm::vec2(bitsSize * 2, bitsSize * 2);
		boxSize = glm::vec2(16, 8);
		x = 2;
		y = 4;
	}

	sprite = Sprite::createSprite(quadSize, sizeInSpriteSheet, &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(NORMAL, 8);
	sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * x, bitsSize * y));

	sprite->setAnimationSpeed(BLINKING, 15);
	sprite->addKeyframe(BLINKING, glm::vec2(bitsSize * x, bitsSize * y));
	sprite->addKeyframe(BLINKING, glm::vec2(bitsSize * 5, bitsSize * 5));


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
	alive = false;
}

bool Food::isAlive() {
	return alive;
}