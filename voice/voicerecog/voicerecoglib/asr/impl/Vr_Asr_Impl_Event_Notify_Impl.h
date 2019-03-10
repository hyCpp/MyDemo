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
 * @file Voice_Asr_Impl_Event_Notify_Impl.h
 * @brief Asr class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Event_Notify_Impl_h__
#define __Vr_Asr_Impl_Event_Notify_Impl_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

/**
 * @brief The C_Event_Notify_Impl class
 *
 * C_Event_Notify_Impl class
 */
class C_Event_Notify_Impl : public C_Event_Notify
{
  public:

    E_Event_Notify_Type m_e_Event_Notify_Type;

    virtual
    ~C_Event_Notify_Impl(); // Destructor

    explicit
    C_Event_Notify_Impl // Constructor of Default
    (
        E_Event_Notify_Type e_Event_Notify_Type
    );

    virtual
    E_Event_Notify_Type // [R] Result string XML.
    Get_Event_Notify_Type() const; // [F] Get result.

    virtual
    int // [R] Result int Signal Levels
    Get_Signal_Level_Energy() const; // [F] Get signal levels

    virtual
    int // [R] Result int Signal Levels
    Get_Signal_Level_SNR() const; // [F] Get signal levels

  PROTECTED:

    C_Event_Notify_Impl // Constructor: Copy
    (
        C_Event_Notify_Impl const & // [I]
    );

    C_Event_Notify_Impl & // [R]
    operator = // Operator: =
    (
        C_Event_Notify_Impl const & // [I]
    );
};

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
