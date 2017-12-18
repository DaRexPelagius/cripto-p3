#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gmp.h>
#include <math.h>
#include <sys/time.h>

#define MAX_NOMBRE 200

int getArgs(int nArgs, char** args, int* nBits, int* nTests,
		char* ficherosalida, int* salida);
int getCadena(int nArgs, char** args, char* cadena, char* modo, int longitud);
int getEntero(int nArgs, char** args, int* entero, char* modo, int longitud);
double nAleatorio(mpz_t number, int nBits, int nTests);
int testMillerRabin(mpz_t number, int nTests, double* prob);
