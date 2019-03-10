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
 * @file Vr_Asr_Impl_Afx.h
 * @brief Asr class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Precompile_h__
#define __Vr_Asr_Impl_Precompile_h__

// Application framework X.

#if defined(_WIN32) || defined(_WIN32_WCE)
#include "stdafx.h"
#endif

// Library C C++ Standard

#include <stdlib.h>
#if defined(_WIN32) || defined(_WIN32_WCE)
#include <io.h>
#include <direct.h>
#include <VR_Log.h>
#endif

// Library C

#include <math.h>

// Library C++ STL

#include <stdexcept>
#include <limits>
#include <iosfwd>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <atomic>
// Library C++ Boost

#include <boost/scoped_array.hpp>
#include <boost/shared_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

// Using C++ STL

using std::istream;
using std::ostream;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::string;
using std::wistream;
using std::wostream;
using std::wifstream;
using std::wofstream;
using std::wstringstream;
using std::wstring;
using std::pair;
using std::auto_ptr; // ap

// Using C++ Boost

using boost::scoped_array; // sca
using boost::shared_array; // sa
using boost::scoped_ptr; // scp
using boost::shared_ptr; // sp
using boost::weak_ptr; // wp
using boost::const_pointer_cast;
using boost::static_pointer_cast;
using boost::dynamic_pointer_cast;
using boost::reinterpret_pointer_cast;
using boost::function;

// Warning
#if defined(_WIN32) || defined(_WIN32_WCE)
#pragma warning(disable:4996)
#pragma warning(disable:4355)
#pragma warning(disable:4800)
#endif

// Pugixml

#include "pugixml.hpp"

// Eval

#include <expreval.h>

// Lib

#include "BL_Thread.h"
#include "CL_WaitObj.h"
#include "CL_SyncObj.h"
#include "CL_AutoSync.h"
#include "BL_Crc32Calc.h"
#include "CL_Dir_Abs.h"

typedef CL_WaitObj C_Semaphore;
typedef CL_SyncObj C_Mutex;
typedef CL_AutoSync C_Lock_Mutex;
typedef BL_Crc32Calc C_Crc32Calc;
typedef CL_Dir_Abs C_Dir;

#if defined(_WIN32) || defined(_WIN32_WCE)
#include "Vr_Fn_Log_VR.h"
#include "Vr_Fn_Get_string_Path_VR.h"
#else
#ifndef CXX_VR_LOG_H
#include "VR_Log.h"
#endif
#endif

//***********************
// Switch for windows tool

#if defined(_WIN32) || defined(_WIN32_WCE)

#if 0

#ifndef MACRO_Feature_Reserved
#define MACRO_Feature_Reserved
#endif

#ifndef MACRO_Dynamic_Debug
#define MACRO_Dynamic_Debug
#endif

#ifndef MACRO_Windows_Debug
#define MACRO_Windows_Debug
#endif

#endif

#endif

//***********************

// New

#if defined(_WIN32) || defined(_WIN32_WCE)

#define MACRO_New new

#else

#define MACRO_New new(MEM_Voice)

#endif

// MACRO_Log_Function

#ifdef MACRO_Dynamic_Debug

#if defined(_WIN32) || defined(_WIN32_WCE)

#define MACRO_Log_Function \
N_Vr::N_Asr::N_Impl::C_Log_Block o_Log_Block(__FUNCTION__);

#else

#define MACRO_Log_Function \
N_Vr::N_Asr::N_Impl::C_Log_Block o_Log_Block(__PRETTY_FUNCTION__);

#endif

#else

#define MACRO_Log_Function \
;

#endif

// MACRO_Log_Error

#ifdef MACRO_Dynamic_Debug

#define MACRO_Log_Error                                                                 \
    static bool b_Error_Loged = false;                                                  \
    if (!b_Error_Loged) {                                                               \
        b_Error_Loged = true;                                                           \
        VR_ERROR(VOICE_VR_ERR_INVALID_PARAM, 0, "%s", "error");                         \
    }                                                                                   \
                                                                                        \
    N_Vr::N_Asr::N_Impl::Fn_Log_Error(__FUNCTION__, __FILE__, __LINE__); assert(0);

#else

#define MACRO_Log_Error \
;

#endif

// IF & ELSE

#ifdef MACRO_Dynamic_Debug

#define IF(x) if (x)

#else

extern
function<bool(bool, char const * const)> g_function_Check;

extern
bool
Fn_Check_Real
(
    bool b,
    char const * const pi_Function
);

extern
bool
Fn_Check
(
    bool b,
    char const * const pi_Function
);

#define IF(x) if (Fn_Check(static_cast<bool>(x), __FUNCTION__))

#endif

#define ELSE else

// PROTECTED & PRIVATE

#ifdef MACRO_Dynamic_Debug

#define PROTECTED protected

#define PRIVATE private

#else

#define PROTECTED public

#define PRIVATE public

#endif

// Vocon

#ifdef _NUANCE
#include "Vr_Asr_Impl_Vocon_Vocon.h"
#endif

// iFlyTek

#ifdef _IFLYTEK
#include "Vr_Asr_Impl_IFlytek_IFlytek.h"
#endif

// Interface

#include "Vr_Asr_Engine.h"
#include "Vr_Asr_Impl_Mock.h"
#include "Vr_Asr_Impl_Log.h"

#endif
/* EOF */
