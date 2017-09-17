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

#include "grammar.h"
#include <assert.h>
#include <stdio.h>

// bnf-tokenize.c
// test if we can tokenize a math expression

#define	EXPECT_TOKEN(typ, val) toklist = (Token*) plNext((Pipeline*) toklist); assert(toklist != NULL); assert(toklist->type == typ); assert(strcmp(toklist->value, val) == 0);
#define EXPECT_END toklist = (Token*) plNext((Pipeline*) toklist); assert(toklist == NULL);

int main()
{
	char *error;
	Token *toklist = tokenize_expr("test", "5 + 6 * 7 + 4 - 2 * 5", &error);
	if (toklist == NULL)
	{
		fprintf(stderr, "tokenize_expr: %s\n", error);
		return 1;
	};
	
	EXPECT_TOKEN(TOK_NUMBER,  "5");
	EXPECT_TOKEN(TOK_ADD_SUB, "+");
	EXPECT_TOKEN(TOK_NUMBER,  "6");
	EXPECT_TOKEN(TOK_MUL_DIV, "*");
	EXPECT_TOKEN(TOK_NUMBER,  "7");
	EXPECT_TOKEN(TOK_ADD_SUB, "+");
	EXPECT_TOKEN(TOK_NUMBER,  "4");
	EXPECT_TOKEN(TOK_ADD_SUB, "-");
	EXPECT_TOKEN(TOK_NUMBER,  "2");
	EXPECT_TOKEN(TOK_MUL_DIV, "*");
	EXPECT_TOKEN(TOK_NUMBER,  "5");
	EXPECT_END;
	
	printf("tokenize OK.\n");
	return 0;
};
