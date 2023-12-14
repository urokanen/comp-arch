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
I implemented a 5-cell stack that had wires: RESET, CLK, INDEX - the cell index from the top of the stack, COMMAND - (01 - add, 10 - pop, 11 - get) DATA(input/output).

## Implementation of the memory cache
Implementation and simulation of cache memory operation together with RAM. Using the matrix multiplication program as an example, this program counted the number of cache hits with the specified system parameters:
-ADDR_LEN = 19 bit
-CACHE_WAY = 4 
-CACHE_IDX_LEN = 4 bit
-CACHE_OFFSET_LEN = 5 bit
-CACHE_SIZE = 2 kilobytes
-CACHE_SETS_COUNT = 16
```
LRU:	hit perc. 96.6571%	time: 4135335
pLRU:	hit perc. 96.6406%	time: 4140297
```
