include Makefile.header

BOOT	:=boot/boot.bin
IMAGE	:=build/Image

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
	$(Q)#qemu-system-i386 -m 64 -hda hda.img -boot a
	$(Q)qemu-system-i386 -m 64 -hda hda.img -boot a > /dev/null 2>&1 &
	$(Q)sleep 0.1

debug : hda.img
	$(QKECHO) Use QEMU emulated computer to run our kernel
	$(Q)qemu-system-i386 -m 64 -hda hda.img -boot a -S -s > /dev/null 2>&1 &
	$(Q)sleep 0.1
	$(QKECHO) Use CGDB to debug our kernel
	$(Q)cgdb -x gdbinit

