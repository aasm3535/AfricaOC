#ifndef VGA_H
#define VGA_H

/**
 * @brief Устанавливает графический режим VGA.
 * @param mode Номер режима (например, 0x13 для 320x200x256).
 */
void vga_set_mode(unsigned int mode);

/**
 * @brief Рисует один пиксель на экране.
 * @param x Координата X.
 * @param y Координата Y.
 * @param color Цвет пикселя (0-255).
 */
void vga_put_pixel(int x, int y, unsigned char color);

/**
 * @brief Очищает экран, заливая его одним цветом.
 * @param color Цвет для заливки.
 */
void vga_clear_screen(unsigned char color);

#endif // VGA_H
