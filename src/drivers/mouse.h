#ifndef MOUSE_H
#define MOUSE_H

// Структура для хранения состояния мыши
typedef struct {
    int x;
    int y;
    int left_button;
    int right_button;
    int middle_button;
} mouse_event_t;

/**
 * @brief Инициализирует драйвер мыши.
 * @return 1 если мышь найдена, 0 если нет.
 */
int mouse_init();

/**
 * @brief Показывает курсор мыши на экране.
 */
void mouse_show_cursor();

/**
 * @brief Скрывает курсор мыши.
 */
void mouse_hide_cursor();

/**
 * @brief Получает текущее состояние мыши (координаты и кнопки).
 * @param event Указатель на структуру, куда будут записаны данные.
 */
void mouse_get_event(mouse_event_t* event);

#endif // MOUSE_H
