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
 * Element: lexMatch
 */

#include "liblex.h"
#include <assert.h>
#include <stdio.h>

void assertMatch(const char *expr, const char *str, ssize_t expectedReturn)
{
	printf("MATCH '%s' AGAINST '%s', EXPECTING %d\n", expr, str, (int) expectedReturn);
	Regex *regex = lexCompileRegex(expr);
	assert(regex != NULL);
	ssize_t matchSize = lexMatch(regex, str);
	printf("->RESULT: %d\n", (int) matchSize);
	assert(matchSize == expectedReturn);
	lexDeleteRegex(regex);
};

int main()
{
	// test some valid matches
	assertMatch("hello", "hello", 5);
	assertMatch("hello", "hello world", 5);
	assertMatch("h+ello", "hello world", 5);
	assertMatch("h+ello", "hhhello world", 7);
	assertMatch("h*ello", "hello world", 5);
	assertMatch("h*ello", "hhhello world", 7);
	assertMatch("h*ello", "ello", 4);
	assertMatch("h?ello", "hello", 5);
	assertMatch("h?ello", "ello", 4);
	assertMatch("h\\?ello", "h?ello", 6);
	assertMatch("\\?hello", "?hello", 6);
	assertMatch("[abc]", "abc", 1);
	assertMatch("[abc]", "b", 1);
	assertMatch("[abc]+", "abc", 3);
	assertMatch("[a-zA-Z]+", "abcdefABCDEF", 12);
	assertMatch("(happy|sad)", "happy", 5);
	assertMatch("(happy|sad)", "sad", 3);
	assertMatch(".*", "test!", 5);
	assertMatch("$*", " \t ", 3);
	assertMatch(".*?", "test!", 5);
	assertMatch(".*?s", "test", 3);
	assertMatch(".*?sb?t", "test", 4);
	assertMatch(".*?sb?", "test", 3);
	
	// test invalid matches
	assertMatch("hello", "world", -1);
	assertMatch("h+ello", "ello", -1);
	assertMatch("h\\+ello", "hello", -1);
	assertMatch("[abc]", "z", -1);

	return 0;
};
