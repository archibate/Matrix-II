#include <kernel/kernel.h>
#include <unistd.h>
#include <kernel/init/init.h>
#include <kernel/sched/sched.h>
#include <kernel/mm/mm.h>
#include <lib/ascommon.h>

struct multiboot {
	u32 flags;	/* Multiboot 的版本信息 */
	u32 mem_lower;	/* 低端内存的大小 */
	u32 mem_upper;	/* 高端内存的大小 */
	u32 boot_device;
	u32 cmdline;
	u32 mods_count;
	u32 mods_addr;
	u32 num;
	u32 size;
	u32 addr;
	u32 shndx;
	u32 mmap_length;
	u32 mmap_addr;
	u32 drives_length;
	u32 drives_addr;
	u32 config_table;
	u32 bootloader_name;
	u32 apm_table;
	u32 vbe_control_info;
	u32 vbe_mode_info;
	u32 vbe_mode;
	u32 vbe_interface_seg;
	u32 vbe_interface_off;
	u32 vbe_interface_len;
} __attribute__((packed));
struct mmap_entry {
	u32 size;
	u32 base_addr_low;
	u32 base_addr_high;
	u32 length_low;
	u32 length_high;
	u32 type;
} __attribute__((packed));

struct multiboot *multiboot_ptr = 0;

int init()
{
	//assert(multiboot_ptr);
	if (multiboot_ptr) {
		//struct mmap_entry *mmap = (void *) multiboot_ptr->mmap_addr;
		//size_t mmap_length = multiboot_ptr->mmap_length;
	}
	mm_init();
	sched_init();
	as_sti();
	__asm__ ("int $0x30");
	for (;;)
		as_hlt();
	return 0;
}

