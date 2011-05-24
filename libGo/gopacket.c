#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gocrypto.h>
#include <goutil.h>
#include <gopacket.h>
#include <goudp.h>
#include <time.h>
#include <unistd.h>

/*
 * Create a packet p using the given datas
 * 
 */
void go_create_packet(PACKET *p, void *data, int len) {
	if (len > sizeof(PACKET_DATA)) {
		perror("[libgo:go_create_packet()] Can not create a so big packet");
		exit(0);
	}
	//Creating the packet
	p->infos.timestamp=time(NULL);
	p->infos.loadavg=go_load();
	gethostname(p->infos.hostname, 35);
	memcpy(p->data, data, len);
}

/*
 * Cypher a packet
 *
 */
void go_cypher_packet(PACKET *p, long long int secret_key, int len) {
	//Cyphering
	p->header.key=rand();
	p->header.key <<= 32;
	p->header.key|=rand();
	p->header.checksum=checksum(p->header.key ^ secret_key, (char*)(p) + sizeof(PACKET_HEADER), len + sizeof(PACKET_INFOS));
	xchaos(p->header.key ^ secret_key, (char*)(p) + sizeof(PACKET_HEADER), len + sizeof(PACKET_INFOS));
}

/*
 * Uncypher a packet
 * return 1 if success, 0 if the checksum is bad!
 */
int go_uncypher_packet(PACKET *p, long long int secret_key, int len) {
	//Uncyphering
	xchaos(p->header.key ^ secret_key, (char*)(p) +sizeof(PACKET_HEADER), len + sizeof(PACKET_INFOS));
	return (p->header.checksum == checksum(p->header.key ^ secret_key, (char*)(p) +sizeof(PACKET_HEADER), len + sizeof(PACKET_INFOS)));
}

/*
 * Quick macro for Client
 *
 */
int go_send_data(UDP *u,long long int secret_key, void *data, int len, int addr, int port) {
	if (len > sizeof(PACKET_DATA)) {
		perror("[libgo:go_send_packet()] Tried to send too big packet");
		exit(0);
	}
	PACKET tmp;
	go_create_packet(&tmp,data,len);
	go_cypher_packet(&tmp,secret_key,len);
	return udp4_broadcast(u, port, &tmp, sizeof(PACKET_HEADER) + sizeof(PACKET_INFOS) + len, addr);
}

void go_send_data_all(UDP *u,long long int secret_key, void *data, int len, int port) {
        if (len > sizeof(PACKET_DATA)) {
                perror("[libgo:go_send_packet()] Tried to send too big packet");
                exit(0);
        }
        PACKET tmp;
        go_create_packet(&tmp,data,len);
        go_cypher_packet(&tmp,secret_key,len);
        udp4_broadcast_all(u,port, &tmp, sizeof(PACKET_HEADER) + sizeof(PACKET_INFOS) + len);
}

void go_data_benchmark(long long int secret_key, void *data, int len) {
	PACKET tmp;
	go_create_packet(&tmp,data,len);
	go_cypher_packet(&tmp,secret_key,len);
	int i;
	for (i=0; i<sizeof(PACKET_HEADER)+sizeof(PACKET_INFOS)+len; i++) {
		printf("%c", *(((char *)&tmp)+i));
	}
}

/*
 * Quick macro for server
 *
 */
int go_recv_packet(UDP *u, long long int secret_key, PACKET *p, int *ip, int  *port) {
	int n, len, i;
	
	if (go_invalidate_current<0 || go_invalidate_current>=INVALIDATE_CACHE)
		go_invalidate_current=0;
	
	while (1) {
packet_loop:
	        n=udp4_recv(u, p, sizeof(PACKET), ip, port);

	        len=n - sizeof(PACKET_HEADER) - sizeof(PACKET_INFOS);

	         if (len > 0) {
			for (i=0; i<INVALIDATE_CACHE; i++) {
				if (go_invalidate[i] == p->header.checksum)
					goto packet_loop;
			}
	                if (go_uncypher_packet(p, secret_key, len)) {
				go_invalidate[go_invalidate_current]=p->header.checksum;
				go_invalidate_current=(go_invalidate_current+1)%INVALIDATE_CACHE;
				return len;
	                } //Bad checksum
	        } //Too small packet
	}
}


