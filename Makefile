CC=gcc
CFLAGS=-g -Wall
DFLAGS=
PROGS=float_parser

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $(DFLAGS) $< -o $@ -lm

clean:
	rm -f $(PROGS) *.o ~*
