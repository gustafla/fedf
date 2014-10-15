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
