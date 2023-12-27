# My laboratory work on the architecture of electronic computing machines

## Implementation of non-integer data types
I implemented non-integer data types using built-in integers in c++. I had a rounding type toward_infinity(type 2). 
The format of the entered numbers: h - half, f - float, A.B - fixed point. Standard mathematical operations: +, -, /, *.
```
f 2 0x414587dd * 0x42ebf110 -> 0x1.6c1b74p+10
8.8 2 0x9c9f -> -99.378
16.16 2 0x6f7600 + 0x173600 -> 134.672
f 2 0x414587dd + 0x42ebf110 -> 0x1.04a20cp+7
h 2 0x4145 * 0x42eb -> 0x1.23cp+3
f 2 0xff800000 / 0x7f800000 -> nan
```
## Stack implementation in logisim and verilog
I implemented a 5-cell stack that had wires: RESET, CLK, INDEX - the cell index from the top of the stack, COMMAND - (01 - add, 10 - pop, 11 - get), DATA(input/output). 

## Implementation of the memory cache
Implementation and simulation of cache memory operation together with RAM. Using the matrix multiplication program as an example, this program counted the number of cache hits with the specified system parameters:
```
ADDR_LEN = 19 bit
CACHE_WAY = 4 
CACHE_IDX_LEN = 4 bit
CACHE_OFFSET_LEN = 5 bit
CACHE_SIZE = 2 kilobytes
CACHE_SETS_COUNT = 16
```
And here is the result of my program:
```
LRU:	hit perc. 96.6571%	time: 4135335
pLRU:	hit perc. 96.6406%	time: 4140297
```
## Disassembler for risk-v commands
This program accepts an elf file, and decodes the .symtab section in it, as well as the .text section. In this section, my program decodes the risk-v r32 and rv32m commands
here is an example of how the elf_test file is decoded:
```
.text

00010074 	<main>:
   10074:	ff010113	   addi	sp, sp, -16
   10078:	00112623	     sw	ra, 12(sp)
   1007c:	030000ef	    jal	ra, 0x100ac <mmul>
   10080:	00c12083	     lw	ra, 12(sp)
   10084:	00000513	   addi	a0, zero, 0
   10088:	01010113	   addi	sp, sp, 16
   1008c:	00008067	   jalr	zero, 0(ra)
   10090:	00000013	   addi	zero, zero, 0
   10094:	00100137	    lui	sp, 0x100
   10098:	fddff0ef	    jal	ra, 0x10074 <main>
   1009c:	00050593	   addi	a1, a0, 0
   100a0:	00a00893	   addi	a7, zero, 10
   100a4:	0ff0000f	  fence	iorw, iorw
   100a8:	00000073	  ecall

000100ac 	<mmul>:
   100ac:	00011f37	    lui	t5, 0x11
   100b0:	124f0513	   addi	a0, t5, 292
   100b4:	65450513	   addi	a0, a0, 1620
   100b8:	124f0f13	   addi	t5, t5, 292
   100bc:	e4018293	   addi	t0, gp, -448
   100c0:	fd018f93	   addi	t6, gp, -48
   100c4:	02800e93	   addi	t4, zero, 40

000100c8 	<L2>:
   100c8:	fec50e13	   addi	t3, a0, -20
   100cc:	000f0313	   addi	t1, t5, 0
   100d0:	000f8893	   addi	a7, t6, 0
   100d4:	00000813	   addi	a6, zero, 0

000100d8 	<L1>:
   100d8:	00088693	   addi	a3, a7, 0
   100dc:	000e0793	   addi	a5, t3, 0
   100e0:	00000613	   addi	a2, zero, 0

000100e4 	<L0>:
   100e4:	00078703	     lb	a4, 0(a5)
   100e8:	00069583	     lh	a1, 0(a3)
   100ec:	00178793	   addi	a5, a5, 1
   100f0:	02868693	   addi	a3, a3, 40
   100f4:	02b70733	    mul	a4, a4, a1
   100f8:	00e60633	    add	a2, a2, a4
   100fc:	fea794e3	    bne	a5, a0, 0x100e4, <L0>
   10100:	00c32023	     sw	a2, 0(t1)
   10104:	00280813	   addi	a6, a6, 2
   10108:	00430313	   addi	t1, t1, 4
   1010c:	00288893	   addi	a7, a7, 2
   10110:	fdd814e3	    bne	a6, t4, 0x100d8, <L1>
   10114:	050f0f13	   addi	t5, t5, 80
   10118:	01478513	   addi	a0, a5, 20
   1011c:	fa5f16e3	    bne	t5, t0, 0x100c8, <L2>
   10120:	00008067	   jalr	zero, 0(ra)


.symtab

Symbol Value              Size Type     Bind     Vis       Index Name
[   0] 0x0                   0 NOTYPE   LOCAL    DEFAULT   UNDEF 
[   1] 0x10074               0 SECTION  LOCAL    DEFAULT       1 
[   2] 0x11124               0 SECTION  LOCAL    DEFAULT       2 
[   3] 0x0                   0 SECTION  LOCAL    DEFAULT       3 
[   4] 0x0                   0 SECTION  LOCAL    DEFAULT       4 
[   5] 0x0                   0 FILE     LOCAL    DEFAULT     ABS test.c
[   6] 0x11924               0 NOTYPE   GLOBAL   DEFAULT     ABS __global_pointer$
[   7] 0x118F4             800 OBJECT   GLOBAL   DEFAULT       2 b
[   8] 0x11124               0 NOTYPE   GLOBAL   DEFAULT       1 __SDATA_BEGIN__
[   9] 0x100AC             120 FUNC     GLOBAL   DEFAULT       1 mmul
[  10] 0x0                   0 NOTYPE   GLOBAL   DEFAULT   UNDEF _start
[  11] 0x11124            1600 OBJECT   GLOBAL   DEFAULT       2 c
[  12] 0x11C14               0 NOTYPE   GLOBAL   DEFAULT       2 __BSS_END__
[  13] 0x11124               0 NOTYPE   GLOBAL   DEFAULT       2 __bss_start
[  14] 0x10074              28 FUNC     GLOBAL   DEFAULT       1 main
[  15] 0x11124               0 NOTYPE   GLOBAL   DEFAULT       1 __DATA_BEGIN__
[  16] 0x11124               0 NOTYPE   GLOBAL   DEFAULT       1 _edata
[  17] 0x11C14               0 NOTYPE   GLOBAL   DEFAULT       2 _end
[  18] 0x11764             400 OBJECT   GLOBAL   DEFAULT       2 a
```
