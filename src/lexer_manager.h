#pragma once
#include "header.h"
#include <iostream>
#include <string>
#include <dlfcn.h>


class lexer_manager
{
private:
    void* libhandler;
    char* lib_name;
public:
    lexer_manager(char *libname, lexer_manager* source);
    lexer_manager(char *libname);
    ~lexer_manager();


    uint8_t char_size;                                          ///< The size of character bytes.
    uint32_t buffer_size;
    int api_version;
    void load_lib(char* lib_name);
    void (*initialize)();
    void (*set_source_func8)(std::string (*)(bool&, bool&));
    void (*set_source_func32)(std::wstring (*)(bool&, bool&));
    std::string (*receive8)(bool&, bool&);
    std::wstring (*receive32)(bool&, bool&);
    void (*finalize)();
};
