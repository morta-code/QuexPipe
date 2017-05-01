/// lexerloader.cpp
/// 
/// This file is a part of the QuexPipe.
/// © Móréh, Tamás 2016-2017
/// 
/// Implementation
/// 


// --- includes --------------------------------------------------------------------------------------------------------

#include	<algorithm>
#include	"quexpipe.hpp"
#include	"lexerloader.hpp"

#if defined (QP_PLATFORM_UNIX) // All POSIX systems: Linux, BSD, macOS
#include <dlfcn.h>
#elif defined (QP_PLATFORM_WINDOWS)
#include <windows.h>
#endif


// --- predeclarations -------------------------------------------------------------------------------------------------

static const String8 SEP = "::";
static const String8 BIN = "-";

using ApiVerFcn				= decltype (&api_version);
using GetLexerFactMapFcn	= decltype (&get_lexerfactorymap);
using LibraryNameFcn		= decltype (&library_name);


// --- class implementations: LexerLoader ------------------------------------------------------------------------------

LexerLoader::LexerLoader ()
{
	// Load the built-in lexers:
	LexerFactoryMap* lexermap = get_lexerfactorymap ();
	builtin_lexers = LexerFactoryMap (*lexermap);
}


LexerLoader::~LexerLoader()
{
	for (size_t i = 0; i < loaded_libs.size (); ++i) {
		#if defined (QP_PLATFORM_UNIX) // All POSIX systems: Linux, BSD, macOS
		dlclose (loaded_libs[i].handler);
		#elif defined (QP_PLATFORM_WINDOWS)
		// TODO win
		#endif
	}
}


LexerLoader& LexerLoader::instance ()
{
	static LexerLoader global;
	return global;
}


Vector<String8>&& LexerLoader::available_lexers ()
{
	Vector<String8> lexers;	
	for (auto& iter : builtin_lexers) {
		lexers.emplace_back (BIN + SEP + iter.first);
	}
	
	for (const LibHandler& lh : loaded_libs) {
		for (auto& iter : lh.lexerMap) {
			lexers.emplace_back (lh.name + SEP + iter.first);
		}
	}
	
	return std::move (lexers);
}


ILexer* LexerLoader::create_lexer (const String8& name, const String8& module)
{
	// from builtins:
	if (module.empty () || module == BIN) {
		if (builtin_lexers.find (name) == builtin_lexers.end ())
			return nullptr;
		else
			return builtin_lexers[name] ();
	}
	
	// Is module a loaded module?
	auto found = std::find_if (loaded_libs.begin (), loaded_libs.end (), FindLoaded (module));
	if (found == loaded_libs.end ())
		return nullptr;
	
	// Does the module have a lexer with the name? If yes, call it:
	LexerFactoryMap& map = (*found).lexerMap;
	if (map.find (name) == map.end ())
		return nullptr;
	else
		return map[name] ();
}



#if defined (QP_PLATFORM_UNIX) // All POSIX systems: Linux, BSD, macOS

LibraryStatus LexerLoader::load_library (const String8& path)
{
	HandlePtr libHandle = dlopen (path.c_str (), RTLD_NOW & RTLD_LOCAL);
	if (!libHandle) {
		return LibraryNotFound;
	}
	
	ApiVerFcn			ld_api_version			= reinterpret_cast<ApiVerFcn> (dlsym (libHandle, "api_version"));
	GetLexerFactMapFcn	ld_get_lexerfactorymap	= reinterpret_cast<GetLexerFactMapFcn> (dlsym (libHandle, "get_lexerfactorymap"));
	LibraryNameFcn		ld_library_name			= reinterpret_cast<LibraryNameFcn> (dlsym (libHandle, "library_name"));
	if (!ld_api_version || !ld_get_lexerfactorymap || !ld_library_name) {
		// The loaded library does not contain theese functions
		dlclose (libHandle);
		return LibraryNotCompatible;
	}
	
	if (ld_api_version () != ld_api_version ()) {
		// The loaded library's version is not the corresponding
		// TODO more sophisticated check
		dlclose (libHandle);
		return LibraryVersionIsNotCompatible;
	}
	
	String8 libname(ld_library_name ());
	auto found = std::find_if (loaded_libs.begin (), loaded_libs.end (), FindLoaded (libname));
	if (found != loaded_libs.end ()) {
		// A library is loaded with this name
		if ((*found).handler != libHandle) {
			dlclose (libHandle);
			return LibraryNameReserved;
		}
		// The libraries are the same
		return LibraryNameReserved;
	}
	
	LexerFactoryMap* loaded_map = ld_get_lexerfactorymap ();
	if (loaded_map->size () == 0) {
		dlclose (libHandle);
		return LibraryEmpty;
	}
		
	loaded_libs.emplace_back (libname, libHandle, *loaded_map);
	return LibraryLoaded;
}

#elif defined (QP_PLATFORM_WINDOWS)
// TODO win

#else
LibraryStatus LexerLoader::load_library (const String8& path)
{
	static_assert (false, "This function cannot be implemented on this platform.");
}
#endif


LexerLoader::LibHandler::LibHandler (const String8& nameInit,
									 const HandlePtr handleInit,
									 const LexerFactoryMap& lexerMapInit) :
	name (nameInit),
	handler (handleInit),
	lexerMap (lexerMapInit)
{
}
