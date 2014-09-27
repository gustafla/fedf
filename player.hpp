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

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL/SDL.h>
#include "config.hpp"
#include "game_data.hpp"
#include "player_controls.hpp"

class Player {
    private:
        GameData* gameData;
        Character character;
        int health;
        int speed;
        
        SDL_Rect coord;
        
        int fall;
        int jump;
        bool right;
        
        PlayerControls controls;
        
        bool active;
        
    public:
        Player(GameData* igameData, PlayerControls icontrols, SDL_Rect spawn, bool iright, Character icharacter);
        void clean();
        unsigned int getHealth();
        void takeDamage(unsigned int amnt);
        SDL_Rect getCoord();
        void update();
        void start();
        void stop();
        bool isActive();
        void push(int vec);
};

#endif
