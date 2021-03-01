CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic

all: banhammer

banhammer: banhammer.o bf.o bv.o hash.o ll.o node.o speck.o parser.o
	$(CC) -o banhammer banhammer.o bf.o bv.o hash.o ll.o node.o speck.o parser.o

banhammer.o: banhammer.c
	$(CC) $(CFLAGS) -c banhammer.c

bf.o: bf.c
	$(CC) $(CFLAGS) -c bf.c

bv.o: bv.c
	$(CC) $(CFLAGS) -c bv.c

hash.o: hash.c
	$(CC) $(CFLAGS) -c hash.c

ll.o: ll.c
	$(CC) $(CFLAGS) -c ll.c
	
node.o: node.c
	$(CC) $(CFLAGS) -c node.c

speck.o: speck.c
	$(CC) $(CFLAGS) -c speck.c

parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c

clean:
	rm -f banhammer *.o

format:
	clang-format -i -style=file *.c *.h

