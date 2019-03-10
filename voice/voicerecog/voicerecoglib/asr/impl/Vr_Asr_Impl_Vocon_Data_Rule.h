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
 * @file Voice_Asr_Impl_Data_Rule.h
 * @brief C_Data_Rule class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Data_Rule_h__
#define __Vr_Asr_Impl_Data_Rule_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

class C_Data_Node_Result;
class C_Data_Party;

/**
 * @brief The C_Data_Rule class
 *
 * C_Data_Rule class
 */

class C_Data_Rule
{
  public:

    C_Data_Party & m_ro_Data_Party;
    list<shared_ptr<C_Data_Node_Result> > m_list_spo_Data_Node_Result;
    string m_string_Type_Rule;
    string m_string_Id_Rule;

    ~C_Data_Rule(); // [F]Destructor

    explicit
    C_Data_Rule // [F]Constructor of Default
    (
        C_Data_Party & ro_Data_Party
    );

  PROTECTED:

    C_Data_Rule // [F]Constructor: Copy
    (
        C_Data_Rule const & // [I]
    );

    C_Data_Rule & // [R]
    operator = // [F]Operator: =
    (
        C_Data_Rule const & // [I]
    );
};

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */