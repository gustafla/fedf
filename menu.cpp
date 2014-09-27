#include "menu.hpp"
#include "gfxutil.hpp"
#include "util.hpp"
#include "config.hpp"
#include <cstdlib>

Menu::Menu(GameData* igameData, bool title):
gameData(igameData),
screenAt(CHARACTER_SCREEN) {
	file2surface("gfx/title.png", &titleScreen);
	file2surface("gfx/characterbg.png", &characterSelectBG);
	if (title)
		screenAt = TITLE_SCREEN;
}

Menu::~Menu() {
	SDL_FreeSurface(titleScreen);
	SDL_FreeSurface(characterSelectBG);
}

Game* Menu::update() {
	if (screenAt == TITLE_SCREEN) {
		SDL_BlitSurface(titleScreen, NULL, gameData->buffer, NULL);
		if (gameData->gameEvent.type == SDL_KEYDOWN)
			screenAt = CHARACTER_SCREEN;
	} else if (screenAt == CHARACTER_SCREEN) {
		SDL_BlitSurface(characterSelectBG, NULL, gameData->buffer, NULL);
		if (gameData->gameEvent.type == SDL_KEYDOWN)
			return new Game(
				gameData,
				Stage(gameData, "gfx/test.png"),
				Player(gameData, PLAYER1_CONTROLS, buildRect(20, 0, 64, 64), true, "gfx/playertest.png"),
				Player(gameData, PLAYER2_CONTROLS, buildRect(WIDTH-20-64, 0, 64, 64), false, "gfx/playertest.png")
			);
	}
	return NULL;
}
