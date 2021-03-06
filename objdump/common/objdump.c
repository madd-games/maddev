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
#include "libobj.h"

const char *getTypeName(int type)
{
	switch (type)
	{
	case OBJTYPE_RELOC:
		return "Relocatable File";
	case OBJTYPE_EXEC:
		return "Executable";
	case OBJTYPE_SHARED:
		return "Shared Library";
	default:
		return "Unknown Object";
	};
};

const char *getSectypeName(int type)
{
	switch (type)
	{
	case SECTYPE_PROGBITS:
		return "PROGBITS";
	case SECTYPE_NOBITS:
		return "NOBITS";
	default:
		return "?";
	};
};

const char *getSymtName(int symt)
{
	switch (symt)
	{
	case SYMT_NONE:
		return "NONE";
	case SYMT_FUNC:
		return "FUNC";
	case SYMT_OBJECT:
		return "OBJECT";
	default:
		return "?";
	};
};

const char *getSymbName(int symb)
{
	switch (symb)
	{
	case SYMB_LOCAL:
		return "LOCAL";
	case SYMB_GLOBAL:
		return "GLOBAL";
	case SYMB_WEAK:
		return "WEAK";
	default:
		return "?";
	};
};

const char *getRelszName(int relsz)
{
	switch (relsz)
	{
	case REL_BYTE:
		return "BYTE";
	case REL_WORD:
		return "WORD";
	case REL_DWORD:
		return "DWORD";
	case REL_QWORD:
		return "QWORD";
	default:
		return "?";
	};
};

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "USAGE:\t%s <option> <filename>\n", argv[0]);
		fprintf(stderr, "\tDump information from an object file.\n");
		fprintf(stderr, "\tAll options show basic info, plus additional info as follows:\n");
		fprintf(stderr, "\n");
		fprintf(stderr, "\t-l\tList sections.\n");
		fprintf(stderr, "\t-s\tList symbols.\n");
		fprintf(stderr, "\t-r\tList relocations.\n");
		return 1;
	};
	
	Object *obj = objRead(argv[2]);
	if (obj == NULL)
	{
		fprintf(stderr, "%s: failed to read object file %s\n", argv[0], argv[2]);
		return 1;
	};
	
	printf("File `%s': %s, entry point %s\n\n",
		argv[2], getTypeName(obj->type), (obj->entry != NULL) ? obj->entry : "<null>"
	);
	
	if (strcmp(argv[1], "-l") == 0)
	{
		printf("Sections:\n");
		printf("%-20s%-10s%-20s%-20s%-20s%-20s%-10s\n", "Name", "Type", "Address", "P. Address",
								"Size", "Alignment", "Flags");
		
		Section *sect;
		for (sect=obj->sections; sect!=NULL; sect=sect->next)
		{
			printf("%-20s%-10s0x%016lX  0x%016lX  0x%016lX  0x%016lX  %c%c%c\n",
				sect->name, getSectypeName(sect->type), sect->vaddr, sect->paddr, sect->size, sect->align,
				(sect->flags & SEC_READ) ? 'R':' ',
				(sect->flags & SEC_WRITE) ? 'W':' ',
				(sect->flags & SEC_EXEC) ? 'X':' '
			);
		};
	}
	else if (strcmp(argv[1], "-s") == 0)
	{
		printf("Symbols:\n");
		printf("%-40s%-10s%-10s%s\n", "Name", "Type", "Binding", "Value");
		
		Symbol *sym;
		for (sym=obj->symbols; sym!=NULL; sym=sym->next)
		{
			printf("%-40s%-10s%-10s%s+0x%lX\n", sym->name, getSymtName(sym->type), getSymbName(sym->binding),
				sym->sect->name, sym->offset);
		};
	}
	else if (strcmp(argv[1], "-r") == 0)
	{
		printf("Relocations:\n");
		printf("%-30s%-10s%-20s%s\n", "Position", "Size", "Type", "Target");
		
		Section *sect;
		for (sect=obj->sections; sect!=NULL; sect=sect->next)
		{	
			Reloc *reloc;
			for (reloc=sect->relocs; reloc!=NULL; reloc=reloc->next)
			{
				char offspec[64];
				snprintf(offspec, 64, "%s+0x%016lX", sect->name, reloc->offset);
				printf("%-30s%-10s%-20s%s%+ld\n", offspec, getRelszName(reloc->size),
					objGetRelocTypeName(reloc->type), reloc->symbol, reloc->addend);
			};
		};
	}
	else
	{
		fprintf(stderr, "%s: unknown option `%s'\n", argv[0], argv[1]);
		return 1;
	};
	
	return 0;
};
