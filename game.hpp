#ifndef GAME_HPP
#define GAME_HPP

#include <SDL/SDL.h>
#include <vector>
#include "game_data.hpp"
#include "player.hpp"
#include "stage.hpp"

class Game {
	private:
		GameData* gameData;
		std::vector<Player> players;
		Stage stage;
		//Hud hud;
		
	public:
		Game(GameData* igameData, Stage istage, Player player1, Player player2/*, unsigned int timer*/);
		void update();
};

#endif
