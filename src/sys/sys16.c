#include "sys16.h"

// --- Функции вывода ---

void print_char(char c) {
    if (c == '\n') {
        __asm__ __volatile__ ( "int $0x10" : : "a" ((0x0E << 8) | '\r'), "b" (0x0000) );
    }
    __asm__ __volatile__ ( "int $0x10" : : "a" ((0x0E << 8) | c), "b" (0x0000) );
}

void print_string(const char* str) {
    // ИСПРАВЛЕНИЕ: Добавлен инкремент указателя str++
    while (*str) {
        print_char(*str++);
    }
}

void clear_screen() {
    __asm__ __volatile__("int $0x10" : : "a"(0x0003));
}

// --- Функции ввода ---

char read_char() {
    char key;
    __asm__ __volatile__ ( "int $0x16" : "=a"(key) : "a"(0x0000) );
    return key;
}

void read_line(char* buffer, int max_len) {
    int i = 0;
    while (i < max_len - 1) {
        char c = read_char();
        if (c == '\r') { break; }
        else if (c == '\b') { // Backspace
            if (i > 0) { i--; print_char('\b'); print_char(' '); print_char('\b'); }
        } else {
            buffer[i++] = c;
            print_char(c);
        }
    }
    buffer[i] = '\0';
    print_string("\n");
}

// --- Вспомогательные функции ---

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void print_hex(unsigned int n) {
    char* hex_chars = "0123456789ABCDEF";
    for (int i = 12; i >= 0; i -= 4) {
        print_char(hex_chars[(n >> i) & 0xF]);
    }
}

void print_dec_recursive(unsigned int n) {
    if (n == 0) return;
    print_dec_recursive(n / 10);
    print_char((n % 10) + '0');
}

void print_dec(unsigned int n) {
    if (n == 0) {
        print_char('0');
    } else {
        print_dec_recursive(n);
    }
}
