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

#include "Vr_Asr_Impl_Vocon_Vocon_Mock.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

C_Vocon_Mock::~C_Vocon_Mock()
{
}

C_Vocon_Mock::C_Vocon_Mock()
    : m_pvoid_Inst_Event(NULL)
    , i_Num_Trigger(0)
    , i_Pos_Trigger(0)
{
    vector_b_Param.resize(E_Param_Size, 0);
#if 0
    {
        m_vector_i_Confidence.push_back(5000);

        m_vector_o_LH_HYPOTHESIS.push_back(LH_HYPOTHESIS());

        LH_HYPOTHESIS & ro_LH_HYPOTHESIS = m_vector_o_LH_HYPOTHESIS.back();

        memset(&ro_LH_HYPOTHESIS, 0, sizeof(LH_HYPOTHESIS));

        ro_LH_HYPOTHESIS.conf = 5143;
        ro_LH_HYPOTHESIS.szStartRule = "grm_cmd_media_play_music#rul_cmd_media_play_music";

        m_vector_vector_o_LH_HYPOTHESIS_WORD_S.push_back(vector<LH_HYPOTHESIS_WORD_S>());

        vector<LH_HYPOTHESIS_WORD_S> & rvector_o_LH_HYPOTHESIS_WORD_S = m_vector_vector_o_LH_HYPOTHESIS_WORD_S.back();
        // 0
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_TRUE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 0;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 62;
            ro_LH_HYPOTHESIS_WORD_S.userID = 62;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "grm_cmd_media_play_music#CMN3012_media_music_x";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 1
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_TRUE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 0;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 2147483648;
            ro_LH_HYPOTHESIS_WORD_S.userID = 2147483648;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "grm_cmd_media_play_music#T_TEXT";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 2
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_TRUE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.userID = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "grm_cmd_media_play_music#rul_slt_media_play_music_list_2";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 3
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_FALSE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 0;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 163;
            ro_LH_HYPOTHESIS_WORD_S.userID = 163;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "Alan Wilson";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 4
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_TRUE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.userID = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "grm_cmd_media_play_music#rul_slt_media_play_music_list_2";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 5
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_TRUE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 0;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 2147483648;
            ro_LH_HYPOTHESIS_WORD_S.userID = 2147483648;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "grm_cmd_media_play_music#T_TEXT";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }

        ro_LH_HYPOTHESIS.nbrWords = rvector_o_LH_HYPOTHESIS_WORD_S.size();
        ro_LH_HYPOTHESIS.pWords = &rvector_o_LH_HYPOTHESIS_WORD_S[0];
    }
#endif
    {
        m_vector_i_Confidence.push_back(5000);

        m_vector_o_LH_HYPOTHESIS.push_back(LH_HYPOTHESIS());

        LH_HYPOTHESIS & ro_LH_HYPOTHESIS = m_vector_o_LH_HYPOTHESIS.back();

        memset(&ro_LH_HYPOTHESIS, 0, sizeof(LH_HYPOTHESIS));

        ro_LH_HYPOTHESIS.conf = 5143;
        ro_LH_HYPOTHESIS.szStartRule = "grm_cmd_phone_main#rul_cmd_phone_main";

        m_vector_vector_o_LH_HYPOTHESIS_WORD_S.push_back(vector<LH_HYPOTHESIS_WORD_S>());

        vector<LH_HYPOTHESIS_WORD_S> & rvector_o_LH_HYPOTHESIS_WORD_S = m_vector_vector_o_LH_HYPOTHESIS_WORD_S.back();
        // 0
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_TRUE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 0;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 97;
            ro_LH_HYPOTHESIS_WORD_S.userID = 97;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "grm_cmd_phone_main#CMN4011_phone_contact_call_x_type_x";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 1
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_FALSE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.userID = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "call";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 2
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_TRUE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 0;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 2147483648;
            ro_LH_HYPOTHESIS_WORD_S.userID = 2147483648;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "grm_cmd_media_play_music#T_TEXT";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 3
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_TRUE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.userID = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "grm_cmd_phone_main#rul_slt_phone_contact_name_list_2";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 4
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_FALSE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 0;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 1;
            ro_LH_HYPOTHESIS_WORD_S.userID = 163;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "Mike";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 5
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_TRUE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.userID = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "grm_cmd_phone_main#rul_slt_phone_contact_name_list_2";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 6
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_TRUE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 0;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 2147483648;
            ro_LH_HYPOTHESIS_WORD_S.userID = 2147483648;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "grm_cmd_media_play_music#T_TEXT";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 7
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_FALSE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.userID = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "at";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 8
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_TRUE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 0;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 142;
            ro_LH_HYPOTHESIS_WORD_S.userID = 142;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "grm_cmd_phone_main#phone_contact_type_home";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 9
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_FALSE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.userID = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "home";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 10
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_TRUE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 0;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 142;
            ro_LH_HYPOTHESIS_WORD_S.userID = 142;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "grm_cmd_phone_main#phone_contact_type_home";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }

        ro_LH_HYPOTHESIS.nbrWords = rvector_o_LH_HYPOTHESIS_WORD_S.size();
        ro_LH_HYPOTHESIS.pWords = &rvector_o_LH_HYPOTHESIS_WORD_S[0];
    }
#if 0
    {
        m_vector_i_Confidence.push_back(5000);

        m_vector_o_LH_HYPOTHESIS.push_back(LH_HYPOTHESIS());

        LH_HYPOTHESIS & ro_LH_HYPOTHESIS = m_vector_o_LH_HYPOTHESIS.back();

        memset(&ro_LH_HYPOTHESIS, 0, sizeof(LH_HYPOTHESIS));

        ro_LH_HYPOTHESIS.conf = 5143;
        ro_LH_HYPOTHESIS.szStartRule = "grm_fld_navi_address_oneshot#rul_fld_navi_address_oneshot";

        m_vector_vector_o_LH_HYPOTHESIS_WORD_S.push_back(vector<LH_HYPOTHESIS_WORD_S>());

        vector<LH_HYPOTHESIS_WORD_S> & rvector_o_LH_HYPOTHESIS_WORD_S = m_vector_vector_o_LH_HYPOTHESIS_WORD_S.back();
        // 0
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_TRUE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.userID = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "rul_slt_navi_address_oneshot_house";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 1
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_TRUE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 0;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 2147483648;
            ro_LH_HYPOTHESIS_WORD_S.userID = 2147483648;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "grm_def_navi_address_house_num_001#T_TEXT";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 2
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_TRUE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 0;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 2000000;
            ro_LH_HYPOTHESIS_WORD_S.userID = 2000000;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 3
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_TRUE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 0;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 2000000;
            ro_LH_HYPOTHESIS_WORD_S.userID = 2000000;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 4
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_FALSE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 0;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 1000001;
            ro_LH_HYPOTHESIS_WORD_S.userID = 1000001;
            ro_LH_HYPOTHESIS_WORD_S.szWord = NULL;
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 5
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_TRUE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 0;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 2000012;
            ro_LH_HYPOTHESIS_WORD_S.userID = 2000012;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 6
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_TRUE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 0;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 2000012;
            ro_LH_HYPOTHESIS_WORD_S.userID = 2000012;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 7
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_FALSE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 0;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 1000100;
            ro_LH_HYPOTHESIS_WORD_S.userID = 1000100;
            ro_LH_HYPOTHESIS_WORD_S.szWord = NULL;
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 8
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_TRUE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 0;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 2000001;
            ro_LH_HYPOTHESIS_WORD_S.userID = 2000001;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 9
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_TRUE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 0;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 2000001;
            ro_LH_HYPOTHESIS_WORD_S.userID = 2000001;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 10
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_TRUE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 0;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 2147483648;
            ro_LH_HYPOTHESIS_WORD_S.userID = 2147483648;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "grm_def_navi_address_house_num_001#T_TEXT";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }
        // 11
        {
            rvector_o_LH_HYPOTHESIS_WORD_S.push_back(LH_HYPOTHESIS_WORD_S());
            LH_HYPOTHESIS_WORD_S & ro_LH_HYPOTHESIS_WORD_S = rvector_o_LH_HYPOTHESIS_WORD_S.back();
            memset(&ro_LH_HYPOTHESIS_WORD_S, 0, sizeof(LH_HYPOTHESIS_WORD_S));
            ro_LH_HYPOTHESIS_WORD_S.isSlot = LH_TRUE;
            ro_LH_HYPOTHESIS_WORD_S.userID64.hi32 = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.userID64.lo32 = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.userID = 4294967295;
            ro_LH_HYPOTHESIS_WORD_S.szWord = "rul_slt_navi_address_oneshot_house";
            ro_LH_HYPOTHESIS_WORD_S.conf = 5000;
        }

        ro_LH_HYPOTHESIS.nbrWords = rvector_o_LH_HYPOTHESIS_WORD_S.size();
        ro_LH_HYPOTHESIS.pWords = &rvector_o_LH_HYPOTHESIS_WORD_S[0];
    }
#endif
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
