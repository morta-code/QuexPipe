/// quexpipe_c_api.cpp
/// 
/// This file is a part of the QuexPipe.
/// © Móréh, Tamás 2016-2017
/// 
/// C API Interface Implementation
/// 



// --- includes --------------------------------------------------------------------------------------------------------
#include	<map>
#include	"quexpipe_library.hpp"
#include	"lexerloader.hpp"


// --- local global data -----------------------------------------------------------------------------------------------

static std::map<int, QuexPipeLibrary*> quexpipes;



// --- local function definitions --------------------------------------------------------------------------------------




// --- api function definitions ----------------------------------------------------------------------------------------

extern "C" {


int		quexpipe_new			()

{
	QuexPipeLibrary* qp = new QuexPipeLibrary;
	int id;
#ifdef QP_DEBUG
	id = 1;
#else
	uintptr_t memaddr = reinterpret_cast<uintptr_t> (qp);
	id = static_cast<int> (memaddr * 7717); // quasi-random id.
#endif	
	while (quexpipes.find (id) != quexpipes.end ()) {
		id++;
	}
	quexpipes[id] = qp;
	return id;
}


char	quexpipe_delete			(int qp_id)
{
	auto iter = quexpipes.find (qp_id);
	if (iter == quexpipes.end ())
		return -1;
	delete quexpipes[qp_id];
	quexpipes.erase (iter);
	return 0;
}


char	qp_load_external_lib	(const char* path)
{
	return LexerLoader::instance ().load_library (path);
}


char*	qp_available_lexers		()
{
	String8 lexernames;
	std::vector<String8> loaded = LexerLoader::instance ().available_lexers ();
	for (const String8& lexername : loaded) {
		lexernames += lexername;
		lexernames += ',';
	}
	if (loaded.size () > 0)
		lexernames.erase (lexernames.size ()-1);
	return strdup (lexernames.c_str ());
}


char	qp_set_input_file		(int qp_id, const char* path)
{
	if (quexpipes.find (qp_id) == quexpipes.end ())
		return -1;
	return quexpipes[qp_id]->input_file (path ? path : "");
}


char	qp_set_output_file		(int qp_id, const char* path, char mode)
{
	if (quexpipes.find (qp_id) == quexpipes.end ())
		return -1;
	if (mode != NewFile && mode != Append)
		mode = NewFile;
	return quexpipes[qp_id]->output_file (path ? path : "", static_cast<FileOutputMode> (mode));
}


char	qp_chain_lexer			(int qp_id, const char* name, const char* module)
{
	if (quexpipes.find (qp_id) == quexpipes.end ())
		return -1;
	return quexpipes[qp_id]->add_lexer (name, module);
}


char	qp_run					(int qp_id)
{
	if (quexpipes.find (qp_id) == quexpipes.end ())
		return -1;
	return quexpipes[qp_id]->run ();
}

// TODO: logger?
// TODO: run from?

} // extern "C"


