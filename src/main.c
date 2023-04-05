#include <glib.h>
#include <stdio.h>

#include <directory.h>

void print_dir(GPtrArray a)
{
    printf("%s\n", (char*)a.pdata);
}

int main()
{
    GPtrArray* dir_list = read_dir(".");

    g_ptr_array_foreach(dir_list, (GFunc)print_dir, NULL);

    g_ptr_array_free(dir_list, FALSE);
    return 0;
}