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

#include "Vr_Asr_Impl_Afx.h"
#include "Vr_Asr_Impl_Event_Notify_Impl_Signal_Level.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

C_Event_Notify_Impl_Signal_Level::~C_Event_Notify_Impl_Signal_Level()
{
}

C_Event_Notify_Impl_Signal_Level::C_Event_Notify_Impl_Signal_Level
(
    int i_Signal_Level_Energy,
    int i_Signal_Level_SNR
)
:  m_i_Signal_Level_Energy(i_Signal_Level_Energy),
  m_i_Signal_Level_SNR(i_Signal_Level_SNR)
{
}

C_Event_Notify::E_Event_Notify_Type
C_Event_Notify_Impl_Signal_Level::Get_Event_Notify_Type() const
{
    return C_Event_Notify::E_Event_Notify_Type_Signal_Level;
}

int
C_Event_Notify_Impl_Signal_Level::Get_Signal_Level_Energy() const
{
    return m_i_Signal_Level_Energy;
}

int
C_Event_Notify_Impl_Signal_Level::Get_Signal_Level_SNR() const
{
    return m_i_Signal_Level_SNR;
}

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */