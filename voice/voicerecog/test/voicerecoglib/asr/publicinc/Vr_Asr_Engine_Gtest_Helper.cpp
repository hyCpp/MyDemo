/**
 * Copyright @ 2013 - 2015 Suntec Software(Shanghai) Co., Ltd.
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
#include "Nuance_Include.h"

#if defined(_WIN32) || defined(_WIN32_WCE)
#include "stdafx.h"
#else
#include "Vr_Asr_Impl_IFlytek_IFlytek_Mock.h"
#endif
#include "Win_Linux_Interface.h"
#include "Vr_Asr_Engine_Gtest_Helper.h"


// Mock

#include "Vr_Asr_Request_Recognize.h"
#include "Vr_Asr_Impl_Fn_Get_string_Path.h"
#include "Vr_Asr_Impl_Vocon_Vocon_Mock.h"

#include "Vr_Asr_Impl_Engine_Agent.h"
#include "Vr_Asr_Impl_Vocon_Data_Context.h"
#include "Vr_Asr_Impl_Vocon_Data_Party.h"
#include "Vr_Asr_Impl_Vocon_Mgr_Rec.h"
#include "Vr_Asr_Impl_Vocon_Engine_Concrete.h"
#include "Vr_Asr_Request_Activate.h"
#include "Vr_Asr_Impl_Vocon_Data_Node_Result.h"
#include "Vr_Asr_Impl_Vocon_Data_Context_In_Dialog.h"
#include "Vr_Asr_Impl_Vocon_Fn_Get_ro_LH_AUDIOIN_INTERFACE.h"
#include "Vr_Asr_Impl_Vocon_Fn_Get_ro_LH_HEAP_INTERFACE.h"
#include "Vr_Asr_Impl_Vocon_Data_Context_List.h"
#include "Vr_Asr_Impl_Vocon_Data_CLC.h"
#include "Vr_Asr_Impl_Vocon_Fn_Callback_LH_Error.h"
#include "Vr_Asr_Impl_Vocon_Result.h"

#include "Record_Log.h"
#include "AudioIn_Adapter.h"

#include "Vr_Asr_Impl_Gtest.h"

#ifndef MACRO_Dynamic_Debug

static std::string s_string_Language = "eng";

#if defined(_WIN32) || defined(_WIN32_WCE)
static std::string s_string_asr_test_data_path = "data/";
static std::string s_string_asr_data_path = "data/ASR/";
#else
static std::string s_string_asr_test_data_path = "/data/asr/";
static std::string s_string_asr_data_path = "/data/asr/ASR/";
#endif

#if defined(_WIN32) || defined(_WIN32_WCE)
wstring
Fn_Get_wstring_from_string1
(
    const string & rstring_I
)
{
    size_t sizeRV;
    size_t sizeDest = rstring_I.size() + 1;
    wchar_t * a_wcDest = new wchar_t[sizeDest];
    wmemset(a_wcDest, 0, sizeDest);
    string curLocale = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, "chs"); 
    mbstowcs_s(&sizeRV, a_wcDest, sizeDest, rstring_I.c_str(), rstring_I.size());
    setlocale(LC_ALL, curLocale.c_str());
    wstring result = a_wcDest;
    delete[] a_wcDest;
    return result;
}

#endif


static const int s_int_time_wait = 100; // ms
static const int s_int_loop_times = 20;

using N_Vr::N_Asr::C_Event_Init_Notify;
/**
 * @brief The C_Get_Path_Mock class
 *
 * C_Get_Path_Mock class
 */

class C_Get_Path_Mock : public N_Vr::N_Asr::N_Impl::C_Get_Path
{
public:

    virtual
    ~C_Get_Path_Mock() // [F]Destructor
    {
    }

    explicit
    C_Get_Path_Mock() // [F]Constructor of Default
    {
    }

    virtual
    string
    M_Fn_Get_string_Path_VData()
    {
#if defined(_WIN32) || defined(_WIN32_WCE)
        return "data/ASR/";
#else
        return "/data/asr/ASR/";
#endif
    }

    virtual
    string
    M_Fn_Get_string_Path_PData()
    {
#if defined(_WIN32) || defined(_WIN32_WCE)
        return "data/ASR/";
#else
        return "/data/asr/ASR/";
#endif
    }

    virtual
    string
    M_Fn_Get_string_Path_Usr()
    {
#if defined(_WIN32) || defined(_WIN32_WCE)
        return "data/ASR/";
#else
        return "/data/asr/ASR/";
#endif
    }

protected:

    C_Get_Path_Mock // [F]Constructor: Copy
    (
        C_Get_Path const & // [I]
    );

    C_Get_Path_Mock & // [R]
    operator = // [F]Operator: =
    (
        C_Get_Path_Mock const & // [I]
    );
};

bool
Asr_Engine_Test_Helper::Prepare_AudioIn()
{
    m_o_Audio_In.reset(new C_AudioIn_Adapter());
    if (NULL == m_o_Audio_In.get()) {
        printf("[GTEST] ERROR: Failed to create aduoin!\n");
        return false;
    }

    C_AudioIn_Adapter* p_AudioIn_Adapter = dynamic_cast<C_AudioIn_Adapter*>(m_o_Audio_In.get());
    if (NULL == p_AudioIn_Adapter) {
        printf("[GTEST] ERROR: Failed to get original audio!\n");
        return false;
    }

    int res = p_AudioIn_Adapter->M_Fn_Open_WavFile(s_string_asr_test_data_path + "huibo.wav");
    if (0 != res) {
        printf("[GTEST] ERROR: Failed to open wav file!\n");
        return false;
    }

    return true;
}

bool
Asr_Engine_Test_Helper::Prepare_Data()
{
    C_File_Resolve file_resovle;

    // ===== m_set_dialog_id =====
    string string_Party_Dialog_Info = s_string_asr_data_path;
    string_Party_Dialog_Info += "Party_Origin/Language_" + s_string_Language + "/Info.dat";
    int n_ret_value = file_resovle.Resolve_Info_Data(string_Party_Dialog_Info, "DIALOG", "", m_set_dialog_id);
    if (0 != n_ret_value) {
        printf("[GTEST] ERROR: Failed to Resolve_Info_Data: %s!\n", string_Party_Dialog_Info.c_str());
        return false;
    }

    printf("[GTEST] dialog size = %zu\n", m_set_dialog_id.size());

    // ===== m_piar_list_context =====
    string string_Path_File_List_Context_Path = s_string_asr_test_data_path;
    string_Path_File_List_Context_Path += "listContextData/Party_Origin/";
    string_Path_File_List_Context_Path += "Language_" + s_string_Language + "/Context_Info/";

    std::set<std::string> set_list_context_files;
    m_o_Engine_Test.M_Fn_Get_Directory_Files(string_Path_File_List_Context_Path, set_list_context_files);

    typedef std::set<std::string>::iterator SetIter;
    SetIter set_list_context_file_end_iter = set_list_context_files.end();
    for (
        SetIter set_list_context_file_iter = set_list_context_files.begin();
        set_list_context_file_iter != set_list_context_file_end_iter;
        ++set_list_context_file_iter
    ) {
        n_ret_value = file_resovle.Resovle_List_Context(*set_list_context_file_iter, m_piar_list_context);
        if (0 != n_ret_value) {
            printf("[GTEST] ERROR: Failed to Resovle_List_Context!\n");
            return false;
        }
    }

    printf("[GTEST] static/dynamic context size = %zu\n", m_piar_list_context.size());

    return true;
}

void
Asr_Engine_Test_Helper::Recognize_Loop(N_Vr::N_Asr::E_Type_Engine e_Type_Engine)
{
    N_Vr::N_Asr::N_Impl::g_scp_o_Get_Path.reset(new C_Get_Path_Mock);

    N_Vr::N_Asr::C_Request_Factory o_Request_Factory;

    int i_param_index = 0;
    int i_param_index_end = 0;

    switch (e_Type_Engine) {
    case N_Vr::N_Asr::E_Type_Engine_Vocon:
    case N_Vr::N_Asr::E_Type_Engine_Vocon_Via_Shared:
    {
        M_Fn_Vocon_Test(e_Type_Engine);
        return;
    }
    case N_Vr::N_Asr::E_Type_Engine_iFlytek:
    {
#if defined(_WIN32) || defined(_WIN32_WCE)

#else
        o_Request_Factory.m_string_Id_Language = N_Vr::N_Asr::g_string_Iss_Sr_Acous_Lang_English;

        o_Request_Factory.m_function_On_Event_Init_Notify = boost::bind(&C_Engine_Test::M_Fn_On_Event_Init_Notify, &m_o_Engine_Test, _1);
        
        i_param_index = N_Vr::N_Asr::N_Impl::N_IFlyTek::C_IFlyTek_Mock::E_Param_Begin;
        i_param_index_end = N_Vr::N_Asr::N_Impl::N_IFlyTek::C_IFlyTek_Mock::E_Param_End;
#endif
        break;
    }
    default:
    {
        o_Request_Factory.m_string_Id_Language = s_string_Language;

        i_param_index = N_Vr::N_Asr::N_Impl::N_Vocon::C_Vocon_Mock::E_Param_Begin;
        i_param_index_end = N_Vr::N_Asr::N_Impl::N_Vocon::C_Vocon_Mock::E_Param_End;
    }
    }

    o_Request_Factory.m_e_Type_Engine = e_Type_Engine;

    // ===== test real engine for once =====
    _Do_One_Test(true, e_Type_Engine, o_Request_Factory, 0, 0);

    // ===== test mock engine in a loop =====
    for (i_param_index; i_param_index != i_param_index_end; ++i_param_index) {
        _Do_One_Test(false, e_Type_Engine, o_Request_Factory, i_param_index, 0);
    }
}


// =====  Private Member Function =====
void
Asr_Engine_Test_Helper::_Mock_Recognize_Engine(N_Vr::N_Asr::E_Type_Engine e_Type_Engine, int i_param_index, int i_loop_index)
{
    switch (e_Type_Engine) {
    case N_Vr::N_Asr::E_Type_Engine_iFlytek:
    {
#if defined(_WIN32) || defined(_WIN32_WCE)

#else
        N_Vr::N_Asr::N_Impl::N_IFlyTek::C_IFlyTek_Mock * po_IFlyTek_Engine_Mock = new N_Vr::N_Asr::N_Impl::N_IFlyTek::C_IFlyTek_Mock;
        po_IFlyTek_Engine_Mock->vector_b_Param[i_param_index] = true;
        N_Vr::N_Asr::N_Impl::N_IFlyTek::g_scp_o_IFlyTek.reset(po_IFlyTek_Engine_Mock);
        po_IFlyTek_Engine_Mock->M_Fn_Init();
        Sleep_WinLinux(10);
#endif
        break;
    }
    default:
    {
        N_Vr::N_Asr::N_Impl::N_Vocon::C_Vocon_Mock * po_Nuance_Engine_Mock = new N_Vr::N_Asr::N_Impl::N_Vocon::C_Vocon_Mock;
        po_Nuance_Engine_Mock->vector_b_Param[i_param_index] = true;
        po_Nuance_Engine_Mock->i_Num_Trigger = i_loop_index;
        N_Vr::N_Asr::N_Impl::N_Vocon::g_scp_o_Vocon.reset(po_Nuance_Engine_Mock);
    }
    }
}

void
Asr_Engine_Test_Helper::_Context_List_Update()
{
    printf("[GTEST]  >> _Context_List_Update\n");

    N_Vr::N_Asr::C_Request_Context_List_Update ro_Request_Context_List_Update;

    typedef map<string, C_List_Context_Item>::const_iterator MapIter;
    MapIter list_context_end_iter = m_piar_list_context.end();
    for (
        MapIter list_context_iter = m_piar_list_context.begin();
        list_context_iter != list_context_end_iter;
        ++list_context_iter
    ) {
        ro_Request_Context_List_Update.m_string_Id_Party = "Origin";
        ro_Request_Context_List_Update.m_string_Id_Context = list_context_iter->second.m_string_list_context_name;

        ro_Request_Context_List_Update.m_list_spo_Term_For_Add.assign(
            list_context_iter->second.m_list_context_add_item.begin(),
            list_context_iter->second.m_list_context_add_item.end());

        ro_Request_Context_List_Update.m_list_spo_Term_For_Delete.assign(
            list_context_iter->second.m_list_context_del_item.begin(),
            list_context_iter->second.m_list_context_del_item.end());

        ro_Request_Context_List_Update.m_function_On_Event_Notify = boost::bind(&C_Engine_Test::M_Fn_Add_List_Context_On_Event_Notify, &m_o_Engine_Test, _1);
        ro_Request_Context_List_Update.m_function_On_Event_Phase = boost::bind(&C_Engine_Test::M_Fn_Add_List_Context_On_Event_Phase, &m_o_Engine_Test, _1);
        ro_Request_Context_List_Update.m_b_Clear = true;

        ++m_o_Engine_Test.m_i_list_add_num;
        N_Vr::N_Asr::E_Result e_Result = m_scpo_Engine->Context_List_Update_Sync(ro_Request_Context_List_Update);
        if (N_Vr::N_Asr::E_Result_Succeeded != e_Result) {
            printf("[GTEST] ERROR: Failed to update context list!\n");
        }
    }

    printf("[GTEST] Context list update sleep %d * ms\n", s_int_time_wait);
    Sleep_WinLinux(5);
    printf("[GTEST]  << _Context_List_Update\n");
}

void
Asr_Engine_Test_Helper::_Request_Recognize(string string_Id_Dialog, bool b_Barge_In)
{
    printf("[GTEST]  >> _Request_Recognize\n");

    static unsigned int i_rec_flag = 0;
    m_o_Engine_Test.m_b_Rec_Start = false;

    N_Vr::N_Asr::C_Request_Recognize o_Request_Recognize;

    o_Request_Recognize.m_b_Barge_In = b_Barge_In;
    o_Request_Recognize.m_string_Id_Party = "Origin";
    o_Request_Recognize.m_string_Id_Dialog = string_Id_Dialog;
    o_Request_Recognize.m_string_Id_Mode = "local";

    boost::shared_ptr<N_Vr::N_Asr::C_Term> item_one(new N_Vr::N_Asr::C_Term);
    item_one->m_i_User_Data_Lo = 1;
    item_one->m_string_Orthography = "rubby";
    o_Request_Recognize.m_list_spo_Term_Content_Local_Cmd.push_back(item_one);
    boost::shared_ptr<N_Vr::N_Asr::C_Term> item_two(new N_Vr::N_Asr::C_Term);
    item_two->m_i_User_Data_Lo = 2;
    item_two->m_string_Orthography = "";
    o_Request_Recognize.m_list_spo_Term_Content_Local_Cmd.push_back(item_two);

    if (0 != i_rec_flag) {
        o_Request_Recognize.m_string_Longtitude = "test";
        o_Request_Recognize.m_string_Latitude = "test";
        o_Request_Recognize.m_string_City = "test";
        o_Request_Recognize.m_string_Custom = "test";
    }
    
    ++i_rec_flag;
    if (i_rec_flag % 3 == 0) {
        o_Request_Recognize.m_string_Id_Mode = "local";

    }
    else if (i_rec_flag % 3 == 1) {
        o_Request_Recognize.m_string_Id_Mode = "";

    }
    else {
        o_Request_Recognize.m_string_Longtitude = "";
        o_Request_Recognize.m_string_Latitude = "";
        o_Request_Recognize.m_string_City = "";
        o_Request_Recognize.m_string_Custom = "";
    }
    list<boost::shared_ptr<N_Vr::N_Asr::C_Request_Activate> > list_spo_Request_Activate;
    o_Request_Recognize.m_list_spo_Request_Activate = list_spo_Request_Activate;

    o_Request_Recognize.m_spo_Audio_In = m_o_Audio_In;
    o_Request_Recognize.m_function_On_Event_Phase = boost::bind(&C_Engine_Test::M_Fn_Recognize_On_Event_Phase, &m_o_Engine_Test, _1);
    o_Request_Recognize.m_function_On_Event_Notify = boost::bind(&C_Engine_Test::M_Fn_Recognize_On_Event_Notify, &m_o_Engine_Test, _1);
    o_Request_Recognize.m_function_On_Event_Result = boost::bind(&C_Engine_Test::M_Fn_Recognize_On_Event_Result, &m_o_Engine_Test, _1);
    o_Request_Recognize.m_b_Measure_Time = false;

    printf("[GTEST] Recognize dialog: %s\n", string_Id_Dialog.c_str());
    // int e_Result = m_scpo_Engine->Recognize_Start_Sync(o_Request_Recognize);
    m_scpo_Engine->Recognize_Start_Sync(o_Request_Recognize);

    if (i_rec_flag % 4 == 1) {
        printf("[GTEST] Recognize cancel\n");
        Sleep_WinLinux(5);
        m_scpo_Engine->Recognize_Cancel();
    }
    // printf("[GTEST] Recognize sleep %d ms\n", s_int_time_wait);
    // usleep(s_int_time_wait * 1000);

    printf("[GTEST]  << _Request_Recognize\n");
}

void
Asr_Engine_Test_Helper::_Do_One_Test(
    bool b_use_real_engine,
    N_Vr::N_Asr::E_Type_Engine e_Type_Engine,
    N_Vr::N_Asr::C_Request_Factory& o_Request_Factory,
    int i_param_index,
    int i_loop_index
)
{
    // ===== mock engine =====
    if (!b_use_real_engine) {
        // Mock recognize interface
        _Mock_Recognize_Engine(e_Type_Engine, i_param_index, i_loop_index);
    }

#if defined(_WIN32) || defined(_WIN32_WCE)
    TRACE2("[GTEST] >> Recognize index = %d, loop = %d.\n", i_param_index, i_loop_index);
#else
    printf("[GTEST] >> Recognize index = %d, loop = %d.\n", i_param_index, i_loop_index);
#endif

    // ===== Engine factory =====
    m_scpo_Engine.reset(N_Vr::N_Asr::C_Engine::Factory(o_Request_Factory));
    Sleep_WinLinux(10);

    if (!m_scpo_Engine) {
        printf("[GTEST] ERROR: Asr engine factory error.\n");
        printf("[GTEST] << Recognize index = %d, loop = %d.\n", i_param_index, i_loop_index);
        return;
    }

    // ===== Engine initialize =====
    m_o_Engine_Test.M_Fn_Initialize();

    // ===== Engine context list update =====
    _Context_List_Update();

    // ===== mock engine =====
    if (!b_use_real_engine) {
    for (int i = N_Vr::N_Asr::E_Param_Type_None; i != N_Vr::N_Asr::E_Param_Type_End; ++i) {
        N_Vr::N_Asr::C_Request_Param_Set o_Request_Param_Set;
        o_Request_Param_Set.m_e_Param_Type = static_cast<N_Vr::N_Asr::E_Param_Type>(i);
        o_Request_Param_Set.m_i_Value = 0;
        m_scpo_Engine->Param_Set(o_Request_Param_Set);
    }
    }

    // ===== Engine request =====
    switch (e_Type_Engine) {
    case N_Vr::N_Asr::E_Type_Engine_Vocon_Via_Shared:
    {
        _Request_Recognize("topmenu_idle", false);
        break;
    }
    default:
    {
#if 0
        std::set<std::string>::iterator m_set_dialog_id_end_iter = m_set_dialog_id.end();
        for (
            std::set<std::string>::iterator m_set_dialog_id_iter = m_set_dialog_id.begin();
            m_set_dialog_id_iter != m_set_dialog_id_end_iter;
            ++m_set_dialog_id_iter
        ) {
            _Request_Recognize(*m_set_dialog_id_iter, false);
        }
#endif
        _Request_Recognize("topmenu_idle", false);
        if (e_Type_Engine == N_Vr::N_Asr::E_Type_Engine_iFlytek) {
            Sleep_WinLinux(50);
        }
        
        _Request_Recognize("topmenu_idle", true);
        if (e_Type_Engine == N_Vr::N_Asr::E_Type_Engine_iFlytek) {
            Sleep_WinLinux(50);
        }
        
        _Request_Recognize("navi_idle", false);
        if (e_Type_Engine == N_Vr::N_Asr::E_Type_Engine_iFlytek) {
            Sleep_WinLinux(50);
        }
        
        _Request_Recognize("voice_tag_record", false);
        if (e_Type_Engine == N_Vr::N_Asr::E_Type_Engine_iFlytek) {
            Sleep_WinLinux(50);
        }
        
        _Request_Recognize("wrong", false);
    }
    }

    if (e_Type_Engine == N_Vr::N_Asr::E_Type_Engine_iFlytek) {
        Sleep_WinLinux(150);
    }
    
    // ===== Engine uninitialize =====
    m_o_Engine_Test.M_Fn_UnInitialize();
    m_scpo_Engine.reset();

    printf("[GTEST] << Recognize loop = %d.\n", i_param_index);
}

void
Asr_Engine_Test_Helper::M_Fn_Log
(
    string const & string_Log
)
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    TRACE1("%s\0", Fn_Get_wstring_from_string1(string_Log).c_str());
#else
    printf(string_Log.c_str());
#endif
}

void
Asr_Engine_Test_Helper::M_Fn_Vocon_Test
(
    N_Vr::N_Asr::E_Type_Engine e_Type_Engine
)
{
    // Real Engine.

    M_Fn_Vocon_Test_Once(e_Type_Engine);

    // Mocked Engine Etc Case.

    N_Vr::N_Asr::N_Impl::N_Vocon::C_Vocon_Mock * po_Vocon_Mock = new N_Vr::N_Asr::N_Impl::N_Vocon::C_Vocon_Mock;
    po_Vocon_Mock->vector_b_Param[N_Vr::N_Asr::N_Impl::N_Vocon::C_Vocon_Mock::E_Param_All_OK] = true;
    po_Vocon_Mock->i_Num_Trigger = 0;
    N_Vr::N_Asr::N_Impl::N_Vocon::g_scp_o_Vocon.reset(po_Vocon_Mock);

    M_Fn_Vocon_Test_Etc();

    // Mocked Engine Loop Case.

    for (
        size_t i_Pos_Param = N_Vr::N_Asr::N_Impl::N_Vocon::C_Vocon_Mock::E_Param_Begin;
        i_Pos_Param != N_Vr::N_Asr::N_Impl::N_Vocon::C_Vocon_Mock::E_Param_End;
        ++i_Pos_Param
    ) {
        {
            stringstream stringstream_Log;
            stringstream_Log << "[GTEST] Vocon Mock Param: " << i_Pos_Param << '\n';
            M_Fn_Log(stringstream_Log.str());
        }

        for (
            size_t i_Pos_Trigger = 0;
            i_Pos_Trigger < 1;
            ++i_Pos_Trigger
        ) {
            {
                stringstream stringstream_Log;
                stringstream_Log << "[GTEST] Vocon Mock Trigger: " << i_Pos_Trigger << '\n';
                M_Fn_Log(stringstream_Log.str());
            }

            N_Vr::N_Asr::N_Impl::N_Vocon::C_Vocon_Mock * po_Vocon_Mock = new N_Vr::N_Asr::N_Impl::N_Vocon::C_Vocon_Mock;
            po_Vocon_Mock->vector_b_Param[i_Pos_Param] = true;
            po_Vocon_Mock->i_Num_Trigger = i_Pos_Trigger;
            N_Vr::N_Asr::N_Impl::N_Vocon::g_scp_o_Vocon.reset(po_Vocon_Mock);

            M_Fn_Vocon_Test_Once(e_Type_Engine);
        }
    }
}

void
Asr_Engine_Test_Helper::M_Fn_Vocon_Test_Once
(
    N_Vr::N_Asr::E_Type_Engine e_Type_Engine
)
{
    N_Vr::N_Asr::C_Request_Factory o_Request_Factory;

    o_Request_Factory.m_e_Type_Engine = e_Type_Engine;
    o_Request_Factory.m_string_Id_Language = "eng";

    m_scpo_Engine.reset(N_Vr::N_Asr::C_Engine::Factory(o_Request_Factory));
    if (!m_scpo_Engine) {
        M_Fn_Log("[GTEST] Factory error.\n");
        return;
    }

    N_Vr::N_Asr::C_Engine & ro_Engine = *m_scpo_Engine;

    m_o_Engine_Test.M_Fn_Initialize();

    {
        N_Vr::N_Asr::C_Request_Context_List_Update ro_Request_Context_List_Update;

        typedef map<string, C_List_Context_Item>::const_iterator MapIter;

        MapIter list_context_end_iter = m_piar_list_context.end();
        for (
            MapIter list_context_iter = m_piar_list_context.begin();
            list_context_iter != list_context_end_iter;
            ++list_context_iter
        ) {

            ro_Request_Context_List_Update.m_string_Id_Party = "Origin";
            ro_Request_Context_List_Update.m_string_Id_Context = list_context_iter->second.m_string_list_context_name;

            ro_Request_Context_List_Update.m_list_spo_Term_For_Add.assign(
            list_context_iter->second.m_list_context_add_item.begin(),
            list_context_iter->second.m_list_context_add_item.end()
        );

        ro_Request_Context_List_Update.m_list_spo_Term_For_Delete.assign(
            list_context_iter->second.m_list_context_del_item.begin(),
            list_context_iter->second.m_list_context_del_item.end()
        );

        ro_Request_Context_List_Update.m_function_On_Event_Notify = boost::bind(&C_Engine_Test::M_Fn_Add_List_Context_On_Event_Notify, &m_o_Engine_Test, _1);
        ro_Request_Context_List_Update.m_function_On_Event_Phase = boost::bind(&C_Engine_Test::M_Fn_Add_List_Context_On_Event_Phase, &m_o_Engine_Test, _1);
        ro_Request_Context_List_Update.m_b_Clear = true;

        ++m_o_Engine_Test.m_i_list_add_num;

        N_Vr::N_Asr::E_Result e_Result = ro_Engine.Context_List_Update_Sync(ro_Request_Context_List_Update);
        }
    }

    {
        N_Vr::N_Asr::C_Request_Param_Set o_Request_Param_Set;

        o_Request_Param_Set.m_e_Param_Type = N_Vr::N_Asr::E_Param_Type_Audio_Timeout;
        o_Request_Param_Set.m_i_Value = N_Vr::N_Asr::E_Param_Value_Default_Audio_Timeout;
        ro_Engine.Param_Set(o_Request_Param_Set);

        o_Request_Param_Set.m_e_Param_Type = N_Vr::N_Asr::E_Param_Type_Fx_Timeout_Speech;
        o_Request_Param_Set.m_i_Value = N_Vr::N_Asr::E_Param_Value_Default_Fx_Timeout_Speech;
        ro_Engine.Param_Set(o_Request_Param_Set);

        o_Request_Param_Set.m_e_Param_Type = N_Vr::N_Asr::E_Param_Type_Fx_Timeout_Silence_Leading;
        o_Request_Param_Set.m_i_Value = N_Vr::N_Asr::E_Param_Value_Default_Fx_Timeout_Silence_Leading;
        ro_Engine.Param_Set(o_Request_Param_Set);

        o_Request_Param_Set.m_e_Param_Type = N_Vr::N_Asr::E_Param_Type_Fx_Timeout_Silence_Trailing;
        o_Request_Param_Set.m_i_Value = N_Vr::N_Asr::E_Param_Value_Default_Fx_Timeout_Silence_Trailing;
        ro_Engine.Param_Set(o_Request_Param_Set);

        o_Request_Param_Set.m_e_Param_Type = N_Vr::N_Asr::E_Param_Type_Fx_Sensitivity;
        o_Request_Param_Set.m_i_Value = N_Vr::N_Asr::E_Param_Value_Default_Fx_Sensitivity;
        ro_Engine.Param_Set(o_Request_Param_Set);

        o_Request_Param_Set.m_e_Param_Type = N_Vr::N_Asr::E_Param_Type_Fx_Absolute_Threshold;
        o_Request_Param_Set.m_i_Value = N_Vr::N_Asr::E_Param_Value_Default_Fx_Absolute_Threshold;
        ro_Engine.Param_Set(o_Request_Param_Set);

        o_Request_Param_Set.m_e_Param_Type = N_Vr::N_Asr::E_Param_Type_Fx_Minspeech;
        o_Request_Param_Set.m_i_Value = N_Vr::N_Asr::E_Param_Value_Default_Fx_Minspeech;
        ro_Engine.Param_Set(o_Request_Param_Set);

        o_Request_Param_Set.m_e_Param_Type = N_Vr::N_Asr::E_Param_Type_Rec_Firstpass_Distapprox;
        o_Request_Param_Set.m_i_Value = N_Vr::N_Asr::E_Param_Value_Default_Rec_Firstpass_Distapprox;
        ro_Engine.Param_Set(o_Request_Param_Set);

        o_Request_Param_Set.m_e_Param_Type = N_Vr::N_Asr::E_Param_Type_Fx_Event_Timer_Interval_Millis;
        o_Request_Param_Set.m_i_Value = N_Vr::N_Asr::E_Param_Value_Default_Fx_Event_Timer_Interval_Millis;
        ro_Engine.Param_Set(o_Request_Param_Set);
    }

    _Request_Recognize("topmenu_idle", true);

    _Request_Recognize("topmenu_idle", false);

    _Request_Recognize("voice_tag_record", false);

    _Request_Recognize("wrong", false);

    m_o_Engine_Test.M_Fn_UnInitialize();

    m_scpo_Engine.reset();
}

void
Asr_Engine_Test_Helper::M_Fn_Vocon_Test_Etc()
{
    C_Vr_Asr_Impl_Gtest o_C_Vr_Asr_Impl_Gtest;
    o_C_Vr_Asr_Impl_Gtest.M_Fn_Run_All_Gtest();

    M_Fn_Vocon_Test_Vr_Asr_Impl_Vocon_Data_Context();

    M_Fn_Vocon_Test_Vr_Asr_Impl_Vocon_Mgr_Rec();

    M_Fn_Vocon_Test_Vr_Asr_Impl_Vocon_Data_Party();

    M_Fn_Vocon_Test_Fn_Get_ro_LH_AUDIOIN_INTERFACE();

    M_Fn_Vocon_Test_Fn_Get_ro_LH_HEAP_INTERFACE();

    M_Fn_Vocon_Test_Vr_Asr_Impl_Vocon_Data_CLC();

    M_Fn_Vocon_Test_Vr_Asr_Impl_Vocon_Fn_Callback_LH_Error();

    g_function_Check = Fn_Check_Real;
}

void
Asr_Engine_Test_Helper::M_Fn_Vocon_Test_Vr_Asr_Impl_Vocon_Data_Context()
{
    M_Fn_Log("[GTEST] Vr_Asr_Impl_Vocon_Data_Context.\n");

    {
        N_Vr::N_Asr::N_Impl::C_Check o_Check;
        o_Check.m_b_Specify_Function = true;

        g_function_Check = Fn_Check_Real;

        // ****

        N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
        boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
        N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);

        N_Vr::N_Asr::N_Impl::N_Vocon::C_Data_Party o_Data_Party(
            o_Engine_Concrete,
            ""
        );

        list<pair<string, string> > a;

        N_Vr::N_Asr::N_Impl::N_Vocon::C_Data_Context o_Data_Context(
            o_Data_Party,
            "",
            "",
            "",
            a
        );

        // ****

        {
            list<shared_ptr<N_Vr::N_Asr::C_Request_Activate> > a;
            g_function_Check = Fn_Check_Real;
            o_Data_Context.M_Fn_Init();
            o_Data_Context.M_Fn_Fina();
            o_Data_Context.M_Fn_Load(a);
            o_Data_Context.M_Fn_Unload();
            o_Data_Context.M_Fn_Slot_Guest_Load();
            o_Data_Context.M_Fn_Slot_Guest_Unload();
            o_Data_Context.M_Fn_Merge_Host_Merge(NULL);
            o_Data_Context._M_Fn_Obj_Activate_Field(a);
            o_Data_Context._M_Fn_Obj_Activate_Fuzzy(a);
        }

        // ****

        o_Check.m_string_Name_Function = "M_Fn_Merge_Host_Clear";
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        g_function_Check = o_Check;
        o_Data_Context.M_Fn_Merge_Host_Clear();

        // ****

        o_Check.m_string_Name_Function = "M_Fn_Rec_Add";
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Data_Context.M_Fn_Rec_Add();

        // ****

        o_Check.m_string_Name_Function = "M_Fn_Rec_Remove";
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Data_Context.M_Fn_Rec_Add();

        // ****

        o_Check.m_string_Name_Function = "M_Fn_List_Update_Clear";
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Data_Context.M_Fn_List_Update_Clear();

        // ****

        o_Check.m_string_Name_Function = "M_Fn_List_Update_Clear";
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Data_Context.M_Fn_List_Update_Clear();

        // ****

        {
            N_Vr::N_Asr::C_Term o_Term;
            o_Check.m_string_Name_Function = "M_Fn_List_Update_Delete";
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context.M_Fn_List_Update_Delete(o_Term);
        }

        // ****

        {
            N_Vr::N_Asr::C_Term o_Term;
            o_Check.m_string_Name_Function = "M_Fn_List_Update_Delete";
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context.M_Fn_List_Update_Delete(o_Term);
        }

        // ****

        {
            o_Check.m_string_Name_Function = "M_Fn_List_Update_Commit";
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context.M_Fn_List_Update_Commit();

            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context.M_Fn_List_Update_Commit();

            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            g_function_Check = o_Check;
            o_Data_Context.M_Fn_List_Update_Commit();

        }

        // ****

        {
            o_Check.m_string_Name_Function = "_M_Fn_Obj_Create_From_File";
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context._M_Fn_Obj_Create_From_File();

            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            g_function_Check = o_Check;
            o_Data_Context._M_Fn_Obj_Create_From_File();

        }

        // ****

        {
            o_Check.m_string_Name_Function = "_M_Fn_Obj_Create_List";

            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context._M_Fn_Obj_Create_List();

            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context._M_Fn_Obj_Create_List();

            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            g_function_Check = o_Check;
            o_Data_Context._M_Fn_Obj_Create_List();

        }

        // ****

        {
            o_Check.m_string_Name_Function = "_M_Fn_Obj_Create_Userword";

            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context._M_Fn_Obj_Create_Userword();

            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context._M_Fn_Obj_Create_Userword();

            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            g_function_Check = o_Check;
            o_Data_Context._M_Fn_Obj_Create_Userword();

        }

        // ****

        {
            o_Check.m_string_Name_Function = "_M_Fn_Obj_Destroy";
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context._M_Fn_Obj_Destroy();
        }

        // ****

        {
            printf("M_Fn_List_Update_Add\n");
            N_Vr::N_Asr::C_Buffer o_Buffer;
            o_Buffer.m_sai_Buffer.reset(new unsigned char[3]);
            o_Buffer.m_i_Size = 3;

            N_Vr::N_Asr::C_Term o_Term;
            o_Term.m_string_Orthography = "test";
            o_Term.m_vector_buffer_Transcriptions.push_back(o_Buffer);

            LH_OBJECT o_LH_OBJECT_CLC;
            LH_OBJECT o_LH_OBJECT_CLC_Lex;
            LH_OBJECT o_LH_OBJECT_CLC_Id_Trans;


            o_Check.m_string_Name_Function = "M_Fn_List_Update_Add";

            printf("----1----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context.M_Fn_List_Update_Add(o_Term, &o_LH_OBJECT_CLC, &o_LH_OBJECT_CLC_Lex, &o_LH_OBJECT_CLC_Id_Trans, 0);

            printf("----2----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context.M_Fn_List_Update_Add(o_Term, &o_LH_OBJECT_CLC, &o_LH_OBJECT_CLC_Lex, &o_LH_OBJECT_CLC_Id_Trans, 0);

            printf("----3----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context.M_Fn_List_Update_Add(o_Term, &o_LH_OBJECT_CLC, &o_LH_OBJECT_CLC_Lex, &o_LH_OBJECT_CLC_Id_Trans, 0);

            printf("----4----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context.M_Fn_List_Update_Add(o_Term, &o_LH_OBJECT_CLC, &o_LH_OBJECT_CLC_Lex, &o_LH_OBJECT_CLC_Id_Trans, 0);

            printf("----5----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context.M_Fn_List_Update_Add(o_Term, &o_LH_OBJECT_CLC, &o_LH_OBJECT_CLC_Lex, &o_LH_OBJECT_CLC_Id_Trans, 0);

            printf("----6----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context.M_Fn_List_Update_Add(o_Term, &o_LH_OBJECT_CLC, &o_LH_OBJECT_CLC_Lex, &o_LH_OBJECT_CLC_Id_Trans, 0);

            printf("----7----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context.M_Fn_List_Update_Add(o_Term, &o_LH_OBJECT_CLC, &o_LH_OBJECT_CLC_Lex, &o_LH_OBJECT_CLC_Id_Trans, 0);

            printf("----8----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context.M_Fn_List_Update_Add(o_Term, &o_LH_OBJECT_CLC, &o_LH_OBJECT_CLC_Lex, &o_LH_OBJECT_CLC_Id_Trans, 0);

            printf("----9----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context.M_Fn_List_Update_Add(o_Term, &o_LH_OBJECT_CLC, &o_LH_OBJECT_CLC_Lex, &o_LH_OBJECT_CLC_Id_Trans, 0);

            printf("----10----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context.M_Fn_List_Update_Add(o_Term, &o_LH_OBJECT_CLC, &o_LH_OBJECT_CLC_Lex, &o_LH_OBJECT_CLC_Id_Trans, 0);

            printf("----11----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context.M_Fn_List_Update_Add(o_Term, &o_LH_OBJECT_CLC, &o_LH_OBJECT_CLC_Lex, &o_LH_OBJECT_CLC_Id_Trans, 0);

            printf("----12----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false); 
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context.M_Fn_List_Update_Add(o_Term, &o_LH_OBJECT_CLC, &o_LH_OBJECT_CLC_Lex, &o_LH_OBJECT_CLC_Id_Trans, 0);

            printf("----13----\n");
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
            o_Data_Context.M_Fn_List_Update_Add(o_Term, &o_LH_OBJECT_CLC, &o_LH_OBJECT_CLC_Lex, &o_LH_OBJECT_CLC_Id_Trans, 0);

            printf("----14----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context.M_Fn_List_Update_Add(o_Term, &o_LH_OBJECT_CLC, &o_LH_OBJECT_CLC_Lex, &o_LH_OBJECT_CLC_Id_Trans, 0);

            printf("----15----\n");
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
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context.M_Fn_List_Update_Add(o_Term, &o_LH_OBJECT_CLC, &o_LH_OBJECT_CLC_Lex, &o_LH_OBJECT_CLC_Id_Trans, 0);

            printf("----16----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context.M_Fn_List_Update_Add(o_Term, &o_LH_OBJECT_CLC, &o_LH_OBJECT_CLC_Lex, &o_LH_OBJECT_CLC_Id_Trans, 0);

            printf("----17----\n");
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
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context.M_Fn_List_Update_Add(o_Term, &o_LH_OBJECT_CLC, &o_LH_OBJECT_CLC_Lex, &o_LH_OBJECT_CLC_Id_Trans, 0);
        }

        // *** 
        
        {
            printf("_M_Fn_Obj_Activate_Grammar\n");
            shared_ptr<N_Vr::N_Asr::C_Request_Activate> sp_Request_Activate_one;
            shared_ptr<N_Vr::N_Asr::C_Request_Activate> sp_Request_Activate_two;
            sp_Request_Activate_one.reset(new N_Vr::N_Asr::C_Request_Activate);
            sp_Request_Activate_two.reset(new N_Vr::N_Asr::C_Request_Activate);
            list<shared_ptr<N_Vr::N_Asr::C_Request_Activate> > list_spo_Request_Activate; 
            list_spo_Request_Activate.push_back(sp_Request_Activate_one);
            list_spo_Request_Activate.push_back(sp_Request_Activate_two);

            o_Check.m_string_Name_Function = "_M_Fn_Obj_Activate_Grammar";

            printf("----1----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context._M_Fn_Obj_Activate_Grammar(list_spo_Request_Activate);

            printf("----2----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context._M_Fn_Obj_Activate_Grammar(list_spo_Request_Activate);
            
            printf("----3----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context._M_Fn_Obj_Activate_Grammar(list_spo_Request_Activate);
            
            printf("----4----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context._M_Fn_Obj_Activate_Grammar(list_spo_Request_Activate);

            printf("----5----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context._M_Fn_Obj_Activate_Grammar(list_spo_Request_Activate);
        }

        // *** 
        o_Data_Context.m_list_pair_string_Id_Slot_string_Id_Context.push_back(std::make_pair("ida", "contexta"));
        o_Data_Context.m_list_pair_string_Id_Slot_string_Id_Context.push_back(std::make_pair("idb", "contextb"));

        {
            printf("_M_Fn_Obj_Slot_Host_Clear\n");
            o_Check.m_string_Name_Function = "_M_Fn_Obj_Slot_Host_Clear";
                        
            printf("----1----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context._M_Fn_Obj_Slot_Host_Clear();

            printf("----2----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context._M_Fn_Obj_Slot_Host_Clear();

            printf("----3----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context._M_Fn_Obj_Slot_Host_Clear();
        }

        // *** 
        
        {
            printf("_M_Fn_Obj_Slot_Host_Add\n");
            o_Check.m_string_Name_Function = "_M_Fn_Obj_Slot_Host_Add";
            
            printf("----1----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context._M_Fn_Obj_Slot_Host_Add();
#if 0
            printf("----2----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context._M_Fn_Obj_Slot_Host_Add();
            
            printf("----3----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context._M_Fn_Obj_Slot_Host_Add();
#endif
        }

        // *** 
        
        {
            printf("_M_Fn_Obj_Create\n");
            o_Check.m_string_Name_Function = "_M_Fn_Obj_Slot_Host_Add";
            
            printf("----1----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context._M_Fn_Obj_Slot_Host_Add();
#if 0
            printf("----2----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context._M_Fn_Obj_Slot_Host_Add();
            
            printf("----3----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context._M_Fn_Obj_Slot_Host_Add();
#endif
        }
    }
}

void
Asr_Engine_Test_Helper::M_Fn_Vocon_Test_Vr_Asr_Impl_Vocon_Mgr_Rec()
{
    M_Fn_Log("[GTEST] Vr_Asr_Impl_Vocon_Mgr_Rec.\n");

    {
        N_Vr::N_Asr::N_Impl::C_Check o_Check;
        o_Check.m_b_Specify_Function = true;

        g_function_Check = Fn_Check_Real;

        // ****

        N_Vr::N_Asr::C_Request_Recognize o_Request_Recognize;
        o_Request_Recognize.m_function_On_Event_Phase = boost::bind(&C_Engine_Test::M_Fn_Recognize_On_Event_Phase, &m_o_Engine_Test, _1);
        o_Request_Recognize.m_function_On_Event_Notify = boost::bind(&C_Engine_Test::M_Fn_Recognize_On_Event_Notify, &m_o_Engine_Test, _1);
        o_Request_Recognize.m_function_On_Event_Result = boost::bind(&C_Engine_Test::M_Fn_Recognize_On_Event_Result, &m_o_Engine_Test, _1);

        N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;

        boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
        N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);

        N_Vr::N_Asr::N_Impl::N_Vocon::C_Mgr_Rec o_C_Mgr_Rec(o_Engine_Concrete);
        o_C_Mgr_Rec.m_scpo_Request_Recognize.reset(
            MACRO_New N_Vr::N_Asr::C_Request_Recognize(
            o_Request_Recognize
            )
        );

        // ****
        printf("M_Fn_Init\n");
        o_Check.m_string_Name_Function = "M_Fn_Init";

        printf("----1----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_C_Mgr_Rec.M_Fn_Init();

        // ****
        printf("M_Fn_Fina\n");
        o_Check.m_string_Name_Function = "M_Fn_Fina";

        printf("----1----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_C_Mgr_Rec.M_Fn_Fina();

        printf("----2----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        g_function_Check = o_Check;
        o_C_Mgr_Rec.M_Fn_Fina();

        // ****
        printf("_M_Fn_Thread_Init\n");
        o_Check.m_string_Name_Function = "_M_Fn_Thread_Init";

        printf("----1----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_C_Mgr_Rec._M_Fn_Thread_Init();

        // ****
        printf("M_Fn_On_Thread_Notified\n");
        o_Check.m_string_Name_Function = "M_Fn_On_Thread_Notified";

        printf("----1----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        g_function_Check = o_Check;
        o_C_Mgr_Rec.M_Fn_On_Thread_Notified();

        printf("----2----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_C_Mgr_Rec.M_Fn_On_Thread_Notified();

        printf("----3----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
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
        o_C_Mgr_Rec.M_Fn_On_Thread_Notified();

        // ****
        printf("M_Fn_Start\n");
        o_Check.m_string_Name_Function = "M_Fn_Start";

        printf("----1----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_C_Mgr_Rec.M_Fn_Start(o_Request_Recognize);

        // ****
        printf("M_Fn_Cancel\n");
        o_Check.m_string_Name_Function = "M_Fn_Cancel";

        printf("----1----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        g_function_Check = o_Check;
        o_C_Mgr_Rec.M_Fn_Cancel();

        // ****

        {
            LH_OBJECT o_LH_OBJECT;
            printf("_M_Fn_Proc_On_Result_Userword\n");
            o_Check.m_string_Name_Function = "_M_Fn_Proc_On_Result_Userword";

            printf("----1----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_C_Mgr_Rec._M_Fn_Proc_On_Result_Userword(o_LH_OBJECT);

            printf("----2----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_C_Mgr_Rec._M_Fn_Proc_On_Result_Userword(o_LH_OBJECT);


            printf("----3----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            g_function_Check = o_Check;
            o_C_Mgr_Rec._M_Fn_Proc_On_Result_Userword(o_LH_OBJECT);
        }

        // ****
#if 0
        {
            LH_OBJECT o_LH_OBJECT;
            bool b_Voicetag_Alike = false;

            printf("_M_Fn_Proc_On_Result_Userword_Check\n");
            o_Check.m_string_Name_Function = "_M_Fn_Proc_On_Result_Userword_Check";

            printf("----1----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_C_Mgr_Rec._M_Fn_Proc_On_Result_Userword_Check(o_LH_OBJECT, b_Voicetag_Alike);

            printf("----2----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_C_Mgr_Rec._M_Fn_Proc_On_Result_Userword_Check(o_LH_OBJECT, b_Voicetag_Alike);

            printf("----3----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_C_Mgr_Rec._M_Fn_Proc_On_Result_Userword_Check(o_LH_OBJECT, b_Voicetag_Alike);

            printf("----4----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_C_Mgr_Rec._M_Fn_Proc_On_Result_Userword_Check(o_LH_OBJECT, b_Voicetag_Alike);
        }
#endif
        // ****

        {
            printf("_M_Fn_Proc_On_Result_Userword_Deal\n");
            o_Check.m_string_Name_Function = "_M_Fn_Proc_On_Result_Userword_Deal";

            printf("----1----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_C_Mgr_Rec._M_Fn_Proc_On_Result_Userword_Deal();

            printf("----2----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_C_Mgr_Rec._M_Fn_Proc_On_Result_Userword_Deal();

            printf("----3----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_C_Mgr_Rec._M_Fn_Proc_On_Result_Userword_Deal();
        }

        // ****

        {
            LH_OBJECT o_LH_OBJECT;

            printf("_M_Fn_Proc_On_Result_Userword_Transcription_Audio\n");
            o_Check.m_string_Name_Function = "_M_Fn_Proc_On_Result_Userword_Transcription_Audio";

            printf("----1----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_C_Mgr_Rec._M_Fn_Proc_On_Result_Userword_Transcription_Audio(o_LH_OBJECT);

            printf("----2----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_C_Mgr_Rec._M_Fn_Proc_On_Result_Userword_Transcription_Audio(o_LH_OBJECT);

            printf("----3----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_C_Mgr_Rec._M_Fn_Proc_On_Result_Userword_Transcription_Audio(o_LH_OBJECT);

            printf("----4----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_C_Mgr_Rec._M_Fn_Proc_On_Result_Userword_Transcription_Audio(o_LH_OBJECT);

            printf("----5----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_C_Mgr_Rec._M_Fn_Proc_On_Result_Userword_Transcription_Audio(o_LH_OBJECT);

            printf("----6----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_C_Mgr_Rec._M_Fn_Proc_On_Result_Userword_Transcription_Audio(o_LH_OBJECT);

            printf("----7----\n");
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
            o_C_Mgr_Rec._M_Fn_Proc_On_Result_Userword_Transcription_Audio(o_LH_OBJECT);

            printf("----8----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_C_Mgr_Rec._M_Fn_Proc_On_Result_Userword_Transcription_Audio(o_LH_OBJECT);
        }

        // ****
#if 0
        {
            N_Vr::N_Asr::N_Impl::N_Vocon::C_Result_Hypothesis o_Result_Hypothesis;
            pugi::xml_document o_xmlDoc;
            pugi::xml_node decl = o_xmlDoc.prepend_child(pugi::node_declaration);
            decl.append_attribute("version") = "1.0";
            decl.append_attribute("encoding") = "UTF-8";
            pugi::xml_node o_RootNode = o_xmlDoc.append_child("result");

            printf("_M_Fn_Proc_On_Result_NBest_Hypothesis_XML\n");
            o_Check.m_string_Name_Function = "_M_Fn_Proc_On_Result_NBest_Hypothesis_XML";

            printf("----1----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_C_Mgr_Rec._M_Fn_Proc_On_Result_NBest_Hypothesis_XML(o_Result_Hypothesis, o_RootNode);

            printf("----2----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_C_Mgr_Rec._M_Fn_Proc_On_Result_NBest_Hypothesis_XML(o_Result_Hypothesis, o_RootNode);
        }
#endif
        // ****

        {
            string string_P;

            printf("M_Fn_Get_string_Eval_Result\n");
            o_Check.m_string_Name_Function = "M_Fn_Get_string_Eval_Result";

            printf("----1----\n");
            o_Check.m_vector_i_Check.clear();
            g_function_Check = o_Check;
            o_C_Mgr_Rec._M_Fn_Get_string_Eval_Result(string_P);
        }

        // ****

        {
            string string_P;

            printf("_M_Fn_Calculate_Expression\n");
            o_Check.m_string_Name_Function = "_M_Fn_Calculate_Expression";

            printf("----1----\n");
            o_Check.m_vector_i_Check.clear();
            g_function_Check = o_Check;
            o_C_Mgr_Rec._M_Fn_Calculate_Expression(string_P);
        }

        // ****

        {
            string string_P;

            printf("_M_Fn_Calculate_Expression\n");
            o_Check.m_string_Name_Function = "_M_Fn_Calculate_Expression";

            printf("----1----\n");
            o_Check.m_vector_i_Check.clear();
            g_function_Check = o_Check;
            o_C_Mgr_Rec._M_Fn_Calculate_Expression(string_P);
        }
#if 0
        // ****
        printf("_M_Fn_Proc\n");
        o_Check.m_string_Name_Function = "_M_Fn_Proc";

        printf("----1----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_C_Mgr_Rec._M_Fn_Proc();

        printf("----2----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_C_Mgr_Rec._M_Fn_Proc();
#endif
        // ****
        printf("_M_Fn_Proc_On_Result\n");
        o_Check.m_string_Name_Function = "_M_Fn_Proc_On_Result";

        printf("----1----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_C_Mgr_Rec._M_Fn_Proc_On_Result();

        printf("----2----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_C_Mgr_Rec._M_Fn_Proc_On_Result();

        printf("----3----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_C_Mgr_Rec._M_Fn_Proc_On_Result();

        printf("----4----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_C_Mgr_Rec._M_Fn_Proc_On_Result();

        printf("----5----\n");
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
        o_C_Mgr_Rec._M_Fn_Proc_On_Result();

    }
}

void
Asr_Engine_Test_Helper::M_Fn_Vocon_Test_Vr_Asr_Impl_Vocon_Data_Party()
{
    M_Fn_Log("[GTEST] Vr_Asr_Impl_Vocon_Data_Party.\n");

    {
        N_Vr::N_Asr::N_Impl::C_Check o_Check;
        o_Check.m_b_Specify_Function = true;

        g_function_Check = Fn_Check_Real;

        // ****

        N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;

        boost::function<void(C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
        N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);

        N_Vr::N_Asr::N_Impl::N_Vocon::C_Data_Party o_Data_Party(o_Engine_Concrete, "Origin");

        // ****
        {
            printf("C_Data_Context_List\n");

            string string_Id_Context;
            string string_Type_Context;
            string string_Data_File_Context;
            list<pair<string, string> > list_pair_string_Id_Slot_string_Id_Context;
            N_Vr::N_Asr::N_Impl::N_Vocon::C_Data_Context_List o_Data_Context_List(
                o_Data_Party,
                string_Id_Context,
                string_Type_Context,
                string_Data_File_Context,
                list_pair_string_Id_Slot_string_Id_Context);

            
            o_Check.m_string_Name_Function = "M_Fn_Is_Ready";
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context_List.M_Fn_Is_Ready();

            o_Check.m_string_Name_Function = "M_Fn_Init";
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            g_function_Check = o_Check;
            o_Data_Context_List.M_Fn_Init();

            o_Check.m_string_Name_Function = "M_Fn_Load";
            list<shared_ptr<N_Vr::N_Asr::C_Request_Activate> > list_spo_Request_Activate;
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context_List.M_Fn_Load(list_spo_Request_Activate);

            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            g_function_Check = o_Check;
            o_Data_Context_List.M_Fn_Load(list_spo_Request_Activate);

            o_Data_Context_List.M_Fn_Unload();

            o_Check.m_string_Name_Function = "M_Fn_Slot_Guest_Load";
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Context_List.M_Fn_Slot_Guest_Load();
        }

        o_Check.m_string_Name_Function = "M_Fn_Init";
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Data_Party.M_Fn_Init();

        {
            printf("M_Fn_Find_po_Data_Rule\n");
            
            std::string string_Id_Rule;
            string_Id_Rule = "grm_def_navi_address_house_01#rul_def_navi_address_house_num";
            o_Data_Party.M_Fn_Find_po_Data_Rule(string_Id_Rule);
            string_Id_Rule = "grm_def_navi_address_house_03#rul_def_navi_address_house_num";
            o_Data_Party.M_Fn_Find_po_Data_Rule(string_Id_Rule);

            string_Id_Rule = "xxxxx";
            
            o_Check.m_string_Name_Function = "M_Fn_Find_po_Data_Rule";
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party.M_Fn_Find_po_Data_Rule(string_Id_Rule);
            
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party.M_Fn_Find_po_Data_Rule(string_Id_Rule);
        }

        {
            printf("_M_Fn_Info_Get_Path_Data_Party_Folder\n");
            o_Check.m_string_Name_Function = "_M_Fn_Info_Get_Path_Data_Party_Folder";

            printf("----1----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Get_Path_Data_Party_Folder("test_path_a", "test_path_b");
            
            printf("----2----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Get_Path_Data_Party_Folder("test_path_a", "test_path_b");

            printf("----3----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Get_Path_Data_Party_Folder("test_path_a", "test_path_b");
        }

        {
            printf("_M_Fn_Info_Parse_Context_In_Dialog\n");
            o_Check.m_string_Name_Function = "_M_Fn_Info_Parse_Context_In_Dialog";
            std::ifstream o_ifstream;
            N_Vr::N_Asr::N_Impl::N_Vocon::C_Data_Context_In_Dialog o_Data_Context_In_Dialog;

            printf("----1----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Context_In_Dialog(o_ifstream, o_Data_Context_In_Dialog);
            
            printf("----2----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Context_In_Dialog(o_ifstream, o_Data_Context_In_Dialog);

            printf("----3----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Context_In_Dialog(o_ifstream, o_Data_Context_In_Dialog);

            printf("----4----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Context_In_Dialog(o_ifstream, o_Data_Context_In_Dialog);

            printf("----5----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Context_In_Dialog(o_ifstream, o_Data_Context_In_Dialog);

        }

        {
            printf("_M_Fn_Info_Parse_File\n");
            o_Check.m_string_Name_Function = "_M_Fn_Info_Parse_File";

            printf("----1----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_File();

            printf("----2----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_File();
        }

        {
            printf("_M_Fn_Info_Parse_Context\n");
            o_Check.m_string_Name_Function = "_M_Fn_Info_Parse_Context";
            std::ifstream o_ifstream;

            printf("----1----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Context(o_ifstream);
            
            printf("----2----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Context(o_ifstream);

            printf("----3----\n");
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
            o_Data_Party._M_Fn_Info_Parse_Context(o_ifstream);

            printf("----4----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Context(o_ifstream);

            printf("----5----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Context(o_ifstream);

            printf("----6----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Context(o_ifstream);

            printf("----7----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Context(o_ifstream);

        }

        {
            printf("_M_Fn_Info_Parse_Slot_In_Context\n");
            o_Check.m_string_Name_Function = "_M_Fn_Info_Parse_Slot_In_Context";
            std::ifstream o_ifstream;
            std::pair<std::string, std::string> pair_Id_Context;

            printf("----1----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Slot_In_Context(o_ifstream, pair_Id_Context);
            
            printf("----2----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Slot_In_Context(o_ifstream, pair_Id_Context);

            printf("----3----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Slot_In_Context(o_ifstream, pair_Id_Context);
        }
        
        {
            printf("_M_Fn_Info_Parse_Rule\n");
            o_Check.m_string_Name_Function = "_M_Fn_Info_Parse_Rule";
            std::ifstream o_ifstream;

            printf("----1----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Rule(o_ifstream);
            
            printf("----2----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Rule(o_ifstream);

            printf("----3----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Rule(o_ifstream);

            printf("----4----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Rule(o_ifstream);

            printf("----5----\n");
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
            o_Data_Party._M_Fn_Info_Parse_Rule(o_ifstream);

            printf("----5----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Rule(o_ifstream);
        }

        {
            printf("_M_Fn_Info_Parse_Tag\n");
            o_Check.m_string_Name_Function = "_M_Fn_Info_Parse_Tag";
            std::ifstream o_ifstream;

            printf("----1----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Tag(o_ifstream);
            
            printf("----2----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Tag(o_ifstream);

            printf("----3----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Tag(o_ifstream);

            printf("----4----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Tag(o_ifstream);

            printf("----5----\n");
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
            o_Data_Party._M_Fn_Info_Parse_Tag(o_ifstream);

            printf("----5----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Tag(o_ifstream);
        }

        {
            printf("_M_Fn_Info_Parse_Node_Result\n");
            o_Check.m_string_Name_Function = "_M_Fn_Info_Parse_Node_Result";
            std::ifstream o_ifstream;
            N_Vr::N_Asr::N_Impl::N_Vocon::C_Data_Node_Result o_Data_Node_Result;

            printf("----1----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Node_Result(o_ifstream, o_Data_Node_Result);
            
            printf("----2----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Node_Result(o_ifstream, o_Data_Node_Result);

            printf("----3----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Node_Result(o_ifstream, o_Data_Node_Result);

            printf("----4----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Node_Result(o_ifstream, o_Data_Node_Result);

            printf("----5----\n");
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
            o_Data_Party._M_Fn_Info_Parse_Node_Result(o_ifstream, o_Data_Node_Result);

            printf("----6----\n");
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
            o_Data_Party._M_Fn_Info_Parse_Node_Result(o_ifstream, o_Data_Node_Result);
        }

        {
            printf("_M_Fn_Info_Parse_Line\n");
            o_Check.m_string_Name_Function = "_M_Fn_Info_Parse_Line";
            std::ifstream o_ifstream;
            std::string string_Key;
            std::string string_Value;
            bool b_Eof;

            printf("----1----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Line(o_ifstream, string_Key, string_Value, b_Eof);
            
            printf("----2----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Parse_Line(o_ifstream, string_Key, string_Value, b_Eof);
        }

        {
            printf("_M_Fn_Info_Get_Version\n");
            o_Check.m_string_Name_Function = "_M_Fn_Info_Get_Version";
            std::string string_file_path = "/data/asr/test.txt";
            long i_Static_Version = 0;
            long i_Navi_Version = 0;

            printf("----1----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Get_Version(string_file_path, i_Static_Version, i_Navi_Version);
            
            printf("----2----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Get_Version(string_file_path, i_Static_Version, i_Navi_Version);

            printf("----2----\n");
            o_Check.m_vector_i_Check.clear();
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(true);
            o_Check.m_vector_i_Check.push_back(false);
            o_Check.m_vector_i_Check.push_back(false);
            g_function_Check = o_Check;
            o_Data_Party._M_Fn_Info_Get_Version(string_file_path, i_Static_Version, i_Navi_Version);
        }

    }
}

void
Asr_Engine_Test_Helper::M_Fn_Vocon_Test_Fn_Get_ro_LH_AUDIOIN_INTERFACE()
{
    M_Fn_Log("[GTEST] Vr_Asr_Fn_Get_ro_LH_AUDIOIN_INTERFACE\n");

    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;
    g_function_Check = Fn_Check_Real;

    // ****
    o_Check.m_string_Name_Function = "S_Fn_Audio_In_Start";
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    N_Vr::N_Asr::N_Impl::N_Vocon::S_Fn_Audio_In_Start(NULL);

    o_Check.m_string_Name_Function = "S_Fn_Audio_In_Stop";
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    N_Vr::N_Asr::N_Impl::N_Vocon::S_Fn_Audio_In_Stop(NULL);

    o_Check.m_string_Name_Function = "S_Fn_Audio_In_Has_Data";
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    N_Vr::N_Asr::N_Impl::N_Vocon::S_Fn_Audio_In_Has_Data(NULL, NULL);

    o_Check.m_string_Name_Function = "S_Fn_Audio_In_Fetch_Data";
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    N_Vr::N_Asr::N_Impl::N_Vocon::S_Fn_Audio_In_Fetch_Data(NULL, NULL, NULL);

    o_Check.m_string_Name_Function = "S_Fn_Audio_In_Return_Data";
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    N_Vr::N_Asr::N_Impl::N_Vocon::S_Fn_Audio_In_Return_Data(NULL, NULL);

}

void
Asr_Engine_Test_Helper::M_Fn_Vocon_Test_Fn_Get_ro_LH_HEAP_INTERFACE()
{
    M_Fn_Log("[GTEST] Vr_Asr_Fn_Get_ro_LH_HEAP_INTERFACE\n");

    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;
    g_function_Check = Fn_Check_Real;

    // ****
    o_Check.m_string_Name_Function = "S_Fn_Heap_Malloc";
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    N_Vr::N_Asr::N_Impl::N_Vocon::S_Fn_Heap_Malloc(NULL, 0);

    o_Check.m_string_Name_Function = "S_Fn_Heap_Calloc";
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    N_Vr::N_Asr::N_Impl::N_Vocon::S_Fn_Heap_Calloc(NULL, 0, 0);

    o_Check.m_string_Name_Function = "S_Fn_Heap_Realloc";
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    N_Vr::N_Asr::N_Impl::N_Vocon::S_Fn_Heap_Realloc(NULL, NULL, 0);

    o_Check.m_string_Name_Function = "S_Fn_Heap_Free";
    o_Check.m_vector_i_Check.clear();
    o_Check.m_vector_i_Check.push_back(true);
    g_function_Check = o_Check;
    N_Vr::N_Asr::N_Impl::N_Vocon::S_Fn_Heap_Free(NULL, NULL);
}

void
Asr_Engine_Test_Helper::M_Fn_Vocon_Test_Vr_Asr_Impl_Vocon_Data_CLC()
{
    M_Fn_Log("[GTEST] Vr_Asr_Impl_Vocon_Data_CLC.\n");

    N_Vr::N_Asr::N_Impl::C_Check o_Check;
    o_Check.m_b_Specify_Function = true;
    g_function_Check = Fn_Check_Real;

    N_Vr::N_Asr::N_Impl::C_Engine_Agent o_Engine_Agent;
    boost::function<void(N_Vr::N_Asr::C_Event_Init_Notify const &)> function_On_Event_Init_Notify;
    N_Vr::N_Asr::N_Impl::N_Vocon::C_Engine_Concrete o_Engine_Concrete(o_Engine_Agent, "eng", function_On_Event_Init_Notify);

    N_Vr::N_Asr::N_Impl::N_Vocon::C_Data_CLC o_Data_CLC(o_Engine_Concrete, "", "");

    {
        printf("M_Fn_Init\n");
        o_Check.m_string_Name_Function = "M_Fn_Init";

        printf("----1----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Data_CLC.M_Fn_Init();

        printf("----2----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Data_CLC.M_Fn_Init();

        printf("----3----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Data_CLC.M_Fn_Init();

        printf("----4----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Data_CLC.M_Fn_Init();

        printf("----5----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(false);
        g_function_Check = o_Check;
        o_Data_CLC.M_Fn_Init();

        printf("----6----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(false);
        g_function_Check = o_Check;
        o_Data_CLC.M_Fn_Init();

        printf("----7----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(false);
        g_function_Check = o_Check;
        o_Data_CLC.M_Fn_Init();

        printf("----8----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(false);
        g_function_Check = o_Check;
        o_Data_CLC.M_Fn_Init();
    }

    {
        printf("M_Fn_Fina\n");
        o_Check.m_string_Name_Function = "M_Fn_Fina";

        printf("----1----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Data_CLC.M_Fn_Fina();

        printf("----2----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Data_CLC.M_Fn_Fina();

        printf("----3----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Data_CLC.M_Fn_Fina();

        printf("----4----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Data_CLC.M_Fn_Fina();
    }

    {
        printf("M_Fn_Init_Id_Trans_Lookup\n");
        o_Check.m_string_Name_Function = "M_Fn_Init_Id_Trans_Lookup";

        printf("----1----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Data_CLC.M_Fn_Init_Id_Trans_Lookup();

        printf("----2----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Data_CLC.M_Fn_Init_Id_Trans_Lookup();

        printf("----3----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Data_CLC.M_Fn_Init_Id_Trans_Lookup();

        printf("----4----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Data_CLC.M_Fn_Init_Id_Trans_Lookup();

        printf("----5----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Data_CLC.M_Fn_Init_Id_Trans_Lookup();

#if 0
        printf("----6----\n");
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
        o_Data_CLC.M_Fn_Init_Id_Trans_Lookup();
#endif
    }

    {
        printf("M_Fn_Fina_Id_Trans_Lookup\n");
        o_Check.m_string_Name_Function = "M_Fn_Fina_Id_Trans_Lookup";

        printf("----1----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Data_CLC.M_Fn_Fina_Id_Trans_Lookup();

        printf("----2----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Data_CLC.M_Fn_Fina_Id_Trans_Lookup();

        printf("----3----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Data_CLC.M_Fn_Fina_Id_Trans_Lookup();

        printf("----4----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(false);
        g_function_Check = o_Check;
        o_Data_CLC.M_Fn_Fina_Id_Trans_Lookup();
    }

    {
        string string_Domain_Category("test");
        
        printf("M_Fn_Domain_Category_Set\n");
        o_Check.m_string_Name_Function = "M_Fn_Domain_Category_Set";

        printf("----1----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Data_CLC.M_Fn_Domain_Category_Set(string_Domain_Category);

        printf("----2----\n");
        o_Check.m_vector_i_Check.clear();
        o_Check.m_vector_i_Check.push_back(false);
        o_Check.m_vector_i_Check.push_back(true);
        g_function_Check = o_Check;
        o_Data_CLC.M_Fn_Domain_Category_Set(string_Domain_Category);
    }


}

void
Asr_Engine_Test_Helper::M_Fn_Vocon_Test_Vr_Asr_Impl_Vocon_Fn_Callback_LH_Error()
{
    M_Fn_Log("[GTEST] Vr_Asr_Impl_Vocon_Fn_Callback_LH_Error.\n");

    // ****

    {
        LH_ERROR o_LH_ERROR;
        N_Vr::N_Asr::N_Impl::N_Vocon::Fn_Callback_LH_Error(NULL, o_LH_ERROR, NULL, NULL);

        char* a = "log";
        char* b = "test";
        N_Vr::N_Asr::N_Impl::N_Vocon::Fn_Callback_LH_Error(NULL, o_LH_ERROR, a, b);
    }

}

#endif

/* EOF */
