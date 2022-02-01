CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra $(shell pkg-config --cflags gmp)
LFLAGS = $(shell pkg-config --libs gmp)


all:	keygen encrypt decrypt
keygen:	keygen.o rsa.o randstate.o numtheory.o 
	$(CC) -o keygen keygen.o rsa.o randstate.o numtheory.o $(LFLAGS)
encrypt: encrypt.o rsa.o randstate.o numtheory.o 
	$(CC) -o encrypt encrypt.o rsa.o randstate.o numtheory.o $(LFLAGS)
decrypt: decrypt.o rsa.o randstate.o numtheory.o
	$(CC) -o decrypt decrypt.o rsa.o randstate.o numtheory.o $(LFLAGS)

randstate.o: randstate.c
	$(CC) $(CFLAGS) -c randstate.c
numtheory.o: numtheory.c
	$(CC) $(CFLAGS) -c numtheory.c
rsa.o: rsa.c
	$(CC) $(CFLAGS) -c rsa.c
keygen.o: keygen.c rsa.h randstate.h numtheory.h
	$(CC) $(CFLAGS) -c keygen.c  
encrypt.o: encrypt.c rsa.h randstate.h numtheory.h
	$(CC) $(CFLAGS) -c encrypt.c
decrypt.o: decrypt.c rsa.h randstate.h numtheory.h
	$(CC) $(CFLAGS) -c decrypt.c

format:
	clang-format -i -style=file *.c *.h

clean:
	rm -f *.o
	rm keygen encrypt decrypt

