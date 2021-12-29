#ifndef OS_EX3_WEBSERVER_THREAD_H
#define OS_EX3_WEBSERVER_THREAD_H

#include "segel.h"

typedef struct Thread
{
    int thread_id;
}Thread;

Thread* create_thread(int thread_id);

#endif //OS_EX3_WEBSERVER_THREAD_H
