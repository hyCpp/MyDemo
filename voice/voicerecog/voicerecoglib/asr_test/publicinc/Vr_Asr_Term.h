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
 * @file Voice_Asr_Term.h
 * @brief Asr class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Term_h__
#define __Vr_Asr_Term_h__

#include "Vr_Asr_Buffer.h"

namespace N_Vr
{

namespace N_Asr
{

/**
 * @brief The C_Term class
 *
 * C_Term class
 */
class ASR_API C_Term
{
  public:
    vector<std::string> m_vector_string_Orthography; // Name: text and fuzzy context column
    vector<C_Buffer> m_vector_buffer_Transcriptions; // For voice tag and fuzzy context
    unsigned int m_i_User_Data_Hi; // User Id (High) Ignore so far
    unsigned int m_i_User_Data_Lo; // User Id (Low) and fuzzy context column attribute
    unsigned int m_i_Id_Trans_Hi; // Transcription Id (High)
    unsigned int m_i_Id_Trans_Lo; // Transcription Id (Low)
    int m_b_Use_Id_Trans; // Use Transcription Id
    std::string m_string_Trans_Category;   // for clc type

    ~C_Term() // Destructor
    {
    }

    explicit
    C_Term() // Constructor of Default
    : m_vector_string_Orthography()
    , m_vector_buffer_Transcriptions()
    , m_i_User_Data_Hi(0)
    , m_i_User_Data_Lo(0)
    , m_i_Id_Trans_Hi(0)
    , m_i_Id_Trans_Lo(0)
    , m_b_Use_Id_Trans(false)
    , m_string_Trans_Category()
    {
    }
};

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
