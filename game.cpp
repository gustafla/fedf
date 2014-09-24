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

#include "game.hpp"
#include <SDL/SDL.h>
#include "game_data.hpp"
#include "player.hpp"
#include "stage.hpp"

Game::Game(GameData* igameData, Stage istage, Player player1, Player player2):
stage(istage),
gameData(igameData) {
    players.push_back(player1);
    players.push_back(player2);
    gameData->stageFloor = istage.getFloor();
    hud = Hud(igameData, &players[0], &players[1]);
}

void Game::update() {
	stage.draw();
	for (int index=0; index<players.size(); index++)
		players[index].update();
    hud.draw();
    players[0].takeDamage(1);
    players[1].takeDamage(1);
}
