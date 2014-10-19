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
#include "character.hpp"
#include "game_data.hpp"
#include "player_controls.hpp"
#include "animated_sprite.hpp"

class Player {
    private:
    
		unsigned int number;    //Unused
    
        GameData* gameData;
        Character character;
        int health;
        unsigned int speed;
        unsigned int jumpSpeed;
        unsigned int jumpPower;
        
        SDL_Rect screenCoord;
        SDL_Rect coord;
        AnimatedSprite* shadow;
        AnimatedSprite* sprite;
        SDL_Surface* spritel;
        
        int fall;
        int jump;
        int xjump;
        bool right;
        unsigned int attackStatus;
        
        PlayerControls controls;
        
        bool active;
        
        unsigned int attackDelay;
        unsigned int attackRegisterDelay;
        
        bool moved;
        
    public:
        Player(GameData* igameData, PlayerControls icontrols, SDL_Rect spawn, bool iright, Character icharacter, unsigned int inumber=0);
        void clean();
        unsigned int getHealth();
        void takeDamage(unsigned int amnt);
        SDL_Rect getCoord();
        void update();
        void drawShadow();
        void draw();
        void start();
        void stop();
        bool isActive();
        void push(int vec);
        SDL_Surface* getPic();
        void lookRight(bool look);
        bool lookingRight();
        unsigned int getAttackStatus();
        void clearAttackStatus();
};

#endif
