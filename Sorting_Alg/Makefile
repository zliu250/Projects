CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic

all: sorting

stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c

sorting.o: sorting.c
	$(CC) $(CFLAGS) -c sorting.c

quick.o: quick.c
	$(CC) $(CFLAGS) -c quick.c

bubble.o: bubble.c
	$(CC) $(CFLAGS) -c bubble.c

shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c

heap.o: heap.c
	$(CC) $(CFLAGS) -c heap.c

swap.o: swap.c
	$(CC) $(CFLAGS) -c swap.c

set.o: set.c
	$(CC) $(CFLAGS) -c set.c

sorting: sorting.o stack.o quick.o bubble.o shell.o heap.o swap.o set.o
	$(CC) -o sorting sorting.o stack.o quick.o bubble.o shell.o heap.o swap.o set.o

clean:
	rm -f sorting *.o

format:
	clang-format -i -style=file *.c *.h
	
