#ifndef ILEXER_HPP
#define ILEXER_HPP

#include    "itextsource.h"

class ILexer : public ITextSource
{
    ITextSource*				source;
public:
								ILexer				(const ITextSource* text_source);
    virtual						~ILexer             () = 0;
    virtual uint8_t				lexer_char_size     () = 0;
};

#endif // ILEXER_HPP
