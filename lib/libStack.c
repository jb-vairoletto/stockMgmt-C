#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "../includes/stack.h"
#include "../includes/baseUtils.h"
#include "../includes/persist.h"

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

void printStack(StackNodePtr stackPointer)
{
    if (isEmpty(stackPointer))
    {
        printf("La pila está vacía");
    }
    StackNodePtr nextNode = stackPointer;
    int iter = 1;
    printf("Ultima:--> ");
    while (nextNode != NULL)
    {
        char operacion = nextNode->op;
        extern char *ESTADOS[];
        switch (operacion)
        {
        case 'c':
            printf("Creaste el elemento con ID: %d,\tNombre: %s\n\t", nextNode->id, nextNode->nombre);
            break;
        case 'u':
            printf("Actualizaste el elemento con ID: %d, que se encontraba %s, %s\n\t", nextNode->id,
                   ESTADOS[nextNode->estado],
                   nextNode->observaciones);
            break;
        case 'd':
            printf("Eliminaste el elemento con ID: %d,\tNombre: %s\n\t", nextNode->id, nextNode->nombre);
            break;
        }
        iter++;
        nextNode = nextNode->PtrProxNodo;
    }
}

void undoCreate(StackNodePtr *stackPointer)
{

    StackNodePtr nodo = (StackNodePtr)malloc(sizeof(StackNode));

    memset(nodo, 0, sizeof(StackNode));
    pop(&nodo, stackPointer);

    borrarElemento(nodo->id);
    // free(*nodo);
    // free(nodo);
}

void undoUpdate(StackNodePtr *stackPointer)
{
    StackNodePtr nodo = (StackNodePtr)malloc(sizeof(StackNode));

    memset(nodo, 0, sizeof(StackNode));
    pop(&nodo, stackPointer);

    Elemento *elem = (Elemento *)malloc(sizeof(Elemento));
    elem = (Elemento *)memset(elem, 0, sizeof(Elemento));

    elem->id = nodo->id;
    elem->estado = nodo->estado;
    strcpy(elem->nombre, nodo->nombre);
    strcpy(elem->observaciones, nodo->observaciones);
    int success = guardarElemento(elem);
    if (success)
    {
        printf("\nSe ha restaurado el elemento previamente modificado con ID: %d\n", elem->id);
    }
    else
    {
        printf("\nHubo un error realizando el proceso solicitado\n");
    }

    free(nodo);
    free(elem);
}

void undoDelete(StackNodePtr *stackPointer)
{
    StackNodePtr nodo = (StackNodePtr)malloc(sizeof(StackNode));

    memset(nodo, 0, sizeof(StackNode));
    pop(&nodo, stackPointer);

    Elemento *elem = (Elemento *)malloc(sizeof(Elemento));
    elem = (Elemento *)memset(elem, 0, sizeof(Elemento));

    elem->id = nodo->id;
    elem->estado = nodo->estado;
    strcpy(elem->nombre, nodo->nombre);
    strcpy(elem->observaciones, nodo->observaciones);
    int success = guardarElemento(elem);
    if (success)
    {
        printf("\nSe ha restaurado el elemento eliminado con ID: %d\n", elem->id);
    }
    else
    {
        printf("\nHubo un error realizando el proceso solicitado\n");
    }
    free(nodo);
    free(elem);
}
