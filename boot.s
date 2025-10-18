BITS 16
ORG 0x7C00

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    ; Simple message to show we're in bootloader
    mov si, boot_msg
    call print_string

    ; Jump directly to kernel at 0x7E00 (right after bootloader)
    jmp 0x0000:0x7E00

print_string:
    mov ah, 0x0E
.loop:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .loop
.done:
    ret

boot_msg db "Booting...", 0

; Fill boot sector and add signature
times 510-($-$$) db 0
dw 0xAA55

; Kernel starts here at 0x7E00
