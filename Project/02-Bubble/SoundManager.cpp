#include "SoundManager.h"
using namespace irrklang;

SoundManager::SoundManager() {
	music = NULL;
	sfx = NULL;

	// start the sound engine with default parameters
	soundEngine = createIrrKlangDevice();
}

SoundManager::~SoundManager() {
	if (music != NULL) delete music;
	if (sfx != NULL) delete sfx;
	soundEngine->drop(); // delete engine
}

void SoundManager::playBGM(const char* filename, bool playLooped) {
	music = soundEngine->play2D(filename, playLooped, false, true, ESM_AUTO_DETECT, true);
}

void SoundManager::stopBGM() {
	if (music != NULL) music->stop();
}

void SoundManager::pauseBGM() {
	if (music != NULL) music->setIsPaused(true);
}

void SoundManager::resumeBGM() {
	if (music != NULL) music->setIsPaused(false);
}

void SoundManager::playSFX(const char* filename) {
	sfx = soundEngine->play2D(filename);
}

void SoundManager::stopAllSounds() {
	soundEngine->stopAllSounds();
}