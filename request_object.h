//
// Created by student on 12/29/21.
//

#ifndef OS_EX3_WEBSERVER_REQUEST_OBJECT_H
#define OS_EX3_WEBSERVER_REQUEST_OBJECT_H

typedef struct request_object_t{
    int val;
//    struct timeval time_arrive;
//    struct timeval disp;
}*RequestObject;

RequestObject createRequestObject(int val);
RequestObject copyRequestObject(RequestObject requestObject);

int compareRequestObject(RequestObject requestObject1, RequestObject requestObject2);
void deleteRequestObject(RequestObject requestObject);


#endif //OS_EX3_WEBSERVER_REQUEST_OBJECT_H
