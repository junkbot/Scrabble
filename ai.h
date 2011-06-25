// ai.h
// 25 June 2011, Joshua Lau
// Interface for an AI that plays the game of scrabble

#ifndef __GAME
#include "game.h"
#endif

#ifndef __TRIE
#include "trie.h"
#endif

#define __AI

// makeMove is called when it's the AI's turn to move
// makeMove must call playMove to make a move
void makeMove(player you, rackRef yourRack, Trie dictionary);
