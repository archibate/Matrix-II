
target : boot.bin

bootsect.bin : bootsect.s
	as -o bootsect.s.o bootsect.s
	ld -N -e _start -Ttext 0 -o bootsect.elf bootsect.s.o
	objcopy -O binary -R .note -R .commit -S bootsect.elf bootsect.bin

setup.bin : setup.s setup.c
	as -o setup.s.o setup.s
	ld -N -e _start -Ttext 0 -o setup.head.elf setup.s.o
	objcopy -O binary -R .note -R .commit -S setup.head.elf setup.head.bin
	cc -c -g -w -o setup.c.o setup.c
	cc -c -g -w -o vsprintf.c.o vsprintf.c
	cc -c -g -w -o memory.c.o memory.c
	ld -g -N -e _start -Ttext 0x90400 -o setup.main.elf setup.c.o \
		vsprintf.c.o memory.c.o
	objcopy -O binary -R .note -R .commit -S setup.main.elf setup.main.bin
	cat setup.head.bin setup.main.bin > setup.bin

boot.bin : bootsect.bin setup.bin
	cat bootsect.bin setup.bin > boot.bin

clean :
	rm -f *.o *.elf *.bin

