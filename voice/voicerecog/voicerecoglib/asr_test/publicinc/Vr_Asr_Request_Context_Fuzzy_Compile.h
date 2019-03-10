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
 * @file Voice_Asr_Request_Context_Fuzzy_Compile.h
 * @brief C_Request_Context_List_Update class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Request_Context_Fuzzy_Compile_h__
#define __Vr_Asr_Request_Context_Fuzzy_Compile_h__

#include "Vr_Asr_Term.h"
#include "Vr_Asr_Event_Phase.h"
#include "Vr_Asr_Event_Notify.h"

namespace N_Vr
{

namespace N_Asr
{

#if 0

/**
 * @brief The C_Request_Context_List_Update class
 *
 * C_Request_Context_List_Update class
 */

class ASR_API C_Request_Context_Fuzzy_Compile
{
  public:
    std::string m_string_Id_Party; // [I] Party id
    std::string m_string_Id_Context; // [I] Context id
    vector<boost::shared_ptr<C_Term> > m_vector_spo_Term_Column; // [I] Context Column and Attribute
    list<vector<boost::shared_ptr<C_Term> > > m_list_vector_spo_Term_Entry; // [I] Context Entry

    boost::function<void(C_Event_Phase const &)> m_function_On_Event_Phase;
    boost::function<void(C_Event_Notify const &)> m_function_On_Event_Notify;

    ~C_Request_Context_Fuzzy_Compile() // [F]Destructor
    {
    }

    explicit
    C_Request_Context_Fuzzy_Compile() // [F]Constructor of Default
    : m_string_Id_Party()
    , m_string_Id_Context()
    , m_vector_spo_Term_Column()
    , m_list_vector_spo_Term_Entry()
    , m_function_On_Event_Phase()
    , m_function_On_Event_Notify()
    {
    }

    // [F]Constructor: Copy
    //    = default

    // [F]Operator: =
    //    = default
};

#endif

} // name N_Asr

} // name N_Vr

#endif
/* EOF */