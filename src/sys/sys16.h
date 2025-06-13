#ifndef SYS16_H
#define SYS16_H

// Определяем тип size_t, если он еще не определен
#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

// <<<--- ДОБАВИТЬ СТРУКТУРУ ---
// Структура для хранения кода клавиши
typedef struct {
    unsigned char scancode;
    unsigned char ascii;
} keypress_t;

// --- Функции для работы с экраном ---
void clear_screen();
void print_char(char c);
void print_char_color(char c, unsigned char color);
void print_string(const char* str);
void move_cursor(int x, int y); // <<<--- ДОБАВИТЬ

// --- Функции для ввода с клавиатуры ---
char read_char();
keypress_t read_keypress(); // <<<--- ДОБАВИТЬ
void read_line(char* buffer, int max_len);

// --- Вспомогательные функции ---
int strcmp(const char* s1, const char* s2);
int strlen(const char* s); // <<<--- ДОБАВИТЬ
void strcpy_s(char* dest, const char* src, int dest_size);
void strcat(char* dest, const char* src); // <<<--- ДОБАВИТЬ
void* memcpy(void* dest, const void* src, size_t n); // <<<--- ДОБАВИТЬ
void* memmove(void* dest, const void* src, size_t n); // <<<--- ДОБАВИТЬ
void print_hex(unsigned int n);
void print_dec(unsigned int n);

#endif // SYS16_H
