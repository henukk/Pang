#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene2.h"
#include "Game.h"

#define SCREEN_X 0
#define SCREEN_Y 32

#define INIT_PLAYER_X_TILES 2
#define INIT_PLAYER_Y_TILES 11

#define TIME_GAME_OVER 7000
#define TIME_GAME_WIN 5000
#define TIME_STAGE_CLEAR 7000
#define TIME_CHANGING_SCENE 3000


Scene::Scene() {
	map = NULL;
	player = NULL;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
}

Scene::~Scene() {
	if (map != NULL) delete map;
	if (player != NULL) delete player;
	for (Ball* b : balls) {
		if (b != NULL)
			delete b;
	}
}

void Scene::init() {
	initShaders();
	maps = vector<TileMap*>(2, nullptr);
	maps[0] = TileMap::createTileMap("levels/world1.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	maps[1] = TileMap::createTileMap("levels/world2.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map = maps[0];
	if (player != nullptr) delete player;
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, &sound);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getBlockSize(), INIT_PLAYER_Y_TILES * map->getBlockSize()));
	player->setTileMap(map);
	camera = Projection(glm::vec2(0.f, 0.f), glm::vec2(float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1)));
	currentTime = 0.0f;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);

	/* Score */
	Score::instance().init();

	/* Start Menu */
	startMenu.init();


	pause = false, keyPausePressed = false;
	gameOver = false;
	gameWin = false, changingScene = true;
	timeStageClear = 0, timeChangingScene = 0;
	if (!text.init("fonts/super-mario-bros-nes.ttf")) {
		cout << "Could not load font!!!" << endl;
	}
}

void Scene::restart() {
	currentTime = 0.0f;

	/* --- Player --- */
	delete player;
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, &sound);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getBlockSize(), INIT_PLAYER_Y_TILES * map->getBlockSize()));
	player->setTileMap(map);

	/* --- Camera --- */
	camera = Projection(glm::vec2(0.f, 0.f), glm::vec2(float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1)));

	/* --- Map --- */
	map->restart();

	/* --- Score --- */
	Score::instance().restart();

	/* --- Sound --- */
	sound.stopBGM();
	if (Score::instance().gameOver()) {
		setGameOver();
	}
	else if (!gameWin) sound.playBGM("music/title.mp3", true);
}

void Scene::worldChange() {
	currentTime = 0.0f;

	/* --- Player --- */
	bool isSuperMario = player->isSuperMario();
	delete player;
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, &sound);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getBlockSize(), INIT_PLAYER_Y_TILES * map->getBlockSize()));
	player->setTileMap(map);
	player->setInitialStateSuperMario(isSuperMario);

	/* --- Camera --- */
	camera = Projection(glm::vec2(0.f, 0.f), glm::vec2(float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1)));

	/* --- Map --- */
	map->restart();

	/* --- Score --- */
	Score::instance().restartTime();

	/* --- Sound --- */
	sound.stopBGM();
	if (Score::instance().gameOver()) {
		setGameOver();
	}

	setChangingScene();
}

void Scene::update(int deltaTime) {
	/* --- Menu --- */
	startMenu.update(deltaTime);
	bool inMenu = startMenu.showingMenu();
	if (inMenu) return;

	/* --- Game Over --- */
	if (gameOver) {
		timeGameOver += deltaTime;
		if (timeGameOver > TIME_GAME_OVER) {
			gameOver = false, pause = false;
			startMenu.openMenu();
			glClearColor(0.3607843137f, 0.5803921569f, 0.9882352941f, 1.0f);
			Score::instance().init();
			setChangingScene();
		}
		return;
	}

	/* Game Win */
	if (gameWin) {
		timeGameWin += deltaTime;
		if (timeGameWin > TIME_GAME_WIN) {
			pause = false;
			startMenu.openMenu();
			glClearColor(0.3607843137f, 0.5803921569f, 0.9882352941f, 1.0f);
			map = maps[0];
			map->restart();
			restart();
			gameWin = false;
			Game::instance().clearInput();
			Score::instance().init();
			setChangingScene();
		}
		return;
	}

	/* Changing Scene */
	if (changingScene) {
		timeChangingScene += deltaTime;
		if (timeChangingScene > TIME_CHANGING_SCENE) {
			changingScene = false;
			timeChangingScene = 0;
			sound.playBGM("music/title.mp3", true);
			glClearColor(0.3607843137f, 0.5803921569f, 0.9882352941f, 1.0f);
		}
		return;
	}

	/* --- Pause --- */
	bool keyPause = Game::instance().getKey(13);
	if (keyPause && !keyPausePressed) {
		keyPausePressed = true;
		sound.playSFX("sfx/pause.wav");
		if (pause) {
			pause = false;
			sound.resumeBGM();
		}
		else {
			pause = true;
			sound.pauseBGM();
			return;
		}
	}
	keyPausePressed = keyPause;
	if (pause) return;

	/* CHECKING IF REACHED THE FINISH LINE */
	if (map->reachFinishLine(player->getPos(), player->getSize(), player->isSuperMario())) {
		sound.stopBGM();
		if (!map->animationOfFlag(deltaTime)) {
			player->animationOfReachingFinal(deltaTime);
		}
		else {
			timeStageClear += deltaTime;
			if (!map->reachEntranceCastle(player->getPos())) {
				player->reachCastleAnimation(deltaTime);
			}
			else if (Score::instance().getTime() > 0) {
				Score::instance().timeToScore();
				sound.playSFX("sfx/getting_points.wav");
			}
			else if (timeStageClear > TIME_STAGE_CLEAR) {
				sound.playSFX("sfx/coin.wav");
				timeStageClear = 0;
				// Move to next stage
				if (map == maps[0]) {
					map = maps[1];
					Score::instance().updateWorld(2, 1);
					worldChange();
				}
				else {
					// Win Game
					gameWin = true;
					timeGameWin = 0;
				}
			}
		}
		return;
	}

	checkWorldKeys();


	/* --- Time --- */
	currentTime += deltaTime;

	/* --- Map --- */
	map->update(deltaTime);
	if (!player->isDead()) map->collisionWithItems(player);

	/* --- Camera --- */
	float actualMid = camera.getXmid();

	/* --- Player --- */
	player->update(deltaTime, camera.getXmin(), actualMid);

	/* --- Enemies --- */
	map->updateEnemies(deltaTime, player, camera.getXmin(), actualMid);

	/* --- Music --- */
	if (player->isDead()) sound.stopBGM();

	if (Score::instance().getTime() <= 0) {
		sound.stopBGM();
		player->setDying();
	}

	/* --- Score --- */
	if (!player->isDead()) Score::instance().update(deltaTime);

	/* --- Camera Position Update --- */
	if (actualMid != camera.getXmid()) {
		camera.setMidXPosition(actualMid);
	}

	/* --- Restart --- */
	glm::vec2 pos = player->getPos();
	if (pos.y > 1600) {
		Score::instance().decreaseLive();
		restart();
	}
}

void Scene::render() {
	if (gameWin && !startMenu.showingMenu()) {
		glClearColor(0, 0, 0, 1.0f);
		text.render("THANK YOU MARIO!", glm::vec2(SCREEN_WIDTH / 2 - 170, SCREEN_HEIGHT / 2 + 10), 20, glm::vec4(1, 1, 1, 1));
		text.render("YOUR QUEST IS OVER.", glm::vec2(SCREEN_WIDTH / 2 - 210, SCREEN_HEIGHT / 2 + 40), 20, glm::vec4(1, 1, 1, 1));
		return;
	}

	else if (gameOver && !startMenu.showingMenu()) {
		glClearColor(0, 0, 0, 1.0f);
		text.render("GAME OVER", glm::vec2(SCREEN_WIDTH / 2 - 90, SCREEN_HEIGHT / 2), 20, glm::vec4(1, 1, 1, 1));
		Score::instance().render();
		return;
	}

	else if (changingScene && !startMenu.showingMenu()) {
		glClearColor(0, 0, 0, 1.0f);
		text.render("WORLD", glm::vec2(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2), 20, glm::vec4(1, 1, 1, 1));
		pair<int, int> world = Score::instance().getWorld();
		string str_world = to_string(world.first) + '-' + to_string(world.second);
		text.render(str_world, glm::vec2(SCREEN_WIDTH / 2 + 24, SCREEN_HEIGHT / 2), 20, glm::vec4(1, 1, 1, 1));
		Score::instance().render();
		return;
	}

	glm::mat4 modelview;
	texProgram.use();
	camera.bindProjection(texProgram);
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	/* Map */
	map->render(camera.getPosition(), camera.getSize(), texProgram);

	/* Player */
	if (!map->reachEntranceCastle(player->getPos())) {
		texProgram.use();
		player->render();
	}

	/* Score */
	Score::instance().render();

	/* Start Menu */
	startMenu.render();
}

void Scene::initShaders() {
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled()) {
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled()) {
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::checkWorldKeys() {
	bool world1 = Game::instance().getKey('1');
	bool world2 = Game::instance().getKey('2');

	if (world1 && map != maps[0]) {
		map = maps[0];
		Score::instance().updateWorld(1, 1);
		worldChange();
	}
	else if (world2 && map != maps[1]) {
		map = maps[1];
		Score::instance().updateWorld(2, 1);
		worldChange();
	}
}

void Scene::setGameOver() {
	sound.playSFX("sfx/game_over.wav");
	map = maps[0];
	map->restart();
	player->setTileMap(map);
	gameOver = true;
	timeGameOver = 0;
	Game::instance().clearInput();
}

void Scene::setChangingScene() {
	changingScene = true;
	timeChangingScene = 0;
}