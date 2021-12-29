#include "segel.h"
#include "request.h"

// 
// server.c: A very, very simple web server
//
// To run:
//  ./server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//

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


int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen, threads, queue_size;
    struct sockaddr_in clientaddr;
    char* schedalg;
    getargs(&port, &threads, &queue_size, &schedalg, argc, argv);
    listenfd = Open_listenfd(port);

    while (1)
    {
	    clientlen = sizeof(clientaddr);
    	connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);

	    requestHandle(connfd);

	    Close(connfd);
    }

}


    


 
