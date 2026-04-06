
hello.o:	file format mach-o arm64

Disassembly of section __TEXT,__text:

0000000000000000 <ltmp0>:
       0: a9bf7bfd     	stp	x29, x30, [sp, #-0x10]!
       4: 910003fd     	mov	x29, sp
       8: 90000000     	adrp	x0, 0x0 <ltmp0>
       c: 91000000     	add	x0, x0, #0x0
      10: 94000000     	bl	0x10 <ltmp0+0x10>
      14: a8c17bfd     	ldp	x29, x30, [sp], #0x10
      18: d65f03c0     	ret
