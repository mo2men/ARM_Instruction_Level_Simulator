mov r1, #0xff
add r2, r1, #0xff00
mov r3, #0xff
adds r3, r3, r3, LSL #8
mov r9,r3, RRX
subs r0,r3,r2
mov r4,#0xff000000
adds r5, r4,#0x80000000 
subs r6,r4,r5

swi #10