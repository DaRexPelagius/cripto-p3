#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gmp.h>
#include <math.h>
#include <sys/time.h>

int getArgs(int nArgs, char** args, mpz_t base, mpz_t exp, mpz_t mod);
int getEnteroMpz(int nArgs, char** args, mpz_t n, char* argumento);
void exponenciacionBinaria(mpz_t result, mpz_t base, mpz_t exp, mpz_t mod);
void clear(mpz_t a, mpz_t b, mpz_t c, mpz_t d, mpz_t e);
