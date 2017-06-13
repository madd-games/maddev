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

#include "asfamily.h"

/**
 * List of registers addressed without the REX prefix (this includes stuff like "ah", "bh", etc,
 * which can never be addressed with the REX prefix!).
 */
RegSpec gprList[8] = {
	{"al",	"ax",	"eax",	""},			// 000
	{"cl",	"cx",	"ecx",	""},			// 001
	{"dl",	"dx",	"edx",	""},			// 010
	{"bl",	"bx",	"ebx",	""},			// 011
	{"ah",	"sp",	"esp",	""},			// 100
	{"ch",	"bp",	"ebp",	""},			// 101
	{"dh",	"si",	"esi",	""},			// 110
	{"bh",	"di",	"edi",	""}			// 111
};

/**
 * List of registers addressed with the REX prefix. Using those forces the assembler to emit a REX
 * prefix and they cannot be used with REX-less registers (and AH, etc, cannot be addressed, and
 * instead we have SPL etc).
 */
RegSpec rexgprList[16] = {
	{"al",	"ax",	"eax",	"rax"},			// 0.000
	{"cl",	"cx",	"ecx",	"rcx"},			// 0.001
	{"dl",	"dx",	"edx",	"rdx"},			// 0.010
	{"bl",	"bx",	"ebx",	"rbx"},			// 0.011
	{"spl",	"sp",	"esp",	"rsp"},			// 0.100
	{"bpl",	"bp",	"ebp",	"rbp"},			// 0.101
	{"sil",	"si",	"esi",	"rsi"},			// 0.110
	{"dil",	"di",	"edi",	"rdi"},			// 0.111
	{"r8l",	"r8w",	"r8d",	"r8"},			// 1.000
	{"r9l",	"r9w",	"r9d",	"r9"},			// 1.001
	{"r10l","r10w",	"r10d",	"r10"},			// 1.010
	{"r11l","r11w",	"r11d",	"r11"},			// 1.011
	{"r12l","r12w",	"r12d",	"r12"},			// 1.100
	{"r13l","r13w",	"r13d",	"r13"},			// 1.101
	{"r14l","r14w",	"r14d",	"r14"},			// 1.110
	{"r15l","r15w",	"r15d",	"r15"},			// 1.111
};

/**
 * List of instructions. See comment in x86/asfamily.h for explanation of format.
 */
InsnSpec insnList[] = {
	// special pseudo-instructions, for inserting data etc. Those may be different for other
	// assembly families. see documentation.
	{"db",		INSN_IB,		"ib",			INSN_ALL_MODES},
	{"dw",		INSN_IW,		"iw",			INSN_ALL_MODES},
	{"dd",		INSN_ID,		"id",			INSN_ALL_MODES},
	{"dq",		INSN_IQ,		"iq",			INSN_ALL_MODES},
	
	// actual x86 instructions as defined by Intel/AMD
	{"aaa",		INSN_NONE,		"37",			INSN_LEG_ONLY},
	{"syscall",	INSN_NONE,		"0F 05",		INSN_64},
	{"pop",		INSN_R,			"58+r",			INSN_ALL_MODES | INSN_DEF64},
	{"push",	INSN_R,			"50+r",			INSN_ALL_MODES | INSN_DEF64},
	{"int",		INSN_IB,		"CD ib",		INSN_ALL_MODES},
	{"mov",		INSN_R8_I8,		"B0+r ib",		INSN_ALL_MODES},
	{"mov",		INSN_R16_I16,		"B8+r iw",		INSN_ALL_MODES},
	{"mov",		INSN_R32_I32,		"B8+r id",		INSN_ALL_MODES},
	{"mov",		INSN_R64_I64,		"B8+r iq",		INSN_ALL_MODES},
	{"movabs",	INSN_R64_I64,		"B8+r iq",		INSN_ALL_MODES},
	{"mov",		INSN_R8_RM8,		"8A /r",		INSN_ALL_MODES},
	{"mov",		INSN_R_RM,		"8B /r",		INSN_ALL_MODES},
	{"mov",		INSN_RM8_R8,		"88 /r",		INSN_ALL_MODES},
	{"mov",		INSN_RM_R,		"89 /r",		INSN_ALL_MODES},
	
	// LIST TERMINATOR
	{NULL, 0, NULL, 0}
};
