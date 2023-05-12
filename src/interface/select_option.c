#include <libfileproc/clean_memory.h>

#include <interface/get_item.h>
#include <interface/init.h>
#include <interface/print.h>
#include <interface/select_option.h>

const char* options[] = {
        "Регистр", "Восстановить значения по умолчанию", "<-Вернуться в меню"};

void set_default_settings(
        WINDOW* menu, Option* opt, GList** inp_s, GList** samp, char* curr_dir)
{
    int y = getmaxy(menu);
    strcpy(curr_dir, ".");
    mvwprintw(menu, y - 2, 2, "Выбранный каталог: %-30s", curr_dir);
    clean_data(opt, inp_s, samp);
}

void print_opt(WINDOW* sub, Option* opt, int i)
{
    switch (i) {
    case 0: // REGISTER
        switch (opt->name_register) {
        case R_DEFAULT:
            mvwprintw(sub, i + 3, 15, "%-20s", "Стандартный");
            break;
        case R_LOW:
            mvwprintw(sub, i + 3, 15, "%-20s", "Нижний");
            break;
        case R_HIGH:
            mvwprintw(sub, i + 3, 15, "%-20s", "Верхний");
            break;
        }
    }
}

void select_option(
        WINDOW* menu, Option* opt, GList** inp_s, GList** samp, char* curr_dir)
{
    int options_cnt = 2;
    int y, x, i;
    getmaxyx(menu, y, x);
    WINDOW* sub = init_sub_window(menu, y, x);
    mvwprintw(sub, 1, 1, "Выберите опцию:");

    mvwprintw_highlite(sub, 3, 2, options[0]);
    print_opt(sub, opt, 0);

    for (i = 1; i <= options_cnt; i++) {
        mvwprintw(sub, i + 3, 2, "%s", options[i]);
    }
    i = 0;
    while ((i = select_items(sub, options, i, 3, 2)) != BACK
           && i != KEY_F(10)) {
        if (i == REGISTER) {
            opt->name_register++;
            opt->name_register %= 3;
            print_opt(sub, opt, i);
            wrefresh(sub);
        } else {
            set_default_settings(menu, opt, inp_s, samp, curr_dir);
            break;
        }
        mvwprintw_highlite(sub, 3, 2, options[i]);
    }

    remove_current_window(sub);
}
