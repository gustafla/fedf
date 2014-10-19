#ifndef EFFECT_HPP
#define EFFECT_HPP

#include <SDL/SDL.h>
#include <string>
#include "animated_sprite.hpp"
#include "game_data.hpp"
#include "config.hpp"
#include "util.hpp"

class Effect {
    public:
        Effect(GameData* igameData, std::string dir, bool right, int ix, int iy);
        ~Effect();
        bool draw();
        bool done;
    private:
        AnimatedSprite* sprite;
        GameData* gameData;
        int x;
        int y;
};

#endif
