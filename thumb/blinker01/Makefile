
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

blinker01.o : blinker01.c
	$(CROSS)-gcc $(COPS) -c blinker01.c -o blinker01.o

blinker01.bin : memmap startup.o blinker01.o 
	$(CROSS)-ld $(LDOPS) startup.o blinker01.o -T memmap -o blinker01.elf
	$(CROSS)-objdump -D blinker01.elf > blinker01.list
	$(CROSS)-objcopy blinker01.elf -O ihex blinker01.hex
	$(CROSS)-objcopy blinker01.elf -O binary blinker01.bin


loader.bin: build.c frame.c blinker01.bin ../tiss ../doemit.c
	../tiss blinker01.bin > translate.c
	gcc build.c -o build
	./build

