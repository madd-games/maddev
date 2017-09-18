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
	OPTYPE_XMM,
	OPTYPE_IMM,
	OPTYPE_OFFSET,
	OPTYPE_MEMREF,
	OPTYPE_MEMREF_ABSEG,		// memory reference with absolute segment
	OPTYPE_ST,
	OPTYPE_SREG,
	OPTYPE_CR,
	OPTYPE_DR,
};

#define OPSZ_FLOAT			0xF0
#define	OPSZ_DOUBLE			0xF1
#define	OPSZ_FPUWORD			0xF2

/**
 * NOTE: gpr, rexgpr, xmm, sreg and cr must have the same format to help with ModR/M bytes.
 */
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
		int type;		// OPTYPE_XMM
		uint8_t num;		// register number (4 bits)
		uint8_t opsz;		// operand size in bits (64, 128, 256, 512)
	} xmm;
	
	struct
	{
		int type;		// OPTYPE_SREG
		uint8_t num;		// register number
		uint8_t opsz;		// always 16, so that the ModRM emitter can be more general
		uint8_t prefix;		// segment override prefix
	} sreg;

	struct
	{
		int type;		// OPTYPE_CR
		uint8_t num;		// register number
	} cr;

	struct
	{
		int type;		// OPTYPE_DR
		uint8_t num;		// register number
	} dr;

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
		int type;		// OPTYPE_MEMREF or OPTYPE_MEMREF_ABSEG
		int opsz;		// operand size in bits (8, 16, 32 or 64)
		char *symbol;		// symbol (if any) or NULL
		int64_t off;		// value to add to symbol
		int scale;		// scale power (0 = 1, 1 = 2, 2 = 4, 3 = 8)
		uint8_t idx;		// index register number
		uint8_t base;		// base register number (0x10 = RIP, 0xFF = no base)
		
		/**
		 * This field has a different meaning depending on 'type':
		 * OPTYPE_MEMREF - 8-bit segment override prefix to use, or 0 if no segment override needed.
		 * OPTYPE_MEMREF_ABSEG - the absolute segment
		 */
		uint16_t segment;
	} memref;
	
	struct
	{
		int type;		// OPTYPE_ST
		uint8_t num;		// register number (3 bits)
	} st;
} x86_Operand;

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
	 * Register and r/m (neither 8-bit nor 16-bit).
	 */
	INSN_R_RM_NO16,
	
	/**
	 * Register and r/m (neither 8-bit nor 16-bit).
	 */
	INSN_RM_R_NO16,
	
	/**
	 * r/m and register (8-bit)
	 */
	INSN_RM8_R8,
	
	/**
	 * r/m and register (not 8-bit)
	 */
	INSN_RM_R,
	
	/**
	 * r/m and immediate value operand of a matching size
	 * NOTE: INSN_RM32_I32 also accepts a 64-bit r/m with an 32-bit
	 * sign-extended immediate.
	 */
	INSN_RM8_I8,
	INSN_RM16_I16,
	INSN_RM32_I32,
	
	/**
	 * r/m (any size) + 8-bit immediate operand.
	 */
	INSN_RM_I8,
	
	/**
	 * specific register and immediate operand of a matching size.
	 * NOTE: INSN_EAX_I also accepts RAX, with 32-bit sign-extended immediates.
	 */
	INSN_AL_I,
	INSN_AX_I,
	INSN_EAX_I,
	
	/**
	 * specific register + imm8
	 */
	INSN_AX_I8,
	INSN_EAX_I8,
	
	/**
	 * specific register operand
	 */
	INSN_AX,
	
	/**
	 * xmm register (or 2 of them) + r/m
	 */
	INSN_XMM_RM,
	
	/**
	 * r/m + xmm register
	 */
	INSN_RM_XMM,
	
	/**
	 * XMM register + r/m + imm8 for some weird instructions.
	 */
	INSN_XMM_RM_IMM8,
	
	/**
	 * r/m + XMM register + imm8 for some weird instructions.
	 */
	INSN_RM_XMM_IMM8,
	
	/**
	 * XMM + XMM + r/m + XMM
	 */
	INSN_XMM4,
	
	/**
	 * BND + r/m
	 */
	INSN_BND_RM,
	
	/**
	 * BND + MIB
	 */
	INSN_BND_MIB,
	INSN_MIB_BND,
	
	/**
	 * BND + BND/M
	 */
	INSN_BND_BNDM,
	INSN_BNDM_BND,
	
	/**
	 * far absolute address
	 */
	INSN_IMM_FAR,
	
	/**
	 * single 8-bit r/m operand
	 */
	INSN_RM8,
	
	/**
	 * single r/m operand that is not 8-bit, with explicit size
	 */
	INSN_RM,
	
	/**
	 * single r/m operand of implied size
	 */
	INSN_RM_FIXED,
	
	/**
	 * MMX + XMM/rm
	 */
	INSN_MM_XMMRM,
	
	/**
	 * MMX + RM
	 */
	INSN_MM_RM,
	
	/**
	 * RM + MMX
	 */
	INSN_RM_MM,
	
	/**
	 * MMX + 32-bit RM or GPR
	 */
	INSN_MM_GPRM32,
	
	/**
	 * MMX + 64-bit RM or GPR
	 */
	INSN_MM_GPRM64,
	
	/**
	 * 32-bit RM or GPR + MMX
	 */
	INSN_GPRM32_MM,
	
	/**
	 * 64-bit RM or GPR + MMX
	 */
	INSN_GPRM64_MM,
	
	/**
	 * XMM + 32-bit RM or GPR
	 */
	INSN_XMM_GPRM32,
	
	/**
	 * XMM + 64-bit RM or GPR
	 */
	INSN_XMM_GPRM64,
	
	/**
	 * 32-bit RM or GPR + XMM
	 */
	INSN_GPRM32_XMM,
	
	/**
	 * 64-bit RM or GPR + XMM
	 */
	INSN_GPRM64_XMM,
	
	/**
	 * 64-bit or 32-bit GPR + XMM
	 */
	INSN_GPR_XMM,
	
	/**
	 * Single immediate operand of any size.
	 */
	INSN_IMM,
	
	/**
	 * 8-bit relative offset.
	 */
	INSN_REL8,
	
	/**
	 * Relative offset operand; 16- or 32-bit.
	 */
	INSN_REL,
	
	/**
	 * Combinations of various FPU stack operands (ST = any ST, ST0 = only ST0).
	 */
	INSN_ST,
	INSN_ST0_ST,
	INSN_ST_ST0,
	
	/**
	 * Memory references to specific types of operands.
	 */
	INSN_RM_FP32,
	INSN_RM_FP64,
	INSN_RM_FP80,
	
	INSN_RM_INT16,
	INSN_RM_INT32,
	INSN_RM_INT64,
	
	/**
	 * Register (non-8-bit) + r/m (matching size to register) + immediate value (also matching size if not 8-bit)
	 */
	INSN_R_RM_I8,
	INSN_R_RM_I16,
	INSN_R_RM_I32,
	
	/**
	 * Register (non-8-bit) + r/m (matching size to register) + immediate value (also matching size if not 8-bit)
	 */
	INSN_AL_DX,
	INSN_AX_DX,
	INSN_EAX_DX,
	
	/**
	 * Weird thingy for int instruction
	 */
	INSN_INT3,
	
	/**
	 * r/m and segment registers
	 */
	INSN_RM16_SREG,
	INSN_RM64_SREG,
	INSN_SREG_RM16,
	INSN_SREG_RM64,
	
	/**
	 * "A" register + memory references in the form of immediate addresses; 64-bit address,
	 * or a segment:offset pair (with size depending on mode).
	 * AL indicates 8-bit data; for just "A", it is implicit based on "66" and "REX".
	 */
	INSN_AL_OFFSET,
	INSN_A_OFFSET,
	INSN_OFFSET_AL,
	INSN_OFFSET_A,
	
	/**
	 * Normal registers and control registers.
	 */
	INSN_R_CR,
	INSN_CR_R,
	
	/**
	 * Normal registers and debug registers.
	 */
	INSN_R_DR,
	INSN_DR_R,
};

/**
 * Instruction specification.
 */
typedef struct
{
	/**
	 * Mnemonic. If it starts with "*", an optional "v" may be present at the beginning.
	 * (for example *addpd matches 'addpd' and 'vaddpd').
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

/**
 * Segment register specification.
 */
typedef struct
{
	const char *name;			/* name of the register (NULL = list terminator) */
	uint8_t num;				/* register number, for "sreg" fields */
	uint8_t prefix;				/* segment override prefix for this segment */
} SregSpec;

extern RegSpec gprList[8];
extern RegSpec rexgprList[16];
extern SregSpec sregList[];
extern InsnSpec insnList[];

/**
 * Assemble an x86 instruction line. Print warnings/errors if necessary.
 */
void FamilyAssemble(const char *filename, int lineno, char *line);

#endif
