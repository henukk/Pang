#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Ball.h"
#include <random>


#define SCREEN_X 48
#define SCREEN_Y 30

#define INIT_PLAYER_X_TILES 2
#define INIT_PLAYER_Y_TILES 21

#define INIT_BALL_X_TILES 2
#define INIT_BALL_Y_TILES 2

#define STOP_WATCH_CYCLES 180;
#define DINAMITE_CYCLES 61

Scene::Scene()
{
	map = NULL;
	player = NULL;
	balls.clear();
	foods.clear();
	items.clear();
	comboCounter = 0;
	lastBallSizeDestoyed = Ball::NONE;
	std::srand(std::time(nullptr));
	playerInterface = NULL;
	stopWatchCycles = 0;
	invencible = false;
	invencibleCycles = 0;
	dinamite = false;
	dinamiteCycles = 0;
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
	for (Food* f : foods) {
		if (f != NULL)
			delete f;
	}
	for (Item* i : items) {
		if (i != NULL)
			delete i;
	}
	if (playerInterface != NULL) {
		delete playerInterface;
	}
}


void Scene::init(string level,const char* song, SoundManager* soundM)
{
	sound = soundM;
	map = NULL;
	player = NULL;
	balls.clear();
	foods.clear();
	items.clear();
	comboCounter = 0;
	lastBallSizeDestoyed = Ball::NONE;
	std::srand(std::time(nullptr));
	std::srand(std::time(nullptr));

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
	
	sound->stopAllSounds();
	sound->playBGM(song, true);

	playerInterface = new PlayerInterface();
	playerInterface->init(texProgram);
	playerInterface->setLives(lives);
	playerInterface->setItem(Item::NONE);

	stopWatchCycles = 0;
	invencible = false;
	invencibleCycles = 0;
	dinamite = false;
	dinamiteCycles = 0;

	startTimer(120.0f);
}

void Scene::update(int deltaTime)
{

	updateTimer(deltaTime);

	if (Game::instance().getKey(GLFW_KEY_A)) { //stop watch¡
		playerInterface->setItem(Item::STOP_WATCH);
		stopWatchCycles = STOP_WATCH_CYCLES;
	}
	if (Game::instance().getKey(GLFW_KEY_S)) { //god mode | invencible
		playerInterface->setItem(Item::INVENCIBLE);
		player->setInvencible(1);
		invencible = true;
		invencibleCycles = 60;
	}
	if (Game::instance().getKey(GLFW_KEY_D)) { //dinamite
		playerInterface->setItem(Item::DINAMITE);
		dinamite = true;
		dinamiteCycles = DINAMITE_CYCLES;
	}


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
			sound->playBGM("music/10 - Balloon Burst.mp3", false);
			splitBall(i);
			harpoon->kill();
			break;
		}
	}
	
	// STOP WATCH
	if(stopWatchCycles > 0) {
		stopWatchCycles--;
		if (stopWatchCycles == 0) {
			playerInterface->setItem(Item::NONE);
		}
	}

	// INVENCIBLE
	if (!invencible && invencibleCycles > 0) {
		invencibleCycles--;
		if (invencibleCycles == 0)
			player->setInvencible(0);
	}

	bool done = true, allSmall = true;
	for (int i = 0; i < balls.size(); ++i) {
		if (balls[i]->getStatus()) {
			done = false;
			if (dinamite && dinamiteCycles <= 60 && balls[i]->getType() == Ball::HUGE) {
				allSmall = false;
				sound->playBGM("music/10 - Balloon Burst.mp3", false);
				splitBall(i);
			}
			if (dinamite && dinamiteCycles <= 40 && balls[i]->getType() == Ball::BIG) {
				allSmall = false;
				sound->playBGM("music/10 - Balloon Burst.mp3", false);
				splitBall(i);
			}
			if (dinamite && dinamiteCycles <= 20 && balls[i]->getType() == Ball::MEDIUM) {
				allSmall = false;
				sound->playBGM("music/10 - Balloon Burst.mp3", false);
				splitBall(i);
			}
		}
		balls[i]->update(deltaTime, stopWatchCycles);
	}
	if (done) {
		cout << "Level done!!!" << endl;
	}
	if (dinamiteCycles == 0) {
		dinamite = false;
	}
	if (dinamite) {
		dinamiteCycles--;
	}

	for (auto& ball : balls) {
		if (ball->getStatus() && checkCollision(ball, player) && stopWatchCycles == 0) {
			if (invencible) {
				invencible = false;
				player->setInvencible(2);
			}
			else if(invencibleCycles == 0) {
				player->hit();
				--lives;
				playerInterface->setLives(lives);
				if (lives > 0)reLoad(currentLevel);
				else Game::instance().changeState(GAME_MENU);
				break;
			}
		}
	}

	for (auto& food : foods) {
		if (food->isAlive()) {
			food->update(deltaTime);
			int points = food->checkCollider(player->getPosition(), player->getSize());
			if (points > 0) {
				score += points;
				sound->playBGM("music/04 - Perfect Bonus.mp3", false);
				food->kill();
			}

		}
	}

	for (auto& item : items) {
		if (item->isAlive()) {
			item->update(deltaTime);
			Item::ITEM_TYPE actItemType = item->checkCollider(player->getPosition(), player->getSize());
			switch (actItemType)
			{
			case Item::STOP_WATCH:
				playerInterface->setItem(Item::STOP_WATCH);
				stopWatchCycles = STOP_WATCH_CYCLES;
				sound->playBGM("music/04 - Perfect Bonus.mp3", false);
				item->kill();
				break;
			case Item::INVENCIBLE:
				playerInterface->setItem(Item::INVENCIBLE);
				player->setInvencible(1);
				invencible = true;
				invencibleCycles = 60;
				sound->playBGM("music/04 - Perfect Bonus.mp3", false);
				item->kill();
				break;
			case Item::DINAMITE:
				playerInterface->setItem(Item::DINAMITE);
				dinamite = true;
				dinamiteCycles = DINAMITE_CYCLES;
				sound->playBGM("music/04 - Perfect Bonus.mp3", false);
				item->kill();
				break;
			case Item::NONE:
			default:
				break;
			}

		}
	}


	playerInterface->update(deltaTime);
	playerInterface->setScore(score);

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
	for (auto item : items) {
		item->render();
	}


	playerInterface->render();

}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
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





void Scene::splitBall(int ballIndex) {
	// Aqu� necesitas el �ndice de la pelota en el vector para dividirla
	Ball* hitBall = balls[ballIndex];
	glm::vec2 pos = hitBall->getPosition();
	glm::ivec2 size = hitBall->getSize();
	Ball::BALL_COLOR color = hitBall->getColor();
	Ball::BALL_SIZE type = hitBall->getType();
	int direction = hitBall->getDirection();

	hitBall->kill();
	if (lastBallSizeDestoyed == type) {
		comboCounter *= 2;
		if (comboCounter > 8)
			comboCounter = 8;
	}
	else comboCounter = 1;
	lastBallSizeDestoyed = type;

	//food generator
	int prob = rand() % 101;
	
	if (prob < 10) {
		int foodPos = 8 + rand() % (360 - 8 + 1);
		int foodType = rand() % 28;
		foods.push_back(new Food());
		foods.back()->init(foodType, glm::vec2(foodPos, 8), texProgram);
		foods.back()->setTileMap(map);
	}
	else if (prob >= 10 && prob < 30) {
		int itemTypeId = 1 + rand() % 3;
		Item::ITEM_TYPE itemType = Item::NONE;
		cout << itemTypeId << endl;
		switch (itemTypeId)
		{
		case 1:
			itemType = Item::STOP_WATCH;
			break;
		case 2:
			itemType = Item::INVENCIBLE;
			break;
		case 3:
			itemType = Item::DINAMITE;
		default:
			break;
		}
		items.push_back(new Item());
		items.back()->init(itemType, pos, texProgram);
		items.back()->setTileMap(map);
	}

	switch (type) {
	case Ball::HUGE:
		balls.push_back(new Ball());
		balls.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, color, Ball::BIG);
		balls.back()->setPosition(glm::vec2(glm::vec2(pos.x - size.x / 2, pos.y)));
		balls.back()->setTileMap(map);
		balls.back()->setDirection(-1);
		balls.back()->addForce(-10);

		balls.push_back(new Ball());
		balls.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, color, Ball::BIG);
		balls.back()->setPosition(glm::vec2(glm::vec2(pos.x + size.x / 2, pos.y)));
		balls.back()->setTileMap(map);
		balls.back()->addForce(-10);
		score += 50 * comboCounter;
		break;
	case Ball::BIG:
		balls.push_back(new Ball());
		balls.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, color, Ball::MEDIUM);
		balls.back()->setPosition(glm::vec2(glm::vec2(pos.x - size.x / 2, pos.y)));
		balls.back()->setTileMap(map);
		balls.back()->setDirection(-1);
		balls.back()->addForce(-10);

		balls.push_back(new Ball());
		balls.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, color, Ball::MEDIUM);
		balls.back()->setPosition(glm::vec2(glm::vec2(pos.x + size.x / 2, pos.y)));
		balls.back()->setTileMap(map);
		balls.back()->addForce(-10);
		score += 100 * comboCounter;
		break;
	case Ball::MEDIUM:
		balls.push_back(new Ball());
		balls.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, color, Ball::SMALL);
		balls.back()->setPosition(glm::vec2(glm::vec2(pos.x - size.x / 2, pos.y)));
		balls.back()->setTileMap(map);
		balls.back()->setDirection(-1);
		balls.back()->addForce(-10);

		balls.push_back(new Ball());
		balls.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, color, Ball::SMALL);
		balls.back()->setPosition(glm::vec2(glm::vec2(pos.x + size.x / 2, pos.y)));
		balls.back()->setTileMap(map);
		balls.back()->addForce(-10);
		score += 150 * comboCounter;
		break;
	case Ball::SMALL:
		score += 200 * comboCounter;
	default:
		break;
	}

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
	std::srand(std::time(nullptr));

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
	foods.clear();

	for (Item* i : items) {
		if (i != NULL) {
			delete i;
		}
	}
	items.clear();

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

	balls.push_back(new Ball());
	balls.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, Ball::RED, Ball::HUGE);
	balls.back()->setPosition(glm::vec2(INIT_BALL_X_TILES * (SCREEN_WIDTH / SCREEN_X), INIT_BALL_Y_TILES * (SCREEN_HEIGHT / SCREEN_Y)));
	balls.back()->setTileMap(map);

	// Reinicia las proyecciones y cualquier otro estado que necesites
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);

	// Aseg�rate de reiniciar cualquier estado del juego como el tiempo, la puntuaci�n, las vidas, etc.
	currentTime = 0.0f;
	// Reinicia otras variables de estado si es necesario...

	comboCounter = 0;
	lastBallSizeDestoyed = Ball::NONE;

	stopWatchCycles = 0;
	invencible = false;
	invencibleCycles = 0;
	dinamite = false;
	score = 0;
	dinamiteCycles = 0;
}

void Scene::stopSong() {
	sound->stopBGM();
}


void Scene::updateTimer(int deltaTime) {
	if (timerActive) {
		timer += deltaTime / 1000.0f; // Convertir de milisegundos a segundos si es necesario.
		if (timer >= timerDuration) {
			timerActive = false;
			timer = timerDuration;
			// Aquí se coloca el código que se ejecuta cuando el temporizador llega a cero.
		}
	}

	playerInterface->setTime(timerDuration -timer);
}

void Scene::startTimer(float duration) {
	timerDuration = duration;
	timer = 0;
	timerActive = true;
}



