#include "engine_memstack.h"
int get_mem_block(int size) {
	return 0;
};
/*
31
EAX 0100 00A0
EBX 1000 1000
ECX 0000 0000
EDX 0000 01C3

31
EAX 0100 02D5
EBX 0002 0002
ECX 0000 0000
EDX 01C8 01D0

31
EAX 0100 1AA1
EBX 0100 0100
ECX 0000 0000
EDX 01D0 01D8

31
EAX 0100 1BA2
EBX 002F 002F
ECX 0000 0000
EDX 02D5 01E0

31
EAX 0100 1BD2
EBX 002F 002F
ECX 0000 0000
EDX 1AA1 01E8

--
31
EAX 0100 0008
EBX 9FC0 73F5
ECX 35513C 35513C
EDX 0020 0020

31
EAX 0100 0008
EBX 9F80 73F5
ECX 35513C 35513C
EDX 0020 0020

31
EAX 0100 0008
EBX 9F40 73F5
ECX 35513C 35513C
EDX 0020 0020

Allocate DOS Memory Block [0.9]
Allocates a block of memory from the DOS memory pool, i.e. memory below the 1 MB boundary that is controlled by DOS. Such memory blocks are typically used to exchange data with real mode programs, TSRs, or device drivers. The function returns both the real mode segment base address of the block and one or more descriptors that can be used by protected mode applications to access the block.
Call With

AX = 0100H
BX = number of (16-byte) paragraphs desired

Returns

if function successful
Carry flag = clear
AX = real mode segment base address of allocated block
DX = selector for allocated block

if function unsuccessful
Carry flag = set
AX = error code
0007H	memory control blocks damaged (also returned by DPMI 0.9 hosts)
0008H	insufficient memory (also returned by DPMI 0.9 hosts).
8011H	descriptor unavailable
BX = size of largest available block in paragraphs

Notes
If the size of the block requested is greater than 64 KB (BX > 1000H) and the client is a 16-bit program, contiguous descriptors are allocated and the base selector is returned. The consecutive selectors for the memory block can be calculated using the value returned by the Get Selector Increment Value function (Int 31H Function 0003H). Each descriptor has a limit of 64 KB, except for the last which has a limit of blocksize MOD 64 KB.
If the DPMI host is 32-bit, the client is 16-bit, and more than one descriptor is allocated, the limit of the first descriptor will be set to the size of the entire block. Subsequent descriptors have limits as described in the previous Note. 16-bit DPMI hosts will always set the limit of the first descriptor to 64 KB even when running on an 80386 (or later) machine.
When the client is 32-bit, this function always allocates only one descriptor.
Client programs should never modify or free any descriptors allocated by this function. The Free DOS Memory Block function (Int 31H Function 0101H) will deallocate the descriptors automatically.
The DOS allocation function (Int 21H Function 48H) is used.
Refer to the rules for descriptor usage in Appendix D.

*/