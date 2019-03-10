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
#include "Vr_Asr_Impl_IFlyTek_Fn_Callback_Msg.h"
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

static C_Engine_Concrete* sg_p_Engine_Concrete = NULL;

ISSErrID
Fn_Network_Connect()
{
    MACRO_Log_Function

    IF (NULL == sg_p_Engine_Concrete) {
        MACRO_Log_Error
        return ISS_ERROR_FAIL;
    }

    IF (!sg_p_Engine_Concrete->m_scpo_Request_Recognize) {
        MACRO_Log_Error
        return ISS_ERROR_FAIL;
    }

    sg_p_Engine_Concrete->m_scpo_Request_Recognize->m_function_On_Event_Notify(
        C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Network_Connect)
    );

    return ISS_SUCCESS;
}

ISSErrID
Fn_Network_Disconnect()
{
    MACRO_Log_Function

    IF (NULL == sg_p_Engine_Concrete) {
        MACRO_Log_Error
        return ISS_ERROR_FAIL;
    }

    IF (!sg_p_Engine_Concrete->m_scpo_Request_Recognize) {
        MACRO_Log_Error
        return ISS_ERROR_FAIL;
    }

    sg_p_Engine_Concrete->m_scpo_Request_Recognize->m_function_On_Event_Notify(
        C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Network_Disconnect)
    );

    return ISS_SUCCESS;
}


C_Engine_Concrete::~C_Engine_Concrete()
{
    MACRO_Log_Function
    sg_p_Engine_Concrete = NULL;
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
, m_o_HISSSR(NULL)
, m_scpo_Mgr_Data(MACRO_New C_Mgr_Data(*this))
, m_function_On_Event_Init_Notify(ro_function_On_Event_Init_Notify)
, m_string_Center_Vr_Url(ro_string_Center_Vr_Url)
, m_p_Func_Network_Change(NULL)
, m_spstring_Result_Json()
, m_b_Init(false)
, m_b_Error(false)
, m_b_Init_Status(false)
, m_b_Rec_Running(false)
, m_b_Acc_Off(false)
, m_i_Audio_Data_Cnt(0)
, m_cond_Init()
, m_o_Time_Speech_Begin()
, m_o_Time_Speech_End()
, m_o_Time_Speech_TimeOut()
, m_o_Time_Result()
, m_b_Log_File(false)
, m_o_Log_File()
, m_string_Id_Language(rstring_Id_Language)
, m_i_Volume_Level(0)
, m_map_string_Iss_Sr_Mode()
, m_map_string_Iss_Sr_Sence()
, m_map_string_Iss_Sr_Acous_Lang()
{
    MACRO_Log_Function
    sg_p_Engine_Concrete = this; 
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

    e_Result = _M_Fn_Mgr_Init();
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

    e_Result = _M_Fn_IFlyTek_Fina();
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
    }

    e_Result = _M_Fn_Mgr_Fina();
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

    ISSErrID o_ISSErrID;

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

    string string_Json_Dictionary;
    e_Result = _M_Fn_Format_Json_Dictionary(
        *spo_Request_Context_List_Update,
        string_Json_Dictionary
    );
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }
    Fn_Log(string_Json_Dictionary);
    
    m_o_Log_File.M_Fn_Log(string_Json_Dictionary);
    m_o_Log_File.M_Fn_Enter();

    m_list_o_Time_Context_list_Update.push_back(C_Time::M_Fn_Get_Time());
    m_list_spo_Request_Context_List_Update.push_back(spo_Request_Context_List_Update);
    o_ISSErrID = G_o_IFlyTek().M_Fn_ISSSRUpLoadDict(m_o_HISSSR, string_Json_Dictionary.c_str());
    IF (ISS_SUCCESS != o_ISSErrID) {
        m_list_o_Time_Context_list_Update.pop_front();
        m_list_spo_Request_Context_List_Update.pop_front();
        Fn_Log_IFlyTek_ERROR(o_ISSErrID);
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}

#if 0

E_Result
C_Engine_Concrete::M_Fn_Data_Context_Fuzzy_Compile
(
    C_Request_Context_Fuzzy_Compile & ro_Request_Context_Fuzzy_Compile
)
{
    MACRO_Log_Function

    MACRO_Log_Error

    return E_Result_Succeeded;
}

#endif

E_Result
C_Engine_Concrete::M_Fn_Rec_Start
(
    C_Request_Recognize & ro_Request_Recognize
)
{
    MACRO_Log_Function

    E_Result e_Result;

    ISSErrID o_ISSErrID;

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

    Fn_Log("Set Request_Recognize Param");
    C_Request_Param_Set o_Request_Param_Set;
    IF (!m_scpo_Request_Recognize->m_string_Longtitude.empty()) {
        o_Request_Param_Set.m_e_Param_Type = E_Param_Type_Longitude;
        o_Request_Param_Set.m_string_Value = m_scpo_Request_Recognize->m_string_Longtitude;
        e_Result = M_Fn_Param_Set(o_Request_Param_Set);
        IF (E_Result_Succeeded != e_Result) {
            Fn_Log("Failed to set longtitude!");
            return E_Result_Error;
        }
    }

    IF (!m_scpo_Request_Recognize->m_string_Latitude.empty()) {
        o_Request_Param_Set.m_e_Param_Type = E_Param_Type_Latitude;
        o_Request_Param_Set.m_string_Value = m_scpo_Request_Recognize->m_string_Latitude;
        e_Result = M_Fn_Param_Set(o_Request_Param_Set);
        IF (E_Result_Succeeded != e_Result) {
            Fn_Log("Failed to set latitude!");
            return E_Result_Error;
        }
    }

    IF (!m_scpo_Request_Recognize->m_string_City.empty()) {
        o_Request_Param_Set.m_e_Param_Type = E_Param_Type_Local_City;
        o_Request_Param_Set.m_string_Value = m_scpo_Request_Recognize->m_string_City;
        e_Result = M_Fn_Param_Set(o_Request_Param_Set);
        IF (E_Result_Succeeded != e_Result) {
            Fn_Log("Failed to set local city!");
            return E_Result_Error;
        }
    }

    string string_Json_Local_Cmd;
    IF (!m_scpo_Request_Recognize->m_list_spo_Term_Content_Local_Cmd.empty()) {
        e_Result = _M_Fn_Format_Json_Local_Cmd(
            m_scpo_Request_Recognize->m_list_spo_Term_Content_Local_Cmd,
            string_Json_Local_Cmd);
        IF (E_Result_Succeeded != e_Result) {
            MACRO_Log_Error
            return E_Result_Error;
        }
    }
    Fn_Log(string_Json_Local_Cmd);

    m_o_Log_File.M_Fn_Log(string_Json_Local_Cmd);
    m_o_Log_File.M_Fn_Enter();

    string string_Sence = m_scpo_Request_Recognize->m_string_Id_Dialog;
    ISS_SR_SCENE o_Iss_Sr_Scence;
    e_Result = _M_Fn_Get_IFlyTek_Sence(
        string_Sence,
        o_Iss_Sr_Scence);
    IF (E_Result_Succeeded != e_Result) {
        Fn_Log("The Sr Scence is not supported : " + m_scpo_Request_Recognize->m_string_Id_Dialog);
        MACRO_Log_Error
        return E_Result_Error;
    }

    string string_Mode = m_scpo_Request_Recognize->m_string_Id_Mode;
    IF (string_Mode.empty()) {
        string_Mode = g_string_Iss_Sr_Mode_Local;
    }

    ISS_SR_MODE o_Iss_Sr_Mode;
    e_Result = _M_Fn_Get_IFlyTek_Mode(
        string_Mode,
        o_Iss_Sr_Mode);
    IF (E_Result_Succeeded != e_Result) {
        Fn_Log("The Sr Mode is not supported : " + m_scpo_Request_Recognize->m_string_Id_Mode);
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_scpo_Request_Recognize->m_function_On_Event_Phase(
        C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Proc_Begin)
    );

    o_ISSErrID = G_o_IFlyTek().M_Fn_ISSSRStart(
        m_o_HISSSR,
        o_Iss_Sr_Scence,
        o_Iss_Sr_Mode,
        string_Json_Local_Cmd.c_str(),
        m_scpo_Request_Recognize->m_string_Custom.c_str());
    IF (ISS_SUCCESS != o_ISSErrID) {
        Fn_Log_IFlyTek_ERROR(o_ISSErrID);
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

    ISSErrID o_ISSErrID;

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

    o_ISSErrID = G_o_IFlyTek().M_Fn_ISSSRStop(m_o_HISSSR);
    IF (ISS_SUCCESS == o_ISSErrID) {
        _M_Fn_On_Rec_End();
    }
    ELSE {
        stringstream stringstream_Log;
        stringstream_Log << "M_Fn_ISSSRStop failed: " << o_ISSErrID;
        Fn_Log(stringstream_Log.str());
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::M_Fn_Append_Audio_Data
(
    short* pi_Audio_Buffer,
    unsigned int i_Audio_Frames,
    unsigned int& i_Write_Audio_Frames
)
{
    ISSErrID o_ISSErrID;
    stringstream stringstream_Log;

    ++m_i_Audio_Data_Cnt;
    IF (m_i_Audio_Data_Cnt <= 10) {
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << ">> VR_P push audio data to iflytek, Cnt: " << m_i_Audio_Data_Cnt;
        Fn_Log(stringstream_Log.str());
    }
   
    o_ISSErrID = G_o_IFlyTek().M_Fn_ISSSRAppendAudioData(
        m_o_HISSSR,
        pi_Audio_Buffer,
        i_Audio_Frames,
        &i_Write_Audio_Frames);
    IF (ISS_SUCCESS != o_ISSErrID) {
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "Append audio data to iflytek : " << o_ISSErrID;
        Fn_Log(stringstream_Log.str());
        return E_Result_Error;
    }

    IF (m_i_Audio_Data_Cnt <= 10) {
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "<< VR_P push audio data to iflytek, Cnt: " << m_i_Audio_Data_Cnt;
        Fn_Log(stringstream_Log.str());
    }

    return E_Result_Succeeded;
}

#if 0

E_Result
C_Engine_Concrete::M_Fn_End_Append_Audio_Data()
{
    ISSErrID o_ISSErrID;

    o_ISSErrID = G_o_IFlyTek().M_Fn_ISSSREndAudioData(m_o_HISSSR);
    IF (ISS_SUCCESS != o_ISSErrID) {
        Fn_Log_IFlyTek_ERROR(o_ISSErrID);
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}

#endif

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

    ISSErrID o_ISSErrID;

    string string_Id_Param;

    int i_Param_Type  = 0;

    ISS_SR_RESOURCE_TYPE o_ISS_SR_RESOURCE_TYPE = ISS_SR_RESOURCE_MAP;

    switch (ro_Request_Param_Set.m_e_Param_Type) {
    case E_Param_Type_Acc_Off: {
        m_b_Acc_Off = ro_Request_Param_Set.m_i_Value;
        return E_Result_Succeeded;
    }
    case E_Param_Type_Fx_Timeout_Speech: {

        string_Id_Param = ISS_SR_PARAM_SPEECH_TIMEOUT;
        break;
    }
    case E_Param_Type_Fx_Timeout_Silence_Leading: {

        string_Id_Param = ISS_SR_PARAM_RESPONSE_TIMEOUT;
        break;
    }
    case E_Param_Type_Fx_Timeout_Silence_Trailing: {

        string_Id_Param = ISS_SR_PARAM_SPEECH_TAIL;
        break;
    }
    case E_Param_Type_Longitude: {

        string_Id_Param = ISS_SR_PARAM_LONGTITUDE;
        break;
    }
    case E_Param_Type_Latitude: {

        string_Id_Param = ISS_SR_PARAM_LATITUDE;
        break;
    }
    case E_Param_Type_Local_City: {
    
        string_Id_Param = ISS_SR_PARAM_CITY;
        break;
    }
    case E_Param_Type_Audio_Src: {

        string_Id_Param = ISS_SR_PARAM_AUDIO_SRC;
        break;
    }
    case E_Param_Type_Debug_Level: {
    
        string_Id_Param = ISS_SR_PARAM_TRACE_LEVEL;
        break;
    }
    case E_Param_Type_Contact_Src: {

        string_Id_Param = ISS_SR_PARAM_CONTACT_SRC;
        break;
    }
    case E_Param_Type_Resource_Map: {
        i_Param_Type = 1;
        o_ISS_SR_RESOURCE_TYPE = ISS_SR_RESOURCE_MAP;
        break;
    }
    case E_Param_Type_Resource_Cmd: {
        i_Param_Type = 1;
        o_ISS_SR_RESOURCE_TYPE = ISS_SR_RESOURCE_CMD;
        break;
    }
    case E_Param_Type_Resource_Song: {
        i_Param_Type = 1;
        o_ISS_SR_RESOURCE_TYPE = ISS_SR_RESOURCE_SONG;
        break;
    }
    case E_Param_Type_Msg_Login: {
        i_Param_Type = 2;
        break;
    }
    case E_Param_Type_Network_Change: {
        i_Param_Type = 3;
        break;
    }
    case E_Param_Type_Center_Vr_Url: {
        i_Param_Type = 4;
        break;
    }
    default: {

        MACRO_Log_Error
        return E_Result_Failed;
    }
    }

    stringstream stringstream_Paramter;
    IF (0 == i_Param_Type) {
        stringstream_Paramter << "Param_Id: " << string_Id_Param
                              << ", Param_Value: " << ro_Request_Param_Set.m_string_Value;
        Fn_Log(stringstream_Paramter.str());

        m_o_Log_File.M_Fn_Log(stringstream_Paramter.str());
        m_o_Log_File.M_Fn_Enter();
    
        o_ISSErrID = G_o_IFlyTek().M_Fn_ISSSRSetParam(
            m_o_HISSSR,
            string_Id_Param.c_str(),
            ro_Request_Param_Set.m_string_Value.c_str());
    }
#ifdef MACRO_Dynamic_Debug    
    ELSE IF (1 == i_Param_Type) {
        stringstream_Paramter << "Load_Resource: " << ro_Request_Param_Set.m_i_Value
                              << ", Resource_Type: " << static_cast<int>(o_ISS_SR_RESOURCE_TYPE);
        Fn_Log(stringstream_Paramter.str()); 
        
        m_o_Log_File.M_Fn_Log(stringstream_Paramter.str());
        m_o_Log_File.M_Fn_Enter();
        
        IF (ro_Request_Param_Set.m_i_Value) {
            o_ISSErrID = G_o_IFlyTek().M_Fn_ISSSRLoadResource(m_o_HISSSR, o_ISS_SR_RESOURCE_TYPE);

        }
        ELSE {
            o_ISSErrID = G_o_IFlyTek().M_Fn_ISSSRUnLoadResource(m_o_HISSSR, o_ISS_SR_RESOURCE_TYPE);
        }
    }
    ELSE IF (2 == i_Param_Type) {
        stringstream_Paramter << "Msp_Login";
        Fn_Log(stringstream_Paramter.str()); 
        
        m_o_Log_File.M_Fn_Log(stringstream_Paramter.str());
        m_o_Log_File.M_Fn_Enter();
        
        o_ISSErrID = G_o_IFlyTek().M_Fn_ISSSRMspLogin(m_o_HISSSR);
    }
    ELSE IF (3 == i_Param_Type) { 
        stringstream_Paramter << "Network Change: " << ro_Request_Param_Set.m_i_Value;
        Fn_Log(stringstream_Paramter.str());

        m_o_Log_File.M_Fn_Log(stringstream_Paramter.str());
        m_o_Log_File.M_Fn_Enter();

        o_ISSErrID = m_p_Func_Network_Change(static_cast<EStatus>(ro_Request_Param_Set.m_i_Value));
    }
    ELSE { // 4 == i_para_Type
        string string_Center_Vr_Url = ro_Request_Param_Set.m_string_Value;

        IF (string_Center_Vr_Url.empty()) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        stringstream_Paramter << "{\"MspServerUrl\": \""
                              << string_Center_Vr_Url
                              << "\"}";
        Fn_Log(stringstream_Paramter.str());

        m_o_Log_File.M_Fn_Log(stringstream_Paramter.str());
        m_o_Log_File.M_Fn_Enter();

        o_ISSErrID = G_o_IFlyTek().M_Fn_ISSSRSetGlobalCfg(stringstream_Paramter.str().c_str());
        IF (ISS_SUCCESS != o_ISSErrID) {
            Fn_Log_IFlyTek_ERROR(o_ISSErrID);
            MACRO_Log_Error
            return E_Result_Error;
        }

        stringstream_Paramter.clear();
        stringstream_Paramter.str("");
        stringstream_Paramter << "Msp_Login";
        Fn_Log(stringstream_Paramter.str()); 
        
        m_o_Log_File.M_Fn_Log(stringstream_Paramter.str());
        m_o_Log_File.M_Fn_Enter();
        
        o_ISSErrID = G_o_IFlyTek().M_Fn_ISSSRMspLogin(m_o_HISSSR);
    }
    
    IF (ISS_SUCCESS != o_ISSErrID) {
        Fn_Log_IFlyTek_ERROR(o_ISSErrID);
        MACRO_Log_Error
        return E_Result_Error;
    }
#endif

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::M_Fn_On_Callback_Msg
(
    unsigned int i_Msg,
    unsigned int i_wParam,
    const void* pv_lParam
)
{
    stringstream stringstream_Log;

    switch (i_Msg) {
    case ISS_SR_MSG_InitStatus: {
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "ISS_SR_MSG_InitStatus: " << i_wParam;
        Fn_Log(stringstream_Log.str());

        IF (ISS_SUCCESS == i_wParam) {
            m_b_Init_Status = true;
        }
        ELSE {
            m_b_Init_Status = false;
        }
        m_cond_Init.Notify();
        
        break;
    }
    case ISS_SR_MSG_UpLoadDictToLocalStatus: {   
        IF (m_list_spo_Request_Context_List_Update.empty()
            || m_list_o_Time_Context_list_Update.empty()) {
            m_b_Error = true;
            MACRO_Log_Error
            return E_Result_Error;
        }      

        C_Time o_list_Context_Update_Time = C_Time::M_Fn_Get_Time() - m_list_o_Time_Context_list_Update.front();
        m_list_o_Time_Context_list_Update.pop_front();
        float f_list_Context_Update_Time = o_list_Context_Update_Time.M_Fn_Normalize();
        
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "ISS_SR_MSG_UpLoadDictToLocalStatus: " << i_wParam
                         << ", Cost Time: " << f_list_Context_Update_Time;
        Fn_Log(stringstream_Log.str());
        m_o_Log_File.M_Fn_Log(stringstream_Log.str());
        m_o_Log_File.M_Fn_Enter();

        shared_ptr<C_Request_Context_List_Update> spo_Request_Context_List_Update;
        spo_Request_Context_List_Update.swap(
            m_list_spo_Request_Context_List_Update.front());
        m_list_spo_Request_Context_List_Update.pop_front();

        IF (ISS_SUCCESS != i_wParam) {
            spo_Request_Context_List_Update->m_function_On_Event_Notify(
               C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Local_Dictionary_Upload_Error)
            );
        }

        spo_Request_Context_List_Update->m_function_On_Event_Notify(
            C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Context_List_File_Save_Finish)
        );

        spo_Request_Context_List_Update->m_function_On_Event_Phase(
            C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Proc_End)
        );
        
        break;
    }
    case ISS_SR_MSG_UpLoadDictToCloudStatus: {     
        IF (m_list_spo_Request_Context_List_Update.empty()
            || m_list_o_Time_Context_list_Update.empty()) {
            m_b_Error = true;
            MACRO_Log_Error
            return E_Result_Error;
        }      

        C_Time o_list_Context_Update_Time = C_Time::M_Fn_Get_Time() - m_list_o_Time_Context_list_Update.front();
        m_list_o_Time_Context_list_Update.pop_front();
        float f_list_Context_Update_Time = o_list_Context_Update_Time.M_Fn_Normalize();
        
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "ISS_SR_MSG_UpLoadDictToCloudStatus: " << i_wParam
                         << ", Cost Time: " << f_list_Context_Update_Time;
        Fn_Log(stringstream_Log.str());
        m_o_Log_File.M_Fn_Log(stringstream_Log.str());
        m_o_Log_File.M_Fn_Enter();

        shared_ptr<C_Request_Context_List_Update> spo_Request_Context_List_Update;
        spo_Request_Context_List_Update.swap(
            m_list_spo_Request_Context_List_Update.front());
        m_list_spo_Request_Context_List_Update.pop_front();

        IF (ISS_SUCCESS != i_wParam) {
            spo_Request_Context_List_Update->m_function_On_Event_Notify(
               C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Cloud_Dictionary_Upload_Error)
            );
        }

        spo_Request_Context_List_Update->m_function_On_Event_Notify(
            C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Context_List_File_Save_Finish)
        );

        spo_Request_Context_List_Update->m_function_On_Event_Phase(
            C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Proc_End)
        );

        break;
    }
    case ISS_SR_MSG_VolumeLevel: {
        IF (i_wParam > 931) {
            Fn_Log("    audio energy out of range!");
            MACRO_Log_Error
        }
        
        unsigned int i_Volume_Level = i_wParam * 100 / 931;
        
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "ISS_SR_MSG_VolumeLevel: " << i_Volume_Level;

        IF (static_cast<int>(i_Volume_Level) != m_i_Volume_Level) {
            Fn_Log(stringstream_Log.str());

            m_i_Volume_Level = static_cast<int>(i_Volume_Level);
            m_scpo_Request_Recognize->m_function_On_Event_Notify(
                C_Event_Notify_Impl_Signal_Level(m_i_Volume_Level, 0)
            );
        }
        break;
    }
    case ISS_SR_MSG_ResponseTimeout: {
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "ISS_SR_MSG_ResponseTimeout";
        Fn_Log(stringstream_Log.str());

        m_scpo_Request_Recognize->m_function_On_Event_Phase(
            C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Response_Timeout)
        );

        _M_Fn_Proc_On_Result(string(""));
        _M_Fn_Stop_Audio();
        _M_Fn_On_Rec_End();
        break;
    }
    case ISS_SR_MSG_SpeechStart: {
    
        m_o_Time_Speech_Begin = C_Time::M_Fn_Get_Time();
        
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "ISS_SR_MSG_SpeechStart at: sec(" 
                                 << m_o_Time_Speech_Begin.i_Sec_S 
                                 << ") nsec(" 
                                 << m_o_Time_Speech_Begin.i_Sec_N << ")";
        Fn_Log(stringstream_Log.str());

        m_o_Log_File.M_Fn_Log(stringstream_Log.str());
        m_o_Log_File.M_Fn_Enter();

        m_scpo_Request_Recognize->m_function_On_Event_Phase(
            C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Speech_Begin_Fx)
        );
        break;
    }
    case ISS_SR_MSG_SpeechTimeOut: {

        m_o_Time_Speech_TimeOut = C_Time::M_Fn_Get_Time();
        C_Time o_Time_Speech_TimeOut = m_o_Time_Speech_TimeOut - m_o_Time_Speech_Begin;
        float f_Speech_TimeOut = o_Time_Speech_TimeOut.M_Fn_Normalize();
        
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "ISS_SR_MSG_SpeechTimeOut at: sec(" 
                                 << o_Time_Speech_TimeOut.i_Sec_S 
                                 << ") nsec(" 
                                 << o_Time_Speech_TimeOut.i_Sec_N 
                                 << "), Speech TimeOut Len: "
                                 << f_Speech_TimeOut;
        Fn_Log(stringstream_Log.str());

        m_o_Log_File.M_Fn_Log(stringstream_Log.str());
        m_o_Log_File.M_Fn_Enter();

        m_scpo_Request_Recognize->m_function_On_Event_Phase(
            C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Speech_Timeout)
        );
        
        _M_Fn_Stop_Audio();
        break;
    }
    case ISS_SR_MSG_SpeechEnd: {

        m_o_Time_Speech_End = C_Time::M_Fn_Get_Time();
        C_Time o_Time_Speech_Len = m_o_Time_Speech_End - m_o_Time_Speech_Begin;
        float f_Speech_Len = o_Time_Speech_Len.M_Fn_Normalize();
        
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "ISS_SR_MSG_SpeechEnd at: sec(" 
                                 << m_o_Time_Speech_End.i_Sec_S 
                                 << ") nsec(" 
                                 << m_o_Time_Speech_End.i_Sec_N 
                                 << "), Speech Time Len: "
                                 << f_Speech_Len;
        Fn_Log(stringstream_Log.str());

        m_o_Log_File.M_Fn_Log(stringstream_Log.str());
        m_o_Log_File.M_Fn_Enter();
        
        m_scpo_Request_Recognize->m_function_On_Event_Phase(
            C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Speech_End_Fx)
        );

        _M_Fn_Stop_Audio();
        break;
    }
    case ISS_SR_MSG_Error: {
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "ISS_SR_MSG_Error: " << i_wParam << " : " << static_cast<const char*>(pv_lParam);
        Fn_Log(stringstream_Log.str());

        _M_Fn_Proc_On_Result(string(""));
        _M_Fn_Stop_Audio();
        _M_Fn_On_Rec_End();
        break;
    }
    case ISS_SR_MSG_PreResult: {
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "ISS_SR_MSG_PreResult: " << static_cast<const char*>(pv_lParam);
        Fn_Log(stringstream_Log.str());
        break;
    }
    case ISS_SR_MSG_Result: {

        m_o_Time_Result = C_Time::M_Fn_Get_Time();
        C_Time o_Time_Result_Len =  m_o_Time_Result - m_o_Time_Speech_End;
        float f_Result_Len = o_Time_Result_Len.M_Fn_Normalize();
        
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "ISS_SR_MSG_Result at: sec(" 
                                 << m_o_Time_Result.i_Sec_S 
                                 << ") nsec(" 
                                 << m_o_Time_Result.i_Sec_N 
                                 << "), Deal Time Len: "
                                 << f_Result_Len;
        Fn_Log(stringstream_Log.str());

        m_o_Log_File.M_Fn_Log(stringstream_Log.str());
        m_o_Log_File.M_Fn_Enter();

        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "ISS_SR_MSG_Result: " << static_cast<const char*>(pv_lParam);
        Fn_Log(stringstream_Log.str());
        
        _M_Fn_Proc_On_Result(string(static_cast<const char*>(pv_lParam)));
        _M_Fn_Stop_Audio();
        _M_Fn_On_Rec_End();
        break;
    }
    case ISS_SR_MSG_LoadBigSrResStatus: {
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "ISS_SR_MSG_LoadBigSrResStatus: " << i_wParam;
        Fn_Log(stringstream_Log.str());
        IF (ISS_SUCCESS != i_wParam) {
            m_b_Error = true;
        }
        
        break;
    }
    case ISS_SR_MSG_ErrorDecodingAudio: {
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "ISS_SR_MSG_ErrorDecodingAudio: ";
        Fn_Log(stringstream_Log.str());

        m_scpo_Request_Recognize->m_function_On_Event_Notify(
            C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Audio_Decode_Error)
        );

        _M_Fn_Proc_On_Result(string(""));
        _M_Fn_Stop_Audio();
        _M_Fn_On_Rec_End();

        break;
    }
    case ISS_SR_MSG_TooLoud: {
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "ISS_SR_MSG_TooLoud: ";
        Fn_Log(stringstream_Log.str());

        m_scpo_Request_Recognize->m_function_On_Event_Notify(
            C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Overload)
        );

        break;
    }
    case ISS_SR_MSG_TooLow: {
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "ISS_SR_MSG_TooLow: ";
        Fn_Log(stringstream_Log.str());

        m_scpo_Request_Recognize->m_function_On_Event_Notify(
            C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Too_Quiet)
        );

        break;
    }
    case ISS_SR_MSG_Noisy: {
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "ISS_SR_MSG_Noisy: ";
        Fn_Log(stringstream_Log.str());
        
        m_scpo_Request_Recognize->m_function_On_Event_Notify(
            C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Too_Noisy)
        );

        break;
    }
    case ISS_SR_MSG_UserCmdResourceBroken: {
        string string_Resource_Id(static_cast<const char*>(pv_lParam));
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "ISS_SR_MSG_UserCmdResourceBroken: " << string_Resource_Id;
        Fn_Log(stringstream_Log.str());

        IF (m_function_On_Event_Init_Notify) {
            m_function_On_Event_Init_Notify(
                C_Event_Init_Notify_Impl(
                    C_Event_Init_Notify::E_Event_Init_Notify_Type_Context_Invalid,
                    string_Resource_Id
                )
            );
        }

        break;
    }
    case ISS_SR_MSG_MapResourceBroken: {
        string string_Map_Resource_Id(static_cast<const char*>(pv_lParam));
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "ISS_SR_MSG_MapResourceBroken: " << string_Map_Resource_Id;
        Fn_Log(stringstream_Log.str());

        IF (m_function_On_Event_Init_Notify) {
            m_function_On_Event_Init_Notify(
                C_Event_Init_Notify_Impl(
                    C_Event_Init_Notify::E_Event_Init_Notify_Type_Map_Resource_Invalid,
                    string_Map_Resource_Id
                )
            );
        }

        break;
    }
    case ISS_SR_MSG_WaitingForCloudResult: {
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "ISS_SR_MSG_WaitingForCloudResult: " << static_cast<const char*>(pv_lParam);
        Fn_Log(stringstream_Log.str());

        break;
    }
    default: {
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "Unhandled Msg: " << i_Msg;
        Fn_Log(stringstream_Log.str());
        m_b_Error = true;

#ifndef MACRO_Dynamic_Debug
        _M_Fn_Proc_On_Result(string(""));
        _M_Fn_Stop_Audio();
        _M_Fn_On_Rec_End();
#endif

        MACRO_Log_Error
        return E_Result_Error;
    }
    }

    return E_Result_Succeeded;
}

void
C_Engine_Concrete::_M_Fn_Proc_On_Result
(
    const string& string_Result
)
{
    C_Event_Result_Impl o_Event_Result_Impl;

    m_spstring_Result_Json.reset(MACRO_New string);
    *m_spstring_Result_Json = string_Result;
    o_Event_Result_Impl.m_spstring_Result_Json = m_spstring_Result_Json;

    m_scpo_Request_Recognize->m_function_On_Event_Result(
        o_Event_Result_Impl
    );

    m_o_Log_File.M_Fn_Log(string_Result);
    m_o_Log_File.M_Fn_Enter();

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
C_Engine_Concrete::_M_Fn_IFlyTek_Fina()
{
    MACRO_Log_Function

    ISSErrID o_ISSErrID;

    IF (m_b_Acc_Off) {
        Fn_Log("iflytek acc off!");
        m_b_Acc_Off = false;
        o_ISSErrID = G_o_IFlyTek().M_Fn_ISSSRAccOff(m_o_HISSSR);
    }
    ELSE {
        o_ISSErrID = G_o_IFlyTek().M_Fn_ISSSRDestroy(m_o_HISSSR);
    }

    IF (o_ISSErrID != ISS_SUCCESS) {
        Fn_Log_IFlyTek_ERROR(o_ISSErrID);
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_b_Init_Status = false;

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_IFlyTek_Init()
{
    MACRO_Log_Function

    ISSErrID o_ISSErrID;

    E_Result e_Result;

    char a_i_fake_machine_id_buf[1024] = "ThisIsAFakeMachineIdWhichNotLongerThan1024";
    o_ISSErrID = G_o_IFlyTek().M_Fn_ISSSetMachineCode(a_i_fake_machine_id_buf);
    IF (ISS_SUCCESS != o_ISSErrID) {
        Fn_Log_IFlyTek_ERROR(o_ISSErrID);
        MACRO_Log_Error
        return E_Result_Error;
    }

    stringstream stringstream_Paramter;
    
    string string_Url_Key_Value;
    if (!m_string_Center_Vr_Url.empty()) {
        stringstream_Paramter << "\"MspServerUrl\": \""
                              << m_string_Center_Vr_Url
                              << "\"";
        string_Url_Key_Value = stringstream_Paramter.str();
    }

    string string_Map_Path = G_o_Get_Path().M_Fn_Get_string_Path_PData();
    size_t i_Dir_Pos = string_Map_Path.find_last_of("ASR");
    IF (string::npos != i_Dir_Pos) {
        string_Map_Path.resize(i_Dir_Pos - 2);
    }

    bool b_Map_Path_Exist = false;
    IF (Fn_File_Access(string_Map_Path + "ResourceOfMap")) {
        b_Map_Path_Exist = true;
        string_Map_Path = "\"LocalMapResource\": \"" + string_Map_Path + "\",";
    }

    stringstream_Paramter.clear();
    stringstream_Paramter.str("");

    stringstream_Paramter << "{"
                          << (b_Map_Path_Exist ? string_Map_Path : "")
                          << "\"LocalCmdResource\": \"" << G_o_Get_Path().M_Fn_Get_string_Path_Usr().c_str() << "\""
                          << (string_Url_Key_Value.empty() ? "" : ("," + string_Url_Key_Value))
                          << "}";
    Fn_Log(stringstream_Paramter.str());                          
    m_o_Log_File.M_Fn_Log(stringstream_Paramter.str());
    m_o_Log_File.M_Fn_Enter();                     
    o_ISSErrID = G_o_IFlyTek().M_Fn_ISSSRSetGlobalCfg(stringstream_Paramter.str().c_str());
    IF (ISS_SUCCESS != o_ISSErrID) {
        Fn_Log_IFlyTek_ERROR(o_ISSErrID);
        MACRO_Log_Error
        return E_Result_Error;
    }

    string string_Path = G_o_Get_Path().M_Fn_Get_string_Path_VData();
    Fn_Log(string_Path);
    char a_i_path[PATH_MAX] = { 0 };
    size_t i_path_Len = string_Path.size();
    IF ((i_path_Len + 1) > PATH_MAX) {
        MACRO_Log_Error
        return E_Result_Error;
    }
    memcpy(a_i_path, string_Path.c_str(), i_path_Len);

    ISS_SR_ACOUS_LANG o_Iss_Sr_Acous_Lang;
#ifdef MACRO_Dynamic_Debug  

    e_Result = _M_Fn_Get_IFlyTek_Language(
        m_string_Id_Language,
        o_Iss_Sr_Acous_Lang);        
    IF (E_Result_Succeeded != e_Result) {
        Fn_Log("The language is not supported : " + m_string_Id_Language);
        MACRO_Log_Error
        return E_Result_Error;
    }
    
#else

    (void)_M_Fn_Get_IFlyTek_Language(
        m_string_Id_Language,
        o_Iss_Sr_Acous_Lang);
        
#endif    

    o_ISSErrID = G_o_IFlyTek().M_Fn_ISSSRCreate(
        &m_o_HISSSR,
        o_Iss_Sr_Acous_Lang,
        a_i_path,
        Fn_Callback_Msg,
        this,
        Fn_Network_Connect,
        Fn_Network_Disconnect,
        &m_p_Func_Network_Change);
    IF (ISS_SUCCESS != o_ISSErrID) {
        Fn_Log_IFlyTek_ERROR(o_ISSErrID);
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_cond_Init.Wait();

    IF (!m_b_Init_Status) {
        Fn_Log("Failed to initialize iflytek!");
        MACRO_Log_Error
        return E_Result_Error;
    }

    C_Request_Param_Set o_Request_Param_Set;

    o_Request_Param_Set.m_e_Param_Type = E_Param_Type_Fx_Timeout_Speech;
    stringstream_Paramter.clear();
    stringstream_Paramter.str("");
    stringstream_Paramter << static_cast<int>(E_Param_Value_Default_Fx_Timeout_Speech);
    o_Request_Param_Set.m_string_Value = stringstream_Paramter.str();
    
#ifdef MACRO_Dynamic_Debug  

    e_Result = M_Fn_Param_Set(o_Request_Param_Set);    
    IF (E_Result_Succeeded != e_Result) {
        Fn_Log("Failed to set the time of speech timeout!");
        return E_Result_Error;
    }
    
#else

    M_Fn_Param_Set(o_Request_Param_Set);

#endif    

    o_Request_Param_Set.m_e_Param_Type = E_Param_Type_Fx_Timeout_Silence_Leading;
    stringstream_Paramter.clear();
    stringstream_Paramter.str("");
    stringstream_Paramter << static_cast<int>(E_Param_Value_Default_Fx_Timeout_Silence_Leading);
    o_Request_Param_Set.m_string_Value = stringstream_Paramter.str();
#ifdef MACRO_Dynamic_Debug  

    e_Result = M_Fn_Param_Set(o_Request_Param_Set);  
    IF (E_Result_Succeeded != e_Result) {
        Fn_Log("Failed to set the time of silence leading!");
        return E_Result_Error;
    }
    
#else

    M_Fn_Param_Set(o_Request_Param_Set);
    
#endif

    o_Request_Param_Set.m_e_Param_Type = E_Param_Type_Fx_Timeout_Silence_Trailing;
    stringstream_Paramter.clear();
    stringstream_Paramter.str("");
    stringstream_Paramter << static_cast<int>(E_Param_Value_Default_Fx_Timeout_Silence_Trailing);
    o_Request_Param_Set.m_string_Value = stringstream_Paramter.str();
#ifdef MACRO_Dynamic_Debug 

    e_Result = M_Fn_Param_Set(o_Request_Param_Set);  
    IF (E_Result_Succeeded != e_Result) {
        Fn_Log("Failed to set the time of silence trailing!");
        return E_Result_Error;
    }
    
#else

    M_Fn_Param_Set(o_Request_Param_Set);

#endif

    o_Request_Param_Set.m_e_Param_Type = E_Param_Type_Debug_Level;
    o_Request_Param_Set.m_string_Value = ISS_SR_PARAM_TRACE_LEVEL_VALUE_DEBUG;
#ifdef MACRO_Dynamic_Debug 

    e_Result = M_Fn_Param_Set(o_Request_Param_Set);   
    IF (E_Result_Succeeded != e_Result) {
        Fn_Log("Failed to set debug level!");
        return E_Result_Error;
    }
    
#else

    M_Fn_Param_Set(o_Request_Param_Set);

#endif    

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
C_Engine_Concrete::_M_Fn_Get_IFlyTek_Mode
(
    const string& string_mode,
    ISS_SR_MODE& o_ISS_SR_MODE
)
{
    stringstream stringstream_Log;

    stringstream_Log << "Mode(string): " << string_mode;
    Fn_Log(stringstream_Log.str());

    m_o_Log_File.M_Fn_Log(stringstream_Log.str());
    m_o_Log_File.M_Fn_Enter();
    
    IF (0 == m_map_string_Iss_Sr_Mode.count(string_mode)) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    o_ISS_SR_MODE = m_map_string_Iss_Sr_Mode[string_mode];
    
    stringstream_Log.clear();
    stringstream_Log.str("");
    stringstream_Log << "Mode(int): " << static_cast<int>(o_ISS_SR_MODE);
    Fn_Log(stringstream_Log.str());

    m_o_Log_File.M_Fn_Log(stringstream_Log.str());
    m_o_Log_File.M_Fn_Enter();

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Get_IFlyTek_Sence
(
    const string& string_sence,
    ISS_SR_SCENE& o_ISS_SR_SCENE
)
{
    stringstream stringstream_Log;

    stringstream_Log << "Sence(string): " << string_sence;
    Fn_Log(stringstream_Log.str());
  
    m_o_Log_File.M_Fn_Log(stringstream_Log.str());
    m_o_Log_File.M_Fn_Enter();

    IF (string_sence.empty()) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (0 == m_map_string_Iss_Sr_Sence.count(string_sence)) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    o_ISS_SR_SCENE = m_map_string_Iss_Sr_Sence[string_sence];

    stringstream_Log.clear();
    stringstream_Log.str("");
    stringstream_Log << "Sence(int): " << static_cast<int>(o_ISS_SR_SCENE);
    Fn_Log(stringstream_Log.str());

    m_o_Log_File.M_Fn_Log(stringstream_Log.str());
    m_o_Log_File.M_Fn_Enter();

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Get_IFlyTek_Language
(
    const string& string_language,
    ISS_SR_ACOUS_LANG& o_ISS_SR_ACOUS_LANG
)
{
    stringstream stringstream_Log;

    stringstream_Log << "Languate(string): " << string_language;
    Fn_Log(stringstream_Log.str());

    m_o_Log_File.M_Fn_Log(stringstream_Log.str());
    m_o_Log_File.M_Fn_Enter();
    
    IF (0 == m_map_string_Iss_Sr_Acous_Lang.count(string_language)) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    o_ISS_SR_ACOUS_LANG = m_map_string_Iss_Sr_Acous_Lang[string_language];
    
    stringstream_Log.clear();
    stringstream_Log.str("");
    stringstream_Log << "Languate(int): " << static_cast<int>(o_ISS_SR_ACOUS_LANG);
    Fn_Log(stringstream_Log.str());

    m_o_Log_File.M_Fn_Log(stringstream_Log.str());
    m_o_Log_File.M_Fn_Enter();

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_IFlyTek_Data_Init()
{
    E_Result e_Result;

    // language
    m_map_string_Iss_Sr_Acous_Lang[g_string_Iss_Sr_Acous_Lang_Mandarin] = ISS_SR_ACOUS_LANG_VALUE_MANDARIN;
    m_map_string_Iss_Sr_Acous_Lang[g_string_Iss_Sr_Acous_Lang_English] = ISS_SR_ACOUS_LANG_VALUE_ENGLISH;
    m_map_string_Iss_Sr_Acous_Lang[g_string_Iss_Sr_Acous_Lang_Cantonese] = ISS_SR_ACOUS_LANG_VALUE_CANTONESE;

    // mode
    m_map_string_Iss_Sr_Mode[g_string_Iss_Sr_Mode_Cloud] = ISS_SR_MODE_CLOUD_REC;
    m_map_string_Iss_Sr_Mode[g_string_Iss_Sr_Mode_Local] = ISS_SR_MODE_LOCAL_REC;
    m_map_string_Iss_Sr_Mode[g_string_Iss_Sr_Mode_Mix] = ISS_SR_MODE_MIX_REC;

    // sence
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
    m_map_string_Iss_Sr_Sence.clear();
    m_map_string_Iss_Sr_Acous_Lang.clear();
    m_map_string_Iss_Sr_Mode.clear();
    
    m_o_Log_File.M_Fn_Fina();
    m_b_Log_File = false;

    return E_Result_Succeeded;
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

        IF (string_Key == "PRIMARY_ID_DIALOG") {
            string_Id_Dialog = string_Value;
        }
        ELSE
        IF (string_Key == "CONTEXT") {
            string_Id_Context = string_Value;
        }
        ELSE
        IF (string_Key == "DIALOG_END") {
            break;
        }
        ELSE
        IF (string_Key.empty()) {
            continue;
        }
        ELSE {
            MACRO_Log_Error
            return E_Result_Error;
        }
    }

    IF (string_Id_Dialog.empty() || string_Id_Context.empty()) {
        MACRO_Log_Error
        return E_Result_Error;
    }
    Fn_Log("    Dialog: " + string_Id_Dialog + "    Context: " + string_Id_Context);

    i_find_pos = string_Id_Context.find_last_of(':');
    IF (string::npos == i_find_pos) {
        MACRO_Log_Error
        return E_Result_Error;
    }
    string_Id_Context = string(string_Id_Context, i_find_pos + 1);
    i_Id_Context = ::atoi(string_Id_Context.c_str());
    Fn_Log("    Context: " + string_Id_Context);

    IF (0 != m_map_string_Iss_Sr_Sence.count(string_Id_Dialog)) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_map_string_Iss_Sr_Sence[string_Id_Dialog] = static_cast<ISS_SR_SCENE>(i_Id_Context);

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Format_Json_Dictionary
(
    C_Request_Context_List_Update & ro_Request_Context_List_Update,
    string & ro_string_Json_Dictionary
)
{
    rapidjson::GenericDocument<rapidjson::UTF8<> > document;
    document.SetObject();
    rapidjson::GenericDocument<rapidjson::UTF8<> >::AllocatorType& allocator = document.GetAllocator();

    // dic name and src
    string string_Dic_Src;
    string string_Dic_Name;
    _M_Fn_Get_Dic_Name_and_Src(
        ro_Request_Context_List_Update.m_string_Id_Context,
        string_Dic_Name,
        string_Dic_Src);
    IF (string_Dic_Name.empty()
        || string_Dic_Src.empty()) {
        
        Fn_Log("dic name: " + string_Dic_Name + " src name: " + string_Dic_Src);
        return E_Result_Error;
    }

    rapidjson::GenericValue<rapidjson::UTF8<> > json_Dic_Src(string_Dic_Src.c_str());
    rapidjson::GenericValue<rapidjson::UTF8<> > json_Dic_Name(string_Dic_Name.c_str());
    
    // dic content
    rapidjson::GenericValue<rapidjson::UTF8<> > json_Dic_Content;
    _M_Fn_Get_Json_Content(
        ro_Request_Context_List_Update.m_list_spo_Term_For_Add,
        json_Dic_Content,
        allocator);
    IF (json_Dic_Content.Empty()) {

        Fn_Log("dic content is empty!"); // empty is valid, for difference update
    }

    // grm content
    rapidjson::GenericValue<rapidjson::UTF8<> > json_Grm_Content;
    json_Grm_Content.SetObject();
    
    json_Grm_Content.AddMember("dictname", json_Dic_Name, allocator);
    json_Grm_Content.AddMember("dictsrc", json_Dic_Src, allocator);
    json_Grm_Content.AddMember("dictcontant", json_Dic_Content, allocator);
    
    // grm
    rapidjson::GenericValue<rapidjson::UTF8<> > json_Grm;
    json_Grm.SetArray();

    json_Grm.PushBack(json_Grm_Content, allocator);

    // document
    document.AddMember("grm", json_Grm, allocator);

    // json string
    rapidjson::StringBuffer json_string_buffer;
    rapidjson::Writer<rapidjson::StringBuffer> json_writer(json_string_buffer);
    
    document.Accept(json_writer);
    ro_string_Json_Dictionary = json_string_buffer.GetString();
    IF (ro_string_Json_Dictionary.empty()) {

        Fn_Log("json dictionary is empty!");
        return E_Result_Error;
    }
    
    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Format_Json_Local_Cmd
(
    list<shared_ptr<C_Term> > & ro_list_spo_Term_Local_Cmd,
    string & ro_string_Json_Local_Cmd
)
{
    rapidjson::GenericDocument<rapidjson::UTF8<> > document;
    document.SetObject();
    rapidjson::GenericDocument<rapidjson::UTF8<> >::AllocatorType& allocator = document.GetAllocator();

    // dic name
    string string_Dic_Name = "cmd";
    rapidjson::GenericValue<rapidjson::UTF8<> > json_Dic_Name(string_Dic_Name.c_str());
    
    // dic content
    rapidjson::GenericValue<rapidjson::UTF8<> > json_Dic_Content;
    _M_Fn_Get_Json_Content(
        ro_list_spo_Term_Local_Cmd,
        json_Dic_Content,
        allocator);
    IF (json_Dic_Content.Empty()) {

        Fn_Log("dic content is empty!");
        return E_Result_Error;
    }

    // grm content
    rapidjson::GenericValue<rapidjson::UTF8<> > json_Grm_Content;
    json_Grm_Content.SetObject();
    
    json_Grm_Content.AddMember("dictname", json_Dic_Name, allocator);
    json_Grm_Content.AddMember("dictcontant", json_Dic_Content, allocator);
    
    // grm
    rapidjson::GenericValue<rapidjson::UTF8<> > json_Grm;
    json_Grm.SetArray();

    json_Grm.PushBack(json_Grm_Content, allocator);

    // document
    document.AddMember("grm", json_Grm, allocator);

    // json string
    rapidjson::StringBuffer json_string_buffer;
    rapidjson::Writer<rapidjson::StringBuffer> json_writer(json_string_buffer);
    
    document.Accept(json_writer);
    ro_string_Json_Local_Cmd = json_string_buffer.GetString();
    IF (ro_string_Json_Local_Cmd.empty()) {

        Fn_Log("json dictionary is empty!");
        return E_Result_Error;
    }
    
    return E_Result_Succeeded;
}


void
C_Engine_Concrete::_M_Fn_Get_Dic_Name_and_Src
(
    string const & ro_string_Id_Conext,
    string & ro_string_Dic_Name,
    string & ro_string_Dic_Src
)
{
    size_t i_found_pos = string::npos;    
    string string_Id_Context = ro_string_Id_Conext;

    ro_string_Dic_Src.clear();
    ro_string_Dic_Name.clear();
    
    do {
        i_found_pos = string_Id_Context.find("ctx_phone_contact_name_list_");      
        IF (std::string::npos != i_found_pos) {
            ro_string_Dic_Name = "contact";
            string_Id_Context.replace(string_Id_Context.begin() + 9, string_Id_Context.end() - 2, "");
            break;
        }

        i_found_pos = string_Id_Context.find("ctx_media_play_album_list_");        
        IF (std::string::npos != i_found_pos) {
            ro_string_Dic_Name = "albums";
            string_Id_Context.replace(string_Id_Context.begin() + 9, string_Id_Context.end() - 2, "");
            break;
        }

        i_found_pos = string_Id_Context.find("ctx_media_play_playlist_list_");        
        IF (std::string::npos != i_found_pos) {
            ro_string_Dic_Name = "playlists";
            string_Id_Context.replace(string_Id_Context.begin() + 9, string_Id_Context.end() - 2, "");
            break;
        }

        i_found_pos = string_Id_Context.find("ctx_media_play_artist_list_");        
        IF (std::string::npos != i_found_pos) {
            ro_string_Dic_Name = "singers";
            string_Id_Context.replace(string_Id_Context.begin() + 9, string_Id_Context.end() - 2, "");
            break;
        }

        i_found_pos = string_Id_Context.find("ctx_media_play_music_list_");        
        IF (std::string::npos != i_found_pos) {
            ro_string_Dic_Name = "songs";
            string_Id_Context.replace(string_Id_Context.begin() + 9, string_Id_Context.end() - 2, "");
            break;
        }     
    } while (0);

    ro_string_Dic_Src = string_Id_Context;

    return;
}

void
C_Engine_Concrete::_M_Fn_Get_Json_Content
(
    list<shared_ptr<C_Term> > & ro_list_spo_Term,
    rapidjson::GenericValue<rapidjson::UTF8<> > & ro_Dic_Content,
    rapidjson::GenericDocument<rapidjson::UTF8<> >::AllocatorType & ro_Allocator
)
{
    ro_Dic_Content.SetArray();

    typedef vector<std::string>::iterator Vec_Iter;

    shared_ptr<C_Term> spo_Term;
    while (!ro_list_spo_Term.empty()) {

        spo_Term.swap(ro_list_spo_Term.front());
        ro_list_spo_Term.pop_front();

        for (
            Vec_Iter iter = spo_Term->m_vector_string_Orthography.begin();
            iter != spo_Term->m_vector_string_Orthography.end();
            ++iter
        ) {
            rapidjson::GenericValue<rapidjson::UTF8<> > content_entry;
            content_entry.SetObject();

            rapidjson::GenericValue<rapidjson::UTF8<> > content_entry_name(iter->c_str());
            rapidjson::GenericValue<rapidjson::UTF8<> > content_entry_id(spo_Term->m_i_User_Data_Lo);

            content_entry.AddMember("name", content_entry_name, ro_Allocator);
            content_entry.AddMember("id", content_entry_id, ro_Allocator);

            ro_Dic_Content.PushBack(content_entry, ro_Allocator);
        }
    }

    return;
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
