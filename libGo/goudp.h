/*
 * This is part of the GO FrameWork
 * <gonet/udp.h>
 *
 * Those methods permits to use UDP Easily
 *
 */

#ifndef GONET_UDP
#define GONET_UDP
#include <goos.h>
#include <string.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#ifdef GO_LINUX
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <sys/ioctl.h>
#endif

#ifdef GO_WINDOWS
	#include <winsock2.h>
	//#pragma comment(lib, "ws2_32.lib")
#endif

/* UDP_t & UDP
 * Struct which contain the UDP's address and the
 * socket reference for it
 */
struct UDP_t {
	int sock;
	struct sockaddr_in sin;
};

typedef struct UDP_t UDP;

/* UDP4 Listening
 * Create a socket and try to bind it on the given port
 */
int udp4_listen(UDP *u,int port);

/* UDP4 Creating
 * Crate an UDP connection
 */
int udp4_create(UDP *u);

/* UDP4 Broadcasting
 * Create a socket and send a given broadcast on it 
 */
int udp4_broadcast(UDP *u,int port,const void *msg,int len,int addr);

/* UDP4 Recieving
 * Recieve data from a listening UDP
 */
int udp4_recv(UDP *u,void *data, int maxsize, int *ip, int *port);

/* UDP4 Closing
 * Close an UDP connection
 */
void udp4_close(UDP *u);

/*
 * IPLIST_t & IP
 * Used to store an ip list
 */
struct IPLIST_t {
        int *ip;
        int len;
};

typedef struct IPLIST_t IPLIST;

IPLIST udp4_all;

/*
 * FIND ALL
 * Look in the ifconfig to find broadcastable addresses
 */
void udp4_findall(void);

/*
 * BROADCAST ALL
 * Same as broadcast, but with all addresses found
 */
void udp4_broadcast_all(UDP *u,int port, const void *msg,int len);

#endif
