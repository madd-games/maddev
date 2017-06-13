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

#include <stdlib.h>
#include <string.h>

#include "libobj.h"

Object* objNew()
{
	Object *obj = (Object*) malloc(sizeof(Object));
	obj->sections = NULL;
	obj->symbols = NULL;
	obj->type = OBJTYPE_RELOC;
	return obj;
};

Section* objGetSection(Object *obj, const char *name)
{
	Section *sect;
	for (sect=obj->sections; sect!=NULL; sect=sect->next)
	{
		if (strcmp(sect->name, name) == 0) return sect;
	};
	
	return NULL;
};

Section* objCreateSection(Object *obj, const char *name, int type, int flags)
{
	Section *sect = (Section*) malloc(sizeof(Section));
	sect->name = strdup(name);
	sect->data = NULL;
	sect->size = 0;
	sect->relocs = NULL;
	sect->type = type;
	sect->flags = flags;
	sect->vaddr = 0;
	sect->paddr = 0;
	sect->align = 0x1000;
	
	sect->next = obj->sections;
	obj->sections = sect;
	
	return sect;
};

int objSectionAppend(Section *sect, const void *data, size_t size)
{
	if (sect->type == SECTYPE_NOBITS) return -1;
	
	sect->data = realloc(sect->data, sect->size + size);
	memcpy((char*) sect->data + sect->size, data, size);
	sect->size += size;
	
	return 0;
};

int objSectionReloc(Section *sect, const char *symbol, int size, int type, int64_t addend)
{
	if (sect->type == SECTYPE_NOBITS) return -1;
	
	Reloc *reloc = (Reloc*) malloc(sizeof(Reloc));
	reloc->type = type;
	reloc->size = size;
	reloc->offset = (int64_t) sect->size;
	reloc->symbol = strdup(symbol);
	reloc->addend = addend;
	
	reloc->next = sect->relocs;
	sect->relocs = reloc;
	
	sect->data = realloc(sect->data, sect->size + size);
	memset((char*) sect->data + sect->size, 0, size);
	sect->size += size;
	
	return 0;
};

int objSectionResv(Section *sect, size_t size)
{
	if (sect->type == SECTYPE_PROGBITS) return -1;
	
	sect->size += size;
	return 0;
};

int objAddSymbol(Object *obj, Section *sect, const char *name)
{
	Symbol *sym;
	for (sym=obj->symbols; sym!=NULL; sym=sym->next)
	{
		if (strcmp(sym->name, name) == 0)
		{
			if (sym->offset == -1)
			{
				sym->offset = (int64_t) sect->size;
				sym->sect = sect;
				return 0;
			}
			else
			{
				return -1;
			};
		};
	};
	
	sym = (Symbol*) malloc(sizeof(Symbol));
	sym->name = strdup(name);
	sym->flags = 0;
	sym->offset = (int64_t) sect->size;
	sym->size = 0;
	sym->binding = SYMB_LOCAL;
	sym->type = SYMT_NONE;
	sym->sect = sect;
	
	sym->next = obj->symbols;
	obj->symbols = sym;
	
	return 0;
};

void objSymbolBinding(Object *obj, const char *name, int binding)
{
	Symbol *sym;
	for (sym=obj->symbols; sym!=NULL; sym=sym->next)
	{
		if (strcmp(sym->name, name) == 0)
		{
			sym->binding = binding;
			return;
		};
	};
	
	sym = (Symbol*) malloc(sizeof(Symbol));
	sym->name = strdup(name);
	sym->flags = 0;
	sym->offset = -1;
	sym->size = 0;
	sym->binding = binding;
	sym->type = SYMT_NONE;
	sym->sect = NULL;
	
	sym->next = obj->symbols;
	obj->symbols = sym;
};

void objSymbolType(Object *obj, const char *name, int type)
{
	Symbol *sym;
	for (sym=obj->symbols; sym!=NULL; sym=sym->next)
	{
		if (strcmp(sym->name, name) == 0)
		{
			sym->type = type;
			return;
		};
	};
	
	sym = (Symbol*) malloc(sizeof(Symbol));
	sym->name = strdup(name);
	sym->flags = 0;
	sym->offset = -1;
	sym->size = 0;
	sym->binding = SYMB_LOCAL;
	sym->type = type;
	sym->sect = NULL;
	
	sym->next = obj->symbols;
	obj->symbols = sym;
};

void objSymbolSize(Object *obj, const char *name, size_t size)
{
	Symbol *sym;
	for (sym=obj->symbols; sym!=NULL; sym=sym->next)
	{
		if (strcmp(sym->name, name) == 0)
		{
			sym->size = size;
			return;
		};
	};
	
	sym = (Symbol*) malloc(sizeof(Symbol));
	sym->name = strdup(name);
	sym->flags = 0;
	sym->offset = -1;
	sym->size = size;
	sym->binding = SYMB_LOCAL;
	sym->type = SYMT_NONE;
	sym->sect = NULL;
	
	sym->next = obj->symbols;
	obj->symbols = sym;
};

ObjLE16 OBJ_MAKE_LE16(uint16_t val)
{
	uint8_t bytes[2];
	bytes[0] = val & 0xFF;
	bytes[1] = (val >> 8) & 0xFF;
	
	ObjLE16 data;
	data.val = *((uint16_t*)bytes);
	return data;
};

uint16_t OBJ_READ_LE16(ObjLE16 val)
{
	uint8_t *bytes = (uint8_t*) &val.val;
	return (uint16_t)bytes[0] | ((uint16_t)bytes[1] << 8);
};

ObjBE16 OBJ_MAKE_BE16(uint16_t val)
{
	uint8_t bytes[2];
	bytes[1] = val & 0xFF;
	bytes[0] = (val >> 8) & 0xFF;
	
	ObjBE16 data;
	data.val = *((uint16_t*)bytes);
	return data;
};

uint16_t OBJ_READ_BE16(ObjBE16 val)
{
	uint8_t *bytes = (uint8_t*) &val.val;
	return (uint16_t)bytes[1] | ((uint16_t)bytes[2] << 8);
};

ObjLE32 OBJ_MAKE_LE32(uint32_t val)
{
	uint8_t bytes[4];
	bytes[0] = val & 0xFF;
	bytes[1] = (val >> 8) & 0xFF;
	bytes[2] = (val >> 16) & 0xFF;
	bytes[3] = (val >> 24) & 0xFF;
	
	ObjLE32 data;
	data.val = *((uint32_t*)bytes);
	return data;
};

uint32_t OBJ_READ_LE32(ObjLE32 val)
{
	uint8_t *bytes = (uint8_t*) &val.val;
	return (uint32_t)bytes[0] | ((uint32_t)bytes[1] << 8) | ((uint32_t)bytes[2] << 16) | ((uint32_t)bytes[3] << 24);
};

ObjBE32 OBJ_MAKE_BE32(uint32_t val)
{
	uint8_t bytes[4];
	bytes[3] = val & 0xFF;
	bytes[2] = (val >> 8) & 0xFF;
	bytes[1] = (val >> 16) & 0xFF;
	bytes[0] = (val >> 24) & 0xFF;
	
	ObjBE32 data;
	data.val = *((uint32_t*)bytes);
	return data;
};

uint32_t OBJ_READ_BE32(ObjBE32 val)
{
	uint8_t *bytes = (uint8_t*) &val.val;
	return (uint32_t)bytes[3] | ((uint32_t)bytes[2] << 8) | ((uint32_t)bytes[1] << 16) | ((uint32_t)bytes[0] << 24);
};

ObjLE64 OBJ_MAKE_LE64(uint64_t val)
{
	uint8_t bytes[8];
	bytes[0] = val & 0xFF;
	bytes[1] = (val >> 8) & 0xFF;
	bytes[2] = (val >> 16) & 0xFF;
	bytes[3] = (val >> 24) & 0xFF;
	bytes[4] = (val >> 32) & 0xFF;
	bytes[5] = (val >> 40) & 0xFF;
	bytes[6] = (val >> 48) & 0xFF;
	bytes[7] = (val >> 56) & 0xFF;
	
	ObjLE64 data;
	data.val = *((uint64_t*)bytes);
	return data;
};

uint64_t OBJ_READ_LE64(ObjLE64 val)
{
	uint8_t *bytes = (uint8_t*) &val.val;
	return (uint64_t)bytes[0] | ((uint64_t)bytes[1] << 8) | ((uint64_t)bytes[2] << 16) | ((uint64_t)bytes[3] << 24)
		| ((uint64_t)bytes[4] << 32) | ((uint64_t)bytes[5] << 40) | ((uint64_t)bytes[6] << 48)
		| ((uint64_t)bytes[7] << 56);
};

ObjBE64 OBJ_MAKE_BE64(uint64_t val)
{
	uint8_t bytes[8];
	bytes[7] = val & 0xFF;
	bytes[6] = (val >> 8) & 0xFF;
	bytes[5] = (val >> 16) & 0xFF;
	bytes[4] = (val >> 24) & 0xFF;
	bytes[3] = (val >> 32) & 0xFF;
	bytes[2] = (val >> 40) & 0xFF;
	bytes[1] = (val >> 48) & 0xFF;
	bytes[0] = (val >> 56) & 0xFF;
	
	ObjBE64 data;
	data.val = *((uint64_t*)bytes);
	return data;
};

uint64_t OBJ_READ_BE64(ObjBE64 val)
{
	uint8_t *bytes = (uint8_t*) &val.val;
	return (uint64_t)bytes[7] | ((uint64_t)bytes[6] << 8) | ((uint64_t)bytes[5] << 16) | ((uint64_t)bytes[4] << 24)
		| ((uint64_t)bytes[3] << 32) | ((uint64_t)bytes[2] << 40) | ((uint64_t)bytes[1] << 48)
		| ((uint64_t)bytes[0] << 56);
};
