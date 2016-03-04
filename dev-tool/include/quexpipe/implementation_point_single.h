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

QUEX_TYPE_STRING     (*read_from_source)(bool&, bool&) = 0;     ///< The function pointer which reads the input text
QUEX_TYPE_ANALYZER*  lexer;                                     ///< Pointer for the Quex lexer object

QUEX_TYPE_CHARACTER  QX_BUFFER[QUEX_SETTING_BUFFER_SIZE + 10];  ///< The local buffer. It will be sign by the chunk.
unsigned int         buffer_size;                               ///< Minimum buffer size. Set by the user.
bool                 source_terminated;                         ///< True if input source is terminated
bool                 ok;                                        ///< Indicates errors while analysis. (Not used.)

QUEX_TYPE_TOKEN      token_bank[2];                             ///< Stores the current and the previous token.
QUEX_TYPE_TOKEN*     prev_token;                                ///< Pointer to the previous token.
QUEX_TYPE_TOKEN_ID   token_id = QUEX_TKN_TERMINATION;           ///< The type-id of the current token.

QUEX_TYPE_STRING     fill_back;                                ///< Stores chars over the buffer size.
QUEX_TYPE_STRING     input_buffer;                             ///< Builds up the buffer from remainders and the input.


/**
 * @brief Initializes the library.
 * Creates the lexer and initializes token_bank.
 */
inline void initialize()
{
    source_terminated = false;
    ok = true;
    lexer = new QUEX_TYPE_ANALYZER((QUEX_TYPE_CHARACTER*)0, 0, (QUEX_TYPE_CHARACTER*)0);
    prev_token = &(token_bank[1]);
    token_bank[0].set(QUEX_TKN_TERMINATION);
    lexer->token_p_switch(&token_bank[0]);
}


/**
 * @brief Closes the library.
 * Destructs the lexer.
 */
inline void finalize()
{
    delete lexer;
}


/**
 * @brief Fills the local buffer from remainders and the input.
 * After the filling it moves data from input_buffer to QX_BUFFER.
 * It also fills fill_back when more caracters arrived then the limit.
 */
inline void fill()
{
    // Sets the first (protected position) char to 0x0 (by the pointing method constraint).
    QX_BUFFER[0] = (QUEX_TYPE_CHARACTER)0;
    // Loads back the previous input-overflowed data, then empty it.
    input_buffer += fill_back;
    fill_back.clear();
    // Loads data from input as many as possible.
    while (input_buffer.size() < QUEX_SETTING_BUFFER_SIZE && !source_terminated) {
        input_buffer += read_from_source(source_terminated, ok);
    }
    // When more data is loaded than the buffer limit,
    // it should be cut and the remainder will be stored away for the next filling cycle.
    size_t qx_size = input_buffer.size();
    if (QUEX_SETTING_BUFFER_SIZE < qx_size) {
        qx_size = QUEX_SETTING_BUFFER_SIZE;
        fill_back += input_buffer.substr(qx_size);
        input_buffer.resize(qx_size);
    }
    // Copies data into the buffer and calls the lexer.
    memcpy(QX_BUFFER + 1, input_buffer.c_str(),
           sizeof(QUEX_TYPE_CHARACTER)*(qx_size+1));
    lexer->reset_buffer(QX_BUFFER, qx_size+2, QX_BUFFER+qx_size+1);
    input_buffer.clear();
}


/**
 * @brief Process the input token by token.
 * @param finished bool reference will be true when the last token is produced.
 * @return the text content of the arrived token.
 */
inline QUEX_TYPE_STRING process(bool &finished, bool &)
{
// Using in-code accumulator to avoid the so-called LeMonde problem.
// (No accululator and multiple token-passing used in the Quex grammar! See the Quex doc.)
#ifdef ACCUMULATOR_IN_IMPLEMENTATION
    QUEX_TYPE_STRING accumulator;
    accumulator.reserve(256);
    prev_token = lexer->token_p_switch(prev_token);
    // Cycle until a valid token arrived.
    do {
        // Performs the analysis
        token_id = lexer->receive();
        if (token_id == QUEX_TKN_TERMINATION) {  // Term.token: The buffer became empty.
            if (!source_terminated) {
                // If the buffer can be refilled, loads back first the unconfirmed token text
                // and the accumulator's content to avoid the so-called LeMonde problem (see the Quex documentation).
                input_buffer += prev_token->text + accumulator;
                fill();
                accumulator.clear();
                prev_token->text.clear();
            } else {
                // If the input of the current lexer is empty, too, it sets finished and returns the remaining text.
                finished = true;
                return prev_token->text + accumulator;
            }
        } else if (token_id == QUEX_TKN_UNUSED) {
            // Push unused token text (actually character) to the accumulator.
            accumulator += lexer->token_p()->text;
        } else {  // token_id is valid
            // It validates the previous token and the accumulator content, therefore returns them.
            return prev_token->text + accumulator;
        }

    } while (true);

// Not using any accumulator in-code, however multiple token passing is also forbidden.
#else
    prev_token = lexer->token_p_switch(prev_token);
    // Cycle until a valid token arrived.
    do {
        // Performs the analysis
        token_id = lexer->receive();
        if (token_id == QUEX_TKN_TERMINATION) {  // Term.token: The buffer became empty.
            // If the input of the current lexer is empty, too, it sets finished and returns the remaining text.
            if (!source_terminated) {
                // If the buffer can be refilled, loads back first the unconfirmed token text.
                input_buffer += prev_token->text;
                fill();
                prev_token->text.clear();
            } else {
                // If the input of the current lexer is empty, too, it sets finished and returns the remaining text.
                finished = true;
                return prev_token->text;
            }
        } else  {  // token_id is valid
             // It validates the previous token, therefore returns them.
             return prev_token->text;
        }
    } while (true);
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

