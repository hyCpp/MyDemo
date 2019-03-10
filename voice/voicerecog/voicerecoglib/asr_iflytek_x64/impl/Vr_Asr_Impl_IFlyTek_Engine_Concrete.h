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

    const char* p_session_id;

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
        unsigned int& i_Write_Audio_Frames,
        bool& b_Speech_End
    );

    E_Result
    M_Fn_On_Callback_Grammar_Build
    (
        int i_Ecode,
        const char* p_Info
    );

    E_Result
    M_Fn_On_Callback_Grammar_Update
    (
        int i_Ecode,
        const char* p_Info
    );

    void
    M_Fn_Proc_On_Result
    (
        bool b_User_Abort
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
    _M_Fn_Build_Grammar();

    void
    _M_Fn_Get_Context_Content
    (
        list<shared_ptr<C_Term> > & ro_list_spo_Term,
        string& ro_string_context_content
    );

    shared_ptr<string> m_spstring_Result_Json;
    int m_b_Init;
    int m_b_Error;
    int m_b_Rec_Running;
    unsigned int m_i_Audio_Data_Cnt;
    C_Semaphore m_cond_grammar_build;
    C_Semaphore m_cond_grammar_update;

    int m_b_Log_File;
    C_Log_File m_o_Log_File;

    string m_string_Id_Language;
    string m_string_IFlyTek_Key;
    string m_string_IFlyTek_Res;
    string m_string_curr_dialog_id;
    map<string, string> m_map_dialog_id_and_grammar_id;
    map<string, string> m_map_dialog_id_and_grammar_bnf;
};

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
