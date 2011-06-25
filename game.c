// game.c
// 7 May 2011, Joshua Lau
// Implementation of the interface

#include "game.h"

#ifndef __TRIE
#include "trie.h"
#endif

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//#define DEBUG
#ifdef DEBUG
#include <stdio.h>
#define D(x...) fprintf(stderr,x)
#else
#define D(x...)
#endif

/* GLOBALS */
int letterValues[NUM_LETTERS] = {1,3,3,2,1,4,2,4,1,8,5,1,3,
								 1,1,3,10,1,1,1,1,4,4,8,4,10};

int nextTile;
letter tileBag[TOTAL_TILES] = "AAAAAAAAAB"
                              "BCCDDDDEEE"
                              "EEEEEEEEEF"
                              "FGGGHHIIII"
                              "IIIIIJKLLL"
                              "LMMNNNNNNO"
                              "OOOOOOOPPQ"
                              "RRRRRRSSSS"
                              "TTTTTTUUUU"
                              "VVWWXYYZ";

// holds the current gameboard
board gameBoard;

// one rack for each player
rack playerRacks[MAX_PLAYERS];

// holds the game dictionary
Trie dict;

// scores of each player
// TODO: implement a void resetScores(void) function
// TODO: implement a void resetGame(void) function
int playerScores[MAX_PLAYERS];

/* STATIC FUNCTION PROTOTYPES */
static int numCharInString(char charToCount, char *stringToSearch);
static int mapLetterToInt(letter letterToMap);
static bool isValidLetter(letter letterToCheck);

static int scoreLetter(letter letterToScore);

static void sortRack(rackRef rackToSort);

// comparison function for sorting alphabetically
// returns:
// < 0 if a < b
// 0 if a = b
// > 0 if a > b
static int alphabeticCompare(const void *a, const void *b);

/* IMPLEMENTATION */

void resetBoard(void) {
    int i,j;
    for(i=0;i<BOARD_SIZE+1;i++) {
        for(j=0;j<BOARD_SIZE+1;j++) {
            // can't use set cell for an invalid letter
            gameBoard[i][j] = INVALID_LETTER;
        }
    }
}

void setCell(int row, int col, letter tileToPlace) {
    D("setCell(%d,%d,'%c')\n",row,col,tileToPlace);

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

void clearRack(rackRef rackToClear) {
	int i;
	for(i=0;i<RACK_SIZE;i++) {
		rackToClear[i] = INVALID_LETTER;
	}
}

bool isInRack(rackRef rackToSearch, letter letterTarget) {
    assert(rackToSearch != NULL);
    assert(FIRST_LETTER <= letterTarget);
    assert(letterTarget <= LAST_LETTER);

    int numLettersOnRack = rackSize(rackToSearch);

    int i;
    for(i=0;i<numLettersOnRack;i++) {
        if(rackToSearch[i] == letterTarget) {
            return TRUE;
        }
    }
    return FALSE;
}

void deleteSingleLetterFromRack(rackRef rackToChange, letter letterToRemove) {
	assert(FIRST_LETTER <= letterToRemove);
	assert(letterToRemove <= LAST_LETTER);
	assert(rackToChange != NULL);

	int foundPos = INITIAL;
	int numLettersOnRack = rackSize(rackToChange);

	int i;
	for(i=0;i<numLettersOnRack;i++) {
		if(rackToChange[i] == letterToRemove) {
			foundPos = i;
		}
	}

	assert(foundPos != INITIAL);

	for(i=foundPos+1;i<numLettersOnRack;i++) {
		rackToChange[i-1] = rackToChange[i];
	}

	rackToChange[numLettersOnRack-1] = INVALID_LETTER;

    sortRack(rackToChange);
}

void addSingleLetterToRack(rackRef rackToChange, letter letterToAdd) {
	assert(FIRST_LETTER <= letterToAdd);
	assert(letterToAdd <= LAST_LETTER);
	assert(rackToChange != NULL);
	
	int numLettersOnRack = rackSize(rackToChange);
	assert(numLettersOnRack < RACK_SIZE);

	rackToChange[numLettersOnRack] = letterToAdd;

    sortRack(rackToChange);
}

int rackSize(rackRef rackToCheck) {
    assert(rackToCheck != NULL);
    return strlen(rackToCheck);
}

// bag

void shuffleBag(void) {
    srand( time(NULL) );

    int i;
    for(i=0;i<TOTAL_TILES;i++) {
        int swapPos = (rand() % (TOTAL_TILES-i))+i;
        letter temp = tileBag[i];
        tileBag[i] = tileBag[swapPos];
        tileBag[swapPos] = temp;
    }

#ifdef DEBUG
    for(i=0;i<TOTAL_TILES;i++) {
        D("%c ",tileBag[i]);
    }
    D("\n");
#endif
}

void resetBag(void) {
    nextTile = 0;
}

letter getNextTile(void) {
    assert(nextTile != TOTAL_TILES);

    nextTile++;
    return tileBag[nextTile-1];
}

int numTilesRemaining(void) {
    assert(nextTile <= TOTAL_TILES);
    return TOTAL_TILES - nextTile;
}

// returns the score of the given player
int getScore(player playerToCheck) {
    assert(playerToCheck >= 0);
    assert(playerToCheck < NUM_PLAYERS);

    return playerScores[playerToCheck];
}

// sets the score of the given player
void setScore(player playerToModify, int scoreToSet) {
    assert(playerToModify >= 0);
    assert(playerToModify < NUM_PLAYERS);
    assert(scoreToSet >= 0);

    playerScores[playerToModify] = scoreToSet;
}

// move

bool isLegalMove(player playerToMove, int row, int col, wordRef wordToPlay,
                 direction dirToMove) {
    assert(wordToPlay != NULL);
    assert(playerToMove >= 0);
    assert(playerToMove < NUM_PLAYERS);

//    D("isLegalMove: %s\n",wordToPlay);

    if(row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        printf("co-ordinates out of bounds.\n");
        return FALSE;
    }

    int wordLength = strlen(wordToPlay);

    assert(wordLength > 0);

    rackRef currentPlayerRack = getPlayerRack(playerToMove);

    // iterators
    int i;
    letter j;

    // for each letter in the word, check that it is a valid letter
    for(i=0;i<wordLength;i++) {
        if(isValidLetter(wordToPlay[i]) == FALSE) {
            printf("invalid letter: '%c'\n",wordToPlay[i]);
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
        printf("word is too long.\n");
        return FALSE;
    }

    // - count the number of each letter in the word that is not already on the
    //   board.
    // - also check that any existing tiles on the board that are where the
    //   word will lie match the corresponding letter in the word
    int numOfEachLetter[NUM_LETTERS] = {0};

    // check if any of the letters is on the board
    bool oneLetterOnBoard = FALSE;

    // check if a tile from the rack is being used
    bool oneLetterFromRack = FALSE;

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

            oneLetterFromRack = TRUE;
    //        D("oneLetterFromRack = %d\n",oneLetterFromRack);

            // only add because it was a blank square
            numOfEachLetter[letterMapping]++;
        } else {
            oneLetterOnBoard = TRUE;
            // existing tile on board
            if(curLetterOnBoard != wordToPlay[i]) {
                // check it matches
                printf("letter %d doesn't match board tile.\n",i+1);
                return FALSE;
            }
        }
        
        // check that no crosswords are formed
        int dx[] = {0,0,1,-1};
        int dy[] = {1,-1,0,0};

        int j = INITIAL;
        if(dirToMove == HORIZONTAL) {
            j = 0;
        } else if(dirToMove == VERTICAL) {
            j = 2;
        }

        int stop = j+2;
        for(;j<stop;j++) {
            int lr = letterRow + dy[j];
            int lc = letterCol + dx[j];
//            D("cur (%d,%d): '%c': %d checking (%d,%d): '%c': %d\n",letterRow,letterCol,getCell(letterRow,letterCol),isValidLetter(getCell(letterRow,letterCol)),lr,lc,getCell(lr,lc),isValidLetter(getCell(lr,lc)));
            if(lr >= 0 && lr < BOARD_SIZE && lc >= 0 && lc < BOARD_SIZE &&
               isValidLetter( getCell(letterRow,letterCol) ) == FALSE &&
               isValidLetter( getCell(lr,lc) ) != FALSE) {
                // crossword formed
                printf("crossword formed with (%d,%d)\n",lr,lc);
                return FALSE;
            }
        }
    }

    if(oneLetterOnBoard == FALSE) {
        // no letters on the board
        printf("not linked to any existing letters on board\n");

        return FALSE;
    }


//    D("oneLetterFromRack = %d\n",oneLetterFromRack);
    if(oneLetterFromRack == FALSE) {
        printf("no letters from rack used!\n");

        return FALSE;
    }

    for(j=FIRST_LETTER;j<=LAST_LETTER;j++) {
        int numOfJInWord = numOfEachLetter[ mapLetterToInt(j) ];
        int numOfJOnRack = numCharInString(j, currentPlayerRack);

        if(numOfJInWord > numOfJOnRack) {
            printf("not enough of %c on rack\n",j);
            return FALSE;
        }
    }

    // check if the words is in the dictionary

    if(hasWord(dict, wordToPlay) == FALSE) {
        printf("'%s' not in the dictionary\n",wordToPlay);
        return FALSE;
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
    if(row != PASS) {
        assert(wordToPlay != NULL);
        assert(playerToMove >= 0);
        assert(playerToMove < NUM_PLAYERS);
        assert(isLegalMove(playerToMove,row,col,wordToPlay,dirToMove));

        int wordLength = strlen(wordToPlay);

        // place each tile on the board
        int numTilesPlaced = 0;

        int moveScore = scoreMove(playerToMove, row, col, wordToPlay, dirToMove);

        int i;
        for(i=0;i<wordLength;i++) {
            int r = row;
            int c = col;

            if(dirToMove == HORIZONTAL) {
                c += i;
            } else if(dirToMove == VERTICAL) {
                r += i;
            }
            
            // current letter on the board
            letter curLetter = getCell(r,c);
            if(isValidLetter(curLetter) == FALSE) {
                // blank square
                setCell(r,c,wordToPlay[i]);
                
                deleteSingleLetterFromRack( getPlayerRack(playerToMove),
                                            wordToPlay[i] );

                numTilesPlaced++;

                // check if there's still any tiles left
                if(numTilesRemaining() > 0) {
                    addSingleLetterToRack( getPlayerRack(playerToMove),
                                           getNextTile() );
                }
            }
        }

        if(numTilesPlaced == RACK_SIZE) {
            moveScore += FULL_RACK_BONUS;
        }

        // adjust scores
        setScore( playerToMove, getScore(playerToMove) + moveScore );
    }
}

// dict
void setDictTrie(Trie trieToSet) {
    dict = trieToSet;
}

Trie getDictTrie(void) {
    return dict;
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
    return (letterToCheck >= FIRST_LETTER && letterToCheck <= LAST_LETTER);
}

static int scoreLetter(letter letterToScore) {
    assert(letterToScore >= FIRST_LETTER);
    assert(letterToScore <= LAST_LETTER);

    return letterValues[mapLetterToInt(letterToScore)];
}

static void sortRack(rackRef rackToSort) {
    assert(rackToSort != NULL);

    int rackLength = rackSize(rackToSort);
    qsort( rackToSort, rackLength, sizeof(letter), alphabeticCompare );
}

static int alphabeticCompare(const void *a, const void *b) {
    return ( *(letter*)a - *(letter*)b );
}
