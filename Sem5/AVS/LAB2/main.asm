; -----------------------------
; Boot sector: enter PM, print, return to RM
; -----------------------------
[BITS 16]
[ORG 0x7C00]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov si, msg_real_mode
    call print_string_real_mode

    call wait_for_keypress

    ; Load GDT
    lgdt [gdt_descriptor]

    ; Set PE=1 in CR0
    mov eax, cr0
    or  eax, 1
    mov cr0, eax

    jmp CODE32_SEL:protected_mode_entry   ; far jump flush

; -----------------------------
; Protected Mode (32-bit)
; -----------------------------
[BITS 32]
protected_mode_entry:
    mov ax, DATA32_SEL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x7C00

    ; clear text screen
    mov edi, 0xB8000
    mov ax, 0x0720
    mov ecx, 80*25
    rep stosw

    ; "Protected Mode"
    mov edi, 0xB8000
    mov esi, msg_protected_mode
    call print_string

    ; Check PE=1
    mov eax, cr0
    test eax, 1
    jz  no_pe
    mov esi, msg_pe
    call print_string
no_pe:

    ; -------- NEW: return to real mode --------
    ; Переходим в 16-битный сегмент кода внутри PM (CS.D=0)
    mov ax, DATA16_SEL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp CODE16_SEL:pm16_entry             ; far jump => 16-bit PM

; -----------------------------
; 16-bit protected segment (D=0), still PE=1
; -----------------------------
[BITS 16]
pm16_entry:
    cli
    ; Сбросить PE
    mov eax, cr0
    and eax, ~1
    mov cr0, eax

    ; Обязателен far jump для сброса конвейера => real mode
    jmp 0x0000:real_mode_entry            ; CS=0x0000, IP=offset(label)

; -----------------------------
; Back in REAL MODE (BIOS ints available)
; -----------------------------
real_mode_entry:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    mov si, msg_back_to_real
    call print_string_real_mode

hang:
    hlt
    jmp hang

; -----------------------------
; real mode print / key
; -----------------------------
print_string_real_mode:
    lodsb
    or  al, al
    jz  .done_real
    mov ah, 0x0E
    int 0x10
    jmp print_string_real_mode
.done_real:
    ret

wait_for_keypress:
    xor ax, ax
    int 0x16
    ret

; -----------------------------
; protected mode print
; -----------------------------
[BITS 32]
print_string:
    lodsb
    test al, al
    jz   .done_pm
    mov ah, 0x07
    stosw
    jmp  print_string
.done_pm:
    ret

; -----------------------------
; messages
; -----------------------------
[BITS 16]
msg_real_mode        db 'Real Mode: Press any key to enter Protected Mode...', 0
msg_protected_mode   db 'Protected Mode: Hello World! ', 0
msg_pe               db 'PE=1 (Protected Mode Active)', 0
msg_back_to_real     db 13,10,'Back to Real Mode. BIOS is alive again!', 0   ; NEW

; -----------------------------
; GDT (add 16-bit segments)  -- NEW
; -----------------------------
align 8
gdt:
    dq 0x0000000000000000           ; null
    dq 0x00CF9A000000FFFF           ; CODE32  base=0, limit=4GB, D=1, G=1
    dq 0x00CF92000000FFFF           ; DATA32  base=0, limit=4GB, D=1, G=1
    dq 0x000F9A000000FFFF           ; CODE16  base=0, limit=64KB, D=0, G=0
    dq 0x000F92000000FFFF           ; DATA16  base=0, limit=64KB, D=0, G=0

gdt_descriptor:
    dw gdt_end - gdt - 1
    dd gdt
gdt_end:

; Selectors
CODE32_SEL equ 0x08
DATA32_SEL equ 0x10
CODE16_SEL equ 0x18
DATA16_SEL equ 0x20

; -----------------------------
; Boot sector signature
; -----------------------------
times 510 - ($ - $$) db 0
dw 0xAA55

; nasm -f bin -o main.bin main.asm
; qemu-system-i386 -drive format=raw,file=main.bin
