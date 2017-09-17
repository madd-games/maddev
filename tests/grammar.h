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

#ifndef GRAMMAR_H_
#define GRAMMAR_H_

// grammar.h
// include file for the grammar test (grammar.bnf)

#include "liblex.h"
#include <string.h>
#include <stdlib.h>

enum
{
	TOK_NUMBER,
	TOK_ADD_SUB,
	TOK_MUL_DIV
};

/**
 * "Number", the terminal symbol representing a number.
 */
typedef struct
{
	const char*			filename;
	int				lineno;
	int				col;
	char*				value;
} Number;

/**
 * Terminal symbol representing a multiply or divide sign.
 */
typedef struct
{
	const char*			filename;
	int				lineno;
	int				col;
	char*				value;
} MulDivOp;

/**
 * Terminal symbol representing a add or subtract sign.
 */
typedef struct
{
	const char*			filename;
	int				lineno;
	int				col;
	char*				value;
} AddSubOp;

/**
 * Represents a product affix, like "* x" or "/ x".
 */
typedef struct
{
	const char*			filename;
	int				lineno;
	int				col;
	
	MulDivOp*			op;
	Number*				value;
} ProdAffix;

/**
 * List of product affixes.
 */
typedef struct ProdAffixList_
{
	const char*			filename;
	int				lineno;
	int				col;
	
	ProdAffix*			affix;
	struct ProdAffixList_*		next;
} ProdAffixList;

/**
 * A product.
 */
typedef struct
{
	const char*			filename;
	int				lineno;
	int				col;
	
	Number*				a;
	ProdAffixList*			chain;
} Product;

/**
 * Represents a sum affix, like "+ x" or "- x".
 */
typedef struct
{
	const char*			filename;
	int				lineno;
	int				col;
	
	AddSubOp*			op;
	Product*			value;
} SumAffix;

/**
 * List of sum affixes.
 */
typedef struct SumAffixList_
{
	const char*			filename;
	int				lineno;
	int				col;
	
	SumAffix*			affix;
	struct SumAffixList_*		next;
} SumAffixList;

/**
 * A sum.
 */
typedef struct
{
	const char*			filename;
	int				lineno;
	int				col;
	
	Product*			a;
	SumAffixList*			chain;
} Sum;

/**
 * Expression.
 */
typedef struct
{
	const char*			filename;
	int				lineno;
	int				col;
	
	Sum*				sum;
} Expr;

Token* tokenize_expr(const char *filename, const char *str, char **errput);

Number* parse_Number(Token *toklist, Token **outlist);
MulDivOp* parse_MulDivOp(Token *toklist, Token **outlist);
AddSubOp* parse_AddSubOp(Token *toklist, Token **outlist);
ProdAffix* parse_ProdAffix(Token *toklist, Token **outlist);
ProdAffixList* parse_ProdAffixList(Token *toklist, Token **outlit);
Product* parse_Product(Token *toklist, Token **outlist);
SumAffix* parse_SumAffix(Token *toklist, Token **outlist);
SumAffixList* parse_SumAffixList(Token *toklist, Token **outlist);
Sum* parse_Sum(Token *toklist, Token **outlist);
Expr* parse_Expr(Token *toklist, Token **outlist);

#endif
