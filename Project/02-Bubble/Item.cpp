#include "Item.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#define bitsSize 0.125
#define liveCycle 300;
#define blinkCycle 60;

Item::Item() {
}

Item::~Item() {

}

void Item::init(ITEM_TYPE type, glm::vec2 initPos, ShaderProgram& shaderProgram) {
	this->itemType = type;
	this->cyclesCounter = 0;
	pos = initPos;

	glm::ivec2 quadSize = glm::ivec2(16, 16);
	glm::vec2 sizeInSpriteSheet = glm::vec2(bitsSize, bitsSize);
	boxSize = glm::vec2(16, 16);
	int x, y;

	spritesheet.loadFromFile("images/Items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	
	sprite = Sprite::createSprite(quadSize, sizeInSpriteSheet, &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	switch (type)
	{
	case Item::STOP_WATCH:
		x = 3;
		y = 0;
		
		sprite->setAnimationSpeed(NORMAL, 8);
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * x, bitsSize * y));

		sprite->setAnimationSpeed(BLINKING, 15);
		sprite->addKeyframe(BLINKING, glm::vec2(bitsSize * x, bitsSize * y));
		sprite->addKeyframe(BLINKING, glm::vec2(bitsSize * 2, bitsSize * 2));
		break;
	case Item::INVENCIBLE:
		x = 0;
		y = 4;

		sprite->setAnimationSpeed(NORMAL, 8);
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * x, bitsSize * y));
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * (x + 1), bitsSize * y));
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * (x + 2), bitsSize * y));
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * (x + 3), bitsSize * y));
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * (x + 4), bitsSize * y));
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * (x + 5), bitsSize * y));
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * (x + 6), bitsSize * y));
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * (x + 7), bitsSize * y));

		sprite->setAnimationSpeed(BLINKING, 15);
		sprite->addKeyframe(BLINKING, glm::vec2(bitsSize * x, bitsSize * y));
		sprite->addKeyframe(BLINKING, glm::vec2(bitsSize * 2, bitsSize * 2));
		break;
	case Item::DINAMITE:
		x = 1;
		y = 1;
		sprite->setAnimationSpeed(NORMAL, 8);
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * x, bitsSize * y));
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * (x + 1), bitsSize * y));
		sprite->addKeyframe(NORMAL, glm::vec2(bitsSize * (x + 2), bitsSize * y));

		sprite->setAnimationSpeed(BLINKING, 15);
		sprite->addKeyframe(BLINKING, glm::vec2(bitsSize * x, bitsSize * y));
		sprite->addKeyframe(BLINKING, glm::vec2(bitsSize * 2, bitsSize * 2));
		break;
	case Item::NONE:
	default:
		break;
	}
	

	sprite->setAnimationSpeed(DEAD, 1);
	sprite->addKeyframe(DEAD, glm::vec2(bitsSize * 2, bitsSize * 2));


	sprite->changeAnimation(NORMAL);
	sprite->setPosition(glm::vec2(float(pos.x), float(pos.y)));

	alive = true;

}

void Item::update(int deltaTime) {
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

void Item::render() {
	sprite->render();
}

void Item::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Item::kill() {
	sprite->changeAnimation(DEAD);
	sprite->update(0);
	sprite->setPosition(glm::vec2(float(pos.x), float(pos.y)));
	alive = false;
}

bool Item::isAlive() {
	return alive;
}


Item::ITEM_TYPE Item::checkCollider(glm::vec2 playerPos, glm::vec2 playerSize) {

	// Comprueba si hay colisi�n (esto es una comprobaci�n de AABB - Axis Aligned Bounding Box)
	bool collisionX = playerPos.x + playerSize.x >= pos.x &&
		pos.x + boxSize.x >= playerPos.x;
	bool collisionY = playerPos.y + playerSize.y >= pos.y &&
		pos.y + boxSize.y >= playerPos.y;

	// Si hay colisi�n en ambas dimensiones, entonces hay una colisi�n
	if (collisionX && collisionY) {
		return itemType;
	}
	return NONE;
}