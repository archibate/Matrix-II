include Makefile.header

MOD_NAME	=kernel
MOD_SRCS	=init/head.s init/entry.c init/init.c \
		 mm/mm.c mm/pmm.c mm/vmm.c mm/heap.c \
		 srv/isr.s srv/isr.c \
		 sched/sched.c sched/sched.s
KERNEL_LIB_DIR	=$(SRC)/lib
KERNEL_LIB	=$(KERNEL_LIB_DIR)/lib.a
MOD_EXTRA_OBJS	=$(KERNEL_LIB)
MOD_TYPE	=ko

include Makefile.build

$(KERNEL_LIB) : $(KERNEL_LIB_DIR)
	$(QKECHO) MAKE $(KERNEL_LIB_DIR) target
	$(Q)$(MAKES) -C $(KERNEL_LIB_DIR) -r target

