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

#ifndef TARGAUX_H
#define TARGAUX_H

#include <inttypes.h>

typedef struct
{
	/**
	 * What section index this section was given in the ELF image.
	 */
	size_t				index;
	
	/**
	 * Offset into the section name string table, for the name of this section.
	 */
	int64_t				nameoff;
	
	/**
	 * Offset into the section name string table, for the name of the relocation section.
	 */
	int64_t				relnameoff;
	
	/**
	 * Offset to the section data in the file (only for PROGBITS sections).
	 */
	int64_t				dataOff;
	
	/**
	 * Offset to the relocation table in the file (only for PROGBITS sections).
	 */
	int64_t				relOff;
	
	/**
	 * Number of padding bytes to isnert at the start of this section.
	 */
	int64_t				padSize;
	
	/**
	 * Number of relocations.
	 */
	size_t				numRelocs;
} SectionAux;

#endif
