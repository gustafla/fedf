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

#ifndef TEXT_HPP
#define TEXT_HPP

#include <vector>
#include <string>
#include <SDL/SDL.h>
#include "char.hpp"
#include "game_data.hpp"

class Text {
    private:
        GameData* gameData;
        std::vector<Char> chars;
        unsigned int whitespace;
        unsigned int length;
    public:
        Text(GameData* igameData, SDL_Surface* iset, std::string text, unsigned int x=0, unsigned int y=0, bool center=false);
        void draw();
};

#endif
