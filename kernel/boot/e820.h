/******************************************************************************/
/* Important Fall 2017 CSCI 402 usage information:                            */
/*                                                                            */
/* This fils is part of CSCI 402 kernel programming assignments at USC.       */
/*         53616c7465645f5f2e8d450c0c5851acd538befe33744efca0f1c4f9fb5f       */
/*         3c8feabc561a99e53d4d21951738da923cd1c7bbd11b30a1afb11172f80b       */
/*         984b1acfbbf8fae6ea57e0583d2610a618379293cb1de8e1e9d07e6287e8       */
/*         de7e82f3d48866aa2009b599e92c852f7dbf7a6e573f1c7228ca34b9f368       */
/*         faaef0c0fcf294cb                                                   */
/* Please understand that you are NOT permitted to distribute or publically   */
/*         display a copy of this file (or ANY PART of it) for any reason.    */
/* If anyone (including your prospective employer) asks you to post the code, */
/*         you must inform them that you do NOT have permissions to do so.    */
/* You are also NOT permitted to remove or alter this comment block.          */
/* If this comment block is removed or altered in a submitted file, 20 points */
/*         will be deducted.                                                  */
/******************************************************************************/

#pragma once

#define MAP_SIGNATURE 0x0534D4150
#define MAP_ENTRY_SIZE 24

.size_offset:
		.short 0
		
/* builds a memory map using the int 0x15 function 0xe820
 * BIOS interrupt, each entry is 24 bytes
 * es:di the location where the map should be stored
 * bp is a count of the number of entries + 4 bytes (for an initial count) you can store */
build_memory_map:
		mov		%di, .size_offset		/* remember where to store the size when we finish */
		add		$4, %di					/* the first 4 bytes is a count of the number of entries */
		xor		%ebx, %ebx				/* must be 0 for first call */
		xor		%bp, %bp
		
		mov		$MAP_SIGNATURE, %edx	/* must be the characters 'SMAP' */
		mov		$0xe820, %eax			/* function code for int 0x15 */
		movl	0x1, %ecx
		movl	%ecx, %es:(%di)			/* indicate we support APIC 3 entries */
		mov		$MAP_ENTRY_SIZE, %ecx	/* tell the BIOS how large the entry can be */
		int		$0x15
		jc		9f						/* if carry bit set operation failed */

		/* BIOS should set eax to 'SMAP' to indicate success */
		mov		$MAP_SIGNATURE, %edx
		cmp		%eax, %edx
		jne		9f

		/* EBX should be updated to the next location */
		test	%ebx, %ebx
		je		9f
		jmp		2f
		
1:
		mov		$MAP_SIGNATURE, %edx
		mov		$0xe820, %eax
		movl	0x1, %ecx
		movl	%ecx, %es:(%di)
		mov		$MAP_ENTRY_SIZE, %ecx
		int		$0x15
		jc		8f	/* if the carry bit is set we finished */
		
2: /* decide if the entry is in the old 20 byte format or the new 24 byte format */
		jcxz	7f		/* if the bios returned 0 bytes skip to checking if we are done */
		cmp		$20, %cl
		jbe		5f
		/*this is a new style 24-byte entry so look for the ignore flag */
		testl	$0x1, %es:20(%di)
		je		7f		/* the entry's ignore flag is set so ignore */
5: /* this handles the old style 20 byte entries, or 24-byte entries which are not ignored */
		mov		%es:8(%di), %ecx
		test	%ecx, %ecx
		jne		6f	/* the length of the region is non-zero so the entry is valid */
		mov		%es:12(%di), %ecx
		jecxz	7f	/* the length of the region is zero so skip */
6: /* we just read a valid entry so incrment count and ds */
		inc		%bp
		add		$MAP_ENTRY_SIZE, %di
7: /* if we are done then exit, otherwise read another entry */
		test	%ebx, %ebx /*if ebx = 0 we are done */
		jne		1b
8: /* done, exit */
		mov		.size_offset, %di
		mov		%bp, %es:(%di)
		ret
		
9: /* failed, exit */
		stc
		ret