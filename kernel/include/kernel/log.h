#ifndef LOG_H
#define LOG_H

enum LOG_LEVEL{ 
	CRITICAL,
	ERROR,
	WARN,
	INFO,
	DEBUG,
	TRACE
};

void kernel_panic(const char* msg);
void initialize_tty(void);
void log_msg(enum LOG_LEVEL level, const char* msg);

#endif
