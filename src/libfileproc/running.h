#pragma once

// Принимает: sample - список с шаблонами
//            dir_path -строка, содержащая путь к файлу
//            opt - указатель на структуру Option
//
// Запускает несколько функций,
// которые сначала получают список файлов в директории,
// а после список файлов готовых к переименованию,
// и переименовывает их
//
// Возвращает: список переименнованых файлов
GList* rename_and_get_renamed_list(GList* sample, char* dir_path, Option* opt);
