/// lexerloader.hpp
/// 
/// This file is a part of the QuexPipe.
/// Loads shared libraries platform independent and instantiates their lexer objects by name.
/// © Móréh, Tamás 2016-2017
/// 
/// Private in Library module
/// 

#ifndef LEXERLOADER_HPP
#define LEXERLOADER_HPP

// --- includes --------------------------------------------------------------------------------------------------------

#include	"QuexPipeConfig.hpp"
#include	"types.hpp"


// --- predeclarations -------------------------------------------------------------------------------------------------

#if defined (QP_PLATFORM_UNIX) // All POSIX systems: Linux, BSD, macOS
using HandlePtr		= void*;
#elif defined (QP_PLATFORM_WINDOWS)
#include <windows.h>
using HandlePtr		= HMODULE;
#endif


// --- class declaration: LexerLoader ----------------------------------------------------------------------------------
// Loads the entire library, not only the lexer (later function)
class LexerLoader
{
	struct LibHandler
	{
		String8			name;
		HandlePtr		handler;
		LexerFactoryMap	lexerMap;
	};
	
	class FindLoaded
	{
		String8		name;
	public:
		FindLoaded (const String8& n)			: name (n) {}
		FindLoaded () = delete;
		bool operator () (const LibHandler& lh)	{ return lh.name == name; }
	};
	

	LexerFactoryMap			builtin_lexers;
	Vector<LibHandler>		loaded_libs;
							LexerLoader			();
public:
							~LexerLoader		();
							LexerLoader			(const LexerLoader&) = delete;
	LexerLoader&			operator=			(LexerLoader&) = delete;
	static LexerLoader&		instance			();
	
	LibraryStatus			load_library		(const String8& path);
	Vector<String8>&&		available_lexers	(); 
	ILexer*					create_lexer		(const String8& name, const String8& module = "");
};

// TODO next ver: close_library

#endif // LEXERLOADER_HPP
