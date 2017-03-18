// --- includes --------------------------------------------------------------------------------------------------------

#include	"quexpipe.hpp"  // TODO elég lenne csak azokat a típusokat, amik itt is előfordulnak.
#include	"quexpipe_library.hpp"
#include	"lexerloader.hpp"
#include	"stdlog_logger.hpp"
#include	"stdinputs.hpp"
#include	<paths.h>

// --- class definition: QuexPipeLibrary -------------------------------------------------------------------------------

QuexPipeLibrary::QuexPipeLibrary () :
	logger	(nullptr),
	input	(nullptr),
	output	(nullptr),
	row		(0),
	column	(0)
{
}

QuexPipeLibrary::~QuexPipeLibrary ()
{
	delete output;  // TODO deletes also the sources (lexers) recursivelly
	delete logger;	
}

FileStatus QuexPipeLibrary::input_file (const String8& path)
{
	ITextInput* tmp;
	if (path.empty ()) {
		tmp = new ConsoleInput ();
	} else {
		tmp = new FileInput (path);
	}
	if (tmp->get_status () != ITextInput::Ok) {
		delete tmp;
		return FileNotExists;
	}
	
	input = tmp;
	if (lexers.size () > 0) {
		lexers[0]->set_source (input);
	}
	return FileOpened;
}

FileStatus QuexPipeLibrary::output_file (const String8& path, FileOutputMode mode)
{
	// TODO ITextInput -> FileOutput, ConsoleOutput
}

LexerStatus QuexPipeLibrary::add_lexer (const String8& name, const String8& from)
{
	ILexer* currentLexer = LexerLoader::instance ().create_lexer (name, from);
	if (currentLexer == nullptr)
		return LexerNotFound;
	if (lexers.size () == 0)
		currentLexer->set_source (input);
	else
		currentLexer->set_source (lexers.back ());
	lexers.push_back (currentLexer);
	return LexerOk;
}

AnalysisResult QuexPipeLibrary::run ()
{
	if (logger == nullptr)
		logger = new NULLLogger;
	if (input == nullptr)
		return InputMissing;
	if (output == nullptr)
		return OutputMissing;
	// TODO complete
}




