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
 * @file Voice_Asr_Impl_IFlyTek_Thread_Data.h
 * @brief Asr class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_IFlyTek_Thread_Callback_Mock_h__
#define __Vr_Asr_Impl_IFlyTek_Thread_Callback_Mock_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_IFlyTek
{

class C_IFlyTek_Callback_Mock;

/**
 * @brief The C_Thread_Data class
 *
 * C_Thread_Data class
 */
class C_IFlytek_Thread_Callback_Mock : public BL_Thread
{
public:

    C_IFlyTek_Callback_Mock & m_ro_IFlyTek_Callback_Mock;

    virtual
    void
    Run(); // Operation

    virtual
    ~C_IFlytek_Thread_Callback_Mock(); // Destructor

    explicit
    C_IFlytek_Thread_Callback_Mock
    (
        C_IFlyTek_Callback_Mock & ro_IFlyTek_Callback_Mock
    ); // Constructor of default

protected:

    C_IFlytek_Thread_Callback_Mock // Constructor of copy
    (
        C_IFlytek_Thread_Callback_Mock const &
    );

    C_IFlytek_Thread_Callback_Mock &
    operator = // Operator: =
    (
        C_IFlytek_Thread_Callback_Mock const &
    );
};

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
