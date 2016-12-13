#ifndef TYPES_HPP
#define TYPES_HPP

// --- includes --------------------------------------------------------------------------------------------------------

#include	<vector>
#include    <string>
#include	<unordered_map>


// --- type predeclarations --------------------------------------------------------------------------------------------

class ITextInput;
class ITextInputReader;
class ILexer;
class ILogger;



// --- type definitions ------------------------------------------------------------------------------------------------

using String8			= std::basic_string<char>;
using String16			= std::basic_string<char16_t>;
using String32			= std::basic_string<char32_t>;



using LexerFactoryMap	= std::unordered_map<String8, ILexer* (*)()>;

// Not implemented in this version:
//typedef std::unordered_map<String8, ILogger* (*)()> LoggerFactoryMap;
//typedef std::unordered_map<String8, ITextInput* (*)()> OutputFactoryMap;
template<typename T>
using Vector			= std::vector<T>;


// --- enumerators -----------------------------------------------------------------------------------------------------

enum LibraryStatus : char
{
	LibraryLoaded,
	LibraryNotFound,
	LibraryNotCompatible,
	LibraryVersionIsNotCompatible,
	LibraryEmpty,
	LibraryNameReserved
};


enum FileOutputMode : char
{
	NewFile	= 'n',
	Append	= 'a'
};


enum FileStatus : char
{
	FileOpened,
	FileNotExists,
	FileWriteProtected
};


enum LexerStatus : char
{
	LexerOk,
	LexerNotFound
};


enum AnalysisResult : char
{
	ResultOk,
	Error
};


enum Encoding : char
{
	Utf8,
	Utf16,
	Utf32
};

#endif // TYPES_HPP
