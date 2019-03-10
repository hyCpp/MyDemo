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

#ifndef __Vr_Asr_Impl_Kaldi_Engine_Concrete_h__
#define __Vr_Asr_Impl_Kaldi_Engine_Concrete_h__

#include "Vr_Asr_Impl_Engine_Base.h"
#include "Vr_Asr_Impl_Log.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{


class C_Engine_Agent;

namespace N_Kaldi
{

class C_Mgr_Rec;

/**
 * @brief The C_Engine_Concrete class
 *
 * C_Engine_Concrete class
 */

class C_Engine_Concrete : public C_Engine_Base
{
  public:

    C_Engine_Agent & m_ro_Engine_Agent;

    string m_string_Id_Language;

    boost::function<void(C_Event_Init_Notify const &)> m_function_On_Event_Init_Notify;

    scoped_ptr<C_Mgr_Rec> m_scpo_Mgr_Rec;

    int m_b_Init;

    int m_b_Error;

    C_Log_File m_o_Log_File;

    ~C_Engine_Concrete(); // [F]Destructor

    explicit
    C_Engine_Concrete // [F]Constructor of Default
    (
        C_Engine_Agent & ro_Engine_Agent,
        string const & rstring_Id_Language,
        boost::function<void(C_Event_Init_Notify const &)>& ro_function_On_Event_Init_Notify
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

    virtual
    E_Result
    M_Fn_Rec_Start
    (
        C_Request_Recognize & ro_Request_Recognize
    );

    virtual
    E_Result
    M_Fn_Rec_Cancel();

    void*
    M_Fn_Get_ro_LH_OBJECT_Rec()
    {
        return m_o_HKALDI;
    }

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
    _M_Fn_Mgr_Init();

    E_Result
    _M_Fn_Mgr_Fina();
    E_Result
    _M_Fn_Data_Init();

    E_Result
    _M_Fn_Data_Fina();

/*
    E_Result
    _M_Fn_Kaldi_Init();

    E_Result
    _M_Fn_Kaldi_Fina();
*/
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
    _M_Fn_Info_Parse_Model
    (
        ifstream & rifstream_File
    );

    void* m_o_HKALDI;
    string m_string_Model_Model;
    string m_string_Model_HCLG;
    string m_string_Model_Word_Syms;
    string m_string_Model_Silence_Phones;
    string m_string_Model_Lad_Mat;

    int m_b_Log_File;
};

} // name N_Kaldi

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
