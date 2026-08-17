#ifndef LOG_H
#define LOG_H

#define LOG_CRITICAL 0
#define LOG_ERROR 1
#define LOG_WARN 2
#define LOG_INFO 3
#define LOG_DEBUG 4
#define LOG_TRACE 5

#define BROADCAST_OK 0
#define BROADCAST_FAIL 1

void kernel_panic(const char* msg);
void initialize_tty(void);
void log_msg(int log_lvl, const char* unit, const char* msg);
void broadcast_status(int broadcast_status_code, char* unit);
char* get_prefix_from_loglvl(int log_lvl);

#endif
