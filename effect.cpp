#include "effect.hpp"
#include <iostream>

Effect::Effect(GameData* igameData, std::string dir, bool right, int ix, int iy):
gameData(igameData),
done(false),
x(ix),
y(iy) {
    if (right)
        sprite = new AnimatedSprite(gameData, dir, CHARACTER_RIGHT_SPRITE_FILE, 64, 64, &done);
    else
        sprite = new AnimatedSprite(gameData, dir, CHARACTER_LEFT_SPRITE_FILE, 64, 64, &done);
    sprite->doOnce(0);
}

Effect::~Effect() {
    delete sprite;
}

bool Effect::draw() {
    SDL_Rect sr = sprite->getFrame();
    SDL_Rect dr = buildRect(x, y, 64, 64);
    SDL_BlitSurface(sprite->getSurface(), &sr, gameData->buffer, &dr);
    return done;
}
