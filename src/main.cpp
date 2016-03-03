/**************************************************************
 * TODO: licensz meg ilyenek
 **************************************************************/

#include <iostream>
#include <fstream>
#include <locale>
#include <string>
#include <vector>
#include <chrono>

#include "lexer_manager.h"

const uint8_t WCHAR_SIZE = sizeof(wchar_t);
const uint8_t CHAR_SIZE = sizeof(char);

std::streamsize input_chunk_size = 1024;
std::wistream *winput = &std::wcin;
std::istream *input = &std::cin;
std::wostream *woutput = &std::wcout;
std::ostream *output = &std::cout;


template <typename CHAR_T>
inline std::basic_string<CHAR_T> read_string(bool& finished, bool& ok, std::basic_istream<CHAR_T> &input) {
    std::basic_string<CHAR_T> buffer;
    if (input) {
        buffer.resize(input_chunk_size);
        input.read(&buffer[0], input_chunk_size);
        std::streamsize read = input.gcount();
        if (input.gcount() < input_chunk_size) {
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
    return read_string(finished, ok, *winput);
}


std::string read_from_input(bool& finished, bool& ok)
{
    return read_string(finished, ok, *input);
}


void run_wide(lexer_manager* last_lexer, std::wostream &output) {
    bool finished = false;
    bool ok = true;
    do {
        output << last_lexer->receive32(finished, ok);
    } while (!finished);
    output.flush();
}


void run_char(lexer_manager* last_lexer, std::ostream &output) {
    bool finished = false;
    bool ok = true;
    do {
        output << last_lexer->receive8(finished, ok);
    } while (!finished);
    output.flush();
}



//------------------------- THE MAIN FUNCTION -------------------------
int main(int argc, char *argv[])
{
    /// -i input
    /// -o output into a new file (or overwrite it) (like >)
    /// -a append into an existing file (or create it) (like >>)
    /// -c encoding

    // TODO: Use Iconv/ICU?
    setlocale(LC_ALL, "C.UTF-8");
    winput->imbue(std::locale("C.UTF-8"));

    if (argc <= 1) {
        std::wcerr << "No libraries given." << std::endl;
        return ERROR_NO_LIBS;
    }

    std::vector<char*> libraries;
    libraries.reserve(argc);
    for (int var = 1; var < argc; ++var) {
        libraries.push_back(argv[var]);
    }


    //------------------- INITIALIZE THE FIRST PLUGIN --------------------
    std::vector<lexer_manager*> managers;
    managers.reserve(libraries.size());
    managers.push_back(new lexer_manager(libraries[0]));


    //--------------- DECIDE THE CHAR TYPE AND INPUT CHUNK ---------------
    if (managers[0]->char_size == CHAR_SIZE) {
        managers[0]->set_source_func8(read_from_input);
    }
    else if (managers[0]->char_size == WCHAR_SIZE) {
        managers[0]->set_source_func32(read_from_winput);
    }
    else {
        std::wcerr << "No input function implemented for the given character size." << std::endl;
        return ERROR_LIB_CHARSIZE_INVALID;
    }
    input_chunk_size = (managers[0]->buffer_size) / 4;


    //-------------------- INITIALIZE OTHER PLUGINS --------------------
    for (uint16_t var = 1; var < libraries.size(); ++var) {
        managers.push_back(new lexer_manager(libraries[var], managers.back()));
    }


    //------------------------ RUN AFTER INIT ------------------------
    auto start_time = std::chrono::high_resolution_clock::now();
    if (managers[0]->char_size == CHAR_SIZE) {
        run_char(managers.back(), *output);
    }
    else if (managers[0]->char_size == WCHAR_SIZE) {
        run_wide(managers.back(), *woutput);
    }
    auto end_time = std::chrono::high_resolution_clock::now();

    //----------------------- TIME MEASUREMENT -----------------------
    std::wcerr << std::endl << L"Analysis is finished in "
               << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count()
               << L" ms."
               << std::endl;


    //------------------------ DELETE AFTER RUN ------------------------
    for (lexer_manager* lm: managers) {
        delete lm;
    }

    return 0;
}

