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

#include "Vr_Asr_Impl_Gtest.h"

#include "Vr_Asr_Impl_Engine_Agent.h"
#include "Vr_Asr_Impl_Vocon_Via_Shared_Engine_Concrete.h"
#include "Vr_Asr_Impl_Vocon_Result.h"
#include "Vr_Asr_Impl_Vocon_Mgr_Data.h"
#include "Vr_Asr_Impl_Vocon_Engine_Concrete.h"
#include "Vr_Asr_Request_Context_List_Update.h"
#include "Vr_Asr_Impl_Vocon_Request_Data.h"
#include "Vr_Asr_Impl_Vocon_Thread_Data.h"
#include "Vr_Asr_Impl_Vocon_Data_Party.h"
#include "Vr_Asr_Impl_Vocon_Data_Context.h"
#include "Vr_Asr_Impl_Vocon_Data_Party.h"
#include "Vr_Asr_Request_Recognize.h"
#include "Vr_Asr_Impl_Vocon_Data_Rule.h"

using N_Vr::N_Asr::N_Impl::C_Engine_Agent;
using N_Vr::N_Asr::N_Impl::C_Check;
using N_Vr::N_Asr::N_Impl::N_Vocon::C_Result_Item;
using N_Vr::N_Asr::N_Impl::N_Vocon::C_Result_Hypothesis;
using N_Vr::N_Asr::N_Impl::N_Vocon::C_Mgr_Data;
using N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete;
using N_Vr::N_Asr::C_Request_Context_List_Update;
using N_Vr::N_Asr::N_Impl::N_Vocon::C_Mgr_Data;
using N_Vr::N_Asr::N_Impl::N_Vocon::C_Request_Data;
using N_Vr::N_Asr::N_Impl::N_Vocon::C_Thread_Data;
using N_Vr::N_Asr::N_Impl::N_Vocon::C_Data_Party;
using N_Vr::N_Asr::N_Impl::N_Vocon::C_Data_Context;
using N_Vr::N_Asr::C_Request_Recognize;
using N_Vr::N_Asr::N_Impl::N_Vocon::C_Data_Rule;
using N_Vr::N_Asr::C_Event_Init_Notify;

using std::pair;
using std::ifstream;

#ifndef MACRO_Dynamic_Debug

void C_Vr_Asr_Impl_Gtest::M_Fn_Run_All_Gtest()
{
    M_Fn_Vocon_Via_Shared_Engine_Concrete();
    M_Fn_Vocon_Result();
    M_Fn_Vocon_Mgr_Data();
    M_FN_Vocon_Data_Party();
}

void C_Vr_Asr_Impl_Gtest::M_Fn_Vocon_Via_Shared_Engine_Concrete()
{
    // test Vr_Asr_Impl_Vocon_Via_Shared_Engine_Concrete.cpp

    g_function_Check = Fn_Check_Real;

    C_Check o_C_Check;
    o_C_Check.m_i_Pos = 0;
    o_C_Check.m_b_Specify_Function = true;


    // C_Engine_Concrete
    C_Engine_Agent o_C_Engine_Agent;
    shared_ptr<LH_COMPONENT> sp_LH_COMPONENT(new LH_COMPONENT());
    shared_ptr<LH_OBJECT>  sp_LH_OBJECT(new LH_OBJECT());
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;

    N_Vr::N_Asr::N_Impl::N_Vocon::N_Via_Shared::C_Engine_Concrete
    o_C_Engine_Concrete(o_C_Engine_Agent, "eng", sp_LH_COMPONENT.get(), sp_LH_OBJECT.get(), function_On_Event_Init_Notify);


    // C_Engine_Concrete::M_Fn_Get_ro_LH_COMPONENT_Asr
    o_C_Engine_Concrete.M_Fn_Get_ro_LH_COMPONENT_Asr();


    // C_Engine_Concrete::M_Fn_Get_ro_LH_OBJECT_Mod
    o_C_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Mod();


    // C_Engine_Concrete::_M_Fn_Base_Init
    o_C_Engine_Concrete._M_Fn_Base_Init();


    // C_Engine_Concrete::_M_Fn_Base_Fina
    o_C_Engine_Concrete._M_Fn_Base_Fina();


    // C_Engine_Concrete::_M_Fn_Asr_Init
    o_C_Check.m_string_Name_Function = "_M_Fn_Asr_Init";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Engine_Concrete._M_Fn_Asr_Init();

    o_C_Check.m_string_Name_Function = "_M_Fn_Asr_Init";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Engine_Concrete._M_Fn_Asr_Init();

    o_C_Check.m_string_Name_Function = "_M_Fn_Asr_Init";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    g_function_Check = o_C_Check;
    o_C_Engine_Concrete._M_Fn_Asr_Init();


    // C_Engine_Concrete::_M_Fn_Asr_Fina
    o_C_Check.m_string_Name_Function = "_M_Fn_Asr_Fina";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    g_function_Check = o_C_Check;
    o_C_Engine_Concrete._M_Fn_Asr_Fina();

    o_C_Check.m_string_Name_Function = "_M_Fn_Asr_Fina";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Engine_Concrete._M_Fn_Asr_Fina();

    o_C_Check.m_string_Name_Function = "_M_Fn_Asr_Fina";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    g_function_Check = o_C_Check;
    o_C_Engine_Concrete._M_Fn_Asr_Fina();

    o_C_Check.m_string_Name_Function = "_M_Fn_Asr_Fina";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Engine_Concrete._M_Fn_Asr_Fina();

    o_C_Check.m_string_Name_Function = "_M_Fn_Asr_Fina";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Engine_Concrete._M_Fn_Asr_Fina();

    o_C_Check.m_string_Name_Function = "_M_Fn_Asr_Fina";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    g_function_Check = o_C_Check;
    o_C_Engine_Concrete._M_Fn_Asr_Fina();


    // C_Engine_Concrete::_M_Fn_Asr_Mod_Init
    o_C_Check.m_string_Name_Function = "_M_Fn_Asr_Mod_Init";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Engine_Concrete._M_Fn_Asr_Mod_Init();

    o_C_Check.m_string_Name_Function = "_M_Fn_Asr_Mod_Init";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Engine_Concrete._M_Fn_Asr_Mod_Init();

    o_C_Check.m_string_Name_Function = "_M_Fn_Asr_Mod_Init";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    g_function_Check = o_C_Check;
    o_C_Engine_Concrete._M_Fn_Asr_Mod_Init();


    // C_Engine_Concrete::_M_Fn_Asr_Mod_Fina
    o_C_Engine_Concrete._M_Fn_Asr_Mod_Fina();


    // C_Engine_Concrete::_M_Fn_Pron_Init
    o_C_Engine_Concrete._M_Fn_Pron_Init();


    // C_Engine_Concrete::_M_Fn_Pron_Fina
    o_C_Engine_Concrete._M_Fn_Pron_Fina();


    g_function_Check = Fn_Check_Real;
}

void C_Vr_Asr_Impl_Gtest::M_Fn_Vocon_Result()
{
    // test Vr_Asr_Impl_Vocon_Result.cpp

    g_function_Check = Fn_Check_Real;

    C_Check o_C_Check;
    o_C_Check.m_i_Pos = 0;
    o_C_Check.m_b_Specify_Function = true;


    // class C_Result_Item
    C_Result_Item o_C_Result_Item;

    shared_ptr<C_Result_Item> sp_item1(new C_Result_Item());
    shared_ptr<C_Result_Item> sp_item2(new C_Result_Item());
    shared_ptr<C_Result_Item> sp_item3(new C_Result_Item());
    shared_ptr<C_Result_Item> sp_item4(new C_Result_Item());
    o_C_Result_Item.m_vector_spo_Result_Item_Tree.push_back(sp_item1);
    o_C_Result_Item.m_vector_spo_Result_Item_Tree.push_back(sp_item2);
    o_C_Result_Item.m_vector_spo_Result_Item_Tree.push_back(sp_item3);
    o_C_Result_Item.m_vector_spo_Result_Item_Tree.push_back(sp_item4);


    // C_Result_Item::Get_string_Terminal_Orthography
    o_C_Check.m_string_Name_Function = "Get_string_Terminal_Orthography";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(0);
    g_function_Check = o_C_Check;
    o_C_Result_Item.Get_string_Terminal_Orthography();


    // C_Result_Item::Get_string_Eval
    o_C_Result_Item.Get_string_Eval();


    // C_Result_Item::Get_po_Result_Item_Terminal_By_Pos

    size_t i_Pos_Terminal_Target = 1;
    size_t i_Pos_Terminal_Curr;

    o_C_Check.m_string_Name_Function = "Get_po_Result_Item_Terminal_By_Pos";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    g_function_Check = o_C_Check;
    o_C_Result_Item.Get_po_Result_Item_Terminal_By_Pos(
        i_Pos_Terminal_Target, i_Pos_Terminal_Curr);

    o_C_Check.m_string_Name_Function = "Get_po_Result_Item_Terminal_By_Pos";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Result_Item.Get_po_Result_Item_Terminal_By_Pos(
        i_Pos_Terminal_Target, i_Pos_Terminal_Curr);


    // C_Result_Item::Get_po_Result_Item_Terminal_By_Code
    o_C_Result_Item.Get_po_Result_Item_Terminal_By_Code(1, 2);


    // class C_Result_Hypothesis
    C_Result_Hypothesis o_C_Result_Hypothesis;

    shared_ptr<C_Result_Item> sp_item_Slot1(new C_Result_Item());
    shared_ptr<C_Result_Item> sp_item_Slot2(new C_Result_Item());
    shared_ptr<C_Result_Item> sp_item_Slot3(new C_Result_Item());
    shared_ptr<C_Result_Item> sp_item_Slot4(new C_Result_Item());
    o_C_Result_Hypothesis.m_vector_spo_Result_Item_Flat_Slot.push_back(sp_item_Slot1);
    o_C_Result_Hypothesis.m_vector_spo_Result_Item_Flat_Slot.push_back(sp_item_Slot2);
    o_C_Result_Hypothesis.m_vector_spo_Result_Item_Flat_Slot.push_back(sp_item_Slot3);
    o_C_Result_Hypothesis.m_vector_spo_Result_Item_Flat_Slot.push_back(sp_item_Slot4);

    // C_Result_Hypothesis::Get_po_Result_Item_Tag_By_Pos
    o_C_Check.m_string_Name_Function = "Get_po_Result_Item_Tag_By_Pos";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Result_Hypothesis.Get_po_Result_Item_Tag_By_Pos(1);

    o_C_Check.m_string_Name_Function = "Get_po_Result_Item_Tag_By_Pos";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    g_function_Check = o_C_Check;
    o_C_Result_Hypothesis.Get_po_Result_Item_Tag_By_Pos(1);


    // C_Result_Hypothesis::Get_po_Result_Item_Tag_By_Code
    o_C_Check.m_string_Name_Function = "Get_po_Result_Item_Tag_By_Code";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Result_Hypothesis.Get_po_Result_Item_Tag_By_Code(1, 2);

    g_function_Check = Fn_Check_Real;
}

void C_Vr_Asr_Impl_Gtest::M_Fn_Vocon_Mgr_Data()
{
    // test Vr_Asr_Impl_Vocon_Mgr_Data.cpp

    g_function_Check = Fn_Check_Real;

    C_Check o_C_Check;
    o_C_Check.m_i_Pos = 0;
    o_C_Check.m_b_Specify_Function = true;


    // class C_Mgr_Data
    C_Engine_Agent o_C_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    C_Engine_Concrete o_C_Engine_Concrete(o_C_Engine_Agent, "eng", function_On_Event_Init_Notify);
    C_Mgr_Data o_C_Mgr_Data(o_C_Engine_Concrete);

    shared_ptr<C_Request_Data> sp_C_Request_Data(new C_Request_Data());
    o_C_Mgr_Data.m_list_spo_Request_Data.push_back(sp_C_Request_Data);
    o_C_Mgr_Data.m_scpo_Thread_Data.reset(new C_Thread_Data(o_C_Mgr_Data));


    // C_Mgr_Data::M_Fn_Init
    o_C_Check.m_string_Name_Function = "M_Fn_Init";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Mgr_Data.M_Fn_Init();

    o_C_Check.m_string_Name_Function = "M_Fn_Init";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    g_function_Check = o_C_Check;
    o_C_Mgr_Data.M_Fn_Init();


    // C_Mgr_Data::M_Fn_Fina
    o_C_Check.m_string_Name_Function = "M_Fn_Fina";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Mgr_Data.M_Fn_Fina();

    o_C_Check.m_string_Name_Function = "M_Fn_Fina";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    g_function_Check = o_C_Check;
    o_C_Mgr_Data.M_Fn_Fina();


    // C_Mgr_Data::M_Fn_On_Thread_Notified
    o_C_Check.m_string_Name_Function = "M_Fn_On_Thread_Notified";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Mgr_Data.M_Fn_On_Thread_Notified();


    // C_Mgr_Data::M_Fn_Context_List_Update
    C_Request_Context_List_Update o_C_Request_Context_List_Update;

    // o_C_Check.m_string_Name_Function = "M_Fn_Context_List_Update";
    // o_C_Check.m_vector_i_Check.clear();
    // o_C_Check.m_vector_i_Check.push_back(1);
    // o_C_Check.m_vector_i_Check.push_back(1);
    // o_C_Check.m_vector_i_Check.push_back(1);
    // g_function_Check = o_C_Check;
    // o_C_Mgr_Data.M_Fn_Context_List_Update(o_C_Request_Context_List_Update);

    // o_C_Check.m_string_Name_Function = "M_Fn_Context_List_Update";
    // o_C_Check.m_vector_i_Check.clear();
    // o_C_Check.m_vector_i_Check.push_back(1);
    // o_C_Check.m_vector_i_Check.push_back(1);
    // o_C_Check.m_vector_i_Check.push_back(0);
    // g_function_Check = o_C_Check;
    // o_C_Mgr_Data.M_Fn_Context_List_Update(o_C_Request_Context_List_Update);

    // o_C_Check.m_string_Name_Function = "M_Fn_Context_List_Update";
    // o_C_Check.m_vector_i_Check.clear();
    // o_C_Check.m_vector_i_Check.push_back(1);
    // o_C_Check.m_vector_i_Check.push_back(0);
    // o_C_Check.m_vector_i_Check.push_back(1);
    // g_function_Check = o_C_Check;
    // o_C_Mgr_Data.M_Fn_Context_List_Update(o_C_Request_Context_List_Update);

    // o_C_Check.m_string_Name_Function = "M_Fn_Context_List_Update";
    // o_C_Check.m_vector_i_Check.clear();
    // o_C_Check.m_vector_i_Check.push_back(1);
    // o_C_Check.m_vector_i_Check.push_back(0);
    // o_C_Check.m_vector_i_Check.push_back(0);
    // g_function_Check = o_C_Check;
    // o_C_Mgr_Data.M_Fn_Context_List_Update(o_C_Request_Context_List_Update);

    // o_C_Check.m_string_Name_Function = "M_Fn_Context_List_Update";
    // o_C_Check.m_vector_i_Check.clear();
    // o_C_Check.m_vector_i_Check.push_back(0);
    // o_C_Check.m_vector_i_Check.push_back(1);
    // o_C_Check.m_vector_i_Check.push_back(1);
    // g_function_Check = o_C_Check;
    // o_C_Mgr_Data.M_Fn_Context_List_Update(o_C_Request_Context_List_Update);

    // o_C_Check.m_string_Name_Function = "M_Fn_Context_List_Update";
    // o_C_Check.m_vector_i_Check.clear();
    // o_C_Check.m_vector_i_Check.push_back(0);
    // o_C_Check.m_vector_i_Check.push_back(0);
    // o_C_Check.m_vector_i_Check.push_back(0);
    // g_function_Check = o_C_Check;
    // o_C_Mgr_Data.M_Fn_Context_List_Update(o_C_Request_Context_List_Update);


    // C_Mgr_Data::M_Fn_Suspend
    o_C_Check.m_string_Name_Function = "M_Fn_Suspend";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Mgr_Data.M_Fn_Suspend();

    o_C_Check.m_string_Name_Function = "M_Fn_Suspend";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    g_function_Check = o_C_Check;
    o_C_Mgr_Data.M_Fn_Suspend();


    // C_Mgr_Data::M_Fn_Resume
    o_C_Check.m_string_Name_Function = "M_Fn_Resume";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Mgr_Data.M_Fn_Suspend();

    o_C_Check.m_string_Name_Function = "M_Fn_Resume";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    g_function_Check = o_C_Check;
    o_C_Mgr_Data.M_Fn_Suspend();


    // C_Mgr_Data::_M_Fn_Thread_Init
    o_C_Check.m_string_Name_Function = "_M_Fn_Thread_Init";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Mgr_Data._M_Fn_Thread_Init();

    o_C_Check.m_string_Name_Function = "_M_Fn_Thread_Init";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Mgr_Data._M_Fn_Thread_Init();
    o_C_Check.m_string_Name_Function = "_M_Fn_Thread_Fina";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Mgr_Data._M_Fn_Thread_Fina();


    // C_Mgr_Data::_M_Fn_Context_List_Update_Begin

    o_C_Mgr_Data.m_spo_Request_Data_Curr.reset(new C_Request_Data());
    o_C_Mgr_Data.m_po_Data_Party_Curr = new C_Data_Party(
        o_C_Mgr_Data.m_ro_Engine_Concrete, "xxx");
    C_Data_Party o_C_Data_Party(o_C_Mgr_Data.m_ro_Engine_Concrete, "xxx");
    list<pair<string, string> > list_str_str;
    list_str_str.push_back(pair<string, string>("111", "222"));
    shared_ptr<C_Data_Context> sp_C_Data_Context(
        new C_Data_Context(o_C_Data_Party, "xxx", "xxx", "xxx", list_str_str));
    o_C_Mgr_Data.m_po_Data_Party_Curr->m_map_string_spo_Data_Context.insert(
        pair<string, shared_ptr<C_Data_Context> >("aa", sp_C_Data_Context));

    o_C_Check.m_string_Name_Function = "_M_Fn_Context_List_Update_Begin";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Mgr_Data._M_Fn_Context_List_Update_Begin();

    // o_C_Check.m_string_Name_Function = "_M_Fn_Context_List_Update_Begin";
    // o_C_Check.m_vector_i_Check.clear();
    // o_C_Check.m_vector_i_Check.push_back(0);
    // o_C_Check.m_vector_i_Check.push_back(1);
    // g_function_Check = o_C_Check;
    // o_C_Mgr_Data._M_Fn_Context_List_Update_Begin();

    // o_C_Check.m_string_Name_Function = "_M_Fn_Context_List_Update_Begin";
    // o_C_Check.m_vector_i_Check.clear();
    // o_C_Check.m_vector_i_Check.push_back(0);
    // o_C_Check.m_vector_i_Check.push_back(0);
    // o_C_Check.m_vector_i_Check.push_back(1);
    // g_function_Check = o_C_Check;
    // o_C_Mgr_Data._M_Fn_Context_List_Update_Begin();

    // o_C_Check.m_string_Name_Function = "_M_Fn_Context_List_Update_Begin";
    // o_C_Check.m_vector_i_Check.clear();
    // o_C_Check.m_vector_i_Check.push_back(0);
    // o_C_Check.m_vector_i_Check.push_back(0);
    // o_C_Check.m_vector_i_Check.push_back(0);
    // o_C_Check.m_vector_i_Check.push_back(1);
    // g_function_Check = o_C_Check;
    // o_C_Mgr_Data._M_Fn_Context_List_Update_Begin();

    // o_C_Check.m_string_Name_Function = "_M_Fn_Context_List_Update_Begin";
    // o_C_Check.m_vector_i_Check.clear();
    // o_C_Check.m_vector_i_Check.push_back(0);
    // o_C_Check.m_vector_i_Check.push_back(0);
    // o_C_Check.m_vector_i_Check.push_back(0);
    // o_C_Check.m_vector_i_Check.push_back(0);
    // g_function_Check = o_C_Check;
    // o_C_Mgr_Data._M_Fn_Context_List_Update_Begin();


    // C_Mgr_Data::_M_Fn_Context_List_Update_Clear
    o_C_Check.m_string_Name_Function = "_M_Fn_Context_List_Update_Clear";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Mgr_Data._M_Fn_Context_List_Update_Clear();


    // C_Mgr_Data::_M_Fn_Context_List_Update_Delete
    o_C_Check.m_string_Name_Function = "_M_Fn_Context_List_Update_Delete";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_C_Check;
    o_C_Mgr_Data._M_Fn_Context_List_Update_Delete();

    o_C_Check.m_string_Name_Function = "_M_Fn_Context_List_Update_Delete";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(false);
    o_C_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_C_Check;
    o_C_Mgr_Data._M_Fn_Context_List_Update_Delete();

    o_C_Check.m_string_Name_Function = "_M_Fn_Context_List_Update_Delete";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(false);
    o_C_Check.m_vector_i_Check.push_back(false);
    o_C_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_C_Check;
    o_C_Mgr_Data._M_Fn_Context_List_Update_Delete();


    // C_Mgr_Data::_M_Fn_Context_List_Update_Add
    o_C_Check.m_string_Name_Function = "_M_Fn_Context_List_Update_Add";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Mgr_Data._M_Fn_Context_List_Update_Add();

    // o_C_Check.m_string_Name_Function = "_M_Fn_Context_List_Update_Add";
    // o_C_Check.m_vector_i_Check.clear();
    // o_C_Check.m_vector_i_Check.push_back(0);
    // o_C_Check.m_vector_i_Check.push_back(1);
    // g_function_Check = o_C_Check;
    // o_C_Mgr_Data._M_Fn_Context_List_Update_Add();

    // o_C_Check.m_string_Name_Function = "_M_Fn_Context_List_Update_Add";
    // o_C_Check.m_vector_i_Check.clear();
    // o_C_Check.m_vector_i_Check.push_back(0);
    // o_C_Check.m_vector_i_Check.push_back(0);
    // o_C_Check.m_vector_i_Check.push_back(1);
    // g_function_Check = o_C_Check;
    // o_C_Mgr_Data._M_Fn_Context_List_Update_Add();


    // C_Mgr_Data::_M_Fn_Context_List_Update_Commit
    // o_C_Check.m_string_Name_Function = "_M_Fn_Context_List_Update_Commit";
    // o_C_Check.m_vector_i_Check.clear();
    // o_C_Check.m_vector_i_Check.push_back(1);
    // o_C_Check.m_vector_i_Check.push_back(1);
    // g_function_Check = o_C_Check;
    // o_C_Mgr_Data._M_Fn_Context_List_Update_Commit();

    // o_C_Check.m_string_Name_Function = "_M_Fn_Context_List_Update_Commit";
    // o_C_Check.m_vector_i_Check.clear();
    // o_C_Check.m_vector_i_Check.push_back(1);
    // o_C_Check.m_vector_i_Check.push_back(0);
    // o_C_Check.m_vector_i_Check.push_back(1);
    // g_function_Check = o_C_Check;
    // o_C_Mgr_Data._M_Fn_Context_List_Update_Commit();

    g_function_Check = Fn_Check_Real;
}

void C_Vr_Asr_Impl_Gtest::M_FN_Vocon_Data_Party()
{
    // test Vr_Asr_Impl_Vocon_Data_Party.cpp

    g_function_Check = Fn_Check_Real;

    C_Check o_C_Check;
    o_C_Check.m_i_Pos = 0;
    o_C_Check.m_b_Specify_Function = true;


    // C_Data_Party
    C_Engine_Agent o_C_Engine_Agent;
    boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    C_Engine_Concrete o_C_Engine_Concrete(o_C_Engine_Agent, "aaa", function_On_Event_Init_Notify);
    C_Data_Party o_C_Data_Party(o_C_Engine_Concrete, "xxx");

    list<pair<string, string> > list_str_str;
    list_str_str.push_back(pair<string, string>("aa", "bb"));
    shared_ptr<C_Data_Context> sp_C_Data_Context(
        new C_Data_Context(o_C_Data_Party, "xx", "yy", "zz", list_str_str));
    o_C_Data_Party.m_map_string_spo_Data_Context.insert(
        pair<string, shared_ptr<C_Data_Context> >("sss", sp_C_Data_Context));

    shared_ptr<C_Data_Rule> sp_C_Data_Rule(new C_Data_Rule(o_C_Data_Party));
    o_C_Data_Party.m_map_string_spo_Data_Rule.insert(
        pair<string, shared_ptr<C_Data_Rule> >("abc", sp_C_Data_Rule));


    // C_Data_Party::M_Fn_Init
    o_C_Check.m_string_Name_Function = "M_Fn_Init";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party.M_Fn_Init();

    o_C_Check.m_string_Name_Function = "M_Fn_Init";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party.M_Fn_Init();

    o_C_Check.m_string_Name_Function = "M_Fn_Init";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party.M_Fn_Init();

    o_C_Check.m_string_Name_Function = "M_Fn_Init";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party.M_Fn_Init();

    o_C_Check.m_string_Name_Function = "M_Fn_Init";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party.M_Fn_Init();


    // C_Data_Party::M_Fn_Fina
    o_C_Check.m_string_Name_Function = "M_Fn_Fina";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party.M_Fn_Fina();

    o_C_Check.m_string_Name_Function = "M_Fn_Fina";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party.M_Fn_Fina();


    // C_Data_Party::M_Fn_Dialog_Unload
    C_Request_Recognize o_C_Request_Recognize;
    o_C_Check.m_string_Name_Function = "M_Fn_Dialog_Unload";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party.M_Fn_Dialog_Unload(o_C_Request_Recognize);


    // C_Data_Party::M_Fn_Find_po_Data_Tag
    o_C_Check.m_string_Name_Function = "M_Fn_Find_po_Data_Tag";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party.M_Fn_Find_po_Data_Tag(1);


    // C_Data_Party::M_Fn_Find_po_Data_Rule
    o_C_Check.m_string_Name_Function = "M_Fn_Find_po_Data_Rule";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party.M_Fn_Find_po_Data_Rule("abc");

    o_C_Check.m_string_Name_Function = "M_Fn_Find_po_Data_Rule";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    g_function_Check = o_C_Check;
    o_C_Data_Party.M_Fn_Find_po_Data_Rule("abc");


    // C_Data_Party::M_Fn_Find_po_Data_Context
    o_C_Check.m_string_Name_Function = "M_Fn_Find_po_Data_Context";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party.M_Fn_Find_po_Data_Context("aa");


    // C_Data_Party::_M_Fn_Info_Get_Path_Data_Party_Folder
    o_C_Check.m_string_Name_Function = "_M_Fn_Info_Get_Path_Data_Party_Folder";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party._M_Fn_Info_Get_Path_Data_Party_Folder("aa", "bb");

    o_C_Check.m_string_Name_Function = "_M_Fn_Info_Get_Path_Data_Party_Folder";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party._M_Fn_Info_Get_Path_Data_Party_Folder("aa", "bb");

    o_C_Check.m_string_Name_Function = "_M_Fn_Info_Get_Path_Data_Party_Folder";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party._M_Fn_Info_Get_Path_Data_Party_Folder("aa", "bb");

    o_C_Check.m_string_Name_Function = "_M_Fn_Info_Get_Path_Data_Party_Folder";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    g_function_Check = o_C_Check;
    o_C_Data_Party._M_Fn_Info_Get_Path_Data_Party_Folder("aa", "bb");

    o_C_Check.m_string_Name_Function = "_M_Fn_Info_Get_Path_Data_Party_Folder";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party._M_Fn_Info_Get_Path_Data_Party_Folder("aa", "bb");

    o_C_Check.m_string_Name_Function = "_M_Fn_Info_Get_Path_Data_Party_Folder";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    g_function_Check = o_C_Check;
    o_C_Data_Party._M_Fn_Info_Get_Path_Data_Party_Folder("aa", "bb");


    // C_Data_Party::_M_Fn_Info_Parse_File
    o_C_Check.m_string_Name_Function = "_M_Fn_Info_Parse_File";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party._M_Fn_Info_Parse_File();

    o_C_Check.m_string_Name_Function = "_M_Fn_Info_Parse_File";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party._M_Fn_Info_Parse_File();

    // o_C_Check.m_string_Name_Function = "_M_Fn_Info_Parse_File";
    // o_C_Check.m_vector_i_Check.clear();
    // o_C_Check.m_vector_i_Check.push_back(0);
    // o_C_Check.m_vector_i_Check.push_back(0);
    // o_C_Check.m_vector_i_Check.push_back(0);
    // o_C_Check.m_vector_i_Check.push_back(1);
    // o_C_Check.m_vector_i_Check.push_back(1);
    // g_function_Check = o_C_Check;
    // o_C_Data_Party._M_Fn_Info_Parse_File();

    o_C_Check.m_string_Name_Function = "_M_Fn_Info_Parse_File";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    g_function_Check = o_C_Check;
    o_C_Data_Party._M_Fn_Info_Parse_File();


    // C_Data_Party::_M_Fn_Info_Parse_Dialog
    ifstream ifstream_file;

    o_C_Check.m_string_Name_Function = "_M_Fn_Info_Parse_Dialog";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party._M_Fn_Info_Parse_Dialog(ifstream_file);

    o_C_Check.m_string_Name_Function = "_M_Fn_Info_Parse_Dialog";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party._M_Fn_Info_Parse_Dialog(ifstream_file);

    o_C_Check.m_string_Name_Function = "_M_Fn_Info_Parse_Dialog";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party._M_Fn_Info_Parse_Dialog(ifstream_file);

    o_C_Check.m_string_Name_Function = "_M_Fn_Info_Parse_Dialog";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party._M_Fn_Info_Parse_Dialog(ifstream_file);

    o_C_Check.m_string_Name_Function = "_M_Fn_Info_Parse_Dialog";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party._M_Fn_Info_Parse_Dialog(ifstream_file);

    o_C_Check.m_string_Name_Function = "_M_Fn_Info_Parse_Dialog";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    g_function_Check = o_C_Check;
    o_C_Data_Party._M_Fn_Info_Parse_Dialog(ifstream_file);

    o_C_Check.m_string_Name_Function = "_M_Fn_Info_Parse_Dialog";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party._M_Fn_Info_Parse_Dialog(ifstream_file);

    o_C_Check.m_string_Name_Function = "_M_Fn_Info_Parse_Dialog";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(0);
    o_C_Check.m_vector_i_Check.push_back(1);
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party._M_Fn_Info_Parse_Dialog(ifstream_file);


    // C_Data_Party::_M_Fn_Info_Parse_Context
    ifstream ifstream_file_Context;

    o_C_Check.m_string_Name_Function = "_M_Fn_Info_Parse_Context";
    o_C_Check.m_vector_i_Check.clear();
    o_C_Check.m_vector_i_Check.push_back(1);
    g_function_Check = o_C_Check;
    o_C_Data_Party._M_Fn_Info_Parse_Context(ifstream_file_Context);



    g_function_Check = Fn_Check_Real;
}

#endif
/* EOF */