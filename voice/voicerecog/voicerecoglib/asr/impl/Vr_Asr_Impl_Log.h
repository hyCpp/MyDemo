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

#ifndef __Vr_Asr_Impl_Log_h__
#define __Vr_Asr_Impl_Log_h__

#include <time.h>

#if defined(_WIN32) || defined(_WIN32_WCE)

#include <stdlib.h>

#else

#include <string.h>

#endif

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

/**
 * @brief The C_Time class
 *
 * C_Time class
 */
class C_Time
{
  public:
#if defined(_WIN32) || defined(_WIN32_WCE)

    clock_t i_Sec_M;

#else

    int i_Sec_S;
    int i_Sec_N;

#endif

    C_Time
    operator -(C_Time const & ro_Time)
    {
        C_Time o_Time;

#if defined(_WIN32) || defined(_WIN32_WCE)

    o_Time.i_Sec_M = i_Sec_M - ro_Time.i_Sec_M;

#else

    o_Time.i_Sec_S = i_Sec_S - ro_Time.i_Sec_S;
    o_Time.i_Sec_N = i_Sec_N - ro_Time.i_Sec_N;

#endif

        return o_Time;
    }

    float
    M_Fn_Normalize()
    {
        float f_Temp = 0;

#if defined(_WIN32) || defined(_WIN32_WCE)

        f_Temp = (float)(i_Sec_M) / CLOCKS_PER_SEC;

#else

        i_Sec_S += static_cast<int>(i_Sec_N / static_cast<int>(1e9f));
        i_Sec_N = i_Sec_N % static_cast<int>(1e9f);

        IF (i_Sec_N < 0) {
            i_Sec_S -= 1;
            i_Sec_N += static_cast<int>(1e9f);
        }

        f_Temp = static_cast<float>(i_Sec_N);
        f_Temp /= 1e9f;
        f_Temp += i_Sec_S;

#endif
        return f_Temp;
    }

    void
    M_Fn_Output
    (
        std::ostream & rostream_Log
    )
    {
        float f_Temp = 0;

#if defined(_WIN32) || defined(_WIN32_WCE)

        f_Temp = (float)(i_Sec_M) / CLOCKS_PER_SEC;

#else

        f_Temp = static_cast<float>(i_Sec_N);
        f_Temp /= 1e9f;
        f_Temp += i_Sec_S;

#endif

        rostream_Log << f_Temp;
    }

    static
    C_Time
    M_Fn_Get_Time()
    {
        C_Time o_Time;

#if defined(_WIN32) || defined(_WIN32_WCE)

        o_Time.i_Sec_M = clock();

#else

        timespec o_timespec;
        memset(&o_timespec, 0, sizeof(o_timespec));
        clock_gettime(CLOCK_MONOTONIC, &o_timespec);

        o_Time.i_Sec_S = o_timespec.tv_sec;
        o_Time.i_Sec_N = o_timespec.tv_nsec;

#endif

        return o_Time;
    }
};

extern
C_Time
Fn_Get_Time();

/**
 * @brief The C_Log_File class
 *
 * C_Log_File class
 */
class C_Log_File
{
  public:
    int m_b_record_log;
    std::ofstream m_ofstream_Log;

    C_Log_File()
    : m_b_record_log(false)
    {
    }

    void
    M_Fn_Enable_Log
    (
        const int& b_Enable
    )
    {
        m_b_record_log = b_Enable;
    }

    void
    M_Fn_Init
    (
        string string_Path
    )
    {
        IF (!m_b_record_log) {
            return;
        }

        m_ofstream_Log.open(string_Path.c_str(), std::ios_base::out | std::ios_base::app | std::ios_base::binary);
        IF (!m_ofstream_Log) {
            assert(0);
        }
    }

    void
    M_Fn_Fina()
    {
        IF (!m_b_record_log) {
            return;
        }

        m_ofstream_Log.close();
    }

    void
    M_Fn_Time
    (
        C_Time const & ro_Time
    )
    {
        IF (!m_b_record_log) {
            return;
        }

        C_Time o_Time(ro_Time);

        o_Time.M_Fn_Normalize();
        o_Time.M_Fn_Output(m_ofstream_Log);
    }

    void
    M_Fn_Log
    (
        const string& ro_string_Log
    )
    {
        IF (!m_b_record_log) {
            return;
        }

        char c_time[256] = { 0 };
#if defined(_WIN32) || defined(_WIN32_WCE)

#else
        time_t t;
        time(&t);
        sprintf(c_time, "%s", ctime(&t));
        size_t i_strlen = strlen(c_time);
        IF (0 != i_strlen) {
            c_time[i_strlen - 1] = 0;
        }
#endif
        m_ofstream_Log << c_time << ": " << ro_string_Log;
    }

    void
    M_Fn_Tab()
    {

        IF (!m_b_record_log) {
            return;
        }

        m_ofstream_Log << '\t';
    }

    void
    M_Fn_Enter()
    {
        IF (!m_b_record_log) {
            return;
        }

        m_ofstream_Log << std::endl;
    }
};

extern
void
Fn_Err
(
    string const & rstring_Log
);

extern
void
Fn_Log
(
    string const & rstring_Log
);

#ifdef _IFLYTEK
#define Fn_Log_IFlyTek_ERROR(o_ISSErrID)                                                            \
    int i_IFlyTek_Error_Code = static_cast<int>(o_ISSErrID);                                        \
    stringstream ss_IFlyTek_Output;                                                                 \
    ss_IFlyTek_Output << "  IFlyTek_ERROR: " << i_IFlyTek_Error_Code;                               \
    static bool b_IFlyTek_Error_Loged = false;                                                      \
    if (!b_IFlyTek_Error_Loged) {                                                                   \
        b_IFlyTek_Error_Loged = true;                                                               \
        VR_ERROR(VOICE_VR_ERR_INVALID_PARAM, i_IFlyTek_Error_Code, "%s", ss_IFlyTek_Output.str().c_str());\
    } else {                                                                                        \
        VR_LOGE("%s", ss_IFlyTek_Output.str().c_str());                                             \
    }
#endif

#ifdef _NUANCE
#define Fn_Log_ST_ERROR(o_ST_ERROR)                                                                 \
    int i_ST_Error_Code = static_cast<int>(o_ST_ERROR);                                             \
    stringstream ss_ST_Output;                                                                      \
    ss_ST_Output << "  ST_ERROR: " << i_ST_Error_Code;                                              \
    static bool b_ST_Error_Loged = false;                                                           \
    if (!b_ST_Error_Loged) {                                                                        \
        b_ST_Error_Loged = true;                                                                    \
        VR_ERROR(VOICE_VR_ERR_INVALID_PARAM, i_ST_Error_Code, "%s", ss_ST_Output.str().c_str());    \
    } else {                                                                                        \
        VR_LOGE("%s", ss_ST_Output.str().c_str());                                                  \
    }

#define Fn_Log_PH_ERROR(o_PH_ERROR)                                                                 \
    int i_PH_Error_Code = static_cast<int>(o_PH_ERROR);                                             \
    stringstream ss_PH_Output;                                                                      \
    ss_PH_Output << "  PH_ERROR: " << i_PH_Error_Code;                                              \
    static bool b_PH_Error_Loged = false;                                                           \
    if (!b_PH_Error_Loged) {                                                                        \
        b_PH_Error_Loged = true;                                                                    \
        VR_ERROR(VOICE_VR_ERR_INVALID_PARAM, i_PH_Error_Code, "%s", ss_PH_Output.str().c_str());    \
    } else {                                                                                        \
        VR_LOGE("%s", ss_PH_Output.str().c_str());                                                  \
    }

#define Fn_Log_LH_ERROR(o_LH_ERROR)                                                                 \
    int i_LH_Error_Code = static_cast<int>(o_LH_ERROR);                                             \
    stringstream ss_LH_Output;                                                                      \
    ss_LH_Output << "  LH_ERROR: " << i_LH_Error_Code;                                              \
    static bool b_LH_Error_Loged = false;                                                           \
    if (!b_LH_Error_Loged) {                                                                        \
        b_LH_Error_Loged = true;                                                                    \
        VR_ERROR(VOICE_VR_ERR_INVALID_PARAM, i_LH_Error_Code, "%s", ss_LH_Output.str().c_str());    \
    } else {                                                                                        \
        VR_LOGE("%s", ss_LH_Output.str().c_str());                                                  \
    }
#endif

void
Fn_Log_Error
(
    char const * const pi_Function,
    char const * const pi_File,
    unsigned int const i_Line
);

void
Fn_Print_Bin_C_String(const char* p_String);

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
        string const & rstring_Name
    );

  PROTECTED:

    C_Log_Block // [F]Constructor: Copy
    (
        C_Log_Block const & // [I]
    );

    C_Log_Block & // [R]
    operator = // [F]Operator: =
    (
        C_Log_Block const & // [I]
    );

    string m_string_Name;
};

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
