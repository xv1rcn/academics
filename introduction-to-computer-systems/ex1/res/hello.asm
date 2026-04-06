
hello:	file format mach-o arm64

Disassembly of section __TEXT,__text:

0000000100000460 <_main>:
100000460: a9bf7bfd    	stp	x29, x30, [sp, #-0x10]!
100000464: 910003fd    	mov	x29, sp
100000468: 90000000    	adrp	x0, 0x100000000 <_printf+0x100000000>
10000046c: 91122000    	add	x0, x0, #0x488
100000470: 94000003    	bl	0x10000047c <_printf+0x10000047c>
100000474: a8c17bfd    	ldp	x29, x30, [sp], #0x10
100000478: d65f03c0    	ret

Disassembly of section __TEXT,__stubs:

000000010000047c <__stubs>:
10000047c: 90000030    	adrp	x16, 0x100004000 <_printf+0x100004000>
100000480: f9400210    	ldr	x16, [x16]
100000484: d61f0200    	br	x16
