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

    return 0;
}