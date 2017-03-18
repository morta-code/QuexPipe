#include "stdinputs.hpp"
#include <fstream>
#include <iostream>

// --- class definition: FileInput -----------------------------------------------------------------------------------

FileInput::FileInput (const String8& name) :
	status (Error)
{
	infile = new std::ifstream (name);
	if (infile->good ())
		status = Ok;
}

FileInput::~FileInput()
{
	infile->close ();
	delete infile;
}

ITextInput::Status FileInput::get_status ()
{
	return status;
}

ITextInput::Status FileInput::read_string8 (String8& output, size_t& row, size_t& column)
{
	// TODO
}

ITextInput::Status FileInput::read_string16 (String16& output, size_t& row, size_t& column)
{
	// TODO
}

ITextInput::Status FileInput::read_string32 (String32& output, size_t& row, size_t& column)
{
	// TODO
}


// --- class definition: ConsoleInput -----------------------------------------------------------------------------------

ConsoleInput::ConsoleInput()
{
	
}

ConsoleInput::~ConsoleInput()
{
	
}

ITextInput::Status ConsoleInput::get_status()
{
	if (std::cin.good ())
		return Ok;
	else
		return End;
}

ITextInput::Status ConsoleInput::read_string8(String8& output, size_t& row, size_t& column)
{
	// TODO
}

ITextInput::Status ConsoleInput::read_string16(String16& output, size_t& row, size_t& column)
{
	// TODO
}

ITextInput::Status ConsoleInput::read_string32(String32& output, size_t& row, size_t& column)
{
	// TODO
}
