#include	<algorithm>
#include	"quexpipe.hpp"	// TODO kell itt minden?
#include	"lexerloader.hpp"

static const String8 SEP = "::";
static const String8 BIN = "-";


using ApiVerFcn				= decltype (&api_version);
using GetLexerFactMapFcn	= decltype (&get_lexerfactorymap);
using LibraryNameFcn		= decltype (&library_name);


LexerLoader::LexerLoader ()
{
	// Load the built-in lexers:
	LexerFactoryMap* lexermap = get_lexerfactorymap ();
	for (auto iter : *lexermap) {
		loaded_lexers_by_name[iter.first] = iter.second;
		loaded_lexers_disambigued_by_groups[BIN + SEP + iter.first] = iter.second;
	}
}

LexerLoader& LexerLoader::instance ()
{
	static LexerLoader global;
	return global;
}


std::vector<String8>&& LexerLoader::available_lexers ()
{
	std::vector<String8> lexers;
	for (auto iter : loaded_lexers_disambigued_by_groups) {
		lexers.push_back (iter.first);
	}
	std::sort (lexers.begin (), lexers.end ());
	return std::move (lexers);
}

ILexer* LexerLoader::create_lexer (const String8& name, const String8& group)
{
	if (loaded_lexers_by_name.find (name) == loaded_lexers_by_name.end ())
		return nullptr;
	
	if (group.empty ()) {
		return loaded_lexers_by_name[name] ();
	}
	
	auto iter = loaded_lexers_disambigued_by_groups.find (group + SEP + name);
	if (iter == loaded_lexers_disambigued_by_groups.end ())
		return nullptr;
	else
		return iter->second ();
}



#if defined (QP_PLATFORM_UNIX)
#include <dlfcn.h>

LibraryStatus LexerLoader::load_library (const String8& path)
{
	void* libID = dlopen (path.c_str (), RTLD_NOW & RTLD_LOCAL);
	if (!libID) {
		return LibraryNotFound;
	}
	
	ApiVerFcn			ld_api_version			= reinterpret_cast<ApiVerFcn> (dlsym (libID, "api_version"));
	GetLexerFactMapFcn	ld_get_lexerfactorymap	= reinterpret_cast<GetLexerFactMapFcn> (dlsym (libID, "get_lexerfactorymap"));
	LibraryNameFcn		ld_library_name			= reinterpret_cast<LibraryNameFcn> (dlsym (libID, "library_name"));
	if (!ld_api_version || !ld_get_lexerfactorymap || !ld_library_name) {
		// The loaded library does not contain theese functions
		dlclose (libID);
		return LibraryNotCompatible;
	}
	
	if (ld_api_version () != api_version ()) {
		// The loaded library's version is not the corresponding
		// TODO more sophisticated check
		dlclose (libID);
		return LibraryVersionIsNotCompatible;
	}
	
	String8 libname(ld_library_name ());
	auto found = std::find_if (loaded_libs.begin (), loaded_libs.end (), FindLoaded (libname));
	if (found != loaded_libs.end ()) {
		if ((*found).handler != libID) {
			dlclose (libID);
			return LibraryNameReserved;
		}
		return LibraryNameReserved;
	}
	
	LexerFactoryMap* loaded_map = ld_get_lexerfactorymap ();
	if (loaded_map->size () == 0) {
		dlclose (libID);
		return LibraryEmpty;
	}
	
	loaded_libs.push_back ({libname, libID});
	
	for (auto iter : *loaded_map) {
		loaded_lexers_by_name[iter.first] = iter.second;
		loaded_lexers_disambigued_by_groups[libname + SEP + iter.first] = iter.second;
	}
	return LibraryLoaded;
}
#elif defined (QP_PLATFORM_WINDOWS)

#endif

