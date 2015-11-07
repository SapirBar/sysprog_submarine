#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string.h>
#include <errno.h>

typedef enum { FALSE = 0, TRUE = 1 } BOOL;

#define LOG_ERROR(msg, ...) fprintf(stderr, "[ERROR] (%s:%d) - " msg "\r\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_WARN(msg, ...) fprintf(stderr, "[WARNING] (%s:%d) - " msg "\r\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_INFO(msg, ...) fprintf(stderr, "[INFO] (%s:%d) - " msg "\r\n", __FILE__, __LINE__, ##__VA_ARGS__)

typedef enum {
	SUCCESS = 0,
	WRONG_PARAMETERS,
	SUBMARINE_INITIALIZATION_FAILED,
	TEXT_READER_INITIALIZATION_FAILED,
	SUBMARINE_OUTPUT_WRITER_FAILED
} ErrorCodes;

#endif //COMMON_H
