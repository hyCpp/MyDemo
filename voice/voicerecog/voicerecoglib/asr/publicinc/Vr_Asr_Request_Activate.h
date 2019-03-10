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
 * @file Voice_Asr_Request_Activate.h
 * @brief C_Request_Activate class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Request_Activate_h__
#define __Vr_Asr_Request_Activate_h__

namespace N_Vr
{

namespace N_Asr
{

/**
 * @brief The C_Request_Activate class
 *
 * C_Request_Activate class
 */

class ASR_API C_Request_Activate
{
  public:

    std::string m_string_Id_Context; // [I] Context id

    // Common

    int m_b_Deactivate_all_then_activate; // [I] true: Deactivate all then activate; false: Activate all then deactivate.

    // For Grammar Context

    list<std::string> m_list_string_Id_Rule; // [I] Rule id

    // For Fuzzy Context
    std::string m_string_Query; // [I] All: empty string
    unsigned long m_i_Value; // [I] Activation Value : 0 ~ 10000 

    // For Field Context

    std::string m_string_Field_Start; // [I] Start Field
    std::string m_string_Field_Stop; // [I] Stop Field
    std::string m_string_Field_Activate_From; // [I] Activate From Field
    vector<unsigned int> m_vector_i_User_Data_Hi; // [I] User Id (High) Ignore so far
    vector<unsigned int> m_vector_i_User_Data_Lo; // [I] User Id (Low)

    ~C_Request_Activate() // [F]Destructor
    {
    }

    explicit
    C_Request_Activate() // [F]Constructor of Default
    : m_string_Id_Context()
    , m_b_Deactivate_all_then_activate(false)
    , m_list_string_Id_Rule()
    , m_string_Query()
    , m_i_Value(0)
    , m_string_Field_Start()
    , m_string_Field_Stop()
    , m_string_Field_Activate_From()
    , m_vector_i_User_Data_Hi()
    , m_vector_i_User_Data_Lo()
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