// Copyright 2014, 2015 Lauri Gustafsson, Kalle Korhonen and Julius Heino
/*
This file is part of Fedora Fighters.

    Fedora Fighters is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Fedora Fighters is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Fedora Fighters, see COPYING. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <vector>
#include <SDL/SDL.h>

bool isdigits(std::string &s);
bool isfloat(std::string &s);
void checkValueParam(int n, int argc, char* argv[]);
void checkValueParamf(int n, int argc, char* argv[]);
bool loadFile(std::string inFileName, std::string& outString, int linesize=4096);
bool fexists(std::string const);
SDL_Rect buildRect(int x, int y, int w, int h);
bool listDir(std::string dir, std::vector<std::string>* names);
float clamp(float x, float a=0.0, float b=1.0);

#endif
