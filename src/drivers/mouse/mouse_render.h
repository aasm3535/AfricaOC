// src/drivers/mouse_render.h

#ifndef MOUSE_RENDER_H
#define MOUSE_RENDER_H

/**
 * @brief Инициализирует рендерер курсора.
 * @param cursor_char Символ для курсора.
 * @param cursor_attr Атрибут (цвет) для курсора.
 */
void mouse_render_init(char cursor_char, unsigned char cursor_attr);

/**
 * @brief Обновляет позицию курсора на экране.
 * Стирает курсор в старой позиции и рисует в новой.
 * @param old_x Старая координата X.
 * @param old_y Старая координата Y.
 * @param new_x Новая координата X.
 * @param new_y Новая координата Y.
 */
void mouse_render_update(int old_x, int old_y, int new_x, int new_y);

#endif // MOUSE_RENDER_H
