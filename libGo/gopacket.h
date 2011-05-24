#ifndef GOPACKET
#define GOPACKET
#include <goudp.h>

#define DATA_MAX_SIZE 512
#define INVALIDATE_CACHE 12

struct PACKET {
	struct PACKET_HEADER {
		long long int key;
		long long int checksum;
	} header;

	struct PACKET_INFOS {
		int timestamp;		
		float loadavg;		
		char hostname[35];
	} infos;
		
	char data[DATA_MAX_SIZE];
};

typedef struct PACKET PACKET;
typedef struct PACKET_HEADER PACKET_HEADER;
typedef struct PACKET_INFOS PACKET_INFOS;
typedef char PACKET_DATA[DATA_MAX_SIZE];

long long int go_invalidate[INVALIDATE_CACHE];
int go_invalidate_current;

void go_create_packet(PACKET *p, void *data, int len);

void go_cypher_packet(PACKET *p, long long int secret_key, int len);

int go_uncypher_packet(PACKET *p, long long int secret_key, int len);

int go_send_data(UDP *u,long long int secret_key, void *data, int len, int addr, int port);

void go_data_benchmark(long long int secret_key, void *data, int len);

void go_send_data_all(UDP *u,long long int secret_key, void *data, int len, int port);

int go_recv_packet(UDP *u, long long  int secret_key, PACKET *p, int *ip, int *port);

	
#endif
