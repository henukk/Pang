#include "Menu.h"
#include <iostream>
#include <GL/glew.h>
// Incluye los headers necesarios para la gestión de shaders

Menu::Menu() : shaderProgram(0) {
}

Menu::~Menu() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // No olvides liberar recursos de shaders si es necesario
}



void Menu::setupShaders() {
    // Carga, compila y enlaza tus shaders aquí
    // shaderProgram = createShaderProgram("vertexShader.vs", "fragmentShader.fs");
}

void Menu::setupBackground() {
    // Carga la textura de fondo
    background.loadFromFile("./images/Start.png", TEXTURE_PIXEL_FORMAT_RGBA);
    background.setWrapS(GL_CLAMP_TO_EDGE);
    background.setWrapT(GL_CLAMP_TO_EDGE);
    background.setMinFilter(GL_NEAREST);
    background.setMagFilter(GL_NEAREST);

    // Prepara los vértices
    float vertices[] = {
        // Posiciones          // Coordenadas de Textura
        0.0f, 0.0f, 0.0f, 0.0f, // Esquina superior izquierda
        0.0f,  208.f, 0.0f, 1.0f, // Esquina inferior izquierda
        384.f,  208.f, 1.0f, 1.0f, // Esquina inferior derecha

        0.0f, 0.0f, 0.0f, 0.0f, // Esquina superior izquierda
        384.f, 208.f, 1.0f, 1.0f, // Esquina inferior derecha
        384.f, 0.0f, 1.0f, 0.0f // Esquina superior derecha
    };

    glGenVertexArrays(1, &vaoBackground);
    glBindVertexArray(vaoBackground);

    glGenBuffers(1, &vboBackground);
    glBindBuffer(GL_ARRAY_BUFFER, vboBackground);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Aquí asumo que tienes un objeto ShaderProgram llamado 'program'
    // que ya fue creado y configurado en algún lugar de tu clase.
    posLocationBackground = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
    texCoordLocationBackground = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void Menu::init() {
    setupBackground();
    // El resto de tu inicialización...
}


void Menu::update(int deltaTime) {
    // Lógica de actualización
}

void Menu::render() {
    glEnable(GL_TEXTURE_2D);

    background.use();
    glBindVertexArray(vaoBackground);
    glEnableVertexAttribArray(posLocationBackground);
    glEnableVertexAttribArray(texCoordLocationBackground);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Menu::keyPressed(int key) {
    // Manejo de la entrada del teclado
}

void Menu::keyReleased(int key) {
    // Manejo de la entrada del teclado
}
