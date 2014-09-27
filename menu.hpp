#ifndef MENU_HPP
#define MENU_HPP

#include "game.hpp"
#include "player.hpp"
#include "stage.hpp"
#include "config.hpp"
#include <SDL/SDL.h>

class Menu {
	private:
		GameData* gameData;
		SDL_Surface* titleScreen;
		SDL_Surface* characterSelectBG;
		SDL_Surface* ok;
		
		unsigned int playerCharacterSelection[2];
		unsigned int playerCharacterDelay[2];
		bool playerCharacterDone[2];
		
		enum {TITLE_SCREEN, CHARACTER_SCREEN, STAGE_SCREEN} screenAt;
	public:
		Menu(GameData* igameData, bool title=false);
		~Menu();
		Game* update();
};

#endif
