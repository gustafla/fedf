#include <string>
#include <vector>
#include <iostream>
#include "char.hpp"
#include "text.hpp"
#include "config.hpp"
#include "game_data.hpp"

Text::Text(GameData* igameData, SDL_Surface* iset, std::string text, unsigned int x, unsigned int y, bool center):
gameData(igameData),
whitespace(0)
{
    bool noMatch=false;
    length = text.length();
    int nextx = x;
    if (center)
        nextx -= (18*(length/2));
    int nexty = y;
    
    const char LOOKUP[30] = {'A','B','C','D','E','F','G','H','I','J',
                             'K','L','M','N','O','P','Q','R','S','T',  //0 and 1 are arbitrary.
                             'U','V','W','X','Y','Z','0','1',',','.'}; //Only supported chars.
                             
    for (unsigned int i=0; i < length; i++)
    {
        for (int look=0; look<30; look++) {
            if ((text[i]) == LOOKUP[look]){
                chars.push_back(Char(gameData, iset, (text[i]), nextx, nexty));
                break;
            }
            else
                if (look >= 29)
                    noMatch=true;
        }
        if (noMatch)
            whitespace++;
        nextx += 18;
        noMatch = false;
    }
}

void Text::draw()
{
    for (unsigned int i=0; i < (length - whitespace); i++)
    {
#ifdef DEBUG_BUILD
    std::cout << "Updating char: " << i << std::endl << "(String len is " << t->length() << ")\n";
#endif
        chars[i].draw();
    }
}
