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

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

C_Data_Node_Result::~C_Data_Node_Result()
{
}

C_Data_Node_Result::C_Data_Node_Result()
: m_list_spo_Data_Node_Result()
, m_string_Name()
, m_e_Type_Value(E_Type_Value_None)
, m_e_Type_Terminal(E_Type_Terminal_None)
, m_e_Type_Tag(E_Type_Tag_None)
, m_i_Pos_Terminal(0)
, m_i_Pos_Tag(0)
, m_i_Code_Terminal_Min(0)
, m_i_Code_Terminal_Max(0)
, m_i_Code_Tag_Min(0)
, m_i_Code_Tag_Max(0)
, m_b_Optional(false)
, m_string_Value_Defined()
{
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */