#pragma once
#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Score.h"
#include "SoundManager.h"
#include <vector>
#include "Text.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene {

public:
	Scene();
	~Scene();

	void init();
	void restart();
	void worldChange();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	void checkWorldKeys();


	void setGameOver();
	void setChangingScene();

	SoundManager sound;

	TileMap* map;
	Player* player;
	ShaderProgram texProgram;
	Text text;
	float currentTime;

	/* Game Pause */
	bool pause, keyPausePressed;

	/* Game Over */
	bool gameOver;
	int timeGameOver;

	/* Game Win */
	bool gameWin;
	int timeGameWin;

	/* Scene Change */
	bool changingScene;
	int timeChangingScene;

	glm::mat4 projection;
	vector<TileMap*> maps;

	int timeStageClear;
};


#endif // _SCENE_INCLUDE