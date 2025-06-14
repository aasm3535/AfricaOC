#include "vga.h"
#include "../../sys/sys16.h"

// Указатель на видеопамять для режима 0x13
static unsigned char* video_memory = (unsigned char*)0xA0000;

void vga_set_mode(unsigned int mode) {
    __asm__ __volatile__("int $0x10" : : "a"(mode));
}

void vga_put_pixel(int x, int y, unsigned char color) {
    // Проверка границ, чтобы не выйти за пределы экрана
    if (x < 0 || x >= 320 || y < 0 || y >= 200) {
        return;
    }
    // В режиме 0x13 память линейная, поэтому адрес вычисляется просто
    video_memory[y * 320 + x] = color;
}

void vga_clear_screen(unsigned char color) {
    // Просто заполняем всю видеопамять одним цветом
    for (int i = 0; i < 320 * 200; i++) {
        video_memory[i] = color;
    }
}
