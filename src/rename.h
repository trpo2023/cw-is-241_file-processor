#pragma once

int file_exist(char* file_path);
char* get_suffix(char* file_name);
int get_correct_name(char* old_name, char* new_name, char* dest);
int rename_file(char* old_name, char* new_name);