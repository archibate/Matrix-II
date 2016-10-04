.section .text
.global	___start

___start:
	movl	$0xCCCCCCCC, 0xB8000
	cli
	hlt

