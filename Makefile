# Makefile

# --- Инструменты ---
ASM = nasm
CC = i686-elf-gcc
LD = i686-elf-ld

# --- Директории ---
BUILD_DIR = build
SRC_DIR = src

# --- Исходные файлы и объекты ---
BOOT_SRC = $(SRC_DIR)/boot.asm

# Автоматически находим все C-файлы в src и его поддиректориях
C_SOURCES = $(shell find $(SRC_DIR) -name '*.c')
# Генерируем имена объектных файлов, сохраняя структуру директорий
# Пример: src/sys/sys16.c -> build/sys/sys16.o
C_OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))

KERNEL_ASM_SRC = $(SRC_DIR)/kernel_entry.asm
KERNEL_ASM_OBJ = $(BUILD_DIR)/kernel_entry.o

BOOT_BIN = $(BUILD_DIR)/boot.bin
KERNEL_BIN = $(BUILD_DIR)/kernel.bin

# --- Финальный образ ---
IMAGE = $(BUILD_DIR)/os.img

# --- Флаги компилятора и линковщика ---
# Добавлены флаги -Wall и -Wextra для вывода всех предупреждений
CFLAGS = -m16 -ffreestanding -O2 -fno-pie -I$(SRC_DIR) -Wall -Wextra
LDFLAGS = -T $(SRC_DIR)/linker.ld --oformat binary

# --- Цели ---
.PHONY: all clean run

all: $(IMAGE)

$(IMAGE): $(BOOT_BIN) $(KERNEL_BIN)
	@echo "Creating disk image: $@"
	@cat $^ > $@
	@truncate -s 1440K $@

$(KERNEL_BIN): $(KERNEL_ASM_OBJ) $(C_OBJECTS)
	@echo "Linking kernel..."
	@$(LD) $(LDFLAGS) -o $@ $^

# Общее правило для компиляции C-файлов.
# Оно будет работать для всех .c файлов в проекте.
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling C file: $<"
	@mkdir -p $(dir $@)  # Создаем поддиректорию в build, если ее нет
	@$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL_ASM_OBJ): $(KERNEL_ASM_SRC)
	@echo "Assembling kernel entry..."
	@mkdir -p $(dir $@)
	@$(ASM) -f elf32 $< -o $@

$(BOOT_BIN): $(BOOT_SRC)
	@echo "Assembling bootloader..."
	@mkdir -p $(dir $@)
	@$(ASM) -f bin $< -o $@

run: all
	@qemu-system-i386 -fda $(IMAGE)

clean:
	@echo "Cleaning up..."
	@rm -rf $(BUILD_DIR)
