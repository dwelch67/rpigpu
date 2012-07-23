
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

clang : uart02.clang.hex uart02.clang.bin

uart02.clang.bc : ../uart02.c
	clang $(LOPS) -c ../uart02.c -o uart02.clang.bc

uart02.clang.opt.elf : ../memmap startup.o uart02.clang.bc
	opt $(OOPS) uart02.clang.bc -o uart02.clang.opt.bc
	llc $(LLCOPS) uart02.clang.opt.bc -o uart02.clang.opt.s
	$(CROSS)-as uart02.clang.opt.s -o uart02.clang.opt.o
	$(CROSS)-ld -o uart02.clang.opt.elf -T ../memmap startup.o uart02.clang.opt.o
	$(CROSS)-objdump -D uart02.clang.opt.elf > uart02.clang.opt.list

uart02.clang.hex : uart02.clang.opt.elf
	$(CROSS)-objcopy uart02.clang.opt.elf uart02.clang.hex -O ihex

uart02.clang.bin : uart02.clang.opt.elf
	$(CROSS)-objcopy uart02.clang.opt.elf uart02.clang.bin -O binary


loader.bin: build.c frame.c uart02.clang.bin ../../tiss ../../doemit.c
	../../tiss uart02.clang.bin > translate.c
	gcc build.c -o build
	./build




