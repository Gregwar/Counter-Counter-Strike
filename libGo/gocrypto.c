#include <stdio.h>
#include <stdlib.h>
#include <gocrypto.h>

int chaos_r[32]={15,16,7,6,12,22,9,25,29,23,24,14,0,4,21,5,31,19,11,28,10,30,3,27,2,1,13,17,18,26,20,8};

long long int chaos(long long int k) {
        long long int a,b,c,d,i,f,t;

	a=0x8172861F94CFEAD5LL;
	b=0x72861F94CFEAD5D0LL;
	c=0xA013983842D15CD4LL;
	d=0x13983842D15CD476LL;

        for (i=0; i<32; i++) {
                f=b^((~c)|d);
                t=a;
                a=b;
                b=c;
                c=d;
                d=((t+f)^k) << chaos_r[i];
                d|=((t+f)^k) >> (32-chaos_r[i]);
        }

        return (a^b^c^d);
}


long long int xchaos(long long int key,char *X, int len) {
        int i;
	long long int tmp = 0x216694FE4DCD8A29LL;
        char *p=X;
        for (i=0; i<len; i++) {
                if (i%8 == 0) {
       			tmp = chaos(tmp ^ key);
		}
                *p ^= (tmp >> (8*(i%4)))&0xFF;
		
                p++;
        }
	tmp = chaos(tmp ^ key);
	return tmp;
}

long long int checksum(long long int key,char *X, int len) {
	int i;
	long long int cs = chaos(key);
	for (i=0; i<len/8; i++) {
		cs ^= *(long long *)(X + 8*i);
	}
	if (len%8 != 0) {
		long long int tmp = 0x0000000000000000LL;
		for (i=0; i<len%8; i++) {
			tmp <<= 8;
			tmp |= (int)((*(X+8*(len/8) + i))&0xFF);
		}
		cs ^= tmp;
	}
	return chaos(cs);
}
