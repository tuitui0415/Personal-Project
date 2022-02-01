CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
LFLAGS = -lm


all: encode decode
encode:	 encode.o io.o node.o pq.o huffman.o code.o stack.o
	$(CC) -o encode encode.o io.o node.o pq.o huffman.o stack.o code.o  $(LFLAGS)
decode:  decode.o io.o node.o pq.o huffman.o code.o stack.o
	$(CC) -o decode decode.o io.o node.o pq.o huffman.o stack.o code.o  $(LFLAGS)

code.o: code.c
	$(CC) $(CFLAGS) -c code.c
io.o: io.c
	$(CC) $(CFLAGS) -c io.c
stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c
node.o: node.c
	$(CC) $(CFLAGS) -c node.c
pq.o: pq.c
	$(CC) $(CFLAGS) -c pq.c
huffman.o: huffman.c
	$(CC) $(CFLAGS) -c huffman.c
encode.o: encode.c pq.h io.h node.h huffman.h code.h  header.h defines.h 
	$(CC) $(CFLAGS) -c encode.c  
decode.o: decode.c pq.h io.h node.h huffman.h code.h  header.h defines.h
	$(CC) $(CFLAGS) -c decode.c	
format:
	clang-format -i -style=file *.c *.h

clean:
	rm -f *.o
	rm encode decode


