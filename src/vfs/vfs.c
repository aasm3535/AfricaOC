// vfs/vfs.c
#include "vfs.h"
#include "../mem/memory.h"
#include "../sys/sys16.h"

// --- Вспомогательные функции (не из .h) ---

// Глобальные указатели на корень и текущую директорию
static vfs_node_t* root_node = 0;
static vfs_node_t* current_dir = 0;

// Безопасная копия строки
// Поиск узла по имени в текущей директории
static vfs_node_t* find_node_in_dir(vfs_node_t* dir, const char* name) {
    for (vfs_node_t* node = dir->child; node != 0; node = node->next_sibling) {
        if (strcmp(node->name, name) == 0) {
            return node;
        }
    }
    return 0; // Не найдено
}

// Создание нового узла (фабрика)
static vfs_node_t* create_node(const char* name, NodeType type, vfs_node_t* parent) {
    // Проверяем, существует ли уже узел с таким именем
    if (find_node_in_dir(parent, name)) {
        print_string("Error: '");
        print_string(name);
        print_string("' already exists.\n");
        return 0;
    }

    vfs_node_t* new_node = (vfs_node_t*)malloc(sizeof(vfs_node_t));
    if (!new_node) {
        print_string("Error: Out of memory!\n");
        return 0;
    }

    strcpy_s(new_node->name, name, MAX_FILENAME_LEN);
    new_node->type = type;
    new_node->parent = parent;
    new_node->child = 0;
    new_node->next_sibling = 0;
    new_node->content = 0;
    new_node->size = 0;

    // Добавляем новый узел в список дочерних элементов родителя
    if (parent->child == 0) {
        parent->child = new_node;
    } else {
        vfs_node_t* last_sibling = parent->child;
        while (last_sibling->next_sibling != 0) {
            last_sibling = last_sibling->next_sibling;
        }
        last_sibling->next_sibling = new_node;
    }

    return new_node;
}


// --- Реализация публичного API VFS ---

void vfs_init() {
    root_node = (vfs_node_t*)malloc(sizeof(vfs_node_t));
    if (!root_node) {
        // Критическая ошибка, без корня VFS невозможна
        print_string("FATAL: Cannot allocate memory for VFS root!\n");
        return;
    }
    strcpy_s(root_node->name, "/", MAX_FILENAME_LEN);
    root_node->type = NODE_DIRECTORY;
    root_node->parent = 0; // У корня нет родителя
    root_node->child = 0;
    root_node->next_sibling = 0;
    root_node->content = 0;
    root_node->size = 0;

    current_dir = root_node;
    print_string("VFS Initialized.\n");
}

void ls() {
    vfs_node_t* node = current_dir->child;
    if (!node) {
        print_string("Directory is empty.\n");
        return;
    }
    while (node) {
        print_string(node->name);
        if (node->type == NODE_DIRECTORY) {
            print_string("/");
        }
        print_string("\n");
        node = node->next_sibling;
    }
}

void mkdir(const char* name) {
    if (!name || *name == '\0') {
        print_string("Usage: mkdir <directory_name>\n");
        return;
    }
    create_node(name, NODE_DIRECTORY, current_dir);
}

void touch(const char* name) {
    if (!name || *name == '\0') {
        print_string("Usage: touch <file_name>\n");
        return;
    }
    create_node(name, NODE_FILE, current_dir);
}

void cd(const char* path) {
    if (!path || *path == '\0' || strcmp(path, ".") == 0) {
        return; // Ничего не делать
    }

    if (strcmp(path, "..") == 0) {
        if (current_dir->parent != 0) {
            current_dir = current_dir->parent;
        }
        return;
    }

    vfs_node_t* target = find_node_in_dir(current_dir, path);
    if (!target) {
        print_string("cd: No such file or directory: ");
        print_string(path);
        print_string("\n");
    } else if (target->type != NODE_DIRECTORY) {
        print_string("cd: Not a directory: ");
        print_string(path);
        print_string("\n");
    } else {
        current_dir = target;
    }
}

vfs_node_t* vfs_find_or_create(const char* name) {
    vfs_node_t* node = find_node_in_dir(current_dir, name);
    if (node) {
        return node;
    }
    // Если не нашли, создаем новый файл (как touch)
    return create_node(name, NODE_FILE, current_dir);
}

void vfs_write_content(vfs_node_t* node, const char* new_content, unsigned int new_size) {
    if (!node || node->type != NODE_FILE) {
        return;
    }

    // Освобождаем старое содержимое, если оно было
    if (node->content) {
        free(node->content);
    }

    // Выделяем новую память и копируем данные
    node->content = (char*)malloc(new_size);
    if (node->content) {
        memcpy(node->content, new_content, new_size);
        node->size = new_size;
    } else {
        node->size = 0; // Ошибка выделения памяти
    }
}

const char* get_current_dir_name() {
    return current_dir->name;
}
