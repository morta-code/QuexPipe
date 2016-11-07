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
	size_t					row;
	size_t					column;
public:
							QuexPipeLibrary			();
							~QuexPipeLibrary		();
	
	FileStatus				input_file				(const String8& path = "");
	FileStatus				output_file				(const String8& path = "", FileOutputMode mode = NewFile);
	LexerStatus				add_lexer				(const String8& lexer, const String8& from = "");
	AnalysisResult			run						();
};

#endif // QUEXPIPELIBRARY_HPP
