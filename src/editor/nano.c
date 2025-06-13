#include "nano.h"
#include "../sys/sys16.h"
#include "../mem/memory.h"
#include "../vfs/vfs.h"

// --- Константы и определения ---

// Скан-коды клавиш, которые нам нужны
#define KEY_UP      0x48
#define KEY_DOWN    0x50
#define KEY_LEFT    0x4B
#define KEY_RIGHT   0x4D
#define KEY_CTRL_S  0x1F // Ctrl+S
#define KEY_CTRL_X  0x18 // Ctrl+X

// Размеры экрана
#define SCREEN_WIDTH  80
#define SCREEN_HEIGHT 25
#define TEXT_HEIGHT   (SCREEN_HEIGHT - 1) // Последняя строка для статуса

// Структуры для управления состоянием редактора
typedef struct {
    char* content;          // Буфер с текстом файла
    size_t size;            // Текущий размер контента
    size_t capacity;        // Выделенная емкость буфера
    int cursor_x, cursor_y; // Позиция курсора на экране (визуальная)
    int offset;             // Позиция курсора в буфере (индекс)
    int top_line;           // Номер строки, с которой начинается отрисовка
    int modified;           // Флаг: были ли изменения в файле
    const char* filename;   // Имя файла
} editor_state_t;

static editor_state_t editor;

// --- Внутренние функции редактора ---

void editor_draw_status_bar() {
    move_cursor(0, SCREEN_HEIGHT - 1);
    char status[80];
    strcpy_s(status, "Ctrl+S: Save | Ctrl+X: Exit", sizeof(status));
    if (editor.modified) {
        strcat(status, " *"); // Показываем, что есть несохраненные изменения
    }
    print_string(status);
    // Заполняем остаток строки пробелами для очистки
    for (int i = strlen(status); i < SCREEN_WIDTH; i++) {
        print_char(' ');
    }
}

void editor_draw_text() {
    int current_y = 0;
    int line_start_offset = 0;
    int lines_to_skip = editor.top_line;

    // Пропускаем строки до top_line
    while (lines_to_skip > 0 && line_start_offset < editor.size) {
        if (editor.content[line_start_offset] == '\n') {
            lines_to_skip--;
        }
        line_start_offset++;
    }

    // Отрисовываем видимую часть текста
    for (int i = line_start_offset; i < editor.size && current_y < TEXT_HEIGHT; i++) {
        if (editor.content[i] == '\n') {
            current_y++;
            move_cursor(0, current_y);
        } else {
            print_char(editor.content[i]);
        }
    }
}

// Пересчитывает экранные координаты курсора (x, y) по его смещению в буфере
void editor_update_cursor_coords() {
    editor.cursor_x = 0;
    editor.cursor_y = 0;
    int line_start_offset = 0;
    int lines_to_skip = editor.top_line;

    while (lines_to_skip > 0 && line_start_offset < editor.offset) {
        if (editor.content[line_start_offset] == '\n') {
            lines_to_skip--;
        }
        line_start_offset++;
    }

    for (int i = line_start_offset; i < editor.offset; i++) {
        if (editor.content[i] == '\n') {
            editor.cursor_y++;
            editor.cursor_x = 0;
        } else {
            editor.cursor_x++;
        }
    }
}

void editor_redraw_screen() {
    clear_screen();
    editor_draw_text();
    editor_draw_status_bar();
    editor_update_cursor_coords();
    move_cursor(editor.cursor_x, editor.cursor_y);
}

void editor_save_file() {
    vfs_node_t* node = vfs_find_or_create(editor.filename);
    if (node) {
        vfs_write_content(node, editor.content, editor.size);
        editor.modified = 0;
    }
    // Краткое сообщение в статус баре
    move_cursor(0, SCREEN_HEIGHT - 1);
    print_string("File saved successfully!              ");
    move_cursor(editor.cursor_x, editor.cursor_y);
}

void editor_insert_char(char c) {
    if (editor.size + 1 >= editor.capacity) {
        // Увеличиваем буфер, если нужно
        editor.capacity *= 2;
        editor.content = (char*)realloc(editor.content, editor.capacity);
        if (!editor.content) return; // Ошибка памяти
    }
    memmove(&editor.content[editor.offset + 1], &editor.content[editor.offset], editor.size - editor.offset);
    editor.content[editor.offset] = c;
    editor.size++;
    editor.offset++;
    editor.modified = 1;
}

void editor_delete_char() {
    if (editor.offset > 0) {
        memmove(&editor.content[editor.offset - 1], &editor.content[editor.offset], editor.size - editor.offset);
        editor.size--;
        editor.offset--;
        editor.modified = 1;
    }
}

void nano_main(const char* filename) {
    // 1. Инициализация
    vfs_node_t* node = vfs_find_or_create(filename);
    if (!node) {
        print_string("Error: could not open or create file.\n");
        return;
    }

    editor.filename = filename;
    editor.capacity = 1024; // Начальный размер буфера
    editor.content = (char*)malloc(editor.capacity);
    editor.size = node->size;
    if (node->content && node->size > 0) {
        memcpy(editor.content, node->content, node->size);
    }

    editor.cursor_x = 0;
    editor.cursor_y = 0;
    editor.offset = 0;
    editor.top_line = 0;
    editor.modified = 0;

    // 2. Главный цикл
    int running = 1;
    while(running) {
        editor_redraw_screen();

        keypress_t key = read_keypress();

        switch (key.scancode) {
            case KEY_UP: if (editor.offset > 0) editor.offset--; break; // Упрощенная навигация
            case KEY_DOWN: if (editor.offset < editor.size) editor.offset++; break;
            case KEY_LEFT: if (editor.offset > 0) editor.offset--; break;
            case KEY_RIGHT: if (editor.offset < editor.size) editor.offset++; break;

            default: // Обработка обычных клавиш
                if (key.ascii == '\b') { // Backspace
                    editor_delete_char();
                } else if (key.ascii == '\r') { // Enter
                    editor_insert_char('\n');
                } else if (key.ascii == KEY_CTRL_S) {
                    editor_save_file();
                } else if (key.ascii == KEY_CTRL_X) {
                    running = 0;
                } else if (key.ascii >= ' ') { // Печатные символы
                    editor_insert_char(key.ascii);
                }
                break;
        }
    }

    // 3. Очистка
    free(editor.content);
    clear_screen(); // Возвращаем управление оболочке
}
