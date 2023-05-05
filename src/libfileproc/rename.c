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
        return 0;
    }

    fclose(file);
    return 1;
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

void write_correct_index(char* new_name, char* dest, char* suffix, Option* opt)
{
    size_t suf_len = 0;
    if (suffix != NULL)
        suf_len = strlen(suffix);

    int counter = 1;
    char* name = get_name(new_name);
    change_register(name, opt->name_register);

    if (suf_len == 0)
        sprintf(dest, "%s", new_name);
    else
        sprintf(dest, "%s%s", new_name, suffix);

    while (file_exist(dest) == 1) {
        change_register(name, opt->name_register);
        if (suf_len == 0)
            sprintf(dest, "%s (%d)", new_name, counter++);
        else
            sprintf(dest, "%s (%d)%s", new_name, counter++, suffix);
    }
}

int get_correct_name(char* old_path, char* new_name, char* dest, Option* opt)
{
    if ((file_exist(old_path) == 0 || strcmp(old_path, new_name) == 0)
        && opt->name_register == R_DEFAULT)
        return -1;

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

    write_correct_index(newest_name, dest, suffix, opt);

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

    if (get_correct_name(old_path, new_name, renamed, opt) == -1)
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
            if (pattern[k] == '.' && pattern[k - 1] != '*') {
                dest[i] = pattern[k];
                dest[i + 1] = pattern[k + 1];
                k++;
                i++;
            } else if (pattern[k + 1] == '\0' && k - i == 0) {
                dest[i] = pattern[k++];
            } else {
                dest[i] = pattern[k + 1];
                k++;
            }
            continue;
        }
        if (pattern[k] == '*' && name[j] != pattern[k + 1] && name[j] != '.') {
            dest[i] = name[j++];
        } else if (pattern[k] == '*' && name[j] == '.') {
            dest[i] = pattern[k + 1];
            k += 2;
            j++;
        } else if (pattern[k] == '*' && name[j] == pattern[k + 1]) {
            dest[i] = name[j++];
            k += 2;
        } else if (pattern[k] == '?' || pattern[k] == name[j]) {
            dest[i] = name[j++];
            k++;
        } else if (
                pattern[k] == '.' && name[j] != '.' && pattern[k - 1] == '?') {
            while (name[j] != '.') {
                j++;
            }
            dest[i] = name[j++];
            k++;
        } else if (pattern[k] != name[j]) {
            dest[i] = pattern[k++];
        }
    }
}

void make_str_smallest(char* str, char* dest, size_t len, size_t max_len)
{
    int i = 0, j = 0;
    int stop1 = max_len / 2;
    int stop2 = len - stop1 + 3;

    while (i < stop1) {
        dest[i] = str[i];
        i++;
    }
    while (i < stop1 + 3)
        dest[i++] = '.';

    while (i < max_len) {
        dest[i++] = str[stop2 + j];
        j++;
    }
    dest[i] = '\0';
}

char* write_correct_renamed_string(int x, char* old_name, char* new_name)
{
    size_t old_len = strlen(old_name);
    size_t new_len = strlen(new_name);
    char* string = malloc(sizeof(char) * (x - 1));
    if (old_len + new_len + 5 < x - 3) {
        sprintf(string,
                "%-*s -> %*s",
                (x / 2) - 4,
                old_name,
                (x / 2) - 5,
                new_name);
    } else {
        char old[MAX_LEN] = {0};
        char new[MAX_LEN] = {0};
        make_str_smallest(old_name, old, old_len, (x / 2) - 4);
        make_str_smallest(new_name, new, new_len, (x / 2) - 5);
        sprintf(string, "%-*s -> %*s", (x / 2) - 4, old, (x / 2) - 5, new);
    }

    return string;
}

GList* rename_pair(GList* pair, GList* renamed_files, Option* opt)
{
    char new_name[MAX_LEN] = {0};
    char* old_name = ((File_to_rename*)pair->data)->filename;
    char* pattern = ((File_to_rename*)pair->data)->pattern;

    char* name = get_name(old_name);
    if (old_name != name) {
        name++;
    }
    get_new_name(name, pattern, new_name);

    char* newest_name = malloc(sizeof(char) * MAX_LEN);
    if (rename_file(old_name, new_name, newest_name, opt) == NULL) {
        return renamed_files;
    }

    RenamedFile* renamed = malloc(sizeof(RenamedFile));
    renamed->old_path = old_name;
    renamed->new_path = newest_name;

    renamed_files = g_list_append(renamed_files, renamed);
    return renamed_files;
}

GList* rename_files(GList* to_rename_list, Option* opt)
{
    GList* renamed_files = NULL;
    for (GList* i = to_rename_list; i != NULL; i = i->next) {
        renamed_files = rename_pair(i, renamed_files, opt);
    }

    return renamed_files;
}
