

all : blinker02.asm.c

blinker02.asm.c : genasm blinker02.asm
	./genasm blinker02.asm


blinker02.asm : blinker02.c
	vbccgeneric -c99 -O=32767 -gpr-args=5 blinker02.c

genasm : genasm.c
	gcc -Wall genasm.c -o genasm


clean  :
	rm -f *.asm
	rm -f *.asm.c
	rm -f genasm
	
