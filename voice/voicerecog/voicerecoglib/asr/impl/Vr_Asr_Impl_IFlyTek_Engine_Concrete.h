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

/**
 * @file Voice_Asr_Impl_Engine_Concrete.h
 * @brief C_Engine_Concrete class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_IFlyTek_Engine_Concrete_h__
#define __Vr_Asr_Impl_IFlyTek_Engine_Concrete_h__

#include <rapidjson/document.h>
#include "Vr_Asr_Impl_Engine_Base.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{


class C_Engine_Agent;

namespace N_IFlyTek
{

class C_Mgr_Data;

#ifndef MACRO_Dynamic_Debug

ISSErrID Fn_Network_Connect();
ISSErrID Fn_Network_Disconnect();

#endif

/**
 * @brief The C_Engine_Concrete class
 *
 * C_Engine_Concrete class
 */

class C_Engine_Concrete : public C_Engine_Base
{
  public:

    C_Engine_Agent & m_ro_Engine_Agent;

    scoped_ptr<C_Request_Recognize> m_scpo_Request_Recognize;
    list<shared_ptr<C_Request_Context_List_Update> > m_list_spo_Request_Context_List_Update;
    list<C_Time> m_list_o_Time_Context_list_Update;

    HISSSR m_o_HISSSR;

    scoped_ptr<C_Mgr_Data> m_scpo_Mgr_Data;

    boost::function<void(C_Event_Init_Notify const &)> m_function_On_Event_Init_Notify;
    string m_string_Center_Vr_Url;

    ~C_Engine_Concrete(); // [F]Destructor

    explicit
    C_Engine_Concrete // [F]Constructor of Default
    (
        C_Engine_Agent & ro_Engine_Agent,
        string const & rstring_Id_Language,
        boost::function<void(C_Event_Init_Notify const &)>& ro_function_On_Event_Init_Notify,
        string const & ro_string_Center_Vr_Url
    );

    virtual
    E_Result
    M_Fn_Init();

    virtual
    E_Result
    M_Fn_Fina();

    virtual
    E_Result
    M_Fn_Party_Load
    (
        string const & rstring_Id_Party
    );

    virtual
    E_Result
    M_Fn_Party_Unload
    (
        string const & rstring_Id_Party
    );

    virtual
    E_Result
    M_Fn_Param_Set
    (
        C_Request_Param_Set & ro_Request_Param_Set
    );

    virtual
    E_Result
    M_Fn_Data_Context_List_Update
    (
        C_Request_Context_List_Update & ro_Request_Context_List_Update
    );

#if 0
    virtual
    E_Result
    M_Fn_Data_Context_Fuzzy_Compile
    (
        C_Request_Context_Fuzzy_Compile & ro_Request_Context_Fuzzy_Compile
    );
#endif

    virtual
    E_Result
    M_Fn_Rec_Start
    (
        C_Request_Recognize & ro_Request_Recognize
    );

    virtual
    E_Result
    M_Fn_Rec_Cancel();

    E_Result
    M_Fn_Append_Audio_Data
    (
        short* pi_Audio_Buffer,          
        unsigned int i_Audio_Frames,
        unsigned int& i_Write_Audio_Frames
    );

#if 0
    E_Result
    M_Fn_End_Append_Audio_Data();
#endif

    E_Result
    M_Fn_On_Callback_Msg
    (
        unsigned int i_Msg,
        unsigned int i_wParam,
        const void* pv_lParam
    );

  protected:

    C_Engine_Concrete // [F]Constructor: Copy
    (
        C_Engine_Concrete const & // [I]
    );

    C_Engine_Concrete & // [R]
    operator = // [F]Operator: =
    (
        C_Engine_Concrete const & // [I]
    );  

  PRIVATE:

    void
    _M_Fn_Proc_On_Result
    (
        const string& string_Result
    );
    
    E_Result
    _M_Fn_Start_Audio();
    
    E_Result
    _M_Fn_Stop_Audio();
    
    E_Result
    _M_Fn_Mgr_Init();

    E_Result
    _M_Fn_Mgr_Fina();

    E_Result
    _M_Fn_IFlyTek_Data_Init();

    E_Result
    _M_Fn_IFlyTek_Data_Fina();

    E_Result
    _M_Fn_IFlyTek_Init();

    E_Result
    _M_Fn_IFlyTek_Fina();

    void
    _M_Fn_On_Rec_End();
    
    E_Result
    _M_Fn_Get_IFlyTek_Mode
    (
        const string& string_mode,
        ISS_SR_MODE& o_ISS_SR_MODE
    );

    E_Result
    _M_Fn_Get_IFlyTek_Sence
    (
        const string& string_sence,
        ISS_SR_SCENE& o_ISS_SR_SCENE
    );

    E_Result
    _M_Fn_Get_IFlyTek_Language
    (
        const string& string_language,
        ISS_SR_ACOUS_LANG& o_ISS_SR_ACOUS_LANG
    );

    E_Result
    _M_Fn_Info_Parse_File();

    E_Result
    _M_Fn_Info_Parse_Line
    (
        ifstream & rifstream_File,
        string & rstring_Key,
        string & rstring_Value,
        bool & rb_Eof
    );

    E_Result
    _M_Fn_Info_Parse_Dialog
    (
        ifstream & rifstream_File
    );

    E_Result
    _M_Fn_Format_Json_Dictionary
    (
        C_Request_Context_List_Update & ro_Request_Context_List_Update,
        string & ro_string_Json_Dictionary
    );

    E_Result
    _M_Fn_Format_Json_Local_Cmd
    (
        list<shared_ptr<C_Term> > & ro_list_spo_Term_Local_Cmd,
        string & ro_string_Json_Local_Cmd
    );

    void
    _M_Fn_Get_Dic_Name_and_Src
    (
        string const & ro_string_Id_Conext,
        string & ro_string_Dic_Name,
        string & ro_string_Dic_Src
    );

    void
    _M_Fn_Get_Json_Content
    (
        list<shared_ptr<C_Term> > & ro_list_spo_Term,
        rapidjson::GenericValue<rapidjson::UTF8<> > & ro_Dic_Content,
        rapidjson::GenericDocument<rapidjson::UTF8<> >::AllocatorType & ro_Allocator
    );

    OnStatusChangedFunc m_p_Func_Network_Change;
    shared_ptr<string> m_spstring_Result_Json;

    int m_b_Init;
    int m_b_Error;
    int m_b_Init_Status;
    int m_b_Rec_Running;
    int m_b_Acc_Off;
    unsigned int m_i_Audio_Data_Cnt;
    C_Semaphore m_cond_Init;

    C_Time m_o_Time_Speech_Begin;
    C_Time m_o_Time_Speech_End;
    C_Time m_o_Time_Speech_TimeOut;
    C_Time m_o_Time_Result;
    int m_b_Log_File;
    C_Log_File m_o_Log_File;
    
    string m_string_Id_Language;
    int m_i_Volume_Level;
    map<string, ISS_SR_MODE> m_map_string_Iss_Sr_Mode;
    map<string, ISS_SR_SCENE> m_map_string_Iss_Sr_Sence;
    map<string, ISS_SR_ACOUS_LANG> m_map_string_Iss_Sr_Acous_Lang;
};

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
