#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 256

typedef struct sample_parts {
    char* search_pattern;
    char* rename_pattern;
} sample_parts;

char* skip_space(char* string); // пропуск пробелов
int check_sample_string(char* string); // проверка полной строки с шаблонами
// Взятие отдельного паттерна из всей строки
char* get_pattern(char* input_string, char* pattern);
// Сдвинуть указатель к началу следующего паттерна
char* to_rename_pattern(char* input_string);
// Разбить строку на паттерны
void split_sample(char* input_string, sample_parts* patterns);
// Разделить строку и записать в структуру
int get_sample(char* inp_str, sample_parts* patterns);
GList* add_sample(GList* patterns, char* input_string, int* exit_code);
void free_sample_parts(void* patterns);
