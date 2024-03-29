#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"


void Game::init() {
    bPlay = true;
    state = GAME_INIT; // Iniciar en el menú
    glClearColor(0.0667f, 0.0667f, 0.0667f, 1.0f);
    initScreen.init();
    SoundManager* sound = new SoundManager();

    
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
        menu.init(&sound);
    }
    else if (key == GLFW_KEY_I) {
        state = GAME_INSTRUCTIONS;
        instructions.init();
    }
    else if (key == GLFW_KEY_6) {
        state = GAME_MENU;
        menu.init(&sound);
    }
    else if (state == GAME_MENU) {
        switch (key){
            case GLFW_KEY_1:                
                state = GAME_PLAYING;
                menu.stopSong();
                scene.init("levels/stage1-1.txt", "music/04. Stage 01 (Hong Kong).mp3", &sound);
                currentLevel = 1;
                break;
            case GLFW_KEY_2:               
                state = GAME_PLAYING;
                menu.stopSong();
                scene.init("levels/stage2-1.txt", "music/05. Stage 02-04 (Java).mp3", &sound);
                currentLevel = 2;
                break;
            case GLFW_KEY_3:
                state = GAME_PLAYING;
                menu.stopSong();
                scene.init("levels/stage2-1.txt", "music/06. Stage 05-07 (Silk Road (Xi'an)).mp3", &sound);
                currentLevel = 3;
                break;
        }

    }    
    else if (key == GLFW_KEY_ESCAPE) {
        // Salir del juego o volver al menú
        if (state == GAME_PLAYING) {
            state = GAME_MENU;
            sound.stopAllSounds();
            menu.init(&sound);
            
        }else if (state == GAME_INSTRUCTIONS) {
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


