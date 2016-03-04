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
 *   The hidden part of the dynamic library
 **************************************************************/

//#define ACCUMULATOR_IN_IMPLEMENTATION

/**
 * Encapsulates the datas and the functions of algorythm
 * (namespace used as a singleton object)
 */
namespace quexpipeitem {

struct
{
    QUEX_TYPE_CHARACTER* begin;
    QUEX_TYPE_CHARACTER* end;
} chunk;


QUEX_TYPE_STRING        (*read_from_source)(bool&, bool&) = 0;  ///< The function pointer which reads the input text
QUEX_TYPE_ANALYZER*     lexer;                                  ///< Pointer for the Quex lexer object

unsigned int            buffer_size;                            ///< Minimum buffer size. Set by the user.
bool                    source_terminated;                      ///< True if input source is terminated
bool                    ok;

QUEX_TYPE_CHARACTER*    rx_buffer = 0;                          ///< The local buffer. It will be sign by the chunk.
unsigned int            rx_size = 0;                            ///< Size of the buffer.

QUEX_TYPE_STRING        fill_back;
QUEX_TYPE_CHARACTER*    prev_lexeme_start_p = 0x0;

QUEX_TYPE_TOKEN*        token_bank[2];
size_t                  queue0_size = 2;
size_t                  queue1_size = 2;

QUEX_TYPE_TOKEN*        prev_tokens;
QUEX_TYPE_TOKEN*        cur_tokens;


inline void initialize()
{
    buffer_size = QUEX_SETTING_BUFFER_SIZE;  // todo: remove buffer_size
    source_terminated = false;
    ok = true;
    lexer = new QUEX_TYPE_ANALYZER((QUEX_TYPE_CHARACTER*)0, 0);
    token_bank[0] = new QUEX_TYPE_TOKEN[queue0_size];
    token_bank[1] = new QUEX_TYPE_TOKEN[queue1_size];
    cur_tokens =  token_bank[0];
    prev_tokens =  token_bank[1];
    cur_tokens[0].set(QUEX_TKN_TERMINATION);
    cur_tokens[1].set(QUEX_TKN_TERMINATION);
    prev_tokens[0].set(QUEX_TKN_TERMINATION);
    prev_tokens[1].set(QUEX_TKN_TERMINATION);
//    cur_tokens[0].set(42);
//    cur_tokens[1].set(42);
//    prev_tokens[0].set(42);
//    prev_tokens[1].set(42);
    lexer->token_queue_switch(&cur_tokens, &queue0_size);
    chunk.begin = chunk.end;
}

inline void finalize()
{
    // TODO: implement
    delete lexer;
    delete [] token_bank[0];
    delete [] token_bank[1];
    if (rx_buffer != 0)
        delete [] rx_buffer;
}



inline void fill()
{
    lexer->buffer_fill_region_prepare();
    QUEX_TYPE_STRING input_buffer;
    input_buffer.reserve(buffer_size);
    while (chunk.begin != chunk.end) {
        input_buffer += chunk.begin[0];
        chunk.begin++;
    }
    input_buffer += fill_back;
    fill_back.clear();
    while (input_buffer.size() < buffer_size && !source_terminated) {
        input_buffer += read_from_source(source_terminated, ok);
    }
    //rx_size = input_buffer.size();
    rx_size = buffer_size;
    if (rx_buffer != 0) {
        delete [] rx_buffer;
        rx_buffer = 0;
    }
    rx_buffer = new QUEX_TYPE_CHARACTER[rx_size];
    memcpy(rx_buffer, input_buffer.data(), sizeof(QUEX_TYPE_CHARACTER)*rx_size);

    fill_back += input_buffer.substr(buffer_size);

    chunk.begin = rx_buffer;
    chunk.end = rx_buffer + rx_size;

    if (chunk.begin != chunk.end)
        chunk.begin = (QUEX_TYPE_CHARACTER*)lexer->buffer_fill_region_append(chunk.begin, chunk.end);
}


inline QUEX_TYPE_STRING read_from_prev_tokens()
{
    if (prev_tokens[0].type_id() == QUEX_TKN_UNUSED) {
        return prev_tokens[0].text + prev_tokens[1].text;
    } else /*if (prev_tokens[0].type_id() == QUEX_TKN_UNUSED)*/ {
        return prev_tokens[0].text;
    }
}


inline QUEX_TYPE_STRING process(bool &finished, bool &)
{
    QUEX_TYPE_STRING result;
    lexer->token_queue_switch(&prev_tokens, &queue1_size);
    while (true) {
        //prev_lexeme_start_p = lexer->buffer_lexeme_start_pointer_get();
        lexer->receive(&cur_tokens);

        if (cur_tokens[0].type_id() == QUEX_TKN_TERMINATION)
        {
            if (source_terminated) {
                finished = true;
                return read_from_prev_tokens();
            } else {
                fill_back += read_from_prev_tokens();
                fill();
                // tartalom törlése? (most current, következőre prev lesz!)
            }
        }
        else if (cur_tokens[1].type_id() == QUEX_TKN_TERMINATION)
        {
            if (source_terminated) {
                finished = true;
                return read_from_prev_tokens() + cur_tokens[0].text;
            } else {
                //fill_back += cur_tokens[0].text;
                fill();
                //lexer->buffer_input_pointer_set(prev_lexeme_start_p);
                cur_tokens[0].text.clear();
            }
        }
        else
        {
            return read_from_prev_tokens();
        }



//        if (cur_tokens[0].type_id() == QUEX_TKN_TERMINATION) {
//            if (prev_tokens[1].type_id() != QUEX_TKN_TERMINATION) {
//                if (prev_tokens[0].type_id() == QUEX_TKN_UNUSED) {
//                    result += prev_tokens[0].text;
//                    result += prev_tokens[1].text;
//                } else {
//                    result += prev_tokens[0].text;
//                }
//            }
//            if (!source_terminated) {
//                fill();
//                continue;
//            }
//        }

//        if (cur_tokens[0].type_id() == QUEX_TKN_UNUSED &&
//                cur_tokens[1].type_id() == QUEX_TKN_TERMINATION) {
//            fill_back += cur_tokens[0].text;
//        }

//        if (prev_tokens[0].type_id() != QUEX_TKN_TERMINATION) {
//            if (prev_tokens[0].type_id() == QUEX_TKN_UNUSED) {
//                result += prev_tokens[0].text;
//                result += prev_tokens[1].text;
//            } else {
//                result += prev_tokens[0].text;
//            }
//        }
//        return result;
    }

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

