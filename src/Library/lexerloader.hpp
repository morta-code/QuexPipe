#ifndef LEXERLOADER_HPP
#define LEXERLOADER_HPP

// --- includes --------------------------------------------------------------------------------------------------------

#include	"types.hpp"


// --- class declaration: LexerLoader ----------------------------------------------------------------------------------


class LexerLoader
{
							LexerLoader			();
							LexerLoader			(const LexerLoader&) = delete;
	LexerLoader&			operator=			(LexerLoader&) = delete;
public:
	static LexerLoader&		instance			();
	
	LibraryStatus			load_library		(const String8& path);
	std::vector<String8>	available_lexers	(); // TODO: libname::lexer - ez lesz a dizambiguacio
	
};


#endif // LEXERLOADER_HPP
