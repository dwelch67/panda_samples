
/*startup.s*/
.code 32

.globl _start
_start:
    b reset
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang

hang:   b hang

reset:
    ldr r0,=thumb_start
    bx r0  /* branch and switch from ARM to thumb mode */

/* ------- ARM ABOVE, THUMB BELOW ------- */

.thumb

.thumb_func
thumb_start:
    ldr r0,=0x40002000
    mov sp,r0

    bl notmain
    b .


.thumb_func
.global dummy
dummy:
    bx lr

.thumb_func
.global PUT32
PUT32:
    str r1,[r0]
    bx lr

.thumb_func
.global GET32
GET32:
    ldr r0,[r0]
    bx lr
