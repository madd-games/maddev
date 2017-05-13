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

int isSymbolChar(char c)
{
	if ((c >= 'a') && (c <= 'z')) return 1;
	if ((c >= 'A') && (c <= 'Z')) return 1;
	return c == '_';
};

int main(int argc, char *argv[])
{
	char *inputFile = NULL;
	char *outputFile = NULL;
	char *sectionName = NULL;
	char *symbolName = NULL;
	int sectionFlags = SEC_READ;
	
	int i;
	for (i=1; i<argc; i++)
	{
		if (argv[i][0] != '-')
		{
			if (inputFile == NULL)
			{
				inputFile = argv[i];
			}
			else
			{
				fprintf(stderr, "%s: expected only 1 input file\n", argv[0]);
				return 1;
			};
		}
		else if (strcmp(argv[i], "-o") == 0)
		{
			if (outputFile == NULL)
			{
				if (argv[i+1] != NULL)
				{
					outputFile = argv[++i];
				}
				else
				{
					fprintf(stderr, "%s: option '-o' expects parameter\n", argv[0]);
					return 1;
				};
			}
			else
			{
				fprintf(stderr, "%s: multiple output files specified\n", argv[0]);
				return 1;
			};
		}
		else if (strcmp(argv[i], "-S") == 0)
		{
			if (sectionName == NULL)
			{
				if (argv[i+1] != NULL)
				{
					sectionName = argv[++i];
				}
				else
				{
					fprintf(stderr, "%s: option '-S' expects parameter\n", argv[0]);
					return 1;
				};
			}
			else
			{
				fprintf(stderr, "%s: multiple section names specified\n", argv[0]);
				return 1;
			};
		}
		else if (strcmp(argv[i], "-s") == 0)
		{
			if (symbolName == NULL)
			{
				if (argv[i+1] != NULL)
				{
					symbolName = argv[++i];
				}
				else
				{
					fprintf(stderr, "%s: option '-s' expects parameter\n", argv[0]);
					return 1;
				};
			}
			else
			{
				fprintf(stderr, "%s: multiple symbol names specified\n", argv[0]);
				return 1;
			};
		}
		else if (strcmp(argv[i], "-w") == 0)
		{
			sectionFlags |= SEC_WRITE;
		}
		else if (strcmp(argv[i], "-x") == 0)
		{
			sectionFlags |= SEC_EXEC;
		}
		else
		{
			fprintf(stderr, "%s: unrecognised option '%s'\n", argv[0], argv[i]);
			return 1;
		};
	};
	
	if (inputFile == NULL)
	{
		fprintf(stderr, "%s: no input file specified\n", argv[0]);
		return 1;
	};
	
	if (outputFile == NULL)
	{
		outputFile = "a.out";
	};
	
	if (sectionName == NULL)
	{
		if (sectionFlags & SEC_WRITE)
		{
			sectionName = ".data";
		}
		else if (sectionFlags & SEC_EXEC)
		{
			sectionName = ".text";
		}
		else
		{
			sectionName = ".rodata";
		};
	};
	
	if (symbolName == NULL)
	{
		symbolName = strdup(inputFile);
		
		size_t i;
		for (i=0; i<strlen(symbolName); i++)
		{
			if (!isSymbolChar(symbolName[i]))
			{
				symbolName[i] = '_';
			};
		};
	};
	
	FILE *fp = fopen(inputFile, "r");
	if (fp == NULL)
	{
		fprintf(stderr, "%s: cannot open %s: %s\n", argv[0], inputFile, strerror(errno));
		return 1;
	};
	
	Object *obj = objNew();
	Section *sect = objCreateSection(obj, sectionName, SECTYPE_PROGBITS, sectionFlags);
	objSymbolBinding(obj, symbolName, SYMB_GLOBAL);
	objSymbolType(obj, symbolName, SYMT_OBJECT);
	objAddSymbol(obj, sect, symbolName);

	char buffer[2048];
	size_t totalSize = 0;
	while (1)
	{
		size_t sz = fread(buffer, 1, 2048, fp);
		if (sz == 0) break;
		objSectionAppend(sect, buffer, sz);
		totalSize += sz;
	};
	
	fclose(fp);
	
	objSymbolSize(obj, symbolName, totalSize);
	
	int errnum = objWrite(obj, outputFile);
	if (errnum != 0)
	{
		fprintf(stderr, "%s: cannot write object to %s: %s\n", argv[0], outputFile, strerror(errnum));
		return 1;
	};
	
	return 0;
};
