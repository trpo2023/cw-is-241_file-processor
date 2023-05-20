#include <interface/start.h>

#include <libfileproc/clean_memory.h>

#include <interface/get_item.h>
#include <interface/get_pattern.h>
#include <interface/info.h>
#include <interface/print.h>
#include <interface/renamed_list.h>
#include <interface/select_dir.h>
#include <interface/select_option.h>

void check_term_size(int x, int y)
{
    if (x < 120 || y < 30) {
        delwin(stdscr);
        endwin();
        printf("\e[1;31mОшибка\e[0m: текущий размер терминала %dx%d\n", x, y);
        printf("Минимальный размер: 120x30 (x, y)\n");
        exit(EXIT_FAILURE);
    }
}

void start(WINDOW* menu)
{
    const char* menu_items[] = {
            "1) Ввести шаблоны",
            "2) Выбрать каталог",
            "3) Список файлов в текущем каталоге",
            "4) Запустить массовое переименование",
            "5) Опции",
            "6) Информация",
            "7) Выход из приложения (F10)",
    };
    GList* input_strings = NULL;
    GList* patterns = NULL;
    Option option = {0};
    char current_dir[MAX_LEN] = ".";
    int y = getmaxy(menu);
    int x = getmaxx(menu);
    int i = INPUT_PATTERN;

    while (i != KEY_F(10)
           && (i = select_items(menu, menu_items, i, 1, 6)) != EXIT) {
        wrefresh(menu);
        switch (i) {
        case INPUT_PATTERN:
            patterns = pattern_input(menu, &input_strings, patterns);
            break;
        case SELECT_DIR:
            if (!select_dir(menu, current_dir))
                i = KEY_F(10);
            mvwprintw(menu, y - 2, 2, "Выбранный каталог:");
            print_str(menu, current_dir, y - 2, (x / 2) - 21, 1, 0, 0, 19);
            break;
        case FILES_LIST:
            if (!print_list_in_current_dir(menu, current_dir))
                i = KEY_F(10);
            break;
        case PROCESS:
            if (!process(menu, patterns, current_dir, &option))
                i = KEY_F(10);
            clean_data(&option, &input_strings, &patterns);
            break;
        case SELECT_OPT:
            if (!select_option(
                        menu, &option, &input_strings, &patterns, current_dir))
                i = KEY_F(10);
            break;
        case INFO:
            if (!show_info(menu))
                i = KEY_F(10);
            break;
        }
        mvwprintw_highlite(menu, i + 1, 2, menu_items[i]);
    }

    clean_data(&option, &input_strings, &patterns);
}