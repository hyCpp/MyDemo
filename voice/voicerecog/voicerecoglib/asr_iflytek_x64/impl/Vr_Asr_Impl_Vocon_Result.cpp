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
#include "Vr_Asr_Impl_Vocon_Result.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

string
C_Result_Item::Get_string_Terminal_Orthography()
{
    string string_Return;

    IF (m_b_Is_Slot) {

        vector<shared_ptr<C_Result_Item> >::iterator iterator_spo_Result_Item_End = m_vector_spo_Result_Item_Tree.end();

        for (
            vector<shared_ptr<C_Result_Item> >::iterator iterator_spo_Result_Item_Curr = m_vector_spo_Result_Item_Tree.begin();
            iterator_spo_Result_Item_Curr != iterator_spo_Result_Item_End;
            ++iterator_spo_Result_Item_Curr
        ) {
            shared_ptr<C_Result_Item> & rspo_Result_Item_Curr = *iterator_spo_Result_Item_Curr;
            IF (!rspo_Result_Item_Curr) {
                continue;
            }

            IF (!string_Return.empty()) {
                string_Return += ' ';
            }

            string_Return += rspo_Result_Item_Curr->Get_string_Terminal_Orthography();
        }
    }
    ELSE {
        string_Return = m_string_Name;
    }

    return string_Return;
}

string
C_Result_Item::Get_string_Eval()
{
    string string_Return;
#ifdef MACRO_Dynamic_Debug

    IF (m_b_Is_Slot) {

        vector<shared_ptr<C_Result_Item> >::iterator iterator_spo_Result_Item_End = m_vector_spo_Result_Item_Tree.end();

        for (
            vector<shared_ptr<C_Result_Item> >::iterator iterator_spo_Result_Item_Curr = m_vector_spo_Result_Item_Tree.begin();
            iterator_spo_Result_Item_Curr != iterator_spo_Result_Item_End;
            ++iterator_spo_Result_Item_Curr
        ) {
            shared_ptr<C_Result_Item> & rspo_Result_Item_Curr = *iterator_spo_Result_Item_Curr;
            IF (!rspo_Result_Item_Curr) {
                continue;
            }

            stringstream stringstream_Temp;

            unsigned int i_Code = rspo_Result_Item_Curr->m_o_Result_Id_User.m_i_Lo;

            switch (i_Code) {
            case 2000000: {

                stringstream_Temp << '{';
                break;
            }
            case 2000001: {

                stringstream_Temp << '}';
                break;
            }
            case 2000002: {

                stringstream_Temp << '(';
                break;
            }
            case 2000003: {

                stringstream_Temp << ')';
                break;
            }
            case 2000010: {

                stringstream_Temp << '+';
                break;
            }
            case 2000012: {

                stringstream_Temp << '*';
                break;
            }
            default: {

                IF (i_Code < 256) {

                    char i_Char = static_cast<char>(i_Code);

                    stringstream_Temp << i_Char;
                }
                ELSE
                IF ((i_Code >= 1000000) && (i_Code < 2000000)) {

                    int i_Value = i_Code - 1000000;

                    stringstream_Temp << i_Value;
                }
            }
            }

            string_Return += stringstream_Temp.str();

            string_Return += rspo_Result_Item_Curr->Get_string_Eval();
        }
    }
#endif
    return string_Return;
}

C_Result_Item *
C_Result_Item::Get_po_Result_Item_Terminal_By_Pos
(
    size_t i_Pos_Terminal_Target,
    size_t & ri_Pos_Terminal_Curr
)
{
    for (
        size_t i_Pos_Result_Item_Curr = 0;
        i_Pos_Result_Item_Curr < m_vector_spo_Result_Item_Tree.size();
        ++i_Pos_Result_Item_Curr
    ) {

        shared_ptr<C_Result_Item> & rspo_Result_Item_Tree = m_vector_spo_Result_Item_Tree[i_Pos_Result_Item_Curr];

        IF (rspo_Result_Item_Tree->m_b_Is_Slot) {

            C_Result_Item * po_Result_Item_Terminal = rspo_Result_Item_Tree->Get_po_Result_Item_Terminal_By_Pos(
                i_Pos_Terminal_Target,
                ri_Pos_Terminal_Curr
            );

            IF (po_Result_Item_Terminal) {
                return po_Result_Item_Terminal;
            }
        }
        ELSE {

            IF (ri_Pos_Terminal_Curr == i_Pos_Terminal_Target) {
                return rspo_Result_Item_Tree.get();
            }

            ++ri_Pos_Terminal_Curr;
        }
    }

    RETURN_NULL;
}

C_Result_Item *
C_Result_Item::Get_po_Result_Item_Terminal_By_Code
(
    unsigned int i_Code_Terminal_Min,
    unsigned int i_Code_Terminal_Max
)
{
#ifdef MACRO_Dynamic_Debug
    for (
        size_t i_Pos_Result_Item_Curr = 0;
        i_Pos_Result_Item_Curr < m_vector_spo_Result_Item_Tree.size();
        ++i_Pos_Result_Item_Curr
    ) {

        shared_ptr<C_Result_Item> & rspo_Result_Item_Tree = m_vector_spo_Result_Item_Tree[i_Pos_Result_Item_Curr];

        IF (rspo_Result_Item_Tree->m_b_Is_Slot) {

            C_Result_Item * po_Result_Item_Terminal = rspo_Result_Item_Tree->Get_po_Result_Item_Terminal_By_Code(
                i_Code_Terminal_Min,
                i_Code_Terminal_Max
            );

            IF (po_Result_Item_Terminal) {
                return po_Result_Item_Terminal;
            }
        }
        ELSE {

            unsigned int i_Code_Terminal = rspo_Result_Item_Tree->m_o_Result_Id_User.m_i_Lo;

            IF ((i_Code_Terminal >= i_Code_Terminal_Min) && (i_Code_Terminal <= i_Code_Terminal_Max)) {

                return rspo_Result_Item_Tree.get();
            }
        }
    }
#endif
    RETURN_NULL;
}

C_Result_Item *
C_Result_Hypothesis::Get_po_Result_Item_Tag_By_Pos
(
    size_t i_Pos_Tag
)
{
    stringstream stringstream_Log;

    IF (i_Pos_Tag >= m_vector_spo_Result_Item_Flat_Slot.size()) {
        stringstream_Log << "    No Result Item Tag Pos: " << i_Pos_Tag;
        Fn_Log(stringstream_Log.str());
        
        MACRO_Log_Error
        RETURN_NULL;
    }

    return m_vector_spo_Result_Item_Flat_Slot[i_Pos_Tag].get();
}

C_Result_Item *
C_Result_Hypothesis::Get_po_Result_Item_Tag_By_Code
(
    unsigned int i_Code_Tag_Min,
    unsigned int i_Code_Tag_Max
)
{
    stringstream stringstream_Log;
    
    size_t i_Num = m_vector_spo_Result_Item_Flat_Slot.size();

    for (
        size_t i_Pos = 0;
        i_Pos < i_Num;
        ++i_Pos
    ) {

        shared_ptr<C_Result_Item> & rspo_Result_Item_Curr = m_vector_spo_Result_Item_Flat_Slot[i_Pos];
        IF (!rspo_Result_Item_Curr) {
            stringstream_Log.clear();
            stringstream_Log.str("");
            stringstream_Log << "    No Result Item Tag Code Pos: " << i_Pos;
            Fn_Log(stringstream_Log.str());
            
            MACRO_Log_Error
            continue;
        }

        unsigned int i_Code_Tag = rspo_Result_Item_Curr->m_o_Result_Id_User.m_i_Lo;

        IF ((i_Code_Tag >= i_Code_Tag_Min) && (i_Code_Tag <= i_Code_Tag_Max)) {

            return rspo_Result_Item_Curr.get();
        }
    }

    stringstream_Log.clear();
    stringstream_Log.str("");
    stringstream_Log << "    No Result Item, Code: Min(" << i_Code_Tag_Min << "), Max(" << i_Code_Tag_Max << ")";
    Fn_Log(stringstream_Log.str());

    RETURN_NULL;
}

bool
Result_Hypothesis_Compare
(
    shared_ptr<C_Result_Hypothesis> const & rspo_Result_Hypothesis_A,
    shared_ptr<C_Result_Hypothesis> const & rspo_Result_Hypothesis_B
)
{
    return rspo_Result_Hypothesis_A->m_i_Confidence < rspo_Result_Hypothesis_B->m_i_Confidence;
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */