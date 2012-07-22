
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

blinker02.o : blinker02.c
	$(CROSS)-gcc $(COPS) -c blinker02.c -o blinker02.o

blinker02.bin : memmap startup.o blinker02.o 
	$(CROSS)-ld $(LDOPS) startup.o blinker02.o -T memmap -o blinker02.elf
	$(CROSS)-objdump -D blinker02.elf > blinker02.list
	$(CROSS)-objcopy blinker02.elf -O ihex blinker02.hex
	$(CROSS)-objcopy blinker02.elf -O binary blinker02.bin


loader.bin: build.c frame.c blinker02.bin ../tiss ../doemit.c
	../tiss blinker02.bin > translate.c
	gcc build.c -o build
	./build

