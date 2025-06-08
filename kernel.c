__asm__(".code16gcc\n");

#define VIDEO_MEMORY 0xB8000
#define COM1 0x3F8

void outb(unsigned short port, unsigned char value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "d"(port));
}

unsigned char inb(unsigned short port) {
    unsigned char value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "d"(port));
    return value;
}

void init_serial() {
    outb(COM1 + 1, 0x00); // Disable interrupts
    outb(COM1 + 3, 0x80); // Enable DLAB
    outb(COM1 + 0, 0x03); // Set baud rate to 38400 (low byte)
    outb(COM1 + 1, 0x00); // Set baud rate (high byte)
    outb(COM1 + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(COM1 + 2, 0xC7); // Enable FIFO
    outb(COM1 + 4, 0x0B); // Enable IRQs, RTS/DSR set
}

int serial_ready() {
    return inb(COM1 + 5) & 0x20;
}

void serial_print(const char* str) {
    while (*str) {
        while (!serial_ready()); // Wait for serial port to be ready
        outb(COM1, *str++);
    }
}

void print(const char* str) {
    volatile char* video = (volatile char*)VIDEO_MEMORY;
    const char* current = str;
    while (*current) {
        *video++ = *current;
        *video++ = 0x07;
        while (!serial_ready()); // Wait for serial port to be ready
        outb(COM1, *current++);
    }
}

void clear_screen() {
    volatile char* video = (volatile char*)VIDEO_MEMORY;
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        video[i] = ' ';
        video[i+1] = 0x07;
    }
}

void kernel_main() __attribute__((noreturn));

void kernel_main() {
    clear_screen();
    init_serial();

    print("  |\\_/|    \n");
    print(" ( -.- )   AfricaOS v0.14\n");
    print("  >x x<    Zzz...\n\n");

    print("OS: AfricaOS\n");
    print("Version: 0.14\n");
    print("Memory: 640K\n");

    while(1) {
        __asm__ volatile ("hlt"); // Halt to reduce CPU usage
    }
}
