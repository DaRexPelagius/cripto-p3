# MAKEFILE

potencia: 2a.c 2a.c 2a.h $(LIBS)
	gcc 2a.c -o potencia -g -Wall -O2 -lgmp -lm
