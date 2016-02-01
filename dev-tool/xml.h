#pragma once
#include <quexpipe/charfuncs.h>

//template <typename CHAR_T>
//inline CHAR_T* qp_xml(const CHAR_T* tagname)
//{
//    std::basic_string<CHAR_T> str;
//    str += (CHAR_T*)L"<";
//    str += tagname;
//    str += (CHAR_T*)L"/";
//    str += (CHAR_T*)L">";
//    CHAR_T* repr = new CHAR_T[str.size()+1];
//    mempcpy(repr, str.data(), sizeof(CHAR_T)*(str.size()+1));
//    return repr;
//}

//template <typename CHAR_T>
//inline CHAR_T* qp_xml(const CHAR_T* tagname, const CHAR_T* content)
//{
//    std::basic_string<CHAR_T> str;
//    str += (CHAR_T*)L"<";
//    str += tagname;
//    str += (CHAR_T*)L">";
//    str += content;
//    str += (CHAR_T*)L"<";
//    str += (CHAR_T*)L"/";
//    str += tagname;
//    str += (CHAR_T*)L">";
//    CHAR_T* repr = new CHAR_T[str.size()+1];
//    mempcpy(repr, str.data(), sizeof(CHAR_T)*(str.size()+1));
//    return repr;
//}


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
    for (size_t i = 0;  tagname[i]; i++) {
        ret[ptr] = tagname[i];
        ++ptr;
    }
    ret[ptr] = (CHAR_T)L'/';
    ++ptr;
    ret[ptr] = (CHAR_T)L'>';
    ++ptr;
    ret[ptr] = 0;
    return ret;
}
