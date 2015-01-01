// Copyright 2014, 2015 Lauri Gustafsson, Kalle Korhonen and Julius Heino
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

#include "char.hpp"
#include "config.hpp"
#include <iostream>
#include <SDL/SDL.h>
#include "game_data.hpp"

Char::Char(GameData* igameData, SDL_Surface* iset, char io, float ix, float iy):
set(iset)
{
    gameData = igameData;
	c.x = ix;
	c.y = iy;
    o = getMapOffset(io);
}

void Char::draw()
{
#ifdef DEBUG
    std::cout << "My c.x now: " << c.x << std::endl;
#endif
    if (c.y > -16 && c.y < HEIGHT && c.x < WIDTH && c.x > -16)
        SDL_BlitSurface(set, &o, gameData->buffer, &c);
}

SDL_Rect Char::getMapOffset(char io)
{
    const unsigned int setsize = 40; //The Mehu charset is 10 chars wide and chars are 16x16.
    SDL_Rect mo;
    mo.x = 0;
    mo.y = 0;
    mo.h = 16;
    mo.w = 16;
    const char LOOKUP[40] = {'A','B','C','D','E','F','G','H','I','J',
                             'K','L','M','N','O','P','Q','R','S','T',  //h and s are arbitrary.
                             'U','V','W','X','Y','Z','h','s',',','.',
                             '0','1','2','3','4','5','6','7','8','9'}; //Only supported chars.
    
    for (unsigned int i=0;i<setsize;i++) //SLOW AS HELL WTF HAVE I DONE ;__;
    {
        if (LOOKUP[i] == io)
        {
#ifdef DEBUG
            std::cout << "Found character " << io << " which appears to be " << LOOKUP[i] << ".\n" << "At: " << i << std::endl << "It's cooridinates are: (" << mo.x << "," << mo.y << ")" << std::endl << "(My x is " << x << ")\n" << "(My y is " << y << ")\n" << "(My h is " << o.h << ")\n" << "(My w is " << o.w << ")\n" << std::endl;
#endif
            return mo;
        }
            
        mo.x+=mo.w;
        if (mo.x>=16*10)
        {
            mo.x=0;
            mo.y+=mo.h;
        }
    }
    mo.x=0;
    mo.y=0;
    
    return mo;
}
