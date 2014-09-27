#include "character.hpp"
#include "gfxutil.hpp"

Character::Character(std::string iname, std::string picf):
name(iname) {
	file2surface(picf, &pic);
}
