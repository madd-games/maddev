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

#ifndef LDSCRIPT_H_
#define LDSCRIPT_H_

#include "liblex.h"
#include <string.h>

#define TERMINAL_SYMBOL(name) typedef struct {const char *filename; int lineno; int col; const char *value;} name;
#define	PARSER_DEF(name) name* parse_##name(Token *toklist, Token **outlist)

enum
{
	TOK_IDENTIFIER,
	TOK_SEMICOLON,
	TOK_OPEN_BRACKET,
	TOK_CLOSE_BRACKET,
	TOK_OPEN_BRACE,
	TOK_CLOSE_BRACE,
	TOK_ASSIGNMENT,
	TOK_COMMA,
	TOK_ADD_SUB,
	TOK_STRING,
	TOK_VALUE,
};

TERMINAL_SYMBOL(Identifier);
TERMINAL_SYMBOL(Semicolon);
TERMINAL_SYMBOL(OpenBracket);
TERMINAL_SYMBOL(CloseBracket);
TERMINAL_SYMBOL(OpenBrace);
TERMINAL_SYMBOL(CloseBrace);
TERMINAL_SYMBOL(AddSubOp);
TERMINAL_SYMBOL(String);
TERMINAL_SYMBOL(Value);

struct Expr_;

typedef struct
{
	const char*			filename;
	int				lineno;
	int				col;
	
	Value*				value;
	Identifier*			name;
	struct Expr_*			expr;
	struct Expr_*			align;
} PrimaryExpr;

typedef struct
{
	const char*			filename;
	int				lineno;
	int				col;
	
	AddSubOp*			op;
	PrimaryExpr*			arg;
} SumAffix;

typedef struct SumAffixList_
{
	const char*			filename;
	int				lineno;
	int				col;
	
	SumAffix*			affix;
	struct SumAffixList_*		next;
} SumAffixList;

typedef struct
{
	const char*			filename;
	int				lineno;
	int				col;
	
	PrimaryExpr*			primary;
	SumAffixList*			chain;
} SumExpr;

typedef struct Expr_
{
	const char*			filename;
	int				lineno;
	int				col;
	
	SumExpr*			sum;
} Expr;

typedef struct
{
	const char*			filename;
	int				lineno;
	int				col;
	
	Identifier*			symbol;
} EntryStatement;

typedef struct
{
	const char*			filename;
	int				lineno;
	int				col;
	
	Identifier*			symbol;
	Expr*				expr;
} SymbolAssignment;

typedef struct
{
	const char*			filename;
	int				lineno;
	int				col;
	
	Identifier*			name;
} LoadStatement;

typedef struct
{
	const char*			filename;
	int				lineno;
	int				col;
} Progbits;

typedef struct
{
	const char*			filename;
	int				lineno;
	int				col;
} Nobits;

typedef struct
{
	const char*			filename;
	int				lineno;
	int				col;
	
	Progbits*			progbits;
	Nobits*				nobits;
} SectionType;

typedef struct
{
	const char*			filename;
	int				lineno;
	int				col;
} MergeStatement;

typedef struct
{
	const char*			filename;
	int				lineno;
	int				col;
	
	SymbolAssignment*		symAssign;
	LoadStatement*			load;
	MergeStatement*			merge;
} SectionStatement;

typedef struct SectionStatementList_
{
	const char*			filename;
	int				lineno;
	int				col;
	
	SectionStatement*		statement;
	struct SectionStatementList_*	next;
} SectionStatementList;

typedef struct
{
	const char*			filename;
	int				lineno;
	int				col;
	
	Identifier*			name;
	SectionType*			type;
	Identifier*			flags;
	SectionStatementList*		body;
} SectionDefinition;

typedef struct
{
	const char*			filename;
	int				lineno;
	int				col;
	
	EntryStatement*			entry;
	SymbolAssignment*		symAssign;
	SectionDefinition*		secdef;
} Statement;

typedef struct StatementList_
{
	const char*			filename;
	int				lineno;
	int				col;
	
	Statement*			statement;
	struct StatementList_*		next;
} StatementList;

typedef struct
{
	const char*			filename;
	int				lineno;
	int				col;
	
	StatementList*			statements;
} LinkerScript;

Token* ldscript_tokenize(const char *filename, const char *data, char **errput);

PARSER_DEF(PrimaryExpr);
PARSER_DEF(SumAffix);
PARSER_DEF(SumAffixList);
PARSER_DEF(SumExpr);
PARSER_DEF(Expr);
PARSER_DEF(EntryStatement);
PARSER_DEF(Progbits);
PARSER_DEF(Nobits);
PARSER_DEF(SymbolAssignment);
PARSER_DEF(LoadStatement);
PARSER_DEF(SectionType);
PARSER_DEF(SectionDefinition);
PARSER_DEF(SectionStatementList);
PARSER_DEF(SectionStatement);
PARSER_DEF(Statement);
PARSER_DEF(StatementList);
PARSER_DEF(LinkerScript);

#endif
