#pragma once
#include <string>

/**************************************************************
 * TODO: licensz meg ilyenek
 **************************************************************/


// TODO: remove, only for testing
// TODO: C esetében nem wchar_t De a főprogram?
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
CFUNC void initialize(unsigned int buffer_size);


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
    return sizeof(QUEX_TYPE_CHARACTER);
}

