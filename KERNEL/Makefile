CC = gcc
AS = nasm
LDSCR = linker.ld
LD = ld
ASFLAGS = -f elf32
CFLAGS = -m32 -ffreestanding -fno-stack-protector -fno-pic -fno-pie \
         -mno-sse -mno-sse2 -mno-mmx -mno-80387 \
          -fno-builtin \
         -Wall -Wextra -O0

SRCS_C = $(wildcard C/*.c)
SRCS_ASM = $(wildcard ASM/*.asm)

OBJS_C = $(SRCS_C:C/%.c=%.o)
OBJS_ASM = $(SRCS_ASM:ASM/%.asm=%.o)
#MG = tst.img
bt1 = BootHDD1.bin
bt2 = BootHDD2.bin
BIN = kernel.bin
%.o: ASM/%.asm
	$(AS) $(ASFLAGS) $< -o $@
%.o: C/%.c
	$(CC) $(CFLAGS) -c $< -o $@	


$(BIN): $(OBJS_C) $(OBJS_ASM)
	$(LD) -m elf_i386 -T $(LDSCR) --oformat binary -o $(BIN) kmain.o $(filter-out kmain.o, $(OBJS_C) $(OBJS_ASM)) -nostdlib



disk: $(bt1) $(bt2) $(BIN)
	dd if=$(bt1) of=tst.img conv=notrunc
	dd if=$(bt2) of=tst.img seek=1 conv=notrunc
	dd if=$(BIN) of=tst.img seek=10 conv=notrunc #CL = 11
	
qemu:
	qemu-system-i386 -drive format=raw,file=tst.img,index=0,media=disk -audiodev pa,id=speaker \
	-machine pcspk-audiodev=speaker \
	-d int,cpu_reset -no-reboot -no-shutdown
qemu_gdb:
	qemu-system-i386 -drive format=raw,file=tst.img,index=0,media=disk -audiodev pa,id=speaker \
	-machine pcspk-audiodev=speaker \
	-d int,cpu_reset -no-reboot -no-shutdown -s -S
boot:
	nasm -f bin BootHDD1.asm -o BootHDD1.bin
	nasm -f bin BootHDD2.asm -o BootHDD2.bin
single:
	gcc $(CFLAGS) -O0 -c kmain.c -o kmain.o
clean:
	rm -f *.o
.PHONY: clean disk qemu
