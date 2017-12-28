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

#ifndef LIBLEX_H_
#define LIBLEX_H_

#include <stdlib.h>
#include <stddef.h>
#include <inttypes.h>

#include "libcommon.h"

/**
 * Matcher flags (must start from bit 8).
 */
#define	LEX_MATCH_NOGREED			(1 << 8)		/* '?' attached */
#define	LEX_MATCH_ASTERISK			(1 << 9)		/* '*' attached */
#define	LEX_MATCH_PLUS				(1 << 10)		/* '+' attached */

/**
 * Matcher types (bottom 8 bits of 'mode').
 */
#define	LEX_MATCH_EXACT				0
#define	LEX_MATCH_SET				1
#define	LEX_MATCH_BRACKET			2

/**
 * Describes an element of a regular expression.
 */
struct Regex_;
typedef union
{
	/**
	 * Type in bottom 8 bits, and then flags (such as LEX_MATCH_NOGREED, LEX_MATCH_ASTERISK, etc).
	 */
	int					mode;
	
	struct
	{
		int				mode;			/* & 0xFF == LEX_MATCH_EXACT */
		char				c;			/* character to match */
	} exact;
	
	struct
	{
		int				mode;			/* & 0xFF == LEX_MATCH_SET */
		unsigned char			bitmap[32];		/* bitmap of bytes that should match */
	} set;
	
	struct
	{
		int				mode;			/* & 0xFF == LEX_MATCH_BRACKET */
		struct Regex_*			first;
	} bracket;
} Matcher;

/**
 * Describes a compiled expression.
 */
typedef struct Regex_
{
	/**
	 * List of matchers.
	 */
	Matcher*				matcherList;
	size_t					numMatchers;
	
	/**
	 * The regex below this one, in a stack. Used internally by lexCompileRegex(), and the value
	 * must be considered meaningless and unsafe to use afterwards!
	 */
	struct Regex_*				down;
	
	/**
	 * Next regex; used to link chains of options in brackets.
	 */
	struct Regex_*				next;
} Regex;

/**
 * Token type specification; specifies what regular expression defines a token of a specific type.
 * If the type is -1, tokens matching this expression are removed (they are considered whitespace).
 * All other types are arbitrary values. 'expr' set to NULL indicates end of the list.
 * An array of those is passed to lexTokenize().
 */
typedef struct
{
	int					type;
	const char*				expr;
	
	/**
	 * For use internally by lexCompileTokenSpecs().
	 */
	Regex*					regex;
} TokenSpec;

/**
 * Represents a token extracted from source.
 */
typedef struct
{
	Pipeline				tag;
	
	/**
	 * The source file name (derived from the 'filename' argument to lexTokenize()).
	 */
	const char*				filename;
	
	/**
	 * Token type.
	 */
	int					type;
	
	/**
	 * The string contained in the token.
	 */
	char*					value;
	
	/**
	 * Line number and column of origin.
	 */
	int					lineno;
	int					col;
} Token;

/**
 * Compile a regular expression and return the description, which may later be used to match.
 * Returns NULL if the expression is invalid.
 */
Regex* lexCompileRegex(const char *spec);

/**
 * Delete a compiled expression.
 */
void lexDeleteRegex(Regex *regex);

/**
 * Match a compiled regular expression against a string. Returns -1 if there is no match; otherwise,
 * returns the number of characters matched (which may be 0).
 */
ssize_t lexMatch(Regex *regex, const char *str);

/**
 * Compile token specifications. Returns 0 on success, -1 on error.
 */
int lexCompileTokenSpecs(TokenSpec *specs);

/**
 * Tokenize the input string, specifying the given filename in output tokens. Returns NULL on success,
 * or an error string (allocated on the heap) on error. The tokens are appended to the specified queue.
 */
char* lexTokenize(Token *out, const char *filename, const char *input, TokenSpec *specs);

/**
 * Parse a string token (C-style), and store in the given buffer. The buffer must have a size at least equal
 * to the token. Returns 0 on success, or a character indicating a broken escape sequence, or -1 if the string
 * is invalid (no quotes).
 */
char lexParseString(const char *token, char *buffer);

#endif
