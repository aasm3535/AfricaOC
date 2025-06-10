#ifndef MEMORY_H
#define MEMORY_H

// Определяем тип size_t, так как у нас нет стандартной библиотеки
typedef unsigned int size_t;

/**
 * @brief Инициализирует менеджер памяти.
 * @param heap_start Начальный адрес кучи.
 * @param heap_size Общий размер кучи в байтах.
 */
void memory_init(void* heap_start, size_t heap_size);

/**
 * @brief Выделяет блок памяти указанного размера.
 * @param size Размер блока в байтах.
 * @return Указатель на выделенную память или 0, если память не может быть выделена.
 */
void* malloc(size_t size);

/**
 * @brief Освобождает ранее выделенный блок памяти.
 * @param ptr Указатель на блок, который нужно освободить.
 */
void free(void* ptr);

/**
 * @brief Выводит на экран карту свободных блоков памяти (для отладки).
 */
void print_memory_map();

#endif // MEMORY_H
