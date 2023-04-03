#include <stdlib.h>
#include <string.h>

#include "list.h"

void init_list(List* list)
{
    *list = NULL;
}

bool list_add(List* list, char* path)
{
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) {
        return false;
    }

    strcpy(&node->path[0], path);
    node->next = *list;
    *list = node;
    return true;
}

void list_clear(List* list)
{
    Node* next;
    while (*list != NULL) {
        next = (*list)->next;
        free(*list);
        *list = next;
    }
}

void traverse_list(List* list, void (*pfun)(Node* list))
{
    Node* pnode = *list;
    while (pnode != NULL) {
        (*pfun)(pnode);
        pnode = pnode->next;
    }
}