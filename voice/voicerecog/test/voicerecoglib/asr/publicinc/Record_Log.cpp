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

#if defined(_WIN32) || defined(_WIN32_WCE)
#include "stdafx.h"
#endif

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <sstream>

#include "Record_Log.h"

std::ofstream g_of_log_file;

int
Fn_Open_Log_File
(
    const std::string& string_log_file_name
)
{
    g_of_log_file.open(string_log_file_name.c_str(), std::ofstream::app);
    if (!g_of_log_file.is_open()) {
        return -1;
    }

    return 0;
}

void
Fn_Close_Log_File()
{
    if (g_of_log_file.is_open()) {
        g_of_log_file.close();
    }
}


void Fn_Log(const char* pc_format, ...)
{
    va_list arg;
    char ac_buffer[256] = { 0 };

    va_start(arg, pc_format);
    // write buffer
    vsnprintf(ac_buffer, 256, pc_format, arg);

    // get current time
    time_t now;
    time(&now);
    struct tm* pTime = NULL;
    pTime = localtime(&now);
    if (NULL == pTime) {
        va_end(arg);
        return;
    }

    int int_year = pTime->tm_year;
    int int_month = pTime->tm_mon + 1;
    int int_day = pTime->tm_mday;
    int int_hour = pTime->tm_hour;
    int int_min = pTime->tm_min;
    int int_sec = pTime->tm_sec;

    g_of_log_file << int_year + 1900 << '/'
                  << (int_month < 10 ? "0" : "") << int_month << '/'
                  << (int_day < 10 ? "0" : "") << int_day << ' '
                  << (int_hour < 10 ? "0" : "") << int_hour << ':'
                  << (int_min < 10 ? "0" : "") << int_min << ':'
                  << (int_sec < 10 ? "0" : "") << int_sec << " :"
                  << ac_buffer << std::endl << std::flush;

    va_end(arg);
    return;
}




void
Fn_Log_Error
(
    char const * const pi_Function,
    char const * const pi_File,
    unsigned int const i_Line
)
{
    std::stringstream stringstream_Output;

    Fn_Log("### ******************************************************************");
    Fn_Log("### Asr Error");

    stringstream_Output << "### Func: " << pi_Function;

    Fn_Log(stringstream_Output.str().c_str());

    std::string string_File = pi_File;
    std::string::size_type i_Pos_Slash_Last = string_File.find_last_of("\\/");
    if (i_Pos_Slash_Last != std::string::npos) {
        string_File = string_File.substr(i_Pos_Slash_Last + 1);
    }

    stringstream_Output.clear();
    stringstream_Output.str("");
    stringstream_Output << "### File: " << string_File;

    Fn_Log(stringstream_Output.str().c_str());

    stringstream_Output.clear();
    stringstream_Output.str("");
    stringstream_Output << "### Line: " << i_Line;

    Fn_Log(stringstream_Output.str().c_str());

    Fn_Log("### ******************************************************************");
}

C_Log_Block::~C_Log_Block()
{
    std::string string_log = "< " + m_string_Name + " (Exit)";
    Fn_Log(string_log.c_str());
}

C_Log_Block::C_Log_Block
(
    std::string const & rstring_Name
)
    : m_string_Name(rstring_Name)
{
    std::string string_log = "> " + m_string_Name + " (Enter)";
    Fn_Log(string_log.c_str());
}

/* EOF */

