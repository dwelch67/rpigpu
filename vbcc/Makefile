

all : blinker02.loader.bin

blinker02.asm.c : genasm blinker02.asm
	./genasm blinker02.asm


blinker02.asm : blinker02.c
	vbccgeneric -c99 -O=32767 -gpr-args=5 blinker02.c

genasm : genasm.c diss.c
	gcc -Wall genasm.c -o genasm

blinker02.loader.bin: blinker02.asm.c tinyasm.c
	gcc blinker02.asm.c -o blinker02.asm.build
	./blinker02.asm.build




clean  :
	rm -f *.asm
	rm -f *.asm.c
	rm -f *.build
	rm -f *.bin
	rm -f genasm
	
