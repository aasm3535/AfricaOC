// src/drivers/mouse_render.c

#include "mouse_render.h"
#include "../../shell/colors.h" // Для цветов

// --- Внутренние (приватные) данные модуля ---

// Указатель на видеопамять
static unsigned char* video_memory = (unsigned char*)0xB8000;

// Структура для хранения символа с экрана
typedef struct {
    char character;
    unsigned char attribute;
} screen_char_t;

// Статические переменные для хранения состояния
static screen_char_t saved_char;    // Символ, который был под курсором
static char g_cursor_char;          // Символ нашего курсора
static unsigned char g_cursor_attr; // Цвет нашего курсора

// --- Внутренние (приватные) функции ---

// Статическая функция для получения символа с экрана
static screen_char_t get_char_at(int x, int y) {
    screen_char_t sc;
    int offset = (y * 80 + x) * 2;
    sc.character = video_memory[offset];
    sc.attribute = video_memory[offset + 1];
    return sc;
}

// Статическая функция для отрисовки символа на экране
static void put_char_at(int x, int y, char c, unsigned char attr) {
    int offset = (y * 80 + x) * 2;
    video_memory[offset] = c;
    video_memory[offset + 1] = attr;
}

// --- Публичные функции ---

void mouse_render_init(char cursor_char, unsigned char cursor_attr) {
    g_cursor_char = cursor_char;
    g_cursor_attr = cursor_attr;
    saved_char.character = ' ';
    saved_char.attribute = COLOR_BLACK;
}

void mouse_render_update(int old_x, int old_y, int new_x, int new_y) {
    // 1. Восстанавливаем символ в старой позиции курсора, если она была
    if (old_x != -1 && old_y != -1) {
        put_char_at(old_x, old_y, saved_char.character, saved_char.attribute);
    }

    // 2. Сохраняем символ из новой позиции
    saved_char = get_char_at(new_x, new_y);

    // 3. Рисуем наш курсор в новой позиции
    // Сохраняем цвет фона оригинального символа
    unsigned char original_bg_color = saved_char.attribute & 0xF0;
    put_char_at(new_x, new_y, g_cursor_char, g_cursor_attr | original_bg_color);
}
