
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

clang : bootloader01.clang.hex bootloader01.clang.bin

bootloader01.clang.bc : ../bootloader01.c
	clang $(LOPS) -c ../bootloader01.c -o bootloader01.clang.bc

bootloader01.clang.opt.elf : ../memmap startup.o bootloader01.clang.bc
	opt $(OOPS) bootloader01.clang.bc -o bootloader01.clang.opt.bc
	llc $(LLCOPS) bootloader01.clang.opt.bc -o bootloader01.clang.opt.s
	$(CROSS)-as bootloader01.clang.opt.s -o bootloader01.clang.opt.o
	$(CROSS)-ld -o bootloader01.clang.opt.elf -T ../memmap startup.o bootloader01.clang.opt.o
	$(CROSS)-objdump -D bootloader01.clang.opt.elf > bootloader01.clang.opt.list

bootloader01.clang.hex : bootloader01.clang.opt.elf
	$(CROSS)-objcopy bootloader01.clang.opt.elf bootloader01.clang.hex -O ihex

bootloader01.clang.bin : bootloader01.clang.opt.elf
	$(CROSS)-objcopy bootloader01.clang.opt.elf bootloader01.clang.bin -O binary


loader.bin: build.c frame.c bootloader01.clang.bin ../../tiss ../../doemit.c
	../../tiss bootloader01.clang.bin > translate.c
	gcc build.c -o build
	./build




