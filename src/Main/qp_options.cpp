//
// Created by Móréh Tamás on 2016. 11. 01..
//
#include <sstream>
#include "qp_options.hpp"
using namespace optparse;
#ifdef nop

if (hello) {
	do ()
} else {
	other ()
}

#endif

//--- Lister -----------------------------------------------------------------------------------------------------------

void Lister::operator()(const Option &option, const std::string &opt, const std::string &val,
						const OptionParser &parser)
{
	// TODO: Implement
	std::cout << opt << std::endl;
	exit(1);
}



//--- QuexPipeOptionParser ---------------------------------------------------------------------------------------------

QuexPipeOptionParser::QuexPipeOptionParser(int argc, const char **argv)
{

}
