#pragma once
#include <stdbool.h>
#define MAX_LEN 256

typedef struct list {
    char path[MAX_LEN];
    struct list* next;
} Node;

typedef Node* List;

void init_list(List* list);
bool list_add(List* list, char* path);
void list_clear(List* list);
void traverse_list(List* list, void (*pfun)(Node* list));