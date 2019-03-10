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

// #define private public
// #define protected public

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "Vr_Asr_Impl_Afx.h"
#include "Vr_Asr_Impl_Engine_Agent_Test.h"
#include "Vr_Asr_Impl_Engine_Agent.h"
#include "Vr_Asr_Request_Factory.h"
#include "Vr_Asr_Impl_Engine_Base_Mock.h"
#include "Vr_Asr_Audio_In_Mock.h"
#include "Vr_Asr_Request_Context_List_Update.h"
#include "Vr_Asr_Request_Recognize.h"
#include "Vr_Asr_Request_Factory.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

using ::testing::Return;
using ::testing::_;

/**
 * @brief The C_Engine_Fake class
 *
 * C_Engine_Fake class
 */

class C_Engine_Fake
{
public:
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


C_Engine_Agent_Test::C_Engine_Agent_Test()
: m_p_C_Engine_Agent(NULL)
{
}

C_Engine_Agent_Test::~C_Engine_Agent_Test()
{
}

void C_Engine_Agent_Test::SetUp()
{
    m_p_C_Engine_Agent = new C_Engine_Agent();
}

void C_Engine_Agent_Test::TearDown()
{
    delete m_p_C_Engine_Agent;
    m_p_C_Engine_Agent = NULL;
}

TEST_F(C_Engine_Agent_Test, Language_Change)
{
    C_Engine_Base_Mock* p_C_Engine_Base_Mock = new C_Engine_Base_Mock();

    printf("----1----\n");
    EXPECT_CALL(*p_C_Engine_Base_Mock, M_Fn_Fina()).WillOnce(Return(E_Result_Failed)).WillOnce(Return(E_Result_Succeeded));
    m_p_C_Engine_Agent->m_scpo_Engine_Base.reset(p_C_Engine_Base_Mock);
    m_p_C_Engine_Agent->Language_Change("eng");
    m_p_C_Engine_Agent->Language_Change("eng");
    
    printf("----2----\n");
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;
    g_function_Check = Fn_Check_Real;
    
    m_p_C_Engine_Agent->m_scpo_Engine_Base.reset();
    
    N_Vr::N_Asr::C_Request_Language_Change o_Request_Language_Change;
    o_Request_Language_Change.m_string_Id_Language = "eng";
    o_Check.m_string_Name_Function = "Language_Change";
    
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    m_p_C_Engine_Agent->Language_Change(o_Request_Language_Change);

    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    g_function_Check = o_Check;
    m_p_C_Engine_Agent->Language_Change(o_Request_Language_Change);

}

TEST_F(C_Engine_Agent_Test, Party_Load)
{
    C_Engine_Base_Mock* p_C_Engine_Base_Mock = new C_Engine_Base_Mock();

    EXPECT_CALL(*p_C_Engine_Base_Mock, M_Fn_Party_Load(_)).WillRepeatedly(Return(E_Result_Failed));
    EXPECT_CALL(*p_C_Engine_Base_Mock, M_Fn_Fina()).WillRepeatedly(Return(E_Result_Succeeded));

    m_p_C_Engine_Agent->m_scpo_Engine_Base.reset(p_C_Engine_Base_Mock);
    m_p_C_Engine_Agent->Party_Load("eng");
}

TEST_F(C_Engine_Agent_Test, Context_List_Update_Sync)
{
    C_Engine_Base_Mock* p_C_Engine_Base_Mock = new C_Engine_Base_Mock();
    C_Request_Context_List_Update o_C_Request_Context_List_Update;
    C_Engine_Fake o_C_Engine_Fake;

    EXPECT_CALL(*p_C_Engine_Base_Mock, M_Fn_Data_Context_List_Update(_)).WillRepeatedly(Return(E_Result_Succeeded));
    EXPECT_CALL(*p_C_Engine_Base_Mock, M_Fn_Fina()).WillRepeatedly(Return(E_Result_Succeeded));

    m_p_C_Engine_Agent->m_scpo_Engine_Base.reset();
    m_p_C_Engine_Agent->Context_List_Update_Sync(o_C_Request_Context_List_Update);

    m_p_C_Engine_Agent->m_scpo_Engine_Base.reset(p_C_Engine_Base_Mock);
    m_p_C_Engine_Agent->Context_List_Update_Sync(o_C_Request_Context_List_Update);

    o_C_Request_Context_List_Update.m_function_On_Event_Phase =
        boost::bind(&C_Engine_Fake::M_Fn_Add_List_Context_On_Event_Phase, &o_C_Engine_Fake, _1);
    m_p_C_Engine_Agent->Context_List_Update_Sync(o_C_Request_Context_List_Update);
    }

TEST_F(C_Engine_Agent_Test, Context_List_Update)
{
    C_Request_Context_List_Update o_C_Request_Context_List_Update;
    m_p_C_Engine_Agent->Context_List_Update(o_C_Request_Context_List_Update);
}

TEST_F(C_Engine_Agent_Test, Recognize_Start_Sync)
{
    C_Engine_Base_Mock* p_C_Engine_Base_Mock = new C_Engine_Base_Mock();
    C_Audio_In_Mock* p_C_Audio_In_Mock = new C_Audio_In_Mock();
    C_Request_Recognize  o_C_Request_Recognize;

    C_Engine_Fake o_C_Engine_Fake;

    EXPECT_CALL(*p_C_Engine_Base_Mock, M_Fn_Fina()).WillRepeatedly(Return(E_Result_Succeeded));

    EXPECT_EQ(E_Result_Failed, m_p_C_Engine_Agent->Recognize_Start_Sync(o_C_Request_Recognize));

    m_p_C_Engine_Agent->m_scpo_Engine_Base.reset(p_C_Engine_Base_Mock);
    EXPECT_EQ(E_Result_Failed, m_p_C_Engine_Agent->Recognize_Start_Sync(o_C_Request_Recognize));

    o_C_Request_Recognize.m_spo_Audio_In.reset(p_C_Audio_In_Mock);
    EXPECT_EQ(E_Result_Failed, m_p_C_Engine_Agent->Recognize_Start_Sync(o_C_Request_Recognize));

    o_C_Request_Recognize.m_function_On_Event_Phase =
        boost::bind(&C_Engine_Fake::M_Fn_Add_List_Context_On_Event_Phase, &o_C_Engine_Fake, _1);
    EXPECT_EQ(E_Result_Failed, m_p_C_Engine_Agent->Recognize_Start_Sync(o_C_Request_Recognize));

    o_C_Request_Recognize.m_function_On_Event_Notify =
        boost::bind(&C_Engine_Fake::M_Fn_Add_List_Context_On_Event_Notify, &o_C_Engine_Fake, _1);
    EXPECT_EQ(E_Result_Failed, m_p_C_Engine_Agent->Recognize_Start_Sync(o_C_Request_Recognize));
}

TEST_F(C_Engine_Agent_Test, Recognize_Start)
{
    C_Request_Recognize o_C_Request_Recognize;
    m_p_C_Engine_Agent->Recognize_Start(o_C_Request_Recognize);
}

TEST_F(C_Engine_Agent_Test, Recognize_Cancel)
{
    C_Engine_Base_Mock* p_C_Engine_Base_Mock = new C_Engine_Base_Mock();

    EXPECT_CALL(*p_C_Engine_Base_Mock, M_Fn_Rec_Cancel()).WillRepeatedly(Return(E_Result_Succeeded));
    EXPECT_CALL(*p_C_Engine_Base_Mock, M_Fn_Fina()).WillRepeatedly(Return(E_Result_Succeeded));

    EXPECT_EQ(E_Result_Failed, m_p_C_Engine_Agent->Recognize_Cancel());

    m_p_C_Engine_Agent->m_scpo_Engine_Base.reset(p_C_Engine_Base_Mock);
    EXPECT_EQ(E_Result_Succeeded, m_p_C_Engine_Agent->Recognize_Cancel());
}

TEST_F(C_Engine_Agent_Test, Party_Unload)
{
    m_p_C_Engine_Agent->Party_Unload("xxx");
}

TEST_F(C_Engine_Agent_Test, Factory)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    o_Check.m_string_Name_Function = "Factory";
    printf("----1----\n");    
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    m_p_C_Engine_Agent->Factory("xxx");

    printf("----2----\n");    
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    m_p_C_Engine_Agent->Factory("xxx");

    printf("----3----\n");    
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    g_function_Check = o_Check;
    m_p_C_Engine_Agent->Factory("xxx");
}

} // namespace N_Impl

} // namespace N_Asr

} // namespace N_Vr

/* EOF */
