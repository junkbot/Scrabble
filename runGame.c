// runGame.c
// 4 June 2011, Joshua Lau
// Main module which runs the scrabble game

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game.h"
#include "trie.h"

#define DEBUG
#ifdef DEBUG
#define D(x...) fprintf(stderr,x)
#else
#define D(x...)
#endif

// command-line prints
#define EMPTY_SQUARE_CHAR "."

// globals

void initialiseGame(void);
void printAll(void);

static void clearRacks(void);
static void dealRacks(void);

static void printBoard(void);
static void printScores(void);
static void printRacks(void);

int main(int argc, char *argv[]) {

    initialiseGame();
    printAll();

	return EXIT_SUCCESS;
}

void initialiseGame(void) {
    shuffleBag();

    clearRacks();
    dealRacks();

    resetBoard();
}

void printAll(void) {
    printBoard();
    printf("\n");
    printRacks();
    printf("\n");
    printScores();
    printf("\n");
    printf("%d tiles remaining in the bag\n",numTilesRemaining());
}

static void clearRacks(void) {
	// clear the racks then distribute tiles
	int i;
	for(i=0;i<NUM_PLAYERS;i++) {
		clearRack( getPlayerRack(i) );
	}
}

static void dealRacks(void) {
    int i;
	// distribute alternate tiles to alternate players
	for(i=0;i<NUM_PLAYERS*RACK_SIZE;i++) {
        addSingleLetterToRack( getPlayerRack(i % NUM_PLAYERS), getNextTile() );
	}
}

static void printBoard(void) {
    int i, j;

    printf(" ");
    for(i=0;i<BOARD_SIZE;i++) {
        printf(" %d",i%10);
    }
    printf("\n");

    for(i=0;i<BOARD_SIZE;i++) {
        
        printf("%d ",i%10);

        for(j=0;j<BOARD_SIZE;j++) {
            if(j > 0) {
                printf(" ");
            }

            letter curLetter = getCell(i, j);
            if(curLetter == INVALID_LETTER) {
                printf(EMPTY_SQUARE_CHAR);
            } else {
                printf("%c", curLetter);
            }
        }
        printf("\n");
    }
}

static void printScores(void) {
    printf("SCORES\n");
    printf("======\n");
    int i;
    for(i=0;i<NUM_PLAYERS;i++) {
        printf("player %d: %d\n",i,getScore(i));
    }
}

static void printRacks(void) {
    printf("RACKS\n");
    printf("=====\n");
    int i;
    for(i=0;i<NUM_PLAYERS;i++) {
        printf("player %d: %s\n",i,getPlayerRack(i));
    }
}
