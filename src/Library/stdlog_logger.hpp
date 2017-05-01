#ifndef STDLOGLOGGER_HPP
#define STDLOGLOGGER_HPP

// --- includes --------------------------------------------------------------------------------------------------------

#include	<iostream>
#include	"interfaces.hpp"



// --- class declaration: NULLLogger -----------------------------------------------------------------------------------

///
/// \brief The NULLLogger class
/// This logger does nothing.
/// 
class NULLLogger : public ILogger
{
public:
					NULLLogger		();
					~NULLLogger		();
	virtual void	log				(const String8& msg)	override;
	virtual void	log				(const String16& msg)	override;
	virtual void	log				(const String32& msg)	override;
	virtual void	warn			(const String8& msg)	override;
	virtual void	warn			(const String16& msg)	override;
	virtual void	warn			(const String32& msg)	override;
	virtual void	err				(const String8& msg)	override;
	virtual void	err				(const String16& msg)	override;
	virtual void	err				(const String32& msg)	override;
};



// --- class declaration: STDLogLogger ---------------------------------------------------------------------------------

///
/// \brief The STDLogger class
/// This logger prints all message to a specified std::ostream in the following format:
/// LOG:	msg
/// WARN:	msg
/// ERR:	msg
/// 
class STDLogger : public ILogger
{
	std::ostream*	dest;
public:
					STDLogger		(std::ostream& out = std::clog);
					~STDLogger		();
	// ILogger interface
	virtual void	log				(const String8& msg)	override;
	virtual void	log				(const String16& msg)	override;
	virtual void	log				(const String32& msg)	override;
	virtual void	warn			(const String8& msg)	override;
	virtual void	warn			(const String16& msg)	override;
	virtual void	warn			(const String32& msg)	override;
	virtual void	err				(const String8& msg)	override;
	virtual void	err				(const String16& msg)	override;
	virtual void	err				(const String32& msg)	override;
};

#endif // STDLOGLOGGER_HPP
