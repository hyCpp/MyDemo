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
#include "Vr_Asr_Impl_Event_Result_Impl.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

C_Event_Result_Impl::~C_Event_Result_Impl()
{
}

C_Event_Result_Impl::C_Event_Result_Impl()
: m_spstring_Result_XML()
, m_spstring_Result_Json()
, m_buffer_Userword_Transcription()
, m_buffer_Userword_Audio()
{
}

shared_ptr<string const>
C_Event_Result_Impl::Get_XML() const
{
    return const_pointer_cast<string const>(m_spstring_Result_XML);
}


shared_ptr<string const>
C_Event_Result_Impl::Get_Json() const
{
    return const_pointer_cast<string const>(m_spstring_Result_Json);
}

C_Buffer
C_Event_Result_Impl::Get_Buffer_Userword_Transcription() const
{
    return m_buffer_Userword_Transcription;
}

C_Buffer
C_Event_Result_Impl::Get_Buffer_Userword_Audio() const
{
    return m_buffer_Userword_Audio;
}

} // name N_Impl

C_Event_Result::~C_Event_Result()
{
}

C_Event_Result::C_Event_Result()
{
}

} // name N_Asr

} // name N_Vr

/* EOF */