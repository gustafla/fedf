#include "menu.hpp"
#include "gfxutil.hpp"
#include "util.hpp"
#include "player_controls.hpp"
#include "config.hpp"
#include <cstdlib>
#include <iostream>

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
	file2surface("gfx/ok.png", &ok);
	
	if (title)
		screenAt = TITLE_SCREEN;
		
	std::vector<std::string> characterDirList;
    if (listDir(CHARACTERS_DIR, &characterDirList)) {
		for (int i=0; i<characterDirList.size(); i++) {
			characters.push_back(Character(characterDirList[i], CHARACTERS_DIR+characterDirList[i]+CHARACTER_PIC_FILE));
		}
	} else {
		std::cout << "Something went wrong parsing characters.\n";
		exit(-4);
	}
	
	std::vector<std::string> stageDirList;
    if (listDir(STAGES_DIR, &stageDirList)) {
		for (int i=0; i<stageDirList.size(); i++) {
			stages.push_back(Stage(igameData, stageDirList[i], STAGES_DIR+stageDirList[i]+STAGE_BG_FILE));
		}
	} else {
		std::cout << "Something went wrong parsing stages.\n";
		exit(-4);
	}
}

Menu::~Menu() {
	SDL_FreeSurface(titleScreen);
	SDL_FreeSurface(characterSelectBG);
	SDL_FreeSurface(ok);
}

Game* Menu::update() {
	switch (screenAt) {
		case TITLE_SCREEN: {
			SDL_BlitSurface(titleScreen, NULL, gameData->buffer, NULL);
			if (gameData->gameEvent.type == SDL_KEYDOWN)
				screenAt = CHARACTER_SCREEN;
		} break; case CHARACTER_SCREEN: {
			
			SDL_Rect tpcoord[2] = {buildRect(WIDTH/4-32, HEIGHT/2-32, 0, 0), buildRect((WIDTH/4)*3-32, HEIGHT/2-32, 0, 0)};
			SDL_Rect okcoord[2] = {buildRect(WIDTH/4-32, HEIGHT/2+32, 0, 0), buildRect((WIDTH/4)*3-32, HEIGHT/2+32, 0, 0)};
			PlayerControls controls[2] = {PLAYER1_CONTROLS, PLAYER2_CONTROLS};
			
			SDL_BlitSurface(characterSelectBG, NULL, gameData->buffer, NULL);
			
			for (int pl=0; pl<2; pl++) {
				if (playerCharacterDelay[pl] != 0)
					playerCharacterDelay[pl]--;
		
				SDL_BlitSurface(characters[playerCharacterSelection[pl]].pic, NULL, gameData->buffer, &tpcoord[pl]);
	
				if (playerCharacterDone[pl])
					SDL_BlitSurface(ok, NULL, gameData->buffer, &okcoord[pl]);
				
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
			
			if (playerCharacterDone[0] && playerCharacterDone[1]) {
				screenAt = STAGE_SCREEN;
				playerStageIDelay = 60;
			}
		} break; case STAGE_SCREEN: {
			SDL_BlitSurface(stages[playerStageSelection].getBG(), NULL, gameData->buffer, NULL);
			
			if (playerStageDelay != 0)
				playerStageDelay--;
			if (playerStageIDelay != 0)
				playerStageIDelay--;
			
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
				return new Game(
						gameData,
						stages[playerStageSelection],
						Player(gameData, PLAYER1_CONTROLS, buildRect(20, 0, 64, 64), true, characters[playerCharacterSelection[0]]),
						Player(gameData, PLAYER2_CONTROLS, buildRect(WIDTH-20-64, 0, 64, 64), false, characters[playerCharacterSelection[1]])
				);
		} break;
	}
	return NULL;
}
