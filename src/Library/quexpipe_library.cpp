// --- includes --------------------------------------------------------------------------------------------------------

#include	"quexpipe.hpp"
#include	"quexpipe_library.hpp"
#include	"lexerloader.hpp"


// --- class definition: QuexPipeLibrary -------------------------------------------------------------------------------

QuexPipeLibrary::QuexPipeLibrary ()
{
	// TODO: Default initializations
}

QuexPipeLibrary::~QuexPipeLibrary()
{
	delete output;  // deletes also the sources (lexers) recursivelly
	delete logger;	
}

FileStatus QuexPipeLibrary::input_file(const String8& path)
{
	
}

FileStatus QuexPipeLibrary::output_file(const String8& path, FileOutputMode mode)
{
	
}

LexerStatus QuexPipeLibrary::add_lexer(const String8& lexer, const String8& from)
{
	
}

AnalysisResult QuexPipeLibrary::run()
{
	
}




