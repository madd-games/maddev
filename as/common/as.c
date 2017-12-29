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
#include <assert.h>
#include <stdarg.h>

#include "asfamily.h"
#include "libobj.h"

int asStatus = 0;
Object *asObject;
Section *asCurrentSection;

void asSwitchSection(const char *name, int sectype, int flags)
{
	Section *sect = objGetSection(asObject, name);
	if (sect == NULL)
	{
		sect = objCreateSection(asObject, name, sectype, flags);
	};
	
	asCurrentSection = sect;
};

void asProcessDirective(const char *filename, int lineno, char *dir)
{
	char *cmd = strtok(dir, " \t\n");
	if (cmd == NULL)
	{
		return;
	}
	else if (strcmp(cmd, ".globl") == 0)
	{
		char *symname = strtok(NULL, " \t\n");
		if (symname == NULL)
		{
			asDiag(filename, lineno, ML_ERROR, "'.globl' directive expects a parameter\n");
			return;
		};
		
		objSymbolBinding(asObject, symname, SYMB_GLOBAL);
	}
	else if (strcmp(cmd, ".weak") == 0)
	{
		char *symname = strtok(NULL, " \t\n");
		if (symname == NULL)
		{
			asDiag(filename, lineno, ML_ERROR, "'.weak' directive expects a parameter\n");
			return;
		};
		
		objSymbolBinding(asObject, symname, SYMB_WEAK);
	}
	else if (strcmp(cmd, ".comm") == 0)
	{
		char *name = strtok(NULL, " \t\n");
		char *alignstr = strtok(NULL, " \t\n");
		char *sizestr = strtok(NULL, " \t\n");
		
		if (name == NULL || alignstr == NULL || sizestr == NULL)
		{
			asDiag(filename, lineno, ML_ERROR, "'.comm' directive expects a parameter\n");
			return;
		};
		
		char *endptr;
		unsigned long align = strtoul(alignstr, &endptr, 0);
		if (*endptr != 0)
		{
			asDiag(filename, lineno, ML_ERROR, "the second argument to '.comm' must be an integer\n");
			return;
		};
		
		unsigned long size = strtoul(sizestr, &endptr, 0);
		if (*endptr != 0)
		{
			asDiag(filename, lineno, ML_ERROR, "the third argument to '.comm' must be an integer\n");
			return;
		};
		
		objAbsoluteSymbol(asObject, name, SYMOFF_COMMON, align, size);
	}
	else if (strcmp(cmd, ".section") == 0)
	{
		char *name = strtok(NULL, " \t\n");
		if (name ==  NULL)
		{
			asDiag(filename, lineno, ML_ERROR, "'.section' requires a name\n");
			return;
		};
		
		int sectype = -1;
		int flags = -1;
		
		char *typename = strtok(NULL, " \t\n");
		if (typename != NULL)
		{
			if (strcmp(typename, "progbits") == 0)
			{
				sectype = SECTYPE_PROGBITS;
			}
			else if (strcmp(typename, "nobits") == 0)
			{
				sectype = SECTYPE_NOBITS;
			}
			else
			{
				asDiag(filename, lineno, ML_ERROR, "invalid section type '%s' (expected 'progbits' or 'nobits')\n", typename);
				return;
			};
			
			char *flagspec = strtok(NULL, " \t\n");
			if (flagspec != NULL)
			{
				flags = 0;
				
				for (; *flagspec!=0; flagspec++)
				{
					switch (*flagspec)
					{
					case 'r':
						flags |= SEC_READ;
						break;
					case 'w':
						flags |= SEC_WRITE;
						break;
					case 'x':
						flags |= SEC_EXEC;
						break;
					default:
						asDiag(filename, lineno, ML_ERROR, "unrecognised flag '%c'\n", *flagspec);
						return;
					};
				};
			};
		};
		
		if (sectype == -1)
		{
			if (strcmp(name, ".bss") == 0)
			{
				sectype = SECTYPE_NOBITS;
			}
			else
			{
				sectype = SECTYPE_PROGBITS;
			};
		};
		
		if (flags == -1)
		{
			if (strcmp(name, ".text") == 0)
			{
				flags = SEC_READ | SEC_EXEC;
			}
			else if (strcmp(name, ".init") == 0)
			{
				flags = SEC_READ | SEC_EXEC;
			}
			else if (strcmp(name, ".fini") == 0)
			{
				flags = SEC_READ | SEC_EXEC;
			}
			else if (strcmp(name, ".rodata") == 0)
			{
				flags = SEC_READ;
			}
			else
			{
				flags = SEC_READ | SEC_WRITE;
			};
		};
		
		asSwitchSection(name, sectype, flags);
	}
	else if (strcmp(cmd, ".text") == 0)
	{
		asSwitchSection(".text", SECTYPE_PROGBITS, SEC_READ | SEC_EXEC);
	}
	else if (strcmp(cmd, ".string") == 0)
	{
		char *token = strtok(NULL, "");
		if (token ==  NULL)
		{
			asDiag(filename, lineno, ML_ERROR, "'.string' requires a parameter\n");
			return;
		};
		
		char *buffer = (char*) malloc(strlen(token)+1);
		char result = lexParseString(token, buffer);
		if (result == -1)
		{
			asDiag(filename, lineno, ML_ERROR, "invalid string token: %s\n", token);
		}
		else if (result != 0)
		{
			asDiag(filename, lineno, ML_ERROR, "invalid escape sequence: \\%c\n", result);
		}
		else
		{
			objSectionAppend(asGetSection(), buffer, strlen(buffer)+1);
		};
		
		free(buffer);
	}
	else
	{
		asDiag(filename, lineno, ML_ERROR, "invalid assembler directive '%s'\n", cmd);
	};
};

Section* asGetSection()
{
	if (asCurrentSection == NULL)
	{
		asCurrentSection = objCreateSection(asObject, ".text", SECTYPE_PROGBITS, SEC_READ | SEC_EXEC);
	};
	
	return asCurrentSection;
};

int main(int argc, char *argv[])
{
	const char *outputFile = NULL;
	const char *inputFile = NULL;
	
	int i;
	for (i=1; i<argc; i++)
	{
		if (argv[i][0] != '-')
		{
			if (inputFile != NULL)
			{
				fprintf(stderr, "%s: multiple input files specified\n", argv[0]);
				return 1;
			};
			
			inputFile = argv[i];
		}
		else if (strcmp(argv[i], "-o") == 0)
		{
			if (outputFile != NULL)
			{
				fprintf(stderr, "%s: multiply output files specified\n", argv[0]);
				return 1;
			};
			
			if (i == argc-1)
			{
				fprintf(stderr, "%s: -o option expects a parameter\n", argv[0]);
				return 1;
			};
			
			i++;
			outputFile = argv[i];
		}
		else
		{
			fprintf(stderr, "%s: unrecognised command-line option: %s\n", argv[0], argv[i]);
			return 1;
		};
	};
	
	if (outputFile == NULL)
	{
		outputFile = "a.out";
	};
	
	FILE *input;
	if (inputFile == NULL)
	{
		input = stdin;
		inputFile = "<stdin>";
	}
	else
	{
		input = fopen(inputFile, "r");
		if (input == NULL)
		{
			fprintf(stderr, "%s: cannot open input file %s: %s\n", argv[0], inputFile, strerror(errno));
			return 1;
		};
	};
	
	asObject = objNew();
	
	char linebuf[1024];
	char *line;
	int lineno = 0;
	
	while ((line = fgets(linebuf, 1024, input)) != NULL)
	{
		lineno++;
		
		char *newline = strchr(line, '\n');
		if (newline != NULL) *newline = 0;
		newline = strchr(line, '\r');
		if (newline != NULL) *newline = 0;
		
		while ((strchr(" \t", *line) != NULL) && (*line != 0)) line++;
		
		if (line[0] == 0)
		{
			continue;
		};
		
		char *compos = strstr(line, "//");
		if (compos != NULL) *compos = 0;
		
		compos = strstr(line, ASFAMILY_COMMENT_PREFIX);
		if (compos != NULL) *compos = 0;
		
		if (line[0] == '.')
		{
			asProcessDirective(inputFile, lineno, line);
		}
		else if (line[strlen(line)-1] == ':')
		{
			line[strlen(line)-1] = 0;
			objAddSymbol(asObject, asGetSection(), line);
		}
		else
		{
			FamilyAssemble(inputFile, lineno, line);
		};
	};
	
	fclose(input);
	
	if (asStatus == 0)
	{
		int errnum = objWrite(asObject, outputFile);
		if (errnum != 0)
		{
			fprintf(stderr, "%s: cannot write object file %s: %s\n", argv[0], outputFile, strerror(errnum));
			return 1;
		};
	};
	
	return asStatus;
};

void asDiag(const char *filename, int lineno, int level, const char *format, ...)
{
	static const char *diagNames[] = {"note", "warning", "error"};
	
	va_list ap;
	va_start(ap, format);
	
	fprintf(stderr, "%s:%d: %s: ", filename, lineno, diagNames[level]);
	vfprintf(stderr, format, ap);
	
	va_end(ap);
	
	if (level >= ML_ERROR)
	{
		asStatus = 1;
	};
};
