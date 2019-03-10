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
 * @file Vr_Asr_Buffer.h
 * @brief Asr class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Buffer_h__
#define __Vr_Asr_Buffer_h__
#include "Vr_Asr_Def.h"

namespace N_Vr
{

namespace N_Asr
{

/**
 * @brief The C_Buffer class
 *
 * C_Buffer class
 */
class C_Buffer
{
  public:

    size_t m_i_Size; // Buffer Size
    boost::shared_array<unsigned char> m_sai_Buffer; // For voice tag

    ~C_Buffer() // Destructor
    {
    }

    explicit
    C_Buffer() // Constructor of Default
    : m_i_Size(0)
    , m_sai_Buffer()
    {
    }
};

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
