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
 * @file Vr_Asr_Audio_In.h
 * @brief class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Audio_In_h__
#define __Vr_Asr_Audio_In_h__

#include "Vr_Asr_Def.h"

namespace N_Vr
{

namespace N_Asr
{

/**
 * @brief The C_Audio_In class
 *
 * C_Audio_In class
 */
class ASR_API C_Audio_In
{
  public:

    virtual
    ~C_Audio_In() // [F]Destructor
    {
    }

    virtual
    void
    M_Fn_Get_Data_Type_Audio_SSE
    (
        void ** ppvoid_Data_Type_Audio_SSE,
        size_t * pi_Size_Data_Type_Audio_SSE
    )
    {
    }

    virtual
    int // [R] 0 for succeed, 1 for failed.
    M_Fn_Start() = 0;

    virtual
    int // [R] 0 for succeed, 1 for failed.
    M_Fn_Stop() = 0;

    virtual
    int // [R] 0 for succeed, 1 for failed.
    M_Fn_Data_Available_Check
    (
        bool * pb_Data_Available
    ) = 0;

    virtual
    int // [R] 0 for succeed, 1 for failed.
    M_Fn_Data_Fetch
    (
        void ** ppvoid_Buffer,
        size_t * pi_Size
    ) = 0;

    virtual
    int // [R] 0 for succeed, 1 for failed.
    M_Fn_Data_Return
    (
        void * const & rpvoid_Buffer
    ) = 0;
};

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
