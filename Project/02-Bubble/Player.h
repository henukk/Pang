#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Harpoon.h"
#include "ShaderProgram.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void shootHarpoon();
	void setInvencible(int mode);
	glm::ivec2 getPosition();

	glm::ivec2 getSize();

	void hit();
	
private:
	bool isDead;
	bool bStairs;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet, spritesheetDeath;
	Sprite *sprite;
	TileMap *map;
	vector<Harpoon*> harpoons;
	ShaderProgram texProgram;
	float deathAnimationTime;

	Texture spritesheetInvencible;
	Sprite* spriteInvencible;

};


#endif // _PLAYER_INCLUDE


