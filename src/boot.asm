; src/boot.asm
org 0x7C00
bits 16

; --- Константы ---
KERNEL_LOAD_ADDR equ 0x7E00
KERNEL_SECTORS   equ 16         ; <<<--- КЛЮЧЕВОЕ ИСПРАВЛЕНИЕ: читаем 16 секторов

start:
    ; Настраиваем сегменты и стек
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Устанавливаем текстовый режим 80x25
    mov ax, 0x0003
    int 0x10

    ; Выводим приветствие
    mov si, boot_msg
    call print_string

    ; --- Загружаем ядро с диска ---
    mov ah, 0x02                ; Функция BIOS: чтение секторов
    mov al, KERNEL_SECTORS      ; Количество секторов для чтения
    mov ch, 0                   ; Цилиндр 0
    mov cl, 2                   ; Начинаем с сектора 2 (первый сектор - это сам загрузчик)
    mov dh, 0                   ; Головка 0
    mov dl, 0                   ; Диск 0 (A:)
    mov bx, KERNEL_LOAD_ADDR    ; Адрес для загрузки ядра
    int 0x13
    jc disk_error               ; Если ошибка (установлен флаг Carry), прыгаем

    mov si, load_msg
    call print_string

    ; Все готово, прыгаем на точку входа в ядро
    jmp 0x0000:KERNEL_LOAD_ADDR

disk_error:
    mov si, error_msg
    call print_string
    jmp $ ; Бесконечный цикл в случае ошибки

print_string:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E ; Функция BIOS: телетайп
    int 0x10
    jmp print_string
.done:
    ret

boot_msg:
    db '  |_/|', 13, 10
    db ' ( -.- )   AfricaOS v0.14', 13, 10
    db '  >x x<    Zzz...', 13, 10, 13, 10, 0

load_msg  db "Kernel (16 sectors) loaded. Jumping...", 13, 10, 0
error_msg db "Disk read error!", 13, 10, 0

times 510-($-$$) db 0
dw 0xAA55
