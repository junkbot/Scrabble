// File:            trie.h
// Date:            20/02/11
// Author:          Joshua Lau

// Description:
// Interface for the ADT that controls the trie data structure
// for quick and easy dictionary lookup to test legality for
// both human plays and AIs

#ifndef TRUE
typedef int bool;
#define TRUE 1
#define FALSE 0
#endif

#define NUM_OF_LETTERS 26

#define END_OF_STRING '\0'
#define FIRST_LETTER 'A'
#define LAST_LETTER 'Z'

#define FIRST_INDEX 0
#define ADD_FOR_NEXT_INDEX 1

typedef struct _trie trie;
typedef trie *Trie;

// creates a trie and returns a pointer to it
Trie createTrie(void);

// destroys a tree and its children FOREVER!
void destroyTrie(Trie trieToDestroy);

// returns the number of words in a Trie
int getSize(Trie trieToInspect);

// gets the child from the edge labelled letterToFollow
// PRECONDITION: FIRST_LETTER <= letterToFollow <= LAST_LETTER
Trie getChild(Trie trieToInspect, char letterToFollow);

// checks if the given Trie has the given word
bool hasWord(Trie trieToInspect, const char *wordToLookFor);

// adds a word to the given Trie
void addWord(Trie trieToModify, const char *wordToAdd);

