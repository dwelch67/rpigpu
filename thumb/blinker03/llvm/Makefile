
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

clang : blinker03.clang.hex blinker03.clang.bin

blinker03.clang.bc : ../blinker03.c
	clang $(LOPS) -c ../blinker03.c -o blinker03.clang.bc

blinker03.clang.opt.elf : ../memmap startup.o blinker03.clang.bc
	opt $(OOPS) blinker03.clang.bc -o blinker03.clang.opt.bc
	llc $(LLCOPS) blinker03.clang.opt.bc -o blinker03.clang.opt.s
	$(CROSS)-as blinker03.clang.opt.s -o blinker03.clang.opt.o
	$(CROSS)-ld -o blinker03.clang.opt.elf -T ../memmap startup.o blinker03.clang.opt.o
	$(CROSS)-objdump -D blinker03.clang.opt.elf > blinker03.clang.opt.list

blinker03.clang.hex : blinker03.clang.opt.elf
	$(CROSS)-objcopy blinker03.clang.opt.elf blinker03.clang.hex -O ihex

blinker03.clang.bin : blinker03.clang.opt.elf
	$(CROSS)-objcopy blinker03.clang.opt.elf blinker03.clang.bin -O binary


loader.bin: build.c frame.c blinker03.clang.bin ../../tiss ../../doemit.c
	../../tiss blinker03.clang.bin > translate.c
	gcc build.c -o build
	./build




