#include "2a.h"

int main(int argc, char** argv) {

	mpz_t base, exp, mod, potencia, aux;

	//Inicializamos las variables de GMP que vamos a usar
	mpz_init(base);
	mpz_init(exp);
	mpz_init(mod);
	mpz_init(potencia);
	mpz_init(aux);

	//Comprobamos que el numero de argumentos es el adecuado
	if (argc != 7) {
		printf("Numero de parametros de entrada incorrecto. Uso:\n");
		printf("./2a -b <base> -e >exponente> -m <modulo>\n");
		clear(base, exp, mod, potencia, aux);
		return -1;
	}

	//Conseguimos los argumentos de entrada
	if (getArgs(argc, argv, base, exp, mod) == -1) {
		printf("Argumentos de entrada incorrectos. Uso:\n");
		printf("./2a -b <base> -e >exponente> -m <modulo> \n");
		clear(base, exp, mod, potencia, aux);
	}

	//Comprobamos que los numeros con los que operar son correctos
	//Tanto la base como el exponente tienen que ser mayor o igual
	//que 0, y el modulo en el que operar tiene que ser mayor que 0
	if ((mpz_cmp_ui(base,0) < 0) || (mpz_cmp_ui(exp,0) < 0)) {
		printf("La base y el exponente han de ser mayor o igual que 0.\n");
		clear(base, exp, mod, potencia, aux);
		return -1;
	}
	if (mpz_cmp_ui(mod,0) <= 0) {
		printf("El modulo ha de ser mayor que 0.\n");
		clear(base, exp, mod, potencia, aux);
		return -1;
	}

	//Ahora vamos a realizar la potencia con nuestra implementacion
	//Imprimimos a^b mod m = x mod m.
	printf("El resultado aplicando la exponenciacion binaria es:\n");
	exponenciacionBinaria(potencia, base, exp, mod);
	gmp_printf("\t%Zd^%Zd mod %Zd = %Zd mod %Zd\n", base, exp, mod, potencia,
			mod);

	//Ahora usamos la libreria GMP
	printf("El resultado usando GMP:\n");
	mpz_powm(aux, base, exp, mod);
	gmp_printf("\t%Zd^%Zd mod %Zd = %Zd mod %Zd\n", base, exp, mod, aux, mod);

	clear(base, exp, mod, potencia, aux);

	return 0;
}

int getArgs(int nArgs, char** args, mpz_t base, mpz_t exp, mpz_t mod) {

	if (getEnteroMpz(nArgs, args, base, "-b") != 1)
		return -1;
	if (getEnteroMpz(nArgs, args, exp, "-e") != 1)
		return -1;
	if (getEnteroMpz(nArgs, args, mod, "-m") != 1)
		return -1;

	return 0;
}

int getEnteroMpz(int nArgs, char** args, mpz_t n, char* argumento) {

	int i;
	int flag = 0;

	for (i = 1; i <= (nArgs - 2); i++)
		if ((strncmp(args[i], argumento, 2) == 0) && (strlen(args[i]) == 2)) {
			if (flag)
				return -1;
			else {
				mpz_set_str(n, args[i + 1], 10);
				flag = 1;
			}
		}

	return flag;
}

void exponenciacionBinaria(mpz_t potencia, mpz_t base, mpz_t exp, mpz_t mod) {

	int i;
	int longitud = mpz_sizeinbase(exp, 2);//Reservamos memoria para los coeficientes
	mpz_t dos, aux;
	mpz_t* coeficientes;

	//Inicializamos el vector coeficientes
	coeficientes = (mpz_t*) malloc(longitud * sizeof(mpz_t));
	for (i = 0; i < longitud; i++) {
		mpz_init(coeficientes[i]);
	}
	//Primero calculamos el exponente en base dos
	mpz_init_set_ui(dos, 2);	//dos = 2
	mpz_init(aux);
	mpz_set(aux, exp);	//aux=exp
	for (i = 0; i < longitud; i++)
		mpz_tdiv_qr(aux, coeficientes[i], aux, dos);//aux = [aux/2], coeficientes[i] = resto
	mpz_clear(aux);
	mpz_clear(dos);

	mpz_set_ui(potencia, 1);

	//Exponenciacion Binaria
	//El vector coeficientes va de forma inversa, es decir:
	//13=0010 en vez de 13=1101
	for (i = longitud - 1; i >= 0; i--) {
		mpz_mul(potencia, potencia, potencia);
		mpz_mod(potencia, potencia, mod);
		if (mpz_cmp_ui(coeficientes[i],1) == 0) {
			mpz_mul(potencia, potencia, base);
			mpz_mod(potencia, potencia, mod);
		}
	}

	for (i = 0; i < longitud; i++) {
		mpz_clear(coeficientes[i]);
	}
}

void clear(mpz_t a, mpz_t b, mpz_t c, mpz_t d, mpz_t e) {
	mpz_clear(a);
	mpz_clear(b);
	mpz_clear(c);
	mpz_clear(d);
	mpz_clear(e);
}

