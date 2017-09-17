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
#include <assert.h>

#include "liblex.h"

enum
{
	TOK_SYMBOL,
	TOK_DIR,
	TOK_LITERAL,
	TOK_OP
};

TokenSpec tokenSpecs[] = {
	// first comments and whitespaces
	{-1,			"$*"},
	{-1,			"\\/\\/.*?\n?"},
	{-1,			"\\/\\*.*?(\\*\\/)"},

	// now actual tokens
	{TOK_SYMBOL,		"\\<([-a-zA-Z0-9]|$)+\\>"},
	{TOK_DIR,		"[_a-zA-Z][_a-zA-Z0-9]*"},
	{TOK_LITERAL,		"\\'.*?\\'"},
	{TOK_OP,		"(\\;|\\:\\:\\=|\\|)"},
	
	// terminator
	{0, NULL}
};

void errormsg(Token *tok, const char *msg)
{
	if (tok == NULL)
	{
		fprintf(stderr, "<EOF>: %s\n", msg);
	}
	else
	{
		fprintf(stderr, "%s:%d: error: %s\n", tok->filename, tok->lineno, msg);
	};
};

void parseSym(char *put, const char *spec)
{
	for (spec++; *spec!='>'; spec++)
	{
		if (*spec == '-')
		{
			*put++ = '_';
		}
		else
		{
			*put++ = *spec;
		};
	};
	
	*put = 0;
};

void parseLiteral(char *put, const char *lit, const char *brackets)
{
	strcpy(put, lit);
	put[0] = brackets[0];
	put[strlen(put)-1] = brackets[1];
};

int main(int argc, char *argv[])
{
	// read the grammar into memory
	char buffer[2048];
	char *data = (char*) malloc(1);
	size_t sz = 0;
	size_t nextSize;
	
	while ((nextSize = fread(buffer, 1, 2048, stdin)) != 0)
	{
		data = (char*) realloc(data, sz + nextSize + 1);
		memcpy(&data[sz], buffer, nextSize);
		sz += nextSize;
	};
	
	// tokenize
	int errnum = lexCompileTokenSpecs(tokenSpecs);
	assert(errnum == 0);
	Token *toklist = (Token*) plCreate(sizeof(Token));
	char *error = lexTokenize(toklist, argv[0], data, tokenSpecs);
	if (error != NULL)
	{
		fprintf(stderr, "%s\n", error);
		return 1;
	};
	
	// compile it all
	printf("#include <assert.h>\n");
	printf("#include \"liblex.h\"\n");
	printf("typedef struct { int dummy; } EOF;\n");
	printf("static EOF* parse_EOF(Token *toklist, Token **outlist)\n");
	printf("{\n");
	printf("\tToken *eofToken = (Token*) plNext((Pipeline*) toklist);\n");
	printf("\tif (eofToken == NULL)\n");
	printf("\t{\n");
	printf("\t\t*outlist = toklist;\n");
	printf("\t\treturn (EOF*) malloc(sizeof(EOF));\n");
	printf("\t};\n");
	printf("\treturn NULL;\n");
	printf("};\n");
	
	while (toklist != NULL)
	{
		toklist = (Token*) plNext((Pipeline*) toklist);
		if (toklist == NULL) break;
		
		if (strcmp(toklist->value, "terminal") == 0)
		{
			toklist = (Token*) plNext((Pipeline*) toklist);
			if (toklist == NULL)
			{
				errormsg(NULL, "expected a terminal symbol name");
				return 1;
			};
			
			if (toklist->type != TOK_SYMBOL)
			{
				errormsg(toklist, "expected a terminal symbol name");
				return 1;
			};
			
			char symname[128];
			parseSym(symname, toklist->value);
			
			toklist = (Token*) plNext((Pipeline*) toklist);
			if (toklist == NULL)
			{
				errormsg(NULL, "expected a token type name");
				return 1;
			};
			
			if (toklist->type != TOK_DIR)
			{
				errormsg(toklist, "expected a token type name");
				return 1;
			};
			
			printf("%s* parse_%s(Token *toklist, Token **outlist)\n", symname, symname);
			printf("{\n");
			printf("\ttoklist = (Token*) plNext((Pipeline*) toklist);\n");
			printf("\tif (toklist == NULL) return NULL;\n");
			printf("\tif (toklist->type != %s) return NULL;\n", toklist->value);
			printf("\t%s *result = (%s*) malloc(sizeof(%s));\n", symname, symname, symname);
			printf("\tresult->value = toklist->value;\n");
			printf("\tresult->filename = toklist->filename;\n");
			printf("\tresult->lineno = toklist->lineno;\n");
			printf("\tresult->col = toklist->col;\n");
			printf("\t*outlist = toklist;\n");
			printf("\treturn result;\n");
			printf("};\n\n");
			
			toklist = (Token*) plNext((Pipeline*) toklist);
			if (toklist == NULL)
			{
				errormsg(NULL, "expected ';'");
				return 1;
			};
			
			if (strcmp(toklist->value, ";") != 0)
			{
				errormsg(toklist, "expected ';'");
				return 1;
			};
		}
		else if (strcmp(toklist->value, "include") == 0)
		{
			toklist = (Token*) plNext((Pipeline*) toklist);
			if (toklist == NULL)
			{
				errormsg(NULL, "expected an include file name");
				return 1;
			};
			
			if (toklist->type != TOK_LITERAL)
			{
				errormsg(toklist, "expected an include file name");
				return 1;
			};
			
			char spec[128];
			parseLiteral(spec, toklist->value, "\"\"");
			
			printf("#include %s\n", spec);
			
			toklist = (Token*) plNext((Pipeline*) toklist);
			if (toklist == NULL)
			{
				errormsg(NULL, "expected ';'");
				return 1;
			};
			
			if (strcmp(toklist->value, ";") != 0)
			{
				errormsg(toklist, "expected ';'");
				return 1;
			};
		}
		else if (toklist->type == TOK_SYMBOL)
		{
			char prodname[128];
			parseSym(prodname, toklist->value);
			
			toklist = (Token*) plNext((Pipeline*) toklist);
			if (toklist == NULL)
			{
				errormsg(NULL, "expected '::='");
				return 1;
			};
			
			if (strcmp(toklist->value, "::=") != 0)
			{
				errormsg(toklist, "expected '::='");
				return 1;
			};
			
			int i;
			for (i=0;;i++)
			{
				printf("static %s* parse_%s_%d(Token *toklist, Token **outlist)\n", prodname, prodname, i);
				printf("{\n");
				printf("\t%s* result = (%s*) malloc(sizeof(%s));\n", prodname, prodname, prodname);
				printf("\tmemset(result, 0, sizeof(%s));\n\n", prodname);
				
				uint64_t matUseBitmap = 0;
				int j;
				for (j=0;;j++)
				{
					toklist = (Token*) plNext((Pipeline*) toklist);
					if (toklist == NULL)
					{
						errormsg(NULL, "expected symbol or '|' or ';'");
						return 1;
					};
					
					if (strcmp(toklist->value, "|") == 0)
					{
						break;
					}
					else if (strcmp(toklist->value, ";") == 0)
					{
						break;
					}
					else if (toklist->type == TOK_SYMBOL)
					{
						char symtype[128];
						parseSym(symtype, toklist->value);
						
						char *refname;
						char *spacePos = strchr(symtype, ' ');
						if (spacePos == NULL)
						{
							refname = NULL;
						}
						else
						{
							*spacePos = 0;
							refname = spacePos+1;
						};
						
						printf("\t%s *mat%d = parse_%s(toklist, outlist);\n", symtype, j, symtype);
						printf("\tif (mat%d == NULL)\n", j);
						printf("\t{\n");
						
						int k;
						for (k=0; k<j; k++)
						{
							printf("\t\tfree(mat%d);\n", k);
						};
						
						printf("\t\tfree(result);\n");
						printf("\t\treturn NULL;\n");
						printf("\t};\n");
						if (refname != NULL)
						{
							matUseBitmap |= (1UL << j);
							printf("\tresult->%s = mat%d;\n", refname, j);
						};
						printf("\ttoklist = *outlist;\n\n");
						if (j == 0)
						{
							printf("\tresult->filename = toklist->filename;\n");
							printf("\tresult->lineno = toklist->lineno;\n");
							printf("\tresult->col = toklist->col;\n");
						}; 
					}
					else if (toklist->type == TOK_LITERAL)
					{
						char litstr[128];
						parseLiteral(litstr, toklist->value, "\"\"");
						
						printf("\tvoid *mat%d = NULL;\n", j);
						printf("\ttoklist = (Token*) plNext((Pipeline*) toklist);\n");
						printf("\tif (toklist == NULL)\n");
						printf("\t{\n");
						
						int k;
						for (k=0; k<j; k++)
						{
							printf("\t\tfree(mat%d);\n", k);
						};
						
						printf("\t\tfree(result);\n");
						printf("\t\treturn NULL;\n");
						printf("\t};\n");
						
						printf("\tif (strcmp(toklist->value, %s) != 0)\n", litstr);
						printf("\t{\n");

						for (k=0; k<j; k++)
						{
							printf("\t\tfree(mat%d);\n", k);
						};
						
						printf("\t\tfree(result);\n");
						printf("\t\treturn NULL;\n");
						printf("\t};\n");

						if (j == 0)
						{
							printf("\tresult->filename = toklist->filename;\n");
							printf("\tresult->lineno = toklist->lineno;\n");
							printf("\tresult->col = toklist->col;\n");
						}; 
					}
					else
					{
						errormsg(toklist, "expected symbol or '|' or ';'");
						return 1;
					};
				};
				
				int numMats = j;
				for (j=0; j<numMats; j++)
				{
					if ((matUseBitmap & (1UL << j)) == 0)
					{
						printf("\tfree(mat%d);\n", j);
					};
				};
				
				printf("\t*outlist = toklist;\n");
				printf("\treturn result;\n");
				printf("};\n\n");
				
				if (strcmp(toklist->value, ";") == 0)
				{
					break;
				};
			};
			
			int numAlts = i+1;
			printf("%s* parse_%s(Token *toklist, Token **outlist)\n", prodname, prodname);
			printf("{\n");
			printf("\t%s *candidate;\n\n", prodname);
			
			for (i=0; i<numAlts; i++)
			{
				printf("\tcandidate = parse_%s_%d(toklist, outlist);\n", prodname, i);
				printf("\tif (candidate != NULL) return candidate;\n\n");
			};
			
			printf("\treturn NULL;\n");
			printf("};\n\n");
		}
		else if (strcmp(toklist->value, "tokenizer") == 0)
		{
			toklist = (Token*) plNext((Pipeline*) toklist);
			if (toklist == NULL)
			{
				errormsg(NULL, "expected tokenizer name");
				return 1;
			};
			
			if (toklist->type != TOK_DIR)
			{
				errormsg(toklist, "expected tokenizer name");
				return 1;
			};
			
			char *tokerName = toklist->value;
			
			printf("static TokenSpec %s_specs[] = {\n", tokerName);
			
			while (1)
			{
				toklist = (Token*) plNext((Pipeline*) toklist);
				if (toklist == NULL)
				{
					errormsg(NULL, "expected token definition");
					return 1;
				};
				
				if (strcmp(toklist->value, ";") == 0) break;
				
				char *typename = "-1";
				if (toklist->type == TOK_DIR)
				{
					typename = toklist->value;

					toklist = (Token*) plNext((Pipeline*) toklist);
					if (toklist == NULL)
					{
						errormsg(NULL, "expected regular expression literal");
						return 1;
					};
				};
				
				if (toklist->type != TOK_LITERAL)
				{
					errormsg(toklist, "expected regular expression literal");
					return 1;
				};
				
				char expr[128];
				parseLiteral(expr, toklist->value, "\"\"");
				
				printf("\t{%s, %s},\n", typename, expr);
			};
			
			printf("\t{0, NULL}\n");
			printf("};\n\n");
			
			printf("Token* %s(const char *filename, const char *str, char **errput)\n", tokerName);
			printf("{\n");
			printf("\tint errnum = lexCompileTokenSpecs(%s_specs);\n", tokerName);
			printf("\tassert(errnum == 0);\n");
			printf("\tToken *toklist = (Token*) plCreate(sizeof(Token));\n");
			printf("\tchar *error = lexTokenize(toklist, filename, str, %s_specs);\n", tokerName);
			printf("\tif (error != NULL)\n");
			printf("\t{\n");
			printf("\t\tif (errput != NULL) *errput = error;\n");
			printf("\t\treturn NULL;\n");
			printf("\t};\n");
			printf("\treturn toklist;\n");
			printf("};\n\n");
		}
		else
		{
			errormsg(toklist, "unexpected token");
			return 1;
		};
	};
	
	return 0;
};
