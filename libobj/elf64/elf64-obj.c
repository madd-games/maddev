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

#include "elf64.h"

#define	SHN_SHSTRTAB			1
#define	SHN_SYMTAB			2
#define	SHN_SYMSTRTAB			3

typedef struct
{
	char*				data;
	size_t				size;
} StringTable;

static int64_t strtabPut(StringTable *table, const char *str)
{
	table->data = (char*) realloc(table->data, table->size + strlen(str) + 1);
	strcpy(table->data + table->size, str);
	int64_t result = (int64_t) table->size;
	table->size += strlen(str) + 1;
	return result;
};

int objWrite(Object *obj, const char *filename)
{
	/**
	 * Count how many sections we need. We always need:
	 *  - the null section (index 0)
	 *  - section name string table
	 *  - symbol table
	 *  - string table for the symbol table
	 * Then for each libobj section, emit the section itself, and its relocation table.
	 *
	 * For program headers: it's PT_NULL plus a header for every section, plus an additional
	 * DYNAMIC header for the ".dynamic" section.
	 */
	size_t numSections = 4;
	size_t numProgHeads = 1;
	Section *sect;
	for (sect=obj->sections; sect!=NULL; sect=sect->next)
	{
		sect->aux.index = numSections;
		numSections += 2;	/* section itself, and the relocation table */
		numProgHeads++;
		
		if (strcmp(sect->name, ".dynamic") == 0)
		{
			// dynamic section needs an additional DYNAMIC header
			numProgHeads++;
		};
	};
	
	/**
	 * Allocate a place inside the ELF64 image for each section etc. The layout is deterministic,
	 * we don't actually seek! This is just to help fill in the offsets in structures. The layout
	 * is as follows:
	 *
	 * ELF64 Header
	 * Section table
	 * Program header table
	 * Section name string table
	 * Symbol table
	 * String table for the symbol table
	 * Array of section contents, each directly followed by its relocation table.
	 */
	int64_t place = sizeof(Elf64_Ehdr) + numSections * sizeof(Elf64_Shdr);
	int64_t offProgHeads = place;
	place += numProgHeads * sizeof(Elf64_Phdr);
	
	/**
	 * Start with section names.
	 */
	StringTable sectNames = {NULL, 0};
	strtabPut(&sectNames, "");
	int64_t offNameShstrtab = strtabPut(&sectNames, ".shstrtab");
	int64_t offNameStrtab = strtabPut(&sectNames, ".strtab");
	int64_t offNameSymtab = strtabPut(&sectNames, ".symtab");
	for (sect=obj->sections; sect!=NULL; sect=sect->next)
	{
		sect->aux.nameoff = strtabPut(&sectNames, sect->name);
		char *buffer = (char*) malloc(strlen(sect->name) + 8);
#ifdef ELF_RELOC_REL
		sprintf(buffer, ".rel%s", sect->name);
#else
		sprintf(buffer, ".rela%s", sect->name);
#endif
		sect->aux.relnameoff = strtabPut(&sectNames, buffer);
		free(buffer);
	};
	
	int64_t offShstrtab = place;
	place += sectNames.size;
	
	/**
	 * Symbol table time.
	 */
	StringTable symstrtab = {NULL, 0};
	strtabPut(&symstrtab, "");
	Elf64_Sym *symtab = (Elf64_Sym*) malloc(sizeof(Elf64_Sym));
	size_t numSymbols = 1;
	size_t numLocalSymbols = 0;		/* maybe later */
	memset(symtab, 0, sizeof(Elf64_Sym));
	
	// local and global symbols
	Symbol *sym;
	for (sym=obj->symbols; sym!=NULL; sym=sym->next)
	{
		size_t index = numSymbols++;
		
		symtab = (Elf64_Sym*) realloc(symtab, numSymbols * sizeof(Elf64_Sym));
		symtab[index].st_name = ELF_MAKE32(strtabPut(&symstrtab, sym->name));
		
		unsigned char b, t;
		switch (sym->binding)
		{
		case SYMB_LOCAL:
			b = STB_LOCAL;
			break;
		case SYMB_GLOBAL:
			b = STB_GLOBAL;
			break;
		case SYMB_WEAK:
			b = STB_WEAK;
			break;
		};
		
		switch (sym->type)
		{
		case SYMT_NONE:
			t = STT_NOTYPE;
			break;
		case SYMT_OBJECT:
			t = STT_OBJECT;
			break;
		case SYMT_FUNC:
			t = STT_FUNC;
			break;
		};
		
		symtab[index].st_info = ELF_MAKE_STINFO(b, t);
		symtab[index].st_other = 0;
		if (sym->sect != NULL)
		{
			symtab[index].st_shndx = ELF_MAKE16(sym->sect->aux.index);
		}
		else
		{
			symtab[index].st_shndx = ELF_MAKE16(SHN_ABS);
		};
		
		if (sym->offset == SYMOFF_COMMON)
		{
			symtab[index].st_shndx = ELF_MAKE16(SHN_COMMON);
			symtab[index].st_value = ELF_MAKE64(sym->align);
		}
		else
		{
			symtab[index].st_value = ELF_MAKE64(sym->offset);
		};
		
		symtab[index].st_size = ELF_MAKE64(sym->size);
	};
	
	// undefined symbols
	for (sect=obj->sections; sect!=NULL; sect=sect->next)
	{
		sect->aux.numRelocs = 0;
		Reloc *reloc;
		for (reloc=sect->relocs; reloc!=NULL; reloc=reloc->next)
		{
			sect->aux.numRelocs++;

#ifdef ELF_RELOC_REL
			switch (reloc->size)
			{
			case REL_BYTE:
				*((int8_t*)((char*)sect->data + reloc->offset)) = (int8_t) reloc->addend;
				break;
			case REL_WORD:
				*((Elf64_Half*)((char*)sect->data + reloc->offset)) = ELF_MAKE16((uint16_t) (int16_t) reloc->addend);
				break;
			case REL_DWORD:
				*((Elf64_Word*)((char*)sect->data + reloc->offset)) = ELF_MAKE32((uint32_t) (int32_t) reloc->addend);
			case REL_QWORD:
				*((Elf64_Xword*)((char*)sect->data + reloc->offset)) = ELF_MAKE64((uint32_t) (int32_t) reloc->addend);
				break;
			};
#endif

			// make sure a symbol exists for this entry.
			size_t i;
			int found = 0;
			for (i=0; i<numSymbols; i++)
			{
				uint32_t nameOffset = ELF_READ32(symtab[i].st_name);
				if (strcmp(&symstrtab.data[nameOffset], reloc->symbol) == 0)
				{
					found = 1;
					break;
				};
			};
			
			if (!found)
			{
				size_t index = numSymbols++;
		
				symtab = (Elf64_Sym*) realloc(symtab, numSymbols * sizeof(Elf64_Sym));
				symtab[index].st_name = ELF_MAKE32(strtabPut(&symstrtab, reloc->symbol));
				symtab[index].st_info = 0;
				symtab[index].st_other = 0;
				symtab[index].st_shndx = ELF_MAKE16(0);
				symtab[index].st_value = ELF_MAKE64(0);
				symtab[index].st_size = ELF_MAKE64(0);
			};
		};
	};
	
	int64_t offSymtab = place;
	place += numSymbols * sizeof(Elf64_Sym);
	int64_t offStrtab = place;
	place += symstrtab.size;
	
	for (sect=obj->sections; sect!=NULL; sect=sect->next)
	{
		if (sect->type == SECTYPE_PROGBITS)
		{
			uint64_t currentPlace = place;
			place = (place + 0xFFF) & (~0xFFF);
			
			sect->aux.padSize = place - currentPlace;
			sect->aux.dataOff = place;
			place += sect->size;
			sect->aux.relOff = place;
#ifdef ELF_RELOC_REL
			place += sect->aux.numRelocs * sizeof(Elf64_Rel);
#else
			place += sect->aux.numRelocs * sizeof(Elf64_Rela);
#endif
		};
	};
	
	/**
	 * Write everything to the file (follow the layout defined above!)
	 */
	FILE *fp = fopen(filename, "w");
	if (fp == NULL)
	{
		int errnum = errno;
		free(sectNames.data);
		return errnum;
	};
	
	Elf64_Ehdr elfHeader;
	memset(&elfHeader, 0, sizeof(Elf64_Ehdr));
	elfHeader.e_ident[EI_MAG0] = 0x7F;
	elfHeader.e_ident[EI_MAG1] = 'E';
	elfHeader.e_ident[EI_MAG2] = 'L';
	elfHeader.e_ident[EI_MAG3] = 'F';
	elfHeader.e_ident[EI_CLASS] = ELFCLASS64;
	elfHeader.e_ident[EI_DATA] = ELF_TARGET_DATA;
	elfHeader.e_ident[EI_VERSION] = EV_CURRENT;
	elfHeader.e_ident[EI_OSABI] = ELFOSABI_SYSV;
	elfHeader.e_ident[EI_ABIVERSION] = 0;
	switch (obj->type)
	{
	case OBJTYPE_RELOC:
		elfHeader.e_type = ELF_MAKE16(ET_REL);
		break;
	case OBJTYPE_EXEC:
		elfHeader.e_type = ELF_MAKE16(ET_EXEC);
		break;
	case OBJTYPE_SHARED:
		elfHeader.e_type = ELF_MAKE16(ET_DYN);
		break;
	default:
		elfHeader.e_type = ELF_MAKE16(ET_NONE);
		break;
	};
	elfHeader.e_machine = ELF_MAKE16(ELF_MACHINE_NUMBER);
	elfHeader.e_version = ELF_MAKE32(EV_CURRENT);
	if (obj->entry == NULL)
	{
		elfHeader.e_entry = ELF_MAKE64(0);
	}
	else
	{
		// find the entry symbol
		Symbol *sym;
		for (sym=obj->symbols; sym!=NULL; sym=sym->next)
		{
			if (sym->binding == SYMB_GLOBAL)
			{
				if (strcmp(sym->name, obj->entry) == 0)
				{
					break;
				};
			};
		};
		
		if (sym == NULL)
		{
			fprintf(stderr, "libobj: cannot find entry symbol `%s': setting to undefined\n", obj->entry);
			elfHeader.e_entry = ELF_MAKE64(0);
		}
		else
		{
			elfHeader.e_entry = ELF_MAKE64(sym->offset);
		};
	};
	elfHeader.e_phoff = ELF_MAKE64(offProgHeads);
	elfHeader.e_shoff = ELF_MAKE64(sizeof(Elf64_Ehdr));
	elfHeader.e_flags = ELF_MAKE32(0);
	elfHeader.e_ehsize = ELF_MAKE16(sizeof(Elf64_Ehdr));
	elfHeader.e_phentsize = ELF_MAKE16(sizeof(Elf64_Phdr));
	elfHeader.e_phnum = ELF_MAKE16(numProgHeads);
	elfHeader.e_shentsize = ELF_MAKE16(sizeof(Elf64_Shdr));
	elfHeader.e_shnum = ELF_MAKE16(numSections);
	elfHeader.e_shstrndx = ELF_MAKE16(SHN_SHSTRTAB);
	fwrite(&elfHeader, 1, sizeof(Elf64_Ehdr), fp);
	
	/* section 0: null section */
	Elf64_Shdr elfSect;
	memset(&elfSect, 0, sizeof(Elf64_Shdr));
	fwrite(&elfSect, 1, sizeof(Elf64_Shdr), fp);
	
	/* section 1: section name string table */
	elfSect.sh_name = ELF_MAKE32(offNameShstrtab);
	elfSect.sh_type = ELF_MAKE32(SHT_STRTAB);
	elfSect.sh_flags = ELF_MAKE64(0);
	elfSect.sh_addr = ELF_MAKE64(0);
	elfSect.sh_offset = ELF_MAKE64(offShstrtab);
	elfSect.sh_size = ELF_MAKE64(sectNames.size);
	elfSect.sh_link = ELF_MAKE32(0);
	elfSect.sh_info = ELF_MAKE32(0);
	elfSect.sh_addralign = ELF_MAKE64(0);
	elfSect.sh_entsize = ELF_MAKE64(0);
	fwrite(&elfSect, 1, sizeof(Elf64_Shdr), fp);
	
	/* section 2: symbol table */
	memset(&elfSect, 0, sizeof(Elf64_Shdr));
	elfSect.sh_name = ELF_MAKE32(offNameSymtab);
	elfSect.sh_type = ELF_MAKE32(SHT_SYMTAB);
	elfSect.sh_link = ELF_MAKE32(SHN_SYMSTRTAB);
	elfSect.sh_info = ELF_MAKE32(numLocalSymbols);
	elfSect.sh_entsize = ELF_MAKE64(sizeof(Elf64_Sym));
	elfSect.sh_size = ELF_MAKE64(numSymbols * sizeof(Elf64_Sym));
	elfSect.sh_offset = ELF_MAKE64(offSymtab);
	fwrite(&elfSect, 1, sizeof(Elf64_Shdr), fp);
	
	/* section 3: symbol table string table */
	memset(&elfSect, 0, sizeof(Elf64_Shdr));
	elfSect.sh_name = ELF_MAKE32(offNameStrtab);
	elfSect.sh_type = ELF_MAKE32(SHT_STRTAB);
	elfSect.sh_offset = ELF_MAKE64(offStrtab);
	elfSect.sh_size = ELF_MAKE64(symstrtab.size);
	fwrite(&elfSect, 1, sizeof(Elf64_Shdr), fp);
	
	/* other sections */
	char zeroPage[0x1000];
	memset(zeroPage, 0, 0x1000);
	size_t index = 4;
	for (sect=obj->sections; sect!=NULL; sect=sect->next)
	{
		// first the actual secton
		memset(&elfSect, 0, sizeof(Elf64_Shdr));
		elfSect.sh_name = ELF_MAKE32(sect->aux.nameoff);
		if (sect->type == SECTYPE_PROGBITS)
		{
			elfSect.sh_type = ELF_MAKE32(SHT_PROGBITS);
			elfSect.sh_offset = ELF_MAKE64(sect->aux.dataOff);
		}
		else
		{
			elfSect.sh_type = ELF_MAKE32(SHT_NOBITS);
		};
		
		uint64_t flags = SHF_ALLOC;
		if (sect->flags & SEC_WRITE)
		{
			flags |= SHF_WRITE;
		};
		if (sect->flags & SEC_EXEC)
		{
			flags |= SHF_EXECINSTR;
		};
		
		elfSect.sh_flags = ELF_MAKE64(flags);
		elfSect.sh_size = ELF_MAKE64(sect->size);
		elfSect.sh_addr = ELF_MAKE64(sect->vaddr);
		elfSect.sh_addralign = ELF_MAKE64(sect->align);
		fwrite(&elfSect, 1, sizeof(Elf64_Shdr), fp);
		
		// now the relocation table
		memset(&elfSect, 0, sizeof(Elf64_Shdr));
		elfSect.sh_name = ELF_MAKE32(sect->aux.relnameoff);
#ifdef ELF_RELOC_REL
		elfSect.sh_type = ELF_MAKE32(SHT_REL);
		elfSect.sh_entsize = ELF_MAKE64(sizeof(Elf64_Rel));
		elfSect.sh_size = ELF_MAKE64(sizeof(Elf64_Rel) * sect->aux.numRelocs);
#else
		elfSect.sh_type = ELF_MAKE32(SHT_RELA);
		elfSect.sh_entsize = ELF_MAKE64(sizeof(Elf64_Rela));
		elfSect.sh_size = ELF_MAKE64(sizeof(Elf64_Rela) * sect->aux.numRelocs);
#endif
		elfSect.sh_link = ELF_MAKE32(SHN_SYMTAB);
		elfSect.sh_info = ELF_MAKE32(index);
		if (sect->aux.numRelocs != 0) elfSect.sh_offset = ELF_MAKE64(sect->aux.relOff);
		fwrite(&elfSect, 1, sizeof(Elf64_Shdr), fp);
		
		index += 2;
	};
	
	/* program header table */
	Elf64_Phdr phdr;
	memset(&phdr, 0, sizeof(Elf64_Phdr));
	fwrite(&phdr, 1, sizeof(Elf64_Phdr), fp);
	for (sect=obj->sections; sect!=NULL; sect=sect->next)
	{
		phdr.p_type = ELF_MAKE32(PT_LOAD);
		if (strcmp(sect->name, ".interp") == 0)
		{
			phdr.p_type = ELF_MAKE32(PT_INTERP);
		};
		
		int flags;
		if (sect->flags & SEC_READ) flags |= PF_R;
		if (sect->flags & SEC_WRITE) flags |= PF_W;
		if (sect->flags & SEC_EXEC) flags |= PF_X;
		
		phdr.p_flags = ELF_MAKE32(flags);
		phdr.p_offset = ELF_MAKE64(sect->aux.dataOff);
		phdr.p_vaddr = ELF_MAKE64(sect->vaddr);
		phdr.p_paddr = ELF_MAKE64(sect->paddr);
		
		if (sect->type == SECTYPE_NOBITS)
		{
			phdr.p_filesz = ELF_MAKE64(0);
		}
		else
		{
			phdr.p_filesz = ELF_MAKE64(sect->size);
		};
		
		phdr.p_memsz = ELF_MAKE64(sect->size);
		phdr.p_align = ELF_MAKE64(sect->align);
		fwrite(&phdr, 1, sizeof(Elf64_Phdr), fp);
		
		if (strcmp(sect->name, ".dynamic") == 0)
		{
			phdr.p_type = ELF_MAKE32(PT_DYNAMIC);
			phdr.p_flags = ELF_MAKE32(flags);
			phdr.p_offset = ELF_MAKE64(sect->aux.dataOff);
			phdr.p_vaddr = ELF_MAKE64(sect->vaddr);
			phdr.p_paddr = ELF_MAKE64(sect->paddr);
			phdr.p_filesz = ELF_MAKE64(sect->size);
			phdr.p_memsz = ELF_MAKE64(sect->size);
			phdr.p_align = ELF_MAKE64(sect->align);
			fwrite(&phdr, 1, sizeof(Elf64_Phdr), fp);
		};
	};
	
	/* section name string table */
	fwrite(sectNames.data, 1, sectNames.size, fp);
	
	/* symbol table */
	fwrite(symtab, numSymbols, sizeof(Elf64_Sym), fp);
	
	/* string table */
	fwrite(symstrtab.data, symstrtab.size, 1, fp);
	
	/* section data */
	for (sect=obj->sections; sect!=NULL; sect=sect->next)
	{
		if (sect->type == SECTYPE_PROGBITS)
		{
			fwrite(zeroPage, 1, sect->aux.padSize, fp);
			fwrite(sect->data, 1, sect->size, fp);
			
			Reloc *reloc;
			for (reloc=sect->relocs; reloc!=NULL; reloc=reloc->next)
			{
				size_t i;
				for (i=0; i<numSymbols; i++)
				{
					uint32_t nameOffset = ELF_READ32(symtab[i].st_name);
					if (strcmp(&symstrtab.data[nameOffset], reloc->symbol) == 0)
					{
						break;
					};
				};
				
				// we simply assume the symbol was found, because we created them all above
#ifdef ELF_RELOC_REL
				Elf64_Rel rel;
				memset(&rel, 0, sizeof(Elf64_Rel));
#else
				Elf64_Rela rel;
				memset(&rel, 0, sizeof(Elf64_Rela));
				rel.r_addend = ELF_MAKE64((uint64_t) reloc->addend);
#endif
				rel.r_offset = ELF_MAKE64(reloc->offset);
				uint64_t t = elfGetReloc(reloc->size, reloc->type);
				rel.r_info = ELF_MAKE64(ELF64_R_INFO(i, t));

#ifdef ELF_RELOC_REL
				fwrite(&rel, 1, sizeof(Elf64_Rel), fp);
#else
				fwrite(&rel, 1, sizeof(Elf64_Rela), fp);
#endif
			};
		};
	};
	
	fclose(fp);
	return 0;
};
