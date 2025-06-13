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

void move_cursor(int x, int y) {
    // int 0x10, ah=0x02 - установить позицию курсора
    // bh = номер страницы (0), dh = строка, dl = столбец
    __asm__ __volatile__ ( "int $0x10" : : "a"(0x0200), "b"(0x0000), "d"((y << 8) | x) );
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

void strcpy_s(char* dest, const char* src, int dest_size) {
    int i = 0;
    // Копируем, пока не кончится исходная строка или место в буфере
    while (src[i] && i < dest_size - 1) {
        dest[i] = src[i];
        i++;
    }
    // Завершаем строку нулевым символом
    dest[i] = '\0';
}

int strlen(const char* s) {
    int len = 0;
    while(s[len]) {
        len++;
    }
    return len;
}

void strcat(char* dest, const char* src) {
    strcpy_s(dest + strlen(dest), src, 255); // Используем существующую безопасную копию
}

void* memcpy(void* dest, const void* src, size_t n) {
    char* d = dest;
    const char* s = src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

keypress_t read_keypress() {
    keypress_t key;
    // int 0x16, ah=0x00 или 0x10 - ждать нажатия клавиши
    // ah=0x00 возвращает в ax, ah=0x10 возвращает расширенные коды
    __asm__ __volatile__ ( "int $0x16" : "=a"(key) : "a"(0x0000) );
    return key;
}

void* memmove(void* dest, const void* src, size_t n) {
    char* d = dest;
    const char* s = src;
    if (d < s) {
        // Копируем с начала
        for (size_t i = 0; i < n; i++) {
            d[i] = s[i];
        }
    } else {
        // Копируем с конца, чтобы избежать затирания
        for (size_t i = n; i != 0; i--) {
            d[i-1] = s[i-1];
        }
    }
    return dest;
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
