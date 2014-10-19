// Copyright 2014 Lauri Gustafsson, Kalle Korhonen and Julius Heino
/*
This file is part of Fedora Fighters.

    Fedora Fighters is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Fedora Fighters is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Fedora Fighters, see COPYING. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MENU_HPP
#define MENU_HPP

#include "game.hpp"
#include "player.hpp"
#include "stage.hpp"
#include "config.hpp"
#include <SDL/SDL.h>
#include "character.hpp"
#include <vector>
#include "text.hpp"

class Menu {
	private:
		GameData* gameData;
		SDL_Surface* titleScreen;
		SDL_Surface* characterSelectBG;
		SDL_Surface* stageSelectOverlay;
		SDL_Surface* ok;
        SDL_Surface* help;
        SDL_Surface* bg;
        SDL_Surface* logo;
		
		unsigned int playerCharacterSelection[2];
		unsigned int playerStageSelection;
		unsigned int playerCharacterDelay[2];
		unsigned int playerStageDelay;
		unsigned int playerStageIDelay;
		unsigned int fpsKeyDelay;
		Text* fpsText;
		bool playerCharacterDone[2];
		
		enum {TITLE_SCREEN, CHARACTER_SCREEN, STAGE_SCREEN} screenAt;
		
		std::vector<Character> characters;
		std::vector<Stage> stages;
		std::vector<Text> stageNames;
		std::vector<Text> characterNames1;
		std::vector<Text> characterNames2;
        
        void mapKeys(PlayerControls* toMap, unsigned int whose=1);
        
	public:
		Menu(GameData* igameData, bool title=false);
		~Menu();
		Game* update();
};

#endif
