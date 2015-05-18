
CABBAGE_DIR=../../

include $(CABBAGE_DIR)/makefiles/build.defs

CFLAGS += 
CPPFLAGS=-I$(CABBAGE_DIR)/hw -I$(CABBAGE_DIR)/include
LDFLAGS=-L$(CABBAGE_DIR)/lib/ldscripts -L$(CABBAGE_DIR)/lib -L$(CABBAGE_DIR)/hw 

%.bin: %.elf
	$(OBJCPY)

%.elf: %.o
	$(LD) $(LDFLAGS) -Tstm32vldiscovery.ld -o $@ $^
%.o: %.c
	$(CC) -nostdlib -Wl,--relocatable $(CPPFLAGS) $(LDFLAGS) $(CFLAGS) -Tcabbage.ld  -o $@ $^

.PHONY: clean
clean:
	rm *.o

.DEFAULT_GOAL :=

