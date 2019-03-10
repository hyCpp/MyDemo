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
 * @file Voice_Asr_Impl_Mgr_Data.h
 * @brief C_Mgr_Data class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Mgr_Data_h__
#define __Vr_Asr_Impl_Mgr_Data_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

class C_Engine_Agent;

namespace N_Vocon
{

class C_Request_Data;
class C_Data_Context;
class C_Data_Party;
class C_Thread_Data;
class C_Engine_Concrete;
class C_Data_CLC;

/**
 * @brief The C_Mgr_Data class
 *
 * C_Mgr_Data class
 */

class C_Mgr_Data
{
  public:

    C_Engine_Concrete & m_ro_Engine_Concrete;

    std::atomic<int> m_b_Stop;

    ~C_Mgr_Data(); // [F]Destructor

    explicit
    C_Mgr_Data // [F]Constructor of Default
    (
        C_Engine_Concrete & ro_Engine_Concrete
    );

    E_Result
    M_Fn_Init();

    E_Result
    M_Fn_Fina();

    void
    M_Fn_On_Thread_Notified();

    E_Result
    M_Fn_Context_List_Update
    (
        C_Request_Context_List_Update & ro_Request_Context_List_Update
    );
#if 0
    E_Result
    M_Fn_Context_Fuzzy_Compile
    (
        C_Request_Context_Fuzzy_Compile & ro_Request_Context_Fuzzy_Compile
    );
#endif

    E_Result
    M_Fn_Suspend();

    E_Result
    M_Fn_Resume();

  PROTECTED:

    E_Result
    _M_Fn_Thread_Init();

    E_Result
    _M_Fn_Thread_Fina();

    E_Result
    _M_Fn_Context_List_Update_Begin();

    E_Result
    _M_Fn_Context_List_Update_Clear();

    E_Result
    _M_Fn_Context_List_Update_Delete();

    E_Result
    _M_Fn_Context_List_Update_Add();

    E_Result
    _M_Fn_Context_List_Update_Commit();

#if 0
    E_Result
    _M_Fn_Context_Fuzzy_Compile_Begin();

    E_Result
    _M_Fn_Context_Fuzzy_Compile_Add_Column();

    E_Result
    _M_Fn_Context_Fuzzy_Compile_Add_Entry();

    E_Result
    _M_Fn_Context_Fuzzy_Compile_Run();
#endif

    E_Result
    _M_Fn_Context_List_File_Save();

    E_Result
    _M_Fn_Context_List_File_Delete();

    E_Result
    _M_Fn_Transcription_Id_Version_Update();

    C_Mgr_Data // [F]Constructor: Copy
    (
        C_Mgr_Data const & // [I]
    );

    C_Mgr_Data & // [R]
    operator = // [F]Operator: =
    (
        C_Mgr_Data const & // [I]
    );

    C_Mutex m_o_Mutex_Request;

    C_Mutex m_o_Mutex_Suspend;

    C_Mutex m_o_Mutex_Lock_Suspend;

    scoped_ptr<C_Lock_Mutex> m_scpo_Lock_Mutex_Suspend;

    scoped_ptr<C_Thread_Data> m_scpo_Thread_Data;

    map<unsigned int, list<shared_ptr<C_Request_Data> > >  m_map_priority_id_and_list_spo_Request_Data;

    shared_ptr<C_Request_Data> m_spo_Request_Data_Curr;
    C_Data_Context* m_po_Data_Context_Curr;
    list<C_Data_Context*> m_list_po_Data_Context;
    C_Data_CLC* m_po_Data_CLC;

    C_Time m_o_Time_Data_Begin;
    C_Time m_o_Time_Data_End;

    unsigned int m_i_Id_Request_Head;
    unsigned int m_i_Id_Request_Tail;

    list<unsigned int> m_list_i_Id_Request_Cancel;
};

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */