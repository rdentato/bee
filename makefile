CFLAGS=-O2 -Wall

all: iter prodcons

iter: iter.o
	$(CC) -o iter iter.o

prodcons: prodcons.o
	$(CC) -o prodcons prodcons.o

fib: fib.o
	$(CC) -o fib fib.o

clean:
	rm -f iter iter.o
	rm -f prodcons prodcons.o
