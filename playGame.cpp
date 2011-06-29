// playGame.cpp
// 29 June 2011, Joshua Lau
// Facilitates game through a GUI

#include <cstdlib>

extern "C" {
    #include "runGame.h"
}

int main(int argc, char *argv[]) {
    runGame();

    return EXIT_SUCCESS;
}
