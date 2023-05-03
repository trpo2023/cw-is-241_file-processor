#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 256

typedef struct Splitted_patterns{
    char* search_pattern;
    char* rename_pattern;
} Splitted_patterns;

char* skip_space(char* string);
int check_sample_string(char* string);
char* get_pattern(char* input_string, char* pattern);
char* to_rename_pattern(char* input_string);
void split_sample(char* input_string, Splitted_patterns* patterns);
int get_sample(char* inp_str, Splitted_patterns* patterns);
GList* add_sample(GList* patterns, char* input_string, int* exit_code);
void free_Splitted_patterns(void* patterns);
