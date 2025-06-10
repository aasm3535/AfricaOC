// src/kernel.c

#include "mem/memory.h"

// Объявления функций из других файлов
void print_char(char c);
void print_string(const char* str);
int strcmp(const char* s1, const char* s2);
void initialize_syscalls();
void print_hex(unsigned int n);

// Объявление символа 'end' из linker.ld.
// '&end' будет адресом начала нашей кучи.
extern unsigned int end;

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
     char input_buffer[100];

     while (1) {
         print_string("> ");
         read_line(input_buffer, sizeof(input_buffer));

         if (strcmp(input_buffer, "ping") == 0) {
             print_string("pong\n");
         } else if (strcmp(input_buffer, "help") == 0) {
             print_string("Available commands:\n");
             print_string("  ping   : Responds with 'pong'.\n");
             print_string("  help   : Shows this help message.\n");
             print_string("  memmap : Displays the memory map.\n");
             print_string("  memtest: Runs a simple memory allocation test.\n");
         } else if (strcmp(input_buffer, "memmap") == 0) {
             print_memory_map();
         } else if (strcmp(input_buffer, "memtest") == 0) {
             print_string("Running memory test...\n");

             print_string("Allocating 32 bytes... ");
             char* block1 = (char*)malloc(32);
             if (block1) {
                 print_string("OK. Addr: 0x"); print_hex((unsigned int)block1); print_string("\n");
             } else {
                 print_string("FAILED.\n");
             }

             print_string("Allocating 128 bytes... ");
             char* block2 = (char*)malloc(128);
             if (block2) {
                 print_string("OK. Addr: 0x"); print_hex((unsigned int)block2); print_string("\n");
             } else {
                 print_string("FAILED.\n");
             }

             print_string("Current memory map:\n");
             print_memory_map();

             print_string("Freeing first block (32 bytes)...\n");
             free(block1);
             print_memory_map();

             print_string("Freeing second block (128 bytes)...\n");
             free(block2);
             print_memory_map();

             print_string("Test finished.\n");

         } else if (input_buffer[0] == '\0') {
             // Ничего не делать
         } else {
             print_string("Unknown command: '");
             print_string(input_buffer);
             print_string("'\n");
         }
     }
 }


// --- Главная функция ядра ---

void kmain() {
    // Определяем начало и размер кучи.
    // Куча начинается после ядра (&end) и простирается до 632 КБ.
    // (Оставляем немного места в конце для безопасности).
    void* heap_start = (void*)&end;
    unsigned int heap_size = 0x9E000 - (unsigned int)heap_start;

    // Инициализируем менеджер памяти
    memory_init(heap_start, heap_size);

    print_string("AfricaOS Kernel with Memory Management\n");
    print_string("Heap starts at 0x");
    print_hex((unsigned int)heap_start);
    print_string("\n\n");

    // Запускаем shell
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
