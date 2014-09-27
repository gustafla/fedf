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

#ifndef GAME_HPP
#define GAME_HPP

#include <SDL/SDL.h>
#include <vector>
#include "game_data.hpp"
#include "player.hpp"
#include "stage.hpp"
#include "hud.hpp"

class Game {
	private:
		GameData* gameData;
		std::vector<Player> players;
		Stage stage;
		Hud hud;
		unsigned int winner;
		unsigned int wintimer;
        unsigned int starttimer;
        bool paused;
        bool wantExit;
        unsigned int pauseKeyDelay;
        SDL_Surface* pausedSceen;
	public:
		Game(GameData* igameData, Stage istage, Player player1, Player player2/*, unsigned int timer*/);
		~Game();
		void update();
        bool isFinished();
};

#endif
