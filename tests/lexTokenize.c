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

/**
 * UNIT TEST
 * Component: liblex
 * Element: lexTokenize
 */

#include "liblex.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

enum
{
	WORD,
	NUMBER,
};

TokenSpec tokenSpecs[] = {
	{WORD, "[a-zA-Z]+"},
	{NUMBER, "[1-9][0-9]*"},
	{-1, "$+"},
	{0, NULL}
};

int main()
{
	int compileResult = lexCompileTokenSpecs(tokenSpecs);
	assert(compileResult == 0);
	
	Token *list = (Token*) plCreate(sizeof(Token));
	char *error = lexTokenize(list, "testfile", "nice \n  567 test", tokenSpecs);
	assert(error == NULL);
	
	Token *tok = (Token*) plNext((Pipeline*)list);
	assert(tok != NULL);
	assert(strcmp(tok->filename, "testfile") == 0);
	assert(tok->type == WORD);
	assert(strcmp(tok->value, "nice") == 0);
	assert(tok->lineno == 1);
	assert(tok->col == 0);
	
	tok = (Token*) plNext((Pipeline*)tok);
	assert(tok != NULL);
	assert(strcmp(tok->filename, "testfile") == 0);
	assert(tok->type == NUMBER);
	assert(strcmp(tok->value, "567") == 0);
	assert(tok->lineno == 2);
	assert(tok->col == 2);

	tok = (Token*) plNext((Pipeline*)tok);
	assert(tok != NULL);
	assert(strcmp(tok->filename, "testfile") == 0);
	assert(tok->type == WORD);
	assert(strcmp(tok->value, "test") == 0);
	assert(tok->lineno == 2);
	assert(tok->col == 6);

	return 0;
};
