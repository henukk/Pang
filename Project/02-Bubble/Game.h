#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include <GLFW/glfw3.h>
#include "Scene.h"
#include "Menu.h"
#include "InitScreen.h"
#include "Instructions.h"

#define SCREEN_WIDTH (48*8)
#define SCREEN_HEIGHT (30*8)


// Game is a singleton (a class with a single instance) that represents our whole application
enum GameState {
	GAME_INIT,
	GAME_MENU,
	GAME_INSTRUCTIONS,
	GAME_PLAYING, 
	CREDITS
};


class Game
{

private:
	Game() {}
	
public:
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

	bool getKey(int key) const;

	void changeState(GameState newState);

	void levelPassed();

private:
	bool bPlay; // Continue to play game?
	bool keys[GLFW_KEY_LAST+1]; // Store key states so that 
							    // we can have access at any time
	Scene scene;
	GameState state;
	Menu menu;
	string level;
	InitScreen initScreen;
	Instructions instructions;
	int levelspassed;
};

#endif // _GAME_INCLUDE


