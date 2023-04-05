#include <glib.h>
#include <stdio.h>

#include <directory.h>

void print(Node* list)
{
    printf("%s\n", list->path);
}

int main()
{
    List dir_list = read_dir(".");
    traverse_list(&dir_list, print);
    list_clear(&dir_list);

    GSList *list = NULL, *iterator = NULL;
    list = g_slist_append(list, "hello");
    list = g_slist_append(list, "world");
    list = g_slist_append(list, "application");

    for (iterator = list; iterator; iterator = iterator->next) {
        printf("Current item is '%s'\n", (char*)iterator->data);
    }

    g_slist_free(list);
    return 0;
}