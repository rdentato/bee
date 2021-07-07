CFLAGS=-O2 -Wall

iter: iter.o
	$(CC) -o iter iter.o

clean:
	rm -f iter iter.o
