#include <string>
#include <vector>
#include <iostream>
#include "char.hpp"
#include "text.hpp"
#include "config.hpp"
#include "game_data.hpp"

Text::Text(GameData* igameData, SDL_Surface* iset, std::string text, unsigned int x, unsigned int y):
gameData(igameData),
whitespace(0)
{
    length = text.length();
    int nextx = x;
    int nexty = y;
    
    for (unsigned int i=0; i < length; i++)
    {
        if ((text[i]) != ' ')
            chars.push_back(Char(gameData, iset, (text[i]), nextx, nexty));
        else
            whitespace++;
        nextx += 18;
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
