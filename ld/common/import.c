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

#include "import.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Entry into the object pool.
 */
typedef struct PoolEntry_
{
	struct PoolEntry_*		next;
	char*				name;
	Object*				obj;
} PoolEntry;

static PoolEntry* poolFirst;
static PoolEntry* poolLast;

int poolImport(const char *filename)
{
	Object *obj = objImport(filename);
	if (obj == NULL)
	{
		fprintf(stderr, "%s: failed to import `%s'\n", progName, filename);
		return 1;
	};
	
	PoolEntry *ent = (PoolEntry*) malloc(sizeof(PoolEntry));
	ent->next = NULL;
	ent->name = strdup(filename);
	ent->obj = obj;
	
	if (poolFirst == NULL)
	{
		poolFirst = poolLast = ent;
	}
	else
	{
		poolLast->next = ent;
		poolLast = ent;
	};
	
	return 0;
};

void poolLoad(const char *secname)
{
	PoolEntry *ent;
	for (ent=poolFirst; ent!=NULL; ent=ent->next)
	{
		Object *obj = ent->obj;
		Section *sect = objGetSection(obj, secname);
		if (sect == NULL) continue;
		
		if (sect->type != currentSection->type)
		{
			fprintf(stderr, "%s: type mismatch between input section `%s' from `%s' and output section `%s'\n",
				progName, secname, ent->name, currentSection->name);
			errorsOccured = 1;
			return;
		};
		
		if (sect->type == SECTYPE_PROGBITS)
		{
			objSectionAppend(currentSection, sect->data, sect->size);
		}
		else if (sect->type == SECTYPE_NOBITS)
		{
			objSectionResv(currentSection, sect->size);
		}
		else
		{
			fprintf(stderr, "%s: unknown section type! internal bug!\n", progName);
			abort();
		};
		
		// transfer the relocations over
		Reloc *srcReloc;
		for (srcReloc=sect->relocs; srcReloc!=NULL; srcReloc=srcReloc->next)
		{
			Reloc *dstReloc = (Reloc*) malloc(sizeof(Reloc));
			memcpy(dstReloc, srcReloc, sizeof(Reloc));
			
			dstReloc->offset += (currentAddr - currentSection->vaddr);
			
			// if it's against a local symbol in this object, we must convert it to the form
			// "object:symbolName"
			Symbol *sym;
			for (sym=obj->symbols; sym!=NULL; sym=sym->next)
			{
				if (strcmp(sym->name, srcReloc->symbol) == 0)
				{
					if (sym->binding == SYMB_LOCAL)
					{
						size_t namesz = strlen(ent->name) + strlen(sym->name) + 2;
						dstReloc->symbol = (char*) malloc(namesz);
						sprintf(dstReloc->symbol, "%s:%s", ent->name, sym->name);
						break;
					};
				};
			};
			
			objSectionAddReloc(currentSection, dstReloc);
		};
		
		// now transfer the symbols
		Symbol *sym;
		for (sym=obj->symbols; sym!=NULL; sym=sym->next)
		{
			if (sym->sect == sect)
			{
				Symbol *newsym = (Symbol*) malloc(sizeof(Symbol));
				memset(newsym, 0, sizeof(Symbol));
		
				newsym->sect = currentSection;
				if (sym->binding == SYMB_GLOBAL)
				{
					newsym->name = strdup(sym->name);
				}
				else
				{
					newsym->name = (char*) malloc(strlen(ent->name) + strlen(sym->name) + 2);
					sprintf(newsym->name, "%s:%s", ent->name, sym->name);
				};
				newsym->offset = (sym->offset - sect->vaddr) + currentSection->vaddr;
				newsym->binding = SYMB_GLOBAL;
				/* type is irrelevant, won't be used (it's only used in loading) */
		
				newsym->next = result->symbols;
				result->symbols = newsym;
			};
		};
		
		// finally, extend the current address
		currentAddr += sect->size;
	};
};

void poolLoadAbsolute()
{
	PoolEntry *ent;
	for (ent=poolFirst; ent!=NULL; ent=ent->next)
	{
		Object *obj = ent->obj;
		Symbol *sym;
		for (sym=obj->symbols; sym!=NULL; sym=sym->next)
		{
			if (sym->sect == NULL)
			{
				Symbol *newsym = (Symbol*) malloc(sizeof(Symbol));
				memset(newsym, 0, sizeof(Symbol));
	
				newsym->sect = currentSection;
				newsym->name = strdup(sym->name);
				newsym->offset = sym->offset;
				newsym->binding = SYMB_GLOBAL;
				/* type is irrelevant, won't be used (it's only used in loading) */
	
				newsym->next = result->symbols;
				result->symbols = newsym;
			};
		};
	};
};
