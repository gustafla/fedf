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
#include <iostream>
#include "game_data.hpp"
#include "player.hpp"
#include "stage.hpp"

Game::Game(GameData* igameData, Stage istage, Player player1, Player player2):
stage(istage),
wintimer(WIN_DELAY),
winner(0),
gameData(igameData) {
    #ifdef DEBUG
        std::cout << "Game constructor\n";
    #endif
    players.push_back(player1);
    players.push_back(player2);
    gameData->stageFloor = istage.getFloor();
    hud = Hud(igameData, &players[0], &players[1]);
    igameData->gameFrame = 0;
    #ifdef DEBUG
        std::cout << "Game constructor finish\n";
    #endif
}

Game::~Game() {
    stage.clean();
    players[0].clean();
    players[1].clean();
    hud.clean();
}

void Game::update() {
    #ifdef DEBUG
        std::cout << "Game update\n";
    #endif
    
    if (gameData->gameFrame > START_DELAY-START_SHOW_DELAY) {
        if (!players[0].isActive() && players[0].getHealth() != 0) {
            players[0].start();
        }
        if (!players[1].isActive() && players[1].getHealth() != 0) {
            players[1].start();
        }
    };
    
    
    
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
    players[0].update();
    players[1].update();
    hud.draw(winner);
    
    #ifdef DEBUG
        std::cout << "Game update checking if dead\n";
    #endif
    if (players[0].getHealth() == 0) {
        players[0].stop();
        winner=2;
    }
    else if (players[1].getHealth() == 0) {
        players[1].stop();
        winner=1;
    }
    
    #ifdef DEBUG
        std::cout << "Game update damage test\n";
    #endif
    players[1].takeDamage(1);
    
    gameData->gameFrame++;
    
    #ifdef DEBUG
        std::cout << "Game update finish\n";
    #endif
}

bool Game::isFinished() {
    if (winner != 0)
        if (wintimer <= 0)
            return true;
        else
            wintimer--;
    return false;
}
