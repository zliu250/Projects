CC = cc 

all: prog0

prog0: test.o
	$(CC) -o prog0 test.o

test.o: test.c
	$(CC) -c test.c 

clean:
	rm -f *.o prog0
