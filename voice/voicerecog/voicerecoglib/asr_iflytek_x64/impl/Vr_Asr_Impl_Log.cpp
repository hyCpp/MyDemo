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

#include "Vr_Asr_Impl_Afx.h"
#include "Vr_Asr_Impl_Log.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

void
Fn_Err
(
    string const & rstring_Log
)
{
    string string_Log("##Asr## ");
    string_Log += rstring_Log;

#ifndef MACRO_Dynamic_Debug

    return;

#endif

#if defined(_WIN32) || defined(_WIN32_WCE)

#else

    VR_LOGE("%s", string_Log.c_str());

#endif

}

void
Fn_Log
(
    string const & rstring_Log
)
{
    string string_Log("##Asr## ");
    string_Log += rstring_Log;

#ifndef MACRO_Dynamic_Debug

    return;

#endif

#if defined(_WIN32) || defined(_WIN32_WCE)

    Fn_Log_VR(string_Log);

#else

    VR_LOG_ASR("%s", string_Log.c_str());

#endif

#ifdef _GTEST_EMULATOR
    printf("%s\n", string_Log.c_str());
#endif

}

extern
void
Fn_Log_Time_Next
(
    string const & rstring_Log
);

void
Fn_Log_Error
(
    char const * const pi_Function,
    char const * const pi_File,
    unsigned int const i_Line
)
{
    stringstream stringstream_Output;

    Fn_Log("### ************");

    Fn_Log("### Asr Error");

    stringstream_Output << "### Func: " << pi_Function;

    Fn_Log(stringstream_Output.str());

    string string_File = pi_File;
    string::size_type i_Pos_Slash_Last = string_File.find_last_of("\\/");
    IF (i_Pos_Slash_Last != string::npos) {
        string_File = string_File.substr(i_Pos_Slash_Last + 1);
    }

    stringstream_Output.clear();
    stringstream_Output.str("");
    stringstream_Output << "### File: " << string_File;

    Fn_Log(stringstream_Output.str());

    stringstream_Output.clear();
    stringstream_Output.str("");
    stringstream_Output << "### Line: " << i_Line;

    Fn_Log(stringstream_Output.str());

    Fn_Log("### ************");

    stringstream stringstream_Error;

    stringstream_Error << " Func: " << pi_Function;
    stringstream_Error << " File: " << string_File;
    stringstream_Error << " Line: " << i_Line;

    Fn_Err(stringstream_Error.str());
}

void
Fn_Print_Bin_C_String(const char* p_String)
{
    IF (NULL == p_String) {
        return;
    }

    stringstream stringstream_Log;
    stringstream_Log << "    Bin: ";
    size_t i_Pos = 0;
    while (p_String[i_Pos] != 0) {
        stringstream_Log
            << std::hex
            << std::setw(2)
            << static_cast<unsigned int>(static_cast<unsigned char>(p_String[i_Pos]))
            << ' ';

        ++i_Pos;
    }

    Fn_Log(stringstream_Log.str().c_str());

    return;
}

C_Log_Block::~C_Log_Block()
{
    Fn_Log("< " + m_string_Name + " (Exit)");
}

C_Log_Block::C_Log_Block
(
    string const & rstring_Name
)
:m_string_Name(rstring_Name)
{
    Fn_Log("> " + m_string_Name + " (Enter)");
}

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */

