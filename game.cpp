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
#include "gfxutil.hpp"
#include "util.hpp"

Game::Game(GameData* igameData, Stage istage, Player player1, Player player2):
stage(istage),
wintimer(WIN_DELAY),
winner(0),
pauseKeyDelay(0),
wantExit(false),
paused(false),
gameData(igameData) {
    #ifdef DEBUG
        std::cout << "Game constructor\n";
    #endif
    players.push_back(player1);
    players.push_back(player2);
    gameData->stageFloor = istage.getFloor();
    gameData->stageHeight = istage.getBG()->h;
    #ifdef DEBUG
        std::cout << "Stage height is: " << gameData->stageHeight << std::endl;
    #endif
    hud = new Hud(igameData, &players[0], &players[1]);
    igameData->gameFrame = 0;
    file2surface("gfx/paused.png", &pausedSceen);
    
    Mix_PlayMusic(gameData->music, 0);
    Mix_FreeMusic(gameData->music);
    gameData->music = Mix_LoadMUS((STAGES_DIR+stage.getName()+STAGE_MUSIC_FILE).c_str());
	Mix_PlayMusic(gameData->music, -1);
	gameData->musicPlaying = true;
    Mix_VolumeMusic(MIX_MAX_VOLUME);
    
    #ifdef DEBUG
        std::cout << "Game constructor finish\n";
    #endif
}

Game::~Game() {
    stage.clean();
    players[0].clean();
    players[1].clean();
    hud->clean();
    delete hud;
    SDL_FreeSurface(pausedSceen);
    gameData->musicPlaying = false;
    Mix_PlayMusic(gameData->music, 0);
    Mix_FreeMusic(gameData->music);
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
    
    if (pauseKeyDelay != 0)
		pauseKeyDelay--;
    
    if (paused) {
		SDL_BlitSurface(pausedSceen, NULL, gameData->buffer, NULL);
        if (!gameData->inTransition) {
            if (pauseKeyDelay == 0 && gameData->keystate[PAUSE_KEY]) {
                pauseKeyDelay = PAUSE_KEY_DELAY;
                paused = false;
                Mix_VolumeMusic(MIX_MAX_VOLUME);
            }
            if (gameData->keystate[CONFIRM_KEY])
                wantExit = true;
        }
	} else {
		
		if (pauseKeyDelay == 0 && gameData->keystate[PAUSE_KEY]) {
			paused = true;
			pauseKeyDelay = PAUSE_KEY_DELAY;
            Mix_VolumeMusic(20);
		}
		
        if (players[0].getCoord().x > players[1].getCoord().x) {
            players[0].lookRight(false);
            players[1].lookRight(true);
        } else {
            players[0].lookRight(true);
            players[1].lookRight(false);
        }
        
        unsigned int plc, prc;
        if (players[0].lookingRight()) {
            plc = players[0].getCoord().x;
            prc = players[1].getCoord().x + players[1].getCoord().w;
        } else {
            plc = players[1].getCoord().x;
            prc = players[0].getCoord().x + players[0].getCoord().w;
        }
        int screenRectx = ((plc + prc)/2)-(WIDTH/2);
        if (screenRectx < 0)
            screenRectx = 0;
        if (screenRectx > stage.getBG()->w - WIDTH)
            screenRectx = stage.getBG()->w - WIDTH;
        int screenRecty = ((players[0].getCoord().y + players[1].getCoord().y)/2)-(HEIGHT/2);
        if (screenRecty < 0)
            screenRecty = 0;
        if (screenRecty > stage.getBG()->h - HEIGHT)
            screenRecty = stage.getBG()->h - HEIGHT;
        gameData->screenRect = buildRect(screenRectx, screenRecty, WIDTH, HEIGHT);
        
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
		players[0].drawShadow();
		players[1].drawShadow();
		players[0].draw();
		players[1].draw();
		hud->draw(winner);
		
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

		gameData->gameFrame++;
	}
    
    #ifdef DEBUG
        std::cout << "Game update finish\n";
    #endif
}

bool Game::isFinished() {
	if (!gameData->inTransition && wantExit)
		gameData->inTransition = true;
    if (!wantExit && winner != 0) {
        if (wintimer <= 0)
            wantExit = true;
        else
            wintimer--;
    }
    if (gameData->inTransition) {
        if (gameData->drawTransition())
            return true;
    }
    return false;
}
