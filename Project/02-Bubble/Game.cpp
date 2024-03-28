#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"




void Game::init() {
    bPlay = true;
    state = GAME_MENU; // Iniciar en el menú
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    menu.init();

    
}

bool Game::update(int deltaTime) {
    switch (state) {
    case GAME_MENU:
        break;
    case GAME_PLAYING:
        scene.update(deltaTime);
        break;
    }

    return bPlay;
}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (state) {
    case GAME_MENU:
        menu.render();
        break;
    case GAME_PLAYING:
        scene.render();
        break;
    }
}

void Game::keyPressed(int key) {
    if (state == GAME_MENU && key == GLFW_KEY_ENTER) {
        // Por ejemplo, entrar a jugar presionando Enter en el menú
        state = GAME_PLAYING;
    }
    else if (key == GLFW_KEY_ESCAPE) {
        // Salir del juego o volver al menú
        if (state == GAME_PLAYING) {
            state = GAME_MENU;
        }
        else {
            bPlay = false;
        }
    }
    else if (state == GAME_MENU && key == GLFW_KEY_1) {
        // Por ejemplo, entrar a jugar presionando Enter en el menú
        state = GAME_PLAYING;
        scene.init("levels/stage1-1.txt");
    }
    else if (state == GAME_MENU && key == GLFW_KEY_2) {
        // Por ejemplo, entrar a jugar presionando Enter en el menú
        state = GAME_PLAYING;
        scene.init("levels/stage2-1.txt");
    }
    else if (state == GAME_MENU && key == GLFW_KEY_3) {
        // Por ejemplo, entrar a jugar presionando Enter en el menú
        state = GAME_PLAYING;
        scene.init("levels/stage2-1.txt");
    }



    keys[key] = true;
}

void Game::keyReleased(int key) {
    keys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
    return keys[key];
}

void Game::changeState(GameState newState) {
    state = newState;
    // Aquí podrías hacer otras operaciones necesarias cuando cambias de estado,
    // como pausar la música, cambiar las visualizaciones, etc.
}
