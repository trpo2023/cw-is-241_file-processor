#include <interface/file_read.h>
#include <interface/init.h>

int null_file(WINDOW* sub, FILE* file)
{
    if (file == NULL) {
        mvwprintw(sub, 2, 1, "Не удалось открыть файл instruction.txt.");
        mvwprintw(sub, 3, 1, "Вывести информацию невозможно.");
        wrefresh(sub);
        wgetch(sub);
        remove_current_window(sub);
        return -1;
    }
    return 0;
}

GList* read_file(WINDOW* sub, int x, char* path)
{
    FILE* file = fopen(path, "r");
    if (null_file(sub, file))
        return NULL;
    GList* str_list = NULL;
    char* str = malloc(2 * x);
    while (s_gets(str, x * 2, file) != NULL) {
        str_list = g_list_append(str_list, str);
        str = malloc(2 * x);
    }
    free(str);
    fclose(file);
    return str_list;
}

char* s_gets(char* dest, size_t n, FILE* stream)
{
    if (fgets(dest, n, stream) == NULL)
        return NULL;
    for (int i = 0; dest[i] != '\0'; i++) {
        if (dest[i] == '\n') {
            dest[i] = '\0';
            break;
        }
    }
    return dest;
}
