C := gcc
CFLAGS += -O -Wall

test_extend:
	make test CFLAGS=-DEXTEND

test: testGame testTrie
	$(C) $(CFLAGS) -o runTests testAll.c testGame.o game.o testTrie.o trie.o

testGame: game
	$(C) $(CFLAGS) -c testGame.c

runGame: game trie
	$(C) $(CFLAGS) -o runGame runGame.c game.o trie.o

game: trie
	$(C) $(CFLAGS) -c game.c

testTrie: trie
	$(C) $(CFLAGS) -c testTrie.c

trie:
	$(C) $(CFLAGS) -c trie.c

clean:
	rm -rf *.o
