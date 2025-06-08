#!/bin/bash

# Компилируем boot.asm и kernel.asm
nasm -f bin boot.asm -o build/boot.bin
nasm -f bin kernel.asm -o build/kernel.bin

# Создаем образ диска
dd if=/dev/zero of=build/disk.img bs=512 count=2880
dd if=build/boot.bin of=build/disk.img bs=512 conv=notrunc
dd if=build/kernel.bin of=build/disk.img bs=512 seek=1 conv=notrunc

echo "Build successful!"

# Запускаем QEMU
qemu-system-i386 -fda build/disk.img -nographic -serial mon:stdio
