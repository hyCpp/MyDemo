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
#include "Vr_Asr_Impl_Event_Notify_Impl_Context_File_Size.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

C_Event_Notify_Impl_Context_File_Size::~C_Event_Notify_Impl_Context_File_Size()
{
}

C_Event_Notify_Impl_Context_File_Size::C_Event_Notify_Impl_Context_File_Size
(
    unsigned int i_File_Size
)
: m_i_File_Size(i_File_Size)
{
}

C_Event_Notify::E_Event_Notify_Type
C_Event_Notify_Impl_Context_File_Size::Get_Event_Notify_Type() const
{
    return C_Event_Notify::E_Event_Notify_Type_Context_List_File_Size;
}

unsigned int // [r] result unsigned int context file size
C_Event_Notify_Impl_Context_File_Size::Get_Context_File_Size() const
{
    return m_i_File_Size;
}


} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
