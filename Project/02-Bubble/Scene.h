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
#include "Item.h"
#include "PlayerInterface.h"
#include "SoundManager.h"
#include "Text.h"

class Scene
{
public:
    Scene();
    ~Scene();

    void init(string level, const char* song, SoundManager* soundM);
    void update(int deltaTime);
    void render();
    void splitBall(int ballIndex);

    bool checkCollision(Ball* ball, Player* player);
    void reLoad(string level);
    void stopSong();
    void updateTimer(int deltaTime);
    void startTimer(float duration);
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
    std::vector<Item*> items;
    std::vector<Harpoon*> harpoons;

    Ball::BALL_SIZE lastBallSizeDestoyed;
    int comboCounter;
    SoundManager *sound;

    bool invencible, dinamite;
    int stopWatchCycles, invencibleCycles, dinamiteCycles;

    float timer;       // Mantiene la cantidad actual de tiempo transcurrido.
    float timerDuration; // Duración total que el temporizador debe correr.
    bool timerActive;  // Indica si el temporizador está activo.



};

#endif // _SCENE_INCLUDE