// src/kernel.c

#include "sys/sys16.h"
#include "mem/memory.h"
#include "shell/shell.h"
#include "vfs/vfs.h" // <<<--- ДОБАВИТЬ
#include "shell/colors.h"
#include "drivers/mouse.h"

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
 * @brief Демонстрационный цикл для мыши.
 */
void mouse_demo_loop() {
    mouse_event_t event;
    int old_x = -1, old_y = -1, old_lb = 0, old_rb = 0;

    while (1) {
        mouse_get_event(&event);

        // Перерисовываем информацию только если что-то изменилось
        if (event.x != old_x || event.y != old_y || event.left_button != old_lb || event.right_button != old_rb) {

            // Прячем курсор мыши, чтобы избежать "грязи" при печати
            mouse_hide_cursor();

            // Сохраняем текущую позицию текстового курсора и перемещаем в угол
            unsigned short dx_val;
            __asm__ __volatile__ ( "int $0x10" : "=d"(dx_val) : "a"(0x0300), "b"(0) : "cx" );
            move_cursor(0, 0);

            // Печатаем информацию
            print_string("Mouse Demo: X=");
            print_dec(event.x);
            print_string(" Y=");
            print_dec(event.y);
            print_string(" LBtn=");
            print_dec(event.left_button);
            print_string(" RBtn=");
            print_dec(event.right_button);
            print_string("    "); // Затираем старые символы

            // Возвращаем текстовый курсор на место
            move_cursor(dx_val & 0xFF, dx_val >> 8);

            // Показываем курсор мыши снова
            mouse_show_cursor();

            old_x = event.x;
            old_y = event.y;
            old_lb = event.left_button;
            old_rb = event.right_button;
        }
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
     mouse_demo_loop();
 }
