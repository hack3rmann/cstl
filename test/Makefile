CC = gcc
CFLAGS = -O3 -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self \
	-Wswitch-default -Wfloat-equal -Wpointer-arith -Wtype-limits -Wempty-body \
	-Wno-logical-op -Wstrict-prototypes -Wold-style-declaration \
	-Wold-style-definition -Wmissing-parameter-type \
	-Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign \
	-Wno-unused-result -std=gnu99 -lm
CFILES = $(wildcard test.c ../*.c)

test:
	$(CC) $(CFLAGS) $(CFILES) -o test.exe
	./test

clean:
	rm test.exe