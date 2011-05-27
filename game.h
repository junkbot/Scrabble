// game.h
// 7 May 2011, Joshua Lau
// Interface to run a scrabble game
// v0.1 (alpha): no blanks, no crosswords, letters worth equal

// Enough functions to run a scrabble game; no breaking abstraction

/* Other included files */


/* Constants */

#define TRUE 1
#define FALSE 0

#define BOARD_SIZE 15
#define RACK_SIZE 7

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

// (struct) Typedefs
typedef int bool;

// board representation

// letters are placed in the board.
// 'A' <= letter <= 'Z'
// anything outside this range represents an invalid letter
// and can be used as a placeholder for an empty board square
typedef char letter;

// stores the letters on the board
typedef letter board[BOARD_SIZE][BOARD_SIZE];

// move

// a single word played on the board
typedef letter word[BOARD_SIZE];
typedef letter *wordRef;

// direction a word is played in
typedef bool direction;


// racks are a string (essentially)
typedef letter rack[RACK_SIZE];
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

// returns the number of tiles on the rack
int rackSize(rackRef rackToCheck);

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

/* Global Vars */
int letterValues[NUM_LETTERS] = {1,3,3,2,1,4,2,4,1,8,5,1,3,
                                 1,1,3,10,1,1,1,1,4,4,8,4,10};
