#pragma once
#include <glib.h>
#define MAX_LEN 256

typedef struct {
    char* old_name;
    char* new_name;
} Renamed_file;

enum Register { R_DEFAULT, R_LOW, R_HIGH };

typedef struct {
    unsigned int name_register;
} Option;

void get_new_name(char* name, char* pattern, char* dest);
int file_exist(char* file_path);
char* get_suffix(char* file_name);
int get_correct_name(char* old_name, char* new_name, char* dest, Option* opt);
char* rename_file(char* old_name, char* new_name, char* renamed, Option* opt);
GList* rename_pair(GList* pair, GList* renamed_files, Option* opt);
GList* rename_files(GList* pair_list, Option* opt);