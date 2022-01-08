//
// Created by student on 12/29/21.
//

#include "request_object.h"
#include "stdlib.h"

RequestObject createRequestObject(int val){
    RequestObject requestObject = malloc(sizeof (*requestObject));
    if(requestObject == NULL)
        return requestObject;
    requestObject->val = val;
    gettimeofday(&(requestObject->time_arrive),NULL);
    return requestObject;
}

RequestObject copyRequestObject(RequestObject requestObject){
    return createRequestObject(requestObject->val);
}

int compareRequestObject(RequestObject requestObject1, RequestObject requestObject2){
    if(requestObject1->val > requestObject2->val) return 1;
    if(requestObject1->val == requestObject2->val) return 0;
    return -1;
}

void deleteRequestObject(RequestObject requestObject){
    free(requestObject);
}