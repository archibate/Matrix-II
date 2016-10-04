
default : target

%.s.o : %.s
	as -o $@ $<

target : kernel.ko

kernel.ko : head.s.o
	ld -N -e ___start -Ttext 0x100000 -o $@ head.s.o

