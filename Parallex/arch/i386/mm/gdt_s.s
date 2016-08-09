[GLOBAL gdt_flush]

gdt_flush:
        mov eax,[esp + 4]
        lgdt [eax]

        mov ax,0x10
        mov ds,ax
        mov es,ax
        mov fs,ax
        mov gs,ax
        mov ss,ax
        jmp 0x08:.flush

.flush:
    ret 

[GLOBAL tss_flush]    ;tss_flush
tss_flush:
        mov ax,0x28      ;tss 在全局是第五个描述段
        ltr ax           ;加载TR寄存器
        ret 

