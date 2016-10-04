
default : hda.img

hda.img : boot/boot.bin kernel/kernel.ko
	tools/edimg $@ 10485760 boot/boot.bin 0x1000 0 kernel/kernel.ko 0x4000 0x4000

boot/boot.bin : boot/
	make -C $< -r target

kernel/kernel.ko : kernel/
	make -C $< -r target

clean :
	rm hda.img
	make -C boot/ -r clean
	make -C kernel/ -r clean

