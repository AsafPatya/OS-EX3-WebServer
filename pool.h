#ifndef OS_EX3_WEBSERVER_POOL_H
#define OS_EX3_WEBSERVER_POOL_H
#include <stdbool.h>
#include <pthread.h>
#include "segel.h"
#include "request.h"

typedef struct pool_t {
    int running_threads;
    int threads_nums;
}*pool;

pool create_new_pool(int threads_nums);

void insert_to_pull(pool pool);

void delete_from_pool(pool pool);


#endif //OS_EX3_WEBSERVER_POOL_H
