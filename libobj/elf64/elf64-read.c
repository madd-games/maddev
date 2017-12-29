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

Object* objRead(const char *filename)
{
	// read the whole file into memory
	FILE *fp = fopen(filename, "rb");
	if (fp == NULL) return NULL;
	
	fseek(fp, 0, SEEK_END);
	long int filesize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	char *filebuf = (char*) malloc(filesize);
	fread(filebuf, 1, filesize, fp);
	fclose(fp);
	
	// validate the ELF64 header
	Elf64_Ehdr *ehdr = (Elf64_Ehdr*) filebuf;
	if (ehdr->e_ident[EI_MAG0] != 0x7F) return NULL;
	if (ehdr->e_ident[EI_MAG1] != 'E') return NULL;
	if (ehdr->e_ident[EI_MAG2] != 'L') return NULL;
	if (ehdr->e_ident[EI_MAG3] != 'F') return NULL;
	if (ehdr->e_ident[EI_CLASS] != ELFCLASS64) return NULL;
	if (ehdr->e_ident[EI_DATA] != ELF_TARGET_DATA) return NULL;
	if (ehdr->e_ident[EI_VERSION] != EV_CURRENT) return NULL;
	if (ehdr->e_ident[EI_OSABI] != ELFOSABI_SYSV) return NULL;
	if (ELF_READ16(ehdr->e_shentsize) != sizeof(Elf64_Shdr)) return NULL;
	
	Elf64_Shdr *sectable = (Elf64_Shdr*) (filebuf + ELF_READ64(ehdr->e_shoff));
	size_t numSections = ELF_READ16(ehdr->e_shnum);
	
	Elf64_Phdr *progHeads = (Elf64_Phdr*) (filebuf + ELF_READ64(ehdr->e_phoff));
	size_t numProgHeads = ELF_READ16(ehdr->e_phnum);
	
	Section **sectPointers = (Section**) malloc(sizeof(void*) * numSections);
	
	Object *obj = objNew();
	switch (ELF_READ16(ehdr->e_type))
	{
	default:
	case ET_REL:
		obj->type = OBJTYPE_RELOC;
		break;
	case ET_EXEC:
		obj->type = OBJTYPE_EXEC;
		break;
	case ET_DYN:
		obj->type = OBJTYPE_SHARED;
		break;
	};

	// load in the PROGBITS/NOBITS sections with the ALLOC flag
	char *sectNameStrtab = filebuf + ELF_READ64(sectable[ELF_READ16(ehdr->e_shstrndx)].sh_offset);
	size_t i;
	for (i=0; i<numSections; i++)
	{
		uint32_t type = ELF_READ32(sectable[i].sh_type);
		if ((type == SHT_PROGBITS) || (type == SHT_NOBITS))
		{
			int libobjType;
			if (type == SHT_PROGBITS) libobjType = SECTYPE_PROGBITS;
			else libobjType = SECTYPE_NOBITS;
			
			uint64_t flags = ELF_READ64(sectable[i].sh_flags);
			int libobjFlags = 0;
			
			if (flags & SHF_ALLOC)
			{
				libobjFlags = SEC_READ;
				if (flags & SHF_WRITE) libobjFlags |= SEC_WRITE;
				if (flags & SHF_EXECINSTR) libobjFlags |= SEC_EXEC;
				
				char *name = sectNameStrtab + ELF_READ32(sectable[i].sh_name);
				
				Section *sect = objCreateSection(obj, name, libobjType, libobjFlags);
				sectPointers[i] = sect;
				if (sect != NULL)
				{
					if (type == SHT_PROGBITS)
					{
						objSectionAppend(
							sect,
							filebuf + ELF_READ64(sectable[i].sh_offset),
							ELF_READ64(sectable[i].sh_size)
						);
					}
					else
					{
						objSectionResv(
							sect,
							ELF_READ64(sectable[i].sh_size)
						);
					};
					
					sect->vaddr = ELF_READ64(sectable[i].sh_addr);
					sect->paddr = sect->vaddr;
					sect->align = ELF_READ64(sectable[i].sh_addralign);
					sect->aux.index = i;
					
					// cross-reference with the program headers to find the physical
					// address. if it's not there, we just default to the virtual
					// address being equal to the physical.
					size_t j;
					for (j=0; j<numProgHeads; j++)
					{
						Elf64_Phdr *phdr = &progHeads[j];
						uint64_t vaddr = ELF_READ64(phdr->p_vaddr);
						
						if (vaddr == sect->vaddr)
						{
							sect->paddr = ELF_READ64(phdr->p_paddr);
							break;
						};
					};
				};
			};
		};
	};
	
	// now load all the SYMTAB sections, to get symbols
	for (i=0; i<numSections; i++)
	{
		uint32_t type = ELF_READ32(sectable[i].sh_type);
		if (type == SHT_SYMTAB)
		{
			Elf64_Shdr *sectStrTab = &sectable[ELF_READ32(sectable[i].sh_link)];
			char *strtab = (filebuf + ELF_READ64(sectStrTab->sh_offset));
			
			// only bother if the size is correct
			if (ELF_READ64(sectable[i].sh_entsize) != sizeof(Elf64_Sym))
			{
				return NULL;
			};
			
			size_t numSyms = ELF_READ64(sectable[i].sh_size) / sizeof(Elf64_Sym);
			Elf64_Sym *symtab = (Elf64_Sym*) (filebuf + ELF_READ64(sectable[i].sh_offset));
			
			size_t j;
			for (j=0; j<numSyms; j++)
			{
				if (ELF_READ32(symtab[j].st_name) != 0)
				{
					const char *name = &strtab[ELF_READ32(symtab[j].st_name)];
					int type = symtab[j].st_info & 0xF;
					int binding = symtab[j].st_info >> 4;
					
					int libobjType;
					int found = 1;
					switch (type)
					{
					case STT_NOTYPE:
						libobjType = SYMT_NONE;
						break;
					case STT_OBJECT:
						libobjType = SYMT_OBJECT;
						break;
					case STT_FUNC:
						libobjType = SYMT_FUNC;
						break;
					default:
						found = 0;
						break;
					};

					switch (ELF_READ16(symtab[j].st_shndx))
					{
					case SHN_UNDEF:
						found = 0;
						break;
					};
					
					if (!found) continue;
					Symbol *symbol = (Symbol*) malloc(sizeof(Symbol));
					memset(symbol, 0, sizeof(Symbol));
					if (ELF_READ16(symtab[j].st_shndx) == SHN_ABS || ELF_READ16(symtab[j].st_shndx) == SHN_COMMON)
					{
						symbol->sect = NULL;
					}
					else
					{
						symbol->sect = sectPointers[ELF_READ16(symtab[j].st_shndx)];
					};
					symbol->name = strdup(name);
					symbol->offset = ELF_READ64(symtab[j].st_value);
					symbol->size = ELF_READ64(symtab[j].st_size);
					symbol->type = libobjType;
					
					switch (binding)
					{
					case STB_GLOBAL:
						symbol->binding = SYMB_GLOBAL;
						break;
					case STB_WEAK:
						symbol->binding = SYMB_WEAK;
						break;
					case STB_LOCAL:
					default:
						symbol->binding = SYMB_LOCAL;
						break;
					};
					
					if (ELF_READ16(symtab[j].st_shndx) == SHN_COMMON)
					{
						symbol->offset = SYMOFF_COMMON;
						symbol->align = ELF_READ64(symtab[j].st_value);
					};
					
					symbol->next = obj->symbols;
					obj->symbols = symbol;
				};
			};
		};
	};
	
	// now go through all the REL(A) sections (currently only RELA)
	for (i=0; i<numSections; i++)
	{
		uint32_t type = ELF_READ32(sectable[i].sh_type);
		if (type == SHT_RELA)
		{
			Section *section = sectPointers[ELF_READ32(sectable[i].sh_info)];
			Elf64_Shdr *sectSymbols = &sectable[ELF_READ32(sectable[i].sh_link)];
			Elf64_Shdr *sectStrTab = &sectable[ELF_READ32(sectSymbols->sh_link)];
			char *strtab = (filebuf + ELF_READ64(sectStrTab->sh_offset));
			Elf64_Sym *symtab = (Elf64_Sym*) (filebuf + ELF_READ64(sectSymbols->sh_offset));
			
			if (ELF_READ64(sectable[i].sh_entsize) != sizeof(Elf64_Rela))
			{
				// invalid size
				return NULL;
			};
			
			size_t numRela = ELF_READ64(sectable[i].sh_size) / sizeof(Elf64_Rela);
			Elf64_Rela *rela = (Elf64_Rela*) (filebuf + ELF_READ64(sectable[i].sh_offset));
			
			for (; numRela--; rela++)
			{
				uint64_t info = ELF_READ64(rela->r_info);
				uint64_t offset = ELF_READ64(rela->r_offset);
				int64_t addend = ELF_READ64(rela->r_addend);
				
				uint64_t type = ELF64_R_TYPE(info);
				uint64_t symidx = ELF64_R_SYM(info);
				
				Reloc *reloc = (Reloc*) malloc(sizeof(Reloc));
				memset(reloc, 0, sizeof(Reloc));
				
				if (elfReadReloc(type, &reloc->size, &reloc->type) != 0) return NULL;
				reloc->offset = offset;
				reloc->symbol = strdup(&strtab[ELF_READ32(symtab[symidx].st_name)]);
				reloc->addend = addend;
				
				objSectionAddReloc(section, reloc);
			};
		};
	};
	
	// TODO: REL-type relocations, entry point
	
	free(sectPointers);
	free(filebuf);
	return obj;
};
