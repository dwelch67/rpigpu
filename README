
Start here to see a possible instruction set for the Raspberry Pi GPU

https://github.com/hermanhermitage/videocoreiv/

disclaimer: I have no other information other than what is publicly
available, the information in the above repo and the Broadcom documents
at the Raspberry Pi site.  I have done my own experiments as needed to
see that the instructions on the above site execute as described on the
above site.

Based on the above information I did some experiments, the assembly was
tedious, adding instructions (to the tinyasm) as I needed them, etc.
Historically I have been a failure at making compiler backends.  Some
day I wont be.  I have had success with static binary translation though,
and that is the path I took to give myself some sort of compiler.

So what you will see here at least initially are some examples written
in C, compiled to thumb (ARM's 16 bit instruction set, original thumb,
not thumb2 extensions), the thumb binary is disassembled, as it is
disassembled raspberry pi gpu machine code is created in its place.

This is not beginner stuff in any way, this is not a generic C compiler
for the Raspberry Pi gpu.  This is for people are capable of writing
their programs in both thumb MACHINE CODE and gpu MACHINE CODE but
want to save some time.  So far every example has required more work
on the translator.  It took three clean slate starts to get this far.  I
just realized that I have been treating immediates as unsigned when they
are signed, so I have to go back through and fix a bunch of stuff.  For
the examples I have so far it does appear to work.

I have a bootloader for the gpu that I wrote using tinyasm but am not
publishing it, it greatly helps with development though.  When I get a
C based, translated, bootloader I will publish that.  To be honest the
only reason I started on this translator was to make a bootloader using
C in some form not all asm.

Understand:  The Raspberry Pi boots like this, there is what I assume
is a flash, OTP or not, in the part that the gpu boots off of, it could
be hardware and not the gpu, I have no idea, doesnt matter.  The first
file loaded is bootcode.bin, you can replace bootcode.bin with your
own, but you wont have any sdram.  I dont know how much memory is on
chip that bootcode.bin is able to boot and use, have not figured that
out yet.  The Raspberry Pi project bootcode.bin appears to initialize
sdram.  The next thing that happens is loader.bin is loaded, this is
also gpu code.  If you replace loader.bin with your program then you
will have sdram.  At least on entry to both bootcode.bin and loader.bin
the gpu is running at the crystal rate which from the schematic is 19MHz,
and experiments confirm that (dividing that clock down for the uart,
blinking leds, etc).

From the published documents the ARM's view of peripherals is from
address 0x20000000, and it shows that to be 0x7E000000 for the GPU and
that appears to work.  I have no idea how to load the ARM with the GPU
nor do I know how to change the system clock to the 250Mhz (or whatever)
that we are used to with the stock gpu firmware.

So starting with the blinkers then the uarts and eventually a bootloader
these programs are progressively helping me develop the translator.
Bootloader01 is working both with gcc and llvm as compilers.  I dont
know if it is sad or funny that when things dont work you assume it
is the translator, put time in to debugging to find that the C code
being compiled then translated has the bug (forgot to setup the rx
gpio pin).  It didnt help that I was using a bootloader written in asm
which had already setup the uart and pins correctly so the fact that
the test program didnt, wasnt standing out as a problem.

If you do nothing else check out the tinyasm/asm.c file in the above
repo.  It a mixture of pure genius and mad scientist.  I love it, I may
model all of my assemblers after that in the future.  I have taken it
and ripped out everything but the emits to create the doemit.c program
I use to aid in the translation.
