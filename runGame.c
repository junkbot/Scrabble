// runGame.c
// 4 June 2011, Joshua Lau
// Main module which runs the scrabble game

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "runGame.h"

#include "game.h"

#ifndef __TRIE
#include "trie.h"
#endif

#include "ai.h"

#define DEBUG
#ifdef DEBUG
#define D(x...) fprintf(stderr,x)
#else
#define D(x...)
#endif

#define HUMAN 0
#define AI 1

// command-line prints
#define EMPTY_SQUARE_CHAR "."

#define DICTIONARY_FILE "sowpods.txt"

#define HUMAN_MOVE_PROMPT "Enter turn [row] [col] [word] [horizontal:0|vertical:1] or -1 to pass: "

// globals

void initialiseGame(void);
void printAll(void);
void playTurn(void);

static void clearRacks(void);
static void dealRacks(void);
static void placeFirstTile(void);
static void initDict(void);

static void printBoard(void);
static void printScores(void);
static void printRacks(void);

player currentPlayer;
int numConsecPasses;

void runGame(void) {
    
    initialiseGame();

    int turnNumber = 0;
    numConsecPasses = 0;

    while(numConsecPasses < NUM_PLAYERS) {
        D("NEXT TURN!\n");
        currentPlayer = turnNumber % NUM_PLAYERS;
        printAll();

        if(currentPlayer == AI) {
            makeMove(currentPlayer, getPlayerRack(currentPlayer), getDictTrie());
            D("got here!\n");
        } else {
            playTurn();
        }

        turnNumber++;
    }
}

void initialiseGame(void) {
    initDict();

    shuffleBag();
    resetBag();

    clearRacks();
    dealRacks();

    resetBoard();
    placeFirstTile();
}

void printAll(void) {
    printBoard();
    printf("\n");
    printRacks();
    printf("\n");
    printScores();
    printf("\n");
    printf("%d tiles remaining in the bag\n",numTilesRemaining());
    printf("\n");
}

void playTurn(void) { 
    bool moveLegal = FALSE;
    while(moveLegal != TRUE) {

        printf(HUMAN_MOVE_PROMPT);

        int row, col;
        word wordToPlay;
        direction dir;
        
        scanf("%d",&row);

        if(row == PASS) {
            D("passed!\n");
            playMove(currentPlayer,PASS,PASS,NULL,HORIZONTAL);
            D("processed pass\n");
            numConsecPasses++;
            moveLegal = TRUE;
        } else {
            numConsecPasses = 0;
            scanf("%d",&col);

            scanf(" %s ",wordToPlay);
            scanf("%d",&dir);

            int wordLength = strlen(wordToPlay);

            int i;
            for(i=0;i<wordLength;i++) {
                wordToPlay[i] = toupper(wordToPlay[i]);
            }

#ifdef DEBUG
            printf("%s @ (%d,%d) going %d\n",wordToPlay,row,col,dir);
#endif

            if(isLegalMove(currentPlayer,row,col,wordToPlay,dir)) {
                D("before play\n");
                playMove(currentPlayer,row,col,wordToPlay,dir);
                D("after play\n");
                moveLegal = TRUE;
            } else {
                printf("Illegal move.\n");
            }
        }
    }
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

static void placeFirstTile(void) {
    int midR = BOARD_SIZE/2;
    int midC = BOARD_SIZE/2;

    letter letterToPlace = getNextTile();
    setCell(midR, midC, letterToPlace);
}

static void initDict(void) {
    FILE *dictionaryFile = fopen(DICTIONARY_FILE,"r");
    Trie dictTrie = trieFromFile(dictionaryFile);
    setDictTrie(dictTrie);
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
//            D("letter @ (%d,%d) is '%c'\n",i,j,curLetter);
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
