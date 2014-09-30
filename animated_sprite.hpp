#ifndef ANIMATED_SPRITE_HPP
#define ANIMATED_SPRITE_HPP

#include "game_data.hpp"
#include <SDL/SDL.h>
#include <string>
#include <vector>
#include <fstream>

class AnimatedSprite {
    private:
        GameData* gameData;
        SDL_Surface* sprite;
        unsigned int fpsDiv;
        std::vector<unsigned int> rowsLengths;
        std::ifstream configFile;
        unsigned int rowAt;
        bool holding;
        unsigned int xholdAt;
        bool doing;
        unsigned int xdoAt;
        unsigned int frameWidth;
        unsigned int frameHeight;
    public:
        AnimatedSprite(GameData* igameData, std::string dir, std::string spritename, unsigned int iframeWidth=64, unsigned int iframeHeight=64);
        ~AnimatedSprite();
        SDL_Rect getFrame();
        SDL_Surface* getSurface();
        void doOnce(unsigned int row);
        void doConstant(unsigned int row);
        void hold(unsigned int row=0, unsigned int col=0);
};

#endif
