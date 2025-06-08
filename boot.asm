org 0x7C00
bits 16

start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov ax, 0x0003
    int 0x10

    mov si, boot_msg
    call print_string

    mov ah, 0x02    ; BIOS read sector function
    mov al, 1       ; Number of sectors to read
    mov ch, 0       ; Cylinder 0
    mov cl, 2       ; Sector 2 (sector 1 is boot sector)
    mov dh, 0       ; Head 0
    mov dl, 0       ; Drive 0 (floppy)
    mov bx, 0x7E00  ; Destination address
    int 0x13
    jc disk_error

    mov si, load_msg
    call print_string

    ; Verify kernel presence at 0x7E00
    mov al, [0x7E00]
    mov ah, 0x0E
    int 0x10        ; Print first byte at 0x7E00 for debug
    mov si, jump_msg
    call print_string

    jmp 0x0000:0x7E00  ; Explicit segment:offset jump

disk_error:
    mov si, error_msg
    call print_string
    jmp $

print_string:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp print_string
.done:
    ret

boot_msg db "Booting AfricaOS...", 13, 10, 0
load_msg db "Kernel loaded, jumping to 0x7E00", 13, 10, 0
jump_msg db "Jumping now...", 13, 10, 0
error_msg db "Disk error", 13, 10, 0

times 510-($-$$) db 0
dw 0xAA55
