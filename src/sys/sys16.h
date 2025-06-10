#ifndef SYS16_H
#define SYS16_H

// --- Функции для работы с экраном ---
void clear_screen();
void print_char(char c);
void print_string(const char* str);

// --- Функции для ввода с клавиатуры ---
char read_char();
void read_line(char* buffer, int max_len);

// --- Вспомогательные функции ---
int strcmp(const char* s1, const char* s2);
void print_hex(unsigned int n);
void print_dec(unsigned int n);

#endif // SYS16_H
