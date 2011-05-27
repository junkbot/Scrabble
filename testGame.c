// testGame.c
// 7 May 2011, Joshua Lau
// Tests the game functions (implementation)

#include <stdio.h>
#include <assert.h>

#include "testGame.h"
#include "game.h"

#define T(x...) fprintf(stderr,x)

#ifdef EXTEND
#define E(x...) fprintf(stderr,x)
#else
#define E(x...)
#endif

void testGame(void) {
    T("\n* Testing testGame...\n");

    testSetCellGetCell();
    testResetBoard();

    T("\n* testGame... PASSED!\n\n");
}

void testSetCellGetCell(void) {
    T("\n* Testing setCell and getCell...\n");

    // comprehensive tests
    int i, j;
    letter k;
    for(i=0;i<BOARD_SIZE;i++) {
        for(j=0;j<BOARD_SIZE;j++) {
            for(k=FIRST_LETTER;k<=LAST_LETTER;k++) {
                E("** setCell(%d, %d, '%c')\n", i, j, k);
                setCell(i, j, k);

                E("** checking getCell(%d, %d) == '%c')... got '%c'\n",
                  i,j,k,getCell(i,j));
                assert( getCell(i,j) == k );
            }
        }
    }

    T("* setCell and getCell... PASSED!\n");
}

void testResetBoard(void) {
    T("\n* Testing resetBoard...\n");

    // fill up the entire grid and call reset and see if they're all blank
    int i, j;
    for(i=0;i<BOARD_SIZE;i++) {
        for(j=0;j<BOARD_SIZE;j++) {
            setCell(i,j,FIRST_LETTER);
            E("** setCell(%d, %d, '%c')\n",i,j,FIRST_LETTER);
        }
    }

    E("** cleared board!\n");
    resetBoard();

    for(i=0;i<BOARD_SIZE;i++) {
        for(j=0;j<BOARD_SIZE;j++) {
            // check that it's out of range
            E("** checking getCell(%d,%d) is an invalid letter... got '%c'\n",
               i,j,getCell(i,j));
            assert( getCell(i,j) < FIRST_LETTER || getCell(i,j) > LAST_LETTER );
        }
    }
    T("* resetBoard... PASSED!\n");
}
