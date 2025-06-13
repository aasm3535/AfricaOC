// src/kernel.c

#include "sys/sys16.h"
#include "mem/memory.h"
#include "shell/shell.h"
#include "vfs/vfs.h" // <<<--- ДОБАВИТЬ
#include "shell/colors.h"

// Объявление символа 'end' из linker.ld.
extern unsigned int end;

/**
 * @brief Главный цикл командного интерпретатора.
 */
void shell_loop() {
    char input_buffer[100];
    while (1) {
        // --- ИЗМЕНЕНИЕ ПРИГЛАШЕНИЯ КОМАНДНОЙ СТРОКИ ---
        print_string("africa:");
        print_string(get_current_dir_name()); // Отображаем текущую папку
        print_string("> ");
        // --- КОНЕЦ ИЗМЕНЕНИЯ ---

        read_line(input_buffer, sizeof(input_buffer));
        shell_execute(input_buffer);
    }
}

/**
 * @brief Главная функция ядра.
 */
 void kmain() {
     // 1. Инициализируем системные модули
     void* heap_start = (void*)&end;
     unsigned int heap_size = 0x9E000 - (unsigned int)heap_start;

     clear_screen();

     memory_init(heap_start, heap_size);
     vfs_init();
     shell_init();

     // 2. Выводим приветствие
     // --- ИЗМЕНЯЕМ ПРИВЕТСТВИЕ НА ЦВЕТНОЕ ---
     print_string_color("Welcome to ", COLOR_LIGHT_GREY);
     print_string_rainbow("AfricaOS");
     print_string_color("! Kernel Initialized.\n", COLOR_LIGHT_GREY);
     // --- КОНЕЦ ИЗМЕНЕНИЯ ---

     print_string("Heap starts at 0x");
     print_hex((unsigned int)heap_start);
     print_string("\n\n");

     // 3. Запускаем главный цикл
     shell_loop();
 }
