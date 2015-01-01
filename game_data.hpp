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

#ifndef GAME_DATA_HPP
#define GAME_DATA_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <string>
#include <player_controls.hpp>
#include "sounds.hpp"

class GameData {
    public:
        GameData(SDL_Surface* iscreen);
        SDL_Surface* buffer;
        SDL_Surface* screen;
        SDL_Surface* transition;
        SDL_Surface* charset;
        SDL_Rect transitionDest;
        SDL_Rect transitionSrc;
        int transitionx;
        unsigned int stageFloor;
        unsigned int stageHeight;
        SDL_Event gameEvent;
        Uint8* keystate;
        bool running;
        std::string name;
        unsigned int frame;
        unsigned int gameFrame;
        bool drawTransition();
        bool inTransition;
        bool postTransition;
        SDL_Rect screenRect;
        Mix_Music* music;
        bool musicPlaying;
        PlayerControls player1Controls;
        PlayerControls player2Controls;
        unsigned int fps;
        GameSounds* sounds;
};

#endif
