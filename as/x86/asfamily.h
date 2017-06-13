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

#ifndef ASFAMILY_H_
#define ASFAMILY_H_

#include "liblex.h"
#include "as.h"

/**
 * Instruction flags.
 */
#define	INSN_16				(1 << 0)
#define	INSN_32				(1 << 1)
#define	INSN_64				(1 << 2)
#define	INSN_DEF64			(1 << 3)	// REX.W not needed for 64-bit operand in 64-bit mode

#define	INSN_ALL_MODES			(INSN_16 | INSN_32 | INSN_64)
#define	INSN_LEG_ONLY			(INSN_16 | INSN_32)

/**
 * Comment character for x86
 */
#define	ASFAMILY_COMMENT_PREFIX		";"

enum
{
	OPTYPE_NONE,
	OPTYPE_GPR,
	OPTYPE_REXGPR,
	OPTYPE_IMM,
	OPTYPE_OFFSET,
	OPTYPE_MEMREF,
};

/**
 * Types of operands in machine code.
 */
enum
{
	INSN_NONE,			// no operand
	INSN_R,				// single register operand
	
	/**
	 * For immediate operands, list the instructions in preferred size order; usually 32-bit first
	 * etc.
	 */
	INSN_IB,
	INSN_IW,
	INSN_ID,
	INSN_IQ,
	
	/**
	 * 8/16/32/64-bit register and immediate operand of a matching size.
	 */
	INSN_R8_I8,
	INSN_R16_I16,
	INSN_R32_I32,
	INSN_R64_I64,
	
	/**
	 * 64-bit register and 32-bit immediate value (most instructions that accept immediate operands).
	 */
	INSN_R64_I32,
	
	/**
	 * Register and r/m (8-bit).
	 */
	INSN_R8_RM8,
	
	/**
	 * Register and r/m (not 8-bit).
	 */
	INSN_R_RM,
	
	/**
	 * r/m and register (8-bit)
	 */
	INSN_RM8_R8,
	
	/**
	 * r/m and register (not 8-bit)
	 */
	INSN_RM_R,
};

/**
 * Instruction specification.
 */
typedef struct
{
	/**
	 * Mnemonic.
	 */
	const char *mnemonic;
	
	/**
	 * Operand types (INSN_*).
	 */
	int types;
	
	/**
	 * Machine code specification. Single-space-separated list of bytes/entries.
	 * XX		; a 2-digit hex number specifies a literal byte. Example: 05
	 * XX+r		; (single register operand) the byte XX with register number added.
	 * /r		; ModR/M and SIB/displacement if necessary
	 * ib/iw/id/iq	; immediate byte/word/dword/quadword
	 */
	const char *mspec;
	
	/**
	 * Flags.
	 */
	int flags;
} InsnSpec;

/**
 * Register specification.
 */
typedef struct
{
	const char *name8;
	const char *name16;
	const char *name32;
	const char *name64;
} RegSpec;

extern RegSpec gprList[8];
extern RegSpec rexgprList[16];

extern InsnSpec insnList[];

/**
 * Assemble an x86 instruction line. Print warnings/errors if necessary.
 */
void FamilyAssemble(const char *filename, int lineno, char *line);

#endif
