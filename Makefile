CC = gcc
CFLAGS = -Wall -Wextra -g

.PHONY: build clean

build: 
	 $(CC) $(CFLAGS) tema2.c trie.c max-heap.c -o search_index
run:
	./search_index
clean:
	rm search_index
