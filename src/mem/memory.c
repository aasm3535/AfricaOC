#include "memory.h"

// Объявляем внешние функции, которые нам понадобятся
void print_string(const char* str);
void print_hex(unsigned int n);
void print_dec(unsigned int n);

// Структура, описывающая заголовок каждого блока памяти (как свободного, так и занятого)
typedef struct header {
    size_t size;          // Размер блока (включая заголовок)
    unsigned char is_free; // 1, если блок свободен, 0 - если занят
    struct header* next;  // Указатель на следующий блок в списке
} header_t;

static header_t* head = 0; // Голова списка блоков
static header_t* tail = 0; // Хвост списка блоков

void memory_init(void* heap_start, size_t heap_size) {
    if (heap_size < sizeof(header_t)) {
        return; // Слишком мало памяти для инициализации
    }
    head = (header_t*)heap_start;
    head->size = heap_size;
    head->is_free = 1;
    head->next = 0;
    tail = head;
}

void* malloc(size_t size) {
    if (size == 0) {
        return 0;
    }

    // Выравниваем размер до 4 байт для простоты
    size_t total_size = size + sizeof(header_t);
    if (total_size % 4 != 0) {
        total_size = ((total_size / 4) + 1) * 4;
    }

    header_t* current = head;
    while (current) {
        // Ищем первый подходящий свободный блок
        if (current->is_free && current->size >= total_size) {
            // Если блок больше, чем нужно, разбиваем его на два
            if (current->size > total_size + sizeof(header_t)) {
                header_t* new_block = (header_t*)((char*)current + total_size);
                new_block->is_free = 1;
                new_block->size = current->size - total_size;
                new_block->next = current->next;

                current->size = total_size;
                current->next = new_block;

                if (tail == current) {
                    tail = new_block;
                }
            }

            current->is_free = 0;
            // Возвращаем указатель на область данных (сразу после заголовка)
            return (void*)((char*)current + sizeof(header_t));
        }
        current = current->next;
    }

    // Если не нашли подходящий блок
    return 0;
}

void free(void* ptr) {
    if (ptr == 0) {
        return;
    }

    // Получаем указатель на заголовок блока
    header_t* block_header = (header_t*)((char*)ptr - sizeof(header_t));
    block_header->is_free = 1;

    // TODO: Продвинутая версия могла бы здесь объединять соседние свободные блоки (coalescing),
    // чтобы бороться с фрагментацией памяти. Пока что оставляем простую реализацию.
}

void* realloc(void* ptr, size_t new_size) {
    if (!ptr) {
        // Если ptr равен NULL, realloc ведет себя как malloc
        return malloc(new_size);
    }
    if (new_size == 0) {
        // Если new_size равен 0, realloc ведет себя как free
        free(ptr);
        return 0;
    }

    header_t* block_header = (header_t*)((char*)ptr - sizeof(header_t));
    size_t old_size = block_header->size - sizeof(header_t);

    if (new_size <= old_size) {
        // Можно использовать тот же блок, если новый размер меньше или равен
        return ptr;
    }

    // Выделяем новый блок и копируем данные
    void* new_ptr = malloc(new_size);
    if (new_ptr) {
        memcpy(new_ptr, ptr, old_size);
        free(ptr); // Освобождаем старый блок
    }
    return new_ptr;
}

void print_memory_map() {
    header_t* current = head;
    print_string("Memory Map:\n");
    print_string("Address      Size         Status\n");
    print_string("----------------------------------\n");
    while (current) {
        print_string("0x");
        print_hex((unsigned int)current);
        print_string("     ");
        print_dec(current->size);
        print_string(" B       ");
        if (current->is_free) {
            print_string("Free\n");
        } else {
            print_string("Used\n");
        }
        current = current->next;
    }
    print_string("----------------------------------\n");
}
