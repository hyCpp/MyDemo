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
 * @file Vr_Asr_Audio_In_Mock.h
 * @brief Declaration file of class C_Audio_In_Mock.
 *
 * This file includes the declaration of class C_Audio_In_Mock.
 *
 * @attention used for C++ only.
 */

#ifndef _Vr_Asr_Audio_In_Mock_IFlyTek_h_
#define _Vr_Asr_Audio_In_Mock_IFlyTek_h_

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "Vr_Asr_Audio_In.h"

namespace N_Vr
{

namespace N_Asr
{

/**
 * @brief The C_Audio_In_Mock class
 *
 * C_Audio_In_Mock class
 */

class C_Audio_In_Mock_IFlyTek : public C_Audio_In
{
public:

    virtual
    ~C_Audio_In_Mock_IFlyTek() // [F]Destructor
    {
    }

    virtual
    int // [R] 0 for succeed, 1 for failed.
    M_Fn_Start()
    {
        return 0;
    }

    virtual
    int // [R] 0 for succeed, 1 for failed.
    M_Fn_Stop()
    {
        return 0;
    }

    virtual
    int // [R] 0 for succeed, 1 for failed.
    M_Fn_Data_Available_Check
    (
        bool * pb_Data_Available
    )
    {
        return 0;
    }

    virtual
    int // [R] 0 for succeed, 1 for failed.
    M_Fn_Data_Fetch
    (
        void ** ppvoid_Buffer,
        size_t * pi_Size
    )
    {
        return 0;
    }

    virtual
    int // [R] 0 for succeed, 1 for failed.
    M_Fn_Data_Return
    (
        void * const & rpvoid_Buffer
    )
    {
        return 0;
    }
};


} // namespace N_Asr

} // namespace N_Vr

#endif // #ifndef _Vr_Asr_Audio_In_Mock_h_
/* EOF */