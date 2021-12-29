#include "segel.h"
#include "request.h"
#include "WorkerThread.h"
#include "request_manager.h"
#include "request_object.h"

pthread_mutex_t Lock;
pthread_cond_t WaitingQueueEmpty;
pthread_cond_t QueuesFull;
RequestManager requestsManager;

void* thread_function(void* thread)
{
    WorkerThread* this_thread = (WorkerThread*)thread;
    pthread_mutex_lock(&Lock);
    while(!requestManagerHasWaitingRequests(requestsManager))
    {
        pthread_cond_wait(&WaitingQueueEmpty, &Lock);
    }
    RequestObject requestObject = requestManagerGetReadyRequest(requestsManager);
    //todo: change the time
    requestManagerAddReadyRequest(requestsManager, requestObject);

    int fd = requestObject->val;

    pthread_mutex_unlock(&Lock);

    // todo: check in part2 and part3 and fix this call
    requestHandle(fd);
    Close(fd);

    pthread_mutex_lock(&Lock);
    requestManagerRemoveFinishedRequest(requestsManager, requestObject);
    //todo:delete requestObject
    pthread_cond_signal(&QueuesFull);
    pthread_mutex_unlock(&Lock);
    return NULL;
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
    requestsManager = requestManagerCreate(0, queue_size);
}

void threads_pool_initialization(int threads){
    int number_of_threads = threads;

    pthread_t *threads_pool = (pthread_t*)calloc(number_of_threads, sizeof(pthread_t));
    for (int i = 0; i < number_of_threads; i++)
    {
        WorkerThread* thread = create_thread(i);
        if (pthread_create(&(threads_pool[i]), NULL, &thread_function, (void*)thread) != 0)
        {
            fprintf(stderr, "pthread_create failed\n");
            exit(1);
        }
    }
}

int AreStringsEqual(char* schedalg, char* s)
{
    int len = strlen(s);
    for (int i = 0; i < len; i++)
    {
        if (schedalg[i] != s[i])
        {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen, threads, queue_size;
    struct sockaddr_in clientaddr;
    char* schedalg;
    getargs(&port, &threads, &queue_size, &schedalg, argc, argv);

    pthread_mutex_init(&Lock, NULL);
    pthread_cond_init(&WaitingQueueEmpty, NULL);
    pthread_cond_init(&QueuesFull, NULL);

    threads_pool_initialization(threads);
    queues_initialization(queue_size);

    listenfd = Open_listenfd(port);

    while (1)
    {
	    clientlen = sizeof(clientaddr);
    	connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
        pthread_mutex_lock(&Lock);

        if (requestManagerCanAcceptRequests(requestsManager))
        {
            RequestObject requestObject = createRequestObject(connfd);
            requestManagerAddPendingRequest(requestsManager,requestObject);
            //todo: free requestObject
            pthread_cond_signal(&WaitingQueueEmpty);
            pthread_mutex_unlock(&Lock);
        }
        else
        // not enough buffers are available (start of part2)
        {
            if (AreStringsEqual(schedalg, "block"))
            {

            }
            else if (AreStringsEqual(schedalg, "df"))
            {

            }
            else if (AreStringsEqual(schedalg, "dh"))
            {

            }
            else if (AreStringsEqual(schedalg, "random"))
            {

            }
        }

    }

}


    


 
