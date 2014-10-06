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

#include "stage.hpp"
#include "game_data.hpp"
#include "gfxutil.hpp"
#include "config.hpp"
#include <SDL/SDL.h>
#include <string>
#include <iostream>

Stage::Stage(GameData* igameData, std::string iname, std::string bgFileName, unsigned int ifloor):
gameData(igameData),
name(iname) {
   file2surface(bgFileName, &bg); 
   if (bg->w < WIDTH || bg->h < HEIGHT) {
       std::cout << "Stage " << iname << " bg too small.\n";
       exit(-9);
   }
   floor = bg->h - ifloor;
}

void Stage::clean() {
    #ifdef DEBUG
        std::cout << "Stage clean\n";
    #endif
    SDL_FreeSurface(bg);
    
    #ifdef DEBUG
        std::cout << "Stage clean finish\n";
    #endif
}

void Stage::draw() {
   SDL_BlitSurface(bg, &gameData->screenRect, gameData->buffer, NULL);
}

unsigned int Stage::getFloor() {
    return floor;
}

std::string Stage::getName() {
    return name;
}

SDL_Surface* Stage::getBG() {
    return bg;
}
