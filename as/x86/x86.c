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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "as.h"
#include "asfamily.h"
#include "libobj.h"
#include "liblex.h"

int x86_bits = X86_DEFAULT_BITS;

// translate a register name used in base/scale to its number. returns 0x10 for RIP (if allowRIP) and 0xFF for
// errors.
uint8_t x86_Regnum(const char *regname, int allowRIP)
{
	if (strcmp(regname, "rip") == 0)
	{
		if ((x86_bits == 64) && allowRIP) return 0x10;
		else return 0xFF;
	};
	
	// 64-bit only regs
	if (x86_bits == 64)
	{
		if (strcmp(regname, "r8") == 0) return 8;
		if (strcmp(regname, "r9") == 0) return 9;
		if (strcmp(regname, "r10") == 0) return 10;
		if (strcmp(regname, "r11") == 0) return 11;
		if (strcmp(regname, "r12") == 0) return 12;
		if (strcmp(regname, "r13") == 0) return 13;
		if (strcmp(regname, "r14") == 0) return 14;
		if (strcmp(regname, "r15") == 0) return 15;
	};
	
	if ((*regname == 'r') || (*regname == 'e')) regname++;
	
	if (strcmp(regname, "ax") == 0) return 0;
	if (strcmp(regname, "cx") == 0) return 1;
	if (strcmp(regname, "dx") == 0) return 2;
	if (strcmp(regname, "bx") == 0) return 3;
	if (strcmp(regname, "sp") == 0) return 4;
	if (strcmp(regname, "bp") == 0) return 5;
	if (strcmp(regname, "si") == 0) return 6;
	if (strcmp(regname, "di") == 0) return 7;
	
	return 0xFF;
};

static int parseOp(char *opspec, x86_Operand *op)
{
	while ((strchr(" \t", *opspec) != NULL) && (*opspec != 0)) opspec++;
	if (*opspec == 0) return -1;
	
	// remove trailing whitespace
	while (strchr(" \t", opspec[strlen(opspec)-1]) != NULL)
	{
		opspec[strlen(opspec)-1] = 0;
	};
	
	uint8_t regnum;
	for (regnum=0; regnum<8; regnum++)
	{
		RegSpec *regspec = &gprList[regnum];
		if (strcmp(regspec->name8, opspec) == 0)
		{
			op->gpr.type = OPTYPE_GPR;
			op->gpr.num = regnum;
			op->gpr.opsz = 8;
			return 0;
		};
		
		if (strcmp(regspec->name16, opspec) == 0)
		{
			op->gpr.type = OPTYPE_GPR;
			op->gpr.num = regnum;
			op->gpr.opsz = 16;
			return 0;
		};

		if (strcmp(regspec->name32, opspec) == 0)
		{
			op->gpr.type = OPTYPE_GPR;
			op->gpr.num = regnum;
			op->gpr.opsz = 32;
			return 0;
		};
	};

	for (regnum=0; sregList[regnum].name!=NULL; regnum++)
	{
		SregSpec *spec = &sregList[regnum];
		if (strcmp(spec->name, opspec) == 0)
		{
			op->sreg.type = OPTYPE_SREG;
			op->sreg.num = spec->num;
			op->sreg.opsz = 16;		// unconditionally
			op->sreg.prefix = spec->prefix;
			return 0;
		};
	};
	
	if (x86_bits == 64)
	{
		for (regnum=0; regnum<16; regnum++)
		{
			RegSpec *regspec = &rexgprList[regnum];
			if (strcmp(regspec->name8, opspec) == 0)
			{
				op->gpr.type = OPTYPE_REXGPR;
				op->gpr.num = regnum;
				op->gpr.opsz = 8;
				return 0;
			};
		
			if (strcmp(regspec->name16, opspec) == 0)
			{
				op->gpr.type = OPTYPE_REXGPR;
				op->gpr.num = regnum;
				op->gpr.opsz = 16;
				return 0;
			};

			if (strcmp(regspec->name32, opspec) == 0)
			{
				op->gpr.type = OPTYPE_REXGPR;
				op->gpr.num = regnum;
				op->gpr.opsz = 32;
				return 0;
			};

			if (strcmp(regspec->name64, opspec) == 0)
			{
				op->gpr.type = OPTYPE_REXGPR;
				op->gpr.num = regnum;
				op->gpr.opsz = 64;
				return 0;
			};
		};
	};

	// XMM registers
	uint8_t mmno;
	int count = 0;
	if (sscanf(opspec, "mm%hhu%n", &mmno, &count) >= 1)
	{
		if (count == strlen(opspec))
		{
			if (mmno >= 8) return -1;
			
			op->xmm.type = OPTYPE_XMM;
			op->xmm.num = mmno;
			op->xmm.opsz = 64;		// 64-bit MMX register
			return 0;
		};
	};
	
	// XMM/YMM/ZMM
	char regletter;
	count = 0;
	if (sscanf(opspec, "%cmm%hhu%n", &regletter, &mmno, &count) >= 2)
	{
		if (count == strlen(opspec))
		{
			if (mmno >= 16) return -1;
			if ((x86_bits != 64) && (mmno >= 8)) return -1;
			
			op->xmm.type = OPTYPE_XMM;
			op->xmm.num = mmno;
			
			switch (regletter)
			{
			case 'x':
				op->xmm.opsz = 128;
				return 0;
			default:
				return -1;
			};
		};
	};
	
	// ST
	if (sscanf(opspec, "st%hhu%n", &mmno, &count) >= 1)
	{
		if (count == strlen(opspec))
		{
			if (mmno >= 8) return -1;
			
			op->st.type = OPTYPE_ST;
			op->st.num = mmno;
			
			return 0;
		};
	};
	
	// CR
	if (sscanf(opspec, "cr%hhu%n", &mmno, &count) >= 1)
	{
		if (count == strlen(opspec))
		{
			if (mmno >= 16) return -1;
			if (mmno >= 8 && x86_bits != 64) return -1;
			
			op->cr.type = OPTYPE_CR;
			op->cr.num = mmno;
			
			return 0;
		};
	};
	
	// DR
	if (sscanf(opspec, "dr%hhu%n", &mmno, &count) >= 1)
	{
		if (count == strlen(opspec))
		{
			if (mmno >= 8) return -1;
			
			op->dr.type = OPTYPE_DR;
			op->dr.num = mmno;
			
			return 0;
		};
	};
	
	// DB - alias for DR registers
	if (sscanf(opspec, "db%hhu%n", &mmno, &count) >= 1)
	{
		if (count == strlen(opspec))
		{
			if (mmno >= 8) return -1;
			
			op->dr.type = OPTYPE_DR;
			op->dr.num = mmno;
			
			return 0;
		};
	};
	
	char *endptr;
	long int value = strtol(opspec, &endptr, 0);
	if (*endptr == 0)
	{
		op->imm.type = OPTYPE_IMM;
		op->imm.value = value;
		return 0;
	};
	
	// every other case: [BYTE|WORD|DWORD|QWORD] OFFSET|PTR ...
	uint8_t opsz;
	char *sizename = strtok(opspec, " \t");
	if (sizename == NULL) return -1;
	
	char *typename = NULL;
	if (strcmp(sizename, "BYTE") == 0)
	{
		opsz = 8;
	}
	else if (strcmp(sizename, "WORD") == 0)
	{
		opsz = 16;
	}
	else if (strcmp(sizename, "DWORD") == 0)
	{
		opsz = 32;
	}
	else if (strcmp(sizename, "QWORD") == 0)
	{
		opsz = 64;
	}
	else if (strcmp(sizename, "XMMWORD") == 0)
	{
		opsz = 128;
	}
	else if (strcmp(sizename, "FLOAT") == 0)
	{
		opsz = OPSZ_FLOAT;
	}
	else if (strcmp(sizename, "DOUBLE") == 0)
	{
		opsz = OPSZ_DOUBLE;
	}
	else if (strcmp(sizename, "FPUWORD") == 0)
	{
		opsz = OPSZ_FPUWORD;
	}
	else
	{
		opsz = -1;
		typename = sizename;
	};
	
	if (typename == NULL) typename = strtok(NULL, " \t");
	
	if (typename == NULL)
	{
		return -1;
	}
	else if (strcmp(typename, "OFFSET") == 0)
	{
		int64_t addend = 0;
		char *label = strtok(NULL, " \t");
		if (label == NULL) return -1;
		
		char *bracketPos = strchr(label, '[');
		if (bracketPos != NULL)
		{
			*bracketPos = 0;
			char *endptr;
			long int offset = strtol(bracketPos+1, &endptr, 0);
			
			if ((endptr[0] != ']') || (endptr[1] != 0))
			{
				return -1;
			};
			
			addend = (int64_t) offset;
		};
		
		op->offset.type = OPTYPE_OFFSET;
		op->offset.symbol = strdup(label);
		op->offset.addend = addend;
		op->offset.opsz = opsz;
		return 0;
	}
	else if (strcmp(typename, "PTR") == 0)
	{
		char *rest = strtok(NULL, "");

		memset(op, 0, sizeof(x86_Operand));
		op->memref.type = OPTYPE_MEMREF;
		op->memref.base = 0xFF;			// no base register
		op->memref.idx = 0xFF;			// no index register
		op->memref.opsz = opsz;
		op->memref.segment = 0;

		char *colonPos = strchr(rest, ':');
		if (colonPos != NULL)
		{
			*colonPos = 0;
			char *segname = rest;
			rest = colonPos+1;
			
			int i;
			int found = 0;
			for (i=0; sregList[i].name!=NULL; i++)
			{
				if (strcmp(segname, sregList[i].name) == 0)
				{
					op->memref.segment = sregList[i].prefix;
					found = 1;
					break;
				};
			};
			
			if (!found)
			{
				char *endptr;
				unsigned long int segment = strtoul(segname, &endptr, 0);
				if (*endptr != 0)
				{
					return -1;
				};
			
				if ((segment & ~0xFFFF) != 0)
				{
					return -1;
				};
			
				op->memref.type = OPTYPE_MEMREF_ABSEG;
				op->memref.segment = (uint16_t) segment;
			};
		};
		
		char *bracketBit = strchr(rest, '[');
		if (bracketBit != NULL) *bracketBit++ = 0;

		char *plusPos = strchr(rest, '+');
		if (plusPos != NULL)
		{
			*plusPos = 0;
			op->memref.symbol = strdup(rest);
			
			char *endptr;
			long int offset = strtol(plusPos+1, &endptr, 0);
			if (*endptr != 0)
			{
				return -1;
			};
			
			op->memref.off = (int64_t) offset;
		}
		else
		{
			char *endptr;
			long int offset = strtol(rest, &endptr, 0);
			if (*endptr != 0)
			{
				op->memref.symbol = strdup(rest);
			}
			else
			{
				op->memref.off = (int64_t) offset;
			};
		};
		
		if (bracketBit != NULL)
		{
			char *endPos = strchr(bracketBit, ']');
			if (endPos == NULL) return -1;
			if (endPos[1] != 0) return -1;
			*endPos = 0;
			
			char *sib = strchr(bracketBit, '+');
			if (sib != NULL) *sib++ = 0;
			
			op->memref.base = x86_Regnum(bracketBit, 1);
			if (op->memref.base == 0xFF) return -1;
			
			if ((op->memref.base >= 8) && (x86_bits != 64))
			{
				return -1;
			};
			
			if (sib != NULL)
			{
				// cannot use SIB with RIP
				if (op->memref.base == 0x10) return -1;
				
				char *mulPos = strchr(sib, '*');
				if (mulPos != NULL)
				{
					*mulPos = 0;
					
					char *endptr;
					long int scalenum = strtol(sib, &endptr, 0);
					if (*endptr != 0)
					{
						return -1;
					};
					
					switch (scalenum)
					{
					case 1:
						op->memref.scale = 0;
						break;
					case 2:
						op->memref.scale = 1;
						break;
					case 4:
						if (x86_bits < 32) return -1;
						op->memref.scale = 2;
						break;
					case 8:
						if (x86_bits < 64) return -1;
						op->memref.scale = 3;
						break;
					default:
						return -1;
					};
					
					sib = mulPos + 1;
				};
				
				op->memref.idx = x86_Regnum(sib, 0);
				if (op->memref.idx == 0xFF) return -1;
				if (op->memref.idx >= 8 && x86_bits < 64) return -1;
			};
		};
		
		if (op->type == OPTYPE_MEMREF_ABSEG)
		{
			if (op->memref.scale != 0 || op->memref.base != 0xFF) return -1;
		};
		
		return 0;
	}
	else
	{
		return -1;
	};
};

void x86_EmitModRM(const char *filename, int lineno, x86_Operand *opA, x86_Operand *opB, int addcount)
{
	Section *sect = asGetSection();
	
	if (((opA->type == OPTYPE_GPR) || (opA->type == OPTYPE_REXGPR) || (opA->type == OPTYPE_XMM) || (opA->type == OPTYPE_SREG) || (opA->type == OPTYPE_CR) || (opA->type == OPTYPE_DR)) &&
		((opB->type == OPTYPE_GPR) || (opB->type == OPTYPE_REXGPR) || (opB->type == OPTYPE_XMM) || (opB->type == OPTYPE_SREG) || (opB->type == OPTYPE_CR) || (opB->type == OPTYPE_DR)))
	{
		uint8_t modrm = 0xC0 | ((opA->gpr.num & 0x7) << 3) | (opB->gpr.num & 0x7);
		objSectionAppend(sect, &modrm, 1);
	}
	else if (opB->type == OPTYPE_MEMREF)
	{
		if (x86_bits == 64)
		{
			if ((opB->memref.base == 0xFF) && (opB->memref.idx == 0xFF))
			{
				// [disp32]
				uint8_t modrm = ((opA->gpr.num & 0x7) << 3) | 0x4;	// mode 00, r/m=100 [SIB]
				objSectionAppend(sect, &modrm, 1);
				
				// SIB: scale=0, index=0100 (0x4), base=0101 (0x5) = [disp32]
				objSectionAppend(sect, "\x25", 1);
				
				// displacement in little-endian
				if (opB->memref.symbol == NULL)
				{
					if ((opB->memref.off & 0xFFFFFFFF00000000) != 0)
					{
						asDiag(filename, lineno, ML_WARNING, "address truncated\n");
					};
					
					ObjLE32 disp32 = OBJ_MAKE_LE32((uint32_t) opB->memref.off);
					objSectionAppend(sect, &disp32, 4);
				}
				else
				{
					objSectionReloc(sect, opB->memref.symbol,
						REL_DWORD, REL_X86_ABSOLUTE, opB->memref.off);
				};
			}
			else if (opB->memref.base == 0x10)
			{
				// indicate RIP-relative mode in the ModR/M
				// mode=00, r/m=101
				uint8_t modrm = ((opA->gpr.num & 0x7) << 3) | 0x5;
				objSectionAppend(sect, &modrm, 1);
				
				// 32-bit displacement
				if (opB->memref.symbol == NULL)
				{
					if ((opB->memref.off & 0xFFFFFFFF00000000) != 0)
					{
						asDiag(filename, lineno, ML_WARNING, "offset truncated\n");
					};
					
					ObjLE32 disp32 = OBJ_MAKE_LE32((uint32_t) opB->memref.off-4-addcount);
					objSectionAppend(sect, &disp32, 4);
				}
				else
				{
					objSectionReloc(sect, opB->memref.symbol,
						REL_DWORD, REL_X86_RELATIVE, opB->memref.off-4-addcount);
				};
			}
			else if ((opB->memref.base != 0xFF) && (opB->memref.idx == 0xFF))
			{
				// [base (+ disp)]
				uint8_t modrm = 0;
				if (opB->memref.symbol != NULL)
				{
					modrm = 0x80;		// [base + disp32]
				}
				else if ((opB->memref.off == 0) && ((opB->memref.base & 0x7) != 5))
				{
					modrm = 0x00;		// [base]
				}
				else if ((opB->memref.off & 0xFF) == opB->memref.off)
				{
					modrm = 0x40;		// [base + disp8]
				}
				else
				{
					modrm = 0x80;		// [base + disp32]
				};
				
				modrm |= ((opA->gpr.num & 0x7) << 3);
				
				uint8_t sib;
				int sibNeeded = 0;
				switch (opB->memref.base)
				{
				// bases that we can use directly
				case 0:			// RAX
				case 1:			// RCX
				case 2:			// RDX
				case 3:			// RBX
				case 5:
				case 6:			// RSI
				case 7:			// RDI
				case 8:
				case 9:
				case 10:
				case 11:
				case 13:
				case 14:
				case 15:
					modrm |= opB->memref.base & 0x07;
					break;
				// RSP/R12
				case 4:
				case 12:
					modrm |= 4;		// SIB
					sib = 0x24;		// index=0.100, base=RSP
					sibNeeded = 1;
				};
				
				objSectionAppend(sect, &modrm, 1);
				if (sibNeeded) objSectionAppend(sect, &sib, 1);
				
				// emit a displacement if necessary
				if (opB->memref.symbol != NULL)
				{
					// disp32
					// TODO: what should the addend be?
					objSectionReloc(sect, opB->memref.symbol,
						REL_DWORD, REL_X86_RELATIVE, opB->memref.off);
				}
				else if ((opB->memref.off == 0) && ((opB->memref.base & 0x7) != 5))
				{
					// no displacement
				}
				else if ((opB->memref.off & 0xFF) == opB->memref.off)
				{
					// 8-bit displacement
					uint8_t disp8 = (uint8_t) opB->memref.off;
					objSectionAppend(sect, &disp8, 1);
				}
				else
				{
					// 32-bit displacement
					if ((opB->memref.off & 0xFFFFFFFF00000000) != 0)
					{
						asDiag(filename, lineno, ML_WARNING, "offset truncated\n");
					};
					
					ObjLE32 disp32 = OBJ_MAKE_LE32((uint32_t) opB->memref.off);
					objSectionAppend(sect, &disp32, 4);
				};
			}
			else
			{
				fprintf(stderr, "assembler internal error: cannot encode ModR/M\n");
				abort();
			};
		}
		else
		{
			fprintf(stderr, "assembler internal failure: cannot encode /r in %d-bit mode!\n", x86_bits);
			abort();
		};
	};
};

void x86_Emit(const char *filename, int lineno, const char *mspec, x86_Operand *opA, x86_Operand *opB, int flags, int types)
{
	Section *sect = asGetSection();
	
	// funny trick
	if (types == INSN_RM_R)
	{
		x86_Operand *temp = opA;
		opA = opB;
		opB = temp;
		types = INSN_R_RM;
	}
	else if (types == INSN_RM8_R8)
	{
		x86_Operand *temp = opA;
		opA = opB;
		opB = temp;
		types = INSN_R8_RM8;
	}
	else if (types == INSN_RM16_SREG)
	{
		x86_Operand *temp = opA;
		opA = opB;
		opB = temp;
		types = INSN_SREG_RM16;
	}
	else if (types == INSN_RM64_SREG)
	{
		x86_Operand *temp = opA;
		opA = opB;
		opB = temp;
		types = INSN_SREG_RM64;
	}
	else if (types == INSN_OFFSET_AL)
	{
		x86_Operand *temp = opA;
		opA = opB;
		opB = temp;
		types = INSN_AL_OFFSET;
	}
	else if (types == INSN_OFFSET_A)
	{
		x86_Operand *temp = opA;
		opA = opB;
		opB = temp;
		types = INSN_A_OFFSET;
	}
	else if (types == INSN_R_CR)
	{
		x86_Operand *temp = opA;
		opA = opB;
		opB = temp;
		types = INSN_CR_R;
	}
	else if (types == INSN_R_DR)
	{
		x86_Operand *temp = opA;
		opA = opB;
		opB = temp;
		types = INSN_DR_R;
	}
	else if (types == INSN_RM_XMM)
	{
		x86_Operand *temp = opA;
		opA = opB;
		opB = temp;
		types = INSN_XMM_RM;
	}
	else if (types == INSN_RM_MM)
	{
		x86_Operand *temp = opA;
		opA = opB;
		opB = temp;
		types = INSN_MM_RM;
	}
	else if (types == INSN_GPRM32_MM)
	{
		x86_Operand *temp = opA;
		opA = opB;
		opB = temp;
		types = INSN_MM_GPRM32;
	}
	else if (types == INSN_GPRM64_MM)
	{
		x86_Operand *temp = opA;
		opA = opB;
		opB = temp;
		types = INSN_MM_GPRM64;
	}
	else if (types == INSN_GPRM32_XMM)
	{
		x86_Operand *temp = opA;
		opA = opB;
		opB = temp;
		types = INSN_XMM_GPRM32;
	}
	else if (types == INSN_GPRM64_XMM)
	{
		x86_Operand *temp = opA;
		opA = opB;
		opB = temp;
		types = INSN_XMM_GPRM64;
	}
	else if (types == INSN_I8_AL)
	{
		x86_Operand *temp = opA;
		opA = opB;
		opB = temp;
		types = INSN_AL_I;
	}
	else if (types == INSN_I8_AX)
	{
		x86_Operand *temp = opA;
		opA = opB;
		opB = temp;
		types = INSN_AX_I8;
	}
	else if (types == INSN_I8_EAX)
	{
		x86_Operand *temp = opA;
		opA = opB;
		opB = temp;
		types = INSN_EAX_I8;
	}
	else if (types == INSN_DX_AL)
	{
		x86_Operand *temp = opA;
		opA = opB;
		opB = temp;
		types = INSN_AL_DX;
	}
	else if (types == INSN_DX_AX)
	{
		x86_Operand *temp = opA;
		opA = opB;
		opB = temp;
		types = INSN_AX_DX;
	}
	else if (types == INSN_DX_EAX)
	{
		x86_Operand *temp = opA;
		opA = opB;
		opB = temp;
		types = INSN_EAX_DX;
	};

	// mandatory prefix (if any) must come before REX and 66 etc
	if (memcmp(mspec, "66 ", 3) == 0)
	{
		objSectionAppend(sect, "\x66", 1);
		mspec += 3;
	}
	else if (memcmp(mspec, "F3 ", 3) == 0)
	{
		objSectionAppend(sect, "\xF3", 1);
		mspec += 3;
	}
	else if (memcmp(mspec, "F2 ", 3) == 0)
	{
		objSectionAppend(sect, "\xF2", 1);
		mspec += 3;
	};

	// segment override if necessary
	x86_Operand *refOp = NULL;
	if (opA->type == OPTYPE_MEMREF) refOp = opA;
	if (opB->type == OPTYPE_MEMREF) refOp = opB;
	if (refOp != NULL)
	{
		if (refOp->memref.segment != 0)
		{
			uint8_t ovr = (uint8_t) refOp->memref.segment;
			objSectionAppend(sect, &ovr, 1);
		};
	};
	
	// emit the size override prefix is necessary
	if ((types == INSN_R16_I16) || (types == INSN_R32_I32) || (types == INSN_R8_RM8) || (types == INSN_R_RM) || (types == INSN_AX_I) || (types == INSN_EAX_I) || (types == INSN_A_OFFSET)
		|| (types == INSN_R16_RM8) || (types == INSN_R32_RM8) || (types == INSN_R64_RM8) || (types == INSN_R32_RM16) || (types == INSN_R64_RM16) || (types == INSN_R64_RM32) || (types == INSN_RM_I8) || (types == INSN_AX_I8) || (types == INSN_EAX_I8) || (types == INSN_EAX_DX) || (types == INSN_AX_DX))
	{
		int defaultBits = x86_bits;
		if (defaultBits == 64) defaultBits = 32;
		
		if ((opA->gpr.opsz == 16) || (opA->gpr.opsz == 32))
		{
			if (opA->gpr.opsz != defaultBits)
			{
				objSectionAppend(sect, "\x66", 1);
			};
		};
	};
	
	if (types == INSN_REL)
	{
		int defaultBits = x86_bits;
		if (defaultBits == 64) defaultBits = 32;
		
		if (opA->offset.opsz != defaultBits)
		{
			objSectionAppend(sect, "\x66", 1);
		};
	};
	
	if (types == INSN_RM)
	{
		int defaultBits = x86_bits;
		if (defaultBits == 64) defaultBits = 32;
		
		if ((opA->memref.opsz == 16) || (opA->memref.opsz == 32))
		{
			if (opA->memref.opsz != defaultBits)
			{
				objSectionAppend(sect, "\x66", 1);
			};
		};
	};
	
	if ((types == INSN_RM16_I16) || (types == INSN_RM32_I32))
	{
		int bits;
		if (types == INSN_RM16_I16) bits = 16;
		else bits = 32;
		
		int defaultBits = x86_bits;
		if (defaultBits == 64) defaultBits = 32;
		
		if (bits != defaultBits)
		{
			objSectionAppend(sect, "\x66", 1);
		};
	};

	// emit REX if necessary and if not already mandated elsewhere in the instruction
	if (x86_bits == 64)
	{
		if (types == INSN_RM || types == INSN_RM32_I32)
		{
			if (opA->memref.opsz == 64 && (flags & INSN_DEF64) == 0)
			{
				uint8_t rex = 0x48;	// REX.W
				objSectionAppend(sect, &rex, 1);
			}
			else if (opA->memref.opsz != 64 && (flags & INSN_DEF64))
			{
				asDiag(filename, lineno, ML_ERROR, "this instruction can only accept a QWORD PTR in 64-bit mode\n");
			};
		}
		else if (types == INSN_CR_R)
		{
			uint8_t rex = 0x40;
			
			if (opA->cr.num & 0x8) rex |= 0x4;
			if (opB->gpr.num & 0x8) rex |= 0x1;
			if (rex != 0x40) objSectionAppend(sect, &rex, 1);
		}
		else if (types == INSN_DR_R)
		{
			uint8_t rex = 0x40;
			
			if (opB->gpr.num & 0x8) rex |= 0x1;
			if (rex != 0x40) objSectionAppend(sect, &rex, 1);
		}
		else if (types == INSN_R_RM || types == INSN_XMM_RM || types == INSN_MM_XMMRM || types == INSN_R_CR || 
			types == INSN_GPRM32_MM || types == INSN_GPRM64_MM || types == INSN_MM_GPRM32 || types == INSN_MM_GPRM64 || types == INSN_GPRM32_XMM || types == INSN_GPRM64_XMM || 
			types == INSN_XMM_GPRM32 || types == INSN_XMM_GPRM64 || types == INSN_GPR_XMM || types == INSN_XMM_MM || types == INSN_R16_RM8 || types == INSN_R32_RM8 || types == INSN_R64_RM8
			|| types == INSN_R32_RM16 || types == INSN_R64_RM16 || types == INSN_R64_RM32)
		{
			uint8_t rex = 0x40;
			if (opA->gpr.opsz == 64 && (types != INSN_MM_XMMRM))
			{
				rex |= 0x08;	// REX.W
			};
		
			if (types == INSN_XMM_GPRM64)
			{
				rex |= 0x08;	// REX.W
			};
			
			if (opA->gpr.num & 0x8)
			{
				rex |= 0x4;
			};
			
			if (opB->type == OPTYPE_REXGPR || opB->type == OPTYPE_XMM)
			{
				if (opB->gpr.num & 0x8)
				{
					rex |= 0x1;
				};
			};
			
			if (opB->type == OPTYPE_MEMREF)
			{
				if ((opB->memref.idx & 0x8) && (opB->memref.idx != 0xFF))
				{
					rex |= 0x2;	// REX.X
				};
				
				if ((opB->memref.base & 0x8) && (opB->memref.base != 0xFF) && (opB->memref.base != 0x10))
				{
					rex |= 0x1;	// REX.B
				};
			};
			
			if ((rex != 0x40) || (opA->type == OPTYPE_REXGPR) || (opB->type == OPTYPE_REXGPR))
			{
				objSectionAppend(sect, &rex, 1);
			};
		}
		else if (types == INSN_A_OFFSET && opB->memref.opsz == 64)
		{
			objSectionAppend(sect, "\x48", 1);
		}
		else if (strstr(mspec, "REX") == NULL)
		{
			uint8_t rex = 0;
			if (opA->type == OPTYPE_REXGPR)
			{
				rex = 0x40 | (opA->rexgpr.num >> 3);
				if ((opA->rexgpr.opsz == 64) && ((flags & INSN_DEF64) == 0)) rex |= 0x08; // REX.W
				if (flags & INSN_DEF64) if (rex == 0x40) rex = 0;	// "plain" REX unnecessary
			}
			else if ((opA->type == OPTYPE_GPR) && (opA->gpr.opsz == 64))
			{
				if ((flags & INSN_DEF64) == 0) rex = 0x48;				// REX.W
			};
		
			if (rex != 0)
			{
				objSectionAppend(sect, &rex, 1);
			};
		};
	};

#if 0
	// emit VEX when necessary
	if (types == INSN_XMM_RM)
	{
		// unpacked fields
		uint8_t ir = !(opA->xmm.num & 0x8);
		uint8_t ix = 1;
		uint8_t ib = 1;
		uint8_t map = 1;
		uint8_t we = 0;
		uint8_t vreg = opA->xmm.num;
		uint8_t l = 0;
		uint8_t p = 0;
		
		if (opB->type == OPTYPE_XMM)
		{
			ib = !(opB->xmm.num & 0x8);
		}
		else
		{
			ix = !((opB->memref.idx & 0x8) && (opB->memref.idx != 0xFF));
			ib = !((opB->memref.base & 0x8) && (opB->memref.base != 0xFF) && (opB->memref.base != 0x10));
		};
		
		// check if VEX is needed here
		if ((ir != 1) || (ix != 1) || (ib != 1) || (l != 0) || (vreg != opA->xmm.num))
		{
			vreg = (~vreg) & 0xF;
			// detect mandatory prefix
			if (memcmp(mspec, "66 ", 3) == 0)
			{
				p = 1;
				mspec += 3;
			}
			else if (memcmp(mspec, "F3 ", 3) == 0)
			{
				p = 2;
				mspec += 3;
			}
			else if (memcmp(mspec, "F2 ", 3) == 0)
			{
				p = 3;
				mspec += 3;
			}
			else
			{
				p = 0;
			};
			
			// detect opcode map
			if (memcmp(mspec, "0F 38 ", 6) == 0)
			{
				map = 2;
				mspec += 6;
			}
			else if (memcmp(mspec, "0F 3A ", 6) == 0)
			{
				map = 3;
				mspec += 6;
			}
			else if (memcmp(mspec, "0F ", 3) == 0)
			{
				map = 1;
				mspec += 3;
			};
			
			// format
			uint8_t vex[3];
			vex[0] = 0xC4;
			vex[1] = (ir << 7) | (ix << 6) | (ib << 5) | map;
			vex[2] = (we << 7) | (vreg << 6) | (l << 2) | p;
			
			objSectionAppend(sect, vex, 3);
		};
	};
#endif

	char buffer[256];
	strcpy(buffer, mspec);

	char *bytespec;
	for (bytespec=strtok(buffer, " "); bytespec!=NULL; bytespec=strtok(NULL, " "))
	{
		uint8_t byte;
		int addcount;
		if ((strlen(bytespec) == 2) && (sscanf(bytespec, "%2hhX", &byte) == 1))
		{
			objSectionAppend(sect, &byte, 1);
		}
		else if ((strlen(bytespec) == 4) && (sscanf(bytespec, "%2hhX+r", &byte) == 1)
				&& (strcmp(&bytespec[2], "+r") == 0))
		{
			if (opA->type == OPTYPE_REXGPR)
			{
				byte += opA->rexgpr.num & 0x7;
			}
			else if (opA->type == OPTYPE_GPR)
			{
				byte += opA->gpr.num;
			};
			
			objSectionAppend(sect, &byte, 1);
		}
		else if ((strlen(bytespec) == 4) && (sscanf(bytespec, "%2hhX+i", &byte) == 1)
				&& (strcmp(&bytespec[2], "+i") == 0))
		{
			if ((types == INSN_ST) || (types == INSN_ST_ST0))
			{
				byte += opA->st.num & 0x7;
			}
			else if (types == INSN_ST0_ST)
			{
				byte += opB->st.num & 0x7;
			};
			
			objSectionAppend(sect, &byte, 1);
		}
		else if (strcmp(bytespec, "ib") == 0)
		{
			x86_Operand *op;
			if ((opA->type == OPTYPE_IMM) || (opA->type == OPTYPE_OFFSET))
			{
				op = opA;
			}
			else
			{
				op = opB;
			};
			
			if (op->type == OPTYPE_IMM)
			{
				uint8_t byte;
				long int value = op->imm.value;
			
				if ((value & 0xFF) != value)
				{
					asDiag(filename, lineno, ML_WARNING, "immediate operand does not fit in 8 bits\n");
				};
			
				byte = (uint8_t) (value & 0xFF);
				objSectionAppend(sect, &byte, 1);
			}
			else
			{
				if (op->offset.opsz != 8)
				{
					asDiag(filename, lineno, ML_ERROR, "OFFSET operand may not fit in 8 bits\n");
				}
				else
				{
					objSectionReloc(sect, op->offset.symbol, REL_BYTE, REL_X86_ABSOLUTE, op->offset.addend);
				};
			};
		}
		else if (strcmp(bytespec, "iw") == 0)
		{
			x86_Operand *op;
			if ((opA->type == OPTYPE_IMM) || (opA->type == OPTYPE_OFFSET))
			{
				op = opA;
			}
			else
			{
				op = opB;
			};
			
			if (op->type == OPTYPE_IMM)
			{
				uint8_t bytes[2];
				long int value = op->imm.value;
			
				if ((value & 0xFFFF) != value)
				{
					asDiag(filename, lineno, ML_WARNING, "immediate operand does not fit in 16 bits\n");
				};
			
				bytes[0] = (uint8_t) (value & 0xFF);
				bytes[1] = (uint8_t) (value >> 8);
				objSectionAppend(sect, bytes, 2);
			}
			else
			{
				if (op->offset.opsz != 16)
				{
					asDiag(filename, lineno, ML_ERROR, "OFFSET operand may not fit in 16 bits\n");
				}
				else
				{
					objSectionReloc(sect, op->offset.symbol, REL_WORD, REL_X86_ABSOLUTE, op->offset.addend);
				};
			};
		}
		else if (strcmp(bytespec, "id") == 0)
		{
			x86_Operand *op;
			if ((opA->type == OPTYPE_IMM) || (opA->type == OPTYPE_OFFSET))
			{
				op = opA;
			}
			else
			{
				op = opB;
			};
			
			if (op->type == OPTYPE_IMM)
			{
				uint8_t bytes[4];
				long int value = op->imm.value;
			
				if ((value & 0xFFFFFFFF) != value)
				{
					asDiag(filename, lineno, ML_WARNING, "immediate operand does not fit in 32 bits\n");
				};
			
				bytes[0] = (uint8_t) (value & 0xFF);
				bytes[1] = (uint8_t) (value >> 8);
				bytes[2] = (uint8_t) (value >> 16);
				bytes[3] = (uint8_t) (value >> 24);
				objSectionAppend(sect, bytes, 4);
			}
			else
			{
				if (op->offset.opsz != 32)
				{
					asDiag(filename, lineno, ML_ERROR, "OFFSET operand may not fit in 32 bits\n");
				}
				else
				{
					objSectionReloc(sect, op->offset.symbol, REL_DWORD, REL_X86_ABSOLUTE, op->offset.addend);
				};
			};
		}
		else if (strcmp(bytespec, "iq") == 0)
		{
			x86_Operand *op;
			if ((opA->type == OPTYPE_IMM) || (opA->type == OPTYPE_OFFSET))
			{
				op = opA;
			}
			else
			{
				op = opB;
			};
			
			if (op->type == OPTYPE_IMM)
			{
				uint8_t bytes[8];
				long int value = op->imm.value;
			
				if ((value & 0xFFFFFFFFFFFFFFFF) != value)
				{
					asDiag(filename, lineno, ML_WARNING, "immediate operand does not fit in 64 bits\n");
				};
			
				bytes[0] = (uint8_t) (value & 0xFF);
				bytes[1] = (uint8_t) (value >> 8);
				bytes[2] = (uint8_t) (value >> 16);
				bytes[3] = (uint8_t) (value >> 24);
				bytes[4] = (uint8_t) (value >> 32);
				bytes[5] = (uint8_t) (value >> 40);
				bytes[6] = (uint8_t) (value >> 48);
				bytes[7] = (uint8_t) (value >> 56);
				objSectionAppend(sect, bytes, 8);
			}
			else
			{
				if (op->offset.opsz != 64)
				{
					asDiag(filename, lineno, ML_ERROR, "OFFSET operand may not fit in 64 bits\n");
				}
				else
				{
					objSectionReloc(sect, op->offset.symbol, REL_QWORD, REL_X86_ABSOLUTE, op->offset.addend);
				};
			};
		}
		else if (sscanf(bytespec, "/r.%d", &addcount) == 1)
		{
			x86_EmitModRM(filename, lineno, opA, opB, addcount);
		}
		else if (sscanf(bytespec, "/%hhu.%d", &byte, &addcount) == 2)
		{
			x86_Operand *otherOp = opA;
			if (opA->type == OPTYPE_IMM || opA->type == OPTYPE_OFFSET) otherOp = opB;
			
			x86_Operand fakeop;
			fakeop.gpr.type = OPTYPE_GPR;
			fakeop.gpr.num = bytespec[1] - '0';
			fakeop.gpr.opsz = otherOp->gpr.opsz;
			if (otherOp->type == OPTYPE_MEMREF) fakeop.gpr.opsz = otherOp->memref.opsz;
			
			x86_EmitModRM(filename, lineno, &fakeop, otherOp, addcount);
		}
		else if (sscanf(bytespec, "rel.%d", &addcount) == 1)
		{
			x86_Operand *op = opA;
			if (op->type != OPTYPE_OFFSET) op = opB;
			
			int type;
			switch (op->offset.opsz)
			{
			case 8:
				type = REL_BYTE;
				break;
			case 16:
				type = REL_WORD;
				break;
			case 32:
				type = REL_DWORD;
				break;
			case 64:
				type = REL_QWORD;
				break;
			};
			
			objSectionReloc(sect, op->offset.symbol, type, REL_X86_RELATIVE,
				op->offset.addend-op->offset.opsz/8-addcount);
		}
		else if (strcmp(bytespec, "moffs") == 0)
		{
			// emit a 64-bit address in 64-bit mode
			if (x86_bits == 64)
			{
				if (opB->memref.symbol != NULL)
				{
					objSectionReloc(sect, opB->memref.symbol, REL_QWORD, REL_X86_ABSOLUTE,
							opB->memref.off);
				}
				else
				{
					uint64_t value = opB->memref.off;
					uint8_t bytes[8];
					bytes[0] = (uint8_t) (value & 0xFF);
					bytes[1] = (uint8_t) (value >> 8);
					bytes[2] = (uint8_t) (value >> 16);
					bytes[3] = (uint8_t) (value >> 24);
					bytes[4] = (uint8_t) (value >> 32);
					bytes[5] = (uint8_t) (value >> 40);
					bytes[6] = (uint8_t) (value >> 48);
					bytes[7] = (uint8_t) (value >> 56);
					objSectionAppend(sect, bytes, 8);
				};
			}
			else
			{
				// in every other case, write the offset and segment
				int type, bytecount;
				switch (x86_bits)
				{
				case 16:
					type = REL_WORD;
					bytecount = 2;
					break;
				case 32:
				case 64:
					type = REL_DWORD;
					bytecount = 4;
					break;
				};
				
				if (opB->memref.symbol != NULL)
				{
					objSectionReloc(sect, opB->memref.symbol, type, REL_X86_ABSOLUTE,
							opB->memref.off);
				}
				else
				{
					uint64_t value = opB->memref.off;
					uint8_t bytes[4];
					memset(bytes, 0, 4);
					bytes[0] = (uint8_t) (value & 0xFF);
					bytes[1] = (uint8_t) (value >> 8);
					bytes[2] = (uint8_t) (value >> 16);
					bytes[3] = (uint8_t) (value >> 24);
					
					if (bytecount == 2)
					{
						if ((value & ~0xFFFF) != 0)
						{
							asDiag(filename, lineno, ML_ERROR, "offset does not fit in 16 bits");
							return;
						};
					};
					
					objSectionAppend(sect, bytes, bytecount);
				};
				
				// and the segment
				uint8_t bytes[2];
				bytes[0] = (uint8_t) (opB->memref.segment);
				bytes[1] = (uint8_t) (opB->memref.segment >> 8);
				objSectionAppend(sect, bytes, 2);
			};
		}
		else
		{
			asDiag(filename, lineno, ML_ERROR, "invalid bytespec in encoding: '%s'\n", bytespec);
			abort();
		};
	};
};

int isR_RM(x86_Operand *opA, x86_Operand *opB, int expect8)
{
	if ((opA->type == OPTYPE_GPR) || (opA->type == OPTYPE_REXGPR))
	{
		if (expect8)
		{
			if (opA->gpr.opsz != 8) return 0;
		}
		else
		{
			if (opA->gpr.opsz == 8) return 0;
		};
		
		
		if (opA->gpr.opsz > x86_bits) return 0;
		
		if ((opB->type == OPTYPE_GPR) || (opB->type == OPTYPE_REXGPR))
		{
			if (opA->type != opB->type) return 0;
			
			if (opB->gpr.opsz != opA->gpr.opsz) return 0;
			else return 1;
		};
		
		if (opB->type == OPTYPE_MEMREF)
		{
			if (opB->memref.opsz == opA->gpr.opsz) return 1;
		};
	};
	
	return 0;
};

static int isRM_I(x86_Operand *opA, x86_Operand *opB, int opsz)
{
	switch (opA->type)
	{
	case OPTYPE_GPR:
	case OPTYPE_REXGPR:
		if (opA->gpr.opsz != opsz) return 0;
		break;
	case OPTYPE_MEMREF:
		if (opA->memref.opsz != opsz) return 0;
		break;
	default:
		return 0;
	};
	
	switch (opB->type)
	{
	case OPTYPE_IMM:
		return 1;
	case OPTYPE_OFFSET:
		return opB->offset.opsz == opsz;
		break;
	default:
		return 0;
	};
};

static int isRM64_I32(x86_Operand *opA, x86_Operand *opB)
{
	switch (opA->type)
	{
	case OPTYPE_GPR:
	case OPTYPE_REXGPR:
		if (opA->gpr.opsz != 64) return 0;
		break;
	case OPTYPE_MEMREF:
		if (opA->memref.opsz != 64) return 0;
		break;
	default:
		return 0;
	};
	
	switch (opB->type)
	{
	case OPTYPE_IMM:
		return 1;
	case OPTYPE_OFFSET:
		return opB->offset.opsz == 32;
		break;
	default:
		return 0;
	};
};

static int isRM_I8(x86_Operand *opA, x86_Operand *opB)
{
	switch (opA->type)
	{
	case OPTYPE_GPR:
	case OPTYPE_REXGPR:
		break;
	case OPTYPE_MEMREF:
		break;
	default:
		return 0;
	};
	
	switch (opB->type)
	{
	case OPTYPE_IMM:
		return 1;
	case OPTYPE_OFFSET:
		return opB->offset.opsz == 8;
		break;
	default:
		return 0;
	};
};

int x86_OpTypeMatch(int types, x86_Operand *opA, x86_Operand *opB)
{
	int typeA = opA->type;
	int typeB = opB->type;
	
	switch (types)
	{
	case INSN_NONE:
		return (typeA == OPTYPE_NONE) && (typeB == OPTYPE_NONE);
	case INSN_R:
		return ((typeA == OPTYPE_GPR) || (typeA == OPTYPE_REXGPR)) && (typeB == OPTYPE_NONE);
	case INSN_IB:
		return ((typeA == OPTYPE_IMM) || ((typeA == OPTYPE_OFFSET) && (opA->offset.opsz == 8)))
				&& (typeB == OPTYPE_NONE);
	case INSN_IW:
		return ((typeA == OPTYPE_IMM) || ((typeA == OPTYPE_OFFSET) && (opA->offset.opsz == 16)))
				&& (typeB == OPTYPE_NONE);
	case INSN_ID:
		return ((typeA == OPTYPE_IMM) || ((typeA == OPTYPE_OFFSET) && (opA->offset.opsz == 32)))
				&& (typeB == OPTYPE_NONE);
	case INSN_IQ:
		return ((typeA == OPTYPE_IMM) || ((typeA == OPTYPE_OFFSET) && (opA->offset.opsz == 64)))
				&& (typeB == OPTYPE_NONE);
	case INSN_R8_I8:
		return ((typeA == OPTYPE_GPR) || (typeA == OPTYPE_REXGPR))
				&& (opA->gpr.opsz == 8)
				&& ((typeB == OPTYPE_IMM) || ((typeB == OPTYPE_OFFSET) && (opB->offset.opsz == 8)));
	case INSN_R16_I16:
		return ((typeA == OPTYPE_GPR) || (typeA == OPTYPE_REXGPR))
				&& (opA->gpr.opsz == 16)
				&& ((typeB == OPTYPE_IMM) || ((typeB == OPTYPE_OFFSET) && (opB->offset.opsz == 16)));
	case INSN_R32_I32:
		return ((typeA == OPTYPE_GPR) || (typeA == OPTYPE_REXGPR))
				&& (opA->gpr.opsz == 32)
				&& ((typeB == OPTYPE_IMM) || ((typeB == OPTYPE_OFFSET) && (opB->offset.opsz == 32)));
	case INSN_R64_I64:
		return ((typeA == OPTYPE_GPR) || (typeA == OPTYPE_REXGPR))
				&& (opA->gpr.opsz == 64)
				&& ((typeB == OPTYPE_IMM) || ((typeB == OPTYPE_OFFSET) && (opB->offset.opsz == 64)));
	case INSN_R64_I32:
		return ((typeA == OPTYPE_GPR) || (typeA == OPTYPE_REXGPR))
				&& (opA->gpr.opsz == 64)
				&& ((typeB == OPTYPE_IMM) || ((typeB == OPTYPE_OFFSET) && (opB->offset.opsz == 32)));
	case INSN_R8_RM8:
		return isR_RM(opA, opB, 1);
	case INSN_R_RM:
		return isR_RM(opA, opB, 0);
	case INSN_R16_RM8:
		return ((typeA == OPTYPE_GPR) || (typeA == OPTYPE_REXGPR)) && (opA->gpr.opsz == 16) && (((typeB == OPTYPE_MEMREF) && (opB->memref.opsz == 8)) || 
			(((typeB == OPTYPE_GPR) || (typeB == OPTYPE_REXGPR)) && (opB->gpr.opsz == 8)));
	case INSN_R32_RM8:
		return ((typeA == OPTYPE_GPR) || (typeA == OPTYPE_REXGPR)) && (opA->gpr.opsz == 32) && (((typeB == OPTYPE_MEMREF) && (opB->memref.opsz == 8)) || 
			(((typeB == OPTYPE_GPR) || (typeB == OPTYPE_REXGPR)) && (opB->gpr.opsz == 8)));
	case INSN_R64_RM8:
		return ((typeA == OPTYPE_GPR) || (typeA == OPTYPE_REXGPR)) && (opA->gpr.opsz == 64) && (((typeB == OPTYPE_MEMREF) && (opB->memref.opsz == 8)) || 
			(((typeB == OPTYPE_GPR) || (typeB == OPTYPE_REXGPR)) && (opB->gpr.opsz == 8)));
	case INSN_R32_RM16:
		return ((typeA == OPTYPE_GPR) || (typeA == OPTYPE_REXGPR)) && (opA->gpr.opsz == 32) && (((typeB == OPTYPE_MEMREF) && (opB->memref.opsz == 16)) || 
			(((typeB == OPTYPE_GPR) || (typeB == OPTYPE_REXGPR)) && (opB->gpr.opsz == 16)));
	case INSN_R64_RM16:
		return ((typeA == OPTYPE_GPR) || (typeA == OPTYPE_REXGPR)) && (opA->gpr.opsz == 64) && (((typeB == OPTYPE_MEMREF) && (opB->memref.opsz == 16)) || 
			(((typeB == OPTYPE_GPR) || (typeB == OPTYPE_REXGPR)) && (opB->gpr.opsz == 16)));
	case INSN_R64_RM32:
		return ((typeA == OPTYPE_GPR) || (typeA == OPTYPE_REXGPR)) && (opA->gpr.opsz == 64) && (((typeB == OPTYPE_MEMREF) && (opB->memref.opsz == 32)) || 
			(((typeB == OPTYPE_GPR) || (typeB == OPTYPE_REXGPR)) && (opB->gpr.opsz == 32)));
	case INSN_MM_GPRM32:
		return (typeA == OPTYPE_XMM) && (opA->xmm.opsz == 64) && (
			(((typeB == OPTYPE_GPR) || (typeB == OPTYPE_REXGPR)) && (opB->gpr.opsz == 32))
			|| ((typeB == OPTYPE_MEMREF) && (opB->memref.opsz == 32))
			);
	case INSN_MM_GPRM64:
		return (typeA == OPTYPE_XMM) && (opA->xmm.opsz == 64) && (
			(((typeB == OPTYPE_GPR) || (typeB == OPTYPE_REXGPR)) && (opB->gpr.opsz == 64))
			|| ((typeB == OPTYPE_MEMREF) && (opB->memref.opsz == 64))
			);
	case INSN_GPRM32_MM:
		return (typeB == OPTYPE_XMM) && (opB->xmm.opsz == 64) && (
			(((typeA == OPTYPE_GPR) || (typeA == OPTYPE_REXGPR)) && (opA->gpr.opsz == 32))
			|| ((typeA == OPTYPE_MEMREF) && (opA->memref.opsz == 32))
			);
	case INSN_GPRM64_MM:
		return (typeB == OPTYPE_XMM) && (opB->xmm.opsz == 64) && (
			(((typeA == OPTYPE_GPR) || (typeA == OPTYPE_REXGPR)) && (opA->gpr.opsz == 64))
			|| ((typeA == OPTYPE_MEMREF) && (opA->memref.opsz == 64))
			);
	case INSN_XMM_GPRM32:
		return (typeA == OPTYPE_XMM) && (opA->xmm.opsz == 128) && (
			(((typeB == OPTYPE_GPR) || (typeB == OPTYPE_REXGPR)) && (opB->gpr.opsz == 32))
			|| ((typeB == OPTYPE_MEMREF) && (opB->memref.opsz == 32))
			);
	case INSN_XMM_GPRM64:
		return (typeA == OPTYPE_XMM) && (opA->xmm.opsz == 128) && (
			(((typeB == OPTYPE_GPR) || (typeB == OPTYPE_REXGPR)) && (opB->gpr.opsz == 64))
			|| ((typeB == OPTYPE_MEMREF) && (opB->memref.opsz == 64))
			);
	case INSN_GPRM32_XMM:
		return (typeB == OPTYPE_XMM) && (opB->xmm.opsz == 128) && (
			(((typeA == OPTYPE_GPR) || (typeA == OPTYPE_REXGPR)) && (opA->gpr.opsz == 32))
			|| ((typeA == OPTYPE_MEMREF) && (opA->memref.opsz == 32))
			);
	case INSN_GPRM64_XMM:
		return (typeB == OPTYPE_XMM) && (opB->xmm.opsz == 128) && (
			(((typeA == OPTYPE_GPR) || (typeA == OPTYPE_REXGPR)) && (opA->gpr.opsz == 64))
			|| ((typeA == OPTYPE_MEMREF) && (opA->memref.opsz == 64))
			);
	case INSN_GPR_XMM:
		return (typeB == OPTYPE_XMM) && (opB->xmm.opsz == 128) &&
			(((typeA == OPTYPE_GPR) || (typeA == OPTYPE_REXGPR)) && ((opA->gpr.opsz == 32) || (opA->gpr.opsz == 64)));
	case INSN_R_RM_NO16:
		return isR_RM(opA, opB, 0) && (opA->gpr.opsz != 16);
	case INSN_RM8_R8:
		return ((typeA == OPTYPE_MEMREF) && ((typeB == OPTYPE_GPR) || (typeB == OPTYPE_REXGPR)))
			&& (opA->memref.opsz == opB->gpr.opsz) && (opA->memref.opsz == 8);
	case INSN_RM_R:
		return ((typeA == OPTYPE_MEMREF) && ((typeB == OPTYPE_GPR) || (typeB == OPTYPE_REXGPR)))
			&& (opA->memref.opsz == opB->gpr.opsz) && (opA->memref.opsz != 8);
	case INSN_XMM_RM:
		return (typeA == OPTYPE_XMM) && ((typeB == OPTYPE_XMM)
			|| ((typeB == OPTYPE_MEMREF) && (opB->memref.opsz == 128)));
	case INSN_RM_XMM:
		return ((typeA == OPTYPE_MEMREF) && (opA->memref.opsz == 128)) && (typeB == OPTYPE_XMM);
	case INSN_RM8_I8:
		return isRM_I(opA, opB, 8);
	case INSN_RM16_I16:
		return isRM_I(opA, opB, 16);
	case INSN_RM32_I32:
		return isRM_I(opA, opB, 32) || isRM64_I32(opA, opB);
	case INSN_RM_I8:
		return isRM_I8(opA, opB);
	case INSN_RM8:
		return (typeB == OPTYPE_NONE) && (((typeA == OPTYPE_MEMREF) && (opA->memref.opsz == 8)
			) || ((typeA == OPTYPE_GPR || typeA == OPTYPE_REXGPR) && opA->gpr.opsz == 8));
	case INSN_RM:
		return (typeB == OPTYPE_NONE) && (((typeA == OPTYPE_MEMREF) && (opA->memref.opsz != 8)
			) || ((typeA == OPTYPE_GPR || typeA == OPTYPE_REXGPR) && opA->gpr.opsz != 8));
	case INSN_RM_FIXED:
		return (typeB == OPTYPE_NONE) && (typeA == OPTYPE_MEMREF);
	case INSN_IMM:
		return (typeB == OPTYPE_NONE) && ((typeA == OPTYPE_OFFSET) || (typeA == OPTYPE_IMM));
	case INSN_REL8:
		return (typeA == OPTYPE_OFFSET) && (opA->offset.opsz == 8) && (typeB == OPTYPE_NONE);
	case INSN_REL:
		return (typeA == OPTYPE_OFFSET) && (opA->offset.opsz == 16 || opA->offset.opsz == 32) && (typeB == OPTYPE_NONE);
	case INSN_ST:
		return (typeA == OPTYPE_ST) && (typeB == OPTYPE_NONE);
	case INSN_ST0_ST:
		return (typeA == OPTYPE_ST) && (typeB == OPTYPE_ST) && (opA->st.num == 0);
	case INSN_ST_ST0:
		return (typeA == OPTYPE_ST) && (typeB == OPTYPE_ST) && (opB->st.num == 0);
	case INSN_RM_INT16:
		return (typeA == OPTYPE_MEMREF) && (opA->memref.opsz == 16) && (typeB == OPTYPE_NONE);
	case INSN_RM_INT32:
		return (typeA == OPTYPE_MEMREF) && (opA->memref.opsz == 32) && (typeB == OPTYPE_NONE);
	case INSN_RM_INT64:
		return (typeA == OPTYPE_MEMREF) && (opA->memref.opsz == 64) && (typeB == OPTYPE_NONE);
	case INSN_RM_FP32:
		return (typeA == OPTYPE_MEMREF) && (opA->memref.opsz == OPSZ_FLOAT) && (typeB == OPTYPE_NONE);
	case INSN_RM_FP64:
		return (typeA == OPTYPE_MEMREF) && (opA->memref.opsz == OPSZ_DOUBLE) && (typeB == OPTYPE_NONE);
	case INSN_RM_FP80:
		return (typeA == OPTYPE_MEMREF) && (opA->memref.opsz == OPSZ_FPUWORD) && (typeB == OPTYPE_NONE);
	case INSN_AX:
		return (typeA == OPTYPE_GPR) && (opA->gpr.num == 0) && (opA->gpr.opsz == 16) && (typeB == OPTYPE_NONE);
	case INSN_I8_AL:
		return (typeB == OPTYPE_GPR) && (opB->gpr.num == 0) && (opB->gpr.opsz == 8) && ((typeA == OPTYPE_OFFSET && (opA->offset.opsz == opB->gpr.opsz)) || (typeA == OPTYPE_IMM));
	case INSN_I8_AX:
		return (typeB == OPTYPE_GPR) && (opB->gpr.num == 0) && (opB->gpr.opsz == 16) && ((typeA == OPTYPE_OFFSET && (opA->offset.opsz == 8)) || (typeA == OPTYPE_IMM));
	case INSN_I8_EAX:
		return (typeB == OPTYPE_GPR) && (opB->gpr.num == 0) && (opB->gpr.opsz == 32) && ((typeA == OPTYPE_OFFSET && (opA->offset.opsz == 8)) || (typeA == OPTYPE_IMM));
	case INSN_AL_I:
		return (typeA == OPTYPE_GPR) && (opA->gpr.num == 0) && (opA->gpr.opsz == 8) && ((typeB == OPTYPE_OFFSET && (opB->offset.opsz == opA->gpr.opsz)) || (typeB == OPTYPE_IMM));
	case INSN_AX_I:
		return (typeA == OPTYPE_GPR) && (opA->gpr.num == 0) && (opA->gpr.opsz == 16) && ((typeB == OPTYPE_OFFSET && (opB->offset.opsz == opA->gpr.opsz)) || (typeB == OPTYPE_IMM));
	case INSN_EAX_I:
		return (typeA == OPTYPE_GPR || typeA == OPTYPE_REXGPR) && (opA->gpr.num == 0) && (opA->gpr.opsz == 32 || opA->gpr.opsz == 64) && ((typeB == OPTYPE_OFFSET && (opB->offset.opsz == 32)) || (typeB == OPTYPE_IMM));
	case INSN_AX_I8:
		return (typeA == OPTYPE_GPR) && (opA->gpr.num == 0) && (opA->gpr.opsz == 16) && ((typeB == OPTYPE_OFFSET && (opB->offset.opsz == 8)) || ((typeB == OPTYPE_IMM) && (((opB->imm.value & ~0x7FL) == 0) || ((opB->imm.value & ~0x7FL) == ~0x7FL))));
	case INSN_EAX_I8:
		return (typeA == OPTYPE_GPR) && (opA->gpr.num == 0) && (opA->gpr.opsz == 32) && ((typeB == OPTYPE_OFFSET && (opB->offset.opsz == 8)) || ((typeB == OPTYPE_IMM) && (((opB->imm.value & ~0x7FL) == 0) || ((opB->imm.value & ~0x7FL) == ~0x7FL))));
	case INSN_AL_DX:
		return (typeA == OPTYPE_GPR) && (opA->gpr.num == 0) && (opA->gpr.opsz == 8) && (typeB == OPTYPE_GPR) && (opB->gpr.num == 2) && (opB->gpr.opsz == 16);
	case INSN_AX_DX:
		return (typeA == OPTYPE_GPR) && (opA->gpr.num == 0) && (opA->gpr.opsz == 16) && (typeB == OPTYPE_GPR) && (opB->gpr.num == 2) && (opB->gpr.opsz == 16);
	case INSN_EAX_DX:
		return (typeA == OPTYPE_GPR) && (opA->gpr.num == 0) && (opA->gpr.opsz == 32) && (typeB == OPTYPE_GPR) && (opB->gpr.num == 2) && (opB->gpr.opsz == 16);
	case INSN_DX_AL:
		return (typeB == OPTYPE_GPR) && (opB->gpr.num == 0) && (opB->gpr.opsz == 8) && (typeA == OPTYPE_GPR) && (opA->gpr.num == 2) && (opA->gpr.opsz == 16);
	case INSN_DX_AX:
		return (typeB == OPTYPE_GPR) && (opB->gpr.num == 0) && (opB->gpr.opsz == 16) && (typeA == OPTYPE_GPR) && (opA->gpr.num == 2) && (opA->gpr.opsz == 16);
	case INSN_DX_EAX:
		return (typeB == OPTYPE_GPR) && (opB->gpr.num == 0) && (opB->gpr.opsz == 32) && (typeA == OPTYPE_GPR) && (opA->gpr.num == 2) && (opA->gpr.opsz == 16);
	case INSN_INT3:
		return (typeB == OPTYPE_NONE) && (typeA == OPTYPE_IMM) && (opA->imm.value == 3);
	case INSN_MM_XMMRM:
		return (typeA == OPTYPE_XMM) && (opA->xmm.opsz == 64) && (
			((typeB == OPTYPE_XMM) && (opB->xmm.opsz == 128))
			|| ((typeB == OPTYPE_MEMREF) && (opB->memref.opsz == 128))
			);
	case INSN_MM_RM:
		return (typeA == OPTYPE_XMM) && (opA->xmm.opsz == 64) && (
			((typeB == OPTYPE_XMM) && (opB->xmm.opsz == 64))
			|| ((typeB == OPTYPE_MEMREF) && (opB->memref.opsz == 64))
			);
	case INSN_RM_MM:
		return (((typeA == OPTYPE_XMM) && (opA->xmm.opsz == 64))
			|| ((typeA == OPTYPE_MEMREF) && (opA->memref.opsz == 64))
			) && (typeB == OPTYPE_XMM) && (opB->xmm.opsz == 64);
	case INSN_XMM_MM:
		return (typeA == OPTYPE_XMM) && (opA->xmm.opsz == 128) && (((typeB == OPTYPE_XMM) && (opB->xmm.opsz == 64)));
	case INSN_RM16_SREG:
		return (((typeA == OPTYPE_MEMREF) && (opA->memref.opsz == 16)) || ((typeA == OPTYPE_GPR || typeA == OPTYPE_REXGPR) && (opA->gpr.opsz == 16))) && (typeB == OPTYPE_SREG);
	case INSN_RM64_SREG:
		return (((typeA == OPTYPE_MEMREF) && (opA->memref.opsz == 64)) || ((typeA == OPTYPE_GPR || typeA == OPTYPE_REXGPR) && (opA->gpr.opsz == 64))) && (typeB == OPTYPE_SREG);
	case INSN_SREG_RM16:
		return (((typeB == OPTYPE_MEMREF) && (opB->memref.opsz == 16)) || ((typeB == OPTYPE_GPR || typeB == OPTYPE_REXGPR) && (opB->gpr.opsz == 16))) && (typeA == OPTYPE_SREG);
	case INSN_SREG_RM64:
		return (((typeB == OPTYPE_MEMREF) && (opB->memref.opsz == 64)) || ((typeB == OPTYPE_GPR || typeB == OPTYPE_REXGPR) && (opB->gpr.opsz == 64))) && (typeA == OPTYPE_SREG);
	case INSN_AL_OFFSET:
		return (typeA == OPTYPE_GPR) && (opA->gpr.num == 0) && (opA->gpr.opsz == 8)
			&& ((typeB == OPTYPE_MEMREF && x86_bits == 64) || (typeB == OPTYPE_MEMREF_ABSEG && x86_bits != 64))
			&& (opB->memref.opsz == 8) && (opB->memref.base == 0xFF) && (opB->memref.scale == 0);
	case INSN_A_OFFSET:
		return (typeA == OPTYPE_GPR || typeA == OPTYPE_REXGPR) && (opA->gpr.num == 0) && (opA->gpr.opsz != 8)
			&& ((typeB == OPTYPE_MEMREF && x86_bits == 64) || (typeB == OPTYPE_MEMREF_ABSEG && x86_bits != 64))
			&& (opB->memref.opsz != 8) && (opB->memref.base == 0xFF) && (opB->memref.scale == 0)
			&& (opB->memref.opsz == opA->gpr.opsz);
	case INSN_OFFSET_AL:
		return (typeB == OPTYPE_GPR) && (opB->gpr.num == 0) && (opB->gpr.opsz == 8)
			&& ((typeA == OPTYPE_MEMREF && x86_bits == 64) || (typeA == OPTYPE_MEMREF_ABSEG && x86_bits != 64))
			&& (opA->memref.opsz == 8) && (opA->memref.base == 0xFF) && (opA->memref.scale == 0);
	case INSN_OFFSET_A:
		return (typeB == OPTYPE_GPR || typeB == OPTYPE_REXGPR) && (opB->gpr.num == 0) && (opB->gpr.opsz != 8)
			&& ((typeA == OPTYPE_MEMREF && x86_bits == 64) || (typeA == OPTYPE_MEMREF_ABSEG && x86_bits != 64))
			&& (opA->memref.opsz != 8) && (opA->memref.base == 0xFF) && (opA->memref.scale == 0)
			&& (opA->memref.opsz == opB->gpr.opsz);
	case INSN_R_CR:
		return (typeA == OPTYPE_GPR || typeA == OPTYPE_REXGPR) && (typeB == OPTYPE_CR);
	case INSN_CR_R:
		return (typeA == OPTYPE_CR) && (typeB == OPTYPE_GPR || typeB == OPTYPE_REXGPR);
	case INSN_R_DR:
		return (typeA == OPTYPE_GPR || typeA == OPTYPE_REXGPR) && (typeB == OPTYPE_DR);
	case INSN_DR_R:
		return (typeA == OPTYPE_DR) && (typeB == OPTYPE_GPR || typeB == OPTYPE_REXGPR);
	default:
		return 0;
	};
};

void tryRexConvert(x86_Operand *op)
{
	if (op->type == OPTYPE_GPR)
	{
		if (op->gpr.opsz == 8)
		{
			if (op->gpr.num < 4)
			{
				op->type = OPTYPE_REXGPR;
			};
		}
		else
		{
			if (op->gpr.num < 8)
			{
				op->type = OPTYPE_REXGPR;
			};
		};
	};
};

int x86_MatchMnemonic(const char *spec, const char *mnemonic)
{
	if (spec[0] == '*')
	{
		if (mnemonic[0] == 'v') mnemonic++;
		spec++;
		return strcmp(spec, mnemonic) == 0;
	}
	else
	{
		return strcmp(spec, mnemonic) == 0;
	};
};

void FamilyAssemble(const char *filename, int lineno, char *line)
{
	char mnemonic[1024];
	char *put = mnemonic;
	
	while (strchr(" \t", *line) == NULL)
	{
		*put++ = *line++;
	};
	
	*put = 0;
	
	while ((strchr(" \t", *line) != NULL) && (*line != 0)) line++;
	
	x86_Operand opA, opB;
	opA.type = opB.type = OPTYPE_NONE;
	
	if (*line != 0)
	{
		char *commaPos = strchr(line, ',');
		if (commaPos != NULL)
		{
			*commaPos = 0;
			if (parseOp(commaPos+1, &opB) != 0)
			{
				asDiag(filename, lineno, ML_ERROR, "invalid operand '%s' in %d-bit mode\n", commaPos+1, x86_bits);
				return;
			};
		};
	
		if (parseOp(line, &opA) != 0)
		{
			asDiag(filename, lineno, ML_ERROR, "invalid operand '%s' in %d-bit mode\n", line, x86_bits);
			return;
		};
	};

	Section *sect = asGetSection();
	if (sect->type != SECTYPE_PROGBITS)
	{
		asDiag(filename, lineno, ML_ERROR, "code/data in non-progbits section\n");
		return;
	};

	// if a REXGPR or CR is used, try to convert the other to REXGPR
	if (opA.type == OPTYPE_REXGPR || opA.type == OPTYPE_CR)
	{
		tryRexConvert(&opB);
	}
	else if (opB.type == OPTYPE_REXGPR || opB.type == OPTYPE_CR)
	{
		tryRexConvert(&opA);
	};
	
	if (strcmp(mnemonic, "bits") == 0)
	{
		if (opA.type != OPTYPE_IMM || opB.type != OPTYPE_NONE)
		{
			asDiag(filename, lineno, ML_ERROR, "x86 directive 'bits' expects an immediate operand");
			return;
		};
		
		switch (opA.imm.value)
		{
		case 16:
		case 32:
		case 64:
			x86_bits = opA.imm.value;
			return;
		default:
			asDiag(filename, lineno, ML_ERROR, "the operand to 'bits' must be 16, 32 or 64");
			return;
		};
	};
	
	int insnFound = 0;
	InsnSpec *insn;
	for (insn=insnList; insn->mnemonic!=NULL; insn++)
	{
		int flagNeeded;
		switch (x86_bits)
		{
		case 16:
			flagNeeded = INSN_16;
			break;
		case 32:
			flagNeeded = INSN_32;
			break;
		case 64:
			flagNeeded = INSN_64;
			break;
		};
		
		if (insn->flags & flagNeeded)
		{
			if (x86_MatchMnemonic(insn->mnemonic, mnemonic))
			{
				insnFound = 1;
			
				if (x86_OpTypeMatch(insn->types, &opA, &opB))
				{
					x86_Emit(filename, lineno, insn->mspec, &opA, &opB, insn->flags, insn->types);
					return;
				};
			};
		};
	};
	
	if (insnFound)
	{
		asDiag(filename, lineno, ML_ERROR, "invalid combination of opcode and operands for '%s' in %d-bit mode\n", mnemonic, x86_bits);
	}
	else
	{
		asDiag(filename, lineno, ML_ERROR, "unrecognised mnemonic '%s' in %d-bit mode\n", mnemonic, x86_bits);
	};
};
