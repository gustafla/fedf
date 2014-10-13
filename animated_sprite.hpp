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

#ifndef ANIMATED_SPRITE_HPP
#define ANIMATED_SPRITE_HPP

#include "game_data.hpp"
#include <SDL/SDL.h>
#include <string>
#include <vector>
#include <fstream>

class AnimatedSprite {
    private:
        GameData* gameData;
        SDL_Surface* sprite;
        unsigned int fpsDiv;
        std::vector<unsigned int> rowsLengths;
        std::ifstream configFile;
        unsigned int rowAt;
        bool holding;
        unsigned int xholdAt;
        bool doing;
        unsigned int xdoAt;
        unsigned int frameWidth;
        unsigned int frameHeight;
    public:
        AnimatedSprite(GameData* igameData, std::string dir, std::string spritename, unsigned int iframeWidth=64, unsigned int iframeHeight=64);
        ~AnimatedSprite();
        SDL_Rect getFrame();
        SDL_Surface* getSurface();
        unsigned int getRow();
        void doOnce(unsigned int row);
        void doConstant(unsigned int row);
        void hold(unsigned int row=0, unsigned int col=0);
};

#endif
