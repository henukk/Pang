#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Ball.h"


#define SCREEN_X 48
#define SCREEN_Y 30

#define INIT_PLAYER_X_TILES 2
#define INIT_PLAYER_Y_TILES 21

#define INIT_BALL_X_TILES 2
#define INIT_BALL_Y_TILES 2


Scene::Scene()
{
	map = NULL;
	player = NULL;
	balls.clear();
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	for (Ball* b : balls) {
		if (b != NULL)
			delete b;
	}
}


void Scene::init(string level)
{
	initShaders();
	map = TileMap::createTileMap(level, glm::vec2(0, 0), texProgram);

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * (SCREEN_WIDTH/SCREEN_X), INIT_PLAYER_Y_TILES * (SCREEN_HEIGHT / SCREEN_Y)));
	player->setTileMap(map);


	harpoon = new Harpoon();
	harpoon->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	harpoon->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), 0));
	harpoon->setTileMap(map);

	balls.push_back(new Ball());
	balls.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, Ball::RED, Ball::HUGE);
	balls.back()->setPosition(glm::vec2(INIT_BALL_X_TILES * (SCREEN_WIDTH / SCREEN_X), INIT_BALL_Y_TILES * (SCREEN_HEIGHT / SCREEN_Y)));
	balls.back()->setTileMap(map);


	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	if (Game::instance().getKey(GLFW_KEY_SPACE)) {
		harpoon->shoot(player->getPosition());
	}
	if (harpoon->shooting())
		harpoon->update(deltaTime);
	else
		harpoon->setPosition(player->getPosition());
	for (int i = 0; i < balls.size(); ++i) {
		if (harpoon->shooting() && balls[i]->getStatus() && balls[i]->isHitByHarpoon(*harpoon)) {
			splitBall(i);
			harpoon->kill();
			break;
		}
	}
	bool done = true;
	for (auto& ball : balls) {
		if (ball->getStatus()) {
			ball->update(deltaTime);
			done = false;
		}
	}
	if (done) {
		//code to end level
		cout << "eiow" << endl;
	}

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
	for (auto ball : balls) {
        ball->render();
    }

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

void Scene::splitBall(int ballIndex) {
	// Aquí necesitas el índice de la pelota en el vector para dividirla
	Ball* hitBall = balls[ballIndex];
	glm::vec2 pos = hitBall->getPosition();
	glm::ivec2 size = hitBall->getSize();
	Ball::BALL_COLOR color = hitBall->getColor();
	Ball::BALL_SIZE type = hitBall->getType();
	int direction = hitBall->getDirection();

	switch (type) {
	case Ball::HUGE:
		balls.push_back(new Ball());
		balls.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, color, Ball::BIG);
		balls.back()->setPosition(glm::vec2(glm::vec2(pos.x - size.x / 2, pos.y)));
		balls.back()->setTileMap(map);
		balls.back()->setDirection(-1);

		balls.push_back(new Ball());
		balls.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, color, Ball::BIG);
		balls.back()->setPosition(glm::vec2(glm::vec2(pos.x + size.x / 2, pos.y)));
		balls.back()->setTileMap(map);
		break;
	case Ball::BIG:
		balls.push_back(new Ball());
		balls.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, color, Ball::MEDIUM);
		balls.back()->setPosition(glm::vec2(glm::vec2(pos.x - size.x / 2, pos.y)));
		balls.back()->setTileMap(map);
		balls.back()->setDirection(-1);

		balls.push_back(new Ball());
		balls.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, color, Ball::MEDIUM);
		balls.back()->setPosition(glm::vec2(glm::vec2(pos.x + size.x / 2, pos.y)));
		balls.back()->setTileMap(map);
		break;
	case Ball::MEDIUM:
		balls.push_back(new Ball());
		balls.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, color, Ball::SMALL);
		balls.back()->setPosition(glm::vec2(glm::vec2(pos.x - size.x / 2, pos.y)));
		balls.back()->setTileMap(map);
		balls.back()->setDirection(-1);

		balls.push_back(new Ball());
		balls.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, color, Ball::SMALL);
		balls.back()->setPosition(glm::vec2(glm::vec2(pos.x + size.x / 2, pos.y)));
		balls.back()->setTileMap(map);
		break;
	case Ball::SMALL:
	default:
		break;
	}

	hitBall->kill();
}




