//
// Created by student on 12/27/21.
//

#include "list.h"
#include "stdlib.h"

///
/// Node section
///

Node nodeCreate(Data data,CopyDataFunction copyDataFunction, FreeDataFunction freeDataFunction) {
    Node newNode = malloc(sizeof(*new_node));
    if (newNode == NULL) {
        return NULL;
    }
    newNode->data = copyDataFunction(data);
    if(newNode->data == NULL){
        free(newNode);
        return NULL;
    }
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->copyDataFunction = copyDataFunction;
    newNode->freeDataFunction = freeDataFunction;
    return new_node;
}

int isNodeEdge(Node node){
    if(node->data == NULL)
        return 1;
    return 0;
}

void nodeSetNext(Node currentNode, Node nextNode){
    Node tempNext = currentNode->next;

    currentNode->next = nextNode;
    nextNode->next = tempNext;

    tempNext->prev = nextNode;
    nextNode->prev = currentNode;
}

void nodeSetPrev(Node currentNode, Node prevNode){
    Node tempPrev = currentNode->prev;

    currentNode->prev = prevNode;
    prevNode->prev = tempPrev;

    tempPrev->next = prevNode;
    prevNode->next = currentNode;
}

void nodeFree(Node node){
    node->freeDataFunction(node->data);
    free(node);
}

///
/// List section
///

List listCreate(CopyDataFunction copyDataFunction, FreeDataFunction freeDataFunction){
    List list = malloc(sizeof(*list));
    if(list == NULL){
        return NULL;
    }
    list->copyDataFunction = copyDataFunction;
    list->freeDataFunction = freeDataFunction;

    list->head = nodeCreate(NULL, copyDataFunction, freeDataFunction);
    if(list->head == NULL){
        free(list);
        return NULL;
    }

    list->tail = nodeCreate(NULL, copyDataFunction, freeDataFunction);
    if(list->tail == NULL){
        nodeFree(list->head);
        free(list);
        return NULL;
    }

    list->head->next = list->tail;
    list->tail->prev = list->head;

    list->size = 0;

    return list;
}


ListResult ListPushBack(List list, Data data){
    if(list == NULL || data == NULL){
        return LS_NULL_ARGUMENT;
    }
    Node node = nodeCreate(list->freeDataFunction);
    if(node == NULL){
        return LS_OUT_OF_MEMORY;
    }
    list->size++;
    nodeSetPrev(list->tail, node);
    return LS_SUCCESS;
}

ListResult ListPushFront(List list, Data data){
    if(list == NULL || data == NULL){
        return LS_NULL_ARGUMENT;
    }
    Node node = nodeCreate(list->freeDataFunction);
    if(node == NULL){
        return LS_OUT_OF_MEMORY;
    }
    list->size++;
    nodeSetNext(list->head, node);
    return LS_SUCCESS;
}

Data listPopFront(List list){
    if(list->size == 0){
        return NULL;
    }
    Node temp = list->head->next;
    list->head->next = temp->next;
    temp->next->prev = list->head;

    Data data = temp->data;
    nodeFree(temp);
    return data;
}

Data listPopBack(List list){
    if(list->size == 0){
        return NULL;
    }
    Node temp = list->tail->prev;
    list->tail->prev = temp->prev;
    temp->prev->next = list->tail;

    Data data = temp->data;
    nodeFree(temp);
    return data;
}