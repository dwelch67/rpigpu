
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

clang : blinker02.clang.hex blinker02.clang.bin

blinker02.clang.bc : ../blinker02.c
	clang $(LOPS) -c ../blinker02.c -o blinker02.clang.bc

blinker02.clang.opt.elf : ../memmap startup.o blinker02.clang.bc
	opt $(OOPS) blinker02.clang.bc -o blinker02.clang.opt.bc
	llc $(LLCOPS) blinker02.clang.opt.bc -o blinker02.clang.opt.s
	$(CROSS)-as blinker02.clang.opt.s -o blinker02.clang.opt.o
	$(CROSS)-ld -o blinker02.clang.opt.elf -T ../memmap startup.o blinker02.clang.opt.o
	$(CROSS)-objdump -D blinker02.clang.opt.elf > blinker02.clang.opt.list

blinker02.clang.hex : blinker02.clang.opt.elf
	$(CROSS)-objcopy blinker02.clang.opt.elf blinker02.clang.hex -O ihex

blinker02.clang.bin : blinker02.clang.opt.elf
	$(CROSS)-objcopy blinker02.clang.opt.elf blinker02.clang.bin -O binary


loader.bin: build.c frame.c blinker02.clang.bin ../../tiss ../../doemit.c
	../../tiss blinker02.clang.bin > translate.c
	gcc build.c -o build
	./build




