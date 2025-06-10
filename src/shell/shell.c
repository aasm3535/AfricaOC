#include "shell.h"

// Подключаем все, что нужно нашим командам
#include "../sys/sys16.h"
#include "../mem/memory.h"

// Объявляем внешние функции, которые мы будем использовать
void print_string(const char* str);
void print_hex(unsigned int n);

// --- Реализации команд ---
// Каждая команда - это простая функция, возвращающая 0 в случае успеха.

int cmd_ping() {
    print_string("pong\n");
    return 0;
}

int cmd_clear() {
    clear_screen();
    return 0;
}

int cmd_memmap() {
    print_memory_map();
    return 0;
}

int cmd_memtest() {
    print_string("Running memory test...\n");
    char* block1 = (char*)malloc(32);
    char* block2 = (char*)malloc(128);

    print_string("Initial allocation:\n");
    print_memory_map();

    print_string("Freeing first block...\n");
    free(block1);
    print_memory_map();

    print_string("Freeing second block...\n");
    free(block2);
    print_memory_map();

    print_string("Test finished.\n");
    return 0;
}

int cmd_fetch() {
    const char* message =
        "  |\_/|     \n"
        " ( -.- )    Welcome in AfricaOS 16bit.\n"
        "  >x x<     Enjoy your stay! Zzz...\n";
    print_string(message);
    return 0;
}

// --- Таблица команд ---
// Структура для описания одной команды
typedef struct {
    const char* name;          // Имя, которое вводит пользователь
    const char* help_text;     // Текст для команды help
    int (*execute)();          // Указатель на функцию, выполняющую команду
} command_t;

// Вспомогательная команда help должна быть объявлена до таблицы,
// так как она использует саму таблицу.
int cmd_help();

// Наша главная таблица. Добавление новой команды - это добавление строчки сюда.
static command_t command_table[] = {
    {"help",    "Shows this help message.", cmd_help},
    {"ping",    "Responds with 'pong'.", cmd_ping},
    {"clear",   "Clears the screen.", cmd_clear},
    {"memmap",  "Displays the memory map.", cmd_memmap},
    {"fetch",   "Fetches data.", cmd_fetch},
    {"memtest", "Runs a simple memory allocation test.", cmd_memtest},
};
// Определяем количество команд в таблице
static int num_commands = sizeof(command_table) / sizeof(command_t);


// Реализация команды help, которая теперь может итерировать по таблице
int cmd_help() {
    print_string("AfricaOS Command List:\n");
    for (int i = 0; i < num_commands; i++) {
        print_string("  ");
        print_string(command_table[i].name);
        print_string(" - ");
        print_string(command_table[i].help_text);
        print_string("\n");
    }
    return 0;
}


// --- Публичные функции модуля ---

void shell_init() {
    // Пока что здесь делать нечего
}

void shell_execute(const char* input) {
    if (input[0] == '\0') {
        return; // Ничего не делать для пустой строки
    }

    for (int i = 0; i < num_commands; i++) {
        if (strcmp(input, command_table[i].name) == 0) {
            command_table[i].execute(); // Нашли и выполнили команду
            return;
        }
    }

    // Если мы здесь, значит, команда не найдена в цикле
    print_string("Unknown command: '");
    print_string(input);
    print_string("'\n");
}
