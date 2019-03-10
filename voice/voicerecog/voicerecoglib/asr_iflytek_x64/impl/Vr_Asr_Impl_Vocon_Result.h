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
 * @file Voice_Asr_Impl_Result.h
 * @brief C_Result class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Result_h__
#define __Vr_Asr_Impl_Result_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

/**
 * @brief The C_Result_Id_User class
 *
 * C_Result_Id_User class
 */
class C_Result_Id_User
{
  public:

    unsigned int m_i_Hi;
    unsigned int m_i_Lo;

    C_Result_Id_User()
    : m_i_Hi(0)
    , m_i_Lo(0)
    {
    }
};

/**
 * @brief The C_Result_Tag class
 *
 * C_Result_Tag class
 */
class C_Result_Tag
{
  public:

    C_Result_Id_User m_o_Result_Id_User;
    string m_string_Name;
    vector<C_Result_Id_User> m_vector_o_Result_Id_User_Terminal;
    vector<string> m_vector_string_Orthography_Terminal;

    C_Result_Tag()
    : m_o_Result_Id_User()
    , m_string_Name()
    , m_vector_o_Result_Id_User_Terminal()
    , m_vector_string_Orthography_Terminal()
    {
    }
};

/**
 * @brief The C_Result_Item class
 *
 * C_Result_Item class
 */
class C_Result_Item
{
  public:

    int m_b_Is_Slot;
    unsigned long m_i_Confidence;
    unsigned long m_i_Score;
    C_Result_Id_User m_o_Result_Id_User;
    string m_string_Name;
    vector<shared_ptr<C_Result_Item> > m_vector_spo_Result_Item_Tree;

    C_Result_Item()
    : m_b_Is_Slot(false)
    , m_i_Confidence(0)
    , m_i_Score(0)
    , m_o_Result_Id_User()
    , m_string_Name()
    , m_vector_spo_Result_Item_Tree()
    {
    }

    string
    Get_string_Terminal_Orthography();

    string
    Get_string_Eval();

    C_Result_Item *
    Get_po_Result_Item_Terminal_By_Pos
    (
        size_t i_Pos_Terminal_Target,
        size_t & ri_Pos_Terminal_Curr
    );

    C_Result_Item *
    Get_po_Result_Item_Terminal_By_Code
    (
        unsigned int i_Code_Terminal_Min,
        unsigned int i_Code_Terminal_Max
    );
};

/**
 * @brief The C_Result_Hypothesis class
 *
 * C_Result_Hypothesis class
 */
class C_Result_Hypothesis
{
  public:

    unsigned long m_i_Confidence;
    string m_string_Rule_Start;
    shared_ptr<C_Result_Item> m_spo_Result_Item_Tree;
    vector<shared_ptr<C_Result_Item> > m_vector_spo_Result_Item_Flat_Slot;
    vector<C_Result_Tag> m_vector_o_Result_Tag;
    vector<C_Result_Id_User> m_vector_o_Result_Id_User_Terminal;
    vector<string> m_vector_string_Orthography_Terminal;

    C_Result_Hypothesis()
    : m_i_Confidence(0)
    , m_string_Rule_Start()
    , m_spo_Result_Item_Tree()
    , m_vector_spo_Result_Item_Flat_Slot()
    , m_vector_o_Result_Tag()
    , m_vector_o_Result_Id_User_Terminal()
    , m_vector_string_Orthography_Terminal()
    {
    }

    C_Result_Item *
    Get_po_Result_Item_Tag_By_Pos
    (
        size_t i_Pos_Tag
    );

    C_Result_Item *
    Get_po_Result_Item_Tag_By_Code
    (
        unsigned int i_Code_Tag_Min,
        unsigned int i_Code_Tag_Max
    );
};

bool
Result_Hypothesis_Compare
(
    shared_ptr<C_Result_Hypothesis> const & rspo_Result_Hypothesis_A,
    shared_ptr<C_Result_Hypothesis> const & rspo_Result_Hypothesis_B
);

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */