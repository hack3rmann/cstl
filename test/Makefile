CC = gcc
CFLAGS = -Wall -Wextra
CFILES = $(wildcard test.c ../*.c)

test:
	$(CC) $(CFLAGS) $(CFILES) -o test.exe
	./test

clean:
	rm test.exe