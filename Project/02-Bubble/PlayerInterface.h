// PlayerInterface.h
#ifndef _PLAYER_INTERFACE_INCLUDE
#define _PLAYER_INTERFACE_INCLUDE

#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "Item.h"

class PlayerInterface
{
public:
    PlayerInterface();
    ~PlayerInterface();

    void init(ShaderProgram & program);
    void update(int deltaTime);
    void render();

    void setLives(int l);
    void setItem(Item::ITEM_TYPE i);

private:
    void prepareLivesSprite();
    void prepareItemsSprite();

private:
    ShaderProgram program;
    Texture text;

    int lives;
    GLuint vaoLives;
    GLuint vboLives;
    GLint posLocationLive, texCoordLocationLive;
    glm::mat4 live1Pos, live2Pos, live3Pos;

    Item::ITEM_TYPE item;
    GLuint vaoItem;
    GLuint vboItem;
    GLint posLocationItem, texCoordLocationItem;
    glm::mat4 itemPos;
};

#endif // _PLAYER_INTERFACE_INCLUDE
