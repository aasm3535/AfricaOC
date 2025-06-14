#ifndef COLORS_H
#define COLORS_H

// Определения цветов для текстового режима CGA
typedef enum {
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GREY = 7,
    COLOR_DARK_GREY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_YELLOW = 14,
    COLOR_WHITE = 15,
} text_color_t;

#define DEFAULT_SCREEN_ATTRIBUTE ((COLOR_DARK_GREY << 4) | COLOR_LIGHT_GREY)

/**
 * @brief Печатает строку, где каждая буква имеет свой цвет (радуга).
 * @param str Строка для вывода.
 */
void print_string_rainbow(const char* str);

/**
 * @brief Печатает строку указанным цветом.
 * @param str Строка для вывода.
 * @param color Цвет из перечисления text_color_t.
 */
void print_string_color(const char* str, unsigned char color);

#endif // COLORS_H
