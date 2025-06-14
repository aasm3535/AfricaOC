#include "sys16.h"

void print_char(char c) {
    if (c == '\n') {
        __asm__ __volatile__ ( "int $0x10" : : "a" ((0x0E << 8) | '\r'), "b" (0x0000) : "memory" );
    }
    __asm__ __volatile__ ( "int $0x10" : : "a" ((0x0E << 8) | c), "b" (0x0000) : "memory" );
}

/**
 * @brief Очищает весь экран, используя заданный цветовой атрибут.
 * @param attribute Байт атрибута (фон << 4 | текст).
 */
void clear_screen_with_color(unsigned char attribute) {
    __asm__ __volatile__ (
        "int $0x10"
        :
        : "a"(0x0600),       // AH=0x06 (scroll up), AL=0x00 (clear entire window)
          "b"(attribute),    // BH = атрибут цвета
          "c"(0x0000),       // CH=0 (top row), CL=0 (left col)
          "d"(0x184F)        // DH=24 (bottom row), DL=79 (right col)
        : "memory"
    );
    // После очистки ставим курсор в начало
    move_cursor(0, 0);
}

void print_char_color(char c, unsigned char color) {
    if (c == '\n' || c == '\r') {
        print_char('\n');
    } else {
        __asm__ __volatile__ ( "int $0x10" : : "a"((0x0900) | c), "b"(color), "c"(1) : "memory");

        unsigned short dx_val;
        __asm__ __volatile__ ( "int $0x10" : "=d"(dx_val) : "a"(0x0300), "b"(0) : "cx" );
        move_cursor((dx_val & 0xFF) + 1, dx_val >> 8);
    }
}

void print_string(const char* str) {
    while (*str) {
        print_char(*str++);
    }
}

void move_cursor(int x, int y) {
    __asm__ __volatile__ ( "int $0x10" : : "a"(0x0200), "b"(0x0000), "d"((y << 8) | x) : "memory" );
}

void clear_screen() {
    __asm__ __volatile__("int $0x10" : : "a"(0x0003) : "memory");
}

char read_char() {
    char key;
    __asm__ __volatile__ ( "int $0x16" : "=a"(key) : "a"(0x0000) );
    return key;
}

keypress_t read_keypress() {
    keypress_t key;
    __asm__ __volatile__ ( "int $0x16" : "=a"(key) : "a"(0x0000) );
    return key;
}

void read_line(char* buffer, int max_len) {
    int i = 0;
    while (i < max_len - 1) {
        char c = read_char();
        if (c == '\r') { break; }
        else if (c == '\b') {
            if (i > 0) { i--; print_char('\b'); print_char(' '); print_char('\b'); }
        } else {
            buffer[i++] = c;
            print_char(c);
        }
    }
    buffer[i] = '\0';
    print_string("\n");
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strlen(const char* s) {
    int len = 0;
    while(s[len]) {
        len++;
    }
    return len;
}

void strcpy_s(char* dest, const char* src, int dest_size) {
    int i = 0;
    while (src[i] && i < dest_size - 1) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void strcat(char* dest, const char* src) {
    strcpy_s(dest + strlen(dest), src, 255);
}

void* memcpy(void* dest, const void* src, size_t n) {
    char* d = dest;
    const char* s = src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

void* memmove(void* dest, const void* src, size_t n) {
    char* d = dest;
    const char* s = src;
    if (d < s) {
        for (size_t i = 0; i < n; i++) {
            d[i] = s[i];
        }
    } else {
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
