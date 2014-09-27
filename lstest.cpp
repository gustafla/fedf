#include <iostream>
#include <sys/types.h>
#include <dirent.h>

int main (){
    DIR* dp;
    dirent* ep;

    dp = opendir("./characters/");
    if (dp != NULL) {
        while (ep = readdir(dp))
            std::cout << ep->d_name << std::endl;
        closedir(dp);
    } else
        std::cout << "Couldn't open directory.\n";
    return 0;
}

