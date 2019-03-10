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
#include "Vr_Asr_Impl_Mock.h"
#include "Vr_Asr_Impl_Vocon_Engine_Concrete_Test.h"
#include "Vr_Asr_Impl_Vocon_Engine_Concrete.h"
#include "Vr_Asr_Impl_Vocon_Fn_Iostream.h"
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

namespace N_Vocon
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
    M_Fn_Add_List_Context_On_Event_Phase(        
        const N_Vr::N_Asr::C_Event_Phase& eventPhaseType) {}

    void
    M_Fn_Add_List_Context_On_Event_Notify(
        const N_Vr::N_Asr::C_Event_Notify& EventNotifyType) {}

    void
    M_Fn_Recognize_On_Event_Result(
        const N_Vr::N_Asr::C_Event_Result& EventResultType) {}
};

C_Vocon_Engine_Concrete_Test::C_Vocon_Engine_Concrete_Test()
{
}

C_Vocon_Engine_Concrete_Test::~C_Vocon_Engine_Concrete_Test()
{
}

void C_Vocon_Engine_Concrete_Test::SetUp()
{
}

void C_Vocon_Engine_Concrete_Test::TearDown()
{
}

TEST_F(C_Vocon_Engine_Concrete_Test, Vocon_Engine_Common)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);

    o_Engine_Concrete.M_Fn_Get_ro_LH_COMPONENT_Base();
    o_Engine_Concrete.M_Fn_Get_ro_LH_COMPONENT_Pron();
    o_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Type_Audio_SSE();
    o_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Fx();
    o_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_FmCtxCpl();
    o_Engine_Concrete.M_Fn_Party_Load("xxx");
    o_Engine_Concrete.M_Fn_Party_Unload("xxx");
}

TEST_F(C_Vocon_Engine_Concrete_Test, Vocon_Engine_M_Fn_Init)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;
    N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);

    o_Check.m_string_Name_Function = "M_Fn_Init";
    printf("----1----\n");    
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Init();
    o_Engine_Concrete.M_Fn_Fina();

    printf("----2----\n");    
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Init();
    o_Engine_Concrete.M_Fn_Fina();

    printf("----3----\n");    
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Init();
    o_Engine_Concrete.M_Fn_Fina();

    printf("----4----\n");    
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Init();
    o_Engine_Concrete.M_Fn_Fina();

    printf("----5----\n");    
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Init();
    o_Engine_Concrete.M_Fn_Fina();

    printf("----6----\n");    
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Init();
    o_Engine_Concrete.M_Fn_Fina();

    printf("----7----\n");    
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Init();
    o_Engine_Concrete.M_Fn_Fina();

    printf("----8----\n");    
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Init();
    o_Engine_Concrete.M_Fn_Fina();

    printf("----9----\n");    
    o_Check.m_i_Pos = false;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Init();
    o_Engine_Concrete.M_Fn_Fina();

    printf("----10----\n");    
    o_Check.m_i_Pos = false;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Init();
    o_Engine_Concrete.M_Fn_Fina();
}

TEST_F(C_Vocon_Engine_Concrete_Test, Vocon_Engine_M_Fn_Fina)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);

    o_Check.m_string_Name_Function = "M_Fn_Fina";

    printf("----1----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    o_Engine_Concrete.M_Fn_Fina();

    printf("----2----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Engine_Concrete.M_Fn_Fina();

    printf("----3----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Engine_Concrete.M_Fn_Fina();

    printf("----4----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Engine_Concrete.M_Fn_Fina();

    printf("----5----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Engine_Concrete.M_Fn_Fina();

    printf("----6----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Engine_Concrete.M_Fn_Fina();

    printf("----7----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Engine_Concrete.M_Fn_Fina();
}

TEST_F(C_Vocon_Engine_Concrete_Test, Vocon_Engine_M_Fn_Data_Context_List_Update)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);
    N_Vr::N_Asr::C_Request_Context_List_Update o_Request_Context_List_Update;

    o_Check.m_string_Name_Function = "M_Fn_Data_Context_List_Update";

    printf("----1----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Data_Context_List_Update(o_Request_Context_List_Update);
    
    printf("----2----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Data_Context_List_Update(o_Request_Context_List_Update);
}

TEST_F(C_Vocon_Engine_Concrete_Test, Vocon_Engine_M_Fn_Rec_Cancel)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);

    o_Check.m_string_Name_Function = "M_Fn_Rec_Cancel";

    printf("----1----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Rec_Cancel();

    printf("----2----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Rec_Cancel();

    printf("----3----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Rec_Cancel();
}

TEST_F(C_Vocon_Engine_Concrete_Test, Vocon_Engine_M_Fn_Find_po_Data_Party)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);

    o_Check.m_string_Name_Function = "M_Fn_Find_po_Data_Party";

    printf("----1----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Find_po_Data_Party("Origin");

    printf("----2----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Find_po_Data_Party("Origin");
}

TEST_F(C_Vocon_Engine_Concrete_Test, Vocon_Engine_M_Fn_Rec_Start)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);
    N_Vr::N_Asr::C_Request_Recognize o_Request_Recognize;

    o_Check.m_string_Name_Function = "M_Fn_Rec_Start";

    printf("----1----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Rec_Start(o_Request_Recognize);

    printf("----2----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Rec_Start(o_Request_Recognize);

    printf("----3----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Rec_Start(o_Request_Recognize);
    o_Engine_Concrete.M_Fn_Fina();

    printf("----4----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Rec_Start(o_Request_Recognize);
    o_Engine_Concrete.M_Fn_Fina();

    printf("----5----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Rec_Start(o_Request_Recognize);
    o_Engine_Concrete.M_Fn_Fina();
}

TEST_F(C_Vocon_Engine_Concrete_Test, Vocon_Engine_M_Fn_Param_Set)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);
    N_Vr::N_Asr::C_Request_Param_Set o_Request_Param_Set;
    o_Request_Param_Set.m_e_Param_Type = N_Vr::N_Asr::E_Param_Type::E_Param_Type_Audio_Timeout;

    o_Check.m_string_Name_Function = "M_Fn_Param_Set";

    printf("----1----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Param_Set(o_Request_Param_Set);

    printf("----2----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Param_Set(o_Request_Param_Set);

    printf("----3----\n");
    o_Request_Param_Set.m_e_Param_Type = E_Param_Type_Fx_Sensitivity;
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Param_Set(o_Request_Param_Set);

    printf("----4----\n");
    o_Request_Param_Set.m_e_Param_Type = E_Param_Type_Fx_Absolute_Threshold;
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Param_Set(o_Request_Param_Set);

    printf("----5----\n");
    o_Request_Param_Set.m_e_Param_Type = E_Param_Type_Fx_Minspeech;
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Param_Set(o_Request_Param_Set);

    printf("----6----\n");
    o_Request_Param_Set.m_e_Param_Type = E_Param_Type_Fx_Timeout_Silence_Leading;
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Param_Set(o_Request_Param_Set);

    printf("----7----\n");
    o_Request_Param_Set.m_e_Param_Type = E_Param_Type_Fx_Timeout_Speech;
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Param_Set(o_Request_Param_Set);

    printf("----8----\n");
    o_Request_Param_Set.m_e_Param_Type = E_Param_Type_Fx_Timeout_Silence_Trailing;
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Param_Set(o_Request_Param_Set);

    printf("----9----\n");
    o_Request_Param_Set.m_e_Param_Type = E_Param_Type_Fx_Event_Timer_Interval_Millis;
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Param_Set(o_Request_Param_Set);

    printf("----10----\n");
    o_Request_Param_Set.m_e_Param_Type = E_Param_Type_Rec_Garbage;
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Param_Set(o_Request_Param_Set);

    printf("----11----\n");
    o_Request_Param_Set.m_e_Param_Type = E_Param_Type_Rec_Firstpass_Distapprox;
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Param_Set(o_Request_Param_Set);

    printf("----12----\n");
    o_Request_Param_Set.m_e_Param_Type = E_Param_Type_Audio_Timeout;
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Param_Set(o_Request_Param_Set);

    printf("----13----\n");
    o_Request_Param_Set.m_e_Param_Type = E_Param_Type_None;
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Param_Set(o_Request_Param_Set);
}

TEST_F(C_Vocon_Engine_Concrete_Test, Vocon_Engine_M_Fn_Write_Speaker_Data)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);

    o_Check.m_string_Name_Function = "M_Fn_Write_Speaker_Data";

    printf("----1----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Write_Speaker_Data(false);

    printf("----2----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Write_Speaker_Data(false);

    printf("----3----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Write_Speaker_Data(false);

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
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete.M_Fn_Write_Speaker_Data(false);
}

TEST_F(C_Vocon_Engine_Concrete_Test, Vocon_Engine_M_Fn_Base_Fina)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);

    o_Check.m_string_Name_Function = "_M_Fn_Base_Fina";

    printf("----1----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Base_Fina();

    printf("----2----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Base_Fina();

    printf("----3----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Base_Fina();
}

TEST_F(C_Vocon_Engine_Concrete_Test, Vocon_Engine_M_Fn_Asr_Fina)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);

    o_Check.m_string_Name_Function = "_M_Fn_Asr_Fina";

    printf("----1----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Fina();

    printf("----2----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Fina();

    printf("----3----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Fina();

    printf("----4----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Fina();

    printf("----5----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Fina();
}

TEST_F(C_Vocon_Engine_Concrete_Test, Vocon_Engine_M_Fn_Asr_Mod_Init)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);

    o_Check.m_string_Name_Function = "_M_Fn_Asr_Mod_Init";

    printf("----1----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Mod_Init();

    printf("----2----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Mod_Init();

    printf("----3----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Mod_Init();

    printf("----4----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Mod_Init();

    printf("----5----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Mod_Init();

    printf("----6----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Mod_Init();

    printf("----6----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Mod_Init();

    printf("----6----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Mod_Init();
}

TEST_F(C_Vocon_Engine_Concrete_Test, Vocon_Engine_M_Fn_Asr_Mod_Fina)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);

    o_Check.m_string_Name_Function = "_M_Fn_Asr_Mod_Fina";

    printf("----1----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Mod_Fina();

    printf("----2----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Mod_Fina();

    printf("----3----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Mod_Fina();
}

TEST_F(C_Vocon_Engine_Concrete_Test, Vocon_Enginev)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);

    o_Check.m_string_Name_Function = "_M_Fn_Asr_Fx_Init";

    printf("----1----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Fx_Init();

    printf("----2----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Fx_Init();

    printf("----3----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Fx_Init();

    printf("----4----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Fx_Init();
}

TEST_F(C_Vocon_Engine_Concrete_Test, Vocon_Engine_M_Fn_Asr_Fx_Fina)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);

    o_Check.m_string_Name_Function = "_M_Fn_Asr_Fx_Fina";

    printf("----1----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Fx_Fina();

    printf("----2----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Fx_Fina();

    printf("----3----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Fx_Fina();
}

TEST_F(C_Vocon_Engine_Concrete_Test, Vocon_Engine_M_Fn_Asr_Rec_Init)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);

    o_Check.m_string_Name_Function = "_M_Fn_Asr_Rec_Init";

    printf("----1----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Rec_Init();

    printf("----2----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Rec_Init();

    printf("----3----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Rec_Init();

    printf("----5----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Rec_Init();

    printf("----6----\n");
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
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Rec_Init();

    printf("----7----\n");
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
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Rec_Init();

    printf("----8----\n");
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
    o_Engine_Concrete._M_Fn_Asr_Rec_Init();

    printf("----9----\n");
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
    o_Engine_Concrete._M_Fn_Asr_Rec_Init();

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
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Rec_Init();

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
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Rec_Init();

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
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Rec_Init();
}

TEST_F(C_Vocon_Engine_Concrete_Test, Vocon_Engine_M_Fn_Asr_Rec_Fina)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);

    o_Check.m_string_Name_Function = "_M_Fn_Asr_Rec_Fina";

    printf("----1----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Rec_Fina();

    printf("----2----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Rec_Fina();

    printf("----3----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Rec_Fina();

    printf("----4----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Asr_Rec_Fina();
}

TEST_F(C_Vocon_Engine_Concrete_Test, Vocon_Engine_M_Fn_Pron_Fina)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);

    o_Check.m_string_Name_Function = "_M_Fn_Pron_Fina";

    printf("----1----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Pron_Fina();

    printf("----2----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Pron_Fina();

    printf("----3----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Pron_Fina();

    printf("----4----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(false);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Pron_Fina();
}

TEST_F(C_Vocon_Engine_Concrete_Test, Vocon_Engine_M_Fn_Pron_CLC_Init)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);

    o_Check.m_string_Name_Function = "_M_Fn_Pron_CLC_Init";

    printf("----1----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Pron_CLC_Init();
}

TEST_F(C_Vocon_Engine_Concrete_Test, Vocon_Engine_M_Fn_Pron_CLC_Fina)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);

    o_Check.m_string_Name_Function = "_M_Fn_Pron_CLC_Fina";

    printf("----1----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Pron_CLC_Fina();

    printf("----2----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Pron_CLC_Fina();

    printf("----3----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Pron_CLC_Fina();

    printf("----4----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Pron_CLC_Fina();
}

TEST_F(C_Vocon_Engine_Concrete_Test, Vocon_Engine_M_Fn_Party_Init)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);

    o_Check.m_string_Name_Function = "_M_Fn_Party_Init";

    printf("----1----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Party_Init();

    printf("----2----\n");
    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(false);
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Party_Init();
}

TEST_F(C_Vocon_Engine_Concrete_Test, Vocon_Engine_M_Fn_Party_Fina)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);

    o_Check.m_string_Name_Function = "_M_Fn_Party_Fina";

    o_Check.m_i_Pos = 0;
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    o_Engine_Concrete._M_Fn_Party_Fina();
}

TEST_F(C_Vocon_Engine_Concrete_Test, Vocon_Engine_Io_Stream)
{
    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;

    printf("S_Fn_File_Seek \n");
    {
        FILE* p_File = fopen("/data/asr/test.txt", "wb+");
        DWORD a_Crc_Value[3] = { 0 };
        fwrite(a_Crc_Value, 3, sizeof(DWORD), p_File);
        o_Check.m_string_Name_Function = "S_Fn_File_Seek";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        S_Fn_File_Seek(p_File, 0, LH_STREAM_SEEK_SET);

        printf("----2----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        S_Fn_File_Seek(p_File, 0, LH_STREAM_SEEK_SET);

        printf("----3----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        S_Fn_File_Seek(p_File, 0, LH_STREAM_SEEK_SET);

        printf("----4----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        S_Fn_File_Seek(p_File, -4, LH_STREAM_SEEK_SET);

        fclose(p_File);
    }

    printf("S_Fn_File_Error \n");
    {
        FILE* p_File = fopen("/data/asr/test.txt", "wb+");
        DWORD a_Crc_Value[2] = { 0 };
        fwrite(a_Crc_Value, 2, sizeof(DWORD), p_File);
        o_Check.m_string_Name_Function = "S_Fn_File_Error";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        S_Fn_File_Error(p_File);

        fclose(p_File);
    }

    printf("S_Fn_Reader_File_Read \n");
    {
        FILE* p_File = fopen("/data/asr/test.txt", "wb+");
        DWORD a_Crc_Value[2] = { 0 };
        fwrite(a_Crc_Value, 2, sizeof(DWORD), p_File);
        o_Check.m_string_Name_Function = "S_Fn_Reader_File_Read";

        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        S_Fn_Reader_File_Read(p_File, 0, 0, NULL);

        fclose(p_File);
    }

    printf("S_Fn_Reader_File_Finished \n");
    {
        FILE* p_File = fopen("/data/asr/test.txt", "wb+");
        DWORD a_Crc_Value[2] = { 0 };
        fwrite(a_Crc_Value, 2, sizeof(DWORD), p_File);
        o_Check.m_string_Name_Function = "S_Fn_Reader_File_Finished";

        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        S_Fn_Reader_File_Finished(p_File);

        // fclose(p_File);
    }

    printf("S_Fn_Writer_File_Write \n");
    {
        FILE* p_File = fopen("/data/asr/test.txt", "wb+");
        DWORD a_Crc_Value[2] = { 0 };
        fwrite(a_Crc_Value, 2, sizeof(DWORD), p_File);
        o_Check.m_string_Name_Function = "S_Fn_Writer_File_Write";

        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        S_Fn_Writer_File_Write(p_File, 0, 0, NULL);

        fclose(p_File);
    }

    printf("S_Fn_Writer_File_Finished \n");
    {
        FILE* p_File = fopen("/data/asr/test.txt", "wb+");
        DWORD a_Crc_Value[2] = { 0 };
        fwrite(a_Crc_Value, 2, sizeof(DWORD), p_File);
        o_Check.m_string_Name_Function = "S_Fn_Writer_File_Finished";

        printf("----1----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        S_Fn_Writer_File_Finished(p_File, LH_TRUE);

        printf("----2----\n");
        p_File = fopen("/data/asr/test.txt", "wb+");
        fwrite(a_Crc_Value, 2, sizeof(DWORD), p_File);
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        S_Fn_Writer_File_Finished(p_File, LH_TRUE);

        printf("----3----\n");
        p_File = fopen("/data/asr/test.txt", "wb+");
        fwrite(a_Crc_Value, 2, sizeof(DWORD), p_File);
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        S_Fn_Writer_File_Finished(p_File, LH_TRUE);

        printf("----4----\n");
        p_File = fopen("/data/asr/test.txt", "wb+");
        fwrite(a_Crc_Value, 2, sizeof(DWORD), p_File);
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        S_Fn_Writer_File_Finished(p_File, LH_TRUE);

        printf("----5----\n");
        p_File = fopen("/data/asr/test.txt", "wb+");
        fwrite(a_Crc_Value, 2, sizeof(DWORD), p_File);
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        S_Fn_Writer_File_Finished(p_File, LH_TRUE);

        printf("----5----\n");
        p_File = fopen("/data/asr/test.txt", "wb+");
        fwrite(a_Crc_Value, 2, sizeof(DWORD), p_File);
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        S_Fn_Writer_File_Finished(p_File, LH_TRUE);

        printf("----6----\n");
        p_File = fopen("/data/asr/test.txt", "wb+");
        fwrite(a_Crc_Value, 2, sizeof(DWORD), p_File);
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        S_Fn_Writer_File_Finished(p_File, LH_TRUE);

        printf("----7----\n");
        p_File = fopen("/data/asr/test.txt", "wb+");
        fwrite(a_Crc_Value, 2, sizeof(DWORD), p_File);
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
        g_function_Check = o_Check;
        S_Fn_Writer_File_Finished(p_File, LH_TRUE);

        // fclose(p_File);
    }

    printf("Fn_ST_CreateStreamWriterToFile \n");
    {	
        string string_file = "/data/asr/test.txt";
        struct LH_OSTREAM_INTERFACE_S o_OSTREAM_INTERFACE_S;
        void* p_Obj = &o_OSTREAM_INTERFACE_S;
        o_Check.m_string_Name_Function = "Fn_ST_CreateStreamWriterToFile";

        printf("----1----\n");
        Fn_ST_CreateStreamWriterToFile(NULL, NULL, &p_Obj);
        Fn_ST_CreateStreamWriterToFile(string_file.c_str(), NULL, &p_Obj);
        Fn_ST_CreateStreamWriterToFile(string_file.c_str(), &o_OSTREAM_INTERFACE_S, NULL);

        printf("----2----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        Fn_ST_CreateStreamWriterToFile(string_file.c_str(), &o_OSTREAM_INTERFACE_S, &p_Obj);

        printf("----3----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        Fn_ST_CreateStreamWriterToFile(string_file.c_str(), &o_OSTREAM_INTERFACE_S, &p_Obj);

        printf("----4----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        Fn_ST_CreateStreamWriterToFile(string_file.c_str(), &o_OSTREAM_INTERFACE_S, &p_Obj);
    }

    printf("Fn_ST_CreateStreamReaderFromFile \n");
    {	
        string string_file = "/data/asr/test.txt";
        struct LH_ISTREAM_INTERFACE_S O_LH_ISTREAM_INTERFACE_S;
        void* p_Obj = &O_LH_ISTREAM_INTERFACE_S;
        o_Check.m_string_Name_Function = "Fn_ST_CreateStreamReaderFromFile";

        printf("----1----\n");
        Fn_ST_CreateStreamReaderFromFile(NULL, NULL, &p_Obj);
        Fn_ST_CreateStreamReaderFromFile(string_file.c_str(), NULL, &p_Obj);
        Fn_ST_CreateStreamReaderFromFile(string_file.c_str(), &O_LH_ISTREAM_INTERFACE_S, NULL);

        printf("----2----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        Fn_ST_CreateStreamReaderFromFile(string_file.c_str(), &O_LH_ISTREAM_INTERFACE_S, &p_Obj);

        printf("----3----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        Fn_ST_CreateStreamReaderFromFile(string_file.c_str(), &O_LH_ISTREAM_INTERFACE_S, &p_Obj);

        printf("----4----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        Fn_ST_CreateStreamReaderFromFile(string_file.c_str(), &O_LH_ISTREAM_INTERFACE_S, &p_Obj);

        printf("----5----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        Fn_ST_CreateStreamReaderFromFile(string_file.c_str(), &O_LH_ISTREAM_INTERFACE_S, &p_Obj);

        printf("----6----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        Fn_ST_CreateStreamReaderFromFile(string_file.c_str(), &O_LH_ISTREAM_INTERFACE_S, &p_Obj);

        printf("----7----\n");
        o_Check.m_i_Pos = 0;
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        Fn_ST_CreateStreamReaderFromFile(string_file.c_str(), &O_LH_ISTREAM_INTERFACE_S, &p_Obj);

        printf("----8----\n");
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
        g_function_Check = o_Check;
        Fn_ST_CreateStreamReaderFromFile(string_file.c_str(), &O_LH_ISTREAM_INTERFACE_S, &p_Obj);

        printf("----9----\n");
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
        Fn_ST_CreateStreamReaderFromFile(string_file.c_str(), &O_LH_ISTREAM_INTERFACE_S, &p_Obj);
    }
}

} // namespace N_Vocon

} // namespace N_Impl

} // namespace N_Asr

} // namespace N_Vr

/* EOF */
