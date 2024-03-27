// Scene.h
#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Ball.h"
#include "Harpoon.h"

class Scene
{
public:
    Scene();
    ~Scene();

    void init(string level);
    void update(int deltaTime);
    void render();
    void splitBall(int ballIndex);

private:
    void initShaders();

    

private:
    TileMap* map;
    Player* player;
    Harpoon* harpoon;
    Ball* ball;
    ShaderProgram texProgram;
    float currentTime;
    glm::mat4 projection;
    std::vector<Ball*> balls; // Vector para manejar múltiples pelotas
};

#endif // _SCENE_INCLUDE
