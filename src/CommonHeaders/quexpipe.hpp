#ifndef QUEXPIPE_HPP
#define QUEXPIPE_HPP
#pragma once
// --- includes --------------------------------------------------------------------------------------------------------

#include	"types.hpp"


// --- Reflection ------------------------------------------------------------------------------------------------------


extern "C" LexerFactoryMap* get_lexerfactorymap ()
{
	static LexerFactoryMap map;
	return &map;
}

inline void register_lexer (const String8& name, ILexer* (*lexer_new)())
{
	LexerFactoryMap* map = get_lexerfactorymap ();
	(*map)[name] = lexer_new;
}



// --- Other API functions ---------------------------------------------------------------------------------------------

extern "C" {

uint32_t	api_version ();
const char*	library_name ();

}

#endif // QUEXPIPE_HPP






