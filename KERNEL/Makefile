CC = gcc
AS = nasm
LDSCR = linker.ld
ASFLAGS = -f elf32
CFLAGS = -m32 -ffreestanding -O0

SRCS_C = $(wildcard C/*.c)
SRCS_ASM = $(wildcard ASM/*.asm)

OBJS_C = $(SRCS_C:C/%.c=%.o)
OBJS_ASM = $(SRCS_ASM:ASM/%.asm=%.o)

BIN = kernel.bin
%.o: ASM/%.asm
	$(AS) $(ASFLAGS) $< -o $@

%.o: C/%.c
	$(CC) $(CFLAGS) -c $< -o $@	

$(BIN): $(OBJS_C) $(OBJS_ASM)
	$(CC) -m32 -ffreestanding -T $(LDSCR) -o kernel.bin $(OBJS_C) $(OBJS_ASM) -nostdlib -lgcc 	
disk:
	mv kernel.bin iso/boot/
	grub-mkrescue -o SkyOS.iso iso	
clean:
	rm -f *.o
.PHONY: clean disk
