#ifndef COMMON_H
#define COMMON_H

typedef enum { FALSE = 0, TRUE = 1 } BOOL;

#define LOG(format, ...) printf(format, ##__VA_ARGS__)

#endif //COMMON_H