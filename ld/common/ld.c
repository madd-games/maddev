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

SymEval evalSum(SumExpr *sum);

/**
 * Get a symbol by name. Returns NULL if not found. The returned symbol is in the final
 * output object, not within input objects (so it must be laready placed).
 */
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
			
			if (currentSection != NULL) eval.offset -= currentSection->vaddr;
			
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

int main(int argc, char *argv[])
{
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
			// symbol assignment
			const char *name = st->symAssign->symbol->value;
			SymEval eval = evalSum(st->symAssign->expr->sum);
			
			if (strcmp(name, ".") == 0)
			{
				if (currentSection != NULL)
				{
					fprintf(stderr, "%s:%d: error: attempting to move place pointer (.) inside a section\n",
							st->filename, st->lineno);
					errorsOccured = 1;
				}
				else
				{
					if (eval.sect != NULL)
					{
						fprintf(stderr, "%s:%d: error: attempting to move place pointer (.)"
								" to section-relative address\n",
								st->filename, st->lineno);
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
							st->filename, st->lineno, name);
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
		}
		else
		{
			fprintf(stderr, "%s: parse tree inconsistent! internal bug!\n", argv[0]);
			abort();
		};
	};
	
	if (!errorsOccured)
	{
		int errnum = objWrite(result, outfile);
		if (errnum != 0)
		{
			fprintf(stderr, "%s: cannot write final output file %s: %s\n", argv[0], outfile, strerror(errnum));
			return 1;
		};
		
		return 0;
	}
	else
	{
		return 1;
	};
};
