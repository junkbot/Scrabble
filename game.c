// game.c
// 7 May 2011, Joshua Lau
// Implementation of the interface

#include "game.h"

#include <assert.h>
#include <string.h>

/* GLOBALS */

// holds the current gameboard
board gameBoard;

// one rack for each player
rack playerRacks[MAX_PLAYERS];

// scores of each player
// TODO: implement a void resetScores(void) function
// TODO: implement a void resetGame(void) function
int playerScores[MAX_PLAYERS];

/* STATIC FUNCTION PROTOTYPES */
static int numCharInString(char charToCount, char *stringToSearch);
static int mapLetterToInt(letter letterToMap);
static bool isValidLetter(letter letterToCheck);

static int scoreLetter(letter letterToScore);

/* IMPLEMENTATION */

void resetBoard(void) {
    int i,j;
    for(i=0;i<BOARD_SIZE;i++) {
        for(j=0;j<BOARD_SIZE;j++) {
            // can't use set cell for an invalid letter
            gameBoard[i][j] = INVALID_LETTER;
        }
    }
}

void setCell(int row, int col, letter tileToPlace) {
    assert(row >= 0);
    assert(row < BOARD_SIZE);
    assert(col >= 0);
    assert(col < BOARD_SIZE);
    assert(FIRST_LETTER <= tileToPlace);
    assert(tileToPlace <= LAST_LETTER);

    gameBoard[row][col] = tileToPlace;
}

letter getCell(int row, int col) {
    assert(row >= 0);
    assert(row < BOARD_SIZE);
    assert(col >= 0);
    assert(col < BOARD_SIZE);

    return gameBoard[row][col];
}

rackRef getPlayerRack(player playerNum) {
    assert(playerNum >= 0);
    assert(playerNum < NUM_PLAYERS);

    return playerRacks[playerNum];
}

int rackSize(rackRef rackToCheck) {
    assert(rackToCheck != NULL);
    return strlen(rackToCheck);
}

bool isLegalMove(player playerToMove, int row, int col, wordRef wordToPlay,
                 direction dirToMove) {
    assert(wordToPlay != NULL);
    assert(playerToMove >= 0);
    assert(playerToMove < NUM_PLAYERS);

    if(row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return FALSE;
    }

    int wordLength = strlen(wordToPlay);
    rackRef currentPlayerRack = getPlayerRack(playerToMove);

    // iterators
    int i;
    letter j;

    // for each letter in the word, check that it is a valid letter
    for(i=0;i<wordLength;i++) {
        if(isValidLetter(wordToPlay[i]) == FALSE) {
            return FALSE;
        }
    }

    // check that the word is within the limits of the board
    int lastCell = INITIAL;
    if(dirToMove == HORIZONTAL) {
        lastCell = col + wordLength - 1;
    } else if(dirToMove == VERTICAL) {
        lastCell = row + wordLength - 1;
    }

    if(lastCell >= BOARD_SIZE) {
        return FALSE;
    }

    // - count the number of each letter in the word that is not already on the
    //   board.
    // - also check that any existing tiles on the board that are where the
    //   word will lie match the corresponding letter in the word
    int numOfEachLetter[NUM_LETTERS] = {0};

    for(i=0;i<wordLength;i++) {
        int letterRow = row;
        int letterCol = col;

        if(dirToMove == HORIZONTAL) {
            letterCol += i;
        } else if(dirToMove == VERTICAL) {
            letterRow += i;
        }

        assert(letterRow >= 0);
        assert(letterRow < BOARD_SIZE);
        assert(letterCol >= 0);
        assert(letterCol < BOARD_SIZE);

        letter curLetterOnBoard = getCell(letterRow, letterCol);
        if(isValidLetter(curLetterOnBoard) == FALSE) {
            // blank square
            int letterMapping = mapLetterToInt(wordToPlay[i]);
            numOfEachLetter[letterMapping]++;
        } else {
            // existing tile on board
            if(curLetterOnBoard != wordToPlay[i]) {
                return FALSE;
            }
        }
    }

    for(j=FIRST_LETTER;j<=LAST_LETTER;j++) {
        int numOfJInWord = numCharInString(j, wordToPlay);
        int numOfJOnRack = numCharInString(j, currentPlayerRack);

        if(numOfJInWord > numOfJOnRack) {
            return FALSE;
        }
    }

    // passed all tests
    return TRUE;
}

int scoreMove(player playerToMove, int row, int col, wordRef wordToPlay,
              direction dirToMove) {
    assert(wordToPlay != NULL);
    assert(playerToMove >= 0);
    assert(playerToMove < BOARD_SIZE);

    if( isLegalMove(playerToMove,row,col,wordToPlay,dirToMove) ) {
        /* TODO:
         *   - Currently only sums the score of each tile in the WORD
         *     (including those that were already on the board)
         *   - TODO: count crosswords as well
         */

        int wordLength = strlen(wordToPlay);
        int i;
        int score = 0;
        for(i=0;i<wordLength;i++) {
            score += scoreLetter(wordToPlay[i]);
        }

        return score;
    } else {
        return ILLEGAL_MOVE_SCORE;
    }
}

void playMove(player playerToMove, int row, int col, wordRef wordToPlay,
              direction dirToMove) {
    assert(wordToPlay != NULL);
    assert(playerToMove >= 0);
    assert(playerToMove < BOARD_SIZE);
    assert(isLegalMove(playerToMove,row,col,wordToPlay,dirToMove));

    // place each tile on the board
}

/* STATIC FUNCTION IMPLEMENTATIONS */
static int numCharInString(char charToCount, char *stringToSearch) {
    assert(stringToSearch != NULL);
    int stringLength = strlen(stringToSearch);

    int numOfChar = 0;

    int i;
    for(i=0;i<stringLength;i++) {
        if(stringToSearch[i] == charToCount) {
            numOfChar++;
        }
    }
    
    return numOfChar;
}

static int mapLetterToInt(letter letterToMap) {
    assert(letterToMap >= FIRST_LETTER);
    assert(letterToMap <= LAST_LETTER);

    return (int)(letterToMap - FIRST_LETTER);
}

static bool isValidLetter(letter letterToCheck) {
    return (letterToCheck >= FIRST_LETTER && letterToCheck >= LAST_LETTER);
}

static int scoreLetter(letter letterToScore) {
    assert(letterToScore >= FIRST_LETTER);
    assert(letterToScore <= LAST_LETTER);

    return letterValues[mapLetterToInt(letterToScore)];
}
