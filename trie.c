// File:            trie.cpp
// Date:            20/02/11
// Author:          Joshua Lau

// Description:
// Implements trie.h

#include "trie.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// a macro which shortens my debugging output statements
// they disappear at compile-time if i comment out the following line

//#define DEBUG
#ifdef DEBUG
#define D(x...) fprintf(stderr,"DEBUG: ***** "x)
#else
#define D(x...)
#endif

// i believe this breaks abstraction :(
// this really should be in the .cpp file (implementation)
struct _trie {
	int size;

	// marker to show that there is a word terminating here
	bool wordEndsHere;

	// each trie's children are tries in themselves ('subtries')

	// ok... this really should be Trie ... but C++ doesn't like me
	// with forward declarations so i had to break abstraction this way
	trie *children[NUM_OF_LETTERS];
};

// maps a letter to a number, ie 'A' to 0, 'B' to 1... 'Z' to 25
static int letterToNumber(char letterToMap);

// gets one child down in the hierachy based on the given letter
// PRECONDITION: FIRST_LETTER <= letterToAdd <= LAST_LETTER
static Trie moveNextLevel(Trie trieToModify, char letterToAdd);

// returns a pointer to the 2nd letter in the word
// PRECONDITION: wordToAdvanceFrom != NULL
static const char *nextLetterInWord(const char *wordToAdvanceFrom);

Trie createTrie(void) {
    Trie newTrie = malloc(sizeof(trie));
    
    newTrie->size = 0;
    newTrie->wordEndsHere = FALSE;

    int i;
    for(i=0;i<NUM_OF_LETTERS;i++) {
        newTrie->children[i] = NULL;
    }

    D("newTrie->wordEndsHere = %d\n",newTrie->wordEndsHere);

    return newTrie;
}

// recursively destroy the tree and all its children
void destroyTrie(Trie trieToDestroy) {
    if(trieToDestroy != NULL) {
        // current trie isn't NULL; recurse further
        int i;
        for(i=0;i<NUM_OF_LETTERS;i++) {
            destroyTrie(trieToDestroy->children[i]);
        }

        free(trieToDestroy);
    }
}

int getSize(Trie trieToInspect) {
    assert(trieToInspect != NULL);

    return trieToInspect->size;
}

Trie getChild(Trie trieToInspect, char letterToFollow) {
    int childIndexToFollow = letterToNumber(letterToFollow);
    return trieToInspect->children[childIndexToFollow];
}

bool hasWord(Trie trieToInspect, const char *wordToLookFor) {
	D("looking for \"%s\"\n", wordToLookFor);

    bool foundWord;
    if(trieToInspect == NULL) {
        // word doesn't exist
        foundWord = FALSE;
    } else if(wordToLookFor[FIRST_INDEX] == END_OF_STRING) {
		D("string ended.\n");
        if(trieToInspect->wordEndsHere) {
            // word exists
            foundWord = TRUE;
        } else {
            // reached the end of the string; still no success
            // therefore the word doesn't exist
            foundWord = FALSE;
        }
    } else {
        // go down to the next level
        foundWord = hasWord( getChild(trieToInspect, wordToLookFor[FIRST_INDEX]),
                             nextLetterInWord(wordToLookFor) );
    }

    return foundWord;
}

void addWord(Trie trieToModify, const char *wordToAdd) {
	D("adding word \"%s\"\n", wordToAdd);
    assert(trieToModify != NULL);

    if(wordToAdd[FIRST_INDEX] == END_OF_STRING) {
		D("string ended.\n");
        // end of the word
        trieToModify->wordEndsHere = TRUE;
    } else {
        addWord( moveNextLevel(trieToModify, wordToAdd[FIRST_INDEX]),
				 nextLetterInWord(wordToAdd) );
    }

    // increase number of words in the Trie
    trieToModify->size++;
}

bool isTerminal(Trie trieToCheck) {
    assert(trieToCheck != NULL);

    return trieToCheck->wordEndsHere;
}

Trie trieFromFile(FILE *inputFile) {
    Trie fileTrie = createTrie();

    char word[MAX_WORD_LENGTH];
    while(fscanf(inputFile, " %s ",word) != EOF) {
        int wordLength = strlen(word);
        int i;
        for(i=0;i<wordLength;i++) {
            word[i] = toupper(word[i]);
            assert(FIRST_LETTER <= word[i]);
            assert(word[i] <= LAST_LETTER);
        }
        addWord(fileTrie, word);
    }
    return fileTrie;
}

static int letterToNumber(char letterToMap) {
    // separate asserts for better error detection
    assert(FIRST_LETTER <= letterToMap);
    assert(letterToMap <= LAST_LETTER);

    int mappedNumber = (int)(letterToMap - FIRST_LETTER);
    return mappedNumber;
}

static Trie moveNextLevel(Trie trieToModify, char letterToAdd) {
    int childIndexToAdd = letterToNumber(letterToAdd);

    if(trieToModify->children[childIndexToAdd] == NULL) {
        // child currently doesn't exist; create it
        trieToModify->children[childIndexToAdd] = createTrie();
    }

    return trieToModify->children[childIndexToAdd];
}

static const char *nextLetterInWord(const char *wordToAdvanceFrom) {
    assert(wordToAdvanceFrom != NULL);
    assert(wordToAdvanceFrom[FIRST_INDEX] != END_OF_STRING);

    // pointer arithmetic :S
    return wordToAdvanceFrom + ADD_FOR_NEXT_INDEX;
}
