#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <golog.h>
#include <unistd.h>
#include <stdarg.h>

static char *go_days[] = {"Dim", "Lun", "Mar", "Mer", "Jeu", "Ven", "Sam"};

static char *go_months[] = {"Jan","Fev","Mar","Avr","Mai","Jun","Jul","Aou","Sep","Oct","Nov","Dec"};

FILE *go_logfile;

void go_logappend(const char *blha, ...) {
	va_list args;
	if (go_logfile!=NULL) {
		struct tm* ti;
		time_t i=time(NULL);
		ti=localtime(&i);
		char buffer[1025];
		snprintf(buffer, 1024, "[%s %02d %s %04d | %02d:%02d:%02d] ", go_days[ti->tm_wday], ti->tm_mday,
				go_months[ti->tm_mon], 1900+ti->tm_year, ti->tm_hour, ti->tm_min, ti->tm_sec);
		fwrite(&buffer, 1, strlen(buffer), go_logfile);
		va_start(args, blha);
		vfprintf(go_logfile, blha, args);
		va_end(args);
		sprintf(buffer,"\n");
		fwrite(&buffer, 1, 1, go_logfile);
		fflush(go_logfile);
	} else {
		go_logprint(blha, args);
	}
}

void go_logvprint(const char*blha, va_list args) {
	struct tm* ti;
	time_t i=time(NULL);
	ti=localtime(&i);
	printf("[%s %02d %s %04d | %02d:%02d:%02d] ", go_days[ti->tm_wday], ti->tm_mday,
			go_months[ti->tm_mon], 1900+ti->tm_year, ti->tm_hour, ti->tm_min, ti->tm_sec);
	vprintf(blha, args);
	printf("\n");
}

void go_logprint(const char *blha, ...) {
	va_list args;
	va_start(args,blha);
	go_logvprint(blha, args);
	va_end(args);

}

void go_loginit(const char *path) {
	int f=creat(path, 0700);
	if (f>0)
		close(f);

	go_logfile=fopen(path, "ab");
	
	if (go_logfile<0) {
		perror("[libgo:go_loginit()] Can not load the log file");
		exit(1);
	}
	go_logappend("----------SESSION---------");
}

void go_logclose() {
	fclose(go_logfile);
}
