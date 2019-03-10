/**
 * Copyright @ 2015 - 2016 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are NOT permitted except as agreed by
 * Suntec Software(Shanghai) Co., Ltd.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */


/**
 * @file Vr_Asr_Impl_Log.h
 * @brief Asr class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Record_Log_h__
#define __Record_Log_h__

#include <string>
#include <cassert>
#include <fstream>

int Fn_Open_Log_File(const std::string& string_log_file_name);
void Fn_Close_Log_File();
void Fn_Log(const char* pc_format, ...);

void
Fn_Log_Error
(
    char const * const pi_Function,
    char const * const pi_File,
    unsigned int const i_Line
);


/**
 * @brief The C_Log_Block class
 *
 * C_Log_Block class
 */

class C_Log_Block
{
public:

    ~C_Log_Block(); // [F]Destructor

    explicit
    C_Log_Block // [F]Constructor of Default
    (
        std::string const & rstring_Name
    );

protected:

    C_Log_Block // [F]Constructor: Copy
    (
        C_Log_Block const & // [I]
    );

    C_Log_Block & // [R]
    operator = // [F]Operator: =
    (
        C_Log_Block const & // [I]
    );

    std::string m_string_Name;
};

#endif
/* EOF */
