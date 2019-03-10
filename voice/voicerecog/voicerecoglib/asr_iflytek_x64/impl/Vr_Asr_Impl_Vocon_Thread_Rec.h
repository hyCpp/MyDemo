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
 * @file Voice_Asr_Impl_Thread_Rec.h
 * @brief Asr class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Thread_Rec_h__
#define __Vr_Asr_Impl_Thread_Rec_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

class C_Mgr_Rec;

/**
 * @brief The C_Thread_Rec class
 *
 * C_Thread_Rec class
 */
class C_Thread_Rec : public BL_Thread
{
  public:

    C_Mgr_Rec & m_ro_Mgr_Rec;

    virtual
    void
    Run(); // Operation

    virtual
    ~C_Thread_Rec(); // Destructor

    explicit
    C_Thread_Rec
    (
        C_Mgr_Rec & ro_Mgr_Rec
    ); // Constructor of default

  PROTECTED:

    C_Thread_Rec // Constructor of copy
    (
        C_Thread_Rec const &
    );

    C_Thread_Rec &
    operator = // Operator: =
    (
        C_Thread_Rec const &
    );
};

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
