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

#include <string>
#include <vector>
#include <iostream>
#include "char.hpp"
#include "text.hpp"
#include "config.hpp"
#include "game_data.hpp"

Text::Text(GameData* igameData, SDL_Surface* iset, std::string text, unsigned int x, unsigned int y, bool center):
gameData(igameData),
whitespace(0)
{
    bool noMatch=false;
    length = text.length();
    int nextx = x;
    if (center)
        nextx -= (18*(length/2));
    int nexty = y;
    
    const char LOOKUP[40] = {'A','B','C','D','E','F','G','H','I','J',
                             'K','L','M','N','O','P','Q','R','S','T',  //h and s are arbitrary.
                             'U','V','W','X','Y','Z','h','s',',','.',
                             '0','1','2','3','4','5','6','7','8','9'}; //Only supported chars.
                             
    for (unsigned int i=0; i < length; i++)
    {
        for (int look=0; look<40; look++) {
            if ((text[i]) == LOOKUP[look]){
                chars.push_back(Char(gameData, iset, (text[i]), nextx, nexty));
                break;
            }
            else
                if (look >= 39)
                    noMatch=true;
        }
        if (noMatch)
            whitespace++;
        nextx += 18;
        noMatch = false;
    }
}

void Text::draw()
{
    for (unsigned int i=0; i < (length - whitespace); i++)
    {
#ifdef DEBUG_BUILD
    std::cout << "Updating char: " << i << std::endl << "(String len is " << t->length() << ")\n";
#endif
        chars[i].draw();
    }
}
