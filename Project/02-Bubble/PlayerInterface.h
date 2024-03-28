// PlayerInterface.h
#ifndef _PLAYER_INTERFACE_INCLUDE
#define _PLAYER_INTERFACE_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"

class PlayerInterface
{
public:
    PlayerInterface();
    ~PlayerInterface();

    void init();
    void update(int deltaTime);
    void render();

private:

private:
    
};

#endif // _PLAYER_INTERFACE_INCLUDE
