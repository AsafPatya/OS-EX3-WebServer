#ifndef OS_EX3_WEBSERVER_WORKERTHREAD_H
#define OS_EX3_WEBSERVER_WORKERTHREAD_H

#include "segel.h"

typedef struct WorkerThread
{
    int thread_id;
}WorkerThread;

WorkerThread* create_thread(int thread_id);

#endif //OS_EX3_WEBSERVER_WORKERTHREAD_H
