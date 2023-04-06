#pragma once
#include <glib.h>
#define MAX_LEN 256

typedef struct pair {
    char old_name[MAX_LEN];
    char new_name[MAX_LEN];
} Renamed_pair;

typedef struct pair1 {
    char name[MAX_LEN];
    char pattern[MAX_LEN];
} Rename_pair;

void get_new_name(char* name, char* pattern, char* dest);
int file_exist(char* file_path);
char* get_suffix(char* file_name);
int get_correct_name(char* old_name, char* new_name, char* dest);
char* rename_file(char* old_name, char* new_name, char* renamed);
GSList* rename_pair(GSList* pair, GSList* renamed_files);
GSList* rename_files(GSList* pair_list);