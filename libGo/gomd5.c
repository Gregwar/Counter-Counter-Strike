#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gomd5.h>

int md5_r[64]={7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
           5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
           4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
           6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21};

//MD5 utilise des sinus d'entiers pour ses constantes:
void init_k() {
	int i;
	if (k==NULL) {
		k = malloc(64*sizeof(unsigned int*));
	}
	for (i=0; i<64; i++)
		k[i] = (unsigned int)floor(fabs(sin(i+1))*4294967296.);
	
}

void go_md5_hash(char *msg, int glen, MD5 *hash) {
	if (k==NULL)
		init_k();

	unsigned char *data;
	int len, t, n;
	hash->h0 = 0x67452301;
	hash->h1 = 0xEFCDAB89;
	hash->h2 = 0x98BADCFE;
	hash->h3 = 0x10325476;

	len = (glen + 9);
	t=len/64 + 1;

	if (len%64==0) 
		t--;

	//Creation d'une zone memoire de t mots de 512 bits
	data=malloc(sizeof(unsigned char)*t*64);
	
	len-=9;
	n=len-1;
	memcpy(data, msg, len);
	data[++n]=0x80;
	while(n%64 < 56) {
		data[n+1]=0x00;
		++n;
	}
	
	len*=8;
	*(long *)(&data[n++]) = len;
	
	unsigned int bloc;
	unsigned int a, b, c, d, i, f, g, tmp, rl;
	unsigned long *w;
	for (bloc=0; bloc<t; bloc++) {
		w = (unsigned long *)(data + bloc*64);
		a=hash->h0;
		b=hash->h1;
		c=hash->h2;
		d=hash->h3;

		for (i=0; i<64; i++) {
			if (i<16) {
				f = (b&c)|((~b)&d);
				g = i;
				goto suite;
			} 
			if (i<32) {
				f = (d&b)|((~d)&c);
				g = (5*i + 1)%16;
				goto suite;
			} 
			if (i<48) {
				f = b^c^d;
				g = (3*i + 5)%16;
				goto suite;
			}
				f = c^(b|(~d));
				g = (7*i)%16;

			suite:

			tmp=d;
			d=c;
			c=b;
			rl  = ((unsigned long)(a+f+k[i]+w[g]) << (md5_r[i]));
			rl |= ((unsigned long)(a+f+k[i]+w[g]) >> (32-md5_r[i]));
			b=rl+b;
			a=tmp;
		}
		hash->h0 += a;
		hash->h1 += b;
		hash->h2 += c;
		hash->h3 += d;
	}
	
}

void go_md5_hex(MD5 *hash, char *out) {
	int z;
	for (z=0; z<16; z++) {
		sprintf(out + 2*z, "%02x", *(((unsigned char *)hash)+z));
	}
}

void md5_free() {
	if (k!=NULL)
		free(k);
}
