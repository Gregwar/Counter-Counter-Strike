#include <goos.h>
#include <stdlib.h>
#include <stdio.h>
#include <goutil.h>
#include <unistd.h>
#include <utime.h>
#include <fcntl.h>
#include <time.h>

#ifdef GO_LINUX
	#include <sys/sysinfo.h>
	#include <sys/stat.h>
	#include <ifaddrs.h>
	#include <linux/if.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <utmp.h>
	#include <pwd.h>
	#include <netdb.h>
#endif

#ifdef GO_WINDOWS
	#include <windows.h>
	#include <winsock2.h>
#endif

//ALONE?
int go_alone() {
	#ifdef GO_LINUX
		struct utmp* tmp;
		struct passwd* usr;
		int me=getuid();
		setutent();
		while ((tmp=getutent())) {
			if ((tmp->ut_type & USER_PROCESS) == USER_PROCESS) {
				usr=getpwnam(tmp->ut_user);
				if (usr->pw_uid != me)
					return 0;
			}
		}
		return 1;
	#endif
	return 0;
}

//LOAD?
double go_load() {
	#ifdef GO_LINUX
		double tmp;
		getloadavg(&tmp,1);
		return tmp;
	#endif
	return 0.0;
}

//CUREIP
char * go_ip2str(int ip,char *str) {
	sprintf(str, "%d.%d.%d.%d",(ip)&0xFF,(ip>>8)&0xFF,(ip>>16)&0xFF,(ip>>24)&0xFF);
	return str;
}

int go_str2ip(char *blha) {
	return inet_addr(blha);
}

//MY IPz
#ifdef GO_LINUX
	struct ifaddrs *go_myip_current;
#endif

int go_myip() {
	#ifdef GO_LINUX
		struct sockaddr *tmp;
	
		if (go_myip_current == NULL)
			return 0x0;
	
			
		tmp = go_myip_current->ifa_addr;
		
		if (tmp == NULL)
			return 0x0;
		else
			return ((struct sockaddr_in *)tmp)->sin_addr.s_addr;
	#endif
	return 0;
}

void go_myip_start() {
	#ifdef GO_LINUX
		struct sockaddr *tmp;
		go_myip_current = NULL;
		getifaddrs(&go_myip_current);
			while (go_myip_current != NULL) {
					tmp = go_myip_current->ifa_broadaddr;
					if (tmp == NULL)
							continue;
					if ((tmp->sa_family==AF_INET) && (go_myip_current->ifa_flags&IFF_UP) &&
						(go_myip_current->ifa_flags & IFF_BROADCAST) && !(go_myip_current->ifa_flags&IFF_LOOPBACK)) {
							break;
					}
					go_myip_current = go_myip_current->ifa_next;
			}
	#endif
}

int go_myip_next() {
	#ifdef GO_LINUX
		struct sockaddr *tmp;
		while (go_myip_current != NULL) {
			go_myip_current = go_myip_current->ifa_next;
			if (go_myip_current==NULL)
				return 0;
			tmp = go_myip_current->ifa_broadaddr;
			if (tmp == NULL)
				continue;
					if ((tmp->sa_family==AF_INET) && (go_myip_current->ifa_flags&IFF_UP) &&
				(go_myip_current->ifa_flags & IFF_BROADCAST) && !(go_myip_current->ifa_flags&IFF_LOOPBACK)) {
				return 1;
			}
		}
	#endif
	return 0;
}

//PROCS
int go_nbprocs() {
	#ifdef GO_LINUX
		return get_nprocs_conf();
	#endif
	return 1;
}

void go_dump(void *c,int length) {
        int i, n;
        void *p = c;

        printf("===== DUMP =====\n");
        for (i=0; i<length; i+=16) {
                printf("%04X: ",i);
                for (n=0; n<16; n++) {
                        if (i+n < length) {
                                printf("%02X ",*((unsigned char *)p));
                                p++;
                        }
                }
                printf("\n");

        }
        printf("================\n");

}



int go_file_exists(const char *path) {
	#ifdef GO_LINUX
		struct stat buf;
		int i=stat(path, &buf);
		return (i>=0);
	#endif
	#ifdef GO_WINDOWS
		int fp=open(path, O_RDONLY);
		if (fp>0) {
			close(fp);
			return 1;
		} else return 0;
	#endif
	return 0;
}

void go_file_touch(const char *path) {
	if (!go_file_exists(path)) {
		int f=creat(path, 0700);
		if (f>=0)
			close(f);
	} 
	struct utimbuf tb;
	tb.actime = tb.modtime = time(NULL);
	utime(path, &tb);
}

int go_file_gettime(const char *path) {
	#ifdef GO_LINUX
		struct stat buf;
		int i=stat(path, &buf);
		if (i>=0) {
			return buf.st_mtime;
		} else return 0;
	#endif
	return 0;
}

void go_srand() {
		#ifdef GO_LINUX
			struct timespec t;
			clock_gettime(CLOCK_REALTIME, &t);
			srand(t.tv_sec ^ t.tv_nsec);
		#endif
		#ifdef GO_WINDOWS
		//TODO: Find something better
			srand(time(NULL));
		#endif
}
