#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Ball.h"


#define SCREEN_X 48
#define SCREEN_Y 30

#define INIT_PLAYER_X_TILES 22
#define INIT_PLAYER_Y_TILES 21

#define INIT_BALL_X_TILES 2
#define INIT_BALL_Y_TILES 2


Scene::Scene()
{
	map = NULL;
	player = NULL;
	ball = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (ball != NULL)
		delete ball;
}


void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(0, 0), texProgram);

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * (SCREEN_WIDTH/SCREEN_X), INIT_PLAYER_Y_TILES * (SCREEN_HEIGHT / SCREEN_Y)));
	player->setTileMap(map);


	harpoon = new Harpoon();
	harpoon->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	harpoon->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), 0));
	harpoon->setTileMap(map);

	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, Ball::RED, Ball::SMALL);
	ball->setPosition(glm::vec2(INIT_BALL_X_TILES * (SCREEN_WIDTH / SCREEN_X), INIT_BALL_Y_TILES * (SCREEN_HEIGHT / SCREEN_Y)));
	ball->setTileMap(map);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	if (Game::instance().getKey(GLFW_KEY_SPACE)) {
		harpoon->shoot(player->getPosition());
	}
	currentTime += deltaTime;
	player->update(deltaTime);
	ball->update(deltaTime);
	if (harpoon->shooting())
		harpoon->update(deltaTime);
	else
		harpoon->setPosition(player->getPosition());
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	player->render();
	if (harpoon->shooting())
		harpoon->render();
	ball->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



