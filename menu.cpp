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

#include "menu.hpp"
#include "gfxutil.hpp"
#include "util.hpp"
#include "player_controls.hpp"
#include "config.hpp"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <SDL/SDL.h>

Menu::Menu(GameData* igameData, bool title):
gameData(igameData),
screenAt(CHARACTER_SCREEN) {
	
	playerCharacterSelection[0]=0;
	playerCharacterSelection[1]=0;
	playerStageSelection=0;
	playerCharacterDone[0]=false;
	playerCharacterDone[1]=false; 
	playerCharacterDelay[0]=0; 
	playerCharacterDelay[1]=0; 
	playerStageDelay=0; 
	playerStageIDelay=60; 
	
	file2surface("gfx/title.png", &titleScreen);
	file2surface("gfx/characterbg.png", &characterSelectBG);
	file2surface("gfx/stageseloverlay.png", &stageSelectOverlay);
	file2surface("gfx/ok.png", &ok);
	file2surface("gfx/help.png", &help);
	file2surface("gfx/bg.png", &bg);
	
	if (title)
		screenAt = TITLE_SCREEN;
		
	std::vector<std::string> characterDirList;
    if (listDir(CHARACTERS_DIR, &characterDirList)) {
		for (int i=0; i<characterDirList.size(); i++) {
			characters.push_back(Character(igameData, characterDirList[i], CHARACTERS_DIR+characterDirList[i]));
		}
	} else {
		std::cout << "Something went wrong parsing characters.\n";
		exit(-4);
	}
	
	std::vector<std::string> stageDirList;
    if (listDir(STAGES_DIR, &stageDirList)) {
		for (int i=0; i<stageDirList.size(); i++) {
			stages.push_back(Stage(igameData, stageDirList[i], STAGES_DIR+stageDirList[i]));
		}
	} else {
		std::cout << "Something went wrong parsing stages.\n";
		exit(-5);
	}
}

Menu::~Menu() {
	SDL_FreeSurface(titleScreen);
	SDL_FreeSurface(characterSelectBG);
	SDL_FreeSurface(stageSelectOverlay);
	SDL_FreeSurface(ok);
}

Game* Menu::update() {
	if (gameData->gameEvent.type == SDL_KEYDOWN)
		{
			if (gameData->gameEvent.key.keysym.sym == SDLK_ESCAPE)
				gameData->running = false;
		}
	if (gameData->keystate[SDLK_F1])
        SDL_BlitSurface(help, NULL, gameData->buffer, NULL);
    else {
        switch (screenAt) {
            case TITLE_SCREEN: {
                SDL_BlitSurface(titleScreen, NULL, gameData->buffer, NULL);
                if (!gameData->inTransition && gameData->gameEvent.type == SDL_KEYDOWN) {
                    gameData->inTransition=true;
                    #ifdef DEBUG
                        std::cout << "Key pressed in title screen.\n";
                    #endif
                }
                if (gameData->inTransition) {
                    #ifdef DEBUG
                        std::cout << "Trying to draw transition.\n";
                    #endif
                    if(gameData->drawTransition())
                        screenAt = CHARACTER_SCREEN;
                }
            } break; case CHARACTER_SCREEN: {
                
                SDL_Rect tpcoord[2] = {buildRect(WIDTH/4-64, HEIGHT/2-64, 0, 0), buildRect((WIDTH/4)*3-64, HEIGHT/2-64, 0, 0)};
                SDL_Rect okcoord[2] = {buildRect(WIDTH/4-32, (HEIGHT/4)*3, 0, 0), buildRect((WIDTH/4)*3-32, (HEIGHT/4)*3, 0, 0)};
                PlayerControls controls[2] = {PLAYER1_CONTROLS, PLAYER2_CONTROLS};
                
                SDL_BlitSurface(characterSelectBG, NULL, gameData->buffer, NULL);
                
                for (int pl=0; pl<2; pl++) {
                    if (playerCharacterDelay[pl] != 0)
                        playerCharacterDelay[pl]--;
            
                    SDL_BlitSurface(characters[playerCharacterSelection[pl]].pic, NULL, gameData->buffer, &tpcoord[pl]);
        
                    if (playerCharacterDone[pl])
                        SDL_BlitSurface(ok, NULL, gameData->buffer, &okcoord[pl]);
                        
                    if (!gameData->inTransition) {
                        if (gameData->keystate[controls[pl].LEFT] && !playerCharacterDone[pl] && playerCharacterDelay[pl]==0) {
                            if (playerCharacterSelection[pl] == 0)
                                playerCharacterSelection[pl] = characters.size()-1;
                            else
                                playerCharacterSelection[pl]--;
                            playerCharacterDelay[pl] = CHARACTER_SELECT_DELAY;
                        }
                        if (gameData->keystate[controls[pl].RIGHT] && !playerCharacterDone[pl] && playerCharacterDelay[pl]==0) {
                            if (playerCharacterSelection[pl] == characters.size()-1)
                                playerCharacterSelection[pl] = 0;
                            else
                                playerCharacterSelection[pl]++;
                            playerCharacterDelay[pl] = CHARACTER_SELECT_DELAY;
                        }
                        if (gameData->keystate[controls[pl].A])
                            playerCharacterDone[pl] = true;
                        if (gameData->keystate[controls[pl].B])
                            playerCharacterDone[pl] = false;
                    }
                }
                
                if (!gameData->inTransition && playerCharacterDone[0] && playerCharacterDone[1]) {
                    gameData->inTransition=true;
                }
                
                if (gameData->inTransition) {
                    if (gameData->drawTransition()) {
                        screenAt = STAGE_SCREEN;
                        playerStageIDelay = 60;
                    }
                }
            } break; case STAGE_SCREEN: {
                SDL_Rect stagesrc = buildRect((sin(gameData->frame/120.0)*0.5+0.5)*(stages[playerStageSelection].getBG()->w - WIDTH), stages[playerStageSelection].getBG()->h - HEIGHT, WIDTH, HEIGHT);
                SDL_BlitSurface(stages[playerStageSelection].getBG(), &stagesrc, gameData->buffer, NULL);
                SDL_BlitSurface(stageSelectOverlay, NULL, gameData->buffer, NULL);
                
                if (playerStageDelay != 0)
                    playerStageDelay--;
                if (playerStageIDelay != 0)
                    playerStageIDelay--;
                if (!gameData->inTransition) {
                    if (gameData->keystate[PLAYER1_CONTROLS.LEFT] && playerStageDelay == 0) {
                            if (playerStageSelection == 0)
                                playerStageSelection = stages.size()-1;
                            else
                                playerStageSelection--;
                            playerStageDelay = STAGE_SELECT_DELAY;
                    }
                    if (gameData->keystate[PLAYER1_CONTROLS.RIGHT] && playerStageDelay == 0) {
                            if (playerStageSelection == stages.size()-1)
                                playerStageSelection = 0;
                            else
                                playerStageSelection++;
                            playerStageDelay = STAGE_SELECT_DELAY;
                    }
                    
                    if (gameData->keystate[PLAYER1_CONTROLS.B]) {
                        screenAt = CHARACTER_SCREEN;
                        playerCharacterDone[0] = false;
                    }
                    if (gameData->keystate[PLAYER2_CONTROLS.B]) {
                        screenAt = CHARACTER_SCREEN;
                        playerCharacterDone[1] = false;
                    }
                    
                    if (gameData->keystate[PLAYER1_CONTROLS.A] && playerStageIDelay == 0)
                        gameData->inTransition=true;
                }
                
                if (gameData->inTransition)
                    if (gameData->drawTransition())
                        return new Game(
                            gameData,
                            stages[playerStageSelection],
                            Player(gameData, PLAYER1_CONTROLS, buildRect(40, 0, CHARACTER_HITBOX_SIZE, CHARACTER_HITBOX_SIZE), true, characters[playerCharacterSelection[0]]),
                            Player(gameData, PLAYER2_CONTROLS, buildRect(WIDTH-40-CHARACTER_HITBOX_SIZE, 0, CHARACTER_HITBOX_SIZE, CHARACTER_HITBOX_SIZE), false, characters[playerCharacterSelection[1]])
                        );
            } break;
        }
    }
	return NULL;
}
