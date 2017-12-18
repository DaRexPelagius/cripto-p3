# MAKEFILE

potencia: 2a.c 2a.c 2a.h
	gcc 2a.c -o potencia -g -Wall -O2 -lgmp -lm

cleanpotencia: 
	rm potencia	
	
primo: 2b.c 2b.c 2b.h 
	gcc 2b.c -o primo -g -Wall -O2 -lgmp -lm
	
cleanprimo: 
	rm primo
	
factorizacionRSA: 2c.c 2c.c 2c.h 
	gcc 2c.c -o factorizacionRSA -g -Wall -O2 -lgmp -lm
	
cleanfactorizacionRSA: 
	rm factorizacionRSA
	