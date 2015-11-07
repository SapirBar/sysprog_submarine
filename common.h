#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string.h>
#include <errno.h>

typedef enum { FALSE = 0, TRUE = 1 } BOOL;

#define LOG_ERROR(msg, ...) fprintf(stderr, "[ERROR] (%s:%d) - " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_WARN(msg, ...) fprintf(stderr, "[WARNING] (%s:%d) - " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_INFO(msg, ...) fprintf(stderr, "[INFO] (%s:%d) - " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#endif //COMMON_H
