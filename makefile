CFLAGS=-O2 -Wall

all: iter prodcons

iter: iter.o
	$(CC) -o iter iter.o

prodcons: prodcons.o
	$(CC) -o prodcons prodcons.o

clean:
	rm -f iter iter.o
	rm -f prodcons prodcons.o
