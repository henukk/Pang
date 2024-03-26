// Scene.h
#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Ball.h"
#include "Harpoon.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

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
    Ball* ball;  // La pelota original; probablemente no sea necesaria si usas el vector 'balls'
    Harpoon* harpoon;
    ShaderProgram texProgram;
    float currentTime;
    glm::mat4 projection;
    std::vector<Ball*> balls; // Vector para manejar m�ltiples pelotas
};

#endif // _SCENE_INCLUDE
