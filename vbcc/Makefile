

all : blinker02.asm.loader.bin blinker03.asm.loader.bin


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





clean  :
	rm -f *.asm
	rm -f *.asm.c
	rm -f *.build
	rm -f *.bin
	rm -f genasm
	
