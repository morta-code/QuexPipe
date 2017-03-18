#ifndef TYPES_HPP
#define TYPES_HPP

// --- includes --------------------------------------------------------------------------------------------------------

#include	<vector>
#include    <string>
#include	<locale>
#include	<codecvt>
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

inline
String8 To_UTF8 (const String16& s)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
    return conv.to_bytes(s);
}

inline
String8 To_UTF8 (const String32& s)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.to_bytes(s);
}

inline
String16 To_UTF16 (const String8& s)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
    return conv.from_bytes(s);
}

inline
String16 To_UTF16 (const String32& s)
{
    std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> conv;
    String8 bytes = conv.to_bytes(s);
    return String16(reinterpret_cast<const char16_t*>(bytes.c_str()), bytes.length()/sizeof(char16_t));
}

inline
String32 To_UTF32 (const String8& s)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.from_bytes(s);
}

inline
String32 To_UTF32 (const String16& s)
{
    const char16_t *pData = s.c_str();
    std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> conv;
    return conv.from_bytes(reinterpret_cast<const char*>(pData), reinterpret_cast<const char*>(pData+s.length()));
}

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
	InputMissing,
	OutputMissing,
	Error
};


enum Encoding : char
{
	Utf8,
	Utf16,
	Utf32
};

#endif // TYPES_HPP
