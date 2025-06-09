; boot.asm
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

    mov ah, 0x02      ; Функция BIOS: чтение сектора
    mov al, 1         ; Количество секторов
    mov ch, 0         ; Цилиндр 0
    mov cl, 2         ; Сектор 2
    mov dh, 0         ; Головка 0
    mov dl, 0         ; Диск 0 (A:)
    mov bx, 0x7E00    ; Адрес для загрузки ядра
    int 0x13
    jc disk_error

    mov si, load_msg
    call print_string

    jmp 0x0000:0x7E00 ; Прыгаем на ядро

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

boot_msg:
    db '  |\_/|     ', 13, 10
    db ' ( -.- )    AfricaOS v0.14', 13, 10
    db '  >x x<     Zzz...', 13, 10, 13, 10, 0

load_msg  db "Kernel loaded. Jumping...", 13, 10, 0
error_msg db "Disk read error!", 13, 10, 0

times 510-($-$$) db 0
dw 0xAA55
