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
	{"adc",			INSN_RM8_I8,		"80 /2 ib",				INSN_ALL_MODES},
	{"adc",			INSN_RM16_I16,		"81 /2 iw",				INSN_ALL_MODES},
	{"adc",			INSN_RM32_I32,		"81 /2 id",				INSN_ALL_MODES},
	{"adc",			INSN_RM8_R8,		"10 /r",				INSN_ALL_MODES},
	{"adc",			INSN_RM_R,		"11 /r",				INSN_ALL_MODES},
	{"adc",			INSN_R8_RM8,		"12 /r",				INSN_ALL_MODES},
	{"adc",			INSN_R_RM,		"13 /r",				INSN_ALL_MODES},
	{"adcx",		INSN_R_RM_NO16,		"66 0F 38 F6 /r",			INSN_ALL_MODES},
	{"add",			INSN_AL_I,		"04 ib",				INSN_ALL_MODES},
	{"add",			INSN_AX_I,		"05 iw",				INSN_ALL_MODES},
	{"add",			INSN_EAX_I,		"05 id",				INSN_ALL_MODES},
	{"add",			INSN_RM8_I8,		"80 /0 ib",				INSN_ALL_MODES},
	{"add",			INSN_RM16_I16,		"81 /0 iw",				INSN_ALL_MODES},
	{"add",			INSN_RM32_I32,		"81 /0 id",				INSN_ALL_MODES},
	{"add",			INSN_RM8_R8,		"00 /r",				INSN_ALL_MODES},
	{"add",			INSN_RM_R,		"01 /r",				INSN_ALL_MODES},
	{"add",			INSN_R8_RM8,		"02 /r",				INSN_ALL_MODES},
	{"add",			INSN_R_RM,		"03 /r",				INSN_ALL_MODES},
	{"*addpd",		INSN_XMM_RM,		"66 0F 58 /r",				INSN_ALL_MODES},
	{"*addps",		INSN_XMM_RM,		"0F 58 /r",				INSN_ALL_MODES},
	{"*addsd",		INSN_XMM_RM,		"F2 0F 58 /r",				INSN_ALL_MODES},
	{"*addss",		INSN_XMM_RM,		"F3 0F 58 /r",				INSN_ALL_MODES},
	{"*addsubpd",		INSN_XMM_RM,		"66 0F D0 /r",				INSN_ALL_MODES},
	{"*addsubps",		INSN_XMM_RM,		"F2 0F D0 /r",				INSN_ALL_MODES},
	{"adox",		INSN_R_RM_NO16,		"F3 0F 38 F6 /r",			INSN_ALL_MODES},
	{"*aesdec",		INSN_XMM_RM,		"66 0F 38 DE /r",			INSN_ALL_MODES},
	{"*aesdeclast",		INSN_XMM_RM,		"66 0F 38 DF /r",			INSN_ALL_MODES},
	{"*aesenc",		INSN_XMM_RM,		"66 0F 38 DC /r",			INSN_ALL_MODES},
	{"*aesenclast",		INSN_XMM_RM,		"66 0F 38 DD /r",			INSN_ALL_MODES},
	{"*aesimc",		INSN_XMM_RM,		"66 0F 38 DB /r",			INSN_ALL_MODES},
	{"*aeskeygenassist",	INSN_XMM_RM_IMM8,	"66 0F 3A DF /r ib",			INSN_ALL_MODES},
	{"and",			INSN_AL_I,		"24 ib",				INSN_ALL_MODES},
	{"and",			INSN_AX_I,		"25 iw",				INSN_ALL_MODES},
	{"and",			INSN_EAX_I,		"25 id",				INSN_ALL_MODES},
	{"and",			INSN_RM8_I8,		"80 /4 ib",				INSN_ALL_MODES},
	{"and",			INSN_RM16_I16,		"81 /4 iw",				INSN_ALL_MODES},
	{"and",			INSN_RM32_I32,		"81 /4 id",				INSN_ALL_MODES},
	{"and",			INSN_RM8_R8,		"20 /r",				INSN_ALL_MODES},
	{"and",			INSN_RM_R,		"21 /r",				INSN_ALL_MODES},
	{"and",			INSN_R8_RM8,		"22 /r",				INSN_ALL_MODES},
	{"and",			INSN_R_RM,		"23 /r",				INSN_ALL_MODES},
	{"andn",		INSN_R_RM,		"VEX.NDS.LZ.0F38.W0 F2 /r",		INSN_ALL_MODES}, // TODO?
	{"*andpd",		INSN_XMM_RM,		"66 0F 54 /r",				INSN_ALL_MODES},
	{"*andps",		INSN_XMM_RM,		"0F 54 /r",				INSN_ALL_MODES},
	{"*andnpd",		INSN_XMM_RM,		"66 0F 55 /r",				INSN_ALL_MODES},
	{"*andnps",		INSN_XMM_RM,		"0F 55 /r",				INSN_ALL_MODES},
	{"arpl",		INSN_RM_R,		"63 /r",				INSN_LEG_ONLY},
	{"*blendpd",		INSN_XMM_RM_IMM8,	"66 0F 3A 0D /r ib",			INSN_ALL_MODES},
	{"bextr",		INSN_R_RM,		"VEX.NDS.LZ.0F38.W0 F7 /r",		INSN_ALL_MODES},
	{"*blendps",		INSN_XMM_RM_IMM8,	"66 0F 3A 0C /r ib",			INSN_ALL_MODES},
	{"blendvpd",		INSN_XMM_RM,		"66 0F 38 15 /r",			INSN_ALL_MODES},
	{"vblendvpd",		INSN_XMM4,		"VEX.NDS.128.66.0F3A.W0 4B /r /is4",	INSN_ALL_MODES},
	{"blendvps",		INSN_XMM_RM,		"66 0F 38 14 /r",			INSN_ALL_MODES},
	{"vblendvps",		INSN_XMM4,		"VEX.NDS.128.66.0F3A.W0 4A /r /is4",	INSN_ALL_MODES},
	{"blsi",		INSN_R_RM,		"VEX.NDD.LZ.0F38.W0 F3 /3",		INSN_ALL_MODES},
	{"blsmk",		INSN_R_RM,		"VEX.NDD.LZ.0F38.W0 F3 /2",		INSN_ALL_MODES},
	{"blsr",		INSN_R_RM,		"VEX.NDD.LZ.0F38.W0 F3 /1",		INSN_ALL_MODES},
	{"bndcl",		INSN_BND_RM,		"F3 0F 1A /r",				INSN_ALL_MODES},
	{"bndcu",		INSN_BND_RM,		"F2 0F 1A /r",				INSN_ALL_MODES},
	{"bndcn",		INSN_BND_RM,		"F2 0F 1B /r",				INSN_ALL_MODES},
	{"bndldx",		INSN_BND_MIB,		"0F 1A /r",				INSN_ALL_MODES},
	{"bndmk",		INSN_BND_RM,		"F3 0F 1B /r",				INSN_ALL_MODES},
	{"bndmov",		INSN_BND_BNDM,		"66 0F 1A /r",				INSN_ALL_MODES},
	{"bndmov",		INSN_BNDM_BND,		"66 0F 1B /r",				INSN_ALL_MODES},
	{"bndstx",		INSN_MIB_BND,		"0F 1B /r",				INSN_ALL_MODES},
	{"bound",		INSN_R_RM,		"62 /r",				INSN_LEG_ONLY},
	{"bsf",			INSN_R_RM,		"0F BC /r",				INSN_ALL_MODES},
	{"bsr",			INSN_R_RM,		"0F BD /r",				INSN_ALL_MODES},
	{"bswap",		INSN_R,			"0F C8+r",				INSN_ALL_MODES},
	{"bt",			INSN_RM_R,		"0F A3 /r",				INSN_ALL_MODES},
	{"bt",			INSN_RM_I8,		"0F BA /4 ib",				INSN_ALL_MODES},
	{"btc",			INSN_RM_R,		"0F BB /r",				INSN_ALL_MODES},
	{"btc",			INSN_RM_I8,		"0F BA /7 ib",				INSN_ALL_MODES},
	{"btr",			INSN_RM_R,		"0F B3 /r",				INSN_ALL_MODES},
	{"btr",			INSN_RM_I8,		"0F BA /6 ib",				INSN_ALL_MODES},
	{"bts",			INSN_RM_R,		"0F AB /r",				INSN_ALL_MODES},
	{"bts",			INSN_RM_I8,		"0F BA /5 ib",				INSN_ALL_MODES},
	// TODO: BZHI
	{"call",		INSN_IMM,		"E8 rel",				INSN_ALL_MODES},
	{"call",		INSN_RM,		"FF /2",				INSN_ALL_MODES},
	{"call",		INSN_IMM_FAR,		"9A relf",				INSN_LEG_ONLY},
	{"callf",		INSN_RM,		"FF /3",				INSN_ALL_MODES}, // far call
	{"cbw",			INSN_NONE,		"98",					INSN_ALL_MODES},
	{"cwde",		INSN_NONE,		"98",					INSN_ALL_MODES},
	{"cdqe",		INSN_NONE,		"48 98",				INSN_64},
	{"clac",		INSN_NONE,		"0F 01 CA",				INSN_ALL_MODES},
	{"clc",			INSN_NONE,		"F8",					INSN_ALL_MODES},
	{"cld",			INSN_NONE,		"FC",					INSN_ALL_MODES},
	{"clflush",		INSN_RM,		"0F AE /7",				INSN_ALL_MODES},
	{"clflushopt",		INSN_RM,		"66 0F AE /7",				INSN_ALL_MODES},
	{"cli",			INSN_NONE,		"FA",					INSN_ALL_MODES},
	{"clts",		INSN_NONE,		"0F 06",				INSN_ALL_MODES},
	{"clwb",		INSN_RM,		"66 0F AE /6",				INSN_ALL_MODES},
	{"cmc",			INSN_NONE,		"F5",					INSN_ALL_MODES},
	{"cmova",		INSN_R_RM,		"0F 47 /r",				INSN_ALL_MODES},
	{"cmovae",		INSN_R_RM,		"0F 43 /r",				INSN_ALL_MODES},
	{"cmovb",		INSN_R_RM,		"0F 42 /r",				INSN_ALL_MODES},
	{"cmovbe",		INSN_R_RM,		"0F 46 /r",				INSN_ALL_MODES},
	{"cmovc",		INSN_R_RM,		"0F 42 /r",				INSN_ALL_MODES},
	{"cmove",		INSN_R_RM,		"0F 44 /r",				INSN_ALL_MODES},
	{"cmovg",		INSN_R_RM,		"0F 4F /r",				INSN_ALL_MODES},
	{"cmovge",		INSN_R_RM,		"0F 4D /r",				INSN_ALL_MODES},
	{"cmovl",		INSN_R_RM,		"0F 4C /r",				INSN_ALL_MODES},
	{"cmovle",		INSN_R_RM,		"0F 4E /r",				INSN_ALL_MODES},
	{"cmovna",		INSN_R_RM,		"0F 46 /r",				INSN_ALL_MODES},
	{"cmovnb",		INSN_R_RM,		"0F 43 /r",				INSN_ALL_MODES},
	{"cmovnbe",		INSN_R_RM,		"0F 47 /r",				INSN_ALL_MODES},
	{"cmovnc",		INSN_R_RM,		"0F 43 /r",				INSN_ALL_MODES},
	{"cmovnae",		INSN_R_RM,		"0F 45 /r",				INSN_ALL_MODES},
	{"cmovng",		INSN_R_RM,		"0F 4E /r",				INSN_ALL_MODES},
	{"cmovnge",		INSN_R_RM,		"0F 4C /r",				INSN_ALL_MODES},
	{"cmovnl",		INSN_R_RM,		"0F 4D /r",				INSN_ALL_MODES},
	{"cmovno",		INSN_R_RM,		"0F 41 /r",				INSN_ALL_MODES},
	{"cmovnp",		INSN_R_RM,		"0F 4B /r",				INSN_ALL_MODES},
	{"cmovns",		INSN_R_RM,		"0F 49 /r",				INSN_ALL_MODES},
	{"cmovnz",		INSN_R_RM,		"0F 45 /r",				INSN_ALL_MODES},
	{"cmovo",		INSN_R_RM,		"0F 40 /r",				INSN_ALL_MODES},
	{"cmovp",		INSN_R_RM,		"0F 4A /r",				INSN_ALL_MODES},
	{"cmovpe",		INSN_R_RM,		"0F 4A /r",				INSN_ALL_MODES},
	{"cmovpo",		INSN_R_RM,		"0F 4B /r",				INSN_ALL_MODES},
	{"cmovs",		INSN_R_RM,		"0F 48 /r",				INSN_ALL_MODES},
	{"cmp",			INSN_AL_I,		"3C ib",				INSN_ALL_MODES},
	{"cmp",			INSN_AX_I,		"3D iw",				INSN_ALL_MODES},
	{"cmp",			INSN_EAX_I,		"3D id",				INSN_ALL_MODES},
	{"cmp",			INSN_RM8_I8,		"80 /7 ib",				INSN_ALL_MODES},
	{"cmp",			INSN_RM_I,		"81 /7 iw",				INSN_ALL_MODES},
	{"cmp",			INSN_RM_I,		"81 /7 id",				INSN_ALL_MODES},
	{"cmp",			INSN_RM_I8,		"83 /7 ib",				INSN_ALL_MODES},
	{"cmp",			INSN_RM8_R8,		"38 /r",				INSN_ALL_MODES},
	{"cmp",			INSN_RM_R,		"39 /r",				INSN_ALL_MODES},
	{"cmp",			INSN_R8_RM8,		"3A /r",				INSN_ALL_MODES},
	{"cmp",			INSN_R_RM,		"3B /r",				INSN_ALL_MODES},
	{"cmp",			INSN_RM8_R8,		"38 /r",				INSN_ALL_MODES},
	{"*cmppd",		INSN_XMM_RM_IMM8,	"66 0F C2 /r ib",			INSN_ALL_MODES},
	{"*cmpps",		INSN_XMM_RM_IMM8,	"0F C2 /r ib",				INSN_ALL_MODES},
	{"cmps",		INSN_NONE,		"A6",					INSN_ALL_MODES},
	{"cmps",		INSN_NONE,		"A7",					INSN_ALL_MODES},
	{"cmpsb",		INSN_NONE,		"A6",					INSN_ALL_MODES},
	{"cmpsw",		INSN_NONE,		"A7",					INSN_ALL_MODES},
	{"cmpsd",		INSN_NONE,		"A7",					INSN_ALL_MODES},
	{"cmpsq",		INSN_NONE,		"48 A7",				INSN_64},
	{"*cmpsd",		INSN_XMM_RM_IMM8,	"F2 0F C2 /r ib",			INSN_ALL_MODES},
	{"*cmpss",		INSN_XMM_RM_IMM8,	"F3 0F C2 /r ib",			INSN_ALL_MODES},
	{"cmpxchg",		INSN_RM8_R8,		"0F B0 /r",				INSN_ALL_MODES},
	{"cmpxchg",		INSN_RM_R,		"0F B1 /r",				INSN_ALL_MODES},
	{"cmpxchg8b",		INSN_RM,		"0F C7 /1",				INSN_ALL_MODES},
	{"cmpxchg16b",		INSN_RM,		"48 0F C7 /1",				INSN_64},
	{"*comisd",		INSN_XMM_RM,		"66 0F 2F /r",				INSN_32 | INSN_64},
	{"*comiss",		INSN_XMM_RM,		"66 0F 2F /r",				INSN_ALL_MODES},
	{"cpuid",		INSN_NONE,		"0F A2",				INSN_ALL_MODES},
	// TODO: CRC32
	{"*cvtdq2pd",		INSN_XMM_RM,		"F3 0F E6 /r",				INSN_32 | INSN_64},
	{"*cvtdq2ps",		INSN_XMM_RM,		"0F 5B /r",				INSN_32 | INSN_64},
	{"*cvtpd2dq",		INSN_XMM_RM,		"F2 0F E6 /r",				INSN_32 | INSN_64},
	{"cvtpd2pi",		INSN_MM_XMMRM,		"66 0F 2D /r",				INSN_ALL_MODES},
	{"*cvtpd2ps",		INSN_XMM_RM,		"66 0F 5A /r",				INSN_32 | INSN_64},
	{"cvtpi2pd",		INSN_XMM_RM,		"66 0F 2A /r",				INSN_ALL_MODES},
	{"cvtpi2ps",		INSN_XMM_RM,		"0F 2A /r",				INSN_ALL_MODES},
	{"*cvtps2dq",		INSN_XMM_RM,		"66 0F 5B /r",				INSN_32 | INSN_64},
	{"*cvtps2pd",		INSN_XMM_RM,		"0F 5A /r",				INSN_32 | INSN_64},
	{"cvtps2pi",		INSN_MM_XMMRM,		"0F 2D /r",				INSN_ALL_MODES},
	{"*cvtsd2si",		INSN_XMM_RM,		"F2 0F 2D /r",				INSN_32 | INSN_64},
	{"*cvtsd2ss",		INSN_XMM_RM,		"F2 0F 5A /r",				INSN_32 | INSN_64},
	{"*cvtsi2sd",		INSN_XMM_RM,		"F2 0F 2A /r",				INSN_32 | INSN_64},
	{"*cvtsi2ss",		INSN_XMM_RM,		"F3 0F 2A /r",				INSN_32 | INSN_64},
	{"*cvtss2sd",		INSN_XMM_RM,		"F3 0F 5A /r",				INSN_32 | INSN_64},
	{"*cvtss2si",		INSN_XMM_RM,		"F3 0F 2D /r",				INSN_32 | INSN_64},
	{"*cvttpd2dq",		INSN_XMM_RM,		"66 0F E6 /r",				INSN_32 | INSN_64},
	{"cvttpd2pi",		INSN_MM_XMMRM,		"66 0F 2C /r",				INSN_ALL_MODES},
	{"*cvttps2dq",		INSN_XMM_RM,		"F3 0F 5B /r",				INSN_32 | INSN_64},
	{"cvttps2pi",		INSN_MM_XMMRM,		"0F 2C /r",				INSN_ALL_MODES},
	{"*cvttsd2si",		INSN_XMM_RM,		"F2 0F 2C /r",				INSN_32 | INSN_64},
	{"*cvttss2si",		INSN_XMM_RM,		"F3 0F 2C /r",				INSN_32 | INSN_64},
	{"cwd",			INSN_NONE,		"99",					INSN_ALL_MODES},
	{"cdq",			INSN_NONE,		"99",					INSN_ALL_MODES},
	{"cqo",			INSN_NONE,		"48 99",				INSN_64},
	{"daa",			INSN_NONE,		"27",					INSN_LEG_ONLY},
	{"das",			INSN_NONE,		"2F",					INSN_LEG_ONLY},
	{"dec",			INSN_RM8,		"FE /1",				INSN_ALL_MODES},
	{"dec",			INSN_RM,		"FF /1",				INSN_ALL_MODES},
	{"dec",			INSN_R,			"48+r",					INSN_LEG_ONLY},
	{"div",			INSN_RM8,		"F6 /6",				INSN_ALL_MODES},
	{"div",			INSN_RM,		"F7 /6",				INSN_ALL_MODES},
	{"*divpd",		INSN_XMM_RM,		"66 0F 5E /r",				INSN_32 | INSN_64},
	{"*divps",		INSN_XMM_RM,		"0F 5E /r",				INSN_32 | INSN_64},
	{"*divsd",		INSN_XMM_RM,		"F2 0F 5E /r",				INSN_32 | INSN_64},
	{"*divss",		INSN_XMM_RM,		"F3 0F 5E /r",				INSN_32 | INSN_64},
	{"*dppd",		INSN_XMM_RM_IMM8,	"66 0F 3A 41 /r ib",			INSN_32 | INSN_64},
	{"*dpps",		INSN_XMM_RM_IMM8,	"66 0F 3A 40 /r ib",			INSN_32 | INSN_64},
	{"emms",		INSN_NONE,		"0F 77",				INSN_ALL_MODES},
	// TODO: ENTER
	{"*extractps",		INSN_RM_XMM_IMM8,	"66 0F 3A 17 /r ib",			INSN_32 | INSN_64},
	{"int",			INSN_IB,		"CD ib",				INSN_ALL_MODES},
	{"mov",			INSN_R8_I8,		"B0+r ib",				INSN_ALL_MODES},
	{"mov",			INSN_R16_I16,		"B8+r iw",				INSN_ALL_MODES},
	{"mov",			INSN_R32_I32,		"B8+r id",				INSN_ALL_MODES},
	{"mov",			INSN_R64_I64,		"B8+r iq",				INSN_ALL_MODES},
	{"movabs",		INSN_R64_I64,		"B8+r iq",				INSN_ALL_MODES},
	{"mov",			INSN_R8_RM8,		"8A /r",				INSN_ALL_MODES},
	{"mov",			INSN_R_RM,		"8B /r",				INSN_ALL_MODES},
	{"mov",			INSN_RM8_R8,		"88 /r",				INSN_ALL_MODES},
	{"mov",			INSN_RM_R,		"89 /r",				INSN_ALL_MODES},
	{"pop",			INSN_R,			"58+r",					INSN_ALL_MODES | INSN_DEF64},
	{"push",		INSN_R,			"50+r",					INSN_ALL_MODES | INSN_DEF64},
	{"syscall",		INSN_NONE,		"0F 05",				INSN_64},
	
	// LIST TERMINATOR
	{NULL, 0, NULL, 0}
};
