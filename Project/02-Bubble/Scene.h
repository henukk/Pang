// Scene.h
#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Ball.h"
#include "Harpoon.h"
#include "Food.h"
#include "PlayerInterface.h"

class Scene
{
public:
    Scene();
    ~Scene();

    void init(string level);
    void update(int deltaTime);
    void render();
    void splitBall(int ballIndex);

    bool checkCollision(Ball* ball, Player* player);
    void reLoad(string level);
private:
    void initShaders();


private:
    PlayerInterface* playerInterface;

    string currentLevel;
    int score;
    int lives;
    TileMap* map;
    Player* player;
    Harpoon* harpoon;
    ShaderProgram texProgram;
    float currentTime;
    glm::mat4 projection;
    std::vector<Ball*> balls;
    std::vector<Food*> foods;

    Ball::BALL_SIZE lastBallSizeDestoyed;
    int comboCounter;
};

#endif // _SCENE_INCLUDE
