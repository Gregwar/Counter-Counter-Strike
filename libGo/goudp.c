#include <string.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <goudp.h>
#include <unistd.h>

#ifdef GO_LINUX
	#include <ifaddrs.h>
	#include <linux/if.h>
#endif

int udp4_listen(UDP *u,int port) {
	int broadcast=1;
	//Creating socket
	u->sock=socket(AF_INET, SOCK_DGRAM, 0);
	setsockopt(u->sock, SOL_SOCKET, SO_BROADCAST, (const char*)&broadcast, sizeof(broadcast));

	//Creating address
	u->sin.sin_addr.s_addr=htonl(INADDR_ANY);
	u->sin.sin_port = htons(port);
	u->sin.sin_family = AF_INET;

	//Binding
        return (bind(u->sock, (struct sockaddr*)&u->sin, sizeof(struct sockaddr_in)) == 0);
}

int udp4_create(UDP *u) {
	int broadcast=1;
	u->sock=socket(AF_INET, SOCK_DGRAM, 0);
	setsockopt(u->sock, SOL_SOCKET, SO_BROADCAST, (const char*)&broadcast, sizeof(broadcast));
	return (u->sock>0);
}

int udp4_broadcast(UDP *u,int port,const void *msg,int len,int addr) {
	int sock;
	struct sockaddr_in sin;
	struct timeval timer;
	
	//Creating socket
	sock=u->sock;
	
	//Creating address
	if (addr==0)
		sin.sin_addr.s_addr=INADDR_BROADCAST;
	else
		sin.sin_addr.s_addr=addr;
	sin.sin_port = htons(port);
	sin.sin_family = AF_INET;

	//Setting attributes
	timer.tv_sec=1;
	timer.tv_usec=0;
	//setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO,&timer, sizeof(timer));
	
	//Send packet
	return sendto(sock, msg, len, 0, (struct sockaddr*)&sin, sizeof(struct sockaddr));
}

int udp4_recv(UDP *u,void *data, int maxsize, int *ip, int *port) {
	struct sockaddr_in sin;
	int n;
	#ifdef GO_LINUX
		socklen_t len=sizeof(struct sockaddr_in);
	#endif
	#ifdef GO_WINDOWS
		int len=sizeof(struct sockaddr_in);
	#endif
	n=recvfrom(u->sock, data, maxsize,0,(struct sockaddr *)&sin,&len);
	if (n<0) {
		perror("recvfrom");
		exit(0);
	}
	if (ip!=NULL)
		*ip=sin.sin_addr.s_addr;
	if (port!=NULL)
		*port=htons(sin.sin_port);
	return n;
}

void udp4_close(UDP *u) {
	close(u->sock);
}

void udp4_findall(void) {
	#ifdef GO_LINUX
        struct ifaddrs *addrs, *addr;
        struct sockaddr *tmp;

	udp4_all.ip = NULL;
	udp4_all.len=0;

       	if (getifaddrs(&addrs)!=0) {
		perror("getifaddrs");
		exit(0);
	}
	for (addr = addrs; addr!=NULL; addr=addr->ifa_next) {
                tmp = addr->ifa_broadaddr;
		if (tmp == NULL)
			continue;
                if (tmp->sa_family==AF_INET && (addr->ifa_flags & IFF_UP) &&
                   (addr->ifa_flags&IFF_BROADCAST) && !(addr->ifa_flags&IFF_LOOPBACK)) {
			if (udp4_all.ip == NULL)
				udp4_all.ip = malloc(sizeof(int));
			else
				udp4_all.ip=realloc(udp4_all.ip, (udp4_all.len+1)*sizeof(int));
			udp4_all.ip[udp4_all.len] = ((struct sockaddr_in *)tmp)->sin_addr.s_addr;
			udp4_all.len++;	
                }
        }
	#endif
}

void udp4_broadcast_all(UDP *u,int port, const void *msg,int len) {
	int i;
	if (udp4_all.ip!=NULL) {
		for (i=0; i<udp4_all.len; i++) {
			udp4_broadcast(u,port,msg,len,udp4_all.ip[i]);
		}
	}
}
