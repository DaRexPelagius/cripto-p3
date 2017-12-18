#include "2b.h"

int main(int argc, char** argv) {

	int nBits, nTests;
	int salida;
	char ficherosalida[MAX_NOMBRE];
	FILE* fsalida = NULL;
	mpz_t number;
	double prob;

	//Comprobamos que el numero de argumentos es el adecuado
	if ((argc != 5) && (argc != 7)) {
		printf("Error en el numero de argumentos. Uso:\n");
		printf("./primo -b bits -t sec [-o fileout]\n");
		return -1;
	}

	//Conseguimos los argumentos de entrada
	if (getArgs(argc, argv, &nBits, &nTests, ficherosalida, &salida) == -1) {
		printf("Error en los argumentos de entrada. Uso:\n");
		printf("./primo -b bits -t sec [-o fileout]\n");
		return -1;
	}

	//Comprobamos que nos han pasada unos parametros adecuados
	if ((nBits < 1) || (nTests < 1)) {
		printf(
				"Tanto el numero de bits como de tests ha de ser mayor que 0.\n");
		return -1;
	}

	//Abrimos el fichero de salida
	if (salida) {
		if ((fsalida = fopen(ficherosalida, "w")) == NULL) {
			fprintf(stderr, "Error al abrir el fichero: %s.\n", ficherosalida);
			return -1;
		}
	} else
		fsalida = stdout;

	//Generamos un numero aleatorio y le aplicamos Miller-Rabin
	//Miller-Rabin segun Internet, devuelve probablemente primo
	//o no primo.

	//Sin embargo veamos como funciona la funcion en la libreria GMP
	//Determine whether n is prime. Return 2 if n is definitely prime,
	//return 1 if n is probably prime (without being certain),
	//or return 0 if n is definitely non-prime.
	//This function performs some trial divisions, then reps Miller-Rabin
	//probabilistic primality tests. A higher reps value will reduce the
	//chances of a non-prime being identified as “probably prime”.
	//A composite number will be identified as a prime with a probability of
	//less than 4^(-reps). Reasonable values of reps are between 15 and 50.
	mpz_init(number);
	if ((prob = nAleatorio(number, nBits, nTests)) != 0.0)
		gmp_fprintf(fsalida,
				"Prime number:\t\t%Zd (probability of error around %.2e)\n",
				number, prob);
	else
		gmp_fprintf(fsalida, "Prime number:\t\t%Zd (definitely prime)\n",
				number);

	/* Tests its primality with GMP */
	switch (mpz_probab_prime_p(number, nTests)) {
	case 0:
		gmp_fprintf(fsalida,
				"GMP primality test:\t%Zd is definitely composite\n", number);
		break;
	case 1:
		gmp_fprintf(fsalida,
				"GMP primality test:\t%Zd is probably prime (without being certain)\n",
				number);
		break;
	case 2:
		gmp_fprintf(fsalida, "GMP primality test:\t%Zd is definitely prime\n",
				number);
		break;
	}

	mpz_clear(number);
	if (fsalida && (fsalida != stdout))
		fclose(fsalida);

	return 0;
}

int getArgs(int nArgs, char** args, int* nBits, int* nTests,
		char* ficherosalida, int* salida) {

	if (getEntero(nArgs, args, nBits, "-b", 2) != 1)
		return -1;
	if (getEntero(nArgs, args, nTests, "-t", 2) != 1)
		return -1;

	if (nArgs == 5)
		*salida = 0;
	else if (((*salida) = getCadena(nArgs, args, ficherosalida, "-o", 2)) != 1)
		return -1;

	return 0;
}

int getCadena(int nArgs, char** args, char* cadena, char* modo, int longitud) {

	int i;
	int flag = 0;

	for (i = 1; i <= (nArgs - 2); i++)
		if ((strncmp(args[i], modo, longitud) == 0)
				&& (strlen(args[i]) == longitud)) {
			if (flag)
				return -1;
			else {
				strcpy(cadena, args[i + 1]);
				flag = 1;
			}
		}
	return flag;
}
/*--------------------------------------------------------------------------
 Obtiene un entero
 - Entrada:
 * Número de argumentos
 * Array de argumentos
 * Puntero al entero
 * Puntero al modo
 * Longitud de la cadena
 - Salida:
 * -1 si ocurre algun error
 * 1 si va bien
 --------------------------------------------------------------------------*/
int getEntero(int nArgs, char** args, int* entero, char* modo, int longitud) {

	int i;
	int flag = 0;

	for (i = 1; i <= (nArgs - 2); i++) {

		if ((strncmp(args[i], modo, longitud) == 0)
				&& (strlen(args[i]) == longitud)) {
			if (flag)
				return -1;
			else {
				*entero = atoi(args[i + 1]);
				if ((*entero == 0) && !isdigit(args[i + 1][0]))
					return -1;
				flag = 1;
			}
		}
	}

	return flag;
}

double nAleatorio(mpz_t number, int nBits, int nTests) {

	double prob;
	gmp_randstate_t state;
	unsigned long seed;

	//Para generar con GMP numeros aleatorios en el rango de 0 a 2^n - 1
	//podemos utilizar gmp_urandomb aunque tenemos que inicializar la variable
	//state. Al estar en este rango de numeros sabemos que tiene nBits
	seed = time(NULL);
	gmp_randinit_default(state);
	gmp_randseed_ui(state, seed);

	//Generamos numeros aleatorios
	while (1) {

		//Como queremos que tenga exactaemnte nBits en representacion
		//binaria, hacemos la comprobacion del bucle while
		mpz_urandomb(number, state, nBits);
		mpz_setbit(number, nBits - 1);
		mpz_setbit(number, 0);

		//Cuando estamos en el nbits que queriamos, buscamos un numero
		//primo asi que rechazaremos aquellos que rechace el test
		while (mpz_sizeinbase(number, 2) == nBits) {
			if (testMillerRabin(number, nTests, &prob) != 0) {
				gmp_randclear(state);
				return prob;
			}
			mpz_add_ui(number, number, 2);//Si no es prob. primo, le sumamos 2
		}
	}
}

int testMillerRabin(mpz_t number, int nTests, double* prob) {

	int i, j;
	int k = 0;
	mpz_t aux, m, a, number_1;
	gmp_randstate_t state;
	unsigned long seed;

	/* MILLER-RABIN */

	/* Finds integers k>0 and m odd such that number-1 = (2^k)*m */
	mpz_init(m);
	mpz_sub_ui(m, number, 1);

	while (mpz_divisible_ui_p(m, 2) != 0) {
		k++;
		mpz_tdiv_q_ui(m, m, 2);
	}

	/* Initialises seed and state for random number generation */
	seed = time(NULL);
	gmp_randinit_default(state);
	gmp_randseed_ui(state, seed);
	mpz_init(a);
	mpz_init(number_1);
	mpz_sub_ui(number_1, number, 1);

	/* Different tests */
	for (i = 0; i < nTests; i++) {

		/* Generates random a such that 1 < a < number-1 */
		mpz_urandomm(a, state, number);
		while ((mpz_cmp_ui(a,1) <= 0) || (mpz_cmp(a, number_1) >= 0))
			mpz_urandomm(a, state, number);

		/* Heuristic checks */
		mpz_powm(a, a, m, number);

		if ((mpz_cmp_ui(a,1) != 0) && (mpz_cmp(a, number_1) != 0)) {

			/* Iterations */
			for (j = 1; j <= k - 2; j++) {
				mpz_powm_ui(a, a, 2, number);
				if ((mpz_cmp_ui(a,1) == 0)) {
					gmp_randclear(state);
					mpz_clear(m);
					mpz_clear(a);
					mpz_clear(number_1);
					*prob = 1.0;
					return 0;
				} else if (mpz_cmp(a, number_1) == 0)
					break;
			}

			/* Last iteration */
			mpz_powm_ui(a, a, 2, number);
			if ((mpz_cmp_ui(a,1) != 0) && (mpz_cmp(a, number_1) != 0)) {
				gmp_randclear(state);
				mpz_clear(m);
				mpz_clear(a);
				mpz_clear(number_1);
				*prob = 1.0;
				return 0;
			}
		}
	}

	gmp_randclear(state);
	mpz_clear(m);
	mpz_clear(a);
	mpz_clear(number_1);

	/* Accuracy of the test */
	*prob = 1. / (1 + pow(4, nTests) / (mpz_sizeinbase(number, 2) * log(2)));

	return 1;
}

