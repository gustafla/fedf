#ifndef SOUNDS_HPP
#define SOUNDS_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <map>
#include <string>
#include "config.hpp"

class GameSounds {
	public:
		~GameSounds();
		void play(std::string name);
	private:
		std::map<std::string, Mix_Chunk*> sounds;
};

#endif
