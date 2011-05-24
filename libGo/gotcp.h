#ifndef GO_TCP
#define GO_TCP
/*
 * This is part of the GO Framework
 *
 * Useful to use TCP easily
 */
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


struct TCP_t {
	#ifdef GO_LINUX
		int sock;
		struct sockaddr_in sin;
	#endif
	#ifdef GO_WINDOWS
		SOCKET sock;
		SOCKADDR_IN sin;
	#endif
};

typedef struct TCP_t TCP;

/*
 * Init the tcp4 API
 */
void tcp4_init();

/*
 * Create a socket
 */
int tcp4_create(TCP *t,char *hostname,int port);

/*
 * Bind a socket to a given port
 */
int tcp4_bind(TCP *t, int port);

/*
 * Listen 
 */
int tcp4_listen(TCP *t, int n);

/*
 * Accept a connection
 */
int tcp4_accept(TCP *t, TCP *tmp);

/*
 * Connect the socket
 */
int tcp4_connect(TCP *t);

/*
 * Send data trough socket
 */
int tcp4_send(TCP *t,char *data,int len);

/*
 * Recieve data trough socket
 */
int tcp4_recv(TCP *t, char *dest, int len);

/*
 * Close a socket
 */
void tcp4_close(TCP *t);

/*
 * Shutdown a socket (level 2)
 */
void tcp4_shutdown(TCP *t);

/*
 * Return the IP of a socket (useful to get a client IP)
 */
int tcp4_ip(TCP *t);

/*
 * Set blocking/nonblocking
 */
void tcp4_blocking(TCP *t, int);

#endif
