// src/shell.c

#include "shell.h"
#include "../sys/sys16.h"
#include "../mem/memory.h"
#include "../vfs/vfs.h" // <<<--- ДОБАВИТЬ
#include "../editor/nano.h"

// --- Вспомогательные функции для парсинга ---

// Проверяет, начинается ли строка str с префикса pre
int starts_with(const char* pre, const char* str) {
    while(*pre) {
        if(*pre++ != *str++)
            return 0; // не совпадает
    }
    return 1; // совпадает
}

// Находит первый символ в строке (нужен для разделения команды и аргумента)
const char* find_char(const char* str, char c) {
    while (*str && *str != c) {
        str++;
    }
    return *str == c ? str : 0;
}


// --- Реализации команд ---

// Команды, не принимающие аргументы
int cmd_ping(const char* arg) { print_string("pong\n"); return 0; }
int cmd_clear(const char* arg) { clear_screen(); return 0; }
int cmd_memmap(const char* arg) { print_memory_map(); return 0; }
int cmd_memtest(const char* arg) { /* ... (код memtest без изменений) ... */ return 0; }
int cmd_fetch(const char* arg) { /* ... (код fetch без изменений) ... */ return 0; }
int cmd_ls(const char* arg) { ls(); return 0; }

// Команды, принимающие аргументы
int cmd_mkdir(const char* arg) { mkdir(arg); return 0; }
int cmd_touch(const char* arg) { touch(arg); return 0; }
int cmd_cd(const char* arg) { cd(arg); return 0; }
int cmd_nano(const char* arg) {
    if (!arg || *arg == '\0') {
        print_string("Usage: nano <filename>\n");
        return 1;
    }
    nano_main(arg);
    return 0;
}

// --- Таблица команд ---

typedef struct {
    const char* name;
    const char* help_text;
    // Указатель на функцию теперь принимает аргумент (const char*)
    int (*execute)(const char* arg);
} command_t;

int cmd_help(); // Объявляем заранее

static command_t command_table[] = {
    {"help",    "Shows this help message.", (void*)cmd_help},
    {"ping",    "Responds with 'pong'.", cmd_ping},
    {"clear",   "Clears the screen.", cmd_clear},
    {"memmap",  "Displays the memory map.", cmd_memmap},
    {"fetch",   "Fetches data.", cmd_fetch},
    {"memtest", "Runs a simple memory allocation test.", cmd_memtest},
    // --- НОВЫЕ КОМАНДЫ VFS ---
    {"ls",      "Lists content of the current directory.", cmd_ls},
    {"mkdir",   "Creates a new directory. Usage: mkdir <name>", cmd_mkdir},
    {"touch",   "Creates a new empty file. Usage: touch <name>", cmd_touch},
    {"cd",      "Changes current directory. Usage: cd <path>", cmd_cd},
    {"nano",    "A simple text editor. Usage: nano <filename>", cmd_nano},
};
static int num_commands = sizeof(command_table) / sizeof(command_t);

// Реализация help (без изменений, кроме вызова)
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

void shell_init() { }

void shell_execute(const char* input) {
    if (input[0] == '\0') {
        return;
    }

    char command[MAX_FILENAME_LEN];
    const char* arg = "";

    const char* space = find_char(input, ' ');
    if (space) {
        // Копируем часть до пробела как команду
        int len = space - input;
        if (len < MAX_FILENAME_LEN) {
            for(int i = 0; i < len; i++) command[i] = input[i];
            command[len] = '\0';
            arg = space + 1; // Аргумент - все что после пробела
        }
    } else {
        // Нет пробела, вся строка - команда
        strcpy_s(command, input, MAX_FILENAME_LEN);
    }

    for (int i = 0; i < num_commands; i++) {
        if (strcmp(command, command_table[i].name) == 0) {
            command_table[i].execute(arg);
            return;
        }
    }

    print_string("Unknown command: '");
    print_string(command);
    print_string("'\n");
}
