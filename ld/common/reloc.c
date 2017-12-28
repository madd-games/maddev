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

#include "reloc.h"
#include "libobj.h"

#include <stdio.h>

static void storeReloc(void *relput, int size, int64_t value, int littleEndian)
{
	int64_t limit;
	switch (size)
	{
	case REL_BYTE:
		limit = 0x7F;
		break;
	case REL_WORD:
		limit = 0x7FFF;
		break;
	case REL_DWORD:
		limit = 0x7FFFFFFF;
		break;
	case REL_QWORD:
		limit = 0x7FFFFFFFFFFFFFFF;
		break;
	};
	
	int64_t testVal = value;
	if (testVal < 0) testVal = -testVal;
	
	if (testVal > limit)
	{
		fprintf(stderr, "%s: relocation truncated\n", progName);
		errorsOccured = 1;
		return;
	};
	
	if (littleEndian)
	{
		switch (size)
		{
		case REL_BYTE:
			*((int8_t*)relput) = (int8_t) value;
			break;
		case REL_WORD:
			*((ObjLE16*)relput) = OBJ_MAKE_LE16((uint16_t) (uint64_t) value);
			break;
		case REL_DWORD:
			*((ObjLE32*)relput) = OBJ_MAKE_LE32((uint32_t) (uint64_t) value);
			break;
		case REL_QWORD:
			*((ObjLE64*)relput) = OBJ_MAKE_LE64((uint64_t) value);
			break;
		};
	}
	else
	{
		switch (size)
		{
		case REL_BYTE:
			*((int8_t*)relput) = (int8_t) value;
			break;
		case REL_WORD:
			*((ObjBE16*)relput) = OBJ_MAKE_BE16((uint16_t) (uint64_t) value);
			break;
		case REL_DWORD:
			*((ObjBE32*)relput) = OBJ_MAKE_BE32((uint32_t) (uint64_t) value);
			break;
		case REL_QWORD:
			*((ObjBE64*)relput) = OBJ_MAKE_BE64((uint64_t) value);
			break;
		};
	};
};

void doReloc(void *relput, int size, int type, uint64_t pos, uint64_t symbol, int64_t addend)
{
	switch (type)
	{
	case REL_X86_ABSOLUTE:
		storeReloc(relput, size, symbol + addend, 1);
		break;
	case REL_X86_RELATIVE:
		storeReloc(relput, size, symbol - pos + addend, 1);
		break;
	default:
		fprintf(stderr, "%s: unknown relocation type (%d) detected!\n", progName, type);
		errorsOccured = 1;
		break;
	};
};
