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


#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "Vr_Asr_Impl_Afx.h"
#include "Vr_Asr_Impl_IFlyTek_Engine_Concrete_Test.h"
#include "Vr_Asr_Impl_IFlyTek_Fn_Callback_Msg.h"
#include "Vr_Asr_Impl_IFlyTek_Engine_Concrete.h"
#include "Vr_Asr_Impl_Engine_Agent.h"
#include "Vr_Asr_Request_Factory.h"
#include "Vr_Asr_Audio_In_Mock.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

using ::testing::Return;
using ::testing::_;

namespace N_IFlyTek
{

/**
 * @brief The C_Engine_Fake class
 *
 * C_Engine_Fake class
 */

class C_Engine_Fake
{
public:
    void
    M_Fn_On_Event_Init_Notify(
        const N_Vr::N_Asr::C_Event_Init_Notify& EventNotifyType) {}

    void
    M_Fn_Add_List_Context_On_Event_Phase(
        const N_Vr::N_Asr::C_Event_Phase& eventPhaseType) {}

    void
    M_Fn_Add_List_Context_On_Event_Notify(
        const N_Vr::N_Asr::C_Event_Notify& EventNotifyType) {}

    void
    M_Fn_Recognize_On_Event_Result(
        const N_Vr::N_Asr::C_Event_Result& EventResultType) {}
};

C_Engine_Concrete_Test::C_Engine_Concrete_Test()
: m_p_Engine_Agent(NULL)
, m_p_Engine_IFlyTek(NULL)
{
}

C_Engine_Concrete_Test::~C_Engine_Concrete_Test()
{
}

void C_Engine_Concrete_Test::SetUp()
{
    C_Engine_Fake o_Engine_Fake;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Notify = boost::bind(&C_Engine_Fake::M_Fn_On_Event_Init_Notify, &o_Engine_Fake, _1);

    m_p_Engine_Agent = new C_Engine_Agent();
    m_p_Engine_IFlyTek = new C_Engine_Concrete(*m_p_Engine_Agent, "eng", function_On_Event_Notify, "none");
}

void C_Engine_Concrete_Test::TearDown()
{
    delete m_p_Engine_Agent;
    m_p_Engine_Agent = NULL;
    delete m_p_Engine_IFlyTek;
    m_p_Engine_IFlyTek = NULL;
}

TEST_F(C_Engine_Concrete_Test, IFlyTek_Engine)
{
    printf("_M_Fn_Get_Dic_Name_and_Src\n");
    {
        string string_Context_Id;
        string string_Dic_Name;
        string string_Dic_Src;

        string_Context_Id = "ctx_phone_contact_name_list_1";
        m_p_Engine_IFlyTek->_M_Fn_Get_Dic_Name_and_Src(string_Context_Id, string_Dic_Name, string_Dic_Src);

        string_Context_Id = "ctx_meida_play_album_list_1";
        m_p_Engine_IFlyTek->_M_Fn_Get_Dic_Name_and_Src(string_Context_Id, string_Dic_Name, string_Dic_Src);

        string_Context_Id = "ctx_media_play_playlist_list_1";
        m_p_Engine_IFlyTek->_M_Fn_Get_Dic_Name_and_Src(string_Context_Id, string_Dic_Name, string_Dic_Src);

        string_Context_Id = "ctx_media_play_artist_list_1";
        m_p_Engine_IFlyTek->_M_Fn_Get_Dic_Name_and_Src(string_Context_Id, string_Dic_Name, string_Dic_Src);
    }

    printf("M_Fn_Party_Load\n");
    {
        m_p_Engine_IFlyTek->M_Fn_Party_Load("xxxx");
    }

    printf("M_Fn_Party_Unload\n");
    {
        m_p_Engine_IFlyTek->M_Fn_Party_Unload("sss");
    }

    C_Request_Recognize o_Requeset_Recognize;
    C_Request_Param_Set o_Request_Param_Set;
    C_Request_Context_List_Update o_Request_Context_List_Update;
    C_Engine_Fake o_Engine_Fake;

    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    printf("M_Fn_Init\n");
    {
        o_Check.m_string_Name_Function = "M_Fn_Init";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Init();

        printf("----2----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Init();

        printf("----3----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Init();
        m_p_Engine_IFlyTek->_M_Fn_IFlyTek_Data_Fina();

        printf("----4----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Init();
        m_p_Engine_IFlyTek->_M_Fn_Mgr_Fina();
        m_p_Engine_IFlyTek->_M_Fn_IFlyTek_Data_Fina();
    }

    printf("M_Fn_Data_Context_List_Update\n");
    {
        o_Check.m_string_Name_Function = "M_Fn_Data_Context_List_Update";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Data_Context_List_Update(o_Request_Context_List_Update);

        printf("----2----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Data_Context_List_Update(o_Request_Context_List_Update);

        printf("----3----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Request_Context_List_Update.m_function_On_Event_Notify = boost::bind(&C_Engine_Fake::M_Fn_Add_List_Context_On_Event_Notify, &o_Engine_Fake, _1);
        o_Request_Context_List_Update.m_function_On_Event_Phase = boost::bind(&C_Engine_Fake::M_Fn_Add_List_Context_On_Event_Phase, &o_Engine_Fake, _1);
        m_p_Engine_IFlyTek->M_Fn_Data_Context_List_Update(o_Request_Context_List_Update);

        printf("----4----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Data_Context_List_Update(o_Request_Context_List_Update);

        printf("----5----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Data_Context_List_Update(o_Request_Context_List_Update);
    }

    printf("M_Fn_Rec_Start\n");
    {
        o_Check.m_string_Name_Function = "M_Fn_Rec_Start";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Rec_Start(o_Requeset_Recognize);

        printf("----2----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Rec_Start(o_Requeset_Recognize);

        printf("----3----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Rec_Start(o_Requeset_Recognize);
        m_p_Engine_IFlyTek->M_Fn_Fina();

        printf("----4----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Rec_Start(o_Requeset_Recognize);
        m_p_Engine_IFlyTek->M_Fn_Fina();

        printf("----5----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Rec_Start(o_Requeset_Recognize);

        printf("----6----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Rec_Start(o_Requeset_Recognize);

        printf("----7----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Rec_Start(o_Requeset_Recognize);

        printf("----8----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Rec_Start(o_Requeset_Recognize);

        printf("----9----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;                
        m_p_Engine_IFlyTek->M_Fn_Rec_Start(o_Requeset_Recognize);

        printf("----10----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true); 
        o_Check.m_vector_i_Check.push_back(true); 
        g_function_Check = o_Check;                
        m_p_Engine_IFlyTek->M_Fn_Rec_Start(o_Requeset_Recognize);

        printf("----11----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true); 
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Rec_Start(o_Requeset_Recognize);

        printf("----12----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Requeset_Recognize.m_function_On_Event_Notify = boost::bind(&C_Engine_Fake::M_Fn_Add_List_Context_On_Event_Notify, &o_Engine_Fake, _1);
        o_Requeset_Recognize.m_function_On_Event_Phase = boost::bind(&C_Engine_Fake::M_Fn_Add_List_Context_On_Event_Phase, &o_Engine_Fake, _1);
        o_Requeset_Recognize.m_function_On_Event_Result = boost::bind(&C_Engine_Fake::M_Fn_Recognize_On_Event_Result, &o_Engine_Fake, _1);
        m_p_Engine_IFlyTek->M_Fn_Rec_Start(o_Requeset_Recognize);

        printf("----13----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Rec_Start(o_Requeset_Recognize);

        printf("----14----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Rec_Start(o_Requeset_Recognize);

        printf("----15----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Rec_Start(o_Requeset_Recognize);
    }

    printf("M_Fn_Rec_Cancel\n");
    {
        o_Check.m_string_Name_Function = "M_Fn_Rec_Cancel";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Rec_Cancel();

        printf("----2----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Rec_Cancel();

        printf("----3----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Rec_Cancel();
    }

    printf("M_Fn_Param_Set\n");
    {
        o_Check.m_string_Name_Function = "M_Fn_Param_Set";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Param_Set(o_Request_Param_Set);

        printf("----2----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Param_Set(o_Request_Param_Set);

        printf("----3----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->M_Fn_Param_Set(o_Request_Param_Set);
    }

    printf("M_Fn_On_Callback_Msg\n");
    {
        m_p_Engine_IFlyTek->M_Fn_On_Callback_Msg(20001, 0, NULL);
        m_p_Engine_IFlyTek->M_Fn_On_Callback_Msg(20002, 0, NULL);
        shared_ptr<C_Request_Context_List_Update> spo_Request_Context_List_Update;
        m_p_Engine_IFlyTek->m_list_spo_Request_Context_List_Update.push_back(spo_Request_Context_List_Update);
        m_p_Engine_IFlyTek->M_Fn_On_Callback_Msg(20001, 0, NULL);
        m_p_Engine_IFlyTek->M_Fn_On_Callback_Msg(20002, 0, NULL);
        m_p_Engine_IFlyTek->m_list_spo_Request_Context_List_Update.clear();
        m_p_Engine_IFlyTek->m_list_o_Time_Context_list_Update.push_back(C_Time::M_Fn_Get_Time());
        m_p_Engine_IFlyTek->M_Fn_On_Callback_Msg(20001, 0, NULL);
        m_p_Engine_IFlyTek->M_Fn_On_Callback_Msg(20002, 0, NULL);
    }

    printf("_M_Fn_Start_Audio\n");
    {
        o_Check.m_string_Name_Function = "_M_Fn_Start_Audio";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Start_Audio();

        printf("----2----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Start_Audio();
    }

    printf("_M_Fn_Stop_Audio\n");
    {
        o_Check.m_string_Name_Function = "_M_Fn_Stop_Audio";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Stop_Audio();

        printf("----2----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Stop_Audio();
    }

    printf("_M_Fn_Mgr_Init\n");
    {
        o_Check.m_string_Name_Function = "_M_Fn_Mgr_Init";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Mgr_Init();
        m_p_Engine_IFlyTek->_M_Fn_Mgr_Fina();
        }

        printf("_M_Fn_Mgr_Fina\n");
        {
        o_Check.m_string_Name_Function = "_M_Fn_Mgr_Fina";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Mgr_Fina();
    }

    printf("_M_Fn_Get_IFlyTek_Mode\n");
    {
        ISS_SR_MODE o_Iss_Sr_Mode;
        o_Check.m_string_Name_Function = "_M_Fn_Get_IFlyTek_Mode";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Get_IFlyTek_Mode("xxxx", o_Iss_Sr_Mode);
    }

    printf("_M_Fn_Get_IFlyTek_Sence\n");
    {
        ISS_SR_SCENE o_Iss_Sr_Scence;
        o_Check.m_string_Name_Function = "_M_Fn_Get_IFlyTek_Sence";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Get_IFlyTek_Sence("xxxx", o_Iss_Sr_Scence);
    }

    printf("_M_Fn_Get_IFlyTek_Language\n");
    {
        ISS_SR_ACOUS_LANG o_ISS_SR_ACOUS_LANG;
        o_Check.m_string_Name_Function = "_M_Fn_Get_IFlyTek_Language";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Get_IFlyTek_Language("xxxx", o_ISS_SR_ACOUS_LANG);
    }

    printf("_M_Fn_IFlyTek_Data_Init\n");
    {
        o_Check.m_string_Name_Function = "_M_Fn_IFlyTek_Data_Init";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_IFlyTek_Data_Init();
    }

    printf("_M_Fn_Info_Parse_File\n");
    {
        o_Check.m_string_Name_Function = "_M_Fn_Info_Parse_File";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Info_Parse_File();

        printf("----2----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Info_Parse_File();

        printf("----3----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(false);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Info_Parse_File();

        printf("----4----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(false);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Info_Parse_File();
    }

    printf("_M_Fn_Info_Parse_Line\n");
    {
        ifstream o_ifstream;
        string string_key;
        string string_value;
        bool b_eof;
        o_Check.m_string_Name_Function = "_M_Fn_Info_Parse_Line";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Info_Parse_Line(o_ifstream, string_key, string_value, b_eof);

        printf("----2----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Info_Parse_Line(o_ifstream, string_key, string_value, b_eof);

        printf("----3----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Info_Parse_Line(o_ifstream, string_key, string_value, b_eof);

        printf("----4----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Info_Parse_Line(o_ifstream, string_key, string_value, b_eof);
    }

    printf("_M_Fn_Info_Parse_Dialog\n");
    {
        ifstream o_ifstream;
        o_Check.m_string_Name_Function = "_M_Fn_Info_Parse_Dialog";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Info_Parse_Dialog(o_ifstream);

        printf("----2----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Info_Parse_Dialog(o_ifstream);

        printf("----3----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Info_Parse_Dialog(o_ifstream);

        printf("----4----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Info_Parse_Dialog(o_ifstream);

        printf("----5----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Info_Parse_Dialog(o_ifstream);

        printf("----6----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Info_Parse_Dialog(o_ifstream);

        printf("----7----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Info_Parse_Dialog(o_ifstream);

        printf("----8----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Info_Parse_Dialog(o_ifstream);

        printf("----9----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Info_Parse_Dialog(o_ifstream);
    }

    printf("_M_Fn_Format_Json_Dictionary\n");
    {	
        string string_Json_Dictionary;

        o_Check.m_string_Name_Function = "_M_Fn_Format_Json_Dictionary";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Format_Json_Dictionary(o_Request_Context_List_Update, string_Json_Dictionary);

        printf("----2----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Format_Json_Dictionary(o_Request_Context_List_Update, string_Json_Dictionary);

        printf("----3----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Format_Json_Dictionary(o_Request_Context_List_Update, string_Json_Dictionary);
    }

    printf("_M_Fn_Format_Json_Local_Cmd\n");
    {	
        string string_Json_Cmd;

        o_Check.m_string_Name_Function = "_M_Fn_Format_Json_Local_Cmd";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Format_Json_Local_Cmd(o_Request_Context_List_Update.m_list_spo_Term_For_Add, string_Json_Cmd);

        printf("----2----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Engine_IFlyTek->_M_Fn_Format_Json_Local_Cmd(o_Request_Context_List_Update.m_list_spo_Term_For_Add, string_Json_Cmd);
    }

    printf("Fn_Callback_Msg\n");
    {	
        o_Check.m_string_Name_Function = "Fn_Callback_Msg";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        Fn_Callback_Msg(m_p_Engine_IFlyTek, 2050, 0, NULL);
    }

    printf("Fn_Network_Connect\n");
    {	
        o_Check.m_string_Name_Function = "Fn_Network_Connect";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        Fn_Network_Connect();

        printf("----2----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        Fn_Network_Connect();

        printf("----3----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        g_function_Check = o_Check;
        Fn_Network_Connect();
    }

    printf("Fn_Network_Disconnect\n");
    {	
        o_Check.m_string_Name_Function = "Fn_Network_Disconnect";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        Fn_Network_Disconnect();

        printf("----2----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        Fn_Network_Disconnect();

        printf("----3----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        g_function_Check = o_Check;
        Fn_Network_Disconnect();
    }
}

} // namespace N_IFlyTek

} // namespace N_Impl

} // namespace N_Asr

} // namespace N_Vr

/* EOF */
