CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic

all: encode decode

decode: decode.o io.o trie.o word.o
	$(CC) -o decode decode.o io.o trie.o word.o -lm

encode: encode.o io.o trie.o word.o 
	$(CC) -o encode encode.o io.o trie.o word.o -lm

word.o: word.c
	$(CC) $(CFLAGS) -c word.c

trie.o: trie.c
	$(CC) $(CFLAGS) -c trie.c

io.o: io.c
	$(CC) $(CFLAGS) -c io.c

clean:
	rm -f encode decode *.o

format:
	clang-format -i -style=file *.c *.h



