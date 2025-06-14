#include "mouse.h"
#include "../../sys/sys16.h" // Нужен для print_string и других

// Переменная для хранения информации о наличии мыши
static int mouse_present = 0;

int mouse_init() {
    unsigned short mouse_exists;
    // AX = 0x00: Инициализация мыши
    __asm__ __volatile__("mov $0x00, %%ax; int $0x33" : "=a"(mouse_exists) : : "bx", "cx", "dx");

    // Если AX = 0xFFFF, мышь найдена
    if (mouse_exists == 0xFFFF) {
        mouse_present = 1;
        return 1;
    }
    return 0;
}

void mouse_show_cursor() {
    if (!mouse_present) return;
    // AX = 0x01: Показать курсор
    __asm__ __volatile__("mov $0x01, %%ax; int $0x33" : : : "ax");
}

void mouse_hide_cursor() {
    if (!mouse_present) return;
    // AX = 0x02: Скрыть курсор
    __asm__ __volatile__("mov $0x02, %%ax; int $0x33" : : : "ax");
}


void mouse_get_event(mouse_event_t* event) {
    if (!mouse_present) return;

    unsigned short buttons, x_pixels, y_pixels;
    // AX = 0x03: Получить состояние мыши
    __asm__ __volatile__("mov $0x03, %%ax; int $0x33" : "=b"(buttons), "=c"(x_pixels), "=d"(y_pixels) : : "ax");

    // <<<--- ИЗМЕНЕНИЕ ---
    // Больше не делим на 8, нам нужны пиксельные координаты
    event->x = x_pixels;
    event->y = y_pixels;
    // --- КОНЕЦ ИЗМЕНЕНИЯ ---

    // Распаковываем состояние кнопок из регистра BX
    event->left_button = (buttons & 0x01) ? 1 : 0;
    event->right_button = (buttons & 0x02) ? 1 : 0;
    event->middle_button = (buttons & 0x04) ? 1 : 0;
}
