
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

clang : uart01.clang.hex uart01.clang.bin

uart01.clang.bc : ../uart01.c
	clang $(LOPS) -c ../uart01.c -o uart01.clang.bc

uart01.clang.opt.elf : ../memmap startup.o uart01.clang.bc
	opt $(OOPS) uart01.clang.bc -o uart01.clang.opt.bc
	llc $(LLCOPS) uart01.clang.opt.bc -o uart01.clang.opt.s
	$(CROSS)-as uart01.clang.opt.s -o uart01.clang.opt.o
	$(CROSS)-ld -o uart01.clang.opt.elf -T ../memmap startup.o uart01.clang.opt.o
	$(CROSS)-objdump -D uart01.clang.opt.elf > uart01.clang.opt.list

uart01.clang.hex : uart01.clang.opt.elf
	$(CROSS)-objcopy uart01.clang.opt.elf uart01.clang.hex -O ihex

uart01.clang.bin : uart01.clang.opt.elf
	$(CROSS)-objcopy uart01.clang.opt.elf uart01.clang.bin -O binary


loader.bin: build.c frame.c uart01.clang.bin ../../tiss ../../doemit.c
	../../tiss uart01.clang.bin > translate.c
	gcc build.c -o build
	./build




