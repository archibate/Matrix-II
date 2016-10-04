include Makefile.header

BOOT	:=boot/boot.bin
IMAGE	:=build/Image
BOOT_FULL	:=$(SRC)/$(BOOT)
IMAGE_FULL	:=$(SRC)/$(IMAGE)

default : target

target : hda.img

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
	$(QKECHO) MAKE boot/ clean
	$(Q)$(MAKES) -C boot/ -r clean
	$(QKECHO) MAKE build/ clean
	$(Q)$(MAKES) -C build/ -r clean

run : hda.img
	$(QKECHO) Use QEMU emulated computer to run our kernel
	$(Q)qemu-system-i386 -m 64 -hda hda.img -boot a

