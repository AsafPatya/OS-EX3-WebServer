//
// Created by student on 12/29/21.
//

#ifndef OS_EX3_WEBSERVER_REQUEST_MANAGER_H
#define OS_EX3_WEBSERVER_REQUEST_MANAGER_H

#include "list.h"
#include "request_object.h"

typedef struct request_manager_t{
    List runningRequests;
    List waitingRequestsQueue;

    int maxAcceptedRequests;
    int threadsNum;//todo: remove

}*RequestManager;

RequestManager requestManagerCreate(int threadsNum, int maxAcceptedRequests);
int requestManagerGetWaitingQueueSize(RequestManager requestManager);
int requestManagerHasWaitingRequests(RequestManager requestManager);
int requestManagerCanAcceptRequests(RequestManager requestManager);

RequestObject requestManagerGetReadyRequest(RequestManager requestManager);//todo: return status instead

void requestManagerAddPendingRequest(RequestManager requestManager, RequestObject requestObject);//todo: return status instead
void requestManagerAddReadyRequest(RequestManager requestManager, RequestObject requestObject);//todo: return status instead
void requestManagerRemoveFinishedRequest(RequestManager requestManager, RequestObject requestObject);//todo: return status instead
void requestManagerRemoveRequestFromWaitingQueue(RequestManager requestManager, RequestObject requestObject);//todo: return status instead
RequestObject requestManagerRemoveRequestFromWaitingQueueAtIndex(RequestManager requestManager, int index);//todo: return status instead

int requestManagerRemoveOldestRequestFromWaitingQueue(RequestManager requestManager);//todo: return status instead

void requestManagerDelete(RequestManager requestManager);


void requestManagerPrint(RequestManager requestManager);
#endif //OS_EX3_WEBSERVER_REQUEST_MANAGER_H
