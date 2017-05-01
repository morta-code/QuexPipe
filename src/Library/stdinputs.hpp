#ifndef STDINPUTS_HPP
#define STDINPUTS_HPP

#include	"interfaces.hpp"


// --- class declaration: FileInput -----------------------------------------------------------------------------------

class FileInput : public ITextInput
{
	std::ifstream*	infile;
	Status			status;
public:
			FileInput (const String8& name);
	virtual ~FileInput ();
	
	// ITextInput interface
	virtual Status get_status		() override;
	virtual Status read_string8		(String8&  output, size_t& row, size_t& column) override;
	virtual Status read_string16	(String16& output, size_t& row, size_t& column) override;
	virtual Status read_string32	(String32& output, size_t& row, size_t& column) override;
};



// --- class declaration: ConsoleInput -----------------------------------------------------------------------------------

class ConsoleInput : public ITextInput
{
public:
			ConsoleInput ();
	virtual ~ConsoleInput ();
	
	// ITextInput interface
	virtual Status get_status		() override;
	virtual Status read_string8		(String8&  output, size_t& row, size_t& column) override;
	virtual Status read_string16	(String16& output, size_t& row, size_t& column) override;
	virtual Status read_string32	(String32& output, size_t& row, size_t& column) override;
};

#endif // STDINPUTS_HPP
