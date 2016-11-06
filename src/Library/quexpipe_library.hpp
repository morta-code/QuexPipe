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
	// TODO: hová?
	LibraryStatus			load_external_library	(const String8& libpath);
	std::vector<String8>	available_lexers		() const;
	
	FileStatus				input_file				(const String8& path);
	FileStatus				output_file				(const String8& path, FileOutputMode mode);
	LexerStatus				add_lexer				(const String8& lexer, const String8& from = "");
	AnalysisResult			run						();
};

#endif // QUEXPIPELIBRARY_HPP
