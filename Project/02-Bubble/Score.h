#ifndef _SCORE_INCLUDE
#define _SCORE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include <utility>
#include "TexturedQuad.h"
#include "Text.h"

using namespace std;

class Score {

public:

	static Score& instance() {
		static Score S;
		return S;
	}

	/* OpenGL */
	void init();
	void update(int deltaTime);
	void render();

	/* Score */
	void increaseScore(int x);
	void increaseCoins();
	void restart();
	void restartLives();
	bool gameOver();
	void restartTime();

	void updateWorld(int x, int y);
	void decreaseLive();
	double getTime();
	pair<int, int> getWorld() const;
	void timeToScore();

private:
	int score, lastScore;
	int coins;
	pair<int, int> world;
	double time;
	int lives;
	Text text;
};

#endif // _SCORE_INCLUDE