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

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "Vr_Asr_Impl_Afx.h"
#include "Vr_Asr_Impl_Fn_string.h"
#include "Vr_Asr_Impl_Fn_Get_string_Path.h"
#include "Vr_Asr_Impl_Fn_Get_string_Line_Fixed.h"
#include "Vr_Asr_Impl_IFlyTek_Engine_Concrete.h"
#include "Vr_Asr_Impl_IFlyTek_Mgr_Data.h"
#include "Vr_Asr_Impl_IFlyTek_Fn_Callback_Grammar_Build.h"
#include "Vr_Asr_Impl_IFlyTek_Fn_Callback_Grammar_Update.h"
#include "Vr_Asr_Impl_Engine_Agent.h"
#include "Vr_Asr_Impl_Event_Phase_Impl.h"
#include "Vr_Asr_Impl_Event_Result_Impl.h"
#include "Vr_Asr_Impl_Event_Notify_Impl.h"
#include "Vr_Asr_Impl_Event_Notify_Impl_Signal_Level.h"
#include "Vr_Asr_Impl_Event_Init_Notify_Impl.h"
#include "Vr_Asr_Impl_Fn_Dir_File.h"


namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_IFlyTek
{

#define AUDIO_SAMPLE_RATE 16000

C_Engine_Concrete::~C_Engine_Concrete()
{
    MACRO_Log_Function
}

C_Engine_Concrete::C_Engine_Concrete
(
    C_Engine_Agent & ro_Engine_Agent,
    string const & rstring_Id_Language,
    boost::function<void(C_Event_Init_Notify const &)>& ro_function_On_Event_Init_Notify,
    string const & ro_string_Center_Vr_Url
)
: m_ro_Engine_Agent(ro_Engine_Agent)
, m_scpo_Request_Recognize()
, m_list_spo_Request_Context_List_Update()
, m_list_o_Time_Context_list_Update()
, p_session_id(NULL)
, m_scpo_Mgr_Data(MACRO_New C_Mgr_Data(*this))
, m_function_On_Event_Init_Notify(ro_function_On_Event_Init_Notify)
, m_string_Center_Vr_Url(ro_string_Center_Vr_Url)
, m_spstring_Result_Json()
, m_b_Init(false)
, m_b_Error(false)
, m_b_Rec_Running(false)
, m_i_Audio_Data_Cnt(0)
, m_cond_grammar_build()
, m_cond_grammar_update()
, m_b_Log_File(false)
, m_o_Log_File()
, m_string_Id_Language(rstring_Id_Language)
, m_string_IFlyTek_Key("appid = 590c49c4")
, m_string_IFlyTek_Res()
, m_string_curr_dialog_id()
, m_map_dialog_id_and_grammar_id()
, m_map_dialog_id_and_grammar_bnf()
{
    MACRO_Log_Function
}


E_Result
C_Engine_Concrete::M_Fn_Init()
{
    MACRO_Log_Function

    E_Result e_Result;

    IF (m_b_Init) {
        MACRO_Log_Error
        return E_Result_Failed;
    }
    m_b_Init = true;

    IF (m_b_Error) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    e_Result = _M_Fn_IFlyTek_Data_Init();
    IF (E_Result_Succeeded != e_Result) {
        m_b_Error = true;
        MACRO_Log_Error
        return E_Result_Error;
    }


    e_Result = _M_Fn_IFlyTek_Init();
    IF (E_Result_Succeeded != e_Result) {
        m_b_Error = true;
        MACRO_Log_Error
        return E_Result_Error;
    }

    e_Result = _M_Fn_Mgr_Init();
    IF (E_Result_Succeeded != e_Result) {
        m_b_Error = true;
        MACRO_Log_Error
        return E_Result_Error;
    }

    e_Result = _M_Fn_Build_Grammar();
    IF (E_Result_Succeeded != e_Result) {
        m_b_Error = true;
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_b_Error = false;

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::M_Fn_Fina()
{
    MACRO_Log_Function

    E_Result e_Result;

    m_b_Init = false;

    m_b_Error = true;

    e_Result = _M_Fn_Mgr_Fina();
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
    }

    e_Result = _M_Fn_IFlyTek_Fina();
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
    }

    e_Result = _M_Fn_IFlyTek_Data_Fina();
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
    }

    m_b_Error = false;

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::M_Fn_Data_Context_List_Update
(
    C_Request_Context_List_Update & ro_Request_Context_List_Update
)
{
    MACRO_Log_Function

    E_Result e_Result;

    IF (!m_b_Init) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    IF (m_b_Error) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF ((!ro_Request_Context_List_Update.m_function_On_Event_Phase)
        || (!ro_Request_Context_List_Update.m_function_On_Event_Notify)
    ) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    shared_ptr<C_Request_Context_List_Update> spo_Request_Context_List_Update;
    spo_Request_Context_List_Update.reset(
        MACRO_New C_Request_Context_List_Update(
            ro_Request_Context_List_Update
        )
    );
    IF (!spo_Request_Context_List_Update) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    spo_Request_Context_List_Update->m_function_On_Event_Phase(
            C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Proc_Begin)
    );

    IF (0 == m_map_dialog_id_and_grammar_id.count(spo_Request_Context_List_Update->m_string_Id_Category)) {
        Fn_Log("no dialog id: " + spo_Request_Context_List_Update->m_string_Id_Category);
        MACRO_Log_Error
        return E_Result_Error;
    }

    string string_context_content;
    _M_Fn_Get_Context_Content(spo_Request_Context_List_Update->m_list_spo_Term_For_Add, string_context_content);

    string string_grammar_id = m_map_dialog_id_and_grammar_id[spo_Request_Context_List_Update->m_string_Id_Category];

    stringstream o_stringstream_content;
    o_stringstream_content << "engine_type = local"
                           << ", text_encoding = UTF-8"
                           << ", asr_res_path = " << "fo|" + G_o_Get_Path().M_Fn_Get_string_Path_VData() + m_string_IFlyTek_Res
                           << ", sample_rate = " << AUDIO_SAMPLE_RATE
                           << ", grm_build_path = " << G_o_Get_Path().M_Fn_Get_string_Path_Usr()
                           << ", grammar_list = " << string_grammar_id;

    Fn_Log(o_stringstream_content.str());

    int i_ret = G_o_IFlyTek().M_Fn_QISRUpdateLexicon(spo_Request_Context_List_Update->m_string_Id_Category.c_str(),
                string_context_content.c_str(),
                string_context_content.size(),
                o_stringstream_content.str().c_str(),
                Fn_Callback_Grammar_Update,
                this);
    IF (0 != i_ret) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_list_spo_Request_Context_List_Update.push_back(spo_Request_Context_List_Update);

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::M_Fn_Rec_Start
(
    C_Request_Recognize & ro_Request_Recognize
)
{
    MACRO_Log_Function

    E_Result e_Result;

    IF (!m_b_Init) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    IF (m_b_Error) {
        MACRO_Log_Error

        e_Result = M_Fn_Fina();
        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }

        e_Result = M_Fn_Init();
        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }

        IF (m_b_Error) {
            MACRO_Log_Error
            return E_Result_Error;
        }
    }

    IF (m_b_Rec_Running) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    IF ((!ro_Request_Recognize.m_spo_Audio_In)
        || (!ro_Request_Recognize.m_function_On_Event_Phase)
        || (!ro_Request_Recognize.m_function_On_Event_Notify)
        || (!ro_Request_Recognize.m_function_On_Event_Result)
    ) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_scpo_Request_Recognize.reset(
        MACRO_New C_Request_Recognize(
            ro_Request_Recognize
        )
    );
    IF (!m_scpo_Request_Recognize) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    string string_dialog = m_scpo_Request_Recognize->m_string_Id_Dialog;
    Fn_Log("dialog id: " + string_dialog);
    IF (0 == m_map_dialog_id_and_grammar_id.count(string_dialog)) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    stringstream o_stringstream_param;
    o_stringstream_param << "engine_type = local"
                         << ", asr_res_path = " << "fo|" + G_o_Get_Path().M_Fn_Get_string_Path_VData() + m_string_IFlyTek_Res
                         << ", sample_rate = " << AUDIO_SAMPLE_RATE
                         << ", grm_build_path = " << G_o_Get_Path().M_Fn_Get_string_Path_Usr()
                         << ", local_grammar = " << m_map_dialog_id_and_grammar_id[string_dialog]
                         << ", result_type = xml"
                         << ", result_encoding = UTF-8,";

    m_scpo_Request_Recognize->m_function_On_Event_Phase(
        C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Proc_Begin)
    );

    int i_err_code = 0;
    p_session_id = G_o_IFlyTek().M_Fn_QISRSessionBegin(NULL, o_stringstream_param.str().c_str(), &i_err_code);
    IF (NULL == p_session_id) {
        Fn_Log_IFlyTek_ERROR(i_err_code);
        MACRO_Log_Error
        return E_Result_Error;
    }

    e_Result = _M_Fn_Start_Audio();
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_b_Rec_Running = true;

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::M_Fn_Rec_Cancel()
{
    MACRO_Log_Function


    E_Result e_Result;

    IF (!m_b_Init) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    IF (m_b_Error) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (!m_b_Rec_Running) {
        return E_Result_Succeeded;
    }

    e_Result = _M_Fn_Stop_Audio();
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    int i_ret = 0;
    i_ret = G_o_IFlyTek().M_Fn_QISRSessionEnd(p_session_id, NULL);
    IF (0 == i_ret) {
        _M_Fn_On_Rec_End();
    }
    ELSE {
        stringstream stringstream_Log;
        stringstream_Log << "QISRSessionEnd failed: " << i_ret;
        Fn_Log(stringstream_Log.str());
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::M_Fn_Append_Audio_Data
(
    short* pi_Audio_Buffer,
    unsigned int i_Audio_Frames,
    unsigned int& i_Write_Audio_Frames,
    bool& b_Speech_End
)
{
    int i_ret = 0;
    int i_aud_state = MSP_AUDIO_SAMPLE_CONTINUE;
    int i_ep_status = MSP_EP_LOOKING_FOR_SPEECH;
    int i_rec_status = MSP_REC_STATUS_INCOMPLETE;

    b_Speech_End = false;

    IF (0 == m_i_Audio_Data_Cnt) {
        m_i_Audio_Data_Cnt++;
        i_aud_state = MSP_AUDIO_SAMPLE_FIRST;
    }

    i_ret = G_o_IFlyTek().M_Fn_QISRAudioWrite(
                p_session_id,
                (const void *)pi_Audio_Buffer,
                i_Audio_Frames * sizeof(short),
                i_aud_state,
                &i_ep_status,
                &i_rec_status);
    IF (0 != i_ret) {
        stringstream stringstream_Log;
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "Append audio data to iflytek : " << i_ret;
        Fn_Log(stringstream_Log.str());
        return E_Result_Error;
    }

    i_Write_Audio_Frames = i_Audio_Frames;

    IF (MSP_EP_AFTER_SPEECH != i_ep_status) {
        return E_Result_Succeeded;
    }

    b_Speech_End = true;

    i_ret = G_o_IFlyTek().M_Fn_QISRAudioWrite(
                p_session_id,
                (const void *)NULL,
                0,
                MSP_AUDIO_SAMPLE_LAST,
                &i_ep_status,
                &i_rec_status);
    IF (0 != i_ret) {
        stringstream stringstream_Log;
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "Append audio data to iflytek : " << i_ret;
        Fn_Log(stringstream_Log.str());
        return E_Result_Error;
    }


    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::M_Fn_Party_Load
(
    string const & rstring_Id_Party
)
{
    MACRO_Log_Function

    MACRO_Log_Error

    return E_Result_Succeeded;
}


E_Result
C_Engine_Concrete::M_Fn_Party_Unload
(
    string const & rstring_Id_Party
)
{
    MACRO_Log_Function

    MACRO_Log_Error

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::M_Fn_Param_Set
(
    C_Request_Param_Set & ro_Request_Param_Set
)
{
    MACRO_Log_Function

    IF (!m_b_Init) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    IF (m_b_Error) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::M_Fn_On_Callback_Grammar_Build
(
    int i_Ecode,
    const char* p_Info
)
{
    MACRO_Log_Function

    IF ((MSP_SUCCESS == i_Ecode)
        || (NULL != p_Info)) {
        Fn_Log("grammar ok: " + m_string_curr_dialog_id);
        IF (0 != m_map_dialog_id_and_grammar_id.count(m_string_curr_dialog_id)) {
            Fn_Log("grammar has been built: " + m_string_curr_dialog_id);
        }

        m_map_dialog_id_and_grammar_id[m_string_curr_dialog_id] = string(p_Info);
    }
    ELSE {
        Fn_Log_IFlyTek_ERROR(i_Ecode);
        MACRO_Log_Error
        return E_Result_Error;
    } 
    m_cond_grammar_build.Notify();

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::M_Fn_On_Callback_Grammar_Update
(
    int i_Ecode,
    const char* p_Info
)
{
    MACRO_Log_Function

    IF (m_list_spo_Request_Context_List_Update.empty()) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    shared_ptr<C_Request_Context_List_Update> spo_Request_Context_List_Update;
    spo_Request_Context_List_Update.swap(m_list_spo_Request_Context_List_Update.front());
    m_list_spo_Request_Context_List_Update.pop_front();

    IF (MSP_SUCCESS != i_Ecode) {
        spo_Request_Context_List_Update->m_function_On_Event_Notify(
            C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Local_Dictionary_Upload_Error)
        );
    }

    spo_Request_Context_List_Update->m_function_On_Event_Phase(
        C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Proc_End)
    );

    return E_Result_Succeeded;
}

void
C_Engine_Concrete::M_Fn_Proc_On_Result
(
    bool b_User_Abort
)
{
    MACRO_Log_Function

    const char* p_ret_result = NULL;

    IF (!b_User_Abort) {
        int i_err_code = 0;
        int i_rss_status = MSP_REC_STATUS_INCOMPLETE;
        while (MSP_REC_STATUS_COMPLETE != i_rss_status && MSP_SUCCESS == i_err_code) {
            p_ret_result = QISRGetResult(p_session_id, &i_rss_status, 0, &i_err_code);
            usleep(150 * 1000);
        }
    }

    string string_Result;

    IF (NULL != p_ret_result) {
        string_Result = string(p_ret_result);
    }

    Fn_Log(string_Result);
    m_o_Log_File.M_Fn_Log(string_Result);
    m_o_Log_File.M_Fn_Enter();

    C_Event_Result_Impl o_Event_Result_Impl;

    m_spstring_Result_Json.reset(MACRO_New string);
    *m_spstring_Result_Json = string_Result;
    o_Event_Result_Impl.m_spstring_Result_Json = m_spstring_Result_Json;

    m_scpo_Request_Recognize->m_function_On_Event_Result(
        o_Event_Result_Impl
    );


    m_b_Rec_Running = false;

    m_scpo_Request_Recognize->m_function_On_Event_Phase(
        C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Proc_End)
    );

    return;
}

E_Result
C_Engine_Concrete::_M_Fn_Start_Audio()
{
    MACRO_Log_Function

    E_Result e_Result;

    IF (!m_scpo_Mgr_Data) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    e_Result = m_scpo_Mgr_Data->M_Fn_Start_Audio();
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_i_Audio_Data_Cnt = 0;

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Stop_Audio()
{
    MACRO_Log_Function

    E_Result e_Result;

    IF (!m_scpo_Mgr_Data) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    e_Result = m_scpo_Mgr_Data->M_Fn_Stop_Audio();
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}


E_Result
C_Engine_Concrete::_M_Fn_Mgr_Init()
{
    MACRO_Log_Function

    E_Result e_Result;

    e_Result = m_scpo_Mgr_Data->M_Fn_Init();
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}


E_Result
C_Engine_Concrete::_M_Fn_Mgr_Fina()
{
    MACRO_Log_Function

    E_Result e_Result;

    _M_Fn_Stop_Audio();

    e_Result = m_scpo_Mgr_Data->M_Fn_Fina();
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_IFlyTek_Init()
{
    MACRO_Log_Function

    int i_ret = 0;
    Fn_Log("IFlyTek_Key: " + m_string_IFlyTek_Key);
    const char* p_login_config = m_string_IFlyTek_Key.c_str();

    // 第一个参数为用户名，第二个参数为密码，传NULL即可，第三个参数是登录参数
    i_ret  = G_o_IFlyTek().M_Fn_MSPLogin(NULL, NULL, p_login_config);
    IF (MSP_SUCCESS != i_ret) {
        stringstream o_stringstream;
        o_stringstream << "iflytek msplogin failed: " << i_ret;
        Fn_Log(o_stringstream.str());
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_IFlyTek_Fina()
{
    MACRO_Log_Function

    G_o_IFlyTek().M_Fn_MSPLogout();

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_IFlyTek_Data_Init()
{
    E_Result e_Result;

    // dialog grammar bnf
    e_Result = _M_Fn_Info_Parse_File();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    // log enable
    m_o_Log_File.M_Fn_Enable_Log(m_b_Log_File);
    m_o_Log_File.M_Fn_Init(G_o_Get_Path().M_Fn_Get_string_Path_Usr() + "Log.txt");



    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_IFlyTek_Data_Fina()
{
    m_o_Log_File.M_Fn_Fina();
    m_b_Log_File = false;

    return E_Result_Succeeded;
}

void
C_Engine_Concrete::_M_Fn_On_Rec_End()
{
    MACRO_Log_Function

    m_b_Rec_Running = false;

    m_scpo_Request_Recognize->m_function_On_Event_Phase(
        C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Proc_End)
    );

    return;
}

E_Result
C_Engine_Concrete::_M_Fn_Info_Parse_File()
{
    MACRO_Log_Function

    E_Result e_Result;

    std::ifstream ifstream_File;
    string string_Path_File_Info = G_o_Get_Path().M_Fn_Get_string_Path_VData() + "Info.dat";
    ifstream_File.open(string_Path_File_Info.c_str());
    IF (!ifstream_File.is_open()) {
        Fn_Log("    Failed to open file: "+ string_Path_File_Info);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // BOM

    char i_BOM;

    ifstream_File >> i_BOM >> i_BOM >> i_BOM;

    string string_Line;

    Fn_Get_string_Line_Fixed(ifstream_File, string_Line);

    // Parse lines.

    string string_Key;
    string string_Value;
    bool b_Eof;

    while (true) {
        e_Result = _M_Fn_Info_Parse_Line(
            ifstream_File,
            string_Key,
            string_Value,
            b_Eof
        );

        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }

        IF (b_Eof) {
            break;
        }

        IF (string_Key == "DIALOG") {
            e_Result = _M_Fn_Info_Parse_Dialog(ifstream_File);
        }
        ELSE
        IF (string_Key == "LOG") {
            IF (string_Value == "enable") {
                Fn_Log("    Enable Log File");
                m_b_Log_File = true;
            }

            continue;
        }
        ELSE
        IF (string_Key == "IFlyTek_Key") {
            IF (string_Value.empty()) {
                MACRO_Log_Error
                return E_Result_Error;
            }
            Fn_Log("IFlyTek_Key: " + string_Value);
            m_string_IFlyTek_Key = string_Value;

            continue;
        }
        ELSE
        IF (string_Key == "IFlyTek_Res") {
            IF (string_Value.empty()) {
                MACRO_Log_Error
                return E_Result_Error;
            }
            Fn_Log("IFlyTek_Res: " + string_Value);
            m_string_IFlyTek_Res = string_Value;

            continue;
        }
        ELSE
        IF (string_Key.empty()) {
            continue;
        }
        ELSE {
            MACRO_Log_Error
            return E_Result_Error;
        }

        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Info_Parse_Line
(
    ifstream & rifstream_File,
    string & rstring_Key,
    string & rstring_Value,
    bool & rb_Eof
)
{
    // Init return value.

    rstring_Key.clear();
    rstring_Value.clear();
    rb_Eof = false;

    // Get line.

    string string_Line;

    Fn_Get_string_Line_Fixed(rifstream_File, string_Line);

    IF (!rifstream_File) {
        // Check stream state: badbit

        IF (rifstream_File.rdstate() & std::ios::badbit) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        // Check StreamFile: eofbit

        IF (rifstream_File.rdstate() & std::ios::eofbit) {
            rb_Eof = true;
            return E_Result_Succeeded;
        }

        // Check StreamFile: failbit

        IF (rifstream_File.rdstate() & std::ios::failbit) {
            MACRO_Log_Error
            return E_Result_Error;
        }
    }

    // Remove trailing comment.

    string::size_type i_Pos_Char_Dollar = string_Line.find('$');
    IF (i_Pos_Char_Dollar != string::npos) {
        string_Line = string_Line.substr(0, i_Pos_Char_Dollar);
    }

    // Get string.

    stringstream stringstream_Line;
    stringstream_Line.clear();
    stringstream_Line.str(string_Line);

    // Get key.

    IF (!stringstream_Line) {
        return E_Result_Succeeded;
    }
    stringstream_Line >> rstring_Key;

    // Get value.

    IF (!stringstream_Line) {
        return E_Result_Succeeded;
    }
    std::getline(stringstream_Line, rstring_Value);
    rstring_Value = Fn_Get_string_Without_Blank(rstring_Value);

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Info_Parse_Dialog
(
    ifstream & rifstream_File
)
{
    E_Result e_Result;

    string string_Id_Dialog;
    size_t i_find_pos = string::npos;
    int i_Id_Context = 0;
    string string_Id_Context;

    // Parse lines.

    string string_Key;
    string string_Value;
    bool b_Eof;

    while (true) {
        e_Result = _M_Fn_Info_Parse_Line(
            rifstream_File,
            string_Key,
            string_Value,
            b_Eof
        );

        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }

        IF (b_Eof) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        IF (string_Key == "DIALOG_END") {
            break;
        }

        IF (string_Key.empty() || string_Value.empty()) {
            MACRO_Log_Error
            return E_Result_Error;
        }
        Fn_Log("    Dialog: " + string_Key + "    bnf: " + string_Value);

        IF (0 != m_map_dialog_id_and_grammar_bnf.count(string_Key)) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        m_map_dialog_id_and_grammar_bnf[string_Key] = string_Value;
    }


    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Build_Grammar()
{
    MACRO_Log_Function

    E_Result e_Result;

    stringstream o_stringstream_param;
    o_stringstream_param << "engine_type = local"
                         << ", asr_res_path = " << "fo|" + G_o_Get_Path().M_Fn_Get_string_Path_VData() + m_string_IFlyTek_Res
                         << ", sample_rate = " << AUDIO_SAMPLE_RATE
                         << ", grm_build_path = " << G_o_Get_Path().M_Fn_Get_string_Path_Usr();
    Fn_Log(o_stringstream_param.str());

    typedef map<string, string>::iterator Map_Iter;
    Map_Iter bnf_iter_end = m_map_dialog_id_and_grammar_bnf.end();
    for (
        Map_Iter bnf_iter = m_map_dialog_id_and_grammar_bnf.begin();
        bnf_iter != bnf_iter_end;
        ++bnf_iter
    ) {
        ifstream o_ifstream_bnf;
        scoped_array<char> o_sca_bnf_content;
        unsigned int i_grm_cnt_len = 0;

        m_string_curr_dialog_id = bnf_iter->first;
        string string_grammar_bnf = G_o_Get_Path().M_Fn_Get_string_Path_VData() + bnf_iter->second;
        o_ifstream_bnf.open(string_grammar_bnf, ifstream::binary);
        IF (!o_ifstream_bnf.is_open()) {
            Fn_Log("failed to open file: " + m_string_curr_dialog_id + " - " + string_grammar_bnf);
            MACRO_Log_Error
            return E_Result_Error;
        }

        o_ifstream_bnf.seekg(0, o_ifstream_bnf.end);
        i_grm_cnt_len = o_ifstream_bnf.tellg();
        o_ifstream_bnf.seekg(0, o_ifstream_bnf.beg);

        o_sca_bnf_content.reset(MACRO_New char[i_grm_cnt_len + 1]);
        IF (!o_sca_bnf_content) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        o_ifstream_bnf.read(o_sca_bnf_content.get(), i_grm_cnt_len);
        (o_sca_bnf_content.get())[i_grm_cnt_len] = 0;
        o_ifstream_bnf.close();

        int i_ret = G_o_IFlyTek().M_Fn_QISRBuildGrammar("bnf",
                                    o_sca_bnf_content.get(),
                                    i_grm_cnt_len,
                                    o_stringstream_param.str().c_str(),
                                    Fn_Callback_Grammar_Build,
                                    this);
        IF (0 != i_ret) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        m_cond_grammar_build.Wait();
    }

    return E_Result_Succeeded;
}

void
C_Engine_Concrete::_M_Fn_Get_Context_Content
(
    list<shared_ptr<C_Term> > & ro_list_spo_Term,
    string& ro_string_context_content
)
{
    typedef vector<std::string>::iterator Vec_Iter;

    stringstream o_stringstream_context;
    shared_ptr<C_Term> spo_Term;
    while (!ro_list_spo_Term.empty()) {

        spo_Term.swap(ro_list_spo_Term.front());
        ro_list_spo_Term.pop_front();

        for (
            Vec_Iter iter = spo_Term->m_vector_string_Orthography.begin();
            iter != spo_Term->m_vector_string_Orthography.end();
            ++iter
        ) {
            o_stringstream_context << iter->c_str() << '\n';
        }
    }

    ro_string_context_content = o_stringstream_context.str();
    IF (!ro_string_context_content.empty()) {
        ro_string_context_content.resize(ro_string_context_content.size() - 1);
    }

    return;
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
