#include <gotcp.h>
#include <string.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void tcp4_init() {
	#ifdef GO_WINDOWS
		WSADATA WSAData;
		WSAStartup(MAKEWORD(2,0), &WSAData);
	#endif
}

int tcp4_create(TCP *t,char *hostname,int port) {
	struct timeval timer;
	struct hostent *hostinfo = NULL;

	if (t == NULL)
		return 0;

    hostinfo = gethostbyname(hostname);

    if (hostinfo == NULL)
    {
		return 0;
    }

    t->sin.sin_addr = *(struct in_addr *) hostinfo->h_addr;
    t->sin.sin_port = htons(port);
    t->sin.sin_family = AF_INET;

    t->sock=socket(AF_INET, SOCK_STREAM, 0);

	if (t->sock < 0) return 0;

    timer.tv_sec=5;
    timer.tv_usec=0;
    setsockopt(t->sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timer, sizeof(timer));
    setsockopt(t->sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timer, sizeof(timer));	
	return (t->sock>0);
}

int tcp4_listen(TCP *t, int n) {
	return listen(t->sock, n);
}

int tcp4_bind(TCP *t, int port) {
	struct timeval timer;
	
    t->sin.sin_addr.s_addr = htonl(INADDR_ANY);
    t->sin.sin_port = htons(port); 
    t->sin.sin_family = AF_INET;

	t->sock=socket(AF_INET, SOCK_STREAM, 0);

    timer.tv_sec=5;
    timer.tv_usec=0;
    setsockopt(t->sock, SOL_SOCKET, SO_SNDTIMEO,(const char*)&timer, sizeof(timer));
	int opt = 1;
	setsockopt(t->sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof opt);

	if (bind(t->sock, (struct sockaddr *)&t->sin,sizeof(struct sockaddr_in)) == -1) {
		perror("bind");
	}
	return 1;
}

int tcp4_accept(TCP *t, TCP *tmp) {
	unsigned int len=sizeof(struct sockaddr);
	tmp->sock=accept(t->sock,(struct sockaddr*)&tmp->sin, &len);
	return (tmp->sock>0);
}

int tcp4_connect(TCP *t) {
	return (connect(t->sock,(struct sockaddr *)&t->sin, sizeof(struct sockaddr)) != -1);
}

int tcp4_send(TCP *t,char *data,int len) {
	return send(t->sock, data, len, 0);
}

int tcp4_recv(TCP *t, char *dest, int len) {
	return recv(t->sock, dest, len, 0);
}

void tcp4_close(TCP *t) {
	close(t->sock);
}

void tcp4_shutdown(TCP *t) {
	shutdown(t->sock, 2);
}

int tcp4_ip(TCP *t) {
	return (t->sin.sin_addr.s_addr);
}

void tcp4_blocking(TCP *t,int b) {
	int m = fcntl(t->sock, F_GETFL, 0);
	if (!b)
		m |= O_NONBLOCK;
	else
		m &= ~O_NONBLOCK;
	fcntl(t->sock, F_SETFL, m);
}
