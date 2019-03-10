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
 * @file Voice_Asr_Impl_Data_Node_Result.h
 * @brief C_Data_Node_Result class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Data_Node_Result_h__
#define __Vr_Asr_Impl_Data_Node_Result_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

class C_Data_Party;

/**
 * @brief The C_Data_Node_Result class
 *
 * C_Data_Node_Result class
 */

class C_Data_Node_Result
{
  public:

    enum E_Type_Value
    {
        E_Type_Value_None = 0,
        E_Type_Value_Defined,
        E_Type_Value_Tag_Name,
        E_Type_Value_Tag_User_Id,
        E_Type_Value_Tag_Orthography,
        E_Type_Value_Tag_Eval,
        E_Type_Value_Terminal_Name,
        E_Type_Value_Terminal_User_Id,
        E_Type_Value_Terminal_Orthography,
    };

    enum E_Type_Terminal
    {
        E_Type_Terminal_None = 0,
        E_Type_Terminal_By_Pos,
        E_Type_Terminal_By_Code,
    };

    enum E_Type_Tag
    {
        E_Type_Tag_None = 0,
        E_Type_Tag_By_Pos,
        E_Type_Tag_By_Code,
    };

    list<shared_ptr<C_Data_Node_Result> > m_list_spo_Data_Node_Result;
    string m_string_Name;
    E_Type_Value m_e_Type_Value;
    E_Type_Terminal m_e_Type_Terminal;
    E_Type_Tag m_e_Type_Tag;
    size_t m_i_Pos_Terminal;
    size_t m_i_Pos_Tag;
    unsigned int m_i_Code_Terminal_Min;
    unsigned int m_i_Code_Terminal_Max;
    unsigned int m_i_Code_Tag_Min;
    unsigned int m_i_Code_Tag_Max;
    int m_b_Optional;
    string m_string_Value_Defined;

    ~C_Data_Node_Result(); // [F]Destructor

    explicit
    C_Data_Node_Result(); // [F]Constructor of Default

  PROTECTED:

    C_Data_Node_Result // [F]Constructor: Copy
    (
        C_Data_Node_Result const & // [I]
    );

    C_Data_Node_Result & // [R]
    operator = // [F]Operator: =
    (
        C_Data_Node_Result const & // [I]
    );
};

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */