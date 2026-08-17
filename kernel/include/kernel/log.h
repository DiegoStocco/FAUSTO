#ifndef LOG_H
#define LOG_H

#define LOG_CRITICAL 0
#define LOG_ERROR 1
#define LOG_WARN 2
#define LOG_INFO 3
#define LOG_DEBUG 4
#define LOG_TRACE 5


void kernel_panic(const char* msg);
void initialize_tty(void);
void log_msg(int log_lvl, const char* msg);

#endif
