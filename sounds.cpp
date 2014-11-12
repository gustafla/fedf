#include "sounds.hpp"
#include <iostream>
#include "config.hpp"

void GameSounds::play(std::string name) {
	if (sounds.find(name) == sounds.end()) {
        sounds[name] = Mix_LoadWAV((SOUNDS_DIR + name).c_str());
    }
    Mix_PlayChannel(-1, sounds[name], 0);
}

GameSounds::~GameSounds() {
	for (std::map<std::string, Mix_Chunk*>::iterator itr = sounds.begin(); itr != sounds.end(); itr++) {
		Mix_FreeChunk(itr->second);
	}
}
