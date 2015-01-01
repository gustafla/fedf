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

#include <cstdlib>
#include <cstring>
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <string>
#include <SDL/SDL.h>
#include <vector>
#include <sys/types.h>
#include <dirent.h>

#define ARGERR "Go fix ur parameters\n"

using namespace std;

bool fexists (std::string const name) {
    struct stat t;
    return (stat(name.c_str(), &t) == 0);
}

bool isdigits(std::string &s)
{
    return s.find_first_not_of("0123456789") == std::string::npos;
}

bool isfloat(std::string &s)
{
    return s.find_first_not_of("0123456789.") == std::string::npos;
}

void checkValueParam(int n, int argc, char* argv[])
{
    if (n==argc)
    {
        cout << ARGERR;
        exit(20);
    }

    string tmps = argv[n];

    if (!isdigits(tmps))
    {
        cout << ARGERR;
        exit(30);
    }
}

bool loadFile(std::string inFileName, std::string& outString, int linesize)
{
    std::ifstream inFile(inFileName.c_str());
    if (!inFile.is_open())
    {
        std::cout << inFileName << ": No such file\n";
        return false;
    }

    char tmp[(linesize)];

    while (!inFile.eof())
    {
        inFile.getline(tmp, linesize);
        outString += tmp;
        outString += "\n";
    }
    return true;
}

void checkValueParamf(int n, int argc, char* argv[])
{
    if (n==argc)
    {
        cout << ARGERR;
        exit(21);
    }

    string tmps = argv[n];

    if (!isfloat(tmps))
    {
        cout << ARGERR;
        exit(31);
    }
}

SDL_Rect buildRect(int x, int y, int w, int h) {
    SDL_Rect t;
    t.x=x;
    t.y=y;
    t.w=w;
    t.h=h;
    return t;
}

bool listDir(std::string dir, std::vector<std::string>* names) {
	DIR* dp;
	dirent *ep;
	std::string tmp;
	
	dp = opendir(dir.c_str());
	if (dp != NULL) {
		while (ep = readdir(dp)) {
			tmp = ep->d_name;
			if (tmp != "." && tmp != "..")
				names->push_back(tmp);
		}
		closedir(dp);
	} else
		return false;
	return true;
}

float clamp(float x, float a, float b){
    return ((x < a) ? a : ((x > b) ? b : x));
}
