#ifndef STDLOGLOGGER_HPP
#define STDLOGLOGGER_HPP

// --- includes --------------------------------------------------------------------------------------------------------

#include	<iostream>
#include	"quexpipe.hpp"



// --- class declaration: NULLLogger -----------------------------------------------------------------------------------

class NULLLogger : public ILogger
{
					NULLLogger		();
					~NULLLogger		();
public:
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

class STDLogLogger : public ILogger
{
	std::ostream*	dest;
public:
					STDLogLogger	(std::ostream& out = std::clog);
					~STDLogLogger	();
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
