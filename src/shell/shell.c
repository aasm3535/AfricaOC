#include "shell.h"
#include "../sys/sys16.h"
#include "../mem/memory.h"
#include "../vfs/vfs.h"
#include "../shell/colors.h"

static const char* find_char(const char* str, char c) {
    while (*str && *str != c) {
        str++;
    }
    return *str == c ? str : 0;
}

int cmd_ping(const char* arg) {
    (void)arg;
    print_string("pong\n");
    return 0;
}

int cmd_clear(const char* arg) {
    (void)arg;
    clear_screen();
    return 0;
}

int cmd_memmap(const char* arg) {
    (void)arg;
    print_memory_map();
    return 0;
}

int cmd_memtest(const char* arg) {
    (void)arg;
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

int cmd_fetch(const char* arg) {
    (void)arg;
    const char* message =
        "  |\_/|     \n"
        " ( -.- )   Welcome in AfricaOS 16bit.\n"
        "  >x x<    Enjoy your stay! Zzz...\n";
    print_string(message);
    return 0;
}

int cmd_ls(const char* arg) {
    (void)arg;
    ls();
    return 0;
}

int cmd_mkdir(const char* arg) {
    mkdir(arg);
    return 0;
}

int cmd_touch(const char* arg) {
    touch(arg);
    return 0;
}

int cmd_cd(const char* arg) {
    cd(arg);
    return 0;
}


typedef struct {
    const char* name;          // The name entered by the user
    const char* help_text;     // Description for the help command
    int (*execute)(const char* arg); // Pointer to the function to be executed
} command_t;


int cmd_help(const char* arg);

static command_t command_table[] = {
    {"help",    "Shows this help message.", cmd_help},
    {"ping",    "Responds with 'pong'.", cmd_ping},
    {"clear",   "Clears the screen.", cmd_clear},
    {"memmap",  "Displays the memory map.", cmd_memmap},
    {"fetch",   "Fetches system information.", cmd_fetch},
    {"memtest", "Runs a simple memory allocation test.", cmd_memtest},
    {"ls",      "Lists content of the current directory.", cmd_ls},
    {"mkdir",   "Creates a new directory. Usage: mkdir <name>", cmd_mkdir},
    {"touch",   "Creates a new empty file. Usage: touch <name>", cmd_touch},
    {"cd",      "Changes current directory. Usage: cd <path>", cmd_cd}
};
static int num_commands = sizeof(command_table) / sizeof(command_t);

int cmd_help(const char* arg) {
    (void)arg;
    print_string("AfricaOS Command List:\n");
    for (int i = 0; i < num_commands; i++) {
        print_string("  ");
        print_string_color(command_table[i].name, COLOR_LIGHT_GREEN);
        print_string(" - ");
        print_string(command_table[i].help_text);
        print_string("\n");
    }
    return 0;
}

void shell_init() {
}

void shell_execute(const char* input) {
    if (input[0] == '\0') {
        return;
    }

    char command[32]; // MAX_FILENAME_LEN = 32
    const char* arg = "";

    const char* space = find_char(input, ' ');
    if (space) {
        int len = space - input;
        if (len < 32) { // Check that the command length does not exceed the limit (MAX_FILENAME_LEN - 1)
            for(int i = 0; i < len; i++) command[i] = input[i];
            command[len] = '\0';
            arg = space + 1;
        } else {
            command[0] = '\0'; // make the command invalid if it's too long
        }
    } else {
        strcpy_s(command, input, 32); // Copy the command considering the maximum length (MAX_FILENAME_LEN)
    }

    for (int i = 0; i < num_commands; i++) {
        if (strcmp(command, command_table[i].name) == 0) {
            command_table[i].execute(arg);
            return;
        }
    }

    print_string("Unknown command: '");
    print_string_color(command, COLOR_LIGHT_RED);
    print_string("'\n");
}
