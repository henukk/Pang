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
	foods.clear();
	comboCounter = 0;
	lastBallSizeDestoyed = Ball::NONE;
}

Scene::~Scene()
{
	if(map != NULL)delete map;
	if(player != NULL)delete player;
	for (Ball* b : balls) {
		if (b != NULL)
			delete b;
	}
	for (Food* f : foods) {
		if (f != NULL)
			delete f;
	}
}


void Scene::init(string level)
{


	currentLevel = level;
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
	lives = 3;
	score = 0;
	comboCounter = 0;
	lastBallSizeDestoyed = Ball::NONE;
	if (!text.init("fonts/OpenSans-Regular.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
	sound.playBGM("music/Stage1-1.mp3", true);
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
			done = false;
		}
		ball->update(deltaTime);
	}
	if (done) {
		cout << "eiow" << endl;
	}

	for (auto& ball : balls) {
		if (ball->getStatus() && checkCollision(ball, player)) {
			player->hit();
			--lives;
			if(lives > 0)reLoad(currentLevel);
			else Game::instance().changeState(GAME_MENU);
			break;
		}
	}

	for (auto& food : foods) {
		if (food->isAlive()) {
			food->update(deltaTime);
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
	for (auto food : foods) {
		food->render();
	}

	text.render("Vid!!!", glm::vec2(SCREEN_WIDTH / 2 - 170, SCREEN_HEIGHT / 2 + 10), 12, glm::vec4(0, 0, 1, 1));
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
	// Aqu� necesitas el �ndice de la pelota en el vector para dividirla
	Ball* hitBall = balls[ballIndex];
	glm::vec2 pos = hitBall->getPosition();
	glm::ivec2 size = hitBall->getSize();
	Ball::BALL_COLOR color = hitBall->getColor();
	Ball::BALL_SIZE type = hitBall->getType();
	int direction = hitBall->getDirection();

	if (lastBallSizeDestoyed == type) {
		comboCounter *= 2;
		if (comboCounter > 8)
			comboCounter = 8;
	}
	else comboCounter = 1;
	lastBallSizeDestoyed = type;

	static int eiow = 0;
	foods.push_back(new Food());
	foods.back()->init(eiow++, &score, pos, texProgram);
	foods.back()->setTileMap(map);

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
		score += 50 * comboCounter;
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
		score += 100 * comboCounter;
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
		score += 150 * comboCounter;
		break;
	case Ball::SMALL:
		score += 200 * comboCounter;
	default:
		break;
	}

	hitBall->kill();
}


bool Scene::checkCollision(Ball* ball, Player* player) {
	// Calcula los bordes del jugador y la pelota
	glm::vec2 playerPos = player->getPosition();
	glm::ivec2 playerSize = player->getSize();
	glm::vec2 ballPos = ball->getPosition();
	glm::ivec2 ballSize = ball->getSize();

	// Comprueba si hay colisi�n (esto es una comprobaci�n de AABB - Axis Aligned Bounding Box)
	bool collisionX = playerPos.x + playerSize.x >= ballPos.x &&
		ballPos.x + ballSize.x >= playerPos.x;
	bool collisionY = playerPos.y + playerSize.y >= ballPos.y &&
		ballPos.y + ballSize.y >= playerPos.y;

	// Si hay colisi�n en ambas dimensiones, entonces hay una colisi�n
	return collisionX && collisionY;
}



void Scene::reLoad(string level) {
	// Primero, limpiar los recursos existentes
	if (map != NULL) {
		delete map;
		map = NULL;
	}
	if (player != NULL) {
		delete player;
		player = NULL;
	}
	for (Ball* b : balls) {
		if (b != NULL) {
			delete b;
		}
	}
	balls.clear();
	for (Food* f : foods) {
		if (f != NULL) {
			delete f;
		}
	}

	if (harpoon != NULL) {
		delete harpoon;
		harpoon = NULL;
	}

	initShaders();
	map = TileMap::createTileMap(level, glm::vec2(0, 0), texProgram);

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * (SCREEN_WIDTH / SCREEN_X), INIT_PLAYER_Y_TILES * (SCREEN_HEIGHT / SCREEN_Y)));
	player->setTileMap(map);

	harpoon = new Harpoon();
	harpoon->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	harpoon->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), 0));
	harpoon->setTileMap(map);

	// Si los niveles tienen diferente n�mero de pelotas, deber�as cargarlas seg�n la configuraci�n del nivel.
	// Esto es un ejemplo para una sola pelota, pero podr�as tener una funci�n para cargar seg�n el nivel
	balls.push_back(new Ball());

	balls.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, Ball::RED, Ball::HUGE);
	balls.back()->setPosition(glm::vec2(INIT_BALL_X_TILES * (SCREEN_WIDTH / SCREEN_X), INIT_BALL_Y_TILES * (SCREEN_HEIGHT / SCREEN_Y)));
	balls.back()->setTileMap(map);

	// Reinicia las proyecciones y cualquier otro estado que necesites
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);



	currentTime = 0.0f;
	// Reinicia otras variables de estado si es necesario...

	lives = 3;
	score = 0;
	comboCounter = 0;
	lastBallSizeDestoyed = Ball::NONE;
}



