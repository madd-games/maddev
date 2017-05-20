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
 * Element: lexCompileRegex
 */

#include "liblex.h"
#include <assert.h>

#define ASSERT_VALID_REGEX(expr) regex = lexCompileRegex(expr); assert(regex != NULL)
#define ASSERT_INVALID_REGEX(expr) regex = lexCompileRegex(expr); assert(regex == NULL)

int main()
{
	Regex *regex;
	
	// test valid expressions
	ASSERT_VALID_REGEX("hello");
	ASSERT_VALID_REGEX("he\\llo");
	ASSERT_VALID_REGEX("h+e");
	ASSERT_VALID_REGEX("h*e");
	ASSERT_VALID_REGEX("h?e");
	ASSERT_VALID_REGEX("h+?e");
	ASSERT_VALID_REGEX("h*?e");
	ASSERT_VALID_REGEX("\\?");
	ASSERT_VALID_REGEX("\\+");
	ASSERT_VALID_REGEX("[abc]");
	ASSERT_VALID_REGEX("[\\]]");
	ASSERT_VALID_REGEX("[a-z]");
	ASSERT_VALID_REGEX("[-a-zA-Z]");
	ASSERT_VALID_REGEX("he(ll)o");
	ASSERT_VALID_REGEX("he(ll|pp)o");
	ASSERT_VALID_REGEX("h(ell(o|p)a)de");
	ASSERT_VALID_REGEX(".");
	
	// test invalid expressions
	ASSERT_INVALID_REGEX("hello\\");
	ASSERT_INVALID_REGEX("+e");
	ASSERT_INVALID_REGEX("*e");
	ASSERT_INVALID_REGEX("?e");
	ASSERT_INVALID_REGEX("h+*");
	ASSERT_INVALID_REGEX("h?+");
	ASSERT_INVALID_REGEX("h?*");
	ASSERT_INVALID_REGEX("[a");
	ASSERT_INVALID_REGEX("(test");
	ASSERT_INVALID_REGEX("test)");
	
	return 0;
};
