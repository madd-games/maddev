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
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "ldscript.h"
#include "libobj.h"
#include "ld.h"
#include "import.h"
#include "reloc.h"
#include "libcommon.h"

int appendLE(long value, size_t bytes)
{
	long working = value;
	uint8_t raw[bytes];
	
	int i;
	for (i=0; i<bytes; i++)
	{
		raw[i] = (working & 0xFF);
		working >>= 8;
	};
	
	// TODO: test for overflows
	
	objSectionAppend(currentSection, raw, bytes);
	return 0;
};

int appendBE(long value, size_t bytes)
{
	long working = value;
	uint8_t raw[bytes];
	
	int i;
	for (i=0; i<bytes; i++)
	{
		raw[bytes-1-i] = (working & 0xFF);
		working >>= 8;
	};
	
	// TODO: test for overflows
	
	objSectionAppend(currentSection, raw, bytes);
	return 0;
};

int appendLE8(long value) {return appendLE(value, 1);};
int appendLE16(long value) {return appendLE(value, 2);};
int appendLE32(long value) {return appendLE(value, 4);};
int appendLE64(long value) {return appendLE(value, 8);};

int appendBE8(long value) {return appendBE(value, 1);};
int appendBE16(long value) {return appendBE(value, 2);};
int appendBE32(long value) {return appendBE(value, 4);};
int appendBE64(long value) {return appendBE(value, 8);};

LinkerType typeLE8 = {NULL, appendLE8, "le8_t"};
LinkerType typeLE16 = {&typeLE8, appendLE16, "le16_t"};
LinkerType typeLE32 = {&typeLE16, appendLE32, "le32_t"};
LinkerType typeLE64 = {&typeLE32, appendLE64, "le64_t"};

LinkerType typeBE8 = {&typeLE64, appendBE8, "be8_t"};
LinkerType typeBE16 = {&typeBE8, appendBE16, "be16_t"};
LinkerType typeBE32 = {&typeBE16, appendBE32, "be32_t"};
LinkerType typeBE64 = {&typeBE32, appendBE64, "be64_t"};

LinkerType *linkerTypes = &typeBE64;

Appender getAppenderByName(const char *name)
{
	LinkerType *type;
	for (type=linkerTypes; type!=NULL; type=type->next)
	{
		if (strcmp(type->name, name) == 0)
		{
			return type->appender;
		};
	};
	
	return NULL;
};

SymEval evalSum(SumExpr *sum);

Symbol* getSymbolByName(const char *name)
{
	Symbol *sym;
	for (sym=result->symbols; sym!=NULL; sym=sym->next)
	{
		if (strcmp(sym->name, name) == 0)
		{
			return sym;
		};
	};
	
	return NULL;
};

/**
 * Evaluate a primary expression.
 */
SymEval evalPrimary(PrimaryExpr *prim)
{
	if (prim->value != NULL)
	{
		SymEval eval;
		eval.sect = NULL;
		eval.offset = strtol(prim->value->value, NULL, 0);
		return eval;
	}
	else if (prim->name != NULL)
	{
		SymEval eval;
		if (strcmp(prim->name->value, ".") == 0)
		{
			eval.sect = currentSection;
			eval.offset = currentAddr;
			
			return eval;
		}
		else
		{
			Symbol *sym = getSymbolByName(prim->name->value);
			if (sym == NULL)
			{
				fprintf(stderr, "%s:%d: error: undefined symbol `%s'\n", prim->filename, prim->lineno,
						prim->name->value);
				errorsOccured = 1;
			};
			
			eval.sect = sym->sect;
			eval.offset = sym->offset;
			return eval;
		};
	}
	else if (prim->align != NULL)
	{
		SymEval eval = evalSum(prim->align->sum);
		if (eval.sect != NULL)
		{
			fprintf(stderr, "%s:%d: error: argument to align() must not be section-relative\n",
				prim->filename, prim->lineno);
			errorsOccured = 1;
		};
		
		if (currentSection != NULL)
		{
			fprintf(stderr, "%s:%d: error: cannot calculate alignment inside a section definition\n",
				prim->filename, prim->lineno);
			errorsOccured = 1;
		};
		
		SymEval result;
		result.sect = NULL;
		result.offset = ((currentAddr + eval.offset - 1) / (eval.offset)) * (eval.offset);
		return result;
	}
	else
	{
		return evalSum(prim->expr->sum);
	};
};

/**
 * Evaluate a sum.
 */
SymEval evalSum(SumExpr *sum)
{
	SymEval out = evalPrimary(sum->primary);
	
	SumAffixList *list;
	for (list=sum->chain; list!=NULL; list=list->next)
	{
		SymEval arg = evalPrimary(list->affix->arg);
		
		const char *op = list->affix->op->value;
		if (strcmp(op, "+") == 0)
		{
			if (arg.sect != NULL)
			{
				fprintf(stderr,
					"%s:%d: error: section-relative address (%s+0x%lX) on right-hand-side of addition\n",
					list->affix->filename, list->affix->lineno,
					arg.sect->name, (uint64_t) arg.offset);
				errorsOccured = 1; 
			}
			else
			{
				out.offset += arg.offset;
			};
		}
		else if (strcmp(op, "-") == 0)
		{
			if (out.sect == arg.sect)
			{
				out.offset -= arg.offset;
			}
			else
			{
				fprintf(stderr, "%s:%d: error: cannot find distance between addresses in different sections\n",
					list->affix->filename, list->affix->lineno);
				errorsOccured = 1;
			};
		}
		else
		{
			fprintf(stderr, "bad operator! internal bug!\n");
			abort();
		};
	};
	
	return out;
};

void handleSymAssign(SymbolAssignment *assign)
{
	// symbol assignment
	const char *name = assign->symbol->value;
	SymEval eval = evalSum(assign->expr->sum);
	
	if (strcmp(name, ".") == 0)
	{
		if (currentSection != NULL)
		{
			fprintf(stderr, "%s:%d: error: attempting to move place pointer (.) inside a section\n",
					assign->filename, assign->lineno);
			errorsOccured = 1;
		}
		else
		{
			if (eval.sect != NULL)
			{
				fprintf(stderr, "%s:%d: error: attempting to move place pointer (.)"
						" to section-relative address\n",
						assign->filename, assign->lineno);
				errorsOccured = 1;
			}
			else
			{
				currentAddr = eval.offset;
			};
		};
	}
	else
	{
		Symbol *sym = getSymbolByName(name);
		if (sym != NULL)
		{
			fprintf(stderr, "%s:%d: error: redefinition of symbol `%s'\n",
					assign->filename, assign->lineno, name);
			errorsOccured = 1;
		}
		else
		{
			sym = (Symbol*) malloc(sizeof(Symbol));
			memset(sym, 0, sizeof(Symbol));
		
			sym->sect = eval.sect;
			sym->name = strdup(name);
			sym->offset = (uint64_t) eval.offset;
			sym->binding = SYMB_GLOBAL;
			/* type is irrelevant, won't be used (it's only used in loading) */
		
			sym->next = result->symbols;
			result->symbols = sym;
		};
	};
};

void processSection(SectionStatementList *list)
{
	for (; list!=NULL; list=list->next)
	{
		SectionStatement *st = list->statement;
		if (st->symAssign != NULL)
		{
			handleSymAssign(st->symAssign);
		}
		else if (st->load != NULL)
		{
			const char *name = st->load->name->value;
			poolLoad(name);
		}
		else if (st->merge != NULL)
		{
			if (currentSection->type != SECTYPE_NOBITS)
			{
				fprintf(stderr, "%s:%d: error: `merge_common' is only allowed in a NOBITS section\n",
					st->filename, st->lineno);
				errorsOccured = 1;
				continue;
			};
			
			poolMergeCommon();
		}
		else if (st->append != NULL)
		{
			if (currentSection->type != SECTYPE_PROGBITS)
			{
				fprintf(stderr, "%s:%d: error: `append' is only allowed in a PROGBITS section\n",
					st->filename, st->lineno);
				errorsOccured = 1;
				continue;
			};
			
			Appender appender = getAppenderByName(st->append->type->value);
			if (appender == NULL)
			{
				fprintf(stderr, "%s:%d: error: unknown type name `%s'\n", st->filename, st->lineno,
					st->append->type->value);
				errorsOccured = 1;
				continue;
			};
			
			SymEval eval = evalSum(st->append->value->sum);
			
			long value = eval.offset;		// section irrelevant
			if (appender(value) != 0)
			{
				fprintf(stderr, "%s:%d: error: expression value (%ld) does not fit in type `%s'\n",
					st->filename, st->lineno, value, st->append->type->value);
				errorsOccured = 1;
			};
		}
		else if (st->str != NULL)
		{
			if (currentSection->type != SECTYPE_PROGBITS)
			{
				fprintf(stderr, "%s:%d: error: `string' is only allowed in a PROGBITS section\n",
					st->filename, st->lineno);
				errorsOccured = 1;
				continue;
			};
			
			const char *str = st->str->str->value;
			char *buffer = (char*) malloc(strlen(str)+1);
			char result = lexParseString(str, buffer);
			
			if (result == -1)
			{
				fprintf(stderr, "%s: a non-string was parsed as a string token! internal bug!\n", progName);
				abort();
			}
			else if (result != 0)
			{
				fprintf(stderr, "%s:%d: error: invalid escape sequence: \\%c\n", st->filename, st->lineno, result);
				errorsOccured = 1;
			}
			else
			{
				objSectionAppend(currentSection, buffer, strlen(buffer));
			};
		}
		else
		{
			fprintf(stderr, "%s: parse tree inconsistent inside `section' statement! internal bug!\n", progName);
			abort();
		};
	};
};

int main(int argc, char *argv[])
{
	progName = argv[0];
	int resultType = OBJTYPE_EXEC;
	const char *ldfile = NULL;
	const char *outfile = "a.out";
	
	int i;
	for (i=1; i<argc; i++)
	{
		if (strcmp(argv[i], "-T") == 0)
		{
			i++;
			if (i == argc)
			{
				fprintf(stderr, "%s: -T option expects a parameter\n", argv[0]);
				return 1;
			};
			
			ldfile = argv[i];
		}
		else if (memcmp(argv[i], "-T", 2) == 0)
		{
			ldfile = &argv[i][2];
		}
		else if (strcmp(argv[i], "-o") == 0)
		{
			i++;
			if (i == argc)
			{
				fprintf(stderr, "%s: -o option expects a parameter\n", argv[0]);
				return 1;
			};
			
			outfile = argv[i];
		}
		else if (memcmp(argv[i], "-o", 2) == 0)
		{
			outfile = &argv[i][2];
		}
		else if (strcmp(argv[i], "-shared") == 0)
		{
			resultType = OBJTYPE_SHARED;
		}
		else if (strcmp(argv[i], "-r") == 0)
		{
			resultType = OBJTYPE_RELOC;
		}
		else if (argv[i][0] != '-')
		{
			if (poolImport(argv[i]) != 0)
			{
				return 1;
			};
		}
		else
		{
			fprintf(stderr, "%s: unrecognised command-line option: %s\n", argv[0], argv[i]);
			return 1;
		};
	};
	
	if (ldfile == NULL)
	{
		fprintf(stderr, "%s: no linker script specified\n", argv[0]);
		return 1;
	};
	
	// load the linker script into memory
	char *ldscript = (char*) malloc(1);
	ldscript[0] = 0;
	
	char linebuf[1024];
	FILE *fp = fopen(ldfile, "r");
	if (fp == NULL)
	{
		fprintf(stderr, "%s: cannot open linker script %s: %s\n", argv[0], ldfile, strerror(errno));
		return 1;
	};
	
	while (fgets(linebuf, 1024, fp) != NULL)
	{
		ldscript = (char*) realloc(ldscript, strlen(ldscript) + strlen(linebuf) + 1);
		strcat(ldscript, linebuf);
	};
	
	fclose(fp);
	
	// parse it
	char *error;
	Token *toklist = ldscript_tokenize(ldfile, ldscript, &error);
	if (toklist == NULL)
	{
		fprintf(stderr, "%s\n", error);
		return 1;
	};
	
	Token *outlist;
	LinkerScript *script = parse_LinkerScript(toklist, &outlist);
	if (script == NULL)
	{
		fprintf(stderr, "%s: %s: bad grammar\n", argv[0], ldfile);
		return 1;
	};
	
	// create the resulting object while parsing the script
	result = objNew();
	result->type = resultType;
	result->entry = "_start";
	
	StatementList *list;
	for (list=script->statements; list!=NULL; list=list->next)
	{
		Statement *st = list->statement;
		
		if (st->entry != NULL)
		{
			// 'entry' statement
			result->entry = strdup(st->entry->symbol->value);
		}
		else if (st->symAssign != NULL)
		{
			handleSymAssign(st->symAssign);
		}
		else if (st->newtype != NULL)
		{
			const char *oldname = st->newtype->oldtype->value;
			const char *newname = st->newtype->newtype->value;
			
			Appender appender = getAppenderByName(oldname);
			if (appender == NULL)
			{
				fprintf(stderr, "%s:%d: error: unknown type name `%s'\n",
					st->filename, st->lineno, oldname);
				errorsOccured = 1;
				continue;
			};
			
			if (getAppenderByName(newname) != NULL)
			{
				fprintf(stderr, "%s:%d: error: redefinition of type `%s'\n",
					st->filename, st->lineno, newname);
				errorsOccured = 1;
				continue;
			};
			
			LinkerType *type = (LinkerType*) malloc(sizeof(LinkerType));
			type->next = linkerTypes;
			linkerTypes = type;
			type->appender = appender;
			type->name = newname;
		}
		else if (st->secdef != NULL)
		{
			// output section definition
			const char *name = st->secdef->name->value;
			SectionType *type = st->secdef->type;
			const char *flags = st->secdef->flags->value;
			
			int sectype;
			if (type->progbits != NULL)
			{
				sectype = SECTYPE_PROGBITS;
			}
			else if (type->nobits != NULL)
			{
				sectype = SECTYPE_NOBITS;
			}
			else
			{
				fprintf(stderr, "%s: invalid section type in parse tree! internal bug!\n", argv[0]);
				abort();
			};
			
			int secflags = 0;
			for (; *flags!=0; flags++)
			{
				switch (*flags)
				{
				case 'r':
				case 'R':
					secflags |= SEC_READ;
					break;
				case 'w':
				case 'W':
					secflags |= SEC_WRITE;
					break;
				case 'x':
				case 'X':
					secflags |= SEC_EXEC;
					break;
				default:
					fprintf(stderr, "%s:%d: error: invalid section flag `%c'\n",
						st->filename, st->lineno, *flags);
					errorsOccured = 1;
					break;
				};
			};
			
			if (objGetSection(result, name) != NULL)
			{
				fprintf(stderr, "%s:%d: error: redefinition of section `%s'\n",
					st->filename, st->lineno, name);
				errorsOccured = 1;
			}
			else
			{
				currentSection = objCreateSection(result, name, sectype, secflags);
				currentSection->vaddr = currentSection->paddr = currentAddr;
				
				processSection(st->secdef->body);
				
				currentSection = NULL;
			};
		}
		else
		{
			fprintf(stderr, "%s: parse tree inconsistent! internal bug!\n", argv[0]);
			abort();
		};
	};
	
	// transfer over absolute symbols
	poolLoadAbsolute();
	
	// perform relocations
	Section *sect;
	for (sect=result->sections; sect!=NULL; sect=sect->next)
	{
		Reloc *reloc;
		for (reloc=sect->relocs; reloc!=NULL; reloc=reloc->next)
		{
			uint64_t relocPos = sect->vaddr + reloc->offset;
			Symbol *sym = getSymbolByName(reloc->symbol);
			if (sym == NULL)
			{
				fprintf(stderr, "%s: undefined reference to `%s'\n", argv[0], reloc->symbol);
				errorsOccured = 1;
				continue;
			};
			
			doReloc((char*) sect->data + reloc->offset, reloc->size, reloc->type, relocPos, sym->offset, reloc->addend);
		};
		
		sect->relocs = NULL;
	};
	
	if (!errorsOccured)
	{
		int errnum = objWrite(result, outfile);
		if (errnum != 0)
		{
			fprintf(stderr, "%s: cannot write final output file %s: %s\n", argv[0], outfile, strerror(errnum));
			return 1;
		};
		
		if (result->type == OBJTYPE_EXEC) markFileExecutable(outfile);
		return 0;
	}
	else
	{
		return 1;
	};
};
