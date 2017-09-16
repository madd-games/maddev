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
 * Segment registers.
 */
SregSpec sregList[] = {
	{"es",	0,	0x26},
	{"cs",	1,	0x2E},
	{"ss",	2,	0x36},
	{"ds",	3,	0x3E},
	{"fs",	4,	0x64},
	{"gs",	5,	0x65},
	
	// LIST TERMINATOR
	{NULL}
};

/**
 * List of instructions. See comment in x86/asfamily.h for explanation of format.
 */
InsnSpec insnList[] = {
	// special pseudo-instructions, for inserting data etc. Those may be different for other
	// assembly families. see documentation.
	{"db",			INSN_IB,		"ib",					INSN_ALL_MODES},
	{"dw",			INSN_IW,		"iw",					INSN_ALL_MODES},
	{"dd",			INSN_ID,		"id",					INSN_ALL_MODES},
	{"dq",			INSN_IQ,		"iq",					INSN_ALL_MODES},
	
	// actual x86 instructions as defined by Intel/AMD
	{"aaa",			INSN_NONE,		"37",					INSN_LEG_ONLY},
	{"aad",			INSN_NONE,		"D5 0A",				INSN_LEG_ONLY},
	{"aad",			INSN_IB,		"D5 ib",				INSN_LEG_ONLY},
	{"aam",			INSN_NONE,		"D4 0A",				INSN_LEG_ONLY},
	{"aam",			INSN_IB,		"D4 ib",				INSN_LEG_ONLY},
	{"aas",			INSN_NONE,		"3F",					INSN_LEG_ONLY},
	{"adc",			INSN_AL_I,		"14 ib",				INSN_ALL_MODES},
	{"adc",			INSN_AX_I,		"15 iw",				INSN_ALL_MODES},
	{"adc",			INSN_EAX_I,		"15 id",				INSN_ALL_MODES},
	{"adc",			INSN_RM8_I8,		"80 /2.1 ib",				INSN_ALL_MODES},
	{"adc",			INSN_RM16_I16,		"81 /2.2 iw",				INSN_ALL_MODES},
	{"adc",			INSN_RM32_I32,		"81 /2.4 id",				INSN_ALL_MODES},
	{"adc",			INSN_RM8_R8,		"10 /r.0",				INSN_ALL_MODES},
	{"adc",			INSN_RM_R,		"11 /r.0",				INSN_ALL_MODES},
	{"adc",			INSN_R8_RM8,		"12 /r.0",				INSN_ALL_MODES},
	{"adc",			INSN_R_RM,		"13 /r.0",				INSN_ALL_MODES},
	{"adcx",		INSN_R_RM_NO16,		"66 0F 38 F6 /r.0",			INSN_ALL_MODES},
	{"add",			INSN_AL_I,		"04 ib",				INSN_ALL_MODES},
	{"add",			INSN_AX_I,		"05 iw",				INSN_ALL_MODES},
	{"add",			INSN_EAX_I,		"05 id",				INSN_ALL_MODES},
	{"add",			INSN_RM8_I8,		"80 /0.1 ib",				INSN_ALL_MODES},
	{"add",			INSN_RM16_I16,		"81 /0.2 iw",				INSN_ALL_MODES},
	{"add",			INSN_RM32_I32,		"81 /0.4 id",				INSN_ALL_MODES},
	{"add",			INSN_RM8_R8,		"00 /r.0",				INSN_ALL_MODES},
	{"add",			INSN_RM_R,		"01 /r.0",				INSN_ALL_MODES},
	{"add",			INSN_R8_RM8,		"02 /r.0",				INSN_ALL_MODES},
	{"add",			INSN_R_RM,		"03 /r.0",				INSN_ALL_MODES},
	{"*addpd",		INSN_XMM_RM,		"66 0F 58 /r.0",			INSN_ALL_MODES},
	{"*addps",		INSN_XMM_RM,		"0F 58 /r.0",				INSN_ALL_MODES},
	{"*addsd",		INSN_XMM_RM,		"F2 0F 58 /r.0",			INSN_ALL_MODES},
	{"*addss",		INSN_XMM_RM,		"F3 0F 58 /r.0",			INSN_ALL_MODES},
	{"*addsubpd",		INSN_XMM_RM,		"66 0F D0 /r.0",			INSN_ALL_MODES},
	{"*addsubps",		INSN_XMM_RM,		"F2 0F D0 /r.0",			INSN_ALL_MODES},
	{"adox",		INSN_R_RM_NO16,		"F3 0F 38 F6 /r.0",			INSN_ALL_MODES},
	{"*aesdec",		INSN_XMM_RM,		"66 0F 38 DE /r.0",			INSN_ALL_MODES},
	{"*aesdeclast",		INSN_XMM_RM,		"66 0F 38 DF /r.0",			INSN_ALL_MODES},
	{"*aesenc",		INSN_XMM_RM,		"66 0F 38 DC /r.0",			INSN_ALL_MODES},
	{"*aesenclast",		INSN_XMM_RM,		"66 0F 38 DD /r.0",			INSN_ALL_MODES},
	{"*aesimc",		INSN_XMM_RM,		"66 0F 38 DB /r.0",			INSN_ALL_MODES},
	{"*aeskeygenassist",	INSN_XMM_RM_IMM8,	"66 0F 3A DF /r.1 ib",			INSN_ALL_MODES},
	{"and",			INSN_AL_I,		"24 ib",				INSN_ALL_MODES},
	{"and",			INSN_AX_I,		"25 iw",				INSN_ALL_MODES},
	{"and",			INSN_EAX_I,		"25 id",				INSN_ALL_MODES},
	{"and",			INSN_RM8_I8,		"80 /4.1 ib",				INSN_ALL_MODES},
	{"and",			INSN_RM16_I16,		"81 /4.2 iw",				INSN_ALL_MODES},
	{"and",			INSN_RM32_I32,		"81 /4.4 id",				INSN_ALL_MODES},
	{"and",			INSN_RM8_R8,		"20 /r.0",				INSN_ALL_MODES},
	{"and",			INSN_RM_R,		"21 /r.0",				INSN_ALL_MODES},
	{"and",			INSN_R8_RM8,		"22 /r.0",				INSN_ALL_MODES},
	{"and",			INSN_R_RM,		"23 /r.0",				INSN_ALL_MODES},
	{"andn",		INSN_R_RM,		"VEX.NDS.LZ.0F38.W0 F2 /r.0",		INSN_ALL_MODES}, // TODO?
	{"*andpd",		INSN_XMM_RM,		"66 0F 54 /r.0",			INSN_ALL_MODES},
	{"*andps",		INSN_XMM_RM,		"0F 54 /r.0",				INSN_ALL_MODES},
	{"*andnpd",		INSN_XMM_RM,		"66 0F 55 /r.0",			INSN_ALL_MODES},
	{"*andnps",		INSN_XMM_RM,		"0F 55 /r.0",				INSN_ALL_MODES},
	{"arpl",		INSN_RM_R,		"63 /r.0",				INSN_LEG_ONLY},
	{"*blendpd",		INSN_XMM_RM_IMM8,	"66 0F 3A 0D /r.1 ib",			INSN_ALL_MODES},
	{"bextr",		INSN_R_RM,		"VEX.NDS.LZ.0F38.W0 F7 /r.0",		INSN_ALL_MODES},
	{"*blendps",		INSN_XMM_RM_IMM8,	"66 0F 3A 0C /r.1 ib",			INSN_ALL_MODES},
	{"blendvpd",		INSN_XMM_RM,		"66 0F 38 15 /r.0",			INSN_ALL_MODES},
	{"vblendvpd",		INSN_XMM4,		"VEX.NDS.128.66.0F3A.W0 4B /r /is4",	INSN_ALL_MODES},
	{"blendvps",		INSN_XMM_RM,		"66 0F 38 14 /r.0",			INSN_ALL_MODES},
	{"vblendvps",		INSN_XMM4,		"VEX.NDS.128.66.0F3A.W0 4A /r /is4",	INSN_ALL_MODES},
	{"blsi",		INSN_R_RM,		"VEX.NDD.LZ.0F38.W0 F3 /3.0",		INSN_ALL_MODES},
	{"blsmk",		INSN_R_RM,		"VEX.NDD.LZ.0F38.W0 F3 /2.0",		INSN_ALL_MODES},
	{"blsr",		INSN_R_RM,		"VEX.NDD.LZ.0F38.W0 F3 /1.0",		INSN_ALL_MODES},
	{"bndcl",		INSN_BND_RM,		"F3 0F 1A /r.0",			INSN_ALL_MODES},
	{"bndcu",		INSN_BND_RM,		"F2 0F 1A /r.0",			INSN_ALL_MODES},
	{"bndcn",		INSN_BND_RM,		"F2 0F 1B /r.0",			INSN_ALL_MODES},
	{"bndldx",		INSN_BND_MIB,		"0F 1A /r.0",				INSN_ALL_MODES},
	{"bndmk",		INSN_BND_RM,		"F3 0F 1B /r.0",			INSN_ALL_MODES},
	{"bndmov",		INSN_BND_BNDM,		"66 0F 1A /r.0",			INSN_ALL_MODES},
	{"bndmov",		INSN_BNDM_BND,		"66 0F 1B /r.0",			INSN_ALL_MODES},
	{"bndstx",		INSN_MIB_BND,		"0F 1B /r.0",				INSN_ALL_MODES},
	{"bound",		INSN_R_RM,		"62 /r.0",				INSN_LEG_ONLY},
	{"bsf",			INSN_R_RM,		"0F BC /r.0",				INSN_ALL_MODES},
	{"bsr",			INSN_R_RM,		"0F BD /r.0",				INSN_ALL_MODES},
	{"bswap",		INSN_R,			"0F C8+r",				INSN_ALL_MODES},
	{"bt",			INSN_RM_R,		"0F A3 /r.0",				INSN_ALL_MODES},
	{"bt",			INSN_RM_I8,		"0F BA /4.1 ib",			INSN_ALL_MODES},
	{"btc",			INSN_RM_R,		"0F BB /r.0",				INSN_ALL_MODES},
	{"btc",			INSN_RM_I8,		"0F BA /7.1 ib",			INSN_ALL_MODES},
	{"btr",			INSN_RM_R,		"0F B3 /r.0",				INSN_ALL_MODES},
	{"btr",			INSN_RM_I8,		"0F BA /6.1 ib",			INSN_ALL_MODES},
	{"bts",			INSN_RM_R,		"0F AB /r.0",				INSN_ALL_MODES},
	{"bts",			INSN_RM_I8,		"0F BA /5.1 ib",			INSN_ALL_MODES},
	// TODO: BZHI
	{"call",		INSN_REL,		"E8 rel.0",				INSN_ALL_MODES},
	{"call",		INSN_RM,		"FF /2.0",				INSN_ALL_MODES | INSN_DEF64},
	{"call",		INSN_IMM_FAR,		"9A ifar",				INSN_LEG_ONLY},
	{"callf",		INSN_RM,		"FF /3.0",				INSN_ALL_MODES}, // far call
	{"cbw",			INSN_NONE,		"66 98",				INSN_32 | INSN_64},
	{"cwde",		INSN_NONE,		"98",					INSN_32 | INSN_64},
	{"cbw",			INSN_NONE,		"98",					INSN_16},
	{"cwde",		INSN_NONE,		"6698",					INSN_16},
	{"cdqe",		INSN_NONE,		"48 98",				INSN_64},
	{"clac",		INSN_NONE,		"0F 01 CA",				INSN_ALL_MODES},
	{"clc",			INSN_NONE,		"F8",					INSN_ALL_MODES},
	{"cld",			INSN_NONE,		"FC",					INSN_ALL_MODES},
	{"clflush",		INSN_RM_FIXED,		"0F AE /7.0",				INSN_ALL_MODES},
	{"clflushopt",		INSN_RM_FIXED,		"66 0F AE /7.0",			INSN_ALL_MODES},
	{"cli",			INSN_NONE,		"FA",					INSN_ALL_MODES},
	{"clts",		INSN_NONE,		"0F 06",				INSN_ALL_MODES},
	{"clwb",		INSN_RM_FIXED,		"66 0F AE /6.0",			INSN_ALL_MODES},
	{"cmc",			INSN_NONE,		"F5",					INSN_ALL_MODES},
	{"cmova",		INSN_R_RM,		"0F 47 /r.0",				INSN_ALL_MODES},
	{"cmovae",		INSN_R_RM,		"0F 43 /r.0",				INSN_ALL_MODES},
	{"cmovb",		INSN_R_RM,		"0F 42 /r.0",				INSN_ALL_MODES},
	{"cmovbe",		INSN_R_RM,		"0F 46 /r.0",				INSN_ALL_MODES},
	{"cmovc",		INSN_R_RM,		"0F 42 /r.0",				INSN_ALL_MODES},
	{"cmove",		INSN_R_RM,		"0F 44 /r.0",				INSN_ALL_MODES},
	{"cmovg",		INSN_R_RM,		"0F 4F /r.0",				INSN_ALL_MODES},
	{"cmovge",		INSN_R_RM,		"0F 4D /r.0",				INSN_ALL_MODES},
	{"cmovl",		INSN_R_RM,		"0F 4C /r.0",				INSN_ALL_MODES},
	{"cmovle",		INSN_R_RM,		"0F 4E /r.0",				INSN_ALL_MODES},
	{"cmovna",		INSN_R_RM,		"0F 46 /r.0",				INSN_ALL_MODES},
	{"cmovnb",		INSN_R_RM,		"0F 43 /r.0",				INSN_ALL_MODES},
	{"cmovnbe",		INSN_R_RM,		"0F 47 /r.0",				INSN_ALL_MODES},
	{"cmovnae",		INSN_R_RM,		"0F 42 /r.0",				INSN_ALL_MODES},
	{"cmovnc",		INSN_R_RM,		"0F 43 /r.0",				INSN_ALL_MODES},
	{"cmovne",		INSN_R_RM,		"0F 45 /r.0",				INSN_ALL_MODES},
	{"cmovng",		INSN_R_RM,		"0F 4E /r.0",				INSN_ALL_MODES},
	{"cmovnge",		INSN_R_RM,		"0F 4C /r.0",				INSN_ALL_MODES},
	{"cmovnl",		INSN_R_RM,		"0F 4D /r.0",				INSN_ALL_MODES},
	{"cmovnle",		INSN_R_RM,		"0F 4F /r.0",				INSN_ALL_MODES},
	{"cmovno",		INSN_R_RM,		"0F 41 /r.0",				INSN_ALL_MODES},
	{"cmovnp",		INSN_R_RM,		"0F 4B /r.0",				INSN_ALL_MODES},
	{"cmovns",		INSN_R_RM,		"0F 49 /r.0",				INSN_ALL_MODES},
	{"cmovnz",		INSN_R_RM,		"0F 45 /r.0",				INSN_ALL_MODES},
	{"cmovo",		INSN_R_RM,		"0F 40 /r.0",				INSN_ALL_MODES},
	{"cmovp",		INSN_R_RM,		"0F 4A /r.0",				INSN_ALL_MODES},
	{"cmovpe",		INSN_R_RM,		"0F 4A /r.0",				INSN_ALL_MODES},
	{"cmovpo",		INSN_R_RM,		"0F 4B /r.0",				INSN_ALL_MODES},
	{"cmovs",		INSN_R_RM,		"0F 48 /r.0",				INSN_ALL_MODES},
	{"cmovz",		INSN_R_RM,		"0F 44 /r.0",				INSN_ALL_MODES},
	{"cmp",			INSN_AL_I,		"3C ib",				INSN_ALL_MODES},
	{"cmp",			INSN_AX_I,		"3D iw",				INSN_ALL_MODES},
	{"cmp",			INSN_EAX_I,		"3D id",				INSN_ALL_MODES},
	{"cmp",			INSN_RM8_I8,		"80 /7.1 ib",				INSN_ALL_MODES},
	{"cmp",			INSN_RM16_I16,		"81 /7.2 iw",				INSN_ALL_MODES},
	{"cmp",			INSN_RM32_I32,		"81 /7.4 id",				INSN_ALL_MODES},
	{"cmp",			INSN_RM_I8,		"83 /7.1 ib",				INSN_ALL_MODES},
	{"cmp",			INSN_RM8_R8,		"38 /r.0",				INSN_ALL_MODES},
	{"cmp",			INSN_RM_R,		"39 /r.0",				INSN_ALL_MODES},
	{"cmp",			INSN_R8_RM8,		"3A /r.0",				INSN_ALL_MODES},
	{"cmp",			INSN_R_RM,		"3B /r.0",				INSN_ALL_MODES},
	{"cmp",			INSN_RM8_R8,		"38 /r.0",				INSN_ALL_MODES},
	{"*cmppd",		INSN_XMM_RM_IMM8,	"66 0F C2 /r.1 ib",			INSN_ALL_MODES},
	{"*cmpps",		INSN_XMM_RM_IMM8,	"0F C2 /r.1 ib",			INSN_ALL_MODES},
	{"cmps",		INSN_NONE,		"A6",					INSN_ALL_MODES},
	{"cmps",		INSN_NONE,		"A7",					INSN_ALL_MODES},
	{"cmpsb",		INSN_NONE,		"A6",					INSN_ALL_MODES},
	{"cmpsw",		INSN_NONE,		"A7",					INSN_ALL_MODES},
	{"cmpsd",		INSN_NONE,		"A7",					INSN_ALL_MODES},
	{"cmpsq",		INSN_NONE,		"48 A7",				INSN_64},
	{"*cmpsd",		INSN_XMM_RM_IMM8,	"F2 0F C2 /r.1 ib",			INSN_ALL_MODES},
	{"*cmpss",		INSN_XMM_RM_IMM8,	"F3 0F C2 /r.1 ib",			INSN_ALL_MODES},
	{"cmpxchg",		INSN_RM8_R8,		"0F B0 /r.0",				INSN_ALL_MODES},
	{"cmpxchg",		INSN_RM_R,		"0F B1 /r.0",				INSN_ALL_MODES},
	{"cmpxchg8b",		INSN_RM_FIXED,		"0F C7 /1.0",				INSN_ALL_MODES},
	{"cmpxchg16b",		INSN_RM_FIXED,		"48 0F C7 /1.0",			INSN_64},
	{"*comisd",		INSN_XMM_RM,		"66 0F 2F /r.0",			INSN_32 | INSN_64},
	{"*comiss",		INSN_XMM_RM,		"0F 2F /r.0",				INSN_ALL_MODES},
	{"cpuid",		INSN_NONE,		"0F A2",				INSN_ALL_MODES},
	// TODO: CRC32
	{"*cvtdq2pd",		INSN_XMM_RM,		"F3 0F E6 /r.0",			INSN_32 | INSN_64},
	{"*cvtdq2ps",		INSN_XMM_RM,		"0F 5B /r.0",				INSN_32 | INSN_64},
	{"*cvtpd2dq",		INSN_XMM_RM,		"F2 0F E6 /r.0",			INSN_32 | INSN_64},
	{"cvtpd2pi",		INSN_MM_XMMRM,		"66 0F 2D /r.0",			INSN_ALL_MODES},
	{"*cvtpd2ps",		INSN_XMM_RM,		"66 0F 5A /r.0",			INSN_32 | INSN_64},
	{"cvtpi2pd",		INSN_XMM_RM,		"66 0F 2A /r.0",			INSN_ALL_MODES},
	{"cvtpi2ps",		INSN_XMM_RM,		"0F 2A /r.0",				INSN_ALL_MODES},
	{"*cvtps2dq",		INSN_XMM_RM,		"66 0F 5B /r.0",			INSN_32 | INSN_64},
	{"*cvtps2pd",		INSN_XMM_RM,		"0F 5A /r.0",				INSN_32 | INSN_64},
	{"cvtps2pi",		INSN_MM_XMMRM,		"0F 2D /r.0",				INSN_ALL_MODES},
	{"*cvtsd2si",		INSN_XMM_RM,		"F2 0F 2D /r.0",			INSN_32 | INSN_64},
	{"*cvtsd2ss",		INSN_XMM_RM,		"F2 0F 5A /r.0",			INSN_32 | INSN_64},
	{"*cvtsi2sd",		INSN_XMM_RM,		"F2 0F 2A /r.0",			INSN_32 | INSN_64},
	{"*cvtsi2ss",		INSN_XMM_RM,		"F3 0F 2A /r.0",			INSN_32 | INSN_64},
	{"*cvtss2sd",		INSN_XMM_RM,		"F3 0F 5A /r.0",			INSN_32 | INSN_64},
	{"*cvtss2si",		INSN_XMM_RM,		"F3 0F 2D /r.0",			INSN_32 | INSN_64},
	{"*cvttpd2dq",		INSN_XMM_RM,		"66 0F E6 /r.0",			INSN_32 | INSN_64},
	{"cvttpd2pi",		INSN_MM_XMMRM,		"66 0F 2C /r.0",			INSN_ALL_MODES},
	{"*cvttps2dq",		INSN_XMM_RM,		"F3 0F 5B /r.0",			INSN_32 | INSN_64},
	{"cvttps2pi",		INSN_MM_XMMRM,		"0F 2C /r.0",				INSN_ALL_MODES},
	{"*cvttsd2si",		INSN_XMM_RM,		"F2 0F 2C /r.0",			INSN_32 | INSN_64},
	{"*cvttss2si",		INSN_XMM_RM,		"F3 0F 2C /r.0",			INSN_32 | INSN_64},
	{"cwd",			INSN_NONE,		"99",					INSN_ALL_MODES},
	{"cdq",			INSN_NONE,		"99",					INSN_ALL_MODES},
	{"cqo",			INSN_NONE,		"48 99",				INSN_64},
	{"daa",			INSN_NONE,		"27",					INSN_LEG_ONLY},
	{"das",			INSN_NONE,		"2F",					INSN_LEG_ONLY},
	{"dec",			INSN_RM8,		"FE /1.0",				INSN_ALL_MODES},
	{"dec",			INSN_RM,		"FF /1.0",				INSN_ALL_MODES},
	{"dec",			INSN_R,			"48+r",					INSN_LEG_ONLY},
	{"div",			INSN_RM8,		"F6 /6.0",				INSN_ALL_MODES},
	{"div",			INSN_RM,		"F7 /6.0",				INSN_ALL_MODES},
	{"*divpd",		INSN_XMM_RM,		"66 0F 5E /r.0",			INSN_32 | INSN_64},
	{"*divps",		INSN_XMM_RM,		"0F 5E /r.0",				INSN_32 | INSN_64},
	{"*divsd",		INSN_XMM_RM,		"F2 0F 5E /r.0",			INSN_32 | INSN_64},
	{"*divss",		INSN_XMM_RM,		"F3 0F 5E /r.0",			INSN_32 | INSN_64},
	{"*dppd",		INSN_XMM_RM_IMM8,	"66 0F 3A 41 /r.1 ib",			INSN_32 | INSN_64},
	{"*dpps",		INSN_XMM_RM_IMM8,	"66 0F 3A 40 /r.1 ib",			INSN_32 | INSN_64},
	{"emms",		INSN_NONE,		"0F 77",				INSN_ALL_MODES},
	// TODO: ENTER
	{"*extractps",		INSN_RM_XMM_IMM8,	"66 0F 3A 17 /r.1 ib",			INSN_32 | INSN_64},
	{"f2xm1",		INSN_NONE,		"D9 F0",				INSN_ALL_MODES},
	{"fabs",		INSN_NONE,		"D9 E1",				INSN_ALL_MODES},
	{"fadd",		INSN_RM_FP32,		"D8 /0.0",				INSN_ALL_MODES},
	{"fadd",		INSN_RM_FP64,		"DC /0.0",				INSN_ALL_MODES},
	{"fadd",		INSN_ST0_ST,		"D8 C0+i",				INSN_ALL_MODES},
	{"fadd",		INSN_ST_ST0,		"DC C0+i",				INSN_ALL_MODES},
	{"faddp",		INSN_ST_ST0,		"DE C0+i",				INSN_ALL_MODES},
	{"faddp",		INSN_NONE,		"DE C1",				INSN_ALL_MODES},
	{"fiadd",		INSN_RM_INT16,		"DA /0.0",				INSN_ALL_MODES},
	{"fiadd",		INSN_RM_INT32,		"DE /0.0",				INSN_ALL_MODES},
	// TODO: FBLD
	// TODO: FBSTP
	{"fchs",		INSN_NONE,		"D9 E0",				INSN_ALL_MODES},
	{"fclex",		INSN_NONE,		"9B DB E2",				INSN_ALL_MODES},
	{"fnclex",		INSN_NONE,		"DB E2",				INSN_ALL_MODES},
	{"fcmovb",		INSN_ST0_ST,		"DA C0+i",				INSN_ALL_MODES},
	{"fcmove",		INSN_ST0_ST,		"DA C8+i",				INSN_ALL_MODES},
	{"fcmovbe",		INSN_ST0_ST,		"DA D0+i",				INSN_ALL_MODES},
	{"fcmovu",		INSN_ST0_ST,		"DA D8+i",				INSN_ALL_MODES},
	{"fcmovnb",		INSN_ST0_ST,		"DB C0+i",				INSN_ALL_MODES},
	{"fcmovne",		INSN_ST0_ST,		"DB C8+i",				INSN_ALL_MODES},
	{"fcmovnbe",		INSN_ST0_ST,		"DB D0+i",				INSN_ALL_MODES},
	{"fcmovnu",		INSN_ST0_ST,		"DB D8+i",				INSN_ALL_MODES},
	{"fcom",		INSN_RM_FP32,		"D8 /2.0",				INSN_ALL_MODES},
	{"fcom",		INSN_RM_FP64,		"DC /2.0",				INSN_ALL_MODES},
	{"fcom",		INSN_ST,		"D8 D0+i",				INSN_ALL_MODES},
	{"fcom",		INSN_NONE,		"D8 D1",				INSN_ALL_MODES},
	{"fcomp",		INSN_RM_FP32,		"D8 /3.0",				INSN_ALL_MODES},
	{"fcomp",		INSN_RM_FP64,		"DC /3.0",				INSN_ALL_MODES},
	{"fcomp",		INSN_ST,		"D8 D8+i",				INSN_ALL_MODES},
	{"fcomp",		INSN_NONE,		"D8 D9",				INSN_ALL_MODES},
	{"fcompp",		INSN_NONE,		"DE D9",				INSN_ALL_MODES},
	{"fcomi",		INSN_ST0_ST,		"DB F0+i",				INSN_ALL_MODES},
	{"fcomip",		INSN_ST0_ST,		"DF F0+i",				INSN_ALL_MODES},
	{"fucomi",		INSN_ST0_ST,		"DB E8+i",				INSN_ALL_MODES},
	{"fucomip",		INSN_ST0_ST,		"DF E8+i",				INSN_ALL_MODES},
	{"fcos",		INSN_NONE,		"D9 FF",				INSN_ALL_MODES},
	{"fdcestp",		INSN_NONE,		"D9 F6",				INSN_ALL_MODES},
	{"fdiv",		INSN_RM_FP32,		"D8 /6.0",				INSN_ALL_MODES},
	{"fdiv",		INSN_RM_FP64,		"DC /6.0",				INSN_ALL_MODES},
	{"fdiv",		INSN_ST0_ST,		"D8 F0+i",				INSN_ALL_MODES},
	{"fdiv",		INSN_ST_ST0,		"DC F8+i",				INSN_ALL_MODES},
	{"fdivp",		INSN_ST_ST0,		"DE F8+i",				INSN_ALL_MODES},
	{"fdivp",		INSN_NONE,		"DE F9",				INSN_ALL_MODES},
	{"fidiv",		INSN_RM_INT32,		"DA /6.0",				INSN_ALL_MODES},
	{"fidiv",		INSN_RM_INT16,		"DE /6.0",				INSN_ALL_MODES},
	{"fdivr",		INSN_RM_FP32,		"D8 /7.0",				INSN_ALL_MODES},
	{"fdivr",		INSN_RM_FP64,		"DC /7.0",				INSN_ALL_MODES},
	{"fdivr",		INSN_ST0_ST,		"D8 F8+i",				INSN_ALL_MODES},
	{"fdivr",		INSN_ST_ST0,		"DC F0+i",				INSN_ALL_MODES},
	{"fdivrp",		INSN_ST_ST0,		"DE F0+i",				INSN_ALL_MODES},
	{"fdivrp",		INSN_NONE,		"DE F1",				INSN_ALL_MODES},
	{"fidivr",		INSN_RM_INT32,		"DA /7.0",				INSN_ALL_MODES},
	{"fidivr",		INSN_RM_INT16,		"DE /7.0",				INSN_ALL_MODES},
	{"ffree",		INSN_NONE,		"DD C0+i",				INSN_ALL_MODES},
	{"ficom",		INSN_RM_INT16,		"DE /2.0",				INSN_ALL_MODES},
	{"ficom",		INSN_RM_INT32,		"DA /2.0",				INSN_ALL_MODES},
	{"ficomp",		INSN_RM_INT16,		"DE /3.0",				INSN_ALL_MODES},
	{"ficomp",		INSN_RM_INT32,		"DA /3.0",				INSN_ALL_MODES},
	{"fild",		INSN_RM_INT16,		"DF /0.0",				INSN_ALL_MODES},
	{"fild",		INSN_RM_INT32,		"DB /0.0",				INSN_ALL_MODES},
	{"fild",		INSN_RM_INT64,		"DF /5.0",				INSN_ALL_MODES},
	{"fincstp",		INSN_NONE,		"D9 F7",				INSN_ALL_MODES},
	{"finit",		INSN_NONE,		"9B DB E3",				INSN_ALL_MODES},
	{"fninit",		INSN_NONE,		"DB E3",				INSN_ALL_MODES},
	{"fist",		INSN_RM_INT16,		"DF /2.0",				INSN_ALL_MODES},
	{"fist",		INSN_RM_INT32,		"DB /2.0",				INSN_ALL_MODES},
	{"fistp",		INSN_RM_INT16,		"DF /3.0",				INSN_ALL_MODES},
	{"fistp",		INSN_RM_INT32,		"DB /3.0",				INSN_ALL_MODES},
	{"fistp",		INSN_RM_INT64,		"DF /7.0",				INSN_ALL_MODES},
	{"fisttp",		INSN_RM_INT16,		"DF /1.0",				INSN_ALL_MODES},
	{"fisttp",		INSN_RM_INT32,		"DB /1.0",				INSN_ALL_MODES},
	{"fisttp",		INSN_RM_INT64,		"DD /1.0",				INSN_ALL_MODES},
	{"fld",			INSN_RM_FP32,		"D9 /0.0",				INSN_ALL_MODES},
	{"fld",			INSN_RM_FP64,		"DD /0.0",				INSN_ALL_MODES},
	{"fld",			INSN_RM_FP80,		"DB /5.0",				INSN_ALL_MODES},
	{"fld",			INSN_ST,		"DB /5.0",				INSN_ALL_MODES},
	{"fld1",		INSN_NONE,		"D9 E8",				INSN_ALL_MODES},
	{"fldl2t",		INSN_NONE,		"D9 E9",				INSN_ALL_MODES},
	{"fldl2e",		INSN_NONE,		"D9 EA",				INSN_ALL_MODES},
	{"fldpi",		INSN_NONE,		"D9 EB",				INSN_ALL_MODES},
	{"fldlg2",		INSN_NONE,		"D9 EC",				INSN_ALL_MODES},
	{"fldln2",		INSN_NONE,		"D9 ED",				INSN_ALL_MODES},
	{"fldz",		INSN_NONE,		"D9 EE",				INSN_ALL_MODES},
	{"fldcw",		INSN_RM_INT16,		"D9 /5.0",				INSN_ALL_MODES},
	{"fldenv",		INSN_RM_FIXED,		"D9 /5.0",				INSN_ALL_MODES},
	{"fmul",		INSN_RM_FP32,		"D8 /1.0",				INSN_ALL_MODES},
	{"fmul",		INSN_RM_FP64,		"DC /1.0",				INSN_ALL_MODES},
	{"fmul",		INSN_ST0_ST,		"D8 C8+i",				INSN_ALL_MODES},
	{"fmul",		INSN_ST_ST0,		"DC C8+i",				INSN_ALL_MODES},
	{"fmulp",		INSN_ST_ST0,		"DE C8+i",				INSN_ALL_MODES},
	{"fmulp",		INSN_NONE,		"DE C9",				INSN_ALL_MODES},
	{"fimul",		INSN_RM_INT32,		"DA /1.0",				INSN_ALL_MODES},
	{"fimul",		INSN_RM_INT16,		"DE /1.0",				INSN_ALL_MODES},
	{"fnop",		INSN_NONE,		"D9 D0",				INSN_ALL_MODES},
	{"fpatan",		INSN_NONE,		"D9 F3",				INSN_ALL_MODES},
	{"fprem",		INSN_NONE,		"D9 F8",				INSN_ALL_MODES},
	{"fprem1",		INSN_NONE,		"D9 F5",				INSN_ALL_MODES},
	{"fptan",		INSN_NONE,		"D9 F2",				INSN_ALL_MODES},
	{"frndint",		INSN_NONE,		"D9 FC",				INSN_ALL_MODES},
	{"frstor",		INSN_RM_FIXED,		"DD /4.0",				INSN_ALL_MODES},
	{"fsave",		INSN_RM_FIXED,		"9B DD /6.0",				INSN_ALL_MODES},
	{"fnsave",		INSN_RM_FIXED,		"DD /6.0",				INSN_ALL_MODES},
	{"fscale",		INSN_NONE,		"D9 FD",				INSN_ALL_MODES},
	{"fsin",		INSN_NONE,		"D9 FE",				INSN_ALL_MODES},
	{"fsincos",		INSN_NONE,		"D9 FB",				INSN_ALL_MODES},
	{"fsqrt",		INSN_NONE,		"D9 FA",				INSN_ALL_MODES},
	{"fst",			INSN_RM_FP32,		"D9 /2.0",				INSN_ALL_MODES},
	{"fst",			INSN_RM_FP64,		"DD /2.0",				INSN_ALL_MODES},
	{"fst",			INSN_ST,		"DD D0+i",				INSN_ALL_MODES},
	{"fstp",		INSN_RM_FP32,		"D9 /3.0",				INSN_ALL_MODES},
	{"fstp",		INSN_RM_FP64,		"DD /3.0",				INSN_ALL_MODES},
	{"fstp",		INSN_RM_FP80,		"DB /7.0",				INSN_ALL_MODES},
	{"fstp",		INSN_ST,		"DD D8+i",				INSN_ALL_MODES},
	{"fstcw",		INSN_RM_INT16,		"9B D9 /7.0",				INSN_ALL_MODES},
	{"fnstcw",		INSN_RM_INT16,		"D9 /7.0",				INSN_ALL_MODES},
	{"fstenv",		INSN_RM_FIXED,		"9B D9 /6.0",				INSN_ALL_MODES},
	{"fnstenv",		INSN_RM_FIXED,		"D9 /6.0",				INSN_ALL_MODES},
	{"fstsw",		INSN_RM_INT16,		"9B DD /7.0",				INSN_ALL_MODES},
	{"fstsw",		INSN_AX,		"9B DF E0",				INSN_ALL_MODES},
	{"fnstsw",		INSN_RM_INT16,		"DD /7.0",				INSN_ALL_MODES},
	{"fnstsw",		INSN_AX,		"DF E0",				INSN_ALL_MODES},
	{"fsub",		INSN_RM_FP32,		"D8 /4.0",				INSN_ALL_MODES},
	{"fsub",		INSN_RM_FP64,		"DC /4.0",				INSN_ALL_MODES},
	{"fsub",		INSN_ST0_ST,		"D8 E0+i",				INSN_ALL_MODES},
	{"fsub",		INSN_ST_ST0,		"DC E8+i",				INSN_ALL_MODES},
	{"fsubp",		INSN_ST_ST0,		"DE E8+i",				INSN_ALL_MODES},
	{"fsubp",		INSN_NONE,		"DE E9",				INSN_ALL_MODES},
	{"fisub",		INSN_RM_INT32,		"DA /4.0",				INSN_ALL_MODES},
	{"fisub",		INSN_RM_INT16,		"DE /4.0",				INSN_ALL_MODES},
	{"fsubr",		INSN_RM_FP32,		"D8 /5.0",				INSN_ALL_MODES},
	{"fsubr",		INSN_RM_FP64,		"DC /5.0",				INSN_ALL_MODES},
	{"fsubr",		INSN_ST0_ST,		"D8 E8+i",				INSN_ALL_MODES},
	{"fsubr",		INSN_ST_ST0,		"DC E0+i",				INSN_ALL_MODES},
	{"fsubpr",		INSN_ST_ST0,		"DE E0+i",				INSN_ALL_MODES},
	{"fsubpr",		INSN_NONE,		"DE E1",				INSN_ALL_MODES},
	{"fisubr",		INSN_RM_INT32,		"DA /5.0",				INSN_ALL_MODES},
	{"fisubr",		INSN_RM_INT16,		"DE /5.0",				INSN_ALL_MODES},
	{"ftst",		INSN_NONE,		"D9 E4",				INSN_ALL_MODES},
	{"fucom",		INSN_ST,		"DD E0+i",				INSN_ALL_MODES},
	{"fucom",		INSN_NONE,		"DD E1",				INSN_ALL_MODES},
	{"fucomp",		INSN_ST,		"DD E8+i",				INSN_ALL_MODES},
	{"fucomp",		INSN_NONE,		"DD E9",				INSN_ALL_MODES},
	{"fucompp",		INSN_NONE,		"DA E9",				INSN_ALL_MODES},
	{"fxam",		INSN_NONE,		"D9 E5",				INSN_ALL_MODES},
	{"fxch",		INSN_ST,		"D9 C8+i",				INSN_ALL_MODES},
	{"fxch",		INSN_NONE,		"D9 C9",				INSN_ALL_MODES},
	{"fxrstor",		INSN_RM_FIXED,		"0F AE /1.0",				INSN_ALL_MODES},
	{"fxsave",		INSN_RM_FIXED,		"0F AE /0.0",				INSN_ALL_MODES},
	{"fxtract",		INSN_NONE,		"D9 F4",				INSN_ALL_MODES},
	{"fyl2x",		INSN_NONE,		"D9 F1",				INSN_ALL_MODES},
	{"fyl2xp1",		INSN_NONE,		"D9 F9",				INSN_ALL_MODES},
	{"*haddpd",		INSN_XMM_RM,		"66 0F 7C /r.0",			INSN_32 | INSN_64},
	{"*haddps",		INSN_XMM_RM,		"F2 0F 7C /r.0",			INSN_32 | INSN_64},
	{"hlt",			INSN_NONE,		"F4",					INSN_ALL_MODES},
	{"*hsubpd",		INSN_XMM_RM,		"66 0F 7D /r.0",			INSN_32 | INSN_64},
	{"*hsubps",		INSN_XMM_RM,		"F2 0F 7D /r.0",			INSN_32 | INSN_64},
	{"idiv",		INSN_RM8,		"F6 /7.0",				INSN_ALL_MODES},
	{"idiv",		INSN_RM,		"F7 /7.0",				INSN_ALL_MODES},
	{"imul",		INSN_RM8,		"F6 /5.0",				INSN_ALL_MODES},
	{"imul",		INSN_RM,		"F7 /5.0",				INSN_ALL_MODES},
	{"imul",		INSN_R_RM,		"0F AF /r.0",				INSN_ALL_MODES},
	{"imul",		INSN_R_RM_I8,		"6B /r.1 ib",				INSN_ALL_MODES},
	{"imul",		INSN_R_RM_I16,		"69 /r.2 iw",				INSN_ALL_MODES},
	{"imul",		INSN_R_RM_I32,		"69 /r.4 id",				INSN_ALL_MODES},
	{"in",			INSN_AL_I,		"E4 ib",				INSN_ALL_MODES},
	{"in",			INSN_AX_I8,		"E5 ib",				INSN_ALL_MODES},
	{"in",			INSN_EAX_I8,		"E5 ib",				INSN_ALL_MODES},
	{"in",			INSN_AL_DX,		"EC",					INSN_ALL_MODES},
	{"in",			INSN_AX_DX,		"ED",					INSN_ALL_MODES},
	{"in",			INSN_EAX_DX,		"ED",					INSN_ALL_MODES},
	{"inc",			INSN_RM8,		"FE /0.0",				INSN_ALL_MODES},
	{"inc",			INSN_RM,		"FF /0.0",				INSN_ALL_MODES},
	{"inc",			INSN_R,			"40+ rw",				INSN_LEG_ONLY},
	{"inc",			INSN_R,			"40+ rd",				INSN_LEG_ONLY},
	{"insb",		INSN_NONE,		"6C",					INSN_ALL_MODES},
	{"insw",		INSN_NONE,		"6D",					INSN_ALL_MODES},
	{"insd",		INSN_NONE,		"6D",					INSN_ALL_MODES},
	{"*insertps",		INSN_XMM_RM_IMM8,	"66 0F 3A 21 /r.1 ib",			INSN_32 | INSN_64},
	{"int",			INSN_INT3,		"CC",					INSN_ALL_MODES},
	{"int",			INSN_IB,		"CD ib",				INSN_ALL_MODES},
	{"into",		INSN_NONE,		"CE",					INSN_LEG_ONLY},
	{"invd",		INSN_NONE,		"0F 08",				INSN_ALL_MODES},
	{"invplg",		INSN_RM_FIXED,		"0F 01 /7.0",				INSN_ALL_MODES},
	{"invpcid",		INSN_R_RM,		"66 0F 38 82 /r.0",			INSN_32 | INSN_64},
	{"iret",		INSN_NONE,		"CF",					INSN_LEG_ONLY},
	{"iret",		INSN_NONE,		"48 CF",				INSN_64},
	{"iretd",		INSN_NONE,		"CF",					INSN_ALL_MODES},
	{"iretq",		INSN_NONE,		"48 CF",				INSN_64},
	{"ja",			INSN_REL8,		"77 rel.0",				INSN_ALL_MODES},
	{"jae",			INSN_REL8,		"73 rel.0",				INSN_ALL_MODES},
	{"jb",			INSN_REL8,		"72 rel.0",				INSN_ALL_MODES},
	{"jbe",			INSN_REL8,		"76 rel.0",				INSN_ALL_MODES},
	{"jc",			INSN_REL8,		"72 rel.0",				INSN_ALL_MODES},
	{"jcxz",		INSN_REL8,		"E3 rel.0",				INSN_LEG_ONLY},
	{"jecxz",		INSN_REL8,		"E3 rel.0",				INSN_ALL_MODES},
	{"jrcxz",		INSN_REL8,		"E3 rel.0",				INSN_64},
	{"je",			INSN_REL8,		"74 rel.0",				INSN_ALL_MODES},
	{"jg",			INSN_REL8,		"7F rel.0",				INSN_ALL_MODES},
	{"jge",			INSN_REL8,		"7D rel.0",				INSN_ALL_MODES},
	{"jl",			INSN_REL8,		"7C rel.0",				INSN_ALL_MODES},
	{"jle",			INSN_REL8,		"7E rel.0",				INSN_ALL_MODES},
	{"jna",			INSN_REL8,		"76 rel.0",				INSN_ALL_MODES},
	{"jnae",		INSN_REL8,		"72 rel.0",				INSN_ALL_MODES},
	{"jnb",			INSN_REL8,		"73 rel.0",				INSN_ALL_MODES},
	{"jnbe",		INSN_REL8,		"77 rel.0",				INSN_ALL_MODES},
	{"jnc",			INSN_REL8,		"73 rel.0",				INSN_ALL_MODES},
	{"jne",			INSN_REL8,		"75 rel.0",				INSN_ALL_MODES},
	{"jng",			INSN_REL8,		"7E rel.0",				INSN_ALL_MODES},
	{"jnge",		INSN_REL8,		"7C rel.0",				INSN_ALL_MODES},
	{"jnl",			INSN_REL8,		"7D rel.0",				INSN_ALL_MODES},
	{"jnle",		INSN_REL8,		"7F rel.0",				INSN_ALL_MODES},
	{"jno",			INSN_REL8,		"71 rel.0",				INSN_ALL_MODES},
	{"jnp",			INSN_REL8,		"7B rel.0",				INSN_ALL_MODES},
	{"jns",			INSN_REL8,		"79 rel.0",				INSN_ALL_MODES},
	{"jnz",			INSN_REL8,		"75 rel.0",				INSN_ALL_MODES},
	{"jo",			INSN_REL8,		"70 rel.0",				INSN_ALL_MODES},
	{"jp",			INSN_REL8,		"7A rel.0",				INSN_ALL_MODES},
	{"jpe",			INSN_REL8,		"7A rel.0",				INSN_ALL_MODES},
	{"jpo",			INSN_REL8,		"7B rel.0",				INSN_ALL_MODES},
	{"js",			INSN_REL8,		"7B rel.0",				INSN_ALL_MODES},
	{"jz",			INSN_REL8,		"74 rel.0",				INSN_ALL_MODES},
	{"ja",			INSN_REL,		"0F 87 rel.0",				INSN_ALL_MODES},
	{"jae",			INSN_REL,		"0F 83 rel.0",				INSN_ALL_MODES},
	{"jb",			INSN_REL,		"0F 82 rel.0",				INSN_ALL_MODES},
	{"jbe",			INSN_REL,		"0F 86 rel.0",				INSN_ALL_MODES},
	{"jc",			INSN_REL,		"0F 82 rel.0",				INSN_ALL_MODES},
	{"je",			INSN_REL,		"0F 84 rel.0",				INSN_ALL_MODES},
	{"jg",			INSN_REL,		"0F 8F rel.0",				INSN_ALL_MODES},
	{"jge",			INSN_REL,		"0F 8D rel.0",				INSN_ALL_MODES},
	{"jl",			INSN_REL,		"0F 8C rel.0",				INSN_ALL_MODES},
	{"jle",			INSN_REL,		"0F 8E rel.0",				INSN_ALL_MODES},
	{"jna",			INSN_REL,		"0F 86 rel.0",				INSN_ALL_MODES},
	{"jnae",		INSN_REL,		"0F 82 rel.0",				INSN_ALL_MODES},
	{"jnb",			INSN_REL,		"0F 83 rel.0",				INSN_ALL_MODES},
	{"jnbe",		INSN_REL,		"0F 87 rel.0",				INSN_ALL_MODES},
	{"jnc",			INSN_REL,		"0F 83 rel.0",				INSN_ALL_MODES},
	{"jne",			INSN_REL,		"0F 85 rel.0",				INSN_ALL_MODES},
	{"jng",			INSN_REL,		"0F 8E rel.0",				INSN_ALL_MODES},
	{"jnge",		INSN_REL,		"0F 8C rel.0",				INSN_ALL_MODES},
	{"jnl",			INSN_REL,		"0F 8D rel.0",				INSN_ALL_MODES},
	{"jnle",		INSN_REL,		"0F 8F rel.0",				INSN_ALL_MODES},
	{"jno",			INSN_REL,		"0F 81 rel.0",				INSN_ALL_MODES},
	{"jnp",			INSN_REL,		"0F 8B rel.0",				INSN_ALL_MODES},
	{"jns",			INSN_REL,		"0F 89 rel.0",				INSN_ALL_MODES},
	{"jnz",			INSN_REL,		"0F 85 rel.0",				INSN_ALL_MODES},
	{"jo",			INSN_REL,		"0F 81 rel.0",				INSN_ALL_MODES},
	{"jp",			INSN_REL,		"0F 8A rel.0",				INSN_ALL_MODES},
	{"jpe",			INSN_REL,		"0F 8A rel.0",				INSN_ALL_MODES},
	{"jpo",			INSN_REL,		"0F 8B rel.0",				INSN_ALL_MODES},
	{"js",			INSN_REL,		"0F 8B rel.0",				INSN_ALL_MODES},
	{"jz",			INSN_REL,		"0F 84 rel.0",				INSN_ALL_MODES},
	{"jmp",			INSN_REL8,		"EB rel.0",				INSN_ALL_MODES},
	{"jmp",			INSN_REL,		"E9 rel.0",				INSN_ALL_MODES},
	{"jmp",			INSN_RM,		"FF /4.0",				INSN_ALL_MODES | INSN_DEF64},
	{"jmp",			INSN_IMM_FAR,		"EA ifar",				INSN_LEG_ONLY},
	{"jmpf",		INSN_RM,		"FF /5.0",				INSN_ALL_MODES},
	// TODO: The literal and absolute entirety of K instructions
	{"lahf",		INSN_NONE,		"9F",					INSN_ALL_MODES},
	{"lar",			INSN_R_RM,		"0F 02 /r.0",				INSN_ALL_MODES},
	{"*lddqu",		INSN_XMM_RM,		"F2 0F F0 /r.0",			INSN_32 | INSN_64},
	{"*ldmxcsr",		INSN_RM,		"0F AE /2.0",				INSN_32 | INSN_64},
	{"lds",			INSN_R_RM,		"C5 /r.0",				INSN_LEG_ONLY},
	{"lss",			INSN_R_RM,		"0F B2 /r.0",				INSN_ALL_MODES},
	{"les",			INSN_R_RM,		"C4 /r.0",				INSN_LEG_ONLY},
	{"lfs",			INSN_R_RM,		"0F B4 /r.0",				INSN_ALL_MODES},
	{"lgs",			INSN_R_RM,		"0F B5 /r.0",				INSN_ALL_MODES},
	{"lea",			INSN_R_RM,		"8D /r.0",				INSN_ALL_MODES},
	{"leave",		INSN_NONE,		"C9",					INSN_ALL_MODES},
	{"lfence",		INSN_NONE,		"0F AE E8",				INSN_ALL_MODES},
	{"lgdt",		INSN_RM_FIXED,		"0F 01 /2.0",				INSN_ALL_MODES},
	{"lidt",		INSN_RM_FIXED,		"0F 01 /3.0",				INSN_ALL_MODES},
	{"lldt",		INSN_RM_FIXED,		"0F 00 /2.0",				INSN_ALL_MODES},
	{"lmsw",		INSN_RM_FIXED,		"0F 01 /6.0",				INSN_ALL_MODES},
	{"lock",		INSN_NONE,		"F0",					INSN_ALL_MODES},
	{"lodsb",		INSN_NONE,		"AC",					INSN_ALL_MODES},
	{"lodsw",		INSN_NONE,		"AD",					INSN_ALL_MODES},
	{"lodsd",		INSN_NONE,		"AD",					INSN_ALL_MODES},
	{"lodsq",		INSN_NONE,		"48 AD",				INSN_64},
	{"loop",		INSN_REL8,		"E2 rel.0",				INSN_ALL_MODES},
	{"loope",		INSN_REL8,		"E1 rel.0",				INSN_ALL_MODES},
	{"loopne",		INSN_REL8,		"E0 rel.0",				INSN_ALL_MODES},
	{"lsl",			INSN_R_RM,		"0F 03 /r.0",				INSN_ALL_MODES},
	{"ltr",			INSN_RM_FIXED,		"0F 00 /3.0",				INSN_ALL_MODES},
	{"lzcnt",		INSN_R_RM,		"F3 0F BD /r.0",			INSN_ALL_MODES},
	{"*maskmovdqu",		INSN_XMM_RM,		"66 0F F7 /r.0",			INSN_ALL_MODES},
	{"maskmovq",		INSN_MM_RM,		"0F F7 /r.0",				INSN_ALL_MODES},
	{"*maxpd",		INSN_XMM_RM,		"66 0F 5F /r.0",			INSN_ALL_MODES},
	{"*maxps",		INSN_XMM_RM,		"0F 5F /r.0",				INSN_ALL_MODES},
	{"*maxsd",		INSN_XMM_RM,		"F2 0F 5F /r.0",			INSN_ALL_MODES},
	{"*maxss",		INSN_XMM_RM,		"F3 0F 5F /r.0",			INSN_ALL_MODES},
	{"*minpd",		INSN_XMM_RM,		"66 0F 5D /r.0",			INSN_ALL_MODES},
	{"*minps",		INSN_XMM_RM,		"0F 5D /r.0",				INSN_ALL_MODES},
	{"*minsd",		INSN_XMM_RM,		"F2 0F 5D /r.0",			INSN_ALL_MODES},
	{"*minss",		INSN_XMM_RM,		"F3 0F 5D /r.0",			INSN_ALL_MODES},
	{"monitor",		INSN_NONE,		"0F 01 C8",				INSN_ALL_MODES},
	{"mov",			INSN_AL_OFFSET,		"A0 moffs",				INSN_ALL_MODES},
	{"mov",			INSN_A_OFFSET,		"A1 moffs",				INSN_ALL_MODES},
	{"mov",			INSN_OFFSET_AL,		"A2 moffs",				INSN_ALL_MODES},
	{"mov",			INSN_OFFSET_A,		"A3 moffs",				INSN_ALL_MODES},
	{"mov",			INSN_R8_RM8,		"8A /r.0",				INSN_ALL_MODES},
	{"mov",			INSN_R_RM,		"8B /r.0",				INSN_ALL_MODES},
	{"mov",			INSN_RM8_R8,		"88 /r.0",				INSN_ALL_MODES},
	{"mov",			INSN_RM_R,		"89 /r.0",				INSN_ALL_MODES},
	{"mov",			INSN_RM16_SREG,		"8C /r.0",				INSN_ALL_MODES},
	{"mov",			INSN_RM64_SREG,		"48 8C /r.0",				INSN_64},
	{"mov",			INSN_SREG_RM16,		"8E /r.0",				INSN_ALL_MODES},
	{"mov",			INSN_SREG_RM64,		"48 8E /r.0",				INSN_64},
	{"mov",			INSN_R8_I8,		"B0+r ib",				INSN_ALL_MODES},
	{"mov",			INSN_R16_I16,		"B8+r iw",				INSN_ALL_MODES},
	{"mov",			INSN_R64_I64,		"B8+r iq",				INSN_ALL_MODES},
	{"movabs",		INSN_R64_I64,		"B8+r iq",				INSN_ALL_MODES},
	{"mov",			INSN_R32_I32,		"B8+r id",				INSN_ALL_MODES},
	{"mov",			INSN_RM8_I8,		"C6 /0.1 ib",				INSN_ALL_MODES},
	{"mov",			INSN_RM16_I16,		"C7 /0.2 iw",				INSN_ALL_MODES},
	{"mov",			INSN_RM32_I32,		"C7 /0.4 id",				INSN_ALL_MODES},
	{"mov",			INSN_R_CR,		"0F 20 /r.0",				INSN_ALL_MODES},
	{"mov",			INSN_CR_R,		"0F 22 /r.0",				INSN_ALL_MODES},
	{"mov",			INSN_R_DR,		"0F 21 /r.0",				INSN_ALL_MODES},
	{"mov",			INSN_DR_R,		"0F 23 /r.0",				INSN_ALL_MODES},
	{"pop",			INSN_R,			"58+r",					INSN_ALL_MODES | INSN_DEF64},
	{"push",		INSN_R,			"50+r",					INSN_ALL_MODES | INSN_DEF64},
	{"syscall",		INSN_NONE,		"0F 05",				INSN_64},
	
	// LIST TERMINATOR
	{NULL, 0, NULL, 0}
};
