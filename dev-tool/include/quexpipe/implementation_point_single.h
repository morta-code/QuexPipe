#pragma once

#include <cstddef>
#include <cstring>
#include <quexpipe/quexpipeitem.h>

/**************************************************************
 *   The hidden part of the dynamic library
 **************************************************************/


/**
 * Encapsulates the datas and the functions of algorythm
 * (namespace used as a singleton object)
 */
namespace quexpipeitem {


QUEX_TYPE_STRING        (*read_from_source)(bool&, bool&) = 0;  ///< The function pointer which reads the input text
QUEX_TYPE_ANALYZER*     lexer;                                  ///< Pointer for the Quex lexer object

QUEX_TYPE_CHARACTER     QX_BUFFER[QUEX_SETTING_BUFFER_SIZE + 10];  ///< The local buffer. It will be sign by the chunk.

unsigned int            buffer_size;                            ///< Minimum buffer size. Set by the user.
bool                    source_terminated;                      ///< True if input source is terminated
bool                    ok;


QUEX_TYPE_TOKEN         token_bank[2];
QUEX_TYPE_TOKEN*        prev_token;
QUEX_TYPE_TOKEN_ID      token_id = QUEX_TKN_TERMINATION;

QUEX_TYPE_STRING        fill_back;
QUEX_TYPE_STRING        input_buffer;


inline void initialize(unsigned int &)
{
    source_terminated = false;
    ok = true;
    lexer = new QUEX_TYPE_ANALYZER((QUEX_TYPE_CHARACTER*)0, 0, (QUEX_TYPE_CHARACTER*)0);
    input_buffer.reserve(QUEX_SETTING_BUFFER_SIZE);
    prev_token = &(token_bank[1]);
    token_bank[0].set(QUEX_TKN_TERMINATION);
    lexer->token_p_switch(&token_bank[0]);
}

inline void finalize()
{
    delete lexer;
}



inline void fill()
{
    QX_BUFFER[0] = (QUEX_TYPE_CHARACTER)0;
    input_buffer += fill_back;
    fill_back.clear();

    while (input_buffer.size() < QUEX_SETTING_BUFFER_SIZE && !source_terminated) {
        input_buffer += read_from_source(source_terminated, ok);
    }

    size_t qx_size = input_buffer.size();
    if (QUEX_SETTING_BUFFER_SIZE < qx_size) {
        qx_size = QUEX_SETTING_BUFFER_SIZE;
        fill_back += input_buffer.substr(qx_size - 1);
        input_buffer.resize(qx_size);
    }

    memcpy(QX_BUFFER + 1, input_buffer.c_str(),
           sizeof(QUEX_TYPE_CHARACTER)*(qx_size+1));
    lexer->reset_buffer(QX_BUFFER, qx_size+1, QX_BUFFER+qx_size);
    input_buffer.clear();
}


inline QUEX_TYPE_STRING process(bool &finished, bool &)
{
#ifdef ACCUMULATOR_IN_IMPLEMENTATION
    QUEX_TYPE_STRING accumulator;
    accumulator.reserve(256);
    prev_token = lexer->token_p_switch(prev_token);
    while (true) {
        token_id = lexer->receive();
        if (token_id == QUEX_TKN_TERMINATION) {
            if (!source_terminated) {
                input_buffer += prev_token->text + accumulator;
                fill();
                accumulator.clear();
                prev_token->text.clear();
            } else {
                finished = true;
                return prev_token->text + accumulator;
            }
        } else if (token_id == QUEX_TKN_UNUSED) {
                accumulator += lexer->token_p()->text;
        } else {  // token_id is valid
            return prev_token->text + accumulator;
        }

    }
#else
    prev_token = lexer->token_p_switch(prev_token);
    while (true) {
        token_id = lexer->receive();
        if (token_id == QUEX_TKN_TERMINATION) {
            if (!source_terminated) {
                input_buffer += prev_token->text;
                fill();
                prev_token->text.clear();
            } else {
                finished = true;
                return prev_token->text;
            }
        } else  {
             return prev_token->text;
        }
    }
#endif
}

}


/**************************************************************
 *   Mandatory (public) functions definitions
 **************************************************************/


CFUNC void
initialize(unsigned int buffer_size)
{
    quexpipeitem::initialize(buffer_size);
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

