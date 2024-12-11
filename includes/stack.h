#include "../includes/baseUtils.h"
#ifndef STACK_H
#define STACK_H

struct Node
{
    char op;
    int id;
    char nombre[31];
    Estado estado;
    char observaciones[51];
    struct Node *PtrProxNodo;
};

typedef struct Node StackNode;
typedef StackNode *StackNodePtr;

void push(StackNodePtr *, StackNodePtr *);
void pop(StackNodePtr *, StackNodePtr *);
int isEmpty(StackNodePtr);
StackNodePtr *createNode(char, Elemento *);
#endif