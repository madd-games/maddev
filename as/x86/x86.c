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
} x86_Operand;

int x86_bits = X86_DEFAULT_BITS;

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
	else
	{
		return -1;
	};
};

void x86_EmitModRM(x86_Operand *opA, x86_Operand *opB)
{
	Section *sect = asGetSection();
	
	if (((opA->type == OPTYPE_GPR) || (opA->type == OPTYPE_REXGPR)) && (opA->type == opB->type))
	{
		uint8_t modrm = 0xC0 | ((opA->gpr.num & 0x7) << 3) | (opB->gpr.num & 0x7);
		objSectionAppend(sect, &modrm, 1);
	};
};

void x86_Emit(const char *filename, int lineno, const char *mspec, x86_Operand *opA, x86_Operand *opB, int flags, int types)
{
	Section *sect = asGetSection();
	char buffer[256];
	strcpy(buffer, mspec);
	
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
			x86_EmitModRM(opA, opB);
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
		
		// TODO: memory reference
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
