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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "arch.h"

typedef struct
{
	const char*			name8;
	const char*			name16;
	const char*			name32;
	const char*			name64;
} X86_64_RegInfo;

/**
 * Specifies a macroregister location; type is either LOC_GPR,
 * in which case 'offset' is one of the register macros defined below,
 * or LOC_STACK, in which case 'offset' is the offset from %rsp,
 * or LOC_XMM, in which case 'offset' is an XMM register number,
 * or LOC_FRAME, in which case 'offset' is the offset from %rbp.
 */
#define	LOC_GPR				0
#define	LOC_STACK			1
#define	LOC_XMM				2
#define	LOC_FRAME			3
typedef struct
{
	int				type;
	int				offset;
} X86_64_RegLoc;

/**
 * Register allocator. "gpr" is a bitmap of registers (indexed by the macros below),
 * where a set bit indicates the corresponding register is in use. "xmm" is the same
 * but indexed by XMM register numbers.
 */
typedef struct
{
	uint16_t			gpr;
	uint16_t			xmm;
	int				frameSize;
} X86_64_RegAlloc;

enum {RAX, RBX, RCX, RDX, RSI, RDI, R8, R9, R10, R11, R12, R13, R14, R15};

X86_64_RegInfo reginfo[] = {
	{"%al",		"%ax",		"%eax",		"%rax"},
	{"%bl",		"%bx",		"%ebx",		"%rbx"},
	{"%cl",		"%cx",		"%ecx",		"%rcx"},
	{"%dl",		"%dx",		"%edx",		"%rdx"},
	{"%sil",	"%si",		"%esi",		"%rsi"},
	{"%dil",	"%di",		"%edi",		"%rdi"},
	{"%r8b",	"%r8w",		"%r8d",		"%r8"},
	{"%r9b",	"%r9w",		"%r9d",		"%r9"},
	{"%r10b",	"%r10w",	"%r10d",	"%r10"},
	{"%r11b",	"%r11w",	"%r11d",	"%r11"},
	{"%r12b",	"%r12w",	"%r12d",	"%r12"},
	{"%r13b",	"%r13w",	"%r13d",	"%r13"},
	{"%r14b",	"%r14w",	"%r14d",	"%r14"},
	{"%t15b",	"%r15w",	"%r15d",	"%r15"}
};

X86_64_Flags compFlags;
FlagSpec X86_64_FlagSpecs[] = {
	{"mno-red-zone",		&compFlags.mno_red_zone,		1},
	{"fomit-frame-pointer",		&compFlags.fno_omit_frame_pointer,	0},
	{"fno-omit-frame-pointer",	&compFlags.fno_omit_frame_pointer,	1},
	{NULL, NULL, 0}
};

static int nextSymbolIndex;

int X86_64_AllocReg(uint16_t type, X86_64_RegLoc *loc, X86_64_RegAlloc *state, const char *conv, int flags)
{
	if (strcmp(conv, "sysv") == 0)
	{
		if ((type == REGTYPE_F) || (type == REGTYPE_FD))
		{
			if (flags & PROC_LEAF)
			{
				int i;
				for (i=0; i<16; i++)
				{
					if ((state->xmm & (1 << i)) == 0)
					{
						state->xmm |= (1 << i);
						loc->type = LOC_XMM;
						loc->offset = i;
						return 0;
					};
				};
			};
			
			int size;
			if (type == REGTYPE_F) size = 4;
			else size = 8;
			
			state->frameSize += size;
			loc->type = LOC_STACK;
			loc->offset = -state->frameSize;
			return 0;
		}
		else
		{
			// not R10 (it is always marked used, because it's a "temporary register")
			static int volregs[] = {RDI, RSI, RDX, RCX, RAX, R8, R9, R11, -1};
			static int savedregs[] = {RBX, R12, R13, R14, R15, -1};
			
			// for leaf functions, prefer volatile registers before preserved, because we don't
			// have to save them in any way.
			// however, for non-leaf functions, we have to save the volatile registers on every
			// function call, while the saved registers only need to saved once in the prolog;
			// so we try using saved registers first
			int *tryFirst, *trySecond;
			if (flags & PROC_LEAF)
			{
				tryFirst = volregs;
				trySecond = savedregs;
			}
			else
			{
				tryFirst = savedregs;
				trySecond = volregs;
			};
			
			int *ptr;
			for (ptr=tryFirst; *ptr!=-1; ptr++)
			{
				int reg = *ptr;
				if ((state->gpr & (1 << reg)) == 0)
				{
					state->gpr |= (1 << reg);
					loc->type = LOC_GPR;
					loc->offset = reg;
					return 0;
				};
			};
			
			for (ptr=trySecond; *ptr!=-1; ptr++)
			{
				int reg = *ptr;
				if ((state->gpr & (1 << reg)) == 0)
				{
					state->gpr |= (1 << reg);
					loc->type = LOC_GPR;
					loc->offset = reg;
					return 0;
				};
			};
			
			// if there are no more free registers, use the frame
			int size;
			switch (type)
			{
			case REGTYPE_IB:
			case REGTYPE_UB:
				size = 1;
				break;
			case REGTYPE_IW:
			case REGTYPE_UW:
				size = 2;
				break;
			case REGTYPE_ID:
			case REGTYPE_UD:
				size = 4;
				break;
			case REGTYPE_IQ:
			case REGTYPE_UQ:
				size = 8;
				break;
			};
			
			state->frameSize -= size;
			loc->type = LOC_FRAME;
			loc->offset = -state->frameSize;
			return 0;
		};
	}
	else
	{
		return -1;
	};
};

const char* X86_64_GprToName(uint16_t type, int gpr)
{
	X86_64_RegInfo *info = &reginfo[gpr];
	
	switch (type)
	{
	case REGTYPE_IB:
	case REGTYPE_UB:
		return info->name8;
	case REGTYPE_IW:
	case REGTYPE_UW:
		return info->name16;
	case REGTYPE_ID:
	case REGTYPE_UD:
		return info->name32;
	case REGTYPE_IQ:
	case REGTYPE_UQ:
		return info->name64;
	default:
		return "failure";
	};
};

void X86_64_Print(uint16_t type, const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	
	for (; *format!=0; format++)
	{
		if (*format == '#')
		{
			X86_64_RegLoc *loc = va_arg(ap, X86_64_RegLoc*);
			
			if (loc->type == LOC_GPR)
			{
				printf("%s", X86_64_GprToName(type, loc->offset));
			}
			else if (loc->type == LOC_STACK)
			{
				printf("%d(%%rsp)", loc->offset);
			}
			else if (loc->type == LOC_XMM)
			{
				printf("%%xmm%d", loc->offset);
			}
			else if (loc->type == LOC_FRAME)
			{
				printf("%d(%%rbp)", loc->offset);
			};
		}
		else
		{
			putc(*format, stdout);
		};
	};
	
	va_end(ap);
};

void X86_64_Expand(const char *progName, Proc *proc)
{
	printf(".type %s, @function\n", proc->name);
	printf("%s:\n", proc->name);
	
	// count how many of each register we need (highest used index + 1)
	uint16_t countRegs[REGTYPE_NUM];
	memset(countRegs, 0, sizeof(countRegs));
	
	Macro *mac;
	for (mac=proc->first; mac!=NULL; mac=mac->next)
	{
		int i;
		for (i=0; i<mac->numOps; i++)
		{
			if (mac->ops[i].type == OPR_REG)
			{
				uint16_t type = REGSPEC_TYPE(mac->ops[i].reg.spec);
				uint16_t index = REGSPEC_INDEX(mac->ops[i].reg.spec);
				
				if (countRegs[type] < (index+1))
				{
					countRegs[type] = index + 1;
				};
			};
		};
	};
	
	int i;
	for (i=0; i<proc->numArgs; i++)
	{
		uint16_t type = REGSPEC_TYPE(proc->argRegs[i]);
		uint16_t index = REGSPEC_INDEX(proc->argRegs[i]);
		
		if (countRegs[type] < (index+1))
		{
			countRegs[type] = index + 1;
		};
	};
	
	// prepare register locations
	X86_64_RegLoc* regLocs[REGTYPE_NUM];
	
	for (i=0; i<REGTYPE_NUM; i++)
	{
		regLocs[i] = (X86_64_RegLoc*) malloc(sizeof(X86_64_RegLoc) * countRegs[i]);
		
		uint16_t j;
		for (j=0; j<countRegs[i]; j++)
		{
			regLocs[i][j].type = -1;	// not allocated
		};
	};
	
	// initialize register allocations
	X86_64_RegAlloc regState = {0, 0, 0};
	
	// specify where the arguments registers are based on calling convention, and decide a "temporary register"
	// used for moving data from memory when needed.
	int tempGPR, tempXMM;
	if (strcmp(proc->conv, "sysv") == 0)
	{
		static const int intRegs[6] = {RDI, RSI, RDX, RCX, R8, R9};
		tempGPR = R10;
		regState.gpr |= (1 << R10);
		tempXMM = 9;
		regState.xmm |= (1 << 9);
		
		int numIntRegs = 0;
		int numFloatRegs = 0;
		int frameOffset = 16;	/* first stack argument is at 16(%rbp) */
		
		for (i=0; i<proc->numArgs; i++)
		{
			uint16_t type = REGSPEC_TYPE(proc->argRegs[i]);
			uint16_t index = REGSPEC_INDEX(proc->argRegs[i]);
			
			if ((type == REGTYPE_F) || (type == REGTYPE_FD))
			{
				if (numFloatRegs < 8)
				{
					regState.xmm |= (1 << numFloatRegs);
					regLocs[type][index].type = LOC_XMM;
					regLocs[type][index].offset = numFloatRegs++;
				}
				else
				{
					regLocs[type][index].type = LOC_FRAME;
					regLocs[type][index].offset = frameOffset;
					frameOffset += 8;
				};
			}
			else
			{
				if (numIntRegs < 6)
				{
					int reg = intRegs[numIntRegs++];
					regState.gpr |= (1 << reg);
					
					regLocs[type][index].type = LOC_GPR;
					regLocs[type][index].offset = reg;
				}
				else
				{
					regLocs[type][index].type = LOC_FRAME;
					regLocs[type][index].offset = frameOffset;
					frameOffset += 8;
				};
			};
		};
	}
	else
	{
		fprintf(stderr, "%s:%d: unknown calling convention '%s' (x86_64)\n", progName, proc->lineno, proc->conv);
		exit(1);
	};
	
	// allocate remaining registers
	for (i=0; i<REGTYPE_NUM; i++)
	{
		int j;
		for (j=0; j<countRegs[i]; j++)
		{
			if (regLocs[i][j].type == -1)
			{
				if (X86_64_AllocReg((uint16_t)i, &regLocs[i][j], &regState, proc->conv, proc->flags) != 0)
				{
					fprintf(stderr, "%s:%d: register allocation failed (x86_64)\n", progName, proc->lineno);
					exit(1);
				};
			};
		};
	};
	
	// TODO: detect if alloca is used
	int haveAlloca = 0;
	
	// locations for saving registers (which ones depends on ABI)
	int saveLocs[16];
	memset(saveLocs, 0, sizeof(saveLocs));
	
	// if true, register saving locations are relative to %rsp (else %rbp)
	int saveRelRSP = 0;
	
	// if true, the frame pointer was omitted
	int frameOmit = 0;
	
	// emit convention-specific prolog
	if (strcmp(proc->conv, "sysv") == 0)
	{
		// calculate where to save each preserved register and how much more frame size we need
		// for that
		static int savedRegs[5] = {RBX, R12, R13, R14, R15};
		
		for (i=0; i<5; i++)
		{
			if (regState.gpr & (1 << savedRegs[i]))
			{
				regState.frameSize += 8;
				saveLocs[savedRegs[i]] = -regState.frameSize;
			};
		};
		
		// 16-byte-align the frame size
		regState.frameSize = (regState.frameSize + 15) & ~15;
		
		// try using the Red Zone if possible
		if ((!compFlags.mno_red_zone) && (!haveAlloca) && (proc->flags & PROC_LEAF) && (regState.frameSize <= 128))
		{
			// save relative to %rsp, not %rbp
			saveRelRSP = 1;
			
			// now move all %rbp-relative macroregisters to the red zone
			for (i=0; i<REGTYPE_NUM; i++)
			{
				int j;
				for (j=0; j<countRegs[i]; j++)
				{
					if ((regLocs[i][j].type == LOC_FRAME) && (regLocs[i][j].offset < 0))
					{
						regLocs[i][j].type = LOC_STACK;
					};
				};
			};
			
			regState.frameSize = 0;
		};
		
		// omit the frame pointer if possible
		if ((!compFlags.fno_omit_frame_pointer) && (!haveAlloca) && (regState.frameSize == 0))
		{
			// omit frame pointer
			frameOmit = 1;
			
			// all %rbp-relative arguments must now be marked %rsp-relative
			for (i=0; i<REGTYPE_NUM; i++)
			{
				int j;
				for (j=0; j<countRegs[i]; j++)
				{
					if ((regLocs[i][j].type == LOC_FRAME) && (regLocs[i][j].offset > 0))
					{
						regLocs[i][j].type = LOC_STACK;
						regLocs[i][j].offset -= 8;
					};
				};
			};
		}
		else
		{
			printf("\tpush %%rbp\n");
			printf("\tmov %%rsp, %%rbp\n");
		};
		
		if (regState.frameSize != 0)
		{
			printf("\tsub $%d, %%rsp\n", regState.frameSize);
		};
	}
	else
	{
		fprintf(stderr, "%s:%d: i don't know how to emit the prolog for '%s' (x86_64)\n", progName, proc->lineno,
					proc->conv);
		exit(1);
	};
	
	// save the registers
	for (i=0; i<16; i++)
	{
		if (saveLocs[i] != 0)
		{
			if (saveRelRSP)
			{
				printf("\tmov %s, %d(%%rsp)\n", reginfo[i].name64, saveLocs[i]);
			}
			else
			{
				printf("\tmov %s, %d(%%rbp)\n", reginfo[i].name64, saveLocs[i]);
			};
		};
	};
	
	int epilogSymbolIndex = nextSymbolIndex++;
	
	// now expand the macros
	for (mac=proc->first; mac!=NULL; mac=mac->next)
	{
		if (mac->type == MAC_RET)
		{
			if (mac->numOps != 1)
			{
				fprintf(stderr, "%s:%d: bad macro\n", progName, mac->lineno);
				exit(1);
			};
			
			if (mac->ops[0].type != OPR_REG)
			{
				fprintf(stderr, "%s:%d: bad macro\n", progName, mac->lineno);
				exit(1);
			};
			
			X86_64_RegLoc returnLoc;
			
			RegSpec reg = mac->ops[0].reg.spec;
			uint16_t type = REGSPEC_TYPE(reg);
			uint16_t index = REGSPEC_INDEX(reg);
			
			if ((type == REGTYPE_F) || (type == REGTYPE_FD))
			{
				returnLoc.type = LOC_XMM;
				returnLoc.offset = 0;
			}
			else
			{
				returnLoc.type = LOC_GPR;
				returnLoc.offset = RAX;
			};
			
			if (memcmp(&returnLoc, &regLocs[type][index], sizeof(X86_64_RegLoc)) != 0)
			{
				X86_64_Print(type, "\tmov #, #\n", &regLocs[type][index], &returnLoc);
			};
			
			if (mac->next != NULL)
			{
				printf("\tjmp .L%d\n", epilogSymbolIndex);
			};
		}
		else if (mac->type == MAC_LABEL)
		{
			if (mac->numOps != 1)
			{
				fprintf(stderr, "%s:%d: bad macro\n", progName, mac->lineno);
				exit(1);
			};
			
			if (mac->ops[0].type != OPR_SYMBOL)
			{
				fprintf(stderr, "%s:%d: bad macro\n", progName, mac->lineno);
				exit(1);
			};
			
			printf("%s:\n", mac->ops[0].symbol.name);
		}
		else if (mac->type == MAC_MOV)
		{
			if (mac->numOps != 2)
			{
				fprintf(stderr, "%s:%d: bad macro\n", progName, mac->lineno);
				exit(1);
			};
			
			if (mac->ops[0].type != OPR_REG)
			{
				fprintf(stderr, "%s:%d: bad macro\n", progName, mac->lineno);
				exit(1);
			};

			if (mac->ops[1].type != OPR_REG)
			{
				fprintf(stderr, "%s:%d: bad macro\n", progName, mac->lineno);
				exit(1);
			};
			
			RegSpec regDest = mac->ops[0].reg.spec;
			RegSpec regSrc = mac->ops[1].reg.spec;
			
			if (REGSPEC_TYPE(regDest) != REGSPEC_TYPE(regSrc))
			{
				fprintf(stderr, "%s:%d: bad macro\n", progName, mac->lineno);
				exit(1);
			};
			
			uint16_t type = REGSPEC_TYPE(regDest);
			uint16_t idxDest = REGSPEC_INDEX(regDest);
			uint16_t idxSrc = REGSPEC_INDEX(regSrc);
			
			if (idxDest == idxSrc)
			{
				// optimize that out
				continue;
			};
			
			X86_64_RegLoc *locDest = &regLocs[type][idxDest];
			X86_64_RegLoc *locSrc = &regLocs[type][idxSrc];
			
			if (locDest->type != locSrc->type)
			{
				// if they are different, then at least one must be in
				// memory, so we must move to the temporary register first
				X86_64_RegLoc tempReg;
				
				if ((type == REGTYPE_F) || (type == REGTYPE_FD))
				{
					tempReg.type = LOC_XMM;
					tempReg.offset = tempXMM;
				}
				else
				{
					tempReg.type = LOC_GPR;
					tempReg.offset = tempGPR;
				};
				
				X86_64_Print(type, "\tmov #, #\n", locSrc, &tempReg);
				X86_64_Print(type, "\tmov #, #\n", &tempReg, locDest);
			}
			else
			{
				// we can move directly
				X86_64_Print(type, "\tmov #, #\n", locSrc, locDest);
			};
		};
	};
	
	// epilog
	printf(".L%d:\n", epilogSymbolIndex);
	
	// register restoration
	for (i=0; i<16; i++)
	{
		if (saveLocs[i] != 0)
		{
			if (saveRelRSP)
			{
				printf("\tmov %d(%%rsp), %s\n", saveLocs[i], reginfo[i].name64);
			}
			else
			{
				printf("\tmov %d(%%rbp), %s\n", saveLocs[i], reginfo[i].name64);
			};
		};
	};
	
	// restore frame if necessary
	if (!frameOmit)
	{
		printf("\tmov %%rbp, %%rsp\n");
		printf("\tpop %%rbp\n");
	};
	
	printf("\tret\n");
	(void)tempGPR;
	(void)tempXMM;
};
