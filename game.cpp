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
}

void Game::update() {
	stage.draw();
	for (int index=0; index<players.size(); index++)
		players[index].update();
}
