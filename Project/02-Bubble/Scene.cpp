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
	
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
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

	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, Ball::RED, Ball::HUGE);
	ball->setPosition(glm::vec2(INIT_BALL_X_TILES * (SCREEN_WIDTH / SCREEN_X), INIT_BALL_Y_TILES * (SCREEN_HEIGHT / SCREEN_Y)));
	ball->setTileMap(map);
	balls.push_back(ball);


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
	for (auto& ball : balls) {
		ball->update(deltaTime);
	}
	if (harpoon->shooting())
		harpoon->update(deltaTime);
	else
		harpoon->setPosition(player->getPosition());

	for (int i = 0; i < balls.size(); ++i) {
		if (harpoon->shooting() && balls[i]->isHitByHarpoon(*harpoon)) {
			splitBall(i);  // Pasar índice de la pelota que fue golpeada
			harpoon->kill();
			break;  // Asumiendo que un arpón solo puede golpear una pelota a la vez
		}
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

	// Crear dos nuevas instancias de Ball de tamaño MEDIUM
	Ball* newBall1 = new Ball();
	newBall1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, Ball::RED, Ball::MEDIUM);
	newBall1->setPosition(glm::vec2(pos.x - size.x / 4, pos.y));  // Ajustar según sea necesario

	Ball* newBall2 = new Ball();
	newBall2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, Ball::RED, Ball::MEDIUM);
	newBall2->setPosition(glm::vec2(pos.x + size.x / 4, pos.y));  // Ajustar según sea necesario

	// Eliminar la pelota golpeada y agregar las nuevas pelotas al vector
	delete hitBall;
	balls[ballIndex] = newBall1;  // Reemplazar la pelota golpeada por una de las nuevas
	balls.push_back(newBall2);  // Agregar la otra pelota al final del vector
}




