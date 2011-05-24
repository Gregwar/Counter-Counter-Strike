/*
 * This is part of the GO FrameWork
 */
#ifndef GOLOG
#define GOLOG
#include <stdarg.h>

/*
 * INIT THE LOG
 * Open the given file
 */
void go_loginit(const char *path);

/*
 * APPEND THE LOG
 * Add the string to the file with timestamp
 */
void go_logappend(const char *blha, ...);
void go_logvprint(const char*blha, va_list args);
void go_logprint(const char *blha, ...);

/*
 * CLOSE THE LOG
 * Close the log file
 */
void go_logclose();

#endif
