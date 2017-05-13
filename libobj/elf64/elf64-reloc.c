/*
	Madd Development Tools

	Copyright (c) 2017, Madd Games.
	All rights reserved.
	
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:
	
	* Redistributions of source code must retain the above copyright notice, this
	  list of conditions and the following disclaimer.
	
	* Redistributions in binary form must reproduce the above copyright notice,
	  this list of conditions and the following disclaimer in the documentation
	  and/or other materials provided with the distribution.
	
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "elf64.h"

typedef struct
{
	int				size;
	int				type;
	uint64_t			elftype;
} RelocMapping;

static RelocMapping relocTypes[] = {
#if ELF_MACHINE_NUMBER == 62					/* x86_64 */
	{REL_BYTE,  REL_X86_ABSOLUTE,	14},			/* R_X86_64_8 */
	{REL_WORD,  REL_X86_ABSOLUTE,	12},			/* R_X86_64_16 */
	{REL_DWORD, REL_X86_ABSOLUTE,	10},			/* R_X86_64_32 */
	{REL_QWORD, REL_X86_ABSOLUTE,	1},			/* R_X86_64_64 */
	
	{REL_BYTE,  REL_X86_RELATIVE,	15},			/* R_X86_64_PC8 */
	{REL_WORD,  REL_X86_RELATIVE,	13},			/* R_X86_64_PC16 */
	{REL_DWORD, REL_X86_RELATIVE,	11},			/* R_X86_64_32S */
#endif
	{-1, -1, 0}						/* list terminator */
};

uint64_t elfGetReloc(int size, int type)
{
	RelocMapping *scan;
	for (scan=relocTypes; scan->size!=-1; scan++)
	{
		if ((scan->size == size) && (scan->type == type)) return scan->elftype;
	};
	
	return 0;	/* "none" */
};
