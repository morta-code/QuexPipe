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

// WARNING: This file contains method they work not correctly.
// It has no affect to the whole software.

#pragma once

#include <cstddef>
#include <cstring>
#include <quexpipe/quexpipeitem.h>

/**************************************************************
 *   The implementation part of the dynamic library
 **************************************************************/


/**
 * Encapsulates the datas and the functions of algorythm
 * (namespace used as a singleton object)
 */
namespace quexpipeitem {

struct MemoryChunk
{
    QUEX_TYPE_CHARACTER* begin;
    QUEX_TYPE_CHARACTER* end;
};


QUEX_TYPE_STRING        (*read_from_source)(bool&, bool&) = 0;  ///< The function pointer which reads the input text
QUEX_TYPE_ANALYZER*     lexer;                                  ///< Pointer for the Quex lexer object

unsigned int            buffer_size;                            ///< Minimum buffer size. Set by the user.
bool                    source_terminated;                      ///< True if input source is terminated
bool                    ok;

QUEX_TYPE_CHARACTER*    rx_buffer = 0;                          ///< The local buffer. It will be sign by the chunk.
unsigned int            rx_size = 0;                            ///< Size of the buffer.
MemoryChunk             chunk;                                  ///< Indicators of local (rx) buffer.

QUEX_TYPE_TOKEN         token_bank[2];
QUEX_TYPE_TOKEN*        prev_token;
QUEX_TYPE_CHARACTER*    prev_lexeme_start_p = 0;

//QUEX_TYPE_STRING read; // todo globál, chunk ennek a mutatóit jelöli ki
#ifdef ACCUMULATOR_IN_IMPLEMENTATION
QUEX_TYPE_STRING        accumulator;
#endif


inline void initialize()
{
    buffer_size = QUEX_SETTING_BUFFER_SIZE;  // todo: remove buffer_size
    source_terminated = false;
    ok = true;
    lexer = new QUEX_TYPE_ANALYZER((QUEX_TYPE_CHARACTER*)0, 0);
    prev_token = &(token_bank[1]);
    token_bank[0].set(QUEX_TKN_TERMINATION);
    lexer->token_p_switch(&token_bank[0]);
    chunk.begin = chunk.end;
}

inline void finalize()
{
    // TODO: implement
    delete lexer;
    if (rx_buffer != 0)
        delete [] rx_buffer;
}



inline void fill()
{

    // lexer->buffer_fill_region_prepare();
    QUEX_TYPE_STRING input_buffer;
    input_buffer.reserve(buffer_size);
    while (chunk.begin != chunk.end) {
        input_buffer += chunk.begin[0];
        chunk.begin++;
    }
    while (input_buffer.size() < buffer_size && !source_terminated) {
        input_buffer += read_from_source(source_terminated, ok);
    }
    rx_size = input_buffer.size();
    if (rx_buffer != 0) {
        delete [] rx_buffer;
        rx_buffer = 0;
    }
    rx_buffer = new QUEX_TYPE_CHARACTER[rx_size];
    memcpy(rx_buffer, input_buffer.data(), sizeof(QUEX_TYPE_CHARACTER)*rx_size);

    chunk.begin = rx_buffer;
    chunk.end = rx_buffer + rx_size;
    // todo: törölni:
    if (chunk.begin != chunk.end)
        chunk.begin = (QUEX_TYPE_CHARACTER*)lexer->buffer_fill_region_append(chunk.begin, chunk.end);
    // Ez jó
    // ki kéne próbálni, hogy string helyett közvetlen a mem.területre másolni, amíg nem telik be.
    // Mindenesetre a puffert a belső amúgy is meghatározza, nem kell egy továbbin dolgozni.

//    if (chunk.begin != chunk.end) {
//        chunk.begin = (QUEX_TYPE_CHARACTER*)lexer->buffer_fill_region_append(chunk.begin, chunk.end);
//        // Todo: létetés, ha tele lenne.
//    }
//    while (chunk.begin == chunk.end && !source_terminated) {
//        read = read_from_source(source_terminated, ok);
//        chunk.begin = &read[0];
//        chunk.end = &read[0]+read.size();
//        chunk.begin = (QUEX_TYPE_CHARACTER*)lexer->buffer_fill_region_append(chunk.begin, chunk.end);
//    }
}


inline QUEX_TYPE_STRING process(bool &finished, bool &)
{

/// Ismételd:
///     Tokenizálás
///     Ha a TOKEN terminál:
///         Ha van még FORRÁS:
///             Puffer feltöltése
///         Különben:
///             RETURN ELŐZŐ_TOKEN.text
///     Ha a TOKEN NEM terminál:
///         RETURN ELŐZŐ_TOKEN.text
///
///
/// Ismételd:
///     Tokenizálás
///     Ha a TOKEN terminál:
///         Ha van még FORRÁS:
///             Puffer feltöltése ACCU
///         Különben:
///             RETURN ACCU + ELŐZŐ_TOKEN.text
///     Ha a TOKEN NEM terminál:
///         Ha ELŐZŐ_TOKEN NEM unused:
///             RETURN ACCU + ELŐZŐ_TOKEN.text
///         Különben:
///             ACCU += ELŐZŐ_TOKEN.text
///

#ifdef ACCUMULATOR_IN_IMPLEMENTATION
    while (true) {
        QUEX_TYPE_TOKEN_ID token_id = QUEX_TKN_TERMINATION;
        //prev_lexeme_start_p = lexer->buffer_lexeme_start_pointer_get();
        prev_token = lexer->token_p_switch(prev_token);
        token_id = lexer->receive();
        if (token_id == QUEX_TKN_TERMINATION) {
            if (!source_terminated) {
                accumulator += prev_token->text;
                chunk.begin = &accumulator[0];
                chunk.end = chunk.begin + accumulator.size() - 1;
                //lexer->buffer_input_pointer_set(prev_lexeme_start_p);
                fill();
            } else {
                finished = true;
                QUEX_TYPE_STRING res = accumulator + prev_token->text;
                accumulator.clear();
                return res;
            }
        } else {
            if (prev_token->type_id() == QUEX_TKN_UNUSED) {
                accumulator += prev_token->text;
            } else {
                QUEX_TYPE_STRING res = accumulator + prev_token->text;
                accumulator.clear();
                return res;
            }
        }
    }
#else
    while (true) {
        QUEX_TYPE_TOKEN_ID token_id = QUEX_TKN_TERMINATION;
        prev_lexeme_start_p = lexer->buffer_lexeme_start_pointer_get();
        prev_token = lexer->token_p_switch(prev_token);
        token_id = lexer->receive();
        if (token_id == QUEX_TKN_TERMINATION) {
            if (!source_terminated) {
                // TODO accumulator here
                // Az UNUSEDeket töltsük vissza (vagy állítsuk vissza a pointert, ha lehet)
                lexer->buffer_input_pointer_set(prev_lexeme_start_p);
                fill();
            } else {
                finished = true;
                return prev_token->text;
            }
        } else /*if (prev_token->type_id() != QUEX_TKN_TERMINATION)*/ {
             return prev_token->text;
        }


        /* else if (prev_token->type_id() != QUEX_TKN_TERMINATION &&
                   prev_token->type_id() != QUEX_TKN_UNUSED) {
            return prev_token->text;
        } else if (prev_token->type_id() != QUEX_TKN_TERMINATION &&
                   prev_token->type_id() == QUEX_TKN_UNUSED) {
            // TODO accumulator here
            // Ha UNUSED jön, tegyük el. Ha Nem UNUSED jön, az eltetteket küldjük ki.
            accu += prev_token->text;
        } */
    }
#endif
}

}


/**************************************************************
 *   Mandatory (public) functions definitions
 **************************************************************/


CFUNC void
initialize()
{
    quexpipeitem::initialize();
}


CFUNC void
set_source_func(QUEX_TYPE_STRING (*source)(bool&, bool&))
{
    quexpipeitem::read_from_source = source;
    quexpipeitem::fill();
}


CFUNC QUEX_TYPE_STRING
receive(bool &finished, bool &ok)
{
    return quexpipeitem::process(finished, ok);
}


CFUNC void
finalize()
{
    quexpipeitem::finalize();
}

