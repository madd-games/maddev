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

#ifndef ARCH_H
#define ARCH_H

/**
 * Include the architecture specific information file, detect what is missing, and either
 * supply a default or throw an error.
 *
 * The archinfo.h file is to be placed in the architecture-specific directory.
 */
#include "archinfo.h"

#ifndef	ArchByteMnemonic
#	define ArchByteMnemonic ".byte"
#endif

#ifndef ArchWordMnemonic
#	define ArchWordMnemonic ".value"
#endif

#ifndef ArchDwordMnemonic
#	define ArchDwordMnemonic ".long"
#endif

#ifndef ArchQwordMnemonic
#	define ArchQwordMnemonic ".quad"
#endif

#ifndef ArchExpand
#	error ArchExpand must be defined!
#endif

#ifndef ArchFlagSpecs
#	error ArchFlagSpecs must be defined!
#endif

/**
 * Format of a flag specification.
 */
typedef struct
{
	/**
	 * The flag.
	 */
	const char*				flag;
	
	/**
	 * Pointer to where 'value' shall be stored if this flag is detected.
	 */
	int*					store;
	
	/**
	 * The value to store at 'store' if the flag is detected.
	 */
	int					value;
} FlagSpec;

/**
 * Expand a procedure into assembly language, and print into stdout.
 */
void ArchExpand(const char *macname, Proc *proc);

/**
 * Flag specifications (terminated with flag == NULL).
 */
extern FlagSpec ArchFlagSpecs[];

#endif	/* ARCH_H */
