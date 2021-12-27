//
// Created by student on 12/27/21.
//

#ifndef OS_EX3_WEBSERVER_LIST_H
#define OS_EX3_WEBSERVER_LIST_H

typedef enum listResult_t {
    LS_SUCCESS,
    LS_OUT_OF_MEMORY,
    LS_NULL_ARGUMENT,
//    PQ_ELEMENT_DOES_NOT_EXISTS,
//    PQ_ITEM_DOES_NOT_EXIST,
//    PQ_ERROR
} ListResult;

typedef void * Data;

typedef void(*FreeDataFunction)(Data data);
typedef Data(*CopyDataFunction)(Data data);

typedef struct node_t {
    struct node_t *next;
    struct node_t *prev;
    CopyDataFunction copyDataFunction;
    FreeDataFunction freeDataFunction;
    Data data;
} *Node;

typedef struct list_t{
    Node head;
    Node tail;

    CopyDataFunction copyDataFunction;
    FreeDataFunction freeDataFunction;

    int size;
}*List;

List listCreate(CopyDataFunction copyDataFunction,FreeDataFunction freeDataFunction);

ListResult listPushBack(List list, Data data);
ListResult listPushFront(List list, Data data);

Data listPopFront(List list);
Data listPopBack(List list);

int listGetSize(List list);

void listPrint(List list);
#endif //OS_EX3_WEBSERVER_LIST_H
