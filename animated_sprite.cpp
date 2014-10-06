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

#include "animated_sprite.hpp"
#include "gfxutil.hpp"
#include "util.hpp"
#include "config.hpp"
#include <cstdlib>
#include <iostream>

AnimatedSprite::AnimatedSprite(GameData* igameData, std::string dir, std::string spritename, unsigned int iframeWidth, unsigned int iframeHeight):
gameData(igameData),
rowAt(0),
holding(false),
xholdAt(0),
doing(false),
xdoAt(0),
frameHeight(iframeHeight),
frameWidth(iframeWidth) {
    std::vector<std::string> lines;
    char buf[16];
    configFile.open((dir+ANIM_CONF_FILE).c_str());
    
    if (!configFile.is_open()) {
        std::cout << "Couldn't read " << dir+ANIM_CONF_FILE << ".\n";
        exit(-6);
    }
    
    while (!configFile.eof()){
        configFile.getline(buf, 16);
        lines.push_back(std::string(buf));
    }
    
    if (lines.size()<2) {
        std::cout << dir+ANIM_CONF_FILE << " incorrect, no rows specified.\n";
        exit(-7);
    }
    
    fpsDiv = atoi(lines[0].c_str());
    if (fpsDiv < 1) {
        std::cout << dir+ANIM_CONF_FILE << " incorrect, frame divider can't be less than 1.\n";
        exit(-8);
    }
    
    for (unsigned int i=1; i<lines.size(); i++) {
        rowsLengths.push_back(atoi(lines[i].c_str()));
    }
    configFile.close();
    
    file2surface(dir+spritename, &sprite);
}

AnimatedSprite::~AnimatedSprite() {
    SDL_FreeSurface(sprite);
}

SDL_Rect AnimatedSprite::getFrame() {
    SDL_Rect tmp;
    if (doing) {
        tmp = buildRect(
            (xdoAt/fpsDiv)*frameWidth,
            ((rowAt>=rowsLengths.size()) ? rowsLengths.size()-1 : rowAt)*frameHeight,
            frameWidth,
            frameHeight
        );
        xdoAt++;
        if (xdoAt >= fpsDiv*(rowsLengths[(rowAt>=rowsLengths.size()) ? rowsLengths.size()-1 : rowAt])) {
            doing = false;
            xdoAt = 0;
            rowAt = 0;
        }
    } else if (holding) {
        tmp = buildRect(
            (((xholdAt)%rowsLengths[(rowAt>=rowsLengths.size()) ? rowsLengths.size()-1 : rowAt])*frameWidth),
            ((rowAt>=rowsLengths.size()) ? rowsLengths.size()-1 : rowAt)*frameHeight,
            frameWidth,
            frameHeight
        );
        holding=false;
        rowAt=0;
    } else {
        tmp = buildRect(
            ((gameData->frame/fpsDiv)%rowsLengths[(rowAt>=rowsLengths.size()) ? rowsLengths.size()-1 : rowAt])*frameWidth,
            ((rowAt>=rowsLengths.size()) ? rowsLengths.size()-1 : rowAt)*frameHeight,
            frameWidth,
            frameHeight
        );
    }
    return tmp;
}

SDL_Surface* AnimatedSprite::getSurface() {
    return sprite;
}

void AnimatedSprite::doOnce(unsigned int row) {
    doing = true;
    xdoAt = 0;
    rowAt = row;
}

void AnimatedSprite::doConstant(unsigned int row) {
    rowAt = row;
}

void AnimatedSprite::hold(unsigned int row, unsigned int col) {
    holding = true;
    rowAt = row;
    xholdAt = col;
}
