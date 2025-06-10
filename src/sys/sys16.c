// src/sys/sys16.c

void print_char(char c); // Объявляем, чтобы можно было использовать

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

// Новая функция для вывода 16-битного числа в 16-ричном формате
void print_hex(unsigned int n) {
    char* hex_chars = "0123456789ABCDEF";
    for (int i = 12; i >= 0; i -= 4) {
        print_char(hex_chars[(n >> i) & 0xF]);
    }
}

// Новая функция для рекурсивного вывода 16-битного числа в 10-ричном формате
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


void initialize_syscalls() {
    // Заглушка
}
