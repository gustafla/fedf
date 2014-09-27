#ifndef MENU_HPP
#define MENU_HPP

#include "game.hpp"
#include "player.hpp"
#include "stage.hpp"
#include <SDL/SDL.h>

class Menu {
	private:
		GameData* gameData;
		SDL_Surface* titleScreen;
		SDL_Surface* characterSelectBG;
		enum {TITLE_SCREEN, CHARACTER_SCREEN, STAGE_SCREEN} screenAt;
	public:
		Menu(GameData* igameData, bool title=false);
		~Menu();
		Game* update();
};

#endif
