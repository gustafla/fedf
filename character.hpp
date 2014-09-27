#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <SDL/SDL.h>
#include <string>

class Character {
	public:
		std::string name;
		SDL_Surface* pic;
		Character(std::string name, std::string picf);
};

#endif
