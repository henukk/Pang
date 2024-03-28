#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"


void Game::init() {
    levelspassed = 0;
    bPlay = true;
    state = GAME_INIT; // Iniciar en el menú
    glClearColor(0.0667f, 0.0667f, 0.0667f, 1.0f);
    initScreen.init();
    
}

bool Game::update(int deltaTime) {
    switch (state) {
    case GAME_INIT:
        break;
    case GAME_MENU:
        break;
    case GAME_INSTRUCTIONS:
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
        case GAME_INIT:
            initScreen.render();
            break;
        case GAME_MENU:
            menu.render();
            break;
        case GAME_INSTRUCTIONS:
            instructions.render();
            break;
        case GAME_PLAYING:
            scene.render();
            break;
        }
}

void Game::keyPressed(int key) {
    if (state == GAME_INIT && key == GLFW_KEY_ENTER) {
        state = GAME_MENU;
        menu.init();
    }
    else if (key == GLFW_KEY_I) {
        state = GAME_INSTRUCTIONS;
        instructions.init();
    }
    else if (key == GLFW_KEY_6) {
        state = GAME_MENU;
        menu.init();
    }
    else if (state == GAME_MENU) {
        switch (key){
            case GLFW_KEY_1:                
                state = GAME_PLAYING;
                scene.init("levels/stage1-1.txt");
                break;
            case GLFW_KEY_2:               
                state = GAME_PLAYING;
                scene.init("levels/stage2-1.txt");
                break;
            case GLFW_KEY_3:
                state = GAME_PLAYING;
                scene.init("levels/stage2-1.txt");
                break;
        }

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
}

void Game::levelPassed() {
    ++levelspassed;
}
