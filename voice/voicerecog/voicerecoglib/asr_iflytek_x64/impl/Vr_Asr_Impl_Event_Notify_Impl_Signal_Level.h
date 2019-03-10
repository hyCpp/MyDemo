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
 * @file Voice_Asr_Impl_Event_Notify_Impl_Signal_Level.h
 * @brief Asr class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Event_Notify_Impl_Signal_Level_h__
#define __Vr_Asr_Impl_Event_Notify_Impl_Signal_Level_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

/**
 * @brief The C_Event_Notify_Impl_Signal_Level class
 *
 * C_Event_Notify_Impl_Signal_Level class
 */
class C_Event_Notify_Impl_Signal_Level : public C_Event_Notify
{
  public:

    int m_i_Signal_Level_Energy;
    int m_i_Signal_Level_SNR;

    virtual
    ~C_Event_Notify_Impl_Signal_Level(); // Destructor

    explicit
    C_Event_Notify_Impl_Signal_Level // Constructor of Overload
    (
        int i_Signal_Level_Energy,
        int i_Signal_Level_SNR
    );

    virtual
    E_Event_Notify_Type // [R] Result string XML.
    Get_Event_Notify_Type() const; // [F] Get result.

    virtual
    int
    Get_Signal_Level_Energy() const;

    virtual
    int
    Get_Signal_Level_SNR() const;

  PROTECTED:

    C_Event_Notify_Impl_Signal_Level // Constructor: Copy
    (
        C_Event_Notify_Impl_Signal_Level const & // [I]
    );

    C_Event_Notify_Impl_Signal_Level & // [R]
    operator = // Operator: =
    (
        C_Event_Notify_Impl_Signal_Level const & // [I]
    );
};

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
