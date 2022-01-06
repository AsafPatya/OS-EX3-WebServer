//
// Created by student on 12/29/21.
//

#include "request_manager.h"
#include "stdlib.h"

void *copyData(void *data)
{
    return copyRequestObject((RequestObject)data);
}

int compareData(Data data1, Data data2){
    return compareRequestObject((RequestObject)data1, (RequestObject)data2);
}

void freeData(void *data)
{
    deleteRequestObject((RequestObject)data);
}

RequestManager requestManagerCreate(int threadsNum, int maxAcceptedRequests){
    RequestManager requestManager = malloc(sizeof (*requestManager));
    requestManager->runningRequests = listCreate(copyData, compareData, freeData);
    if(requestManager->runningRequests == NULL){
        free(requestManager);
        return NULL;
    }
    requestManager->waitingRequestsQueue = listCreate(copyData, compareData, freeData);
    if(requestManager->waitingRequestsQueue == NULL){
        //todo:list free
        free(requestManager);
        return NULL;
    }
    requestManager->threadsNum = threadsNum;
    requestManager->maxAcceptedRequests = maxAcceptedRequests;
    return requestManager;
}

int requestManagerGetWaitingQueueSize(RequestManager requestManager){
    int size = listGetSize(requestManager->waitingRequestsQueue);
    return size;
}

int requestManagerHasWaitingRequests(RequestManager requestManager){
    int size = listGetSize(requestManager->waitingRequestsQueue);
    if(size > 1) return 1;
    return 0;
}

int requestManagerCanAcceptRequests(RequestManager requestManager){
    int s1 = listGetSize(requestManager->waitingRequestsQueue);
    int s2 = listGetSize(requestManager->runningRequests);
    if(requestManager->maxAcceptedRequests > s1 + s2)
        return 1;
    return 0;
}

RequestObject requestManagerGetReadyRequest(RequestManager requestManager){
    int size = listGetSize(requestManager->waitingRequestsQueue);
    if(size == 0)
        return NULL;//todo: return status instead
    RequestObject requestObject = listPopFront(requestManager->waitingRequestsQueue);
    return requestObject;
}

void requestManagerAddPendingRequest(RequestManager requestManager, RequestObject requestObject) {
    //todo: add only if there is enough room
    listPushBack(requestManager->waitingRequestsQueue, requestObject);
}

void requestManagerAddReadyRequest(RequestManager requestManager, RequestObject requestObject){
    //todo: add only if there is enough room
    listPushBack(requestManager->runningRequests, requestObject);
}

void requestManagerRemoveFinishedRequest(RequestManager requestManager, RequestObject requestObject){
    RequestObject requestObject1 = createRequestObject(-2);
    listRemoveAtData(requestManager->runningRequests,requestObject,(void**)(&requestObject1));
    //todo: there will be leak of memory
}

void requestManagerRemoveFinishedRequest(RequestManager requestManager, RequestObject requestObject){
    RequestObject requestObject1 = createRequestObject(-2);
    listRemoveAtData(requestManager->waitingRequestsQueue,requestObject,(void**)(&requestObject1));
    //todo: there will be leak of memory
}

void requestManagerRemoveOldestRequestFromWaitingQueue(RequestManager requestManager) {
    listPopFront(requestManager->waitingRequestsQueue);
}