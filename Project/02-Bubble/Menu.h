#include "Texture.h"
#include "ShaderProgram.h"

class Menu {
public:
    Menu();
    virtual ~Menu();

    void init();
    void update(int deltaTime);
    void render();
    void keyPressed(int key);
    void keyReleased(int key);
    void setupBackground();
    void setupShaders();


private:
    unsigned int VBO, VAO;
    unsigned int shaderProgram; // ID del programa de shader

    
    Texture background; // Ya deberías tener esto para la textura de fondo
    unsigned int vaoBackground, vboBackground;
    GLint posLocationBackground, texCoordLocationBackground;

    ShaderProgram program;

};
