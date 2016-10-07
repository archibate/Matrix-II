typedef unsigned int size_t;
typedef unsigned char byte_t;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned int u_t;
typedef	unsigned int paddr_t, laddr_t, vaddr_t;

void _start(void);
void csetup(void);

void _start(void)
{
	csetup();
}

static inline int io_inb(int port)
{
	int __result;
	__asm__ ("inb	%%dx, %%al\n" : "=a" (__result) : "d" (port));
	return __result;
}

static inline void io_outb(int data, int port)
{
	__asm__ ("outb	%%al, %%dx\n" :: "d" (port), "a" (data));
}

static inline void io_outw(int data, int port)
{
	__asm__ ("outw	%%ax, %%dx\n" :: "d" (port), "a" (data));
}

static inline int io_insl(int port, void *buf, size_t count)
{
	__asm__ ("cld\nrep\ninsl\n" ::
			"d" (port), "D" (buf), "c" (count));
}

#define	KERNEL_DISK_OFFSET	0x4000

#define	ELF_MAGIC	0x464C457FU

struct ehdr {
	u32 magic;
	u8 eident[12];
	u16 type;
	u16 machine;
	u32 version;
	u32 entry;
	u32 phoff;
	u32 shoff;
	u32 flags;
	u16 ehsize;
	u16 phdentsize;
	u16 phnum;
	u16 shentsize;
	u16 shnum;
	u16 shstrndx;
};

struct phdr {
	u32 type;
	u32 offset;
	u32 vaddr;
	u32 paddr;
	u32 filesz;
	u32 memsz;
	u32 flags;
	u32 align;
};

void read_disk_sect(void *buf, size_t lba);
void read_disk_seg(vaddr_t addr, size_t size, size_t offset);
static inline void wait_disk();
int sprintf(char *buf, const char *fmt, ...);

void csetup(void)
{
	char *buf = (char *) 0xB8000;

	static char s[80];
	//read_disk_seg((vaddr_t) buf, 512, 0);
	//read_disk_sect(buf, 1);
	//for (;;);

	struct ehdr *elf = (struct ehdr *) 0x92000;
	read_disk_seg((vaddr_t) elf, 8 * 512, KERNEL_DISK_OFFSET);
	/* In these 8 sectors containing elf header and program header */
	if (elf->magic != ELF_MAGIC)
		goto bad;

	//(* (short *) 0xB8000) = 0x0303;

	struct phdr *phdr = (struct phdr *) (((u8 *) elf) + elf->phoff);
	struct phdr *end_phdr = phdr + elf->phnum;
	//if (elf->phnum != 1) __asm__ ("cli;hlt");
	
	for (; phdr < end_phdr; ++phdr) {
		/*sprintf(s, "$$KERNELDR: Program header No.%d: "
				"Physical address %p", 0,
				phdr->vaddr & 0x7FFFFFFF);
		for (int i = 0; i < sizeof(s); ++i)
			((short *) 0xB8000)[i] = 0x0F00 | (short) s[i];*/

		read_disk_seg(phdr->paddr, phdr->memsz,
				KERNEL_DISK_OFFSET + phdr->offset);
	}

	//sprintf(s, "phdr->vaddr = %p", phdr->vaddr);
	//sprintf(s, "elf->phoff = %p", elf->phoff);
	//for (char *p = s; *p; ++p)
	//	(* (short *) 0xB8000) = 0x0F00 | *p;

	sprintf(s, "$$BOOT-IDENT: Bootloader: Umbrella v0.0.1, "
			    "Kernel: Matrix-II v0.0.0.1");
	for (int i = 0; i < sizeof(s); ++i)
		((short *) 0xB8000)[80 + i] = 0x0F00 | (short) s[i];

	void (* entry) (void);
	entry = (void (*) (void)) (elf->entry);
	entry();

bad:
	io_outw(0x8A00, 0x8A00);
	io_outw(0x8A00, 0x8E00);
	for (;;);
	__asm__ ("cli;hlt");
}

void read_disk_sect(void *buf, size_t lba)
{
	wait_disk();
	io_outb(1, 0x1F2);
	io_outb(lba, 0x1F3);
	io_outb(lba >> 8, 0x1F4);
	io_outb(lba >> 16, 0x1F5);
	io_outb((lba >> 24) | 0xE0, 0x1F6);
	io_outb(0x20, 0x1F7);
	wait_disk();
	io_insl(0x1F0, buf, 128);
}

void read_disk_seg(vaddr_t addr, size_t size, size_t offset)
{
	size = (size + 0x1FF) & ~0x1FF;
	vaddr_t end_addr = addr + size;
	//addr &= ~0x1FF;
	size_t lba = (offset >> 9);
	addr -= offset & 0x1FF;

	for (; addr < end_addr; addr += 512, ++lba)
		read_disk_sect((void *) addr, lba);
}

static inline void wait_disk()
{
	while ((io_inb(0x1F7) & 0xC0) != 0x40);
}

