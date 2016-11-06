#ifndef QUEXPIPELIBRARY_HPP
#define QUEXPIPELIBRARY_HPP

// --- includes --------------------------------------------------------------------------------------------------------

#include	"types.hpp"

// --- class declaration: QuexPipeLibrary ------------------------------------------------------------------------------

class QuexPipeLibrary
{
	ILogger*				logger;
	ITextInput*				input;
	std::vector<ILexer*>	lexers;
	ITextInputReader*		output;
public:
							QuexPipeLibrary			();
							~QuexPipeLibrary		();
	
	LibraryStatus			load_external_library	(const String8& libpath);
	std::vector<String8>	available_lexers		() const;
	
};

#endif // QUEXPIPELIBRARY_HPP
