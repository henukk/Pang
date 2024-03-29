#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims {
	MOVE_LEFT, MOVE_RIGHT, STAY, DISPARA, PUJA, DIE_RIGHT, DIE_LEFT
};
enum InvencibleStatus {
	ENABLED, DISABLED, BLINKING
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	deathAnimationTime = 0.0f;
	bStairs = false;
	spritesheet.loadFromFile("images/BluePlayer.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetDeath.loadFromFile("images/PlayerDie", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(
		glm::ivec2(32, 32),
		glm::vec2(0.1, 0.25),
		&spritesheet, &shaderProgram
	);
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
	
	//Invencible
	spritesheetInvencible.loadFromFile("images/Items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteInvencible = Sprite::createSprite(
		glm::ivec2(32, 48),
		glm::vec2(0.125 * 2, 0.125 * 3),
		&spritesheetInvencible,
		&shaderProgram
	);
	spriteInvencible->setNumberAnimations(3);

	spriteInvencible->setAnimationSpeed(ENABLED, 8);
	spriteInvencible->addKeyframe(ENABLED, glm::vec2(0, 0.125 * 5));
	spriteInvencible->addKeyframe(ENABLED, glm::vec2(0.125 * 2, 0.125 * 5));

	spriteInvencible->setAnimationSpeed(DISABLED, 8);
	spriteInvencible->addKeyframe(DISABLED, glm::vec2(0.125 * 4, 0.125 * 5));

	spriteInvencible->setAnimationSpeed(BLINKING, 8);
	spriteInvencible->addKeyframe(BLINKING, glm::vec2(0.125 * 4, 0.125 * 5));
	spriteInvencible->addKeyframe(BLINKING, glm::vec2(0, 0.125 * 5));
	spriteInvencible->addKeyframe(BLINKING, glm::vec2(0.125 * 4, 0.125 * 5));
	spriteInvencible->addKeyframe(BLINKING, glm::vec2(0.125 * 2, 0.125 * 5));

	spriteInvencible->changeAnimation(DISABLED);
	spriteInvencible->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y - 12)));
}

void Player::update(int deltaTime)
{
	spriteInvencible->update(deltaTime);
	sprite->update(deltaTime);
	bool onStairs = map->bStairs(posPlayer, glm::ivec2(32, 32));
	bool downStairs = map->bStairsDown(posPlayer, glm::ivec2(32, 32));
	bool isfalling = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32)) && !onStairs;
	bool floorDown = map->floorDown(posPlayer, glm::ivec2(32, 32));
	/*cout << isfalling << endl;
	cout << posPlayer.x << ' ' << posPlayer.y << endl;*/
	if (isDead) {
		if (deathAnimationTime > 0) {
			// Subtract the time since last update
			deathAnimationTime -= deltaTime;

			// If the death animation time has elapsed, fall off the screen
			if (deathAnimationTime <= 0) {
				// Code to make the player fall indefinitely
				posPlayer.y += FALL_STEP;
			}
		}
		else {
			// Code to make the player fall indefinitely if deathAnimationTime is already 0
			posPlayer.y += FALL_STEP;
		}
	}
	
	if (!map->collisionMoveDown(posPlayer, glm::ivec2(32, 32)) && !onStairs) {
		posPlayer.y += 2;
		//sprite->changeAnimation(STAY);

		if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.y -= 2;
			//sprite->changeAnimation(STAY);
		}
	}
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
	else if(Game::instance().getKey(GLFW_KEY_RIGHT))
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
	
	else sprite->changeAnimation(STAY);

	spriteInvencible->setPosition(glm::vec2(float(posPlayer.x), float(posPlayer.y - 12)));
	sprite->setPosition(glm::vec2(float(posPlayer.x), float(posPlayer.y)));
}

void Player::render()
{
	spriteInvencible->render();
	sprite->render();
	
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	spriteInvencible->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y - 12)));
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPosition()  {
	return glm::ivec2(posPlayer.x+8, posPlayer.y+6);
}

glm::ivec2 Player::getSize() {
	return glm::ivec2(16,26);
}

void Player::hit() {
	// Assuming 'spritesheetDeath' is already loaded with the death animation texture
	sprite = Sprite::createSprite(glm::ivec2(40, 32), glm::vec2(0.5, 0.5), &spritesheet, &texProgram);
	sprite->changeTexture(&spritesheetDeath); // Update the texture if needed
	sprite->setNumberAnimations(6); // Add a new animation slot if not enough

	sprite->setAnimationSpeed(DIE_RIGHT, 8); // Set the speed of the death animation
	// Here we are assuming the death animation frames go from (0.0, 0.0) to (0.5, 0.5) as a single row
	for (float i = 0.0f; i <= 0.5f; i += 0.1f) { // Adjust if the frame step is different
		sprite->addKeyframe(DIE_RIGHT, glm::vec2(i, 0.f));
	}

	sprite->changeAnimation(DIE_RIGHT); // Change to the death animation
	isDead = true; // Set the death flag to true
	deathAnimationTime = 3000.0f;
}


void Player::setInvencible(int mode) { //0 = disable, 1 = enable, 2 blink
	if (mode == 1)
		spriteInvencible->changeAnimation(ENABLED);
	else if (mode == 2) {
		spriteInvencible->changeAnimation(BLINKING);
	}
	else
		spriteInvencible->changeAnimation(DISABLED);
}






