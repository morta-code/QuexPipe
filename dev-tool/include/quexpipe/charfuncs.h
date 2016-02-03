#pragma once
#include <cstddef>
#include <cstring>


/**
 * Calculates the length of the given cstring.
 */
template<typename CHARTYPE>
inline size_t qp_len(CHARTYPE* str)
{
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}


/**
 * Converts cstring represented number to int.
 */
template <typename CHAR_T>
inline int qp_atoi(const CHAR_T* lexeme)
{
    int k = 0;
    int mpy = 1;
    if (lexeme[0] == '-') {
        mpy = -1;
        ++lexeme;
    } else if (lexeme[0] == '+') {
        ++lexeme;
    }
    while (*lexeme) {
        k = (k<<3)+(k<<1)+(*lexeme)-'0';
        ++lexeme;
    }
    return k * mpy;
}


/**
 * Converts cstring represented number to float.
 */
template <typename CHAR_T>
inline float qp_atof(const CHAR_T* lexeme)
{
    int int_part = 0;
    float fract_part = 0;
    int divisor_for_fract = 1;
    int mpy = 1;
    bool in_fract = false;
    if (*lexeme == '-') {
        ++lexeme;
        mpy = -1;
    } else if (*lexeme == '+') {
        ++lexeme;
    }
    while (*lexeme != '\0') {
        if (*lexeme >= '0' && *lexeme <= '9') {
            if (in_fract) {
                fract_part = fract_part*10 + (*lexeme - '0');
                divisor_for_fract *= 10;
            } else {
                int_part = int_part*10 + (*lexeme - '0');
            }
        } else if (*lexeme == '.') {
            if (in_fract)
                return mpy * (int_part + fract_part/divisor_for_fract);
            else
                in_fract = true;
        } else {
            return mpy * (int_part + fract_part/divisor_for_fract);
        }
        ++lexeme;
    }
    return mpy * (int_part + fract_part/divisor_for_fract);
}


/**
 * Appends a cstring to an empty one.
 */
template <typename CHAR_T>
inline CHAR_T* qp_first_append(CHAR_T* str, size_t &len, size_t &capacity, const CHAR_T* new_str)
{
    size_t new_len = qp_len(new_str);
    if (new_len > capacity) {
        capacity += 2 * new_len;
        CHAR_T *str2 = new CHAR_T[capacity];
        memcpy(str2, new_str, sizeof(CHAR_T)*(new_len+1));
        len += new_len;
        delete str;
        return str2;
    } else {
        memcpy(str, new_str, sizeof(CHAR_T)*(new_len+1));
        len += new_len;
        return str;
    }
}


/**
 * Appends a cstring to an existing one.
 */
template <typename CHAR_T>
inline CHAR_T* qp_append(CHAR_T *str, size_t &len, size_t &capacity, const CHAR_T* new_str)
{
    size_t new_len = qp_len(new_str);
    if (len + new_len > capacity) {
        capacity += 2 * new_len;
        CHAR_T *str2 = new CHAR_T[capacity];
        memcpy(str2, str, sizeof(CHAR_T)*len);
        memcpy(str2+len, new_str, sizeof(CHAR_T)*(new_len+1));
        len += new_len;
        delete str;
        return str2;
    } else {
        memcpy(str+len, new_str, sizeof(CHAR_T)*(new_len+1));
        len += new_len;
        return str;
    }
}


/**
 * Concatenates two cstrings into a new one.
 */
template <typename CHAR_T>
inline CHAR_T* qp_concat(const CHAR_T *str1, const CHAR_T *str2) {
    size_t len1 = qp_len(str1);
    size_t len2 = qp_len(str2);
    CHAR_T *str = new CHAR_T[len1+len2+1];
    memcpy(str, str1, sizeof(CHAR_T)*(len1));
    memcpy(str+len1, str2, sizeof(CHAR_T)*(len2+1));
    return str;
}


/**
 * Concatenates three cstrings into a new one.
 */
template <typename CHAR_T>
inline CHAR_T* qp_concat(const CHAR_T *str1, const CHAR_T *str2, const CHAR_T *str3) {
    size_t len1 = qp_len(str1);
    size_t len2 = qp_len(str2);
    size_t len3 = qp_len(str3);
    CHAR_T *str = new CHAR_T[len1+len2+len3+1];
    memcpy(str, str1, sizeof(CHAR_T)*(len1));
    memcpy(str+len1, str2, sizeof(CHAR_T)*(len2));
    memcpy(str+len1+len2, str3, sizeof(CHAR_T)*(len3+1));
    return str;
}


/**
 * Concatenates four cstrings into a new one.
 */
template <typename CHAR_T>
inline CHAR_T* qp_concat(const CHAR_T *str1, const CHAR_T *str2, const CHAR_T *str3, const CHAR_T *str4) {
    size_t len1 = qp_len(str1);
    size_t len2 = qp_len(str2);
    size_t len3 = qp_len(str3);
    size_t len4 = qp_len(str4);
    CHAR_T *str = new CHAR_T[len1+len2+len3+len4+1];
    memcpy(str, str1, sizeof(CHAR_T)*(len1));
    memcpy(str+len1, str2, sizeof(CHAR_T)*(len2));
    memcpy(str+len1+len2, str3, sizeof(CHAR_T)*(len3));
    memcpy(str+len1+len2+len3, str4, sizeof(CHAR_T)*(len4+1));
    return str;
}



