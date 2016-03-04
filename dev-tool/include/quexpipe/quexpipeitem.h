/*******************************************************************************
 * Copyright (c) 2016 MÓRÉH Tamás
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser Public License v3
 * which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/
 *
 * This file is part of QuexPipe.
 *
 * QuexPipe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QuexPipe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 *
 * Contributors:
 *     MÓRÉH Tamás - Software architecture design and implementation
 ******************************************************************************/

#pragma once
#include <string>


#define API_VERSION 0x010000
#ifndef QUEX_SETTING_BUFFER_SIZE
#define QUEX_SETTING_BUFFER_SIZE 65536
#endif
#ifndef QUEX_TYPE_CHARACTER
#define QUEX_TYPE_CHARACTER wchar_t
#endif
#ifndef QUEX_TYPE_STRING
#define QUEX_TYPE_STRING std::basic_string<QUEX_TYPE_CHARACTER>
#endif
#ifndef QUEX_TYPE_ANALYZER
#define QUEX_TYPE_ANALYZER void
#endif
#ifndef QUEX_TYPE_TOKEN
#define QUEX_TYPE_TOKEN void
#endif
#ifndef QUEX_TYPE_TOKEN_ID
#define QUEX_TYPE_TOKEN_ID int
#endif


/**************************************************************
 *   Mandatory (public) functions for the dynamic library
 **************************************************************/

#ifdef __cplusplus
#define CFUNC extern "C"
#else
#define CFUNC
#endif

/**
 *
 */
CFUNC void initialize();


/**
 * Set the function pointer which reads the input text
 */
CFUNC void set_source_func(QUEX_TYPE_STRING (*source)(bool&, bool&));


/**
 * Performs the analysis and returns the result text
 */
CFUNC QUEX_TYPE_STRING receive(bool &finished, bool &ok);


/**
 * Frees the resources.
 */
CFUNC void finalize();


/**
 * Returns the character representation size.
 */
CFUNC unsigned char
charsize()
{
    return (unsigned char)sizeof(QUEX_TYPE_CHARACTER);
}


/**
 * Returns the size of current lexer's buffer.
 */
CFUNC unsigned int
buffer_size()
{
    return (unsigned int)QUEX_SETTING_BUFFER_SIZE;
}


/**
 * Returns the size of current lexer's buffer.
 */
CFUNC int
api_version()
{
    return (int)API_VERSION;
}
