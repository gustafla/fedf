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

#ifndef SOUNDS_HPP
#define SOUNDS_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <map>
#include <string>
#include "config.hpp"

class GameSounds {
	public:
		~GameSounds();
		void play(std::string name);
	private:
		std::map<std::string, Mix_Chunk*> sounds;
};

#endif
