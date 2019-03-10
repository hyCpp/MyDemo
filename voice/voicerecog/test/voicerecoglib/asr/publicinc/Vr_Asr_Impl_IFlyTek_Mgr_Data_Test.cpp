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
#include "Vr_Asr_Impl_IFlyTek_Engine_Concrete.h"
#include "Vr_Asr_Impl_IFlyTek_Mgr_Data_Test.h"
#include "Vr_Asr_Impl_IFlyTek_Mgr_Data.h"
#include "Vr_Asr_Impl_Engine_Agent.h"
#include "Vr_Asr_Request_Factory.h"
#include "Vr_Asr_Audio_In_Mock_IFlyTek.h"
#include "Vr_Asr_Request_Recognize.h"

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

C_Mgr_Data_Test::C_Mgr_Data_Test()
: m_p_Engine_Agent(NULL)
, m_p_Engine_IFlyTek(NULL)
, m_p_Mgr_Data(NULL)
{
}

C_Mgr_Data_Test::~C_Mgr_Data_Test()
{
}

void C_Mgr_Data_Test::SetUp()
{
    C_Engine_Fake o_Engine_Fake;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Notify = boost::bind(&C_Engine_Fake::M_Fn_On_Event_Init_Notify, &o_Engine_Fake, _1);
    m_p_Engine_Agent = new C_Engine_Agent();
    m_p_Engine_IFlyTek = new C_Engine_Concrete(*m_p_Engine_Agent, "eng", function_On_Event_Notify, "none");
    m_p_Mgr_Data = new C_Mgr_Data(*m_p_Engine_IFlyTek);
}

void C_Mgr_Data_Test::TearDown()
{
    delete m_p_Engine_IFlyTek;
    m_p_Engine_IFlyTek = NULL;

    delete m_p_Engine_Agent;
    m_p_Engine_Agent = NULL;

    delete m_p_Mgr_Data;
    m_p_Mgr_Data = NULL;
}



TEST_F(C_Mgr_Data_Test, IFlyTek_Mgr_Data)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    C_Engine_Fake o_Engine_Fake;
    C_Request_Recognize o_Request_Recognize;
    C_Audio_In_Mock_IFlyTek* p_Audio_In_Mock_IFlyTek = new C_Audio_In_Mock_IFlyTek();
    o_Request_Recognize.m_spo_Audio_In.reset(p_Audio_In_Mock_IFlyTek);

    o_Request_Recognize.m_function_On_Event_Phase = boost::bind(&C_Engine_Fake::M_Fn_Add_List_Context_On_Event_Phase, &o_Engine_Fake, _1);
    o_Request_Recognize.m_function_On_Event_Notify = boost::bind(&C_Engine_Fake::M_Fn_Add_List_Context_On_Event_Notify, &o_Engine_Fake, _1);
    o_Request_Recognize.m_function_On_Event_Result = boost::bind(&C_Engine_Fake::M_Fn_Recognize_On_Event_Result, &o_Engine_Fake, _1);
    m_p_Engine_IFlyTek->m_scpo_Request_Recognize.reset(new C_Request_Recognize(o_Request_Recognize));
	
    printf("M_Fn_Init \n");
    {
        o_Check.m_string_Name_Function = "M_Fn_Init";

        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Mgr_Data->M_Fn_Init();
        m_p_Mgr_Data->_M_Fn_Thread_Fina();
    }
	
    printf("M_Fn_Fina \n");
    {
        o_Check.m_string_Name_Function = "M_Fn_Fina";

        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Mgr_Data->M_Fn_Fina();
    }
	
    
    printf("M_Fn_Start_Audio\n");
    {
        o_Check.m_string_Name_Function = "M_Fn_Start_Audio";

        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Mgr_Data->M_Fn_Start_Audio();
        usleep(50 * 1000);
    }

    m_p_Mgr_Data->m_spo_Audio_In = o_Request_Recognize.m_spo_Audio_In;
    m_p_Mgr_Data->m_function_On_Event_Phase = o_Request_Recognize.m_function_On_Event_Phase;
    m_p_Mgr_Data->m_b_Audio_Close = false;

    printf("M_Fn_On_Thread_Notified\n");
    {
        o_Check.m_string_Name_Function = "M_Fn_On_Thread_Notified";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Mgr_Data->M_Fn_On_Thread_Notified();

        printf("----2----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Mgr_Data->M_Fn_On_Thread_Notified();

        printf("----3----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Mgr_Data->M_Fn_On_Thread_Notified();

        printf("----4----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Mgr_Data->M_Fn_On_Thread_Notified();

        printf("----5----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Mgr_Data->M_Fn_On_Thread_Notified();
    }

    printf("_M_Fn_Thread_Init\n");
    {
        o_Check.m_string_Name_Function = "_M_Fn_Thread_Init";

        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        g_function_Check = o_Check;
        m_p_Mgr_Data->_M_Fn_Thread_Init();
    }

    printf("_M_Fn_Append_Audio_Data\n");
    {
        o_Check.m_string_Name_Function = "_M_Fn_Append_Audio_Data";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Mgr_Data->_M_Fn_Append_Audio_Data(NULL, 0);

        printf("----2----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Mgr_Data->_M_Fn_Append_Audio_Data(NULL, 0);

        printf("----3----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Mgr_Data->_M_Fn_Append_Audio_Data(NULL, 0);

        printf("----4----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        m_p_Mgr_Data->_M_Fn_Append_Audio_Data(NULL, 0);
    }
}

} // namespace N_IFlyTek

} // namespace N_Impl

} // namespace N_Asr

} // namespace N_Vr

/* EOF */
