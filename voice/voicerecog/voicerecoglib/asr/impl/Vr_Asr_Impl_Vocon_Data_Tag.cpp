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
#include "Vr_Asr_Impl_Vocon_Data_Node_Result.h"
#include "Vr_Asr_Impl_Vocon_Data_Tag.h"


namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

C_Data_Tag::~C_Data_Tag()
{
}

C_Data_Tag::C_Data_Tag
(
    C_Data_Party & ro_Data_Party
)
: m_ro_Data_Party(ro_Data_Party)
, m_list_spo_Data_Node_Result()
, m_i_Pos_Tag(0)
, m_string_Id_Tag()
{
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */