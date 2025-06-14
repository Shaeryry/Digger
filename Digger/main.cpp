#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif


#include "Minigin.h"
#include "Scene.h"
#include "DiggerGame.h"
#include "ServiceLocator.h"
#include "SDLMixerSoundSystem.h"

#include "ResourceManager.h"
#include "DiggerConstants.h"


int main(int, char* []) {
	DiggerGame digger(DIGGER::SCREEN_WIDTH, DIGGER::SCREEN_HEIGHT);
	return 0;
} 