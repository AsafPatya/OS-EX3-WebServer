#include "segel.h"
#include "request.h"
#include "Thread.h"

pthread_mutex_t Lock;
pthread_cond_t isWaitingQueueEmpty;
pthread_cond_t isWaitingQueueFull;
//todo: declaration of struct Queue WaitingQueue
//todo: declaration of struct Queue WorkingQueue

void thread_function(void* thread)
{
    Thread* this_thread = (Thread*)thread;
    pthread_mutex_lock(&Lock);
    //todo: implement IsWaitingQueueEmpty
    while(IsWaitingQueueEmpty(WaitingQueue))
    {
        pthread_cond_wait(&isWaitingQueueEmpty, &Lock);
    }

    // todo: implement GetFirstReqWaitingQueue
    FirstReqInWaitingQueue = GetFirstReqWaitingQueue(WaitingQueue);
    int fd = FirstReqInWaitingQueue->fd;
    RemoveReqFromWaitingQueue(fd);
    AddNewReqToWorkingQueue(fd);

    pthread_mutex_unlock(&Lock);

    // todo: check in part2 and part3 and fix this call
    requestHandle(fd);
    Close(fd);

    pthread_mutex_lock(&Lock);
    pthread_cond_signal(&isWaitingQueueFull);
    pthread_mutex_unlock(&Lock);



}

void getargs(int *port, int *threads, int *queue_size, char *schedalg, int argc, char *argv[])
{
    if (argc < 5)
    {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    *port = atoi(argv[1]);
    *threads = atoi(argv[2]);
    *queue_size = atoi(argv[3]);
    strcpy(schedalg, argv[4]);
}

void queues_initialization(int queue_size)
{
    //todo: initialization of the two queues
}

void threads_pool_initialization(int threads){
    int number_of_threads = threads;

    pthread_t *threads_pool = (pthread_t*)calloc(number_of_threads, sizeof(pthread_t));
    for (int i = 0; i < number_of_threads; i++)
    {
        Thread* thread = create_thread(i);
        if (pthread_create(&(threads_pool[i]), NULL, &thread_function, (void*)thread) != 0)
        {
            fprintf(stderr, "pthread_create failed\n");
            exit(1);
        }
    }
}

int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen, threads, queue_size;
    struct sockaddr_in clientaddr;
    char* schedalg;
    getargs(&port, &threads, &queue_size, &schedalg, argc, argv);

    threads_pool_initialization(threads);
    queues_initialization(queue_size);

    listenfd = Open_listenfd(port);

    while (1)
    {
	    clientlen = sizeof(clientaddr);
    	connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);

	    requestHandle(connfd);

	    Close(connfd);
    }

}


    


 
