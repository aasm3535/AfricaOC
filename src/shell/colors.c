#include "colors.h"
#include "../sys/sys16.h"

// Набор цветов для радуги
static const unsigned char rainbow_colors[] = {
    COLOR_LIGHT_RED,
    COLOR_YELLOW,
    COLOR_LIGHT_GREEN,
    COLOR_LIGHT_CYAN,
    COLOR_LIGHT_BLUE,
    COLOR_LIGHT_MAGENTA
};
static const int num_rainbow_colors = sizeof(rainbow_colors) / sizeof(unsigned char);

void print_string_rainbow(const char* str) {
    int i = 0;
    while (*str) {
        // Выбираем цвет по кругу
        unsigned char color = rainbow_colors[i % num_rainbow_colors];
        print_char_color(*str, color); // Используем новую функцию
        str++;
        i++;
    }
}

void print_string_color(const char* str, unsigned char color) {
    while (*str) {
        print_char_color(*str, color);
        str++;
    }
}
