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
#include "Vr_Asr_Impl_Event_Phase_Impl.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

C_Event_Phase_Impl::~C_Event_Phase_Impl()
{
}

C_Event_Phase_Impl::C_Event_Phase_Impl
(
    E_Event_Phase_Type e_Event_Phase_Type
)
: m_e_Event_Phase_Type(e_Event_Phase_Type)
{
}

C_Event_Phase::E_Event_Phase_Type
C_Event_Phase_Impl::Get_Event_Phase_Type() const
{
    return m_e_Event_Phase_Type;
}

} // name N_Impl

C_Event_Phase::~C_Event_Phase()
{
}

C_Event_Phase::C_Event_Phase()
{
}

} // name N_Asr

} // name N_Vr

/* EOF */