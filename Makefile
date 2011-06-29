C := gcc
CPP := g++
CFLAGS += -O -Wall -g

test_extend:
	make test CFLAGS=-DEXTEND

test: testGame testTrie ai
	$(C) $(CFLAGS) -o runTests testAll.c testGame.o game.o testTrie.o trie.o

testGame: game
	$(C) $(CFLAGS) -c testGame.c

testTrie: trie
	$(C) $(CFLAGS) -c testTrie.c

playGame: runGame
	$(CPP) $(CFLAGS) -o scrabble playGame.cpp runGame.o ai.o game.o trie.o

runGame: game trie ai
	$(C) $(CFLAGS) -c runGame.c

ai: game trie
	$(C) $(CFLAGS) -c ai.c

game: trie
	$(C) $(CFLAGS) -c game.c

trie:
	$(C) $(CFLAGS) -c trie.c

clean:
	rm -rf *.o
