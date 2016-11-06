// --- includes --------------------------------------------------------------------------------------------------------
#include	<map>
#include	"quexpipe_library.hpp"
#include	"lexerloader.hpp"


// --- local data ------------------------------------------------------------------------------------------------------

static std::map<int, QuexPipeLibrary*> quexpipes;



// --- local function definitions --------------------------------------------------------------------------------------




// --- api function definitions ----------------------------------------------------------------------------------------

extern "C" {

///
/// \brief quepipe_new Instantiates a QuexPipe object.
/// \return Returns the id.
///
int		quexpipe_new			()
{
	QuexPipeLibrary* qp = new QuexPipeLibrary;
	long memaddr = reinterpret_cast<long> (qp);
	int id = static_cast<int> (memaddr * 7717);
	while (quexpipes.find (id) != quexpipes.end ()) {
		id++;
	}
	quexpipes[id] = qp;
	return id;
}


///
/// \brief quexpipe_delete calls the destructor of a QuexPipe object
/// \param qp_id the identifier of the QuexPipe object
/// \return 0 if Ok, -1 if the id is wrong
///
char	quexpipe_delete			(int qp_id)
{
	auto iter = quexpipes.find (qp_id);
	if (iter == quexpipes.end ())
		return -1;
	delete quexpipes[qp_id];
	quexpipes.erase (iter);
	return 0;
}


///
/// \brief qp_load_external_lib loads a library into the memory
/// \param path of the library file
/// \return 0 if the library is loaded;
///			1 if the file is not found;
///			2 if the library is not comatible;
///			3 if the library version is not same as the current API version
///
char	qp_load_external_lib	(const char* path)
{
	return LexerLoader::instance ().load_library (path);
}


// get the available lexers separated with, source:  TODO: QuexpipeLibrary, vagy közös?

///
/// \brief qp_available_lexers gives the all built-in and loaded lexer names
/// \return the names are prefixed with the module with :: and are separated with comma:
///			-::firstlexer,-::secondlexer,loadedmodule::firstlexer
///
char*	qp_available_lexers		()
{
	String8 lexernames;
	std::vector<String8> loaded = LexerLoader::instance ().available_lexers ();
	for (String8 lexername : loaded) {
		lexernames += lexername;
		lexernames += ',';
	}
	if (loaded.size () > 0)
		lexernames.erase (lexernames.size ()-1);
	return strdup (lexernames.c_str ());
}


///
/// \brief set_input_file specifies a file as the input source
/// \param qp_id the identifier of the QuexPipe object
/// \param path input file
/// \return 0 if Ok; -1 if the id is wrong; 1 if the file doesn't exist
///
char	qp_set_input_file		(int qp_id, const char* path)
{
	if (quexpipes.find (qp_id) == quexpipes.end ())
		return -1;
	return quexpipes[qp_id]->input_file (path);
}


///
/// \brief set_output_file specifies a file as the output
/// If the file doesn't exist it will be created, if it exists, however the new mode is set,
/// it will be owerwritten.
/// \param qp_id the identifier of the QuexPipe object
/// \param path output file
/// \param mode	'n': new or overwrite; 'a': append
/// \return 0 if Ok, file is opened; -1 if the id is wrong; 2 if the file is write protected
///
char	qp_set_output_file		(int qp_id, const char* path, char mode = NewFile)
{
	if (quexpipes.find (qp_id) == quexpipes.end ())
		return -1;
	if (mode != NewFile && mode != Append)
		mode = NewFile;
	return quexpipes[qp_id]->output_file (path, static_cast<FileOutputMode>(mode));
}

// TODO doc
///
/// \brief qp_chain_lexer
/// \param qp_id
/// \param name
/// \param module
/// \return 
///
char	qp_chain_lexer			(int qp_id, const char* name, const char* module = "")
{
	if (quexpipes.find (qp_id) == quexpipes.end ())
		return -1;
	return quexpipes[qp_id]->add_lexer (name, module);
}


///
/// \brief qp_run
/// \param qp_id the identifier of the QuexPipe object
/// \return 0 if Ok, 1 if something happened
///
char	qp_run					(int qp_id)
{
	if (quexpipes.find (qp_id) == quexpipes.end ())
		return -1;
	return quexpipes[qp_id]->run ();
}
// TODO: logger?

}


