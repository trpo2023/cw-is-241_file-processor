#pragma once

#define MAX_LEN 256 // Максимальная длина пути или имени

typedef struct {
    char* old_path; // Относительный путь к файлу до переименования
    char* new_path; // Относительный путь к файлу после переименония
} RenamedFile;

enum Register { R_DEFAULT, R_LOW, R_HIGH };

typedef struct {
    unsigned int name_register; // Принимает одно из значение Register
} Option;

// Принимает:   file_name - строка с именем файла.
//
// Возвращает:  Указатель на последнее вхождение символа '.'.
//              Если строка не содержит символ '.', возвращается NULL.
char* get_suffix(char* file_name);

// Принимает:   file_path - строка с путем к файлу.
//
// Возвращает:  Указатель на последнее вхождение символа '/'.
//              Если строка не содержит символ '/', возвращается
//              указатель на исходный путь.
char* get_name(char* file_path);

// Принимает:   file_path - строка с путем к файлу.
//              pattern - строка с шаблоном.
//              dest - строка для записи нового имени.
//
// Функция генерирует новое имя для файла в соответствии с шаблоном.
void get_new_name(char* file_path, char* pattern, char* dest);

// Принимает:   old_path - строка с путем к файлу.
//              new_name - строка с новым именем для файла.
//              dest - строка для записи корректного имени.
//              opt - укзаатель на структуру Option.
//
// Функция пытается установить, существует ли файл new_name.
// Если файл new_name уже сущетвует, то к имени файла добавляется индекс.
// Например file_exist.txt уже существует, тогда функция проверит
// file_exist (1).txt на существование и т.д.
//
// Возвращает:  0, если удалось получить корректное имя.
//              -1, если файл old_path не существует или
//              если new_name не отличается от old_path.
int get_correct_name(char* old_path, char* new_name, char* dest, Option* opt);

void make_str_smallest(char* str, char* dest, size_t len, size_t max_len);
char* write_correct_renamed_string(int x, char* old_name, char* new_name);

// Принимает:   to_rename_list - список структур File_to_rename.
//              opt - укзаатель на структуру Option.
//
// Функция проходится по каждому узлу списка to_rename_list
// и переименовывает каждый файл в соответствии с шаблоном.
//
// Возвращает:  Список структур Renamed_file - список
//              переименнованных файлов.
//              NULL, если не было переименованных файлов.
GList* rename_files(GList* to_rename_list, Option* opt);