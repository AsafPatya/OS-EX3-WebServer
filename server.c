#include "segel.h"
#include "request.h"
#include "WorkerThread.h"
#include "request_manager.h"
#include "request_object.h"

#define MAXSCHHEDALGLEN 7

pthread_mutex_t Lock;
pthread_cond_t WaitingQueueEmpty;
pthread_cond_t QueuesFull;
RequestManager requestsManager;
//
void* thread_function(void* thread)
{
    WorkerThread* this_thread = (WorkerThread*)thread;
    while (1)
    {
        pthread_mutex_lock(&Lock);
        while(!requestManagerHasWaitingRequests(requestsManager))
        {
            pthread_cond_wait(&WaitingQueueEmpty, &Lock);
        }
        RequestObject requestObject = requestManagerGetReadyRequest(requestsManager);
        requestManagerAddReadyRequest(requestsManager, requestObject);

        printf("\n\n*******request(%d) is now running*******\n\n", requestObject->val);
        requestManagerPrint(requestsManager);

        int fd = requestObject->val;
        struct timeval arrival_time = requestObject->time_arrive;
        struct timeval dispatch_interval = requestObject->disp;

        pthread_mutex_unlock(&Lock);

//        sleep(25);

        requestHandle(fd, this_thread, arrival_time, dispatch_interval);
        Close(fd);

        pthread_mutex_lock(&Lock);
        requestManagerRemoveFinishedRequest(requestsManager, requestObject);
        printf("\n\n*******request(%d) finished*******\n\n", requestObject->val);
        requestManagerPrint(requestsManager);
        //todo:delete requestObject
        pthread_cond_signal(&QueuesFull);
        pthread_mutex_unlock(&Lock);
    }
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
    // no use in threadsNum
    requestsManager = requestManagerCreate(0, queue_size);
}

void threads_pool_initialization(int threads){
    int number_of_threads = threads;

    pthread_t *thread_pool = (pthread_t *)malloc(sizeof(pthread_t) * number_of_threads);
    memset(thread_pool, 0, number_of_threads * sizeof(thread_pool[0]));

    for (int i = 0; i < number_of_threads; i++)
    {
        WorkerThread* thread = create_thread(i);
        if (pthread_create(&(thread_pool[i]), NULL, &thread_function, (void*)thread) != 0)
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

void addSignalAndUnlock(RequestObject requestObject){
    requestManagerAddPendingRequest(requestsManager, requestObject);
    pthread_cond_signal(&WaitingQueueEmpty);
    pthread_mutex_unlock(&Lock);

    printf("\n\n*******request (%d) added to w.q*******\n\n", requestObject->val);
    requestManagerPrint(requestsManager);
}

int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen, threads, queue_size;
    struct sockaddr_in clientaddr;
    char *schedalg = (char*)malloc(MAXSCHHEDALGLEN);

    getargs(&port, &threads, &queue_size, schedalg, argc, argv);

    pthread_mutex_init(&Lock, NULL);
    pthread_cond_init(&WaitingQueueEmpty, NULL); // only for waiting queue
    pthread_cond_init(&QueuesFull, NULL);        // for both queues

    queues_initialization(queue_size);
    threads_pool_initialization(threads);

    listenfd = Open_listenfd(port);

    while (1)
    {
	    clientlen = sizeof(clientaddr);
    	connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
        pthread_mutex_lock(&Lock);

        printf("\n\n********************************\n");
        printf("*******got a request (%d)*******", connfd);
        printf("\n********************************\n\n");
//        requestManagerPrint(requestsManager);

        if (requestManagerCanAcceptRequests(requestsManager))
        {
            RequestObject requestObject = createRequestObject(connfd);
            addSignalAndUnlock(requestObject);
        }
        else
        // not enough buffers are available (start of part2)
        {
            printf("can't accept any request. starting --%s-- procedure\n", schedalg);
            if (AreStringsEqual(schedalg, "block"))
            {
                while (!requestManagerCanAcceptRequests(requestsManager))
                {
                    pthread_cond_wait(&QueuesFull, &Lock);
                }
                RequestObject requestObject = createRequestObject(connfd);
                addSignalAndUnlock(requestObject);
            }
            else if (AreStringsEqual(schedalg, "dh"))
            {
                printf("dh1\n");
                if (!requestManagerCanAcceptRequests(requestsManager))
                {
                    printf("dh2\n");
                    if (!requestManagerHasWaitingRequests(requestsManager))
                    {
                        Close(connfd);
                        pthread_mutex_unlock(&Lock);
                        continue;
                    }
                    printf("dh3\n");
                    int fd1 = requestManagerRemoveOldestRequestFromWaitingQueue(requestsManager);
                    printf("removing %d from waiting queue\n", fd1);
                    Close(fd1);
                    RequestObject requestObject = createRequestObject(connfd);
                    addSignalAndUnlock(requestObject);
                }
            }
            else if (AreStringsEqual(schedalg, "random"))
            {
                printf("rand 1\n");
                if (!requestManagerCanAcceptRequests(requestsManager))
                {
                    printf("rand 2\n");
                    if(!requestManagerHasWaitingRequests(requestsManager))
                    {
                        Close(connfd);
                        pthread_mutex_unlock(&Lock);
                        continue;
                    }
                    printf("rand 3\n");
                    int waiting_queue_size = requestManagerGetWaitingQueueSize(requestsManager);
                    double half_waiting_queue = (((double) waiting_queue_size) / 2);
                    double num_to_delete = ceil((half_waiting_queue));
                    for (int i = 0; i < num_to_delete; i++) {
                        //rand between the queue size
                        int waiting_queue_size2 = requestManagerGetWaitingQueueSize(requestsManager);
                        int fd_to_delete = rand() % waiting_queue_size2;
                        RequestObject requestObject = requestManagerRemoveRequestFromWaitingQueueAtIndex(requestsManager, fd_to_delete);
                        printf("closing fd number %d\n", requestObject->val);
                        Close(requestObject->val);
                    }
                    printf("rand 4\n");
                    RequestObject requestObject = createRequestObject(connfd);
                    addSignalAndUnlock(requestObject);
                }
                //todo: free requestObject
            }
            else if (AreStringsEqual(schedalg, "dt"))
            {
                //todo: check if there is a need to remove the tail
                if (!requestManagerCanAcceptRequests(requestsManager))
                {
                    Close(connfd);
                    pthread_mutex_unlock(&Lock);
                    continue;
                }
            }
        }
    }
    pthread_mutex_destroy(&Lock);
    pthread_cond_destroy(&WaitingQueueEmpty);
    pthread_cond_destroy(&QueuesFull);
    requestManagerDelete(requestsManager);
}


    


 
