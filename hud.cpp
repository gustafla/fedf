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

#include "hud.hpp"
#include "player.hpp"
#include "gfxutil.hpp"
#include "util.hpp"
#include "config.hpp"
#include <SDL/SDL.h>
#include <iostream>
#include <cstdlib>

Hud::Hud(GameData* igameData, Player* ip1, Player* ip2):
p1(ip1),
p2(ip2),
gameData(igameData),
p1hcoord(buildRect(20, 32, ((WIDTH/2)-20-32), 32)),
p2hcoord(buildRect((WIDTH/2)+32, 32, ((WIDTH/2)-20-32), 32)),
bgcoord(buildRect(0, 16, 640, 64)) {
    #ifdef DEBUG
        std::cout << "Hud constructor\n";
    #endif
    file2surface("gfx/health1frame.png", &p1hframe);
    file2surface("gfx/health2frame.png", &p2hframe);
    file2surface("gfx/health1bars.png", &p1hbar);
    file2surface("gfx/health2bars.png", &p2hbar);
    file2surface("gfx/hudbg.png", &bg);
    file2surface("gfx/fight.png", &start);
    file2surface("gfx/p1win.png", &p1win);
    file2surface("gfx/p2win.png", &p2win);
    #ifdef DEBUG
        std::cout << "Hud constructor finish\n";
    #endif
}

Hud::Hud() {
    std::cout << "Using the dangerous workaround constructor for Hud.\n";
};

void Hud::clean() {
    #ifdef DEBUG
        std::cout << "Hud clean\n";
    #endif
    SDL_FreeSurface(p1hbar);
    SDL_FreeSurface(p2hbar);
    SDL_FreeSurface(p1hframe);
    SDL_FreeSurface(p2hframe);
    SDL_FreeSurface(p1win);
    SDL_FreeSurface(p2win);
    SDL_FreeSurface(start);
    SDL_FreeSurface(bg);

    #ifdef DEBUG
        std::cout << "Hud clean finish\n";
    #endif
};

void Hud::draw(int winner) {
    #ifdef DEBUG
        std::cout << "Hud draw\n";
    #endif
    SDL_BlitSurface(bg, NULL, gameData->buffer, &bgcoord);
    SDL_BlitSurface(p1hframe, NULL, gameData->buffer, &p1hcoord);
    SDL_BlitSurface(p2hframe, NULL, gameData->buffer, &p2hcoord);
    SDL_Rect tp1bar = buildRect(0, 0, (unsigned int)((p1hcoord.w/100.0)*(float)p1->getHealth()), p1hcoord.h);
    unsigned int p2magic = (unsigned int)((p2hcoord.w/100.0)*(float)p2->getHealth());
    SDL_Rect tp2bar = buildRect(p2hcoord.w-p2magic, 0, p2magic, p2hcoord.h);
    SDL_Rect tp2coord = buildRect(p2hcoord.x+(p2hcoord.w-p2magic), p2hcoord.y, p2hcoord.w, p2hcoord.h);
    if (p1->getHealth()>1)
        SDL_BlitSurface(p1hbar, &tp1bar, gameData->buffer, &p1hcoord);
    if (p2->getHealth()>1)
        SDL_BlitSurface(p2hbar, &tp2bar, gameData->buffer, &tp2coord);
        
    if (gameData->gameFrame > START_DELAY-START_SHOW_DELAY && gameData->gameFrame < START_DELAY) {
        SDL_Rect startsrc = buildRect(int((float(rand()%100000)/100000.0)*float(gameData->gameFrame-(START_DELAY-START_SHOW_DELAY))*0.4), int((float(rand()%100000)/100000.0)*float(gameData->gameFrame-(START_DELAY-START_SHOW_DELAY))*0.4), 640, 480);
        SDL_Rect startdst = buildRect(int((float(rand()%100000)/100000.0)*float(gameData->gameFrame-(START_DELAY-START_SHOW_DELAY))*0.4), int((float(rand()%100000)/100000.0)*float(gameData->gameFrame-(START_DELAY-START_SHOW_DELAY))*0.4), 640, 480);
        SDL_BlitSurface(start, &startsrc, gameData->buffer, &startdst);
    }
        
    if (winner == 1)
        SDL_BlitSurface(p1win, NULL, gameData->buffer, NULL);
    if (winner == 2)
        SDL_BlitSurface(p2win, NULL, gameData->buffer, NULL);
        
    #ifdef DEBUG
        std::cout << "Hud draw finish\n";
    #endif
}
