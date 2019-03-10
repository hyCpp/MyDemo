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

#ifndef __Vr_Asr_Impl_Engine_Concrete_h__
#define __Vr_Asr_Impl_Engine_Concrete_h__

#include "Vr_Asr_Impl_Engine_Base.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

class C_Engine_Agent;

namespace N_Vocon
{

class C_Heap;
class C_Data_CLC;
class C_Data_Party;
class C_Mgr_Data;
class C_Mgr_Rec;

/**
 * @brief The C_Engine_Concrete class
 *
 * C_Engine_Concrete class
 */

class C_Engine_Concrete : public C_Engine_Base
{
  public:

    friend class C_Data_CLC;

    C_Engine_Agent & m_ro_Engine_Agent;

    boost::function<void(C_Event_Init_Notify const &)> m_function_On_Event_Init_Notify;

    int m_b_Init;
    int m_b_Error;

    scoped_ptr<C_Heap> m_scpo_Heap; // Heap.

    scoped_ptr<C_Mgr_Data> m_scpo_Mgr_Data;

    scoped_ptr<C_Mgr_Rec> m_scpo_Mgr_Rec;

    C_Log_File m_o_Log_File;

    unsigned int m_i_Context_Count;

    virtual
    ~C_Engine_Concrete(); // [F]Destructor

    explicit
    C_Engine_Concrete // [F]Constructor of Default
    (
        C_Engine_Agent & ro_Engine_Agent,
        string const & rstring_Id_Language,
        boost::function<void(C_Event_Init_Notify const &)>& ro_function_On_Event_Init_Notify
    );

    string const &
    M_Fn_Get_rstring_Id_Language()
    {
        return m_string_Id_Language;
    }

    virtual
    LH_COMPONENT &
    M_Fn_Get_ro_LH_COMPONENT_Base();

    virtual
    LH_COMPONENT &
    M_Fn_Get_ro_LH_COMPONENT_Asr();

    virtual
    LH_COMPONENT &
    M_Fn_Get_ro_LH_COMPONENT_Pron();

    virtual
    LH_OBJECT &
    M_Fn_Get_ro_LH_OBJECT_Mod();

    virtual
    LH_OBJECT &
    M_Fn_Get_ro_LH_OBJECT_Rec();

    virtual
    LH_OBJECT &
    M_Fn_Get_ro_LH_OBJECT_Type_Audio_Sample();

    virtual
    LH_OBJECT &
    M_Fn_Get_ro_LH_OBJECT_Type_Audio_SSE();

    virtual
    LH_OBJECT &
    M_Fn_Get_ro_LH_OBJECT_Fx();

    virtual
    LH_OBJECT &
    M_Fn_Get_ro_LH_OBJECT_FmCtxCpl();

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
    M_Fn_Data_Suspend();

    E_Result
    M_Fn_Data_Resume();

    C_Data_CLC *
    M_Fn_Prepare_po_Data_CLC
    (
        string const & rstring_Id,
        string& ro_string_Id_Prev
    );

    E_Result
    M_Fn_Data_CLC_Transcription_Id_Version_Update
    (
        string const& ro_string_Transcription_Id_Version,
        int & b_Transcription_Id_Version_Supported
    );

    C_Data_Party *
    M_Fn_Find_po_Data_Party
    (
        string const & rstring_Id_Party
    );

    E_Result
    M_Fn_Write_Speaker_Data
    (
        int b_Fina
    );

    E_Result
    M_Fn_AcMod_Check_Dependency
    (
        string& string_File_Path,
        bool& b_Acmod_Match
    );

    E_Result
    M_Fn_Get_Transcriptions
    (
        C_Term& ro_Term
    );

  PROTECTED:

    virtual
    E_Result
    _M_Fn_Base_Init();

    virtual
    E_Result
    _M_Fn_Base_Fina();

    virtual
    E_Result
    _M_Fn_Asr_Init();

    virtual
    E_Result
    _M_Fn_Asr_Fina();

    virtual
    E_Result
    _M_Fn_Asr_Mod_Init();

    virtual
    E_Result
    _M_Fn_Asr_Mod_Fina();

    E_Result
    _M_Fn_Asr_Fx_Init();

    E_Result
    _M_Fn_Asr_Fx_Fina();

    E_Result
    _M_Fn_Asr_Rec_Init();

    E_Result
    _M_Fn_Asr_Rec_Fina();

    virtual
    E_Result
    _M_Fn_Pron_Init();

    virtual
    E_Result
    _M_Fn_Pron_Fina();

    E_Result
    _M_Fn_Pron_CLC_Init();

    E_Result
    _M_Fn_Pron_CLC_Fina();

    E_Result
    _M_Fn_Party_Init();

    E_Result
    _M_Fn_Party_Fina();

    E_Result
    _M_Fn_Mgr_Init();

    E_Result
    _M_Fn_Mgr_Fina();

    void
    _M_Fn_Check_Speaker_Data();

    E_Result
    _M_Fn_Get_Transcriptions
    (
        C_Term& ro_Term,
        C_Buffer& ro_Buffer_Transcription
    );

    C_Engine_Concrete // [F]Constructor: Copy
    (
        C_Engine_Concrete const & // [I]
    );

    C_Engine_Concrete & // [R]
    operator = // [F]Operator: =
    (
        C_Engine_Concrete const & // [I]
    );

    LH_COMPONENT m_o_LH_COMPONENT_Base; // Handle to the base component.
    LH_COMPONENT m_o_LH_COMPONENT_Asr; // Handle to the ASR component.
    LH_COMPONENT m_o_LH_COMPONENT_Pron; // Handle to the Pronounce component.

    LH_OBJECT m_o_LH_OBJECT_Mod; // Handle to the acoustic mode object.
    LH_OBJECT m_o_LH_OBJECT_Rec; // Handle to the SingleThreadedRec (STR) object.
    LH_OBJECT m_o_LH_OBJECT_Type_Audio_Sample; // Handle to the Audio type sample object.
    LH_OBJECT m_o_LH_OBJECT_Type_Audio_SSE; // Handle to the Audio type SSE object.
    LH_OBJECT m_o_LH_OBJECT_Fx; // Handle to the feature extractor (FX) object.
    LH_OBJECT m_o_LH_OBJECT_FmCtxCpl; // Handle to the Fuzzy Match Context Compiler object

    string m_string_Id_Language;
    string m_string_Path_Folder_Mod;
    string m_string_Path_File_Mod;
    string m_string_Path_File_CLC;
    string m_string_Path_File_CLC_V;
    string m_string_Path_File_CLC_M;
    string m_string_Path_File_Id_Trans_Lookup;
    string m_string_Path_Acmod_Session_Data;
    string m_string_Path_Fx_Session_Data;
    string m_string_Path_Speaker_Profile;

    list<shared_ptr<C_Data_CLC> > m_list_spo_Data_CLC;

    map<string, C_Data_CLC * > m_map_string_po_Data_CLC;

    map<string, shared_ptr<C_Data_Party> > m_map_string_spo_Data_Party;

    unsigned long m_i_Frequency_Sample; // Sampling frequency.
    unsigned long m_i_Frame_Shift; // Size of one frame in samples.

};

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
