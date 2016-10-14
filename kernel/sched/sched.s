.section .text
.global	__switch_to_context

#__switch_to_context:
__switch_to_context1:
	movl	4(%esp), %eax
	pushl	72(%eax)
	pushl	68(%eax)
	pushl	64(%eax)
	pushl	60(%eax)
	pushl	56(%eax)
	movl	%esp, 28(%eax)
	movl	%eax, %esp
	popl	%gs
	popl	%fs
	popl	%es
	popl	%ds
	popal
	movl	-20(%esp), %esp
	cli
	hlt
	iretl

#__switch_to_context:
__switch_to_context2:
	movl	4(%esp), %eax
	movl	%esp, 28(%eax)
	movl	56(%eax), %edx
	movl	%edx, ret_ptr
	movl	%eax, %esp
	addl	$16, %esp
	popal
	#addl	$16, %esp
	#popfl
	#movl	-40(%esp), %esp
	movl	-20(%esp), %esp
	jmp	*ret_ptr

__switch_to_context:
__switch_to_context3:
	movl	4(%esp), %eax
	#movl	%esp, old_esp
	movl	%eax, %esp
	movl	56(%eax), %eax
	movl	%eax, ret_ptr
	addl	$16, %esp
	popal
	movl	20(%esp), %esp
	jmp	*ret_ptr
	/*movl	8(%esp), %esp
	movl	%esp, ret_ptr
	#movl	old_esp, %esp
	#movl	$0xC03F8000, %esp
	jmp	*ret_ptr*/

	.align	4
ret_ptr:
	.long	0
old_esp:
	.long	0

