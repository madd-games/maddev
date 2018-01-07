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

#ifndef ELF64_H
#define ELF64_H

#include "libobj.h"

/**
 * Handle endianness and stuff.
 */
#if defined(ELF_ARCH_LE)
#define	ELF_TARGET_DATA		ELFDATA2LSB
#define	ELF_MAKE16		OBJ_MAKE_LE16
#define	ELF_MAKE32		OBJ_MAKE_LE32
#define	ELF_MAKE64		OBJ_MAKE_LE64
#define	ELF_READ16		OBJ_READ_LE16
#define	ELF_READ32		OBJ_READ_LE32
#define	ELF_READ64		OBJ_READ_LE64
typedef	ObjLE64			Elf64_Addr;
typedef	ObjLE64			Elf64_Off;
typedef	ObjLE16			Elf64_Half;
typedef	ObjLE32			Elf64_Word;
typedef	ObjLE32			Elf64_Sword;
typedef	ObjLE64			Elf64_Xword;
typedef	ObjLE64			Elf64_Sxword;
#elif defined(ELF_ARCH_BE)
#define	ELF_TARGET_DATA		ELFDATA2MSB
#define	ELF_MAKE16		OBJ_MAKE_BE16
#define	ELF_MAKE32		OBJ_MAKE_BE32
#define	ELF_MAKE64		OBJ_MAKE_BE64
#define	ELF_READ16		OBJ_READ_BE16
#define	ELF_READ32		OBJ_READ_BE32
#define	ELF_READ64		OBJ_READ_BE64
typedef	ObjBE64			Elf64_Addr;
typedef	ObjBE64			Elf64_Off;
typedef	ObjBE16			Elf64_Half;
typedef	ObjBE32			Elf64_Word;
typedef	ObjBE32			Elf64_Sword;
typedef	ObjBE64			Elf64_Xword;
typedef	ObjBE64			Elf64_Sxword;
#else
#error Target machine endianness unknown!
#endif

#define	EI_MAG0			0
#define	EI_MAG1			1
#define	EI_MAG2			2
#define	EI_MAG3			3
#define	EI_CLASS		4
#define	EI_DATA			5
#define	EI_VERSION		6
#define	EI_OSABI		7
#define	EI_ABIVERSION		8
#define	EI_NIDENT		16

#define	ELFCLASS64		2

#define	ELFDATA2LSB		1
#define	ELFDATA2MSB		2

#define	ELFOSABI_SYSV		0
#define	ELFOSABI_HPUX		1
#define	ELFOSABI_STANDALONE	255

#define	EV_CURRENT		1

#define	ET_NONE			0
#define	ET_REL			1
#define	ET_EXEC			2
#define	ET_DYN			3
#define	ET_CORE			4

#define	SHN_UNDEF		0
#define	SHN_ABS			0xFFF1
#define	SHN_COMMON		0xFFF2

#define	SHT_NULL		0
#define	SHT_PROGBITS		1
#define	SHT_SYMTAB		2
#define	SHT_STRTAB		3
#define	SHT_RELA		4
#define	SHT_HASH		5
#define	SHT_DYNAMIC		6
#define	SHT_NOTE		7
#define	SHT_NOBITS		8
#define	SHT_REL			9
#define	SHT_SHLIB		10
#define	SHT_DYNSYM		11

#define	SHF_WRITE		0x1
#define	SHF_ALLOC		0x2
#define	SHF_EXECINSTR		0x4

#define	STN_UNDEF		0

#define	STB_LOCAL		0
#define	STB_GLOBAL		1
#define	STB_WEAK		2

#define	STT_NOTYPE		0
#define	STT_OBJECT		1
#define	STT_FUNC		2
#define	STT_SECTION		3
#define	STT_FILE		4

#define	ELF_MAKE_STINFO(b, t)	(((b) << 4) | (t))

#define ELF64_R_SYM(i)		((i) >> 32)
#define ELF64_R_TYPE(i)		((i) & 0xffffffffL)
#define ELF64_R_INFO(s, t)	(((s) << 32) + ((t) & 0xffffffffL))

#define	PT_NULL			0
#define	PT_LOAD			1
#define	PT_DYNAMIC		2
#define	PT_INTERP		3
#define	PT_NOTE			4
#define	PT_SHLIB		5
#define	PT_PHDR			6

#define	PF_X			0x1
#define	PF_W			0x2
#define	PF_R			0x4

typedef struct
{
	unsigned char		e_ident[EI_NIDENT];
	Elf64_Half		e_type;
	Elf64_Half		e_machine;
	Elf64_Word		e_version;
	Elf64_Addr		e_entry;
	Elf64_Off		e_phoff;
	Elf64_Off		e_shoff;
	Elf64_Word		e_flags;
	Elf64_Half		e_ehsize;
	Elf64_Half		e_phentsize;
	Elf64_Half		e_phnum;
	Elf64_Half		e_shentsize;
	Elf64_Half		e_shnum;
	Elf64_Half		e_shstrndx;
} Elf64_Ehdr;

typedef struct
{
	Elf64_Word		sh_name;
	Elf64_Word		sh_type;
	Elf64_Xword		sh_flags;
	Elf64_Addr		sh_addr;
	Elf64_Off		sh_offset;
	Elf64_Xword		sh_size;
	Elf64_Word		sh_link;
	Elf64_Word		sh_info;
	Elf64_Xword		sh_addralign;
	Elf64_Xword		sh_entsize;
} Elf64_Shdr;

typedef struct
{
	Elf64_Word		st_name;
	unsigned char		st_info;
	unsigned char		st_other;
	Elf64_Half		st_shndx;
	Elf64_Addr		st_value;
	Elf64_Xword		st_size;
} Elf64_Sym;

typedef struct
{
	Elf64_Addr		r_offset;
	Elf64_Xword		r_info;
} Elf64_Rel;

typedef struct
{
	Elf64_Addr		r_offset;
	Elf64_Xword		r_info;
	Elf64_Sxword		r_addend;
} Elf64_Rela;

typedef struct
{
	Elf64_Word		p_type;
	Elf64_Word		p_flags;
	Elf64_Off		p_offset;
	Elf64_Addr		p_vaddr;
	Elf64_Addr		p_paddr;
	Elf64_Xword		p_filesz;
	Elf64_Xword		p_memsz;
	Elf64_Xword		p_align;
} Elf64_Phdr;

typedef struct
{
	Elf64_Xword d_tag;
	union
	{
		Elf64_Xword	d_val;
		Elf64_Addr	d_ptr;
	} d_un;
} Elf64_Dyn;

uint64_t elfGetReloc(int size, int type);
int elfReadReloc(uint64_t elfrel, int *size, int *type);

#endif
