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

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include "player_controls.hpp"
#include <SDL/SDL.h>

extern std::string const VERSION;
extern unsigned int const WIDTH;
extern unsigned int const HEIGHT;

extern PlayerControls const PLAYER1_CONTROLS;
extern PlayerControls const PLAYER2_CONTROLS;

extern SDLKey const PAUSE_KEY;
extern unsigned int const PAUSE_KEY_DELAY;
extern SDLKey const CONFIRM_KEY;

extern unsigned int const SPLASH_DELAY;

extern unsigned int const WIN_DELAY;
extern unsigned int const START_DELAY;
extern unsigned int const START_SHOW_DELAY;
extern unsigned int const CHARACTER_SELECT_DELAY;
extern unsigned int const STAGE_SELECT_DELAY;

extern std::string const CHARACTERS_DIR;
extern std::string const CHARACTER_PIC_FILE;

extern std::string const STAGES_DIR;
extern std::string const STAGE_BG_FILE;

#endif
