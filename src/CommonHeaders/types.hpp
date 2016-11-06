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

typedef std::basic_string<char>		String8;
typedef std::basic_string<char16_t>	String16;
typedef std::basic_string<char32_t>	String32;

typedef std::unordered_map<String8, ILexer* (*)()> LexerFactoryMap;

// Not implemented in this version:
//typedef std::unordered_map<String8, ILogger* (*)()> LoggerFactoryMap;
//typedef std::unordered_map<String8, ITextInput* (*)()> OutputFactoryMap;



// --- status codes ----------------------------------------------------------------------------------------------------

enum LibraryStatus
{
	LibraryLoaded,
	LibraryNotFound,
	LibraryNotCompatible,
	LibraryVersionIsNotCompatible
};

#endif // TYPES_HPP
