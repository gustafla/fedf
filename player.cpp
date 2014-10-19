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

#include "gfxutil.hpp"
#include "player.hpp"
#include "player_controls.hpp"
#include "config.hpp"
#include "game_data.hpp"
#include "util.hpp"
#include "character.hpp"
#include <SDL/SDL.h>
#include <iostream>

Player::Player(GameData* igameData, PlayerControls icontrols, SDL_Rect spawn, bool iright, Character icharacter, unsigned int inumber):
number(inumber),
health(100),
gameData(igameData),
character(icharacter),
controls(icontrols),
active(false),
moved(false),
fall(0),
jump(0),
xjump(0),
speed(3),
jumpSpeed(5),
jumpPower(18),
attackDelay(0),
attackRegisterDelay(0),
right(iright) {
    #ifdef DEBUG
        std::cout << "Player constructor\n";
    #endif
    coord.x = spawn.x;
    coord.y = spawn.y;
    coord.w = CHARACTER_HITBOX_SIZE;
    coord.h = CHARACTER_HITBOX_SIZE;
    screenCoord = coord;
    
    sprite = new AnimatedSprite(igameData, icharacter.dir, CHARACTER_RIGHT_SPRITE_FILE, CHARACTER_FRAME_SIZE, CHARACTER_FRAME_SIZE);
    file2surface(icharacter.dir+CHARACTER_LEFT_SPRITE_FILE, &spritel);
    shadow = new AnimatedSprite(igameData, "gfx/shadow", CHARACTER_SPRITE_FILE);
    
    #ifdef DEBUG
        std::cout << "Player constructor finish\n";
    #endif
}

void Player::clean() {
    #ifdef DEBUG
        std::cout << "Player clean\n";
    #endif
    delete sprite;
    delete shadow;
    SDL_FreeSurface(spritel);
    #ifdef DEBUG
        std::cout << "Player clean finish\n";
    #endif
}

unsigned int Player::getHealth() {
    return (health > 0) ? health : 0;
}

bool Player::waiting() {
    return attackDelay;
}

void Player::takeDamage(unsigned int amnt) {
    health -= amnt;
    if (health < 0)
        health = 0;
    if (attackStatus == 2) // kick can be avoided
        attackStatus = 0;
    stop();
    attackDelay += ((50*amnt)/100);
}

SDL_Rect Player::getCoord() {
    return coord;
}

unsigned int Player::getAttackStatus() {
    if (!attackRegisterDelay)
        return attackStatus;
    else
        return 0;
}

void Player::clearAttackStatus() {
    attackStatus = 0;
}

void Player::start() {
    active = true;
}

void Player::stop() {
    active = false;
}

bool Player::isActive() {
    return active;
}

void Player::push(int vec) {
    coord.x += vec;
}

SDL_Surface* Player::getPic() {
    return character.pic;
}

void Player::update() {
    #ifdef DEBUG
        std::cout << "Player update\n";
    #endif
    if (health < 0)
        health = 0;
    
    if (attackDelay != 0)
		attackDelay--;
    else
        start();
    if (attackRegisterDelay != 0)
		attackRegisterDelay--;
    
    if (fall > 0) {
        coord.y += fall;
        fall++;
        moved = true;
        #ifdef DEBUG
            std::cout << "Falling...\n";
        #endif
    }
    if (jump > 0){
        fall = 0;
        coord.y -= jump;
        jump--;
        moved = true;
        #ifdef DEBUG
            std::cout << "Jumping...\n";
        #endif
    }
    coord.x+=xjump;
    if (coord.y+coord.h > gameData->stageFloor) {
        fall = 0;
        jump = 0;
        xjump = 0;
        coord.y = gameData->stageFloor-coord.h;
        moved = false;
        #ifdef DEBUG
            std::cout << "Fell to ground...\n";
        #endif
    }
    if ((fall == 0) && (jump == 0) && (coord.y+coord.h < gameData->stageFloor)) {
        fall=1;
        moved=true;
        #ifdef DEBUG
            std::cout << "Now falling...\n";
        #endif
    }
    
    if (active) {
        if (gameData->keystate[controls.LEFT] && jump==0 && fall==0) {
            moved = true;
            coord.x-=speed;
        } if (gameData->keystate[controls.RIGHT] && jump==0 && fall==0) {
            moved = true;
            coord.x+=speed;
        } if (gameData->keystate[controls.JUMP]) {
            if (jump == 0 && fall == 0) {
                moved = true;
                jump = jumpPower;
                if (gameData->keystate[controls.RIGHT])
					xjump = jumpSpeed;
				if (gameData->keystate[controls.LEFT])
					xjump = -jumpSpeed;
			}
        } if (gameData->keystate[controls.A] && !attackDelay) {
			sprite->doOnce(1);
			attackStatus = 1;
            attackDelay = PUNCH_DELAY;
            attackRegisterDelay = ATTACK_REGISTER_DELAY;
            stop();
		} if (gameData->keystate[controls.B] && !attackDelay) {
			sprite->doOnce(2);
			attackStatus = 2;
            attackDelay = KICK_DELAY;
            attackRegisterDelay = ATTACK_REGISTER_DELAY;
            stop();
		}
    }
    
    if (coord.x < gameData->screenRect.x) {
        coord.x = gameData->screenRect.x;
        #ifdef DEBUG
            std::cout << "Player out of left bound, moving.\n";
        #endif
    }
    if (coord.y <= 0)
        coord.y = 1;
    if (coord.x+coord.w > gameData->screenRect.x+gameData->screenRect.w) {
        coord.x = gameData->screenRect.x+gameData->screenRect.w-coord.w;
        #ifdef DEBUG
            std::cout << "Player out of right bound, moving.\n";
        #endif
    }
    
    if (!moved)
        sprite->hold();
        
    moved = false;
    
    #ifdef DEBUG
        std::cout << "Player update finish\n";
    #endif
}

void Player::drawShadow() {
    #ifdef DEBUG
        int stageFloorPx = (gameData->stageHeight - gameData->stageFloor);
        std::cout << 4.0-((float(((gameData->stageHeight - stageFloorPx)-(HEIGHT/2))-((gameData->stageHeight-coord.y)/(HEIGHT/2)))/
                           float((gameData->stageHeight - stageFloorPx)-(HEIGHT/2)))*4.0) << std::endl;
    #endif
    if (coord.h+coord.y > (gameData->stageHeight - HEIGHT)+(HEIGHT/2)) {
        #ifndef DEBUG
            int stageFloorPx = (gameData->stageHeight - gameData->stageFloor);
        #endif
        int xlevel=0;
        for (int l=0; l < 4; l++)
            if (coord.h+coord.y > ((gameData->stageHeight - HEIGHT)+(HEIGHT/2))+(((HEIGHT/2)-stageFloorPx)/4)*l)
                xlevel=3-l;
        shadow->hold(0, xlevel);
        SDL_Rect animSrc = shadow->getFrame();
        SDL_Rect shadowRect = buildRect(coord.x - gameData->screenRect.x, gameData->stageFloor-16  - gameData->screenRect.y, 64, 32);
        SDL_BlitSurface(shadow->getSurface(), &animSrc, gameData->buffer, &shadowRect);
    }
}

void Player::draw() {
    screenCoord = buildRect(coord.x - gameData->screenRect.x, coord.y - gameData->screenRect.y, coord.w, coord.h);
    SDL_Rect animSrc = sprite->getFrame();
    SDL_Rect finalCoord = buildRect(screenCoord.x-((CHARACTER_FRAME_SIZE-CHARACTER_HITBOX_SIZE)/2), screenCoord.y-(CHARACTER_FRAME_SIZE-CHARACTER_HITBOX_SIZE), screenCoord.w, screenCoord.h);
	SDL_BlitSurface(right ? sprite->getSurface() : spritel, &animSrc, gameData->buffer, &finalCoord);
}

void Player::lookRight(bool look) {
    right = look;
}

bool Player::lookingRight() {
    return right;
}
