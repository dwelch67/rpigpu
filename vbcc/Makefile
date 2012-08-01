

all : blinker02.asm.loader.bin blinker03.asm.loader.bin uart02.asm.loader.bin


genasm : genasm.c diss.c
	gcc -Wall genasm.c -o genasm



blinker02.asm.c : genasm blinker02.asm
	./genasm blinker02.asm

blinker02.asm : blinker02.c
	vbccgeneric -c99 -O=32767 -gpr-args=5 blinker02.c

blinker02.asm.loader.bin: blinker02.asm.c tinyasm.c
	gcc blinker02.asm.c -o blinker02.asm.build
	./blinker02.asm.build



blinker03.asm.c : genasm blinker03.asm
	./genasm blinker03.asm

blinker03.asm : blinker03.c
	vbccgeneric -c99 -O=32767 -gpr-args=5 blinker03.c

blinker03.asm.loader.bin: blinker03.asm.c tinyasm.c
	gcc blinker03.asm.c -o blinker03.asm.build
	./blinker03.asm.build



uart02.asm.c : genasm uart02.asm
	./genasm uart02.asm

uart02.asm : uart02.c
	vbccgeneric -c99 -O=32767 -gpr-args=5 uart02.c

uart02.asm.loader.bin: uart02.asm.c tinyasm.c
	gcc uart02.asm.c -o uart02.asm.build
	./uart02.asm.build



bootloader01.asm.c : genasm bootloader01.asm
	./genasm bootloader01.asm

bootloader01.asm : bootloader01.c
	vbccgeneric -c99 -O=32767 -gpr-args=5 bootloader01.c

bootloader01.asm.loader.bin: bootloader01.asm.c tinyasm.c
	gcc bootloader01.asm.c -o bootloader01.asm.build
	./bootloader01.asm.build



clean  :
	rm -f *.asm
	rm -f *.asm.c
	rm -f *.build
	rm -f *.bin
	rm -f genasm
	
