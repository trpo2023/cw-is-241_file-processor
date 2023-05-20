#include <interface/get_item.h>
#include <interface/print.h>

#include <libfileproc/rename.h>

int select_items(WINDOW* win, const char* items[], int i, int offset, int max)
{
    int y, x;
    getmaxyx(win, y, x);
    int ch;
    while ((ch = wgetch(win)) != KEY_F(10)) {
        mvwprintw(win, i + offset, 2, "%s", items[i]);
        switch (ch) {
        case KEY_UP:
            i--;
            i = (i < 0) ? max : i;
            break;
        case KEY_DOWN:
            i++;
            i = (i > max) ? 0 : i;
            break;
        case 10: // KEY_ENTER
            return i;
        }
        mvwprintw_highlite(win, i + offset, 2, items[i]);
        if (max == 4) // menu options
        {
            mvwprintw(win, y - 3, 2, "%-*s", x / 2, items[i]);
        }
    }

    return KEY_F(10);
}

void print_str(
        WINDOW* sub, char* str, int i, int x, int dx, int dy, int x_offset)
{
    size_t str_len = strlen(str);
    char dest[MAX_LEN] = {0};
    if (str_len >= x - dx) {
        make_str_smaller(str, dest, str_len, x - dx);
    } else {
        strcpy(dest, str);
    }

    mvwprintw(sub, i + dy, 2 + x_offset, "%-*s", x - dx, dest);
}

void print_str_highlite(
        WINDOW* sub, char* str, int i, int x, int x_offset, int y_offset)
{
    size_t str_len = strlen(str);
    char dest[MAX_LEN] = {0};
    if (str_len >= x - x_offset) {
        make_str_smaller(str, dest, str_len, x - x_offset);
    } else {
        strcpy(dest, str);
    }

    mvwprintw_highlite(sub, i + y_offset, 2, dest);
}

char* get_item(
        WINDOW* sub,
        GList* dir_list,
        int y,
        size_t len,
        int str_cnt,
        int a,
        int b)
{
    int ch, i = 0, page = 0, x = getmaxx(sub);
    size_t max_pages = len / y;
    GList* pages[max_pages];

    // page counter
    print_counter(sub, x, page + 1, max_pages + 1);

    while ((ch = wgetch(sub)) != 10) { // 10 - KEY_ENTER
        print_str(sub, (char*)dir_list->data, i, x, a, b, 0);
        if (i == 0)
            pages[page] = dir_list;

        switch (ch) {
        case KEY_F(10):
            return NULL;
        case KEY_UP:
            i--;
            if (i < 0 && page >= 1) {
                page--;
                print_new_page(
                        sub, pages[page], &str_cnt, page, max_pages, a, b);
                i = y - 1;
            } else if (i < 0)
                i++;
            else
                dir_list = dir_list->prev;
            break;
        case KEY_DOWN:
            i++;
            if (i >= str_cnt && page == max_pages)
                i--;
            else if (i >= y) {
                page++;
                print_new_page(sub, dir_list, &str_cnt, page, max_pages, a, b);
                i = 0;
            } else
                dir_list = dir_list->next;
        }
        print_str_highlite(sub, (char*)dir_list->data, i, x, a, b);
        wrefresh(sub);
    }

    return (char*)dir_list->data;
}