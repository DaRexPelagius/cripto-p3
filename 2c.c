#include "2c.h"

int main(int argc, char** argv) {

	int n, d, e;
	int k, suma;
	int p, q;

	//Tenemos que conocer la clave publica y el exponente de la clave privada
	//d

	//Comprobamos que el numero de argumentos es el adecuado
	if (argc != 7) {
		printf("Error en el numero de argumentos. Uso:\n");
		printf("./factorizacion -n p*q -e exp. cifrado -d exp. descifrado\n");
		return -1;
	}

	//Conseguimos los argumentos de entrada
	if (getArgs(argc, argv, &n, &d, &e) == -1) {
		printf("Error en los argumentos de entrada. Uso:\n");
		printf("./factorizacion -n p*q -e exp. cifrado -d exp. descifrado\n");
		return -1;
	}

	//Comprobamos que nos han pasada unos parametros adecuados
	if ((e < 1) || (n < 1) || (d < 1)) {
		printf("Todos los argumentos han de ser mayor que 0.\n");
		return -1;
	}

	//Ahora sabemos que tenemos ambas claves, tanto publica como privada
	// pub=(n,e) , priv=(n,d). Por lo que lei en Internet hay dos metodos
	//y el que voy a implementar no estoy seguro que sirva para todas las claves
	//(debe dar problemas si d > N/2.

	//ed=1+k(p−1)(q−1)=1+k(N−p−q+1).

	k =
			(int) (((e * d) / n) < 0 ?
					(((e * d) / n) - 0.5) : (((e * d) / n) + 0.5));

	// N−(ed−1)/k−1=p+q
	suma =
			(int) ((n - (e * d - 1) / k - 1) < 0 ?
					((n - (e * d - 1) / k - 1) - 0.5) : ((n - (e * d - 1) / k - 1) + 0.5));

	//p,q son raices de X2−(p+q)X+pq=0.
	p = (suma + sqrt((suma ^ 2) - n)) / 2;
	q = (suma - sqrt((suma ^ 2) - n)) / 2;

	printf("La factorizacion de n = %d es: \n", n);
	printf("%d = %d * %d", n, p, q);
}

int getArgs(int nArgs, char** args, int* n, int* e, int* d) {

	if (getEntero(nArgs, args, n, "-n", 2) != 1)
		return -1;
	if (getEntero(nArgs, args, e, "-e", 2) != 1)
		return -1;
	if (getEntero(nArgs, args, d, "-d", 2) != 1)
		return -1;

	return 0;
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
