#include "Thread.h"

Thread* create_thread(int thread_id)
{
    Thread* thread = (Thread*) malloc(sizeof (Thread));
    thread->thread_id = thread_id;
    return thread;
}