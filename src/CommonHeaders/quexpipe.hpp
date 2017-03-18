#ifndef QUEXPIPE_HPP
#define QUEXPIPE_HPP

// --- includes --------------------------------------------------------------------------------------------------------

#include	"types.hpp"

 
// --- class declaration: ITextInput -----------------------------------------------------------------------------------

class ITextInput
{
public:
	enum Status
	{
		Ok		= 0x00,
		End		= 0x01,
		Error	= 0x10
	};

	virtual				~ITextInput			() = 0;
	
	virtual Status		get_status			() = 0;
	// Outputs:
	virtual Status		read_string8		(String8&  output, size_t& row, size_t& column) = 0;
	virtual Status		read_string16		(String16& output, size_t& row, size_t& column) = 0;
	virtual Status		read_string32		(String32& output, size_t& row, size_t& column) = 0;
};



// --- class declaration: ILexer ---------------------------------------------------------------------------------------

class ILexer : public ITextInput
{
	const ITextInput*	source;
public:
						ILexer				();
	virtual				~ILexer				() = 0;
	
	// Methods:
	virtual void		set_source			(const ITextInput* text_source) { source = text_source; }
	virtual uint8_t		lexer_char_size		() const = 0;
};



// --- class declaration: ITextOutput ----------------------------------------------------------------------------------

class ITextInputReader
{
	const ITextInput*	source;
public:
						ITextInputReader	();
	virtual				~ITextInputReader	() = 0;
						
	// Methods:
	virtual void		set_source			(const ITextInput* text_source) { source = text_source; }
	virtual void		run					(const ILogger* logger = nullptr) = 0;
};



// --- class declaration: ILogger --------------------------------------------------------------------------------------

class ILogger
{
public:
						ILogger				();
						ILogger				(const ILogger&)	= delete;
						ILogger				(ILogger&&)			= delete;
	ILogger&			operator =			(const ILogger&)	= delete;
	ILogger&			operator =			(ILogger&&)			= delete;
	virtual				~ILogger			() = 0;
	
	virtual void		log					(const String8&  msg) = 0;
	virtual void		log					(const String16& msg) = 0;
	virtual void		log					(const String32& msg) = 0;
	virtual void		warn				(const String8&  msg) = 0;
	virtual void		warn				(const String16& msg) = 0;
	virtual void		warn				(const String32& msg) = 0;
	virtual void		err					(const String8&  msg) = 0;
	virtual void		err					(const String16& msg) = 0;
	virtual void		err					(const String32& msg) = 0;
};



// --- Reflection ------------------------------------------------------------------------------------------------------


extern "C" LexerFactoryMap* get_lexerfactorymap ()
{
	static LexerFactoryMap map;
	return &map;
}

inline void register_lexer (const String8& name, ILexer* (*lexer_new)())
{
	LexerFactoryMap* map = get_lexerfactorymap ();
	(*map)[name] = lexer_new;
}



// --- Other API functions ---------------------------------------------------------------------------------------------

extern "C" {

uint32_t	api_version ();
const char*	library_name ();

}

// TODO divide headers

#endif // QUEXPIPE_HPP






