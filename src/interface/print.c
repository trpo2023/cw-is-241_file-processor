#include <interface/print.h>

#include <libfileproc/directory.h>
#include <libfileproc/lexer.h>
#include <libfileproc/rename.h>

#include <interface/get_item.h>
#include <interface/init.h>

void print_counter(WINDOW* sub, int x, int cnt, int max_cnt)
{
    char counter[MAX_LEN] = {0};
    sprintf(counter, "%d/%d", cnt, max_cnt);
    mvwprintw(sub, 1, x - (strlen(counter) + 2), "%s", counter);
}

void mvwprintw_highlite(WINDOW* win, int y, int x, const char* str)
{
    wattron(win, A_STANDOUT);
    mvwprintw(win, y, x, "%s", str);
    wattroff(win, A_STANDOUT);
}

void print_str(
        WINDOW* sub,
        char* str,
        int i,
        int x,
        int small,
        int dx,
        int dy,
        int x_offset)
{
    size_t str_len = strlen(str);
    char dest[MAX_LEN] = {0};
    if (str_len >= x - dx && small) {
        make_str_smaller(str, dest, str_len, x - dx);
    } else {
        strcpy(dest, str);
    }

    mvwprintw(sub, i + dy, 2 + x_offset, "%-*s", x - dx, dest);
}

void print_str_highlite(
        WINDOW* sub,
        char* str,
        int i,
        int x,
        int small,
        int x_offset,
        int y_offset)
{
    size_t str_len = strlen(str);
    char dest[MAX_LEN] = {0};
    if (str_len >= x - x_offset && small) {
        make_str_smaller(str, dest, str_len, x - x_offset);
    } else {
        strcpy(dest, str);
    }

    mvwprintw_highlite(sub, i + y_offset, 2, dest);
}

void print_items(
        WINDOW* sub, GList* list, int* str_cnt, int small, int a, int b)
{
    int y = getmaxy(sub) - a;
    int x = getmaxx(sub) - a;
    int cnt = 0;
    for (GList* i = list; i != NULL && cnt < y; i = i->next) {
        size_t str_len = strlen((char*)i->data);
        char dest[MAX_LEN] = {0};
        if (str_len >= x && small) {
            make_str_smaller(i->data, dest, str_len, x);
        } else {
            strcpy(dest, i->data);
        }

        mvwprintw(sub, cnt + b, 2, "%-*s", x, dest);
        cnt++;
    }
    *str_cnt = cnt;

    x = getmaxx(sub);
    while (cnt < y) {
        mvwprintw(sub, cnt + b, 2, "%-*s", x - 3, " ");
        cnt++;
    }

    wrefresh(sub);
}

void print_new_page(
        WINDOW* sub,
        GList* list,
        int* cnt,
        int p,
        int max_p,
        int small,
        int a,
        int b)
{
    int x = getmaxx(sub);
    print_items(sub, list, cnt, small, a, b);
    print_counter(sub, x, p + 1, max_p + 1);
}

int print_input_strings(WINDOW* sub, GList* input_strings)
{
    int cnt = 0;
    for (GList* i = input_strings; i != NULL; i = i->next) {
        mvwprintw(sub, 2 + cnt, 2, "%s", (char*)i->data);
        cnt++;
    }

    return cnt;
}

int print_info(WINDOW* sub, GList* str_list, int y)
{
    size_t size = g_list_length(str_list);
    int cnt = 0;
    print_items(sub, str_list, &cnt, 0, 3, 2);
    mvwprintw_highlite(sub, 2, 2, (char*)str_list->data);
    if (!get_item(sub, str_list, y - 5, size, cnt, 0, 3, 2))
        return 0;
    return 1;
}

int print_list_in_current_dir(WINDOW* menu, char* path)
{
    int y, x;
    getmaxyx(menu, y, x);
    WINDOW* sub = init_sub_window(menu, y, x);
    getmaxyx(sub, y, x);

    mvwprintw(sub, 1, 1, "Текущая директория: ");
    print_str(sub, get_name(path), 1, (x / 2), 1, 0, 0, 19);
    wrefresh(sub);

    GList* files = get_files_or_dirs_list(path, FILES);
    if (g_list_length(files) == 0) {
        mvwprintw(
                sub, 3, (x - 37) / 2, "Файлов в данной директории не найдено");
        wrefresh(sub);
        g_list_free(files);
        return 1;
    }
    GList* names = get_files_names(files);
    names = g_list_sort(names, my_comparator);

    size_t names_len = g_list_length(names);

    int names_cnt = 0;
    print_items(sub, names, &names_cnt, 1, 3, 2);
    mvwprintw_highlite(sub, 2, 2, (char*)names->data);

    char* stat = get_item(sub, names, y - 5, names_len, names_cnt, 1, 3, 2);
    if (!stat)
        return 0;

    remove_current_window(sub);

    g_list_free(names);
    g_list_free_full(files, free);

    return 1;
}
