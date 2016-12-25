// --- includes --------------------------------------------------------------------------------------------------------

#include	"stdlog_logger.hpp"


// --- class definition: NULLLogger ------------------------------------------------------------------------------------

NULLLogger::NULLLogger ()
{}

NULLLogger::~NULLLogger ()
{}

void NULLLogger::log (const String8&)
{}

void NULLLogger::log (const String16&)
{}

void NULLLogger::log (const String32&)
{}

void NULLLogger::warn (const String8&)
{}

void NULLLogger::warn (const String16&)
{}

void NULLLogger::warn (const String32&)
{}

void NULLLogger::err (const String8&)
{}

void NULLLogger::err (const String16&)
{}

void NULLLogger::err (const String32&)
{}



// --- class declaration: STDLogLogger ---------------------------------------------------------------------------------


STDLogger::STDLogger (std::ostream& out)
{
	dest = &out;
}

STDLogger::~STDLogger ()
{
	dest->flush ();
}

void STDLogger::log (const String8& msg)
{
	(*dest) << "LOG:\t" << msg << std::endl;
}

void STDLogger::log (const String16& msg)
{
	log (To_UTF8 (msg));
}

void STDLogger::log (const String32& msg)
{
	log (To_UTF8 (msg));
}

void STDLogger::warn (const String8& msg)
{
	(*dest) << "WARN:\t" << msg << std::endl;
}

void STDLogger::warn (const String16& msg)
{
	warn (To_UTF8 (msg));
}

void STDLogger::warn (const String32& msg)
{
	warn (To_UTF8 (msg));
}

void STDLogger::err (const String8& msg)
{
	(*dest) << "ERROR:\t" << msg << std::endl;
}

void STDLogger::err (const String16& msg)
{
	err (To_UTF8 (msg));
}

void STDLogger::err (const String32& msg)
{
	err (To_UTF8 (msg));
}

