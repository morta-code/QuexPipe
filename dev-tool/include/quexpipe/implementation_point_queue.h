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

QUEX_TYPE_STRING     (*read_from_source)(bool&, bool&) = 0;    ///< The function pointer which reads the input text
QUEX_TYPE_ANALYZER*  lexer;                                    ///< Pointer for the Quex lexer object
bool                 source_terminated;                        ///< True if input source is terminated
bool                 ok;                                       ///< Indicates errors while analysis. (Not used.)

QUEX_TYPE_CHARACTER  QX_BUFFER[QUEX_SETTING_BUFFER_SIZE + 10]; ///< The local buffer. It will be used by the lexer.
QUEX_TYPE_STRING     fill_back;                                ///< Stores chars over the buffer size.
QUEX_TYPE_STRING     input_buffer;                             ///< Builds up the buffer from remainders and the input.

QUEX_TYPE_STRING     prev_token_text;                          ///< Stores the text of the previous received token.
QUEX_TYPE_TOKEN*     tokens_begin = 0;                         ///< First element of the received token queue.
QUEX_TYPE_TOKEN*     tokens_end = 0;                           ///< Exclusive upper boundary of the token queue.
QUEX_TYPE_TOKEN*     tokens_iterator = 0;                      ///< Pointer to the current token.


/**
 * @brief Initializes the library.
 * Creates the lexer.
 */
inline void initialize()
{
    source_terminated = false;
    ok = true;
    lexer = new QUEX_TYPE_ANALYZER((QUEX_TYPE_CHARACTER*)0, 0, (QUEX_TYPE_CHARACTER*)0);
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
    // Cycle until a valid token arrived.
    do {
        if (tokens_iterator < tokens_end) {  // There are tokens in the queue.
            if (tokens_iterator->type_id() == QUEX_TKN_TERMINATION) {  // Term.token: The buffer became empty.
                // If the input of the current lexer is empty, too, it sets finished and returns the remaining text.
                if (source_terminated) {
                    finished = true;
                    return prev_token_text;
                }
                // If the buffer can be refilled, loads back first the unconfirmed token text
                // to avoid the so-called LeMonde problem (see the Quex documentation).
                input_buffer += prev_token_text;
                prev_token_text.clear();
                fill();
                // Empties the queue. (See down.)
                tokens_iterator = tokens_end;
            } else {
                // A valid token arrived, it confirms the previous is also valid.
                // Stores the current token's text away and returns the previous'.
                QUEX_TYPE_STRING result = prev_token_text;
                prev_token_text.clear();
                prev_token_text += tokens_iterator->text;
                // Step to the next token.
                tokens_iterator++;
                return result;
            }
        } else {  // The token queue is empty.
            // Performs the analysis and sets the corresponding token pointers.
            // (By the lexer-managed memory management.)
            lexer->receive(&tokens_iterator);
            lexer->token_queue_remainder_get(&tokens_begin, &tokens_end);
        }
    } while (true);

}


}


/**************************************************************
 *   Mandatory (public) function definitions
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

