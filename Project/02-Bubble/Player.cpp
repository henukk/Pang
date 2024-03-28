#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	MOVE_LEFT, MOVE_RIGHT, STAY, DISPARA, PUJA, DIE_RIGHT, DIE_LEFT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{

	bStairs = false;
	spritesheet.loadFromFile("images/BluePlayer.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.6f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.7f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.8f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.9f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.3, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4, 0.f));

		sprite->setAnimationSpeed(STAY, 8);
		sprite->addKeyframe(STAY, glm::vec2(0.0, 0.75f));

		sprite->setAnimationSpeed(DISPARA, 8);
		sprite->addKeyframe(DISPARA, glm::vec2(0.1, 0.75f));

		sprite->setAnimationSpeed(PUJA, 8);
		sprite->addKeyframe(PUJA, glm::vec2(0.0, 0.25f));
		sprite->addKeyframe(PUJA, glm::vec2(0.1, 0.25f));
		sprite->addKeyframe(PUJA, glm::vec2(0.2, 0.25f));
		sprite->addKeyframe(PUJA, glm::vec2(0.3, 0.25f));
	
		
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}



void Player::update(int deltaTime)
{


	sprite->update(deltaTime);
	bool onStairs = map->bStairs(posPlayer, glm::ivec2(32, 32));
	bool downStairs = map->bStairsDown(posPlayer, glm::ivec2(32, 32));
	bool isfalling = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32)) && !onStairs;
	bool floorDown = map->floorDown(posPlayer, glm::ivec2(32, 32));
	/*cout << isfalling << endl;
	cout << posPlayer.x << ' ' << posPlayer.y << endl;*/
	if(Game::instance().getKey(GLFW_KEY_LEFT) )
	{

		
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x += 2;
		}
	}
	else if(Game::instance().getKey(GLFW_KEY_RIGHT) )
	{

		
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{	posPlayer.x -= 1;
		if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
			posPlayer.x -= 1;
			sprite->changeAnimation(MOVE_RIGHT);
		}
	}

	else if(Game::instance().getKey(GLFW_KEY_UP) && onStairs) {
		if (sprite->animation() != PUJA )
			sprite->changeAnimation(PUJA);
		posPlayer.y -= 1;
		if (map->collisionMoveUpPlayer(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.y += 1;
			sprite->changeAnimation(PUJA);
		}
		//MIREM SI TENIM UNA ESCALA
		
	}else if (Game::instance().getKey(GLFW_KEY_DOWN) && downStairs) {
		if (sprite->animation() != PUJA) 
			sprite->changeAnimation(PUJA);
		posPlayer.y += 1;

		if (map->collisionMoveDownPlayer(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.y -= 1;
			sprite->changeAnimation(PUJA);
		}
		//MIREM SI TENIM UNA ESCALA

	}else if (Game::instance().getKey(GLFW_KEY_SPACE)) {
		//if (sprite->animation() != PUJA) sprite->changeAnimation(PUJA);
		sprite->changeAnimation(DISPARA);
		
	}
	else if (!map->collisionMoveDown(posPlayer, glm::ivec2(32, 32)) && !onStairs) {
		posPlayer.y += 2;
		sprite->changeAnimation(STAY);

		if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.y -= 2;
			sprite->changeAnimation(STAY);
		}
	}
	
	
	else sprite->changeAnimation(STAY);
	


	sprite->setPosition(glm::vec2(float(posPlayer.x), float(posPlayer.y)));
}

void Player::render()
{
	sprite->render();
	
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPosition()  {
	return posPlayer;
}

glm::ivec2 Player::getSize() {
	return glm::ivec2(32,32);
}

void Player::hit() {
	// Cambia a la textura de la muerte y configura la animación de muerte
	sprite->changeTexture(&spritesheetDeath);
	sprite->changeAnimation(DIE_RIGHT); 
	isDead = true;
}








