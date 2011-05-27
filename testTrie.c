// File:            testTrie.cpp
// Date:            02/03/11
// Author:          Joshua Lau

// Description:
// Implementation for testing the Trie ADT
// Has heaps and HEAPS of tests...
// Trust me... there are heaps :D

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "testTrie.h"
#include "trie.h"

#define T(x...) fprintf(stderr,x)

#ifdef EXTEND
#define E(x...) fprintf(stderr,x)
#else
#define E(x...)
#endif

void testTrie(void) {
    T("\nTesting Trie...\n");

    testAddHasWord();
    testGetSize();
    testGetChild();

    T("\nTrie... all tests PASSED!\n");
}

void testAddHasWord(void) {
    T("\n* Testing addWord & hasWord...\n");

    {
        // general test
        E("** addWord & hasWord... TEST CASE %d\n", 1);

        E("*** creating a new Trie... ");
        Trie dictionaryTrie = createTrie();
        T("OK\n");

        int numWordsToAdd = 3;
        char wordsToAdd[][MAX_WORD_LENGTH] = {"A","AN","ANT"};

        int i;
        for(i=0;i<numWordsToAdd;i++) {
            E("*** adding '%s'... ",wordsToAdd[i]);
            addWord(dictionaryTrie,wordsToAdd[i]);
            T("OK\n");
        }

        for(i=0;i<numWordsToAdd;i++) {
            E("*** checking for '%s'... EXPECTED %d... GOT %d\n",wordsToAdd[i],TRUE, hasWord(dictionaryTrie,wordsToAdd[i]));
            assert( hasWord(dictionaryTrie,wordsToAdd[i]) );
        }

        int numNonDictionaryWordsToTest = 3;
        char nonDictionaryWordsToTest[][MAX_WORD_LENGTH] = {"AND","B","ANE"};

        for(i=0;i<numNonDictionaryWordsToTest;i++) {
            E("*** checking for '%s'... EXPECTED %d... GOT %d\n",nonDictionaryWordsToTest[i],FALSE, hasWord(dictionaryTrie,nonDictionaryWordsToTest[i]));
            assert( !hasWord(dictionaryTrie,nonDictionaryWordsToTest[i]) );
        }

        E("*** destroying the Trie... ");
        destroyTrie(dictionaryTrie);
        T("OK\n");

        E("** addWord & hasWord... TEST CASE %d... PASSED!\n", 1);
    }

    {
        E("** addWord & hasWord... TEST CASE %d\n", 2);

        E("*** creating new Trie... ");
        Trie dictionaryTrie = createTrie();
        T("OK\n");

        int numWordsToAdd = 8;
        char wordsToAdd[][MAX_WORD_LENGTH] = {
            "A","ARMY","ARMISTICE","ADD","ARE","ARID","ART","AS"
        };

        int i;
        for(i=0;i<numWordsToAdd;i++) {
            E("*** adding '%s'... ",wordsToAdd[i]);
            addWord(dictionaryTrie,wordsToAdd[i]);
            T("OK\n");
        }

        for(i=0;i<numWordsToAdd;i++) {
            E("*** checking for '%s'... EXPECTED %d... GOT %d\n",wordsToAdd[i],TRUE, hasWord(dictionaryTrie,wordsToAdd[i]));
            assert( hasWord(dictionaryTrie,wordsToAdd[i]) );
        }

        int numNonDictionaryWordsToTest = 8;
        // tests words with heaps of confusing common rooots/prefixes
        char nonDictionaryWordsToTest[][MAX_WORD_LENGTH] = {
            "AR","ARM","AD","AIRD","ARMS","ASK","ARSE","ARTS"
        };

        for(i=0;i<numNonDictionaryWordsToTest;i++) {
            E("*** checking for '%s'... EXPECTED %d... GOT %d\n",nonDictionaryWordsToTest[i],FALSE, hasWord(dictionaryTrie,nonDictionaryWordsToTest[i]));
            assert( !hasWord(dictionaryTrie,nonDictionaryWordsToTest[i]) );
        }

        E("*** destroying the Trie... ");
        destroyTrie(dictionaryTrie);  
        T("OK\n");

        E("** addWord & hasWord... TEST CASE %d... PASSED!\n", 2);
    }

    T("\n* addWord & hasWord... all tests PASSED!\n");
}

void testGetSize(void) {
    T("\n* Testing getSize...\n");

    {
        E("** getSize... TEST CASE %d\n", 1);

        E("*** creating the Trie... ");
        Trie dictionaryTrie = createTrie();
        T("OK\n");

        int numWordsToAdd = 7;
        char wordsToAdd[][MAX_WORD_LENGTH] = {
            "DELTA","DEEM","DEER","DOOR","DREAM","ALTER","RAT"
        };

        int i;
        for(i=0;i<3;i++) {
            E("*** adding '%s'... ",wordsToAdd[i]);
            addWord(dictionaryTrie,wordsToAdd[i]);
            T("OK\n");
        }

        E("*** checking size... EXPECTED %d... GOT %d\n",3,getSize(dictionaryTrie));
        assert( getSize(dictionaryTrie) == 3 );

        for(i=3;i<numWordsToAdd;i++) {
            E("*** adding '%s'... ",wordsToAdd[i]);
            addWord(dictionaryTrie,wordsToAdd[i]);
            T("OK\n");
        }

        E("*** checking size... EXPECTED %d... GOT %d\n",7,getSize(dictionaryTrie));
        assert( getSize(dictionaryTrie) == 7 );

        E("*** destroying the Trie... ");
        destroyTrie(dictionaryTrie);
        T("OK\n");

        E("** getSize... TEST CASE %d... PASSED!\n", 1);
    }

    T("* getSize... all tests PASSED!\n");
}

void testGetChild(void) {
    T("\n* Testing getChild...\n");

    {
        Trie dictionaryTrie = createTrie();

        E("** getChild... TEST CASE %d\n", 1);

        int numWordsToAdd = 12;
        char wordsToAdd[][MAX_WORD_LENGTH] = {
            "TRIE","DESCRIPTION","INTERFACE","STRUCTURE","DICTIONARY","HUMAN",
            "LETTERS","STRING","FIRST","LETTER","INDEX","ADD"
        };

        int i,j;

        for(i=0;i<numWordsToAdd;i++) {
            E("*** adding '%s'... ",wordsToAdd[i]);
            addWord(dictionaryTrie,wordsToAdd[i]);
            T("OK\n");
        }

        for(i=0;i<numWordsToAdd;i++) {
            Trie tempTrie = dictionaryTrie;

            for(j=0;j<strlen(wordsToAdd[i]);j++) {
                E("*** checking for '%s'... EXPECTED %d... GOT %d\n",wordsToAdd[i]+j,TRUE, hasWord(tempTrie,wordsToAdd[i]+j));
                assert( hasWord(tempTrie, wordsToAdd[i] + j) );

                E("*** going to child -> '%c'... ",wordsToAdd[i][j]);
                tempTrie = getChild(tempTrie, wordsToAdd[i][j]);
                T("OK\n");
            }
        }

        E("** getChild... TEST CASE %d... PASSED!\n", 1);
    }
    
    T("* getChild... all tests PASSED!\n");
}
