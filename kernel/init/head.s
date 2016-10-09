.section .tempload.text
.global	__begin
.global _start
.global __entry
.global __bootloader_info
.global __mboot_header
.extern centry
.global	headwork_is_done
.extern mboot_ptr

.equ	MBOOT_HEADER_MAGIC,	0x1BADB002
.equ	MBOOT_HEADER_FLAGS,	3	# 0011b: PAGE_ALIGN | MEM_INFO
.equ	MBOOT_CHECKSUM,		-(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

__begin:
__mboot_header:
	.long	MBOOT_HEADER_MAGIC
	.long	MBOOT_HEADER_FLAGS
	.long	MBOOT_CHECKSUM

multiboot_tmp_ptr:
	.long	0

__entry:
_start:
	#ljmp	$0x0008, $_head

_head:
	cli
	cmpl	$0x10000000 + MBOOT_HEADER_MAGIC, %eax
	jne	not_multiboot
	movl	%ebx, multiboot_tmp_ptr
not_multiboot:
	movl	$stack_top0, %esp
	andl	$-15, %esp
	movl	%esp, %ebp

load_gdt:
	movl	$gdt0, %esi
	movl	$gdt0_pa, %edi
	movl	$gdt0_len >> 2, %ecx
	rep
	movsl
	lgdt	gdtr0

init_8259a_pic:
	movb	$0x11, %al
	outb	%al, $0x20
	outb	%al, $0xA0
	movb	$0x20, %al
	outb	%al, $0x21
	movb	$0x28, %al
	outb	%al, $0xA1
	movb	$0x04, %al
	outb	%al, $0x21
	movb	$0x02, %al
	outb	%al, $0xA1
	movb	$0x01, %al
	outb	%al, $0x21
	outb	%al, $0xA1
	movb	$0x00, %al
	outb	%al, $0x21
	outb	%al, $0xA1

init_pit_timer:
.equ	pit_timer_freq,	200	# 每秒 200 次时钟中断
	movb	$0x43, %al
	outb	%al, $0x36
	movw	$1193180 / pit_timer_freq, %ax
	outb	%al, $0x40
	shrw	$8, %ax
	outb	%al, $0x40

load_idt:
	#movl	$idt0, %esi
	#xorl	%eax, %eax
	#movl	$idt0_pa, %edi
	#movl	$idt0_len >> 2, %ecx
	#rep
	#stosl
.set_isr_entrys:
.extern	isr_entry_table
	movl	$isr_entry_table - 0xC0000000, %esi
	movl	$idt0_pa, %edi
	movl	$256, %ecx
.next_desc:
	movl	$0x00080000, %eax
	movw	(%esi), %ax	# 入口点低16位
	stosl
	lodsl			# 入口点高16位
	movw	$0x8E00, %ax
	stosl
	#idt0_pa:
	#.long	0x00080000 | (handler_entry & 0xFFFF)
	#.long	0x00008E00 | (handler_entry & 0xFFFF0000)
	loop	.next_desc
	lidt	idtr0

/*
load_tss:
	movl	$tss0, %esi
	movl	$tss0_pa, %edi
	movl	$tss0_len >> 2, %ecx
	rep
	movsl
	movw	$0x0018, %ax
	ltr	%ax
*/

set_tmp_page_table:
set_tmp_pde:
	//movl	$tmp_pte0_pa + 3, %eax
	//movl	$tmp_pde_pa, %edi
	//stosl
	//addl	$0x1000, %eax
	//movl	$tmp_pde_pa + (0xC0000 >> 8), %edi
	//stosl

	movl	$tmp_pde_pa, %edi
	movl	$0x400, %ecx
	xorl	%eax, %eax
	rep
	stosl
	movl	$tmp_pte0_pa + 3, tmp_pde_pa
	movl	$tmp_pte0_pa + 0x1000 + 3, tmp_pde_pa + 0xC00
	movl	$tmp_pte0_pa + 0x2000 + 3, tmp_pde_pa + 0xC04

set_tmp_pte:
	movl	$1024, %ecx	# 映射物理地址前 4MB
	movl	$3, %eax
	movl	$tmp_pte0_pa, %edi
.loop1:	stosl
	addl	$0x1000, %eax
	loop	.loop1
	movl	$2048, %ecx	# 我们把 0x00000000-0x00800000
	movl	$3, %eax	# 映射到 0xC0000000-0xC0800000
.loop2:	stosl
	addl	$0x1000, %eax
	loop	.loop2

set_cr3:
	movl	$cr3_value, %eax
	movl	%eax, %cr3
	movl	%cr0, %eax
	orl	$0x80000000, %eax
	movl	%eax, %cr0

reload_sregs:
	movw	$0x0010, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %ss
	movw	%ax, %fs
	movw	%ax, %gs
	pushl	$0x0008
	call	to_retf

	jmp	headwork_is_done

to_retf:
	retf

	.align	8
gdt0:
	.quad	0x0000000000000000	# NULL
	.quad	0x00CF9A000000FFFF	# SYS_CODE
	.quad	0x00CF92000000FFFF	# SYS_DATA
	.quad	0x00CFFA000000FFFF	# USR_CODE
	.quad	0x00CFF2000000FFFF	# USR_DATA
	#.quad	0x00008B083800006C	# TSS at 0x83800
	.quad	0x0000000000000000	# reserved for TSS
	.quad	0x0000000000000000	# reserved for LDT

.equ	gdt0_len,	. - gdt0

gdtr0:
	.word	gdt0_len - 1
	.long	gdt0_la

#	.align	8
#idt0:
	#.quad	0x0000000000000000
	//.equ	temp,	0x00008E0000080000 | (isr_handler_0x00 & 0xFFFF) | ((isr_handler_0x00 << 32) & 0xFFFF0000)
	#.long	0x00080000 | (handler_entry & 0xFFFF)
	#.equ	handler_entry, isr_handler_0x00
	#.long	0x00008E00 | (handler_entry & 0xFFFF0000)
	//.quad	temp

/*
struct idt_entry {
	u16 base_lo;
	u16 code_sel;
	u8 always_zero;
	u8 flags;
	u16 base_hi;
};
*/

#.equ	idt0_len,	. - idt0
.equ	idt0_len,	0x800

idtr0:
#.if	idt0_len
#	.word	idt0_len - 1
#.else
	.word	idt0_len
#.endif
	.long	idt0_la		# 注意：IDTR 中的 IDT 表基地址是线性地址
				# 现在 IDT 还不能用，要在分页机制开启后才行

	.align	8
tss0:
	.long	0		# BACK
	.long	stack_top	# ESP0
	.long	0x0010		# SS0
	.long	0		# ESP1
	.long	0		# SS1
	.long	0		# ESP2
	.long	0		# SS2
	.long	cr3_value	# CR3
	.long	0		# EIP
	.long	0		# EFL
	.long	0		# EAX
	.long	0		# ECX
	.long	0		# EDX
	.long	0		# EBX
	.long	0		# ESP
	.long	0		# EBP
	.long	0		# ESI
	.long	0		# EDI
	.long	0		# ES
	.long	0		# CS
	.long	0		# SS
	.long	0		# DS
	.long	0		# FS
	.long	0		# GS
	.long	0		# LDTR
	.word	0		# TRAP_SYM
	.word	tss0_sub_len	# IOMAP_OFF
				# IOMAP
	.byte	0xFF		# END_SYMBOL

.equ	tss0_sub_len,	. - tss0

	.align	4

.equ	tss0_len,	. - tss0

.equ	gdt0_pa,	0x800
.equ	gdt0_la,	gdt0_pa + 0xC0000000
.equ	idt0_pa,	0x0
.equ	idt0_la,	idt0_pa + 0xC0000000
.equ	cr3_value,	tmp_pde_pa
.equ	tss0_pa,	0x83800
.equ	tmp_pde_pa,	0x80000
.equ	tmp_pte0_pa,	0x81000
/*.equ	l1pt0_max,	0x200
.equ	l1pt1_pa,	l1pt0_pa + l1pt0_max * 4
.equ	l1pt1_max,	0x10000*/

headwork_is_done:
	movl	multiboot_tmp_ptr, %eax
	movl	%eax, multiboot_ptr

	movl	$stack_top, %esp
	movl	%esp, %ebp
	andl	$-15, %esp

	call	centry

	.long  0xCCCCCCB8, 0x8000BFCC, 0xE8B9C00B, 0xF3000003, 0x90F4FAAB

	.org	0x800, 0
stack_top0:

.equ	stack_top,	stack_top0 + 0xC0000000


