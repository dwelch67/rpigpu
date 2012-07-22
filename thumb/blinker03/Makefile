
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

startup.o : startup.s
	$(CROSS)-as $(AOPS) startup.s -o startup.o

blinker03.o : blinker03.c
	$(CROSS)-gcc $(COPS) -c blinker03.c -o blinker03.o

blinker03.bin : memmap startup.o blinker03.o 
	$(CROSS)-ld $(LDOPS) startup.o blinker03.o -T memmap -o blinker03.elf
	$(CROSS)-objdump -D blinker03.elf > blinker03.list
	$(CROSS)-objcopy blinker03.elf -O ihex blinker03.hex
	$(CROSS)-objcopy blinker03.elf -O binary blinker03.bin


loader.bin: build.c frame.c blinker03.bin ../tiss ../doemit.c
	../tiss blinker03.bin > translate.c
	gcc build.c -o build
	./build

