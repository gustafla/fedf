#ifndef CHAR_HPP
#define CHAR_HPP

#include <SDL/SDL.h>
#include "game_data.hpp"

class Char
{
	public:
        Char(GameData* igameData, SDL_Surface* iset, char io, float ix=0, float iy=0);
        void draw();

	private:
        SDL_Rect c;
        SDL_Rect o;
        GameData* gameData;
        SDL_Surface* set;
        SDL_Rect getMapOffset(char io);
};

#endif
