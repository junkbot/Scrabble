// ai.c
// 25 June 2011, Joshua Lau
// Plays the game

// Algorithm taken from "The World's Fastest Scrabble Program"
// ACM, 1988, Andrew W Appel and Guy J Jacobson

#include <string.h>
#include <assert.h>

#include "ai.h"

#ifndef __GAME
#include "game.h"
#endif

// slight haax that use the trie (breaks abstraction a little bit)
#ifndef __TRIE
#include "trie.h"
#endif

#define DEBUG
#ifdef DEBUG
#define D(x...) fprintf(stderr,x)
#else
#define D(x...)
#endif

// an anchor is a square that is adjacent to an existing square on the baord
bool anchors[BOARD_SIZE][BOARD_SIZE];

// me
player me;

// my rack
rack myRack;

// the dictionary
Trie dict;

// best move so far
int bestRow, bestCol, bestScore;
word bestWord;
direction bestDir;

// takes a possible move and handles it
static void handlePossibleMove(int row, int col, wordRef wordToPlay,
                               direction dirToMove);

// limit is the maximum number of squares we can extend in the "left" direction
// expandDir is the direction in which we are expanding
static void leftPart(wordRef partialWord, Trie trieNode, int limit,
                     direction expandDir, int anchorRow, int anchorCol);

// finds all the anchors
static void findAnchors(direction dir);

// as above, without limit
// NOTE: ensure to check that we don't overflow the board
static void extendRight(wordRef partialWord, Trie trieNode,
                        direction expandDir, int row, int col,
                        bool coveredAnchor);

// finds the maximum "left" squares that can be expanded
static int findLimit(int row, int col, direction expandDir);

void makeMove(player you, rackRef yourRack, Trie dictionary) {
    strncpy(myRack, yourRack, RACK_SIZE+1);
    me = you;
    bestScore = INITIAL;
    dict = dictionary;

    word emptyWord = {INVALID_LETTER};
    word startingPartialWord = {INVALID_LETTER};

    int i, j;
    direction d;
    for(d=HORIZONTAL;d<=VERTICAL;d++) {
        D("DIRECTION: %d\n",d);
        findAnchors(d);

        for(i=0;i<BOARD_SIZE;i++) {
            for(j=0;j<BOARD_SIZE;j++){
                strncpy(startingPartialWord, emptyWord, BOARD_SIZE);
                if(anchors[i][j]) {
//                if(getCell(i,j) != INVALID_LETTER) {
                    D("finding word from (%d,%d)\n",i,j);
                    int limit = findLimit(i, j, d);
                    if(limit > 0) {
                        leftPart(startingPartialWord, dict,
                                 findLimit(i, j, d), d, i, j);
                    } else {
                        int spwLength = 0;

                        // we already have a left part
                        int row = i;
                        int col = j;

                        int rowChange = 0;
                        int colChange = 0;

                        if(d == HORIZONTAL) {
                            colChange = -1;
                        } else if(d == VERTICAL) {
                            rowChange = -1;
                        }

                        row += rowChange;
                        col += colChange;

                        while(row >= 0 && col >= 0 &&
                              row < BOARD_SIZE && col < BOARD_SIZE) {
                            startingPartialWord[spwLength] = getCell(row, col);
                            spwLength++;

                            row += rowChange;
                            col += colChange;
                        }

                        // reverse the part
                        int k;
                        for(k=0;k<spwLength;k++) {
                            letter temp = startingPartialWord[k];
                            startingPartialWord[k] = startingPartialWord
                                                        [spwLength-k-1];
                            startingPartialWord[spwLength-k-1] = temp;
                        }

                        // extend to the right
                        extendRight(startingPartialWord, dict, d,
                                    i, j, FALSE);
                    }
                }
            }
        }
    }

    if(bestScore == INITIAL) {
        D("DECIDED TO PASS\n");
        playMove(me, PASS, PASS, NULL, HORIZONTAL);
    } else {
        playMove(me, bestRow, bestCol, bestWord, bestDir);
    }
}

static void handlePossibleMove(int row, int col, wordRef wordToPlay,
                               direction dirToMove) {
    // adjust row, col: they are anchor co-ords

//    D("handling: (%d,%d) word = \"%s\" dir = %d\n",row,col,wordToPlay,dirToMove);
    if(isLegalMove(me, row, col, wordToPlay, dirToMove)) {
        int score = scoreMove(me, row, col, wordToPlay, dirToMove);
        if(score > bestScore) {
            bestRow = row;
            bestCol = col;
            bestScore = score;
            strncpy(bestWord, wordToPlay, BOARD_SIZE);
            bestDir = dirToMove;
        }
    }
}

static void leftPart(wordRef partialWord, Trie trieNode, int limit,
                     direction expandDir, int anchorRow, int anchorCol) {
    int length = strlen(partialWord);
    extendRight(partialWord, trieNode, expandDir, anchorRow, anchorCol, FALSE);
    if(limit > 0) {
        letter i;
        for(i=FIRST_LETTER;i<=LAST_LETTER;i++) {
            // for each letter, check if it's legal
            if(getChild(trieNode, i) != NULL) {
                if(isInRack(myRack, i)) {
                    deleteSingleLetterFromRack(myRack, i);

                    // recurse!
                    partialWord[length] = i;
                    leftPart(partialWord, getChild(trieNode, i), limit - 1,
                             expandDir, anchorRow, anchorCol);

                    addSingleLetterToRack(myRack, i);
                    
                    // reset the partialWord
                    partialWord[length] = INVALID_LETTER;
                }
            }
        }
    }
}

static void extendRight(wordRef partialWord, Trie trieNode,
                        direction expandDir, int row, int col,
                        bool coveredAnchor) {
//    D("extendRight: (%d,%d) \"%s\" covered %d\n", row, col, partialWord, coveredAnchor);
    if(row >= 0 && col >= 0 && row < BOARD_SIZE && col < BOARD_SIZE) {
        int length = strlen(partialWord);

        // new co-ordinates, should we need them
        int newRow = row;
        int newCol = col;
        if(expandDir == HORIZONTAL) {
            newCol++;
        } else if(expandDir == VERTICAL) {
            newRow++;
        }

        if(getCell(row, col) == INVALID_LETTER) {
//            D("extendRight: (%d,%d) is empty!\n",row,col);
            if(coveredAnchor != FALSE && isTerminal(trieNode)) {
                int rowChange = 0;
                int colChange = 0;
                if(expandDir == HORIZONTAL) {
                    colChange = -length;
                } else if(expandDir == VERTICAL) {
                    rowChange = -length;
                }

                handlePossibleMove(row + rowChange, col + colChange,
                                   partialWord, expandDir);
            }

            letter i;
            for(i=FIRST_LETTER;i<=LAST_LETTER;i++) {
                if(getChild(trieNode, i) != NULL) {
                    if(isInRack(myRack, i)) {
                        deleteSingleLetterFromRack(myRack, i);

                        // recurse!
                        partialWord[length] = i;

//                        D("going to try '%c' and goto (%d,%d)\n",i,newRow,newCol);
                        extendRight(partialWord, getChild(trieNode, i), 
                                    expandDir, newRow, newCol, TRUE);

                        addSingleLetterToRack(myRack, i);
                        
                        // reset the partialWord
                        partialWord[length] = INVALID_LETTER;
                    }
                }
            }
        } else {
            if(getChild(trieNode, getCell(row, col)) != NULL) {
                // recurse!
                partialWord[length] = getCell(row, col);

//                D("filled!\n");
                extendRight(partialWord, getChild(trieNode, getCell(row, col)), 
                            expandDir, newRow, newCol, TRUE);

                // reset the partialWord
                partialWord[length] = INVALID_LETTER;
            }
        }
    }
}

static void findAnchors(direction dir) {
    int i,j,k;
    int dx[] = {0,0,1,-1};
    int dy[] = {1,-1,0,0};

    for(i=0;i<BOARD_SIZE;i++) {
        for(j=0;j<BOARD_SIZE;j++) {
            anchors[i][j] = FALSE;
            
            if(dir == HORIZONTAL) {
                k = 2;
            } else if(dir == VERTICAL) {
                k = 0;
            }

            for(;k<4;k++) {
                if(i+dy[k] >= 0 && i+dy[k] < BOARD_SIZE &&
                   j+dx[k] >= 0 && j+dx[k] < BOARD_SIZE &&
                   getCell(i+dy[k], j+dx[k]) != INVALID_LETTER) {
                    anchors[i][j] |= TRUE;
                }
            }
        }
    }
}

static int findLimit(int row, int col, direction expandDir) {
    int rowChange = 0;
    int colChange = 0;

    if(expandDir == HORIZONTAL) {
        colChange = -1;
    } else if(expandDir == VERTICAL) {
        rowChange = -1;
    }

    int i = 0;
    row += rowChange;
    col += colChange;

    while(row >= 0 && col >= 0 && row < BOARD_SIZE && col < BOARD_SIZE) {
        if(anchors[row][col]) {
            break;
        }

        row += rowChange;
        col += colChange;
        i++;
    }
    return i;
}
