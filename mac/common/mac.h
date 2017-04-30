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

#ifndef MAC_H
#define	MAC_H

#include <inttypes.h>

/**
 * Types of (macro-)registers.
 */
enum
{
	REGTYPE_IB,			/* 8-bit signed */
	REGTYPE_UB,			/* 8-bit unsigned */
	REGTYPE_IW,			/* 16-bit signed */
	REGTYPE_UW,			/* 16-bit unsigned */
	REGTYPE_ID,			/* 32-bit signed */
	REGTYPE_UD,			/* 32-bit unsigned */
	REGTYPE_IQ,			/* 64-bit signed */
	REGTYPE_UQ,			/* 64-bit unsigned */
	REGTYPE_F,			/* 32-bit float */
	REGTYPE_FD,			/* 64-bit float (double) */
	
	REGTYPE_NUM,			/* number of types */
};

/**
 * Types of operands to macros.
 */
enum
{
	OPR_REG,			/* register */
	OPR_CONST,			/* numeric constant */
	OPR_SYMBOL,			/* symbol */
};

/**
 * Macro codes (must match macro names in mac.c!)
 */
enum
{
	MAC_RET,
	MAC_LABEL,
	MAC_MOV
};

/**
 * Macro indicating an invalid register specification.
 */
#define	REGSPEC_INVALID			0xFFFFFFFF

/**
 * Macros to get each part of a register specification.
 */
#define	REGSPEC_TYPE(rs)		((rs) >> 16)
#define	REGSPEC_INDEX(rs)		((rs) & 0xFFFF)

/**
 * Macro to make a register specification.
 */
#define	REGSPEC_MAKE(type, index)	(((type) << 16) | (index))

/**
 * Characters which delimit operands.
 */
#define	SPACE_CHARS			" \t"

/**
 * Procedure flags.
 */
#define	PROC_VARIADIC			(1 << 0)		/* function is variadic */
#define	PROC_LEAF			(1 << 1)		/* leaf function */

/**
 * A RegSpec (register specific) has the following format:
 * The top 16 bits specify the TYPE of register, and the low 16 bits
 * are its INDEX. The types are enumerated above. Each part may be
 * accessed with a macro: REGSPEC_TYPE() and REGSPEC_INDEX().
 *
 * A RegSpec may be formatted with REGSPEC_MAKE().
 */
typedef uint32_t RegSpec;

/**
 * Describes a macro operand.
 */
typedef union
{
	int				type;			/* OPR_* */
	
	/**
	 * OPR_REG
	 */
	struct
	{
		int			type;
		RegSpec			spec;
	} reg;
	
	/**
	 * OPR_CONST
	 */
	struct
	{
		int			type;
		int			value;
	} con;
	
	/**
	 * OPR_SYMBOL
	 */
	struct
	{
		int			type;
		char*			name;
	} symbol;
} Operand;

/**
 * Describes a macro.
 */
typedef struct Macro_
{
	struct Macro_*			next;
	
	/**
	 * Macro type (MAC_*).
	 */
	int				type;
	
	/**
	 * Which line it came from.
	 */
	int				lineno;
	
	/**
	 * List of operands.
	 */
	Operand*			ops;
	size_t				numOps;
} Macro;

/**
 * Describes a procedure; a named list of macros.
 */
typedef struct
{
	/**
	 * Name of the procedure (loaded with strdup()).
	 */
	char*				name;
	
	/**
	 * Name of the calling convention (loaded with strdup()), as specified
	 * with the -conv= option. If NULL, the default convention for the target
	 * shall be used.
	 */
	char*				conv;
	
	/**
	 * Flags (PROC_*).
	 */
	int				flags;
	
	/**
	 * List of macros forming this procedure.
	 */
	Macro*				first;
	Macro*				last;
	
	/**
	 * The registers into which arguments shall be placed.
	 */
	RegSpec*			argRegs;
	size_t				numArgs;
	
	/**
	 * Starting line (where the 'proc' declaration is placed).
	 */
	int				lineno;
} Proc;

#endif
