#ifndef GO_MD5
#define GO_MD5
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Valeurs des constantes sinus
unsigned int *k;

//Structure temporaire pour un hash
struct md5_hash {
	int h0;
	int h1;
	int h2;
	int h3;
};

typedef struct md5_hash MD5;

//Hash un MD5
void go_md5_hash(char *msg, int len, MD5 *hash);

//Convertir un hash MD5 en hexadecimal
void go_md5_hex(MD5 *hash, char *out);

//Libere eventuellement la memoire
void go_md5_free();

#endif
