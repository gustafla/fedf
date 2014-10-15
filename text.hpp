#ifndef TEXT_HPP
#define TEXT_HPP

#include <vector>
#include <string>
#include <SDL/SDL.h>
#include "char.hpp"
#include "game_data.hpp"

class Text {
    private:
        GameData* gameData;
        std::vector<Char> chars;
        unsigned int whitespace;
        unsigned int length;
    public:
        Text(GameData* igameData, SDL_Surface* iset, std::string text, unsigned int x=0, unsigned int y=0);
        void draw();
};

#endif
