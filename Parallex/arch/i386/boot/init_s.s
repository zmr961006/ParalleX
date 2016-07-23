;初始化内核使用 multiboot 格式

MBOOT_HEADER_MAGIC    equ  0x1BADB002


MBOOT_PAGE_ALIGN    equ  1 << 0 

MBOOT_MEM_INFO        equ  1 << 1

MBOOT_HEADER_FLAGS    equ  MBOOT_PAGE_ALIGN  | MBOOT_MEM_INFO

MBOOT_CHECKSUM        equ  - (MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS )


[bits 32]

section .text 

dd MBOOT_HEADER_MAGIC 
dd MBOOT_HEADER_FLAGS 
dd MBOOT_CHECKSUM 

[GLOBAL  start]

[GLOBAL  glb_mboot_ptr]

[EXTERN  kern_entry]


start:

    mov [glb_mboot_ptr],ebx
    mov esp,STACK_TOP
    and esp,0FFFFFFF0H
    mov ebp,0

    call kern_entry 

noreturn:
    hlt
    jmp noreturn

section .data

stack: times 1024 db 0

STACK_TOP equ $-stack-1

glb_mboot_ptr:dd 0 

