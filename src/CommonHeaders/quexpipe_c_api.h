/// quexpipe_c_api.h
/// 
/// This file is a part of the QuexPipe.
/// Controls the QuexPipe core library language independent.
/// © Móréh, Tamás 2016-2017
/// 
/// Public, link your external code with the QuexPipe library.
/// 


#ifndef QUEXPIPE_C_API_H
#define QUEXPIPE_C_API_H

// --- api function declarations ---------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#else

#endif

///
/// \brief quepipe_new Instantiates a QuexPipe object.
/// \return Returns the id.
///
int		quexpipe_new			();

///
/// \brief quexpipe_delete calls the destructor of a QuexPipe object
/// \param qp_id the identifier of the QuexPipe object
/// \return 0 if Ok, -1 if the id is wrong
///
char	quexpipe_delete			(int qp_id);

///
/// \brief qp_load_external_lib loads a library into the memory
/// \param path of the library file
/// \return 0 if the library is loaded;
///			1 if the file is not found;
///			2 if the library is not comatible;
///			3 if the library version is not same as the current API version
///			4 if the library is loaded but it is empty
///			5 if the library could be loaded but it's name is reserved
///
char	qp_load_external_lib	(const char* path);

///
/// \brief qp_available_lexers gives the all built-in and loaded lexer names
/// \return the names are prefixed with the module with :: and are separated with comma:
///			-::firstlexer,-::secondlexer,loadedmodule::firstlexer
///			This string is dynamically allocated, don't forget to free the memory!
///
char*	qp_available_lexers		();

///
/// \brief set_input_file specifies a file as the input source
/// If this function will not be called, the standard input will be set by default.
/// \param qp_id the identifier of the QuexPipe object
/// \param path input file
/// If path is an empty string or nullptr, the standard input will be (re)set.
/// \return	0 if Ok;
///			-1 if the id is wrong;
///			1 if the file doesn't exist
///
char	qp_set_input_file		(int qp_id, const char* path);

///
/// \brief set_output_file specifies a file as the output
/// If the file doesn't exist it will be created, if it exists, however the new mode is set,
/// it will be owerwritten.
/// If this function will not be called, the standard output will be set by default.
/// \param qp_id the identifier of the QuexPipe object
/// \param path output file
/// If path is an empty string or nullptr, the standard input will be (re)set.
/// \param mode	'n': new or overwrite; 'a': append
/// \return	0 if Ok, file is opened;
///			-1 if the id is wrong; 
///			2 if the file is write protected
///
char	qp_set_output_file		(int qp_id, const char* path, char mode);

///
/// \brief qp_chain_lexer appends a lexer to the analysis chain
/// \param qp_id the identifier of the QuexPipe object
/// \param name of the lexer
/// \param module where the lexer is (a quasi-namespace)
/// If module is not present and more lexers have the same name, the very last loaded one will be set.
/// \return	0 if Ok, lexer is chained;
///			-1 if the id is wrong;
///			1 if the lexer is not found
///
char	qp_chain_lexer			(int qp_id, const char* name, const char* module);

///
/// \brief qp_run starts the analysis
/// \param qp_id the identifier of the QuexPipe object
/// \return	0 if Ok, analysis completed successfully
///			-1 if the id is wrong;
///			1 if no input defined
///			2 if no output defined
///			3 if error happened during analysis
///
char	qp_run					(int qp_id);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // QUEXPIPE_C_API_H
