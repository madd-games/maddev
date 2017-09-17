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

// bnf-tree.c
// test if we can generate a valid parse tree

char treeBuffer[2000];

void print_number(Number *num)
{
	strcat(treeBuffer, num->value);
};

void print_product(Product *prod)
{
	assert(prod->a != NULL);
	
	if (prod->chain == NULL)
	{
		print_number(prod->a);
	}
	else
	{
		strcat(treeBuffer, "(");
		print_number(prod->a);
		
		ProdAffixList *list;
		for (list=prod->chain; list!=NULL; list=list->next)
		{
			assert(list->affix != NULL);
			assert(list->affix->op != NULL);
			assert(list->affix->value != NULL);
			
			strcat(treeBuffer, " ");
			strcat(treeBuffer, list->affix->op->value);
			strcat(treeBuffer, " ");
			
			print_number(list->affix->value);
		};
		
		strcat(treeBuffer, ")");
	};
};

void print_sum(Sum *sum)
{
	assert(sum->a != NULL);
	
	if (sum->chain == NULL)
	{
		print_product(sum->a);
	}
	else
	{
		strcat(treeBuffer, "(");
		print_product(sum->a);
		
		SumAffixList *list;
		for (list=sum->chain; list!=NULL; list=list->next)
		{
			assert(list->affix != NULL);
			assert(list->affix->op != NULL);
			assert(list->affix->value != NULL);
			
			strcat(treeBuffer, " ");
			strcat(treeBuffer, list->affix->op->value);
			strcat(treeBuffer, " ");
			
			print_product(list->affix->value);
		};
		
		strcat(treeBuffer, ")");
	};
};

void print_expr(Expr *expr)
{
	assert(expr->sum != NULL);
	print_sum(expr->sum);
	strcat(treeBuffer, ";");
};

int main()
{
	char *error;
	Token *toklist = tokenize_expr("test", "5 + 6 * 7 + 4 - 2 * 5", &error);
	if (toklist == NULL)
	{
		fprintf(stderr, "tokenize_expr: %s\n", error);
		return 1;
	};
	
	Token *outlist;
	Expr *tree = parse_Expr(toklist, &outlist);
	
	assert(tree != NULL);
	toklist = (Token*) plNext((Pipeline*) outlist);
	assert(toklist == NULL);
	
	// print the tree to the buffer
	print_expr(tree);
	printf("Parse tree: %s\n", treeBuffer);
	
	const char *correctTree = "(5 + (6 * 7) + 4 - (2 * 5));";
	printf("Correct tree: %s\n", correctTree);
	
	assert(strcmp(treeBuffer, correctTree) == 0);
	printf("tree OK.\n");
	return 0;
};
