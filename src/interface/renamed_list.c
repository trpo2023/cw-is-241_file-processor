#include <libfileproc/clean_memory.h>
#include <libfileproc/directory.h>
#include <libfileproc/running.h>

#include <interface/get_item.h>
#include <interface/init.h>
#include <interface/print.h>
#include <interface/renamed_list.h>

GList* get_renamed_list(WINDOW* sub, GList* renamed_files)
{
    int x = getmaxx(sub);

    GList* str_list = NULL;
    for (GList* i = renamed_files; i != NULL; i = i->next) {
        char* old_name = ((RenamedFile*)i->data)->old_path;
        char* new_name = ((RenamedFile*)i->data)->new_path;
        char* str = write_correct_renamed_string(x - 5, old_name, new_name);
        str_list = g_list_append(str_list, str);
    }

    return str_list;
}

void print_renamed_list(WINDOW* sub, GList* renamed_list, int x, int y)
{
    if (renamed_list == NULL) {
        wgetch(sub);
        return;
    }

    int str_cnt = 0;
    size_t size = g_list_length(renamed_list);

    print_items(sub, renamed_list, &str_cnt, 5, 4);
    mvwprintw_highlite(sub, 4, 2, (char*)renamed_list->data);

    get_item(sub, renamed_list, y - 5, size, str_cnt, 5, 4);
}

void process_with_renamed_list(
        WINDOW* sub, GList* renamed_file_list, int x, int y)
{
    GList* list = get_renamed_list(sub, renamed_file_list);
    list = g_list_sort(list, my_comparator);
    print_renamed_list(sub, list, x, y);
    free_renamed_list(renamed_file_list, list);
}

void print_succes_message(GList* renamed_file_list, WINDOW* sub)
{
    int x = getmaxx(sub);
    if (g_list_length(renamed_file_list) == 0) {
        char str[] = "По введённым шаблонам файлов не найдено";
        mvwprintw(sub, 3, (x - 39) / 2, "%s", str);
        return;
    }
    char str[MAX_LEN];
    sprintf(str,
            "Успешно! Файлов переименовано: %d!",
            g_list_length(renamed_file_list));
    mvwprintw(sub, 3, (x - 34) / 2, "%s", str);
}

void process(WINDOW* menu, GList* sample, char* dir_path, Option* opt)
{
    int y, x;
    getmaxyx(menu, y, x);
    WINDOW* sub = init_sub_window(menu, y, x);
    getmaxyx(sub, y, x);
    GList* renamed_file_list = NULL;
    if (g_list_length(sample) == 0) {
        char stri[] = "Сперва введите шаблоны, после запускайте переименование";
        mvwprintw(sub, 1, (x - 55) / 2, "%s", stri);
        wrefresh(sub);
        return;
    }
    mvwprintw(sub, 1, (x - 30) / 2, "Идет процесс переименования...");
    wrefresh(sub);
    renamed_file_list = rename_and_get_renamed_list(sample, dir_path, opt);
    print_succes_message(renamed_file_list, sub);
    process_with_renamed_list(sub, renamed_file_list, x, y);
    remove_current_window(sub);
}
