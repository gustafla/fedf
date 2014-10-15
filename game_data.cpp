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

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <cmath>
#include <iostream>
#include "game_data.hpp"
#include "gfxutil.hpp"
#include "util.hpp"
#include "config.hpp"

GameData::GameData(SDL_Surface* screen):
running(true),
frame(0),
musicPlaying(false),
inTransition(false),
postTransition(false),
name("Fedora Fighters") {
    #ifndef DBHACK
        buffer = screen;
    #else
        buffer = SDL_CreateRGBSurface(
            screen->flags,
            screen->w,
            screen->h,
            screen->format->BitsPerPixel,
            screen->format->Rmask,
            screen->format->Gmask,
            screen->format->Bmask,
            screen->format->Amask
        );
    #endif
    
    file2surface("gfx/transition.png", &transition);
    file2surface("gfx/mehusans.png", &charset);
    transitionDest = buildRect(WIDTH, 0, TRANSITION_WIDTH, 480);
    transitionSrc = buildRect(0, 0, TRANSITION_WIDTH, 480);
    transitionx = WIDTH;
    
    screenRect = buildRect(0, 0, WIDTH, HEIGHT);
    
    if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, (4096*4) ) == -1 )
    {
		std::cout << "Can't do audio D:\n";
        exit(-8);
    }
}

bool GameData::drawTransition() {
	SDL_BlitSurface(transition, &transitionSrc, buffer, &transitionDest);
	transitionx += X_TRANSITION_VEC;
	transitionDest.x += X_TRANSITION_VEC;
	if (transitionx<=0)
		transitionSrc.x = abs(transitionx);
	if (transitionx < ((int)TRANSITION_WIDTH)*(-1)) {
		transitionDest.x = WIDTH;
		transitionx = WIDTH;
		transitionSrc.x = 0;
		inTransition = false;
		postTransition = false;
		return false;
	}
	if (!postTransition && transitionx <= (((((int)TRANSITION_WIDTH)-(int)WIDTH)/2)*(-1))) {
        #ifdef DEBUG
            std::cout << "transitionx(" << transitionx << ") <= -((TRANSITION_WIDTH-WIDTH)/2)(" << (((((int)TRANSITION_WIDTH)-(int)WIDTH)/2)*(-1)) << "), so returning true.\n";
            std::cout << "TRANSITION_WIDTH: " << TRANSITION_WIDTH << "\n";
        #endif
		postTransition=true;
		return true;
	}
	return false;
}
