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
 * @file Voice_Asr_Request_Context_List_Update.h
 * @brief C_Request_Context_List_Update class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Request_Context_List_Update_h__
#define __Vr_Asr_Request_Context_List_Update_h__

#include "Vr_Asr_Term.h"
#include "Vr_Asr_Event_Phase.h"
#include "Vr_Asr_Event_Notify.h"

namespace N_Vr
{

namespace N_Asr
{

/**
 * @brief The C_Request_Context_List_Update class
 *
 * C_Request_Context_List_Update class
 */

class ASR_API C_Request_Context_List_Update
{
  public:
    E_Data_Request_Type m_e_Requst_Type; // [I] request type

    // for nuance
    std::string m_string_Id_Party; // [I] Party id
    std::string m_string_Id_Context; // [I] Context id
    std::string m_string_Id_Category; // [I] Category id
    list<boost::shared_ptr<C_Term> > m_list_spo_Term_For_Delete; // [I] Terms
    list<boost::shared_ptr<C_Term> > m_list_spo_Term_For_Add; // [I] Terms
    int m_b_Clear; // [I] Clear all first.
    int m_b_Cancel; // [I] Cancel.
    unsigned int m_i_Id_Request; // [IO] Request ID.
    int m_b_Save_List_Context; // [I] Control Flag: ture -- save list context. default: ture
    unsigned int m_i_Priority_Id; // [I] priority id for update context: m_i_Priority_Id lagger and then lower priority
    std::string m_string_Transcription_Id_Version; // [I] transcription id version

    // common
    boost::function<void(C_Event_Phase const &)> m_function_On_Event_Phase;
    boost::function<void(C_Event_Notify const &)> m_function_On_Event_Notify;

    ~C_Request_Context_List_Update() // [F]Destructor
    {
    }

    explicit
    C_Request_Context_List_Update() // [F]Constructor of Default
    : m_e_Requst_Type(E_Data_Request_Type_Context_List_Update)
    , m_string_Id_Party()
    , m_string_Id_Context()
    , m_string_Id_Category()
    , m_list_spo_Term_For_Delete()
    , m_list_spo_Term_For_Add()
    , m_b_Clear(false)
    , m_b_Cancel(false)
    , m_i_Id_Request(0)
    , m_b_Save_List_Context(true)
    , m_i_Priority_Id(static_cast<unsigned int>(-1))
    , m_string_Transcription_Id_Version()
    , m_function_On_Event_Phase()
    , m_function_On_Event_Notify()
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
