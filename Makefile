CC = gcc
LD = ld
ASM = nasm
CFLAGS = -ffreestanding -fno-pic -fno-stack-protector -nostdlib -m16 -Wall -Wextra
LDFLAGS = -nostdlib -static -T linker.ld --nmagic -m elf_i386
BUILD_DIR = build

all: $(BUILD_DIR)/os.img

$(BUILD_DIR)/boot.bin: boot.asm
	@mkdir -p $(BUILD_DIR)
	$(ASM) -f bin $< -o $@

$(BUILD_DIR)/kernel.bin: kernel.c linker.ld
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c kernel.c -o $(BUILD_DIR)/kernel.o
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/kernel.bin $(BUILD_DIR)/kernel.o

$(BUILD_DIR)/os.img: $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin
	cat $^ > $@
	truncate -s 1440K $@

clean:
	rm -rf $(BUILD_DIR) *.bin *.o *.img

run: $(BUILD_DIR)/os.img
	qemu-system-i386 -fda $<
