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
    for(int pushc=0; pushc<16; pushc++) {
        if ((players[0].getCoord().x+players[0].getCoord().w >= players[1].getCoord().x) &&
            (players[0].getCoord().x+players[0].getCoord().w <= players[1].getCoord().x+players[1].getCoord().w) &&
            (players[0].getCoord().y+players[0].getCoord().h >= players[1].getCoord().y) &&
            (players[0].getCoord().y+players[0].getCoord().h <= players[1].getCoord().y+players[1].getCoord().h))
            players[0].push(-1);
        else if ((players[0].getCoord().x <= players[1].getCoord().x+players[1].getCoord().w) &&
                (players[0].getCoord().x >= players[1].getCoord().x) &&
                (players[0].getCoord().y+players[0].getCoord().h >= players[1].getCoord().y) &&
                (players[0].getCoord().y+players[0].getCoord().h <= players[1].getCoord().y+players[1].getCoord().h))
            players[0].push(1);
        
        if ((players[1].getCoord().x+players[1].getCoord().w >= players[0].getCoord().x) &&
            (players[1].getCoord().x+players[1].getCoord().w <= players[0].getCoord().x+players[0].getCoord().w) &&
            (players[1].getCoord().y+players[1].getCoord().h >= players[0].getCoord().y) &&
            (players[1].getCoord().y+players[1].getCoord().h <= players[0].getCoord().y+players[0].getCoord().h))
            players[1].push(-1);
        else if ((players[1].getCoord().x <= players[0].getCoord().x+players[0].getCoord().w) &&
                (players[1].getCoord().x >= players[0].getCoord().x) &&
                (players[1].getCoord().y+players[1].getCoord().h >= players[0].getCoord().y) &&
                (players[1].getCoord().y+players[1].getCoord().h <= players[0].getCoord().y+players[0].getCoord().h))
            players[1].push(1);
    }
    
	stage.draw();
	for (int index=0; index<players.size(); index++)
		players[index].update();
    hud.draw();
    /*if (players[0].getHealth() == 0 && players[1].getHealth() == 0)
    re*/
}
