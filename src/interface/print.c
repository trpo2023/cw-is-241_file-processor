#include <interface/print.h>

#include <interface/get_item.h>
#include <libfileproc/lexer.h>

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

void print_items(WINDOW* sub, GList* list, int* str_cnt, int a, int b)
{
    int y = getmaxy(sub) - a;
    int x = getmaxx(sub) - a;
    int cnt = 0;
    for (GList* i = list; i != NULL && cnt < y; i = i->next) {
        mvwprintw(sub, cnt + b, 2, "%-*s", x, (char*)i->data);
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
        WINDOW* sub, GList* list, int* cnt, int p, int max_p, int a, int b)
{
    int x = getmaxx(sub);
    print_items(sub, list, cnt, a, b);
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

void print_info(WINDOW* sub, GList* str_list, int y)
{
    size_t size = g_list_length(str_list);
    int cnt = 0;
    print_items(sub, str_list, &cnt, 3, 2);
    mvwprintw_highlite(sub, 2, 2, (char*)str_list->data);
    get_item(sub, str_list, y - 5, size, cnt, 3, 2);
}
