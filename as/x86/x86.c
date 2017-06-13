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

typedef union
{
	int type;
	
	struct
	{
		int type;		// OPTYPE_GPR
		uint8_t num;		// register number (3 bits)
		uint8_t	opsz;		// operand size in bits (8, 16, 32 or 64)
	} gpr;
	
	struct
	{
		int type;		// OPTYPE_REXGPR
		uint8_t num;		// register number (4 bits)
		uint8_t	opsz;		// operand size in bits (8, 16, 32, or 64)
	} rexgpr;
	
	struct
	{
		int type;		// OPTYPE_IMM
		long int value;
	} imm;
	
	struct
	{
		int type;		// OPTYPE_OFFSET
		char *symbol;		// referenced symbol
		int64_t addend;		// addend
		uint8_t opsz;		// operand size in bits (8, 16, 32 or 64)
	} offset;
	
	struct
	{
		int type;		// OPTYPE_OFFSET
		int opsz;		// operand size in bits (8, 16, 32 or 64)
		char *symbol;		// symbol (if any) or NULL
		int64_t off;		// value to add to symbol
		int scale;		// scale power (0 = 1, 1 = 2, 2 = 4, 3 = 8)
		uint8_t idx;		// index register number
		uint8_t base;		// base register number (0x10 = RIP, 0xFF = no base)
	} memref;
} x86_Operand;

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

	char *endptr;
	long int value = strtol(opspec, &endptr, 0);
	if (*endptr == 0)
	{
		op->imm.type = OPTYPE_IMM;
		op->imm.value = value;
		return 0;
	};
	
	// every other case: BYTE|WORD|DWORD|QWORD OFFSET|PTR ...
	uint8_t opsz;
	char *sizename = strtok(opspec, " \t");
	if (sizename == NULL) return -1;
	
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
	else
	{
		return -1;
	};
	
	char *typename = strtok(NULL, " \t");
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
		char *bracketBit = strchr(rest, '[');
		if (bracketBit != NULL) *bracketBit++ = 0;

		memset(op, 0, sizeof(x86_Operand));
		op->memref.type = OPTYPE_MEMREF;
		op->memref.base = 0xFF;			// no base register
		op->memref.idx = 0xFF;			// no index register
		op->memref.opsz = opsz;
		
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
		
		return 0;
	}
	else
	{
		return -1;
	};
};

void x86_EmitModRM(const char *filename, int lineno, x86_Operand *opA, x86_Operand *opB)
{
	Section *sect = asGetSection();
	
	if (((opA->type == OPTYPE_GPR) || (opA->type == OPTYPE_REXGPR)) && (opA->type == opB->type))
	{
		uint8_t modrm = 0xC0 | ((opA->gpr.num & 0x7) << 3) | (opB->gpr.num & 0x7);
		objSectionAppend(sect, &modrm, 1);
	}
	else if (((opA->type == OPTYPE_GPR) || (opA->type == OPTYPE_REXGPR)) && (opB->type == OPTYPE_MEMREF))
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
					
					ObjLE32 disp32 = OBJ_MAKE_LE32((uint32_t) opB->memref.off-4);
					objSectionAppend(sect, &disp32, 4);
				}
				else
				{
					objSectionReloc(sect, opB->memref.symbol,
						REL_DWORD, REL_X86_RELATIVE, opB->memref.off-4);
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
					
					ObjLE32 disp32 = OBJ_MAKE_LE32((uint32_t) opB->memref.off-4);
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
	char buffer[256];
	strcpy(buffer, mspec);
	
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
	};
	
	// emit the size override prefix is necessary
	if ((types == INSN_R16_I16) || (types == INSN_R32_I32) || (types == INSN_R8_RM8) || (types == INSN_R_RM))
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
	
	// emit REX if necessary and if not already mandated elsewhere in the instruction
	if (x86_bits == 64)
	{
		if (types == INSN_R_RM)
		{
			uint8_t rex = 0x40;
			if (opA->gpr.opsz == 64)
			{
				rex |= 0x08;	// REX.W
			};
		
			if (opA->gpr.num & 0x8)
			{
				rex |= 0x4;
			};
			
			if (opB->type == OPTYPE_REXGPR)
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
	
	char *bytespec;
	for (bytespec=strtok(buffer, " "); bytespec!=NULL; bytespec=strtok(NULL, " "))
	{
		uint8_t byte;
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
		else if (strcmp(bytespec, "/r") == 0)
		{
			x86_EmitModRM(filename, lineno, opA, opB);
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
	case INSN_RM8_R8:
		return ((typeA == OPTYPE_MEMREF) && ((typeB == OPTYPE_GPR) || (typeB == OPTYPE_REXGPR)))
			&& (opA->memref.opsz == opB->gpr.opsz) && (opA->memref.opsz == 8);
	case INSN_RM_R:
		return ((typeA == OPTYPE_MEMREF) && ((typeB == OPTYPE_GPR) || (typeB == OPTYPE_REXGPR)))
			&& (opA->memref.opsz == opB->gpr.opsz) && (opA->memref.opsz != 8);
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

	// if a REXGPR is used, try to convert the other to REXGPR
	if (opA.type == OPTYPE_REXGPR)
	{
		tryRexConvert(&opB);
	}
	else if (opB.type == OPTYPE_REXGPR)
	{
		tryRexConvert(&opA);
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
			if (strcmp(insn->mnemonic, mnemonic) == 0)
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
