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

#ifndef EFFECT_HPP
#define EFFECT_HPP

#include <SDL/SDL.h>
#include <string>
#include "animated_sprite.hpp"
#include "game_data.hpp"
#include "config.hpp"
#include "util.hpp"

class Effect {
    public:
        Effect(GameData* igameData, std::string dir, bool right, int ix, int iy);
        ~Effect();
        bool draw();
        bool done;
    private:
        AnimatedSprite* sprite;
        GameData* gameData;
        int x;
        int y;
};

#endif
