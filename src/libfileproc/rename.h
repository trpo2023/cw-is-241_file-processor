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

enum Register { R_DEFAULT, R_LOW, R_HIGH };

typedef struct option {
    unsigned int name_register;
} Option;

void get_new_name(char* name, char* pattern, char* dest);
int file_exist(char* file_path);
char* get_suffix(char* file_name);
int get_correct_name(char* old_name, char* new_name, char* dest, Option* opt);
char* rename_file(char* old_name, char* new_name, char* renamed, Option* opt);
GSList* rename_pair(GSList* pair, GSList* renamed_files, Option* opt);
GSList* rename_files(GSList* pair_list, Option* opt);