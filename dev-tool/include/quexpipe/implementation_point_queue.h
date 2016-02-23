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

bool                    source_terminated;                      ///< True if input source is terminated
bool                    ok;

QUEX_TYPE_CHARACTER     QX_BUFFER[QUEX_SETTING_BUFFER_SIZE + 10];  ///< The local buffer. It will be sign by the chunk.

QUEX_TYPE_STRING        fill_back;
QUEX_TYPE_STRING        input_buffer;

QUEX_TYPE_STRING        prev_token_text;

QUEX_TYPE_TOKEN*        tokens_begin = 0;
QUEX_TYPE_TOKEN*        tokens_end = 0;
QUEX_TYPE_TOKEN*        tokens_iterator = 0;


inline void initialize(unsigned int &)
{
    source_terminated = false;
    ok = true;
    lexer = new QUEX_TYPE_ANALYZER((QUEX_TYPE_CHARACTER*)0, 0, (QUEX_TYPE_CHARACTER*)0);
    input_buffer.reserve(QUEX_SETTING_BUFFER_SIZE);
}

inline void finalize()
{
    delete lexer;
}


// Ez jó!
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
    QUEX_TYPE_STRING result;
    while (true) {
        if (tokens_iterator < tokens_end) {
            if (tokens_iterator->type_id() == QUEX_TKN_TERMINATION) {
                // Ha már kifogyott a forrás, előző kiírása.
                if (source_terminated) {
                    finished = true;
                    return prev_token_text;
                }
                // Előző text visszatöltése (unused/LeMonde), feltöltés és újraolvasás kérése
                input_buffer += prev_token_text;
                prev_token_text.clear();
                fill();
                tokens_iterator = tokens_end;
            } else {
                // Előző kiírása, jelenlegi az előzőbe és léptetés
                result += prev_token_text;
                prev_token_text.clear();
                prev_token_text += tokens_iterator->text;
                tokens_iterator++;
                return result;
            }
        } else {
            lexer->receive(&tokens_iterator);
            lexer->token_queue_remainder_get(&tokens_begin, &tokens_end);
        }
    }

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

