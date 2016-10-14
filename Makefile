include Makefile.header

BOOT		:=boot/boot.bin
IMAGE		:=build/Image
GRUB_FLOPPY	:=grub-floppy
BOOT_DISK	:=fd

default : target

target : $(BOOT_DISK)a.img

all : fda.img hda.img

fda.img : $(GRUB_FLOPPY) $(IMAGE)
	$(QKECHO) Creating floppy image '$@'...
	cp $< $@
	sudo mount $@ /mnt/img/
	sudo cp $(IMAGE) /mnt/img/boot/
	sudo umount /mnt/img/
	$(QKECHO) Create floppy image '$@' done

hda.img : $(BOOT) $(IMAGE)
	$(QKECHO) Creating harddisk image '$@'...
	$(Q)$(EDIMG) $@ 10485760 $(BOOT) 0x1000 0 $(IMAGE) 0x4000 0x4000
	$(QKECHO) Create harddisk image '$@' done

$(BOOT) : boot/
	$(QKECHO) MAKE boot/ target
	$(Q)$(MAKES) -C boot/ -r target

$(IMAGE) : $(SRC)/
	$(QKECHO) MAKE build/ target
	$(Q)$(MAKES) -C build/ -r target

clean :
	$(QKECHO) CLEAN $(WD)
	$(Q)$(RM) hda.img
	$(Q)$(RM) fda.img
	$(QKECHO) MAKE boot/ clean
	$(Q)$(MAKES) -C boot/ -r clean
	$(QKECHO) MAKE build/ clean
	$(Q)$(MAKES) -C build/ -r clean

run-hd : hda.img
	$(QKECHO) Use QEMU emulated computer to run our kernel \
		"(boot with harddisk)"
	$(Q)#qemu-system-i386 -m 64 -hda hda.img -boot c
	$(Q)qemu-system-i386 -m 64 -hda hda.img -boot c > /dev/null 2>&1 &
	$(Q)sleep 0.1

debug-hd : hda.img
	$(QKECHO) Use QEMU emulated computer to run our kernel \
		"(boot with harddisk)"
	$(Q)qemu-system-i386 -m 64 -hda hda.img -boot c -S -s > /dev/null 2>&1 &
	$(Q)sleep 0.1
	$(QKECHO) Use CGDB to debug our kernel
	$(Q)cgdb -x gdbinit

run-fd : fda.img
	$(QKECHO) Use QEMU emulated computer to run our kernel \
		"(boot with floppy)"
	$(Q)qemu-system-i386 -m 64 -fda fda.img -boot a > /dev/null 2>&1 &
	$(Q)sleep 0.1

debug-fd : fda.img
	$(QKECHO) Use QEMU emulated computer to run our kernel \
		"(boot with floppy)"
	$(Q)qemu-system-i386 -m 64 -fda fda.img -boot a -S -s > /dev/null 2>&1 &
	$(Q)sleep 0.1
	$(QKECHO) Use CGDB to debug our kernel
	$(Q)cgdb -x gdbinit

run : run-$(BOOT_DISK)

debug : debug-$(BOOT_DISK)

