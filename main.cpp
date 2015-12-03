/**************************************************************
 * TODO: licensz meg ilyenek
 **************************************************************/

#include <iostream>
#include <fstream>
#include <locale>
#include <string>
#include <vector>

#include "lexer_manager.h"

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


std::wstring read_from_winput(bool& finished, bool& ok)
{
    return read_string(finished, ok, std::wcin);
}


std::string read_from_input(bool& finished, bool& ok)
{
    return read_string(finished, ok, std::cin);
}


void run_wide(lexer_manager* last_lexer, std::wostream &output) {
    bool finished = false;
    bool ok = true;
    while (!finished) {
        std::wstring read = last_lexer->receive32(finished, ok);
        output << read;
    }
    output << std::endl;
    output.flush();
}


void run_char(lexer_manager* last_lexer, std::ostream &output) {
    bool finished = false;
    bool ok = true;
    while (!finished) {
        output << last_lexer->receive8(finished, ok);
    }
    output << std::endl;
    output.flush();
}



/**************************************************************
 *  The main function
 **************************************************************/
int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "hu_HU.utf8");

    if (sizeof(wchar_t) < 4)
        std::wcerr << "WARNING! Non 4 bytes character representation may occurs problems." << std::endl;

    if (argc <= 1) {
        std::wcerr << "No libraries given." << std::endl;
        return ERROR_NO_LIBS;
    }

    std::vector<char*> libraries;
    for (int var = 1; var < argc; ++var) {
        libraries.push_back(argv[var]);
    }

    std::vector<lexer_manager*> managers;
    // TODO: kódolás!
    managers.push_back(new lexer_manager(libraries[0]));

    //---------------------- DECIDE THE CHAR TYPE ----------------------
    if (managers[0]->char_size == (uint8_t)sizeof(char)) {
        managers[0]->set_source_func8(read_from_input);
    }
    else if (managers[0]->char_size == (uint8_t)sizeof(wchar_t)) {
        managers[0]->set_source_func32(read_from_winput);
    }
    else {
        std::wcerr << "No input function implemented for the given character size." << std::endl;
        return ERROR_LIB_CHARSIZE_INVALID;
    }

    //-------------------- INITIALIZE OTHER PLUGINS --------------------
    for (uint16_t var = 1; var < libraries.size(); ++var) {
        managers.push_back(new lexer_manager(libraries[var], managers.back()));
    }

    //------------------------ RUN AFTER INIT ------------------------

    // TODO ebbe az if-be helyezhető el a bemenet és a kimenet specifikálása, fájl esetén a kódolás is.
    if (managers[0]->char_size == (uint8_t)sizeof(char))
        run_char(managers.back(), std::cout);
    else if (managers[0]->char_size == (uint8_t)sizeof(wchar_t))
        run_wide(managers.back(), std::wcout);

    //------------------------ DELETE AFTER RUN ------------------------
    for (lexer_manager* lm: managers) {
        delete lm;
    }

    return 0;
}

