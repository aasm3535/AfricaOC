#include "memory.h"
#include "../sys/sys16.h" // Добавлено для объявления memcpy

typedef struct header {
    size_t size;
    unsigned char is_free;
    struct header* next;
} header_t;

static header_t* head = 0;
static header_t* tail = 0;

void memory_init(void* heap_start, size_t heap_size) {
    if (heap_size < sizeof(header_t)) {
        return;
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

    size_t total_size = size + sizeof(header_t);
    if (total_size % 4 != 0) {
        total_size = ((total_size / 4) + 1) * 4;
    }

    header_t* current = head;
    while (current) {
        if (current->is_free && current->size >= total_size) {
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
            return (void*)((char*)current + sizeof(header_t));
        }
        current = current->next;
    }
    return 0;
}

void free(void* ptr) {
    if (ptr == 0) {
        return;
    }
    header_t* block_header = (header_t*)((char*)ptr - sizeof(header_t));
    block_header->is_free = 1;
}

void* realloc(void* ptr, size_t new_size) {
    if (!ptr) {
        return malloc(new_size);
    }
    if (new_size == 0) {
        free(ptr);
        return 0;
    }

    header_t* block_header = (header_t*)((char*)ptr - sizeof(header_t));
    size_t old_size = block_header->size - sizeof(header_t);

    if (new_size <= old_size) {
        return ptr;
    }

    void* new_ptr = malloc(new_size);
    if (new_ptr) {
        memcpy(new_ptr, ptr, old_size);
        free(ptr);
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
