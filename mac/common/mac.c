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

#include "mac.h"
#include "arch.h"

typedef struct
{
	const char *format;
	uint32_t type;
} RegFormat;

RegFormat macRegFormats[] = {
	{"%%ib%u%n",		REGTYPE_IB},
	{"%%ub%u%n",		REGTYPE_UB},
	{"%%iw%u%n",		REGTYPE_IW},
	{"%%uw%u%n",		REGTYPE_UW},
	{"%%id%u%n",		REGTYPE_ID},
	{"%%ud%u%n",		REGTYPE_UD},
	{"%%iq%u%n",		REGTYPE_IQ},
	{"%%uq%u%n",		REGTYPE_UQ},
	{"%%f%u%n",		REGTYPE_F},
	{"%%fd%u%n",		REGTYPE_FD},
	{NULL,			0}
};

/**
 * Macro names MUST correspond to identifiers in mac.h!
 */
const char *macNames[] = {
	"ret",
	"label",
	"mov",
	
	NULL,			/* terminator */
};

RegSpec macRegByName(const char *regname)
{
	int count;
	unsigned int index;
	RegFormat *scan;
	
	for (scan=macRegFormats; scan->format != NULL; scan++)
	{
		if (sscanf(regname, scan->format, &index, &count) > 0)
		{
			if (count == strlen(regname))
			{
				return REGSPEC_MAKE(scan->type, index);
			};
		};
	};
	
	return REGSPEC_INVALID;
};

int main(int argc, char *argv[])
{
	char linebuf[2048];
	char *line;
	int lineno = 0;
	
	Proc *currentProc = NULL;
	while ((line = fgets(linebuf, 2048, stdin)) != NULL)
	{
		lineno++;
		
		char *endline = strchr(line, '\n');
		if (endline == NULL)
		{
			fprintf(stderr, "%s:%d: line too long\n", argv[0], lineno);
			return 1;
		};
		
		*endline = 0;
		
		char *commentPos = strchr(line, '#');
		if (commentPos != NULL) *commentPos = 0;
		
		char *cmd = strtok(line, SPACE_CHARS);
		if (cmd == NULL) continue;
		
		if (strcmp(cmd, "flag") == 0)
		{
			char *flag = strtok(NULL, SPACE_CHARS);
			if (flag == NULL)
			{
				fprintf(stderr, "%s:%d: syntax for 'flag': flag <flag>\n", argv[0], lineno);
				return 1;
			};
			
			FlagSpec *spec;
			for (spec=ArchFlagSpecs; spec->flag!=NULL; spec++)
			{
				if (strcmp(spec->flag, flag) == 0)
				{
					break;
				};
			};
			
			if (spec == NULL)
			{
				fprintf(stderr, "%s:%d: invalid flag '%s'\n", argv[0], lineno, flag);
				return 1;
			}
			else
			{
				*(spec->store) = spec->value;
			};
		}
		else if (strcmp(cmd, "global") == 0)
		{
			char *symname = strtok(NULL, SPACE_CHARS);
			if (symname == NULL)
			{
				fprintf(stderr, "%s:%d: syntax for 'global': global <symname>\n", argv[0], lineno);
				return 1;
			};
			
			printf(".globl %s\n", symname);
		}
		else if (strcmp(cmd, "weak") == 0)
		{
			char *symname = strtok(NULL, SPACE_CHARS);
			if (symname == NULL)
			{
				fprintf(stderr, "%s:%d: syntax for 'weak': weak <symname>\n", argv[0], lineno);
				return 1;
			};
			
			printf(".weak %s\n", symname);
		}
		else if (strcmp(cmd, "section") == 0)
		{
			char *name = strtok(NULL, SPACE_CHARS);
			if (name == NULL)
			{
				fprintf(stderr, "%s:%d: syntax for 'section': section <name>\n", argv[0], lineno);
				return 1;
			};
			
			if (strcmp(name, ".text") == 0)
			{
				// just to make it prettier
				printf(".text\n");
			}
			else
			{
				printf("section %s\n", name);
			};
		}
		else if (strcmp(cmd, "byte") == 0)
		{
			char *val = strtok(NULL, SPACE_CHARS);
			if (val == NULL)
			{
				fprintf(stderr, "%s:%d: syntax for 'byte': byte <value>\n", argv[0], lineno);
				return 1;
			};
			
			printf("%s %s\n", ArchByteMnemonic, val);
		}
		else if (strcmp(cmd, "word") == 0)
		{
			char *val = strtok(NULL, SPACE_CHARS);
			if (val == NULL)
			{
				fprintf(stderr, "%s:%d: syntax for 'word': word <value>\n", argv[0], lineno);
				return 1;
			};
			
			printf("%s %s\n", ArchWordMnemonic, val);
		}
		else if (strcmp(cmd, "dword") == 0)
		{
			char *val = strtok(NULL, SPACE_CHARS);
			if (val == NULL)
			{
				fprintf(stderr, "%s:%d: syntax for 'dword': dword <value>\n", argv[0], lineno);
				return 1;
			};
			
			printf("%s %s\n", ArchDwordMnemonic, val);
		}
		else if (strcmp(cmd, "qword") == 0)
		{
			char *val = strtok(NULL, SPACE_CHARS);
			if (val == NULL)
			{
				fprintf(stderr, "%s:%d: syntax for 'qword': qword <value>\n", argv[0], lineno);
				return 1;
			};
			
			printf("%s %s\n", ArchQwordMnemonic, val);
		}
		else if (strcmp(cmd, "object") == 0)
		{
			char *name = strtok(NULL, SPACE_CHARS);
			if (name == NULL)
			{
				fprintf(stderr, "%s:%d: syntax for 'object': object <name> <size>\n", argv[0], lineno);
				return 1;
			};
			
			char *size = strtok(NULL, SPACE_CHARS);
			if (size == NULL)
			{
				fprintf(stderr, "%s:%d: syntax for 'object': object <name> <size>\n", argv[0], lineno);
				return 1;
			};
			
			printf(".type %s, @object\n", name);
			printf(".size %s, %s\n", name, size);
		}
		else if (strcmp(cmd, "align") == 0)
		{
			char *align = strtok(NULL, SPACE_CHARS);
			if (align == NULL)
			{
				fprintf(stderr, "%s:%d: syntax for 'align': align <nbytes>\n", argv[0], lineno);
				return 1;
			};
			
			printf(".align %s\n", align);
		}
		else if (strcmp(cmd, "proc") == 0)
		{
			if (currentProc != NULL)
			{
				fprintf(stderr, "%s:%d: nested 'proc'\n", argv[0], lineno);
				return 1;
			};
			
			char *name = strtok(NULL, SPACE_CHARS);
			if (name == NULL)
			{
				fprintf(stderr, "%s:%d: snytax for 'proc': proc <name> [-param=value...] [arg-regs...]\n",
						argv[0], lineno);
				return 1;
			};
			
			currentProc = (Proc*) malloc(sizeof(Proc));
			currentProc->name = strdup(name);
			currentProc->conv = strdup(MAC_ABI);
			currentProc->flags = 0;
			currentProc->first = NULL;
			currentProc->last = NULL;
			currentProc->argRegs = NULL;
			currentProc->numArgs = 0;
			currentProc->lineno = lineno;
			
			char *par;
			for (par=strtok(NULL, SPACE_CHARS); par!=NULL; par=strtok(NULL, SPACE_CHARS))
			{
				if (par[0] == '-')
				{
					if (memcmp(par, "-conv=", 6) == 0)
					{
						free(currentProc->conv);
						currentProc->conv = strdup(&par[6]);
					}
					else if (strcmp(par, "-var") == 0)
					{
						currentProc->flags |= PROC_VARIADIC;
					}
					else if (strcmp(par, "-leaf") == 0)
					{
						currentProc->flags |= PROC_LEAF;
					}
					else
					{
						fprintf(stderr, "%s:%d: unrecognised option '%s'\n", argv[0], lineno, par);
						return 1;
					};
				}
				else
				{
					RegSpec spec = macRegByName(par);
					if (spec == REGSPEC_INVALID)
					{
						fprintf(stderr, "%s:%d: invalid register name '%s'\n", argv[0], lineno, par);
						return 1;
					};
					
					size_t index = currentProc->numArgs++;
					currentProc->argRegs = (RegSpec*) realloc(currentProc->argRegs,
										sizeof(RegSpec)*currentProc->numArgs);
					currentProc->argRegs[index] = spec;
				};
			};
		}
		else if (strcmp(cmd, "endproc") == 0)
		{
			if (currentProc == NULL)
			{
				fprintf(stderr, "%s:%d: 'endproc' without matching 'proc'\n", argv[0], lineno);
				return 1;
			};
			
			ArchExpand(argv[0], currentProc);
			
			free(currentProc->name);
			free(currentProc->conv);
			free(currentProc->argRegs);
			
			while (currentProc->first != NULL)
			{
				Macro *macro = currentProc->first;
				currentProc->first = macro->next;
				free(macro);
			};
			
			free(currentProc);
			currentProc = NULL;
		}
		else if (cmd[strlen(cmd)-1] == ':')
		{
			if (currentProc == NULL)
			{
				printf("%s\n", cmd);
			}
			else
			{
				Macro *mac = (Macro*) malloc(sizeof(Macro));
				mac->next = NULL;
				mac->type = MAC_LABEL;
				mac->lineno = lineno;
				mac->ops = (Operand*) malloc(sizeof(Operand));
				mac->numOps = 1;
				
				if (currentProc->last == NULL)
				{
					currentProc->first = currentProc->last = mac;
				}
				else
				{
					currentProc->last->next = mac;
					currentProc->last = mac;
				};
				
				mac->ops[0].symbol.type = OPR_SYMBOL;
				cmd[strlen(cmd)-1] = 0;		// remove the ":"
				mac->ops[0].symbol.name = strdup(cmd);
			};
		}
		else
		{
			int mactype;
			for (mactype=0; macNames[mactype]!=NULL; mactype++)
			{
				if (strcmp(macNames[mactype], cmd) == 0)
				{
					break;
				};
			};
			
			if (macNames[mactype] == NULL)
			{
				fprintf(stderr, "%s:%d: invalid macro name '%s'\n", argv[0], lineno, cmd);
				return 1;
			};
			
			if (currentProc == NULL)
			{
				fprintf(stderr, "%s:%d: procedural macro '%s' used outside procedure\n", argv[0], lineno, cmd);
				return 1;
			};
			
			Macro *mac = (Macro*) malloc(sizeof(Macro));
			mac->next = NULL;
			mac->type = mactype;
			mac->lineno = lineno;
			mac->ops = NULL;
			mac->numOps = 0;
			
			if (currentProc->last == NULL)
			{
				currentProc->first = currentProc->last = mac;
			}
			else
			{
				currentProc->last->next = mac;
				currentProc->last = mac;
			};
			
			char *par;
			for (par=strtok(NULL, SPACE_CHARS); par!=NULL; par=strtok(NULL, SPACE_CHARS))
			{
				size_t index = mac->numOps++;
				mac->ops = realloc(mac->ops, sizeof(Operand)*mac->numOps);
				Operand *op = &mac->ops[index];
				
				if (par[0] == '%')
				{
					op->reg.type = OPR_REG;
					op->reg.spec = macRegByName(par);
					
					if (op->reg.spec == REGSPEC_INVALID)
					{
						fprintf(stderr, "%s:%d: invalid register name '%s'\n", argv[0], lineno, par);
						return 1;
					};
				}
				else if (par[0] == '$')
				{
					op->con.type = OPR_CONST;
					int sz;
					if (sscanf(par, "$%d%n", &op->con.value, &sz) < 1)
					{
						fprintf(stderr, "%s:%d: invalid constant '%s'\n", argv[0], lineno, par);
						return 1;
					};
					
					if ((size_t)sz != strlen(par))
					{
						fprintf(stderr, "%s:%d: invalid constant '%s'\n", argv[0], lineno, par);
						return 1;
					};
				}
				else
				{
					op->symbol.type = OPR_SYMBOL;
					op->symbol.name = strdup(par);
				};
			};
		};
	};
	
	if (currentProc != NULL)
	{
		fprintf(stderr, "%s:%d: unterminated 'proc'\n", argv[0], lineno);
		return 1;
	};
	
	return 0;
};
