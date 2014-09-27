#include "menu.hpp"
#include "gfxutil.hpp"
#include "util.hpp"
#include "config.hpp"
#include <cstdlib>

Menu::Menu(GameData* igameData, bool title):
gameData(igameData),
screenAt(CHARACTER_SCREEN) {
	
	playerCharacterSelection[0]=0;
	playerCharacterSelection[1]=0;
	playerCharacterDone[0]=false;
	playerCharacterDone[1]=false; 
	playerCharacterDelay[0]=0; 
	playerCharacterDelay[1]=0; 
	
	file2surface("gfx/title.png", &titleScreen);
	file2surface("gfx/characterbg.png", &characterSelectBG);
	file2surface("gfx/ok.png", &ok);
	
	if (title)
		screenAt = TITLE_SCREEN;
}

Menu::~Menu() {
	SDL_FreeSurface(titleScreen);
	SDL_FreeSurface(characterSelectBG);
	SDL_FreeSurface(ok);
}

Game* Menu::update() {
	if (screenAt == TITLE_SCREEN) {
		SDL_BlitSurface(titleScreen, NULL, gameData->buffer, NULL);
		if (gameData->gameEvent.type == SDL_KEYDOWN)
			screenAt = CHARACTER_SCREEN;
	} else if (screenAt == CHARACTER_SCREEN) {
		if (playerCharacterDelay[0] != 0)
			playerCharacterDelay[0]--;
		if (playerCharacterDelay[1] != 0)
			playerCharacterDelay[1]--;
		
		SDL_BlitSurface(characterSelectBG, NULL, gameData->buffer, NULL);
		SDL_Rect tpcoord[2] = {buildRect(WIDTH/4-32, HEIGHT/2-32, 0, 0), buildRect((WIDTH/4)*3-32, HEIGHT/2-32, 0, 0)};
		SDL_Rect okcoord[2] = {buildRect(WIDTH/4-32, HEIGHT/2+32, 0, 0), buildRect((WIDTH/4)*3-32, HEIGHT/2+32, 0, 0)};
		SDL_BlitSurface(gameData->characters[playerCharacterSelection[0]].pic, NULL, gameData->buffer, &tpcoord[0]);
		SDL_BlitSurface(gameData->characters[playerCharacterSelection[1]].pic, NULL, gameData->buffer, &tpcoord[1]);
		if (playerCharacterDone[0])
			SDL_BlitSurface(ok, NULL, gameData->buffer, &okcoord[0]);
		if (playerCharacterDone[1])
			SDL_BlitSurface(ok, NULL, gameData->buffer, &okcoord[1]);
		
		
		if (gameData->keystate[PLAYER1_CONTROLS.LEFT] && !playerCharacterDone[0] && playerCharacterDelay[0] == 0) {
			if (playerCharacterSelection[0] == 0)
				playerCharacterSelection[0] = gameData->characters.size()-1;
			else
				playerCharacterSelection[0]--;
			playerCharacterDelay[0] = CHARACTER_SELECT_DELAY;
		}
		if (gameData->keystate[PLAYER2_CONTROLS.LEFT] && !playerCharacterDone[1] && playerCharacterDelay[1] == 0) {
			if (playerCharacterSelection[1] == 0)
				playerCharacterSelection[1] = gameData->characters.size()-1;
			else
				playerCharacterSelection[1]--;
			playerCharacterDelay[1] = CHARACTER_SELECT_DELAY;
		}
		if (gameData->keystate[PLAYER1_CONTROLS.RIGHT] && !playerCharacterDone[0] && playerCharacterDelay[0] == 0) {
			if (playerCharacterSelection[0] == gameData->characters.size()-1)
				playerCharacterSelection[0] = 0;
			else
				playerCharacterSelection[0]++;
			playerCharacterDelay[0] = CHARACTER_SELECT_DELAY;
		}
		if (gameData->keystate[PLAYER2_CONTROLS.RIGHT] && !playerCharacterDone[1] && playerCharacterDelay[1] == 0) {
			if (playerCharacterSelection[1] == gameData->characters.size()-1)
				playerCharacterSelection[1] = 0;
			else
				playerCharacterSelection[1]++;
			playerCharacterDelay[1] = CHARACTER_SELECT_DELAY;
		}
		if (gameData->keystate[PLAYER1_CONTROLS.A])
			playerCharacterDone[0] = true;
		if (gameData->keystate[PLAYER2_CONTROLS.A])
			playerCharacterDone[1] = true;
		if (gameData->keystate[PLAYER1_CONTROLS.B])
			playerCharacterDone[0] = false;
		if (gameData->keystate[PLAYER2_CONTROLS.B])
			playerCharacterDone[1] = false;
			
		if (playerCharacterDone[0] && playerCharacterDone[1])
			return new Game(
				gameData,
				Stage(gameData, "gfx/test.png"),
				Player(gameData, PLAYER1_CONTROLS, buildRect(20, 0, 64, 64), true, gameData->characters[playerCharacterSelection[0]]),
				Player(gameData, PLAYER2_CONTROLS, buildRect(WIDTH-20-64, 0, 64, 64), false, gameData->characters[playerCharacterSelection[1]])
			);
	}
	return NULL;
}
