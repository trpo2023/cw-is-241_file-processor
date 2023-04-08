#include <glib.h>
#include <stdio.h>

#include <libfileproc/directory.h>
#include <libfileproc/rename.h>

void print_dir(GSList* a)
{
    printf("%s\n", (char*)a->data);
}

void print_pair(GSList* pair)
{
    char* name = (char*)((Renamed_pair*)pair->data)->old_name;
    char* new_name = (char*)((Renamed_pair*)pair->data)->new_name;
    printf("%s %s\n", name, new_name);
}

void clear_list_data(GSList* list)
{
    for (GSList* i = list; i != NULL; i = i->next) {
        free(i->data);
    }
}

// driver
int main()
{
    GSList* dir_list = read_dir(".");

    for (GSList* i = dir_list; i != NULL; i = i->next)
        print_dir(i);
    g_slist_free(dir_list);

    printf("\nUsing get_new_name(\"test.txt\", \"??.*.old\"):\n");
    char dest[50] = {0};
    get_new_name("test.txt", "??.*.old", dest);
    printf("Results: %s\n\n", dest);

    // trying create new files and rename them after
    // output: <old_file_name> <new_name>
    FILE* f = fopen("file-test.txt", "w");
    FILE* f1 = fopen("file-test123.txt", "w");
    fclose(f);
    fclose(f1);

    GSList* list_for_rename = NULL;
    Rename_pair pair1;
    strcpy(pair1.name, "file-test.txt");
    strcpy(pair1.pattern, "test.txt");
    list_for_rename = g_slist_append(list_for_rename, &pair1);

    Rename_pair pair2;
    strcpy(pair2.name, "file-test123.txt");
    strcpy(pair2.pattern, "new-*.txt");
    list_for_rename = g_slist_append(list_for_rename, &pair2);

    GSList* renamed_files = rename_files(list_for_rename);
    for (GSList* i = renamed_files; i != NULL; i = i->next) {
        print_pair(i);
    }

    clear_list_data(renamed_files);
    g_slist_free(renamed_files);
    g_slist_free(list_for_rename);
    return 0;
}