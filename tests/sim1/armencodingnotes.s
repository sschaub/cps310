@@@ ARMEncoding notes examples

.global _start
.text
_start:
  mov r5, r3
  add r4, r5, #25
  add r4, r5, r6
  add r4, r5, r6, lsl #2
  add r4, r5, r6, lsl r7
  ldr r4, [r5, #25]
  str r4, [r5, r6, lsl #2]
  ldr r4, [r5, #25]!
  str r4, [r5, r6, lsl #2]!
  ldr r4, [r5], #25
  str r4, [r5], r6, lsl #2
  