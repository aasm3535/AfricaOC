// src/kernel.c

// Объявления функций из других файлов, чтобы компилятор их "видел"
void print_char(char c);
void print_string(const char* str);
int strcmp(const char* s1, const char* s2); // Из sys16.c
void initialize_syscalls();                 // Из sys16.c

// --- Функции для ввода с клавиатуры ---

/**
 * Читает один символ с клавиатуры с помощью прерывания BIOS.
 * @return ASCII-код нажатой клавиши.
 */
char read_char() {
    char key;
    // Используем прерывание BIOS 0x16 для ожидания нажатия клавиши
    // ah = 0x00 - ждать и считать клавишу
    __asm__ __volatile__ (
        "int $0x16"
        : "=a"(key)  // Выход: ASCII-код в al
        : "a"(0x0000)  // Вход: ah = 0
    );
    return key;
}

/**
 * Читает строку с клавиатуры и сохраняет ее в буфер.
 * Поддерживает Backspace для редактирования.
 * @param buffer Указатель на буфер для сохранения строки.
 * @param max_len Максимальный размер буфера.
 */
void read_line(char* buffer, int max_len) {
    int i = 0;
    while (i < max_len - 1) {
        char c = read_char();

        if (c == '\r') { // Нажат Enter (возврат каретки)
            break; // Завершаем ввод
        } else if (c == '\b') { // Нажат Backspace
            if (i > 0) {
                i--;
                print_char('\b'); // Сдвинуть курсор назад
                print_char(' ');  // Затереть символ на экране
                print_char('\b'); // Снова сдвинуть курсор
            }
        } else {
            buffer[i] = c;
            i++;
            print_char(c); // Отображаем введенный символ на экране
        }
    }
    buffer[i] = '\0'; // Завершаем строку нулевым символом
    print_string("\n"); // Переходим на новую строку после ввода
}

// --- Простой командный интерпретатор (shell) ---

/**
 * Главный цикл командного интерпретатора.
 * Читает команду и выполняет ее.
 */
void shell_loop() {
    char input_buffer[100]; // Буфер для команд пользователя

    while (1) {
        print_string("> "); // Приглашение к вводу
        read_line(input_buffer, sizeof(input_buffer));

        if (strcmp(input_buffer, "ping") == 0) {
            print_string("pong\n");
        } else if (strcmp(input_buffer, "help") == 0) {
            print_string("Available commands:\n");
            print_string("  ping : Responds with 'pong'.\n");
            print_string("  help : Shows this help message.\n");
        } else if (input_buffer[0] == '\0') {
            // Ничего не делать, если введена пустая строка
        } else {
            print_string("Unknown command: '");
            print_string(input_buffer);
            print_string("'\nType 'help' for a list of commands.\n");
        }
    }
}


// --- Главная функция ядра ---

void kmain() {
    print_string("Hello from the 16-bit C Kernel!\n");
    print_string("AfricaOS is now interactive. Zzz...\n\n");

    // Инициализируем наши "системные вызовы" (пока что заглушка)
    initialize_syscalls();

    // Запускаем бесконечный цикл командного интерпретатора
    shell_loop();
}


// --- Функции вывода (улучшенная версия) ---

void print_char(char c) {
    // В режиме телетайпа BIOS для новой строки '\n' (line feed)
    // нужно также послать возврат каретки '\r' (carriage return).
    if (c == '\n') {
        __asm__ __volatile__ ( "int $0x10" : : "a" ((0x0E << 8) | '\r'), "b" (0x0000) );
    }
    __asm__ __volatile__ ( "int $0x10" : : "a" ((0x0E << 8) | c), "b" (0x0000) );
}

void print_string(const char* str) {
    while (*str) {
        print_char(*str);
        str++;
    }
}
