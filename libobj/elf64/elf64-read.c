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
				};
			};
		};
	};
	
	// TODO: symbols, relocations, entry point
	
	free(filebuf);
	return obj;
};
