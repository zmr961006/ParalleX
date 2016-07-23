#!Makefile

# --------------------------------------------------------
#
#    Parallex OS Makefile 
#
#    默认使用的C语言编译器是 GCC、汇编语言编译器是 nasm
#
#        XiYouLinux Group  2016.07
# --------------------------------------------------------

C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))
S_SOURCES = $(shell find . -name "*.s")
S_OBJECTS = $(patsubst %.s, %.o, $(S_SOURCES))

CC = gcc
LD = ld
ASM = nasm

C_FLAGS = -std=c99 -c -m32 -Wall -Wextra -ggdb -gstabs+ -ffreestanding \
                 -I. -Iinclude -Iarch/i386 -Iarch/i386/include
LD_FLAGS = -T arch/i386/scripts/kernel.lds -nostdlib  -m elf_i386
ASM_FLAGS = -f elf -g -F stabs  

all: $(S_OBJECTS) $(C_OBJECTS) link 

# The automatic variable `$<' is just the first prerequisite
.c.o:
	@echo 编译代码文件 $< ...
	$(CC) $(C_FLAGS) $< -o $@

.s.o:
	@echo 编译汇编文件 $< ...
	$(ASM) $(ASM_FLAGS) $<

link:
	@echo 链接内核文件...
	$(LD) $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS) -o Parallex

.PHONY:clean
clean:
	$(RM) $(S_OBJECTS) $(C_OBJECTS) Parallex

.PHONY:disk 
disk:
	qemu-img  create -f qcow disk.img 8G

.PHONY:iso
iso:
	cp Parallex isodir/boot/
	grub2-mkrescue -o Parallex.iso isodir

.PHONY:runiso
runiso:
	qemu -m 128 -hda disk.img -cdrom Parallex.iso -boot d


.PHONY:debug
debug:
	#gdb -x scripts/gdbinit &
	qemu  -s -m 128 -hda disk.img -cdrom Parallex.iso -boot d 
	sleep 1

.PHONY:code_line_count
code_line_count:
	find . -type f -name "*.[c|h|s]" -exec cat {} \; | wc -l
