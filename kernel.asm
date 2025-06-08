org 0x7E00
bits 16

%define ROOT_PASSWORD "africa"

start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7E00

    mov ax, 0x0003
    int 0x10

    mov si, africa_art
    call print_string
    mov si, help_msg
    call print_string
    jmp shell

shell:
    mov si, prompt
    call print_string

    mov di, cmd_buffer
    call clear_buffer
    call read_input

    ; Убраны PUSH/POP - основная ошибка
    mov si, cmd_buffer

    mov di, cmd_clear
    call strcmp
    je .clear_cmd

    mov di, cmd_fetch
    call strcmp
    je .fetch_cmd

    mov di, cmd_reboot
    call strcmp
    je .reboot_cmd

    mov di, cmd_root
    call strcmp
    je .root_cmd

    mov di, cmd_help
    call strcmp
    je .help_cmd

    mov si, unknown_cmd_msg
    call print_string
    jmp shell

.clear_cmd:
    call clear_screen
    jmp shell

.fetch_cmd:
    mov si, africa_art
    call print_string
    mov si, system_info
    call print_string
    jmp shell

.reboot_cmd:
    cmp byte [is_root], 1
    je .do_reboot
    mov si, no_permission_msg
    call print_string
    jmp shell

.do_reboot:
    mov si, rebooting_msg
    call print_string
    jmp 0xFFFF:0x0000

.root_cmd:
    mov si, password_prompt
    call print_string
    mov di, input_buffer
    call clear_buffer
    call read_input
    mov si, input_buffer
    mov di, root_password
    call strcmp
    je .root_success
    mov si, wrong_password_msg
    call print_string
    jmp shell

.root_success:
    mov byte [is_root], 1
    mov si, root_granted_msg
    call print_string
    jmp shell

.help_cmd:
    mov si, help_output
    call print_string
    jmp shell

print_string:
    push si
    push ax
.loop:
    lodsb
    test al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp .loop
.done:
    pop ax
    pop si
    ret

read_input:
    push ax
    push cx
    push di
    xor cx, cx
.loop:
    mov ah, 0x00
    int 0x16
    cmp al, 0x0D
    je .done
    cmp al, 0x08
    je .backspace
    cmp al, 0x7F
    je .backspace
    cmp cx, 15
    je .loop
    stosb
    inc cx
    mov ah, 0x0E
    int 0x10
    jmp .loop
.backspace:
    test cx, cx
    jz .loop
    dec di
    dec cx
    mov ah, 0x0E
    mov al, 0x08
    int 0x10
    mov al, ' '
    int 0x10
    mov al, 0x08
    int 0x10
    jmp .loop
.done:
    mov al, 0
    stosb
    mov ah, 0x0E
    mov al, 0x0D
    int 0x10
    mov al, 0x0A
    int 0x10
    pop di
    pop cx
    pop ax
    ret

clear_buffer:
    push di
    push cx
    mov cx, 16
    mov al, 0
    rep stosb
    pop cx
    pop di
    ret

; ФИКС: переписана strcmp для корректной работы
strcmp:
    push si
    push di
    push ax
    push bx
.loop:
    mov al, [si]
    mov bl, [di]
    cmp al, bl
    jne .not_equal
    test al, al
    jz .equal
    inc si
    inc di
    jmp .loop
.not_equal:
    xor al, al  ; Сбрасываем ZF
    test al, 1  ; Устанавливаем ZF=0
    jmp .done
.equal:
    xor al, al  ; Устанавливаем ZF=1
    test al, al
.done:
    pop bx
    pop ax
    pop di
    pop si
    ret

clear_screen:
    push ax
    mov ax, 0x0003
    int 0x10
    pop ax
    ret

africa_art:
    db "  /\_/\    ", 13, 10
    db " ( o.o )   AfricaOS", 13, 10
    db "  > ^ <    v0x14", 13, 10, 0
help_msg        db "Type 'help'", 13, 10, 0
prompt          db "/aos> ", 0
unknown_cmd_msg db "Bad cmd", 13, 10, 0
no_permission_msg db "No root", 13, 10, 0
rebooting_msg   db "Reboot...", 13, 10, 0
password_prompt db "Pass: ", 0
wrong_password_msg db "Bad pass", 13, 10, 0
root_granted_msg db "Root OK", 13, 10, 0
system_info     db "AfricaOS v0x14", 13, 10, 0
help_output     db "clear fetch reboot root help", 13, 10, 0
cmd_clear       db "clear", 0
cmd_fetch       db "fetch", 0
cmd_reboot      db "reboot", 0
cmd_root        db "root", 0
cmd_help        db "help", 0
root_password   db ROOT_PASSWORD, 0
is_root         db 0
cmd_buffer      times 16 db 0
input_buffer    times 16 db 0
