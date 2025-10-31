; AFRICA 2025 - Simple Boot Sector OS
; This boot sector displays "AFRICA 2025" in rainbow colors with sound

[BITS 16]
[ORG 0x7C00]

start:
    ; Initialize segment registers
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Clear screen
    mov ax, 0x0003
    int 0x10

    ; Set up cursor position
    mov dh, 10        ; Row 10
    mov dl, 20        ; Column 20
    call move_cursor

    ; Display "AFRICA 2025" in rainbow colors
    call display_rainbow_text
    
    ; Play some beeps
    call play_startup_sound
    
    ; Hang the system
hang:
    jmp hang

; Move cursor to position (DH=row, DL=col)
move_cursor:
    pusha
    mov bh, 0
    mov ah, 0x02
    int 0x10
    popa
    ret

; Display text in rainbow colors
display_rainbow_text:
    pusha
    
    ; Print "AFRICA" in different colors
    mov si, text_africa
    mov cx, 6         ; Length of "AFRICA"
    mov ah, 0x0E      ; BIOS teletype function
    
.loop_africa:
    lodsb             ; Load character from [SI] into AL
    add ah, 1         ; Increment color (cycling through colors)
    cmp ah, 0x0F      ; Check if we've exceeded max color
    jle .continue_africa
    mov ah, 0x09      ; Reset to bright colors
.continue_africa:
    int 0x10          ; Display character
    loop .loop_africa
    
    ; Add space
    mov al, ' '
    int 0x10
    
    ; Print "2025" in different colors
    mov si, text_year
    mov cx, 4         ; Length of "2025"
    
.loop_year:
    lodsb             ; Load character from [SI] into AL
    add ah, 2         ; Different color progression for numbers
    cmp ah, 0x0F      ; Check if we've exceeded max color
    jle .continue_year
    mov ah, 0x09      ; Reset to bright colors
.continue_year:
    int 0x10          ; Display character
    loop .loop_year
    
    popa
    ret

; Play startup sound using PC speaker
play_startup_sound:
    pusha
    
    ; First beep (high pitch)
    mov al, 0xB6
    out 0x43, al      ; PIT control word
    mov ax, 1000      ; Frequency divisor for high pitch
    out 0x42, al      ; Send low byte
    mov al, ah
    out 0x42, al      ; Send high byte
    
    ; Enable PC speaker
    in al, 0x61
    or al, 3
    out 0x61, al
    
    ; Wait a bit
    call delay_short
    
    ; Disable speaker
    in al, 0x61
    and al, 0xFC
    out 0x61, al
    
    ; Second beep (lower pitch)
    mov al, 0xB6
    out 0x43, al
    mov ax, 1500      ; Different frequency
    out 0x42, al
    mov al, ah
    out 0x42, al
    
    ; Enable speaker
    in al, 0x61
    or al, 3
    out 0x61, al
    
    ; Wait
    call delay_short
    
    ; Disable speaker
    in al, 0x61
    and al, 0xFC
    out 0x61, al
    
    ; Third beep (highest pitch)
    mov al, 0xB6
    out 0x43, al
    mov ax, 800       ; Highest frequency
    out 0x42, al
    mov al, ah
    out 0x42, al
    
    ; Enable speaker
    in al, 0x61
    or al, 3
    out 0x61, al
    
    ; Wait
    call delay_short
    
    ; Disable speaker
    in al, 0x61
    and al, 0xFC
    out 0x61, al
    
    popa
    ret

; Short delay routine
delay_short:
    pusha
    mov cx, 0xFFFF
.delay_loop:
    nop
    loop .delay_loop
    popa
    ret

; Data
text_africa db 'AFRICA'
text_year   db '2025'

; Fill boot sector and add boot signature
times 510-($-$$) db 0
dw 0xAA55