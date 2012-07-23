
CROSS ?= arm-none-eabi

COPS = -mthumb -Wall -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = -mthumb --warn --fatal-warnings

LDOPS = 


all : loader.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f build
	rm -f translate.c
	rm -f *.bc
	rm -f *.clang.opt.s

startup.o : ../startup.s
	$(CROSS)-as $(AOPS) ../startup.s -o startup.o


LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

clang : uart03.clang.hex uart03.clang.bin

uart03.clang.bc : ../uart03.c
	clang $(LOPS) -c ../uart03.c -o uart03.clang.bc

uart03.clang.opt.elf : ../memmap startup.o uart03.clang.bc
	opt $(OOPS) uart03.clang.bc -o uart03.clang.opt.bc
	llc $(LLCOPS) uart03.clang.opt.bc -o uart03.clang.opt.s
	$(CROSS)-as uart03.clang.opt.s -o uart03.clang.opt.o
	$(CROSS)-ld -o uart03.clang.opt.elf -T ../memmap startup.o uart03.clang.opt.o
	$(CROSS)-objdump -D uart03.clang.opt.elf > uart03.clang.opt.list

uart03.clang.hex : uart03.clang.opt.elf
	$(CROSS)-objcopy uart03.clang.opt.elf uart03.clang.hex -O ihex

uart03.clang.bin : uart03.clang.opt.elf
	$(CROSS)-objcopy uart03.clang.opt.elf uart03.clang.bin -O binary


loader.bin: build.c frame.c uart03.clang.bin ../../tiss ../../doemit.c
	../../tiss uart03.clang.bin > translate.c
	gcc build.c -o build
	./build




