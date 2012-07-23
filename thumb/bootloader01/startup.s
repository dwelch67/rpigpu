
.thumb



.thumb_func
.globl _start
_start:
    bl notmain
    b hang

.thumb_func
hang:   b .

.thumb_func
.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.thumb_func
.globl GET32
GET32:
    ldr r0,[r0]
    bx lr


.thumb_func
.globl PUT8
PUT8:
    strb r1,[r0]
    bx lr


.thumb_func
.globl dummy
dummy:
    bx lr

.thumb_func
.globl BRANCHTO
BRANCHTO:
    mov sp,r1
    bx r0
