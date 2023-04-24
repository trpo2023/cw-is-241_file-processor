#include <ctype.h>
#include <dirent.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <libfileproc/directory.h>
#include <libfileproc/rename.h>

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

void to_lower_string(char* string)
{
    while (*string != '\0') {
        *string = tolower(*string);
        string++;
    }
}

void to_upper_string(char* string)
{
    while (*string != '\0') {
        *string = toupper(*string);
        string++;
    }
}

void change_register(char* name, unsigned int status)
{
    switch (status) {
    case R_LOW:
        to_lower_string(name);
        break;
    case R_HIGH:
        to_upper_string(name);
        break;
    }
}

char* get_name(char* file_path)
{
    char* name = file_path;
    int i;
    for (i = 0; file_path[i] != '\0'; i++) {
        if (file_path[i] == '/') {
            name = &file_path[i];
        }
    }

    return name;
}

int get_correct_name(char* old_name, char* new_name, char* dest, Option* opt)
{
    if (file_exist(old_name) == 1)
        return -1;

    if (strcmp(old_name, new_name) == 0) {
        strcpy(dest, old_name);
        return -1;
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

    char* name = get_name(newest_name);

    change_register(name, opt->name_register);
    if (suffix_len == 0)
        sprintf(dest, "%s", newest_name);
    else
        sprintf(dest, "%s%s", newest_name, suffix);

    while (file_exist(dest) == 0) {
        change_register(name, opt->name_register);
        if (suffix_len == 0)
            sprintf(dest, "%s (%d)", newest_name, counter++);
        else
            sprintf(dest, "%s (%d)%s", newest_name, counter++, suffix);
    }

    return 0;
}

char* rename_file(char* old_path, char* new_name, char* renamed, Option* opt)
{
    char* old_name = get_name(old_path);
    char* name = get_name(new_name);
    char new_path[MAX_LEN] = {0};
    if (old_name != old_path && name == new_name) {
        strncpy(new_path, old_path, old_name - old_path);
        strcat(new_path, "/");
        strcat(new_path, new_name);
        new_name = new_path;
    }
    int result = get_correct_name(old_path, new_name, renamed, opt);
    if (result == -1)
        return NULL;

    rename(old_path, renamed);
    return renamed;
}

void get_new_name(char* name, char* pattern, char* dest)
{
    size_t pattern_size = strlen(pattern);
    size_t name_size = strlen(name);

    for (int i = 0, j = 0, k = 0; k < pattern_size; i++) {
        if (j >= name_size) {
            dest[i] = pattern[k + 1];
            k++;
            continue;
        }
        if (pattern[k] == '*' && name[j] != pattern[k + 1] && name[j] != '.') {
            dest[i] = name[j];
            j++;
        } else if (pattern[k] == '*' && name[j] == '.') {
            dest[i] = pattern[k + 1];
            k += 2;
            j++;
        } else if (pattern[k] == '*' && name[j] == pattern[k + 1]) {
            dest[i] = name[j];
            j++;
            k += 2;
        } else if (pattern[k] == '?' || pattern[k] == name[j]) {
            dest[i] = name[j];
            j++;
            k++;
        } else if (
                pattern[k] == '.' && name[j] != '.' && pattern[k - 1] == '?') {
            while (name[j] != '.') {
                j++;
            }
            dest[i] = name[j++];
            k++;
        } else if (pattern[k] != name[j]) {
            dest[i] = pattern[k];
            k++;
        }
    }
}

GList* rename_pair(GList* pair, GList* renamed_files, Option* opt)
{
    char new_name[MAX_LEN] = {0};
    char* old_name = (char*)((File_to_rename*)pair->data)->filename;
    char* pattern = (char*)((File_to_rename*)pair->data)->pattern;
    get_new_name(old_name, pattern, new_name);

    char* newest_name = malloc(sizeof(char) * MAX_LEN);
    if (rename_file(old_name, new_name, newest_name, opt) == NULL) {
        return renamed_files;
    }

    // will be free, don't worry
    Renamed_file* renamed = malloc(sizeof(renamed));
    renamed->old_name = old_name;
    renamed->new_name = newest_name;

    renamed_files = g_list_append(renamed_files, renamed);
    return renamed_files;
}

GList* rename_files(GList* pair_list, Option* opt)
{
    GList* renamed_files = NULL;
    for (GList* i = pair_list; i != NULL; i = i->next) {
        renamed_files = rename_pair(i, renamed_files, opt);
    }
    return renamed_files;
}