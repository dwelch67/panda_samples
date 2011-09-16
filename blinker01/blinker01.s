
/*blinker01.s*/

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
    ldr sp,=0x40002000

    ldr r0,=0xE01FC1A0
    ldr r1,[r0]
    bic r1,#1
    str r1,[r0]

    ldr r0,=0xE0028000 /* I/O port 0 legacy base address */


    mov r1,#0x00010000 /* bit 16 */

    /* make P0.15 an output, 0xE0028008 */
    ldr r2,[r0,#0x08]
    orr r2,r2,r1
    str r2,[r0,#0x08]
    /* blink led */

loop:
    str r1,[r0,#0x04] /* IO0SET 0xE0028004 */
    mov r2,#0x40000
loop0:
    subs r2,r2,#1
    bne loop0

    str r1,[r0,#0x0C] /* IO0CLR 0xE002800C */
    mov r2,#0x40000
loop1:
    subs r2,r2,#1
    bne loop1

    b loop
