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

#ifndef LIBOBJ_H_
#define LIBOBJ_H_

#include <inttypes.h>
#include <stdlib.h>

#include "targaux.h"
#include "relocs.h"

/**
 * Section types.
 */
#define	SECTYPE_NOBITS				0
#define	SECTYPE_PROGBITS			1

/**
 * Sections flags/permissions.
 */
#define	SEC_READ				(1 << 0)
#define	SEC_WRITE				(1 << 1)
#define	SEC_EXEC				(1 << 2)

/**
 * Symbol bindings.
 */
#define	SYMB_LOCAL				0
#define	SYMB_GLOBAL				1
#define	SYMB_WEAK				2

/**
 * Symbol types.
 */
#define	SYMT_NONE				0
#define	SYMT_OBJECT				1
#define	SYMT_FUNC				2

/**
 * Macros for relocation sizes.
 */
#define	REL_BYTE				1
#define	REL_WORD				2
#define	REL_DWORD				4
#define	REL_QWORD				8

/**
 * Object types.
 */
#define	OBJTYPE_RELOC				0	/* relocatable object */
#define	OBJTYPE_EXEC				1	/* executable file */
#define	OBJTYPE_SHARED				2	/* shared library */

/**
 * Offset value for a common symbol.
 */
#define	SYMOFF_COMMON				(~((uint64_t)0))

/**
 * Structures used to represent types in specific byte order. We define that as structures to
 * prevent the compiler from accepting writes to the wrong type.
 */
#define	OBJ_NEWTYPE(oldtype, newname) typedef struct {oldtype val;} newname;
OBJ_NEWTYPE(uint16_t, ObjLE16);
OBJ_NEWTYPE(uint16_t, ObjBE16);
OBJ_NEWTYPE(uint32_t, ObjLE32);
OBJ_NEWTYPE(uint32_t, ObjBE32);
OBJ_NEWTYPE(uint64_t, ObjLE64);
OBJ_NEWTYPE(uint64_t, ObjBE64);

/**
 * Abstract description of a relocation.
 */
typedef struct Reloc_
{
	/**
	 * Link.
	 */
	struct Reloc_*				next;
	
	/**
	 * Type of relocation.
	 */
	int					type;
	
	/**
	 * Size of relocation (1, 2, 4, or 8, etc).
	 */
	int					size;
	
	/**
	 * Offset into the section.
	 */
	int64_t					offset;
	
	/**
	 * Name of the symbol against which this relocation is to be calculated.
	 */
	char*					symbol;
	
	/**
	 * Addend.
	 */
	int64_t					addend;
} Reloc;

/**
 * Abstract description of a section.
 */
typedef struct Section_
{
	/**
	 * Link.
	 */
	struct Section_*			next;
	
	/**
	 * Section name. Note that it may be limited to 8 bytes!
	 */
	char*					name;
	
	/**
	 * Section data and current size ('data' is NULL for SECTYPE_NOBITS).
	 */
	void*					data;
	size_t					size;
	
	/**
	 * List of relocations.
	 */
	Reloc*					relocs;
	
	/**
	 * Section type (SECTYPE_*).
	 */
	int					type;
	
	/**
	 * Sections flags (SEC_*).
	 */
	int					flags;
	
	/**
	 * Virtual and physical addresses of section.
	 */
	uint64_t				vaddr;
	uint64_t				paddr;
	
	/**
	 * Alignment.
	 */
	uint64_t				align;
	
	/**
	 * Auxillary information, produced by the backend.
	 */
	SectionAux				aux;
} Section;

/**
 * Abstract description of a symbol.
 */
typedef struct Symbol_
{
	/**
	 * Link.
	 */
	struct Symbol_*				next;
	
	/**
	 * The section to which this symbol belongs. NULL means absolute symbol.
	 */
	Section*				sect;
	
	/**
	 * Name of the symbol.
	 */
	char*					name;
	
	/**
	 * Symbol flags (currently must be zero).
	 */
	int					flags;
	
	/**
	 * The offset from the start of memory where this symbol is defined. If set to SYMOFF_COMMON,
	 * then this is a common symbol.
	 */
	uint64_t				offset;
	
	/**
	 * Size of symbol (if applicable; else 0).
	 */
	size_t					size;
	
	/**
	 * Binding of the symbol (SYMB_*).
	 */
	int					binding;
	
	/**
	 * Type of symbol (SYMT_*).
	 */
	int					type;
	
	/**
	 * Alignment, for common symbols. Ignored for anything else.
	 */
	uint64_t				align;
} Symbol;

/**
 * Abstract description of an object file.
 */
typedef struct
{
	/**
	 * List of sections in this object.
	 */
	Section*				sections;
	
	/**
	 * Symbols.
	 */
	Symbol*					symbols;
	
	/**
	 * Object type (OBJTYPE_*). Default is OBJTYPE_RELOC.
	 */
	int					type;
	
	/**
	 * Name of the entry symbol (on the heap), or NULL if unspecified.
	 */
	char*					entry;
} Object;

/**
 * Functions used to read/write byte-order-specific type (ObjLE16 etc).
 * All defined in common/libobj.c.
 */
ObjLE16 OBJ_MAKE_LE16(uint16_t val);
uint16_t OBJ_READ_LE16(ObjLE16 val);
ObjBE16 OBJ_MAKE_BE16(uint16_t val);
uint16_t OBJ_READ_BE16(ObjBE16 val);

ObjLE32 OBJ_MAKE_LE32(uint32_t val);
uint32_t OBJ_READ_LE32(ObjLE32 val);
ObjBE32 OBJ_MAKE_BE32(uint32_t val);
uint32_t OBJ_READ_BE32(ObjBE32 val);

ObjLE64 OBJ_MAKE_LE64(uint64_t val);
uint64_t OBJ_READ_LE64(ObjLE64 val);
ObjBE64 OBJ_MAKE_BE64(uint64_t val);
uint64_t OBJ_READ_BE64(ObjBE64 val);

/**
 * Create a new, empty object. Defined in common/libobj.c.
 */
Object* objNew();

/**
 * Get a section by name. Returns NULL if no such section exists. Defined in common/libobj.c.
 */
Section* objGetSection(Object *obj, const char *name);

/**
 * Create a new section with the specified attributes, and return it. NOTE: You must first make
 * sure it doesn't already exist, using objGetSection()! Defined in common/libobj.c.
 */
Section* objCreateSection(Object *obj, const char *name, int type, int flags);

/**
 * Append data to a section. If the section is SECTYPE_NOBITS, this fails and returns -1. Otherwise,
 * it returns 0. Defined in common/libobj.c.
 */
int objSectionAppend(Section *sect, const void *data, size_t size);

/**
 * Add a relocation to a section. The section must be PROGBITS. This automatically emits the correct
 * number of bytes (zeroes), and adds a relocation entry. If the section is not PROGBITS, returns -1;
 * otherwise returns 0 on success. Defined in common/libobj.c.
 */
int objSectionReloc(Section *sect, const char *symbol, int size, int type, int64_t addend);

/**
 * Add a relocation to a section.
 */
void objSectionAddReloc(Section *sect, Reloc *rel);

/**
 * Reserve some number of bytes in a section, at the end. If this section is SECTYPE_PROGBITS, this
 * fails and returns -1. Otherwise, it returns 0. Defined in common/libobj.c.
 */
int objSectionResv(Section *sect, size_t size);

/**
 * Emit a symbol definition at the end of the section, with the given name. Returns -1 if the symbol
 * is already defined; returns 0 on success. Defined in common/libobj.c.
 */
int objAddSymbol(Object *obj, Section *sect, const char *name);

/**
 * Set the binding of a symbol. It does not have to be defined yet.
 */
void objSymbolBinding(Object *obj, const char *name, int binding);

/**
 * Set the type of a symbol. It does not have to be defined yet.
 */
void objSymbolType(Object *obj, const char *name, int type);

/**
 * Set the size of a symbol. It does not have to be defined yet.
 */
void objSymbolSize(Object *obj, const char *name, size_t size);

/**
 * Define an absolute symbol with the specified name, value, alignment and size.
 * If 'value' is SYMOFF_COMMON, it's a common symbol.
 * Alignment is ignored except for common symbols.
 */
void objAbsoluteSymbol(Object *obj, const char *name, uint64_t value, uint64_t align, uint64_t size);

/**
 * Write an object description to an object file. This is implemented by the backend. Returns 0 on
 * success, or an error number on error.
 */
int objWrite(Object *obj, const char *filename);

/**
 * Read an object description from an object file. This is implemented by the backend. Returns NULL on
 * error. This reads exact contents from the file; in order to attempt converting into a relocatable
 * object file (e.g. to implicitly create an import library), use objImport().
 */
Object* objRead(const char *filename);

/**
 * Import an object. For normal relocatable files, this simply reads the object; for shared libraries it
 * generates an import library. Always returns an object of type OBJTYPE_RELOC. On error returns NULL.
 */
Object* objImport(const char *filename);

#endif
