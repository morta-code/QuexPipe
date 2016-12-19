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


STDLogLogger::STDLogLogger (std::ostream& out)
{
	dest = &out;
}

STDLogLogger::~STDLogLogger ()
{
	dest->flush ();
}

void STDLogLogger::log (const String8& msg)
{
	(*dest) << "LOG:\t" << msg << std::endl;
}

void STDLogLogger::log (const String16& msg)
{
	log (To_UTF8 (msg));
}

void STDLogLogger::log (const String32& msg)
{
	log (To_UTF8 (msg));
}

void STDLogLogger::warn (const String8& msg)
{
	(*dest) << "WARN:\t" << msg << std::endl;
}

void STDLogLogger::warn (const String16& msg)
{
	warn (To_UTF8 (msg));
}

void STDLogLogger::warn (const String32& msg)
{
	warn (To_UTF8 (msg));
}

void STDLogLogger::err (const String8& msg)
{
	(*dest) << "ERROR:\t" << msg << std::endl;
}

void STDLogLogger::err (const String16& msg)
{
	err (To_UTF8 (msg));
}

void STDLogLogger::err (const String32& msg)
{
	err (To_UTF8 (msg));
}

