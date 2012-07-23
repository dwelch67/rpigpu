
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

bootloader01.o : bootloader01.c
	$(CROSS)-gcc $(COPS) -c bootloader01.c -o bootloader01.o

bootloader01.bin : memmap startup.o bootloader01.o 
	$(CROSS)-ld $(LDOPS) startup.o bootloader01.o -T memmap -o bootloader01.elf
	$(CROSS)-objdump -D bootloader01.elf > bootloader01.list
	$(CROSS)-objcopy bootloader01.elf -O ihex bootloader01.hex
	$(CROSS)-objcopy bootloader01.elf -O binary bootloader01.bin


loader.bin: build.c frame.c bootloader01.bin ../tiss ../doemit.c
	../tiss bootloader01.bin > translate.c
	gcc build.c -o build
	./build

