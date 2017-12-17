# MAKEFILE

2a: 2a.c 2a.c 2a.h $(LIBS)
	gcc 2a.c -o 2a -g -Wall -O2 -lgmp -lm
