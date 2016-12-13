#ifndef LEXERLOADER_HPP
#define LEXERLOADER_HPP

// --- includes --------------------------------------------------------------------------------------------------------
#include	"QuexPipeConfig.hpp"
#include	"types.hpp"


// --- class declaration: LexerLoader ----------------------------------------------------------------------------------


class LexerLoader
{
	struct LibHandler
	{
		String8	name;
		void*	handler;
	};
	class FindLoaded
	{
		String8		name;
	public:
		FindLoaded (const String8& n)			: name (n) {}
		FindLoaded () = delete;
		bool operator () (const LibHandler& lh)	{ return lh.name == name; }
	};
	

	LexerFactoryMap			loaded_lexers_by_name;
	LexerFactoryMap			loaded_lexers_disambigued_by_groups;
	Vector<LibHandler>		loaded_libs;
							LexerLoader			();
							LexerLoader			(const LexerLoader&) = delete;
	LexerLoader&			operator=			(LexerLoader&) = delete;
public:
	static LexerLoader&		instance			();
	
	LibraryStatus			load_library		(const String8& path);
	std::vector<String8>&&	available_lexers	(); // TODO: libname::lexer - ez lesz a dizambiguacio
	ILexer*					create_lexer		(const String8& name, const String8& group = "");
};

// TODO next ver: close_library

#endif // LEXERLOADER_HPP
