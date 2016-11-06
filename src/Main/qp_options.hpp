//
// Created by Móréh Tamás on 2016. 11. 01..
//
#ifndef QUEXPIPE_QP_OPTIONS_HPP
#define QUEXPIPE_QP_OPTIONS_HPP

#include <string>
#include "optionparser.hpp"


//--- Lister -----------------------------------------------------------------------------------------------------------

class Lister : public optparse::Callback
{
public:
	virtual void operator() (const optparse::Option&		option,
							 const std::string&				opt,
							 const std::string&				val,
							 const optparse::OptionParser&	parser) override ;
};


//--- QuexPipeOptionParser ---------------------------------------------------------------------------------------------

class QuexPipeOptionParser
{
	std::string					version_str,
								usage_str,
								desc_str,
								epilog_str;

	optparse::OptionParser		parser;

public:
	QuexPipeOptionParser (int argc, char const *argv[]);

};

#endif //QUEXPIPE_QP_OPTIONS_HPP
