include Makefile.header

default : target

include Makefile.compile

target : Image

Image : $(SRC)/
	$(QKECHO) MAKE ../kernel/ target
	$(Q)$(MAKES) -C ../kernel/ -r target
	$(QKECHO) Generating main kernel Image...
	$(Q)cp ../kernel/kernel.ko $@
	$(QKECHO) Generate main kernel Image done

clean :
	$(Q)$(MAKES) -C ../kernel/ -r clean

