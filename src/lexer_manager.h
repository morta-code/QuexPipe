/*******************************************************************************
 * Copyright (c) 2016 MÓRÉH Tamás
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser Public License v3
 * which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/
 *
 * This file is part of QuexPipe.
 *
 * QuexPipe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QuexPipe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 *
 * Contributors:
 *     MÓRÉH Tamás - Software architecture design and implementation
 ******************************************************************************/

#pragma once
#include "header.h"
#include <iostream>
#include <string>
#include <dlfcn.h>


/**
 * @brief The lexer_manager class which manages the lexers implemented in dynamic libraries.
 */
class lexer_manager
{
private:
    void* libhandler;                                       ///< The handler file of the loaded library.
    char* lib_name;                                         ///< The name of the loaded library (from constructor)
public:
    /**
     * @brief lexer_manager constructor
     * @param libname Tries to load dynamic library with this name.
     * @param source Other library which output will be used as input.
     * It checks the char type and sets the input function.
     */
    lexer_manager(char *libname, lexer_manager* source);
    /**
     * @brief lexer_manager constructor
     * @param libname Tries to load dynamic library with this name.
     * After initialization the source function pointer must be specified
     * using the corresponding set_source_func* function.
     */
    lexer_manager(char *libname);
    ~lexer_manager();

    /**
     * @brief Tries to load the library.
     * @param lib_name
     * It checks also the compatibility and terminates the program if
     * the given library is invalid or doesn't exist.
     */
    void load_lib(char* lib_name);
    /**
     * @brief Initializes the loaded library.
     * Creates the lexer and allocates the buffer.
     */
    void (*initialize)();
    /**
     * @brief Sets the input source of the lexer.
     * @param Function pointer with "finished" and "ok" params returning string.
     */
    void (*set_source_func8)(std::string (*)(bool&, bool&));
    void (*set_source_func32)(std::wstring (*)(bool&, bool&));
    /**
     * @brief Returns the analysis' results.
     * @param finished bool reference
     * @param ok bool reference
     */
    std::string (*receive8)(bool&, bool&);
    std::wstring (*receive32)(bool&, bool&);
    /**
     * @brief Closes the lexer and frees the buffer.
     */
    void (*finalize)();

    uint8_t char_size;                                      ///< The size of character bytes.
    uint32_t buffer_size;                                   ///< The lexer's buffer size.
    int api_version;                                        ///< Version of plugin API.
};
