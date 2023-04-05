#include <glib.h>
#include <stdio.h>

#include <directory.h>

int main()
{
    GSList* dir_list = read_dir(".");

    for (GSList* i = dir_list; i != NULL; i = i->next)
        printf("%s\n", (char*)i->data);

    g_slist_free(dir_list);
    return 0;
}