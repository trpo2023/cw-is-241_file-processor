#include <glib.h>

#include <libfileproc/directory.h>

#include <interface/get_item.h>
#include <interface/init.h>
#include <interface/print.h>
#include <interface/select_dir.h>

char* select_dir(WINDOW* menu, char* current_dir)
{
    int y, x;
    getmaxyx(menu, y, x);
    WINDOW* sub = init_sub_window(menu, y, x);
    mvwprintw(sub, 1, 1, "Выберите каталог:");
    char* dir = NULL;

    GList* dir_list = get_files_or_dirs_list(".", DIRS);
    dir_list = g_list_sort(dir_list, my_comparator);
    size_t dir_len = g_list_length(dir_list);

    int dir_cnt = 0; // count of directories on one page
    print_items(sub, dir_list, &dir_cnt, 3, 2);
    mvwprintw_highlite(sub, 2, 2, (char*)dir_list->data);

    dir = get_item(sub, dir_list, y - 5, dir_len, dir_cnt, 3, 2);
    strcpy(current_dir, dir);

    remove_current_window(sub);
    g_list_free_full(dir_list, free);

    return current_dir;
}