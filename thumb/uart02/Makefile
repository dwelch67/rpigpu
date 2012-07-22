
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

uart02.o : uart02.c
	$(CROSS)-gcc $(COPS) -c uart02.c -o uart02.o

uart02.bin : memmap startup.o uart02.o 
	$(CROSS)-ld $(LDOPS) startup.o uart02.o -T memmap -o uart02.elf
	$(CROSS)-objdump -D uart02.elf > uart02.list
	$(CROSS)-objcopy uart02.elf -O ihex uart02.hex
	$(CROSS)-objcopy uart02.elf -O binary uart02.bin


loader.bin: build.c frame.c uart02.bin ../tiss ../doemit.c
	../tiss uart02.bin > translate.c
	gcc build.c -o build
	./build

