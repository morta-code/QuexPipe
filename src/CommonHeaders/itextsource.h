#ifndef ITEXTSOURCE_H
#define ITEXTSOURCE_H

#include    <string>

class ITextSource
{
public:
	enum Status
	{
		Ok		= 0x00,
		End		= 0x01,
		Error	= 0x10
	};

    virtual                                 ~ITextSource        () = 0;

    // Outputs:
    virtual Status     read_as_string8     (std::basic_string<char>&	 output, size_t& row, size_t& column) = 0;
    virtual Status     read_as_string16    (std::basic_string<char16_t>& output, size_t& row, size_t& column) = 0;
    virtual Status     read_as_string32    (std::basic_string<char32_t>& output, size_t& row, size_t& column) = 0;
};

#endif // ITEXTSOURCE_H
