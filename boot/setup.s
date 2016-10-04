.section .text
.global _start

_start:
.code16
rmode_start:
	xorw	%ax, %ax
	movw	%ax, %ds	# let linker take care for it
	movw	%ax, %es
	movw	%ax, %ss
	movw	$stack_top, %sp
	#movw	$msg_start, %si
	#call	print_bios

copy_gdt:
	#movw	%cs, %ax
	#movw	%ax, %cx
	#shrw	$16, %ax	# high 4 bits let %cs:gdt0 out of 0xFFFF,
				# we need put it into 16~20 bit of GDT
				# physical address
	#movw	%ax, gdtr0 + 4	# high 16 bits of GDT 32-bit physical address
	#shlw	$4, %cx		# this compute low 16 bits of address
	#addw	$gdt0, %cx
	#movw	%ax, gdtr0 + 2	# so, wherever our kernel is loaded,
				# we can always get physical address of GDT
	#pushw	%cs
	pushw	$0x8000		# set GDT to 0x80000, templately,
	popw	%es		# we will set it later (in init)
	pushw	%cs
	popw	%ds
	xorw	%ax, %ax
	movw	%ax, %ss
	movw	%ax, %fs
	movw	%ax, %gs
	movw	%ax, %di
	movw	$gdt0, %si	# gdt0 is already added 0x8000 by linker
	movw	$gdt0_len >> 1, %cx	# gdt0_len should be an even number
	cld
	rep	# %cx
	movsw	# %ds:(%si), %es:(%di)
	xorw	%ax, %ax
	movw	%ax, %ds
	movw	%ax, %es

load_gdt:
	cli
	lgdt	%cs:gdtr0	# %ds is zero
				# while linking, gdtr0 will be set to
				# 0x80**, because we specified flag :
				# -Ttext 0x8000 in Makefile.

load_idt:
	lidt	%cs:idtr0

	movb	$0xFF, %al
	outb	%al, $0xA1
	call	delay
	movb	$0xFF, %al
	outb	%al, $0x21

reset_math:
	movb	$0x00, %al
	outb	%al, $0xF0
	call	delay
	outb	%al, $0xF1
	call	delay

enable_a20:
	inb	$0x64, %al
	call	delay
	testb	$0x02, %al
	jnz	enable_a20
	movb	$0xDF, %al
	outb	%al, $0x64
	call	delay

set_cr0_pe:
	movl	%cr0, %eax
	orl	$0x00000001, %eax
	movl	%eax, %cr0		# OK!!! pmode now

jump_to_pmode:			# same as : ljmp $0x0008, $pmode_start
	.byte	0x66, 0xEA	# but, it will be failed  work if
	.long	0x90200 + pmode_start	# <<-THIS is greater than 0xFFFF.
	.word	0x0008		# this is because the assembler is in
				# real mode (.code16), it does not
				# understand 32-bit opcode. so we use
				# .byte, .word and .long instead
				# to create a 32-bit opcode

fin:	hlt
	jmp	fin

delay:	outb	%al, $0x80
	ret

	.align	2

gdtr0:
	.word	gdt0_len
	#.long	0	# will be filled later
	.long	0x80000

idtr0:			# will reloaded later
	.word	0
	.long	0

	.align	8

gdt0:
	.quad	0x0000000000000000
	.quad	0x00CF9A000000FFFF
	.quad	0x00CF92000000FFFF
	.quad	0x0000000000000000
	.quad	0x0000000000000000

.equ	gdt0_len,	. - gdt0 - 1

print_bios:
	movb	$0x0E, %ah
print_bios_loop:
	lodsb
	testb	%al, %al
	jz	print_bios_done
	int	$0x10
	jmp	print_bios_loop
print_bios_done:
	ret

.code32
pmode_start:
	cli
	movw	$0x0010, %ax
	movw	%ax, %ss
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs
	movl	$stack_top, %esp
	movl	%esp, %ebp

	xorl	%eax, %eax
	movl	%eax, %ecx
	movl	%eax, %ebx
	movl	%eax, %edx
	movl	%eax, %esi
	movl	%eax, %edi
	cmp	%esp, %eax
	cld
	clc

	call	csetup
	ud2
halt_loop:
	hlt
	jmp	halt_loop
	jmp	.

#csetup:
#	movl	%esp, %ebp
#	subl	$16, %esp
#	pushl	%esi
#	pushl	%edi
#	leal	0xB8000, %edi
#	movw	$0x0303, %ax
#	stosw
#	popl	%edi
#	popl	%esi
#	jmp	.
#	leave
#	ret

start_datas:
msg_start:	.ascii	"Kernel started\r\n\0"

	.org	0x200, 0
stack_top:

csetup:


