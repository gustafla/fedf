#include "char.hpp"
#include "config.hpp"
#include <iostream>
#include <SDL/SDL.h>
#include "game_data.hpp"

Char::Char(GameData* igameData, SDL_Surface* iset, char io, float ix, float iy):
set(iset)
{
    gameData = igameData;
	c.x = ix;
	c.y = iy;
    o = getMapOffset(io);
}

void Char::draw()
{
#ifdef DEBUG
    std::cout << "My c.x now: " << c.x << std::endl;
#endif
    if (c.y > -16 && c.y < HEIGHT && c.x < WIDTH && c.x > -16)
        SDL_BlitSurface(set, &o, gameData->buffer, &c);
}

SDL_Rect Char::getMapOffset(char io)
{
    const unsigned int setsize = 30; //The Mehu charset is 10 chars wide and chars are 16x16.
    SDL_Rect mo;
    mo.x = 0;
    mo.y = 0;
    mo.h = 16;
    mo.w = 16;
    const char LOOKUP[30] = {'A','B','C','D','E','F','G','H','I','J',
                             'K','L','M','N','O','P','Q','R','S','T',  //0 and 1 are arbitrary.
                             'U','V','W','X','Y','Z','0','1',',','.'}; //Only supported chars.
    
    for (unsigned int i=0;i<setsize;i++) //SLOW AS HELL WTF HAVE I DONE ;__;
    {
        if (LOOKUP[i] == io)
        {
#ifdef DEBUG
            std::cout << "Found character " << io << " which appears to be " << LOOKUP[i] << ".\n" << "At: " << i << std::endl << "It's cooridinates are: (" << mo.x << "," << mo.y << ")" << std::endl << "(My x is " << x << ")\n" << "(My y is " << y << ")\n" << "(My h is " << o.h << ")\n" << "(My w is " << o.w << ")\n" << std::endl;
#endif
            return mo;
        }
            
        mo.x+=mo.w;
        if (mo.x>=16*10)
        {
            mo.x=0;
            mo.y+=mo.h;
        }
    }
    mo.x=0;
    mo.y=0;
    
    return mo;
}
