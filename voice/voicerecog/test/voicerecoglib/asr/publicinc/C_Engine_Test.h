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
 * @file C_Engine_Test.h
 * @brief Declaration file of class C_Engine_Test.
 *
 * This file includes the declaration of class C_Engine_Test.
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef _C_Engine_Test_h__
#define _C_Engine_Test_h__

#if defined(_WIN32) || defined(_WIN32_WCE)
#else
#include <semaphore.h>
#endif

#include <map>
#include <list>
#include <string>
#include <fstream>

#include <boost/shared_ptr.hpp>

// #include "pugixml/pugixml.hpp"
// #include "File_Resolve_gtest.h"
// #include "Voicerecog_Result_Deal.h"

namespace N_Vr
{

namespace N_Asr
{

class C_Engine;

class C_Audio_In;

class C_Event_Phase;

class C_Event_Notify;

class C_Event_Result;

} // name N_Asr

} // name N_Vr

/**
 * @brief The C_Engine_Test class
 *
 * C_Engine_Test class
 */
class C_Engine_Test
{
public:
    C_Engine_Test();

    virtual ~C_Engine_Test();
public:
    int
    M_Fn_Initialize();

    int
    M_Fn_UnInitialize();

    void
    M_Fn_On_Event_Init_Notify
    (
        const N_Vr::N_Asr::C_Event_Init_Notify& EventNotifyType
    );

    void
    M_Fn_Recognize_On_Event_Phase
    (
        const N_Vr::N_Asr::C_Event_Phase& eventPhaseType
    );

    void
    M_Fn_Recognize_On_Event_Notify
    (
        const N_Vr::N_Asr::C_Event_Notify& EventNotifyType
    );

    void
    M_Fn_Recognize_On_Event_Result
    (
        const N_Vr::N_Asr::C_Event_Result& EventResultType
    );

    void
    M_Fn_Add_List_Context_On_Event_Phase
    (
        const N_Vr::N_Asr::C_Event_Phase& eventPhaseType
    );

    void
    M_Fn_Add_List_Context_On_Event_Notify
    (
        const N_Vr::N_Asr::C_Event_Notify& EventNotifyType
    );

    int
    M_Fn_Get_Directory_Files
    (
        const std::string&  string_path,
        std::set<std::string>& set_directory_files
    );

public:
    bool m_b_Context_Start;
    bool m_b_Rec_Start;

    unsigned int m_i_list_add_num;
    unsigned int m_i_list_add_complete;

private:
    C_Engine_Test(const C_Engine_Test&);

    void operator= (const C_Engine_Test&);
};

#endif //  _C_Engine_Test_h__
/* EOF */
