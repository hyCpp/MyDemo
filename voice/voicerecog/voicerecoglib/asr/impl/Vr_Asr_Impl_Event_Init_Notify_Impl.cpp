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
#include "Vr_Asr_Impl_Event_Init_Notify_Impl.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

C_Event_Init_Notify_Impl::~C_Event_Init_Notify_Impl()
{
}

C_Event_Init_Notify_Impl::C_Event_Init_Notify_Impl
(
    E_Event_Init_Type e_Event_Init_Type,
    string& ro_string_Param
)
: m_e_Event_Init_Type(e_Event_Init_Type)
, m_string_Param(ro_string_Param)
{
}

C_Event_Init_Notify::E_Event_Init_Type
C_Event_Init_Notify_Impl::Get_Event_Init_Notify_Type() const
{
    return m_e_Event_Init_Type;
}

string
C_Event_Init_Notify_Impl::Get_Event_Init_Notify_Param_String() const
{
    return m_string_Param;
}

} // name N_Impl

C_Event_Init_Notify::~C_Event_Init_Notify()
{
}

C_Event_Init_Notify::C_Event_Init_Notify()
{
}

} // name N_Asr

} // name N_Vr

/* EOF */
