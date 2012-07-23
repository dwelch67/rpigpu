
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

uart03.o : uart03.c
	$(CROSS)-gcc $(COPS) -c uart03.c -o uart03.o

uart03.bin : memmap startup.o uart03.o 
	$(CROSS)-ld $(LDOPS) startup.o uart03.o -T memmap -o uart03.elf
	$(CROSS)-objdump -D uart03.elf > uart03.list
	$(CROSS)-objcopy uart03.elf -O ihex uart03.hex
	$(CROSS)-objcopy uart03.elf -O binary uart03.bin


loader.bin: build.c frame.c uart03.bin ../tiss ../doemit.c
	../tiss uart03.bin > translate.c
	gcc build.c -o build
	./build

