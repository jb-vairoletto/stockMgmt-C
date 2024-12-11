#include "../includes/stack.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "../includes/baseUtils.h"

void push(StackNodePtr *nodoPtr, StackNodePtr *stackPtr)
{
    (*nodoPtr)->PtrProxNodo = *stackPtr;
    *stackPtr = *nodoPtr;
}

void pop(StackNodePtr *nodoPtr, StackNodePtr *stackPtr)
{
    // memcpy(*nodoPtr, *stackPtr, sizeof(StackNode));
    (*nodoPtr)->op = (*stackPtr)->op;
    (*nodoPtr)->id = (*stackPtr)->id;
    (*nodoPtr)->estado = (*stackPtr)->estado;
    (*nodoPtr)->PtrProxNodo = (*stackPtr)->PtrProxNodo;
    strcpy((*nodoPtr)->nombre, (*stackPtr)->nombre);
    strcpy((*nodoPtr)->observaciones, (*stackPtr)->observaciones);

    StackNodePtr tempPtr = *stackPtr;
    *stackPtr = (*stackPtr)->PtrProxNodo;
    free(tempPtr);
}

int isEmpty(StackNodePtr stackPtr)
{
    if (stackPtr == NULL)
    {
        return 1;
    }
    return 0;
}

StackNodePtr *createNode(char operacion, Elemento *elemento)
{
    StackNodePtr *nodoNuevo = (StackNodePtr *)malloc(sizeof(StackNodePtr));
    *nodoNuevo = (StackNodePtr)malloc(sizeof(StackNode));
    memset(*nodoNuevo, 0, sizeof(StackNode));
    (*nodoNuevo)->op = operacion;
    (*nodoNuevo)->id = elemento->id;
    (*nodoNuevo)->estado = elemento->estado;
    strcpy((*nodoNuevo)->nombre, elemento->nombre);
    strcpy((*nodoNuevo)->observaciones, elemento->observaciones);
    (*nodoNuevo)->PtrProxNodo = NULL;
    return nodoNuevo;
}
