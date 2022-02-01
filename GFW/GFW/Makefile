CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
LFLAGS = -lm


all: banhammer
banhammer: banhammer.o speck.o ht.o bst.o node.o bf.o bv.o parser.o
	$(CC) -o banhammer banhammer.o speck.o ht.o bst.o node.o bf.o bv.o parser.o $(LFLAGS)

speck.o: speck.c
	$(CC) $(CFLAGS) -c speck.c
ht.o: ht.c
	$(CC) $(CFLAGS) -c ht.c
bst.o: bst.c
	$(CC) $(CFLAGS) -c bst.c
node.o: node.c
	$(CC) $(CFLAGS) -c node.c
bf.o: bf.c
	$(CC) $(CFLAGS) -c bf.c
bv.o: bv.c
	$(CC) $(CFLAGS) -c bv.c
parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c
banhammer.o: banhammer.c speck.h ht.h bst.h node.h bf.h bv.h parser.h salts.h messages.h
	$(CC) $(CFLAGS) -c banhammer.c

format:
	clang-format -i -style=file *.c *.h

clean:
	rm -f *.o
	rm banhammer


