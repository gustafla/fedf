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

void Game::updateEffects() {
    for (int i=effects.size(); i>0; i--) {
        if (effects[i-1]->draw()) {
            delete effects[i-1];
            effects.erase(effects.begin()+(i-1));
        }
    }
}

void Game::playHitSound(unsigned int player) {
    gameData->sounds->play(HIT_SOUNDS[int((1.0-(players[player].getHealth()/100.0))*(NUM_HIT_SOUNDS-1))]);
}

void Game::doFightingCheck(unsigned int playerSelf, unsigned int playerOther) {
	if (players[playerSelf].getAttackStatus()) {
        if (players[playerSelf].lookingRight() ? 
               ((players[playerSelf].getCoord().x + players[playerSelf].getCoord().w + ATTACK_REACH) >= players[playerOther].getCoord().x) : 
               ((players[playerSelf].getCoord().x - ATTACK_REACH) <= players[playerOther].getCoord().x + players[playerOther].getCoord().w)
           )
            if (players[playerSelf].getCoord().y >= players[playerOther].getCoord().y && players[playerSelf].getCoord().y <= players[playerOther].getCoord().y + players[playerOther].getCoord().h)
                if (players[playerSelf].getCoord().y + players[playerSelf].getCoord().h >= players[playerOther].getCoord().y && players[playerSelf].getCoord().y + players[playerSelf].getCoord().h <= players[playerOther].getCoord().y + players[playerOther].getCoord().h) {
                    if (players[playerOther].isShielding()) {
                        effects.push_back(new Effect(gameData, "gfx/shield", !players[playerSelf].lookingRight(), buildRect((players[playerOther].getCoord().x-CHARACTER_HITBOX_SIZE/2)-gameData->screenRect.x, (players[playerOther].getCoord().y-CHARACTER_HITBOX_SIZE)-gameData->screenRect.y, 128, 128)));
                        players[playerOther].pause(14);
                    } else if (players[playerSelf].getAttackStatus() == 1) {
                        players[playerOther].takeDamage(PUNCH_DAMAGE);
                        playHitSound(playerOther);
                        effects.push_back(new Effect(gameData, "gfx/ef1", players[playerSelf].lookingRight(), (players[playerSelf].lookingRight() ? 
                                                                                                                  (buildRect(((players[playerSelf].getCoord().x + players[playerSelf].getCoord().w + ATTACK_REACH/2  - 32 -(rand()%10))-gameData->screenRect.x), (players[playerSelf].getCoord().y - gameData->screenRect.y)-players[playerSelf].getCoord().h+(rand()%20), 64, 64)) :
                                                                                                                  (buildRect(((players[playerSelf].getCoord().x - ATTACK_REACH/2 - 64 + 32 +(rand()%10))-gameData->screenRect.x), (players[playerSelf].getCoord().y - gameData->screenRect.y)-players[playerSelf].getCoord().h+(rand()%20), 64, 64))
                                                                                                              )
                                                    )
                                         );
                    }
                    else if (players[playerSelf].getAttackStatus() == 2) {
                        players[playerOther].takeDamage(KICK_DAMAGE);
                        players[playerOther].sendFlying(4*(players[playerSelf].lookingRight() ? 1 : -1), 4);
                    } else
                        players[playerOther].takeDamage(DEFAULT_DAMAGE);
                    
                }
        players[playerSelf].clearAttackStatus();
    }
}

void Game::update() {
    #ifdef DEBUG
        std::cout << "Game update\n";
    #endif
    
    if (gameData->gameFrame > START_DELAY-START_SHOW_DELAY) {
        if (!players[0].isActive() && players[0].getHealth() != 0 && !players[0].waiting()) {
            players[0].start();
        }
        if (!players[1].isActive() && players[1].getHealth() != 0 && !players[1].waiting()) {
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
			players[0].pause(WIN_DELAY+1);
			winner=2;
		}
		else if (players[1].getHealth() == 0) {
			players[1].stop();
			players[1].pause(WIN_DELAY+1);
			winner=1;
		}
        
        doFightingCheck();
        doFightingCheck(1, 0);
        
        updateEffects();
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
