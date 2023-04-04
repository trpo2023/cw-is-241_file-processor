#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <rename.h>

int file_exist(char* file_path)
{
    FILE* file = fopen(file_path, "r");
    if (!file) {
        return 1;
    }

    fclose(file);
    return 0;
}

char* get_suffix(char* file_name)
{
    while (*file_name != '\0') {
        if (*file_name == '.')
            return file_name;
        file_name++;
    }

    return NULL;
}

int get_correct_name(char* old_name, char* new_name, char* dest)
{
    if (file_exist(old_name) == 1)
        return -1;

    if (strcmp(old_name, new_name) == 0) {
        strcpy(dest, old_name);
        return 1;
    }

    int counter = 1;
    size_t dest_len = strlen(new_name);
    size_t name_len = dest_len;
    size_t suffix_len = 0;

    char* suffix = get_suffix(new_name);
    if (suffix != NULL) {
        suffix_len = strlen(suffix);
        name_len = dest_len - suffix_len;
    }

    char newest_name[dest_len];
    strncpy(newest_name, new_name, name_len);
    newest_name[name_len] = '\0';

    if (suffix_len == 0)
        sprintf(dest, "%s", newest_name);
    else
        sprintf(dest, "%s%s", newest_name, suffix);

    while (file_exist(dest) == 0) {
        if (suffix_len == 0)
            sprintf(dest, "%s (%d)", newest_name, counter++);
        else
            sprintf(dest, "%s (%d)%s", newest_name, counter++, suffix);
    }

    return 0;
}

int rename_file(char* old_name, char* new_name)
{
    size_t len = strlen(new_name);
    char name[len + 10]; // 10 bytes for number of file
    int result = get_correct_name(old_name, new_name, name);
    if (result == -1)
        return -1;

    rename(old_name, name);
    return 0;
}