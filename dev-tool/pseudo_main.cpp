#include <iostream>
#include <fstream>
#include <locale>
#include <string>
#include <vector>

#include "spaces"
#include "implementation_copy_single.h"


std::streamsize stdin_chunk_size = 100;  // TODO: paraméterezhetőség
//std::wistream* winput = &std::wcin;
//std::istream* input = &std::cin;
//std::wostream* woutput = &std::wcout;
//std::ostream* output = &std::cout;


template <typename CHAR_T>
std::basic_string<CHAR_T> read_string(bool& finished, bool& ok, std::basic_istream<CHAR_T> &input) {
    std::basic_string<CHAR_T> buffer;
    if (input) {
        buffer.resize(stdin_chunk_size);
        input.read(&buffer[0], stdin_chunk_size);
        std::streamsize read = input.gcount();
        if (input.gcount() < stdin_chunk_size) {
            buffer.resize(read);
            finished = true;
        } else {
            finished = input.eof();
        }
        ok = input.good();
    } else {
        finished = input.eof();
        ok = input.good();
    }
    return buffer;
}

std::wifstream input;

std::wstring read_from_winput(bool& finished, bool& ok)
{
    return read_string(finished, ok, input);
}




int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "hu_HU.utf8");
    input.open(argv[1]);
    input.imbue(std::locale("C.UTF-8"));
    initialize(128);
    set_source_func(read_from_winput);

    bool finished = false;
    bool ok = true;
    while (!finished) {
        std::wstring read = receive(finished, ok);
        std::wcout << read;
    }
    std::wcout << std::endl;
}
