Shell := /bin/sh
CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic `pkg-config --cflags gmp`

LIBS = `pkg-config --libs gmp`

all: keygen encrypt decrypt

keygen: keygen.o randstate.o ss.o numtheory.o
	$(CC) -o keygen keygen.o randstate.o ss.o numtheory.o $(LIBS)

keygen.o: keygen.c randstate.h numtheory.h ss.h
	$(CC) $(CFLAGS) -c keygen.c

randstate.o: randstate.c randstate.h numtheory.h ss.h
	$(CC) $(CFLAGS) -c randstate.c

ss.o: ss.c ss.h randstate.h numtheory.h
	$(CC) $(CFLAGS) -c ss.c

numtheory.o: numtheory.c numtheory.h ss.h randstate.h
	$(CC) $(CFLAGS) -c numtheory.c

encrypt: encrypt.o randstate.o ss.o numtheory.o
	$(CC) $(CFLAGS) -o encrypt encrypt.o randstate.o ss.o numtheory.o $(LIBS)

encrypt.o: encrypt.c randstate.h numtheory.h ss.h
	$(CC) $(CFLAGS) -c encrypt.c

decrypt: decrypt.o randstate.o ss.o numtheory.o
	$(CC) $(CFLAGS) -o decrypt decrypt.o randstate.o ss.o numtheory.o $(LIBS)

decrypt.o: decrypt.c randstate.h numtheory.h ss.h
	$(CC) $(CFLAGS) -c decrypt.c

clean:
	rm keygen encrypt decrypt -f *.o

format:
	clang-format -i -style=file *.[ch]
