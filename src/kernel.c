#include "sys/sys16.h"
#include "mem/memory.h"
#include "shell/shell.h"

// Объявление символа 'end' из linker.ld.
extern unsigned int end;

/**
 * @brief Главный цикл командного интерпретатора.
 */
void shell_loop() {
    char input_buffer[100];
    while (1) {
        print_string("> ");
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
    memory_init(heap_start, heap_size);
    shell_init();

    // 2. Выводим приветствие
    clear_screen();
    print_string("AfricaOS Kernel Initialized. Welcome!\n");
    print_string("Heap starts at 0x");
    print_hex((unsigned int)heap_start);
    print_string("\n\n");

    // 3. Запускаем главный цикл
    shell_loop();
}
