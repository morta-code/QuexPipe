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
#include <quexpipe/charfuncs.h>


/**
 * Creates an empty XML element from the given tagname.
 * @example qp_xml("title") returns "<title/>"
 */
template <typename CHAR_T>
inline CHAR_T* qp_xml(const CHAR_T* tagname)
{
    size_t len = qp_len(tagname);
    CHAR_T* ret = new CHAR_T[len+4];
    ret[0] = (CHAR_T)L'<';
    for (size_t i = 0;  tagname[i]; i++)
        ret[i+1] = tagname[i];
    ret[len+1] = (CHAR_T)L'/';
    ret[len+2] = (CHAR_T)L'>';
    ret[len+3] = 0;
    return ret;
}


/**
 * Creates an XML element from the given tagname and content.
 * @example qp_xml("title", "Star Wars") returns "<title>Star Wars</title>"
 */
template <typename CHAR_T>
CHAR_T* qp_xml(const CHAR_T* tagname, const CHAR_T* content)
{
    size_t len = 2 * qp_len(tagname) + qp_len(content);
    size_t ptr = 0;
    CHAR_T* ret = new CHAR_T[len+6];
    ret[ptr] = (CHAR_T)L'<';
    ++ptr;
    for (size_t i = 0;  tagname[i]; i++) {
        ret[ptr] = tagname[i];
        ++ptr;
    }
    ret[ptr] = (CHAR_T)L'>';
    ++ptr;
    for (size_t i = 0;  content[i]; i++) {
        ret[ptr] = content[i];
        ++ptr;
    }
    ret[ptr] = (CHAR_T)L'<';
    ++ptr;
    ret[ptr] = (CHAR_T)L'/';
    ++ptr;
    for (size_t i = 0;  tagname[i]; i++) {
        ret[ptr] = tagname[i];
        ++ptr;
    }
    ret[ptr] = (CHAR_T)L'>';
    ++ptr;
    ret[ptr] = 0;
    return ret;
}

/**
 * Creates a closing and an opening XML tag.
 * @example qp_xml_closeopen("title") returns "</title><title>"
 */
template <typename CHAR_T>
inline CHAR_T* qp_xml_closeopen(const CHAR_T* tagname)
{
    size_t len = 2 * qp_len(tagname) + 6;
    size_t ptr = 0;
    CHAR_T* ret = new CHAR_T[len];
    ret[ptr] = (CHAR_T)L'<';
    ++ptr;
    ret[ptr] = (CHAR_T)L'/';
    ++ptr;
    for (size_t i = 0; tagname[i]; i++) {
        ret[ptr] = tagname[i];
        ++ptr;
    }
    ret[ptr] = (CHAR_T)L'>';
    ++ptr;
    ret[ptr] = (CHAR_T)L'<';
    ++ptr;
    for (size_t i = 0; tagname[i]; i++) {
        ret[ptr] = tagname[i];
        ++ptr;
    }
    ret[ptr] = (CHAR_T)L'>';
    ++ptr;

    ret[ptr] = 0;
    return ret;
}
