/*
 * This is part of the GO Framework
 *
 */

#ifndef GO_UTIL
#define GO_UTIL

/*
 * ALONE?
 * Return 0 if there is somebody else connected,
 * 1 if the user is alone.
 */

int go_alone();

/*
 * LOAD?
 * Return the load average
 */

double go_load();

/*
 * IP TO STR
 * Transform an IP (4Bytes) to a String (A.B.C.D)
 */
char *go_ip2str(int ip, char *str);
int go_str2ip(char *str);


/*
 * MY IPZ
 * Get all the current IPz
 */

void go_myip_start();
int go_myip();
int go_myip_next();
int go_nbprocs();


/*
 * Dump a block of datas
 */

void go_dump(void *c,int length);

/*
 * Stuff about files
 *
 */

int go_file_exists(const char *path);
void go_file_touch(const char *path);
int go_file_gettime(const char *path);

void go_srand();

#endif
