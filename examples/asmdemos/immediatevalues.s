@ Sums an array of numbers
.text
.global _start
_start:
   mov r0,#1
   mov r0,#255
   mov r0,#256
   mov r0,#0x3fc
   @ mov r0,#257  @ 257 not legal
   
   @ load a large constant with ldr
   @ assembler puts constant in "constant pool" in code segment
   @ and loads it from memory
   ldr r0, =0x05
   ldr r0, =0x01234567
   swi #0x11
   
.end
