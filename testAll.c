// testAll.c
// 7 May 2011, Joshua Lau
// Tests all modules

#include <stdio.h>

#include "testAll.h"
#include "testGame.h"
#include "testTrie.h"

#define D(x...) fprintf(stderr,x)

int main(int argc, char *argv[]) {
    testAll();
    return 0;
}

void testAll(void) {
    D("Testing all modules...\n");
    testGame();
    testTrie();
    D("All modules... PASSED!\n");
}
