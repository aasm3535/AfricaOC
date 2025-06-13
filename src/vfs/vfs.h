#ifndef VFS_H
#define VFS_H

#define MAX_FILENAME_LEN 32

// Тип узла: файл или директория
typedef enum {
    NODE_FILE,
    NODE_DIRECTORY
} NodeType;

// Основная структура для представления файла или директории
typedef struct vfs_node {
    char name[MAX_FILENAME_LEN];
    NodeType type;

    struct vfs_node* parent;        // Указатель на родительскую директорию
    struct vfs_node* child;         // Указатель на первый дочерний элемент
    struct vfs_node* next_sibling;  // Указатель на следующий элемент в этой же папке

    // Для файлов
    char* content;
    unsigned int size;
} vfs_node_t;


// --- Публичный API файловой системы ---

/**
 * @brief Инициализирует VFS, создает корневую директорию "/".
 */
void vfs_init();

/**
 * @brief Выводит содержимое текущей директории.
 */
void ls();

/**
 * @brief Создает новую директорию в текущей папке.
 * @param name Имя новой директории.
 */
void mkdir(const char* name);

/**
 * @brief Создает новый пустой файл в текущей директории.
 * @param name Имя нового файла.
 */
void touch(const char* name);

/**
 * @brief Меняет текущую рабочую директорию.
 * @param path Имя директории для перехода (включая "..").
 */
void cd(const char* path);

/**
 * @brief Возвращает имя текущей директории для отображения в prompt.
 */
const char* get_current_dir_name();

/**
 * @brief Находит узел по имени. Если не находит, создает новый файл.
 * @param name Имя файла.
 * @return Указатель на найденный или созданный узел, или 0 в случае ошибки.
 */
vfs_node_t* vfs_find_or_create(const char* name); // <<<--- ДОБАВИТЬ

/**
 * @brief Записывает новое содержимое в файл.
 * @param node Узел файла.
 * @param new_content Указатель на буфер с новым содержимым.
 * @param new_size Размер нового содержимого.
 */
void vfs_write_content(vfs_node_t* node, const char* new_content, unsigned int new_size); // <<<--- ДОБАВИТЬ


#endif // VFS_H
