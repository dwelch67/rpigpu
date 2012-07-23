
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

clang : blinker01.clang.hex blinker01.clang.bin

blinker01.clang.bc : ../blinker01.c
	clang $(LOPS) -c ../blinker01.c -o blinker01.clang.bc

blinker01.clang.opt.elf : ../memmap startup.o blinker01.clang.bc
	opt $(OOPS) blinker01.clang.bc -o blinker01.clang.opt.bc
	llc $(LLCOPS) blinker01.clang.opt.bc -o blinker01.clang.opt.s
	$(CROSS)-as blinker01.clang.opt.s -o blinker01.clang.opt.o
	$(CROSS)-ld -o blinker01.clang.opt.elf -T ../memmap startup.o blinker01.clang.opt.o
	$(CROSS)-objdump -D blinker01.clang.opt.elf > blinker01.clang.opt.list

blinker01.clang.hex : blinker01.clang.opt.elf
	$(CROSS)-objcopy blinker01.clang.opt.elf blinker01.clang.hex -O ihex

blinker01.clang.bin : blinker01.clang.opt.elf
	$(CROSS)-objcopy blinker01.clang.opt.elf blinker01.clang.bin -O binary


loader.bin: build.c frame.c blinker01.clang.bin ../../tiss ../../doemit.c
	../../tiss blinker01.clang.bin > translate.c
	gcc build.c -o build
	./build




