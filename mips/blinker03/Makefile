
CROSS ?= mips-elf

COPS = -march=mips1 -G0 -Wall -O2 -nostdlib -nostartfiles -ffreestanding -mpatfree -mnohwdiv -mnohwmult 

AOPS = -G0 --warn --fatal-warnings

LDOPS = -G0




all : loader.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f build
	rm -f translate.c

startup.o : startup.s
	$(CROSS)-as $(AOPS) startup.s -o startup.o

blinker03.o : blinker03.c
	$(CROSS)-gcc $(COPS) -c blinker03.c -o blinker03.o

blinker03.bin : memmap startup.o blinker03.o 
	$(CROSS)-ld $(LDOPS) startup.o blinker03.o -T memmap -o blinker03.elf
	$(CROSS)-objdump -D blinker03.elf > blinker03.list
	$(CROSS)-objcopy blinker03.elf -O ihex blinker03.hex
	$(CROSS)-objcopy blinker03.elf -O binary blinker03.bin


loader.bin: build.c frame.c blinker03.bin ../miss ../doemit.c
	../miss blinker03.bin > translate.c
	gcc build.c -o build
	./build

