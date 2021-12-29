#include "WorkerThread.h"

WorkerThread* create_thread(int thread_id)
{
    WorkerThread* thread = (WorkerThread*) malloc(sizeof (WorkerThread));
    thread->thread_id = thread_id;
    return thread;
}