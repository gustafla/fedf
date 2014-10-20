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
#include "text.hpp"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <algorithm>
#include <sstream>

Menu::Menu(GameData* igameData, bool title):
gameData(igameData),
fpsKeyDelay(0),
fpsText(NULL),
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
	file2surface("gfx/logo.png", &logo);
	
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
    
    std::string tmpStageName;
    for (int i=0; i<stages.size(); i++) {
        tmpStageName = stages[i].getName();
        std::transform(tmpStageName.begin(), tmpStageName.end(), tmpStageName.begin(), ::toupper);
        stageNames.push_back(Text(gameData, gameData->charset, tmpStageName, (WIDTH/2), (HEIGHT/4)*3, true));
    }
    
    std::string tmpCharacterName;
    for (int i=0; i<characters.size(); i++) {
        tmpCharacterName = characters[i].name;
        std::transform(tmpCharacterName.begin(), tmpCharacterName.end(), tmpCharacterName.begin(), ::toupper);
        characterNames1.push_back(Text(gameData, gameData->charset, tmpCharacterName, (WIDTH/4)*1, (HEIGHT/8)*5, true));
        characterNames2.push_back(Text(gameData, gameData->charset, tmpCharacterName, (WIDTH/4)*3, (HEIGHT/8)*5, true));
    }
    
    Mix_VolumeMusic(MIX_MAX_VOLUME);
    gameData->music = Mix_LoadMUS(MAIN_MUSIC_FILE.c_str());
    Mix_PlayMusic(gameData->music, -1);
    gameData->musicPlaying = true;
}

Menu::~Menu() {
	SDL_FreeSurface(titleScreen);
	SDL_FreeSurface(characterSelectBG);
	SDL_FreeSurface(stageSelectOverlay);
	SDL_FreeSurface(ok);
	SDL_FreeSurface(help);
	SDL_FreeSurface(bg);
	SDL_FreeSurface(logo);
    gameData->musicPlaying = false;
}

Game* Menu::update() {
	if (gameData->gameEvent.type == SDL_KEYDOWN)
		{
			if (gameData->gameEvent.key.keysym.sym == SDLK_ESCAPE)
				gameData->running = false;
		}
	if (gameData->keystate[SDLK_F1]) {
        SDL_BlitSurface(help, NULL, gameData->buffer, NULL);
        #ifdef DEBUG
            std::cout << "Showing help\n";
        #endif
    } else if (gameData->keystate[SDLK_F2]) {
        mapKeys(&gameData->player1Controls);
        mapKeys(&gameData->player2Controls, 2);
    } else if (gameData->keystate[SDLK_F3] && !fpsKeyDelay) {
        gameData->fps-=10;
        if (gameData->fps < FPS_MIN)
            gameData->fps = FPS;
        fpsKeyDelay = FPS_SHOW_DELAY;
        if (fpsText != NULL)
            delete fpsText;
        std::ostringstream tmpFpsText;
        tmpFpsText << "FPS SET TO " << gameData->fps << ".";
        fpsText = new Text(gameData, gameData->charset, tmpFpsText.str(), (WIDTH/4)*3, 0, true);
    } else {
        switch (screenAt) {
            case TITLE_SCREEN: {
                SDL_BlitSurface(titleScreen, NULL, gameData->buffer, NULL);
                SDL_Rect logoC = buildRect(WIDTH/2-(logo->w/2), HEIGHT/2-(logo->h/2)+int(sin(gameData->frame/40.0)*6.0), logo->w, logo->h);
                SDL_BlitSurface(logo, NULL, gameData->buffer, &logoC);
                if (!gameData->inTransition && gameData->gameEvent.type == SDL_KEYDOWN && gameData->gameEvent.key.keysym.sym != SDLK_F3) {
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
                PlayerControls controls[2] = {gameData->player1Controls, gameData->player2Controls};
                
                SDL_BlitSurface(characterSelectBG, NULL, gameData->buffer, NULL);
                
                for (int pl=0; pl<2; pl++) {
                    if (playerCharacterDelay[pl] != 0)
                        playerCharacterDelay[pl]--;
                    
                    if (pl==0)
                        SDL_BlitSurface(characters[playerCharacterSelection[pl]].pic, &characters[playerCharacterSelection[pl]].picArea, gameData->buffer, &tpcoord[pl]);
                    else
                        SDL_BlitSurface(characters[playerCharacterSelection[pl]].lpic, &characters[playerCharacterSelection[pl]].picArea, gameData->buffer, &tpcoord[pl]);
                        
                    if (pl==0)
                        characterNames1[playerCharacterSelection[pl]].draw();
                    else
                        characterNames2[playerCharacterSelection[pl]].draw();
                        
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
                stageNames[playerStageSelection].draw();
                
                if (playerStageDelay != 0)
                    playerStageDelay--;
                if (playerStageIDelay != 0)
                    playerStageIDelay--;
                if (!gameData->inTransition) {
                    if (gameData->keystate[gameData->player1Controls.LEFT] && playerStageDelay == 0) {
                            if (playerStageSelection == 0)
                                playerStageSelection = stages.size()-1;
                            else
                                playerStageSelection--;
                            playerStageDelay = STAGE_SELECT_DELAY;
                    }
                    if (gameData->keystate[gameData->player1Controls.RIGHT] && playerStageDelay == 0) {
                            if (playerStageSelection == stages.size()-1)
                                playerStageSelection = 0;
                            else
                                playerStageSelection++;
                            playerStageDelay = STAGE_SELECT_DELAY;
                    }
                    
                    if (gameData->keystate[gameData->player1Controls.B]) {
                        screenAt = CHARACTER_SCREEN;
                        playerCharacterDone[0] = false;
                    }
                    if (gameData->keystate[gameData->player2Controls.B]) {
                        screenAt = CHARACTER_SCREEN;
                        playerCharacterDone[1] = false;
                    }
                    
                    if (gameData->keystate[gameData->player1Controls.A] && playerStageIDelay == 0)
                        gameData->inTransition=true;
                }
                
                if (gameData->inTransition)
                    if (gameData->drawTransition())
                        return new Game(
                            gameData,
                            stages[playerStageSelection],
                            Player(gameData, gameData->player1Controls, buildRect(40, 0, CHARACTER_HITBOX_SIZE, CHARACTER_HITBOX_SIZE), true, characters[playerCharacterSelection[0]]),
                            Player(gameData, gameData->player2Controls, buildRect(WIDTH-40-CHARACTER_HITBOX_SIZE, 0, CHARACTER_HITBOX_SIZE, CHARACTER_HITBOX_SIZE), false, characters[playerCharacterSelection[1]])
                        );
            } break;
        }
    }
    if (fpsKeyDelay) {
        fpsText->draw();
        fpsKeyDelay--;
    }
	return NULL;
}

void Menu::mapKeys(PlayerControls* toMap, unsigned int whose) {
    std::string caption = gameData->name;
    caption += " ";
    caption += VERSION;
    caption += " ";
    caption += "(mapping keys)";
    SDL_WM_SetCaption(caption.c_str(), NULL);
    
    PlayerControls tmp;
    unsigned int keyDelay = STAGE_SELECT_DELAY;
    int delay = 0;
    unsigned int timeLast = 0;
    enum {INFO=0, JUMP, LEFT, CROUCH, RIGHT, A, B, DONE} keyAt;
    keyAt = INFO;
    Text* screenText;
    if (whose == 1)
        screenText = new Text(gameData, gameData->charset, "PLAYER ONE, PRESS ANY KEY.");
    else
        screenText = new Text(gameData, gameData->charset, "PLAYER TWO, PRESS ANY KEY.");
    for (;;) {
        timeLast = SDL_GetTicks();
        SDL_PollEvent(&gameData->gameEvent);
        if (gameData->gameEvent.type == SDL_QUIT)
            return;
        if (gameData->gameEvent.type == SDL_KEYDOWN)
		{
			if (gameData->gameEvent.key.keysym.sym == SDLK_ESCAPE)
				return;
		}
        if (keyDelay)
            keyDelay--;
            
        SDL_BlitSurface(bg, NULL, gameData->buffer, NULL);
        screenText->draw();
        
        switch (keyAt) {
            case INFO:
                if (gameData->gameEvent.type == SDL_KEYDOWN && !keyDelay)
                {
                    keyDelay = STAGE_SELECT_DELAY;
                    keyAt = JUMP;
                    delete screenText;
                    screenText = new Text(gameData, gameData->charset, "JUMP");
                }
            break;
            case JUMP:
                if (gameData->gameEvent.type == SDL_KEYDOWN && !keyDelay)
                {
                    if (gameData->gameEvent.key.keysym.sym != SDLK_ESCAPE) {
                        keyDelay = STAGE_SELECT_DELAY;
                        tmp.JUMP = gameData->gameEvent.key.keysym.sym;
                        keyAt = LEFT;
                        delete screenText;
                        screenText = new Text(gameData, gameData->charset, "LEFT");
                    }
                }
            break;
            case LEFT:
                if (gameData->gameEvent.type == SDL_KEYDOWN && !keyDelay)
                {
                    if (gameData->gameEvent.key.keysym.sym != SDLK_ESCAPE) {
                        keyDelay = STAGE_SELECT_DELAY;
                        tmp.LEFT = gameData->gameEvent.key.keysym.sym;
                        keyAt = CROUCH;
                        delete screenText;
                        screenText = new Text(gameData, gameData->charset, "CROUCH");
                    }
                }
            break;
            case CROUCH:
                if (gameData->gameEvent.type == SDL_KEYDOWN && !keyDelay)
                {
                    if (gameData->gameEvent.key.keysym.sym != SDLK_ESCAPE) {
                        keyDelay = STAGE_SELECT_DELAY;
                        tmp.CROUCH = gameData->gameEvent.key.keysym.sym;
                        keyAt = RIGHT;
                        delete screenText;
                        screenText = new Text(gameData, gameData->charset, "RIGHT");
                    }
                }
            break;
            case RIGHT:
                if (gameData->gameEvent.type == SDL_KEYDOWN && !keyDelay)
                {
                    if (gameData->gameEvent.key.keysym.sym != SDLK_ESCAPE) {
                        keyDelay = STAGE_SELECT_DELAY;
                        tmp.RIGHT = gameData->gameEvent.key.keysym.sym;
                        keyAt = A;
                        delete screenText;
                        screenText = new Text(gameData, gameData->charset, "A");
                    }
                }
            break;
            case A:
                if (gameData->gameEvent.type == SDL_KEYDOWN && !keyDelay)
                {
                    if (gameData->gameEvent.key.keysym.sym != SDLK_ESCAPE) {
                        keyDelay = STAGE_SELECT_DELAY;
                        tmp.A = gameData->gameEvent.key.keysym.sym;
                        keyAt = B;
                        delete screenText;
                        screenText = new Text(gameData, gameData->charset, "B");
                    }
                }
            break;
            case B:
                if (gameData->gameEvent.type == SDL_KEYDOWN && !keyDelay)
                {
                    if (gameData->gameEvent.key.keysym.sym != SDLK_ESCAPE) {
                        keyDelay = STAGE_SELECT_DELAY;
                        tmp.B = gameData->gameEvent.key.keysym.sym;
                        keyAt = DONE;
                        delete screenText;
                    }
                }
            break;
        }
        if (keyAt == DONE)
            break;
        
        #ifdef DBHACK
            SDL_BlitSurface(gameData->buffer, NULL, gameData->screen, NULL);
        #endif
        SDL_Flip(gameData->screen);
        delay = (1000/gameData->fps - (SDL_GetTicks() - timeLast));
        SDL_Delay(delay < 0 ? 0 : delay);
    }
    if (keyAt == DONE)
        *toMap = tmp;
        
    std::string origCaption = gameData->name;
    origCaption += " ";
    origCaption += VERSION;
    SDL_WM_SetCaption(origCaption.c_str(), NULL);
}
