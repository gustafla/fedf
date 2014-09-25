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

#include "config.hpp"
#include "player_controls.hpp"
#include <string>
#include <SDL/SDL.h>

#ifdef DEBUG
    std::string const VERSION = "debug build";
#else
    std::string const VERSION = "0.1 alpha";
#endif

unsigned int const WIDTH = 640;
unsigned int const HEIGHT = 480;

PlayerControls const PLAYER1_CONTROLS = {
    SDLK_UP,        //jump
    SDLK_DOWN,      //crouch
    SDLK_LEFT,      //left
    SDLK_RIGHT,     //right
    SDLK_k,         //a
    SDLK_l          //b
};

PlayerControls const PLAYER2_CONTROLS = {
    SDLK_w,         //jump
    SDLK_s,         //crouch
    SDLK_a,         //left
    SDLK_d,         //right
    SDLK_c,         //a
    SDLK_v          //b
};

unsigned int const SPLASH_DELAY = 4000;

unsigned int const WIN_DELAY = 60*3;
unsigned int const START_DELAY = 60*3;
unsigned int const START_SHOW_DELAY = 60*1;
