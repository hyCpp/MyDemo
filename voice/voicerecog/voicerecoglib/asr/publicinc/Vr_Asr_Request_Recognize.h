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
 * @file Voice_Asr_Request_Recognize.h
 * @brief C_Request_Recognize class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Request_Recognize_h__
#define __Vr_Asr_Request_Recognize_h__

#include "Vr_Asr_Event_Phase.h"
#include "Vr_Asr_Event_Notify.h"
#include "Vr_Asr_Event_Result.h"

namespace N_Vr
{

namespace N_Asr
{

/**
 * @brief The C_Request_Recognize class
 *
 * C_Request_Recognize class
 */

class ASR_API C_Request_Recognize
{
  public:
    // for nanuce
    std::string m_string_Id_Party;
    list<boost::shared_ptr<C_Request_Activate> > m_list_spo_Request_Activate;
    int m_b_Barge_In;
    int m_b_Measure_Time;
    int m_b_Context_Check;
    unsigned long m_i_Voicetag_Valid_Score; // check voicetag similarity: 0 ~ 100
    std::string m_string_Voicetag_Context_Id;

    // for iFkyTek
    std::string m_string_Id_Mode;
    std::string m_string_Longtitude;
    std::string m_string_Latitude;
    std::string m_string_City;
    std::string m_string_Custom;
    list<boost::shared_ptr<C_Term> > m_list_spo_Term_Content_Local_Cmd;

    // common
    std::string m_string_Id_Dialog;   // nuance: dialog, iflytek: sence
    boost::shared_ptr<C_Audio_In> m_spo_Audio_In;
    boost::function<void(C_Event_Phase const &)> m_function_On_Event_Phase;
    boost::function<void(C_Event_Notify const &)> m_function_On_Event_Notify;
    boost::function<void(C_Event_Result const &)> m_function_On_Event_Result;


    ~C_Request_Recognize() // [F]Destructor
    {
    }

    explicit
    C_Request_Recognize() // [F]Constructor of Default
    : m_string_Id_Party()
    , m_list_spo_Request_Activate()
    , m_b_Barge_In(false)
    , m_b_Measure_Time(false)
    , m_b_Context_Check(false)
    , m_i_Voicetag_Valid_Score(0)
    , m_string_Voicetag_Context_Id()
    , m_string_Id_Mode()
    , m_string_Longtitude()
    , m_string_Latitude()
    , m_string_City()
    , m_string_Custom()
    , m_list_spo_Term_Content_Local_Cmd()
    , m_string_Id_Dialog()
    , m_spo_Audio_In()
    , m_function_On_Event_Phase()
    , m_function_On_Event_Notify()
    , m_function_On_Event_Result()
    {
    }

    // [F]Constructor: Copy
    //    = default

    // [F]Operator: =
    //    = default
};

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
