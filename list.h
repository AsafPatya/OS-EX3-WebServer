//
// Created by student on 12/27/21.
//

#ifndef LIST_GENERIC_LIST_H
#define LIST_GENERIC_LIST_H

typedef enum listResult_t {
    LS_SUCCESS,
    LS_OUT_OF_MEMORY,
    LS_NULL_ARGUMENT,
    LS_OUT_OF_INDEX,
    LS_EMPTY,
    LS_NOT_FOUND,
    LS_ERROR
} ListResult;

typedef void * Data;

typedef Data(*CopyDataFunction)(Data data);
typedef void(*FreeDataFunction)(Data data);
typedef int(*CompareDataFunction)(Data data1,Data data2);

typedef struct node_t {
    struct node_t *next;
    struct node_t *prev;
    CopyDataFunction copyDataFunction;
    CompareDataFunction compareDataFunction;
    FreeDataFunction freeDataFunction;
    Data data;
} *Node;

typedef struct list_t{
    Node head;
    Node tail;

    CopyDataFunction copyDataFunction;
    CompareDataFunction compareDataFunction;
    FreeDataFunction freeDataFunction;

    int size;
}*List;

List listCreate(CopyDataFunction copyDataFunction, CompareDataFunction compareDataFunction, FreeDataFunction freeDataFunction);

ListResult listPushBack(List list, Data data);
ListResult listPushFront(List list, Data data);

ListResult listInsertAtIndex(List list, int index, Data data);

Data listPopFront(List list);
Data listPopBack(List list);

ListResult listRemoveAtIndex(List list, int index, Data* removedData);
ListResult listRemoveAtData(List list, Data data, Data* removedData);

int listGetSize(List list);

void listPrint(List list);
#endif //LIST_GENERIC_LIST_H
