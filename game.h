// game.h
// 7 May 2011, Joshua Lau
// Interface to run a scrabble game
// v0.1 (alpha): no blanks, no crosswords, letters worth equal

// Enough functions to run a scrabble game; no breaking abstraction

/* Other included files */


/* Constants */

#ifndef __TRIE
#include "trie.h"
#endif

#ifndef TRUE
typedef int bool;
#define TRUE 1
#define FALSE 0
#endif

#define BOARD_SIZE 15

#define RACK_SIZE 7
#define TOTAL_TILES 98

#define NUM_PLAYERS 2
#define MAX_PLAYERS 4

#define HUMAN 0
#define BOT 1

#define HORIZONTAL 0
#define VERTICAL 1

#define FIRST_LETTER 'A'
#define LAST_LETTER 'Z'
#define INVALID_LETTER 0
#define NUM_LETTERS (LAST_LETTER - FIRST_LETTER + 1)

#define INITIAL -1
#define ILLEGAL_MOVE_SCORE (-(1<<30))

#define FULL_RACK_BONUS 50

#define PASS -1

// (struct) Typedefs

// board representation

// letters are placed in the board.
// 'A' <= letter <= 'Z'
// anything outside this range represents an invalid letter
// and can be used as a placeholder for an empty board square
typedef char letter;

// stores the letters on the board
typedef letter board[BOARD_SIZE+1][BOARD_SIZE+1];

// move

// a single word played on the board
typedef letter word[BOARD_SIZE];
typedef letter *wordRef;

// direction a word is played in
typedef bool direction;


// racks are a string (essentially)
typedef letter rack[RACK_SIZE+1];
typedef letter *rackRef;


// player
typedef int player;

/* Prototypes */

// board related

// clears the current board
void resetBoard(void);

// places a letter in the given cell
void setCell(int row, int col, letter tileToPlace);

// returns the letter at the given cell if non-empty,
// a non-letter otherwise
letter getCell(int row, int col);


// rack related

// for a given player, returns a (pointer to) rack
rackRef getPlayerRack(player playerNum);

// clears a rack
void clearRack(rackRef rackToClear);

// given a letter, delete one occurence of it from the rack
// and maintain sorted order
// PRECONDITION: letter appears on the rack
void deleteSingleLetterFromRack(rackRef rackToChange, letter letterToRemove);

// given a letter, add one of it to the rack
// PRECONDITION: rack is not full
// maintain sorted order
void addSingleLetterToRack(rackRef rackToChange, letter letterToAdd);

// returns the number of tiles on the rack
int rackSize(rackRef rackToCheck);

// bag related

// randomly shuffles the bag and resets the bag counter
void shuffleBag(void);

// returns the next tile in the bag and increments the bag counter
letter getNextTile(void);

// returns the number of tiles left in the bag
int numTilesRemaining(void);

// player related

// returns the score of the given player
int getScore(player playerToCheck);

// sets the score of the given player
void setScore(player playerToModify, int scoreToSet);

// move related

// returns TRUE if the given move is legal
// FALSE otherwise
bool isLegalMove(player playerToMove, int row, int col, wordRef wordToPlay,
                 direction dirToMove);

// if move is legal, returns the score of the move without playing it
// returns ILLEGAL_MOVE_SCORE, otherwise
int scoreMove(player playerToMove, int row, int col, wordRef wordToPlay,
              direction dirToMove);

void playMove(player playerToMove, int row, int col, wordRef wordToPlay,
              direction dirToMove);

// dictionary
void setDictTrie(Trie trieToSet);

/* Global Vars */
extern letter tileBag[TOTAL_TILES];
