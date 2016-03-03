#include "lexer_manager.h"

//------------------- LEXER_MANAGER DEFINITIONS --------------------

lexer_manager::lexer_manager(char *libname, lexer_manager *source)
{
    lib_name = libname;
    load_lib(lib_name);
    if (char_size != source->char_size)
    {
        std::wcerr << "The character size of "
                   << lib_name
                   << " module is not the same as the "
                   << source->lib_name
                   << "'s."
                   << std::endl;
        exit(ERROR_LIB_CHARSIZE_INVALID);
    }
    // Beláncolás
    if (char_size == (uint8_t)sizeof(char)) {
        set_source_func8(source->receive8);
    } else if (char_size == (uint8_t)sizeof(wchar_t)) {
        set_source_func32(source->receive32);
    }
}


lexer_manager::lexer_manager(char *libname)
{
    lib_name = libname;
    load_lib(lib_name);
}


lexer_manager::~lexer_manager()
{
    finalize();
    dlclose(libhandler);
}


void lexer_manager::load_lib(char *name)
{
    libhandler = dlopen(name, RTLD_LAZY);
    if (!libhandler) {
        std::wcerr << dlerror() << std::endl;
        exit(ERROR_LIB_NOT_FOUND);
    }
    // load function pointers from the loaded library.
    char_size = ((uint8_t (*)())dlsym(libhandler, "charsize"))();
    buffer_size = ((uint32_t (*)())dlsym(libhandler, "buffer_size"))();
    api_version = ((int (*)())dlsym(libhandler, "api_version"))();
    if (api_version != CURRENT_API_VERSION) {
        std::wcerr << L"The given library is invalid. No matching API version." << std::endl;
        exit(ERROR_LIB_IS_INVALID);
    }
    initialize = (void (*)())dlsym(libhandler, "initialize");
    set_source_func8 = (void (*)(std::string (*)(bool&, bool&)))dlsym(libhandler, "set_source_func");
    set_source_func32 = (void (*)(std::wstring (*)(bool&, bool&)))dlsym(libhandler, "set_source_func");
    receive8 = (std::string (*)(bool&, bool&))dlsym(libhandler, "receive");
    receive32 = (std::wstring (*)(bool&, bool&))dlsym(libhandler, "receive");
    finalize = (void (*)())dlsym(libhandler, "finalize");
//    if (!initialize || !finalize) {
//        std::cerr << dlerror() << std::endl
//                  << "The given library: " << lib_name << " is not compatible with the program." << std::endl;
//        std::cerr.flush();
//        exit(ERROR_LIB_IS_INVALID);
//    }
    initialize();
}
