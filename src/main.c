#include <glib.h>
#include <stdio.h>

#include <directory.h>
#include <rename.h>

void print_dir(GPtrArray a)
{
    printf("%s\n", (char*)a.pdata);
}

// for example
int main()
{
    GPtrArray* dir_list = read_dir(".");

    g_ptr_array_foreach(dir_list, (GFunc)print_dir, NULL);

    g_ptr_array_free(dir_list, FALSE);
    char name[128] = {0};
    char file1[] = ".gitignore";
    char file2[] = "Makefile";
    printf("\nUsing get_correct_name(%s, %s):\n", file1, file2);
    int result = get_correct_name(file1, file2, name);
    switch (result) {
    case 0:
        printf("Correct name for '%s to %s':\n%s\n", file1, file2, name);
        break;
    case -1:
        printf("%s is not exist\n", file2);
        break;
    case 1:
        printf("%s == %s\n", file1, file2);
        printf("Correct name for '%s to %s':\n%s\n", file1, file2, name);
    }

    return 0;
}