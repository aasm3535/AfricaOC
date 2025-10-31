#!/bin/bash

# Build script for AFRICA 2025 Boot System

echo "Building AFRICA 2025 Boot System..."

# Clean previous builds
rm -f *.bin *.img

# Compile assembly to binary
echo "Compiling assembly code..."
nasm -f bin africa2025.asm -o africa2025.bin

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo "Assembly compilation failed!"
    exit 1
fi

# Create a 1.44MB floppy disk image
echo "Creating 1.44MB floppy disk image..."
dd if=/dev/zero of=africa2025.img bs=1024 count=1440

# Copy our boot sector to the image
echo "Installing boot sector..."
dd if=africa2025.bin of=africa2025.img seek=0 count=1 bs=512 conv=notrunc

# Verify the image
echo "Verifying image..."
ls -la africa2025.img

echo "Build complete! Created africa2025.img"
echo ""
echo "To test the image:"
echo "1. qemu-system-i386 -drive format=raw,file=africa2025.img"
echo "2. Or burn to a USB drive and boot from it"