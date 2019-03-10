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
#include "Vr_Asr_Impl_Event_Phase_Impl.h"
#include "Vr_Asr_Impl_Event_Notify_Impl.h"
#include "Vr_Asr_Impl_Vocon_Data_CLC.h"
#include "Vr_Asr_Impl_Vocon_Data_Context.h"
#include "Vr_Asr_Impl_Vocon_Data_Party.h"
#include "Vr_Asr_Impl_Vocon_Request_Data.h"
#include "Vr_Asr_Impl_Vocon_Thread_Data.h"
#include "Vr_Asr_Impl_Vocon_Mgr_Data.h"
#include "Vr_Asr_Impl_Vocon_Engine_Concrete.h"
#include "Vr_Asr_Impl_Event_Notify_Impl_Context_File_Size.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

C_Mgr_Data::~C_Mgr_Data()
{
    MACRO_Log_Function
}

C_Mgr_Data::C_Mgr_Data
(
    C_Engine_Concrete & ro_Engine_Concrete
)
: m_ro_Engine_Concrete(ro_Engine_Concrete)
, m_b_Stop(false)
, m_o_Mutex_Request()
, m_o_Mutex_Suspend()
, m_o_Mutex_Lock_Suspend()
, m_scpo_Lock_Mutex_Suspend()
, m_scpo_Thread_Data()
, m_map_priority_id_and_list_spo_Request_Data()
, m_spo_Request_Data_Curr()
, m_po_Data_Context_Curr(NULL)
, m_list_po_Data_Context()
, m_po_Data_CLC(NULL)
, m_o_Time_Data_Begin()
, m_o_Time_Data_End()
, m_i_Id_Request_Head(0)
, m_i_Id_Request_Tail(0)
, m_list_i_Id_Request_Cancel()
{
    MACRO_Log_Function
}

E_Result
C_Mgr_Data::M_Fn_Init()
{
    MACRO_Log_Function

    E_Result e_Result;

    m_b_Stop = false;

    e_Result = _M_Fn_Thread_Init();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Data::M_Fn_Fina()
{
    MACRO_Log_Function

    E_Result e_Result;

    m_b_Stop = true;

    e_Result = _M_Fn_Thread_Fina();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    return E_Result_Succeeded;
}

void
C_Mgr_Data::M_Fn_On_Thread_Notified()
{
    MACRO_Log_Function

    E_Result e_Result;

    while (true) {
        C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Suspend);

        {
            C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Request);

            map<unsigned int, list<shared_ptr<C_Request_Data> > >::iterator data_request_iter;
            data_request_iter = m_map_priority_id_and_list_spo_Request_Data.begin();
            IF (data_request_iter == m_map_priority_id_and_list_spo_Request_Data.end()) {
                break;
            }

            list<shared_ptr<C_Request_Data> >& ro_list_spo_Request_Data = data_request_iter->second;
            IF (ro_list_spo_Request_Data.empty()) {
                MACRO_Log_Error
                return;
            }

            m_spo_Request_Data_Curr = ro_list_spo_Request_Data.front();
            ro_list_spo_Request_Data.pop_front();

            IF (ro_list_spo_Request_Data.empty()) {
                m_map_priority_id_and_list_spo_Request_Data.erase(data_request_iter);
            }
        }

        IF (!m_spo_Request_Data_Curr) {

            MACRO_Log_Error
            return;
        }

        switch (m_spo_Request_Data_Curr->M_Fn_Get_Type()) {
        case C_Request_Data::E_Type_Context_List_Update_Begin: {

            e_Result = _M_Fn_Context_List_Update_Begin();
            break;
        }
        case C_Request_Data::E_Type_Context_List_Update_Clear: {

            e_Result = _M_Fn_Context_List_Update_Clear();
            break;
        }
        case C_Request_Data::E_Type_Context_List_Update_Delete: {

            e_Result = _M_Fn_Context_List_Update_Delete();
            break;
        }
        case C_Request_Data::E_Type_Context_List_Update_Add: {

            e_Result = _M_Fn_Context_List_Update_Add();
            break;
        }
        case C_Request_Data::E_Type_Context_List_Update_Commit: {

            e_Result = _M_Fn_Context_List_Update_Commit();
            break;
        }
        case C_Request_Data::E_Type_Context_List_File_Save: {

            e_Result = _M_Fn_Context_List_File_Save();
            break;
        }
        case C_Request_Data::E_Type_Context_List_File_Delete: {

            e_Result = _M_Fn_Context_List_File_Delete();
            break;
        }
        case C_Request_Data::E_Type_Transcription_Id_Version_Update: {

            e_Result = _M_Fn_Transcription_Id_Version_Update();
            break;
        }

#if 0
        case C_Request_Data::E_Type_Context_Fuzzy_Compile_Begin: {

            e_Result = _M_Fn_Context_Fuzzy_Compile_Begin();
            break;
        }
        case C_Request_Data::E_Type_Context_Fuzzy_Compile_Add_Column: {

            e_Result = _M_Fn_Context_Fuzzy_Compile_Add_Column();
            break;
        }
        case C_Request_Data::E_Type_Context_Fuzzy_Compile_Add_Entry: {

            e_Result = _M_Fn_Context_Fuzzy_Compile_Add_Entry();
            break;
        }
        case C_Request_Data::E_Type_Context_Fuzzy_Compile_Run: {

            e_Result = _M_Fn_Context_Fuzzy_Compile_Run();
            break;
        }
#endif
        default: {
            MACRO_Log_Error
            return;
        }
        }

        IF (e_Result != E_Result_Succeeded) {
            m_ro_Engine_Concrete.m_b_Error = true;
            MACRO_Log_Error
        }
    }
}

E_Result
C_Mgr_Data::M_Fn_Context_List_Update
(
    C_Request_Context_List_Update & ro_Request_Context_List_Update
)
{
    MACRO_Log_Function

    {
        C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Request);

        // Check for cancel.

        IF (ro_Request_Context_List_Update.m_b_Cancel) {
            IF (m_i_Id_Request_Head >= m_i_Id_Request_Tail) {
                IF ((ro_Request_Context_List_Update.m_i_Id_Request > m_i_Id_Request_Head)
                    || (ro_Request_Context_List_Update.m_i_Id_Request <= m_i_Id_Request_Tail)) {
                    return E_Result_Succeeded;
                }
            }
            ELSE {
                IF ((ro_Request_Context_List_Update.m_i_Id_Request > m_i_Id_Request_Head)
                    && (ro_Request_Context_List_Update.m_i_Id_Request <= m_i_Id_Request_Tail)) {
                    return E_Result_Succeeded;
                }
            }

            m_list_i_Id_Request_Cancel.push_back(ro_Request_Context_List_Update.m_i_Id_Request);

            return E_Result_Succeeded;
        }

        // Accept request.

        shared_ptr<C_Request_Context_List_Update> spo_Request_Context_List_Update(
            MACRO_New C_Request_Context_List_Update
        );

        IF (m_i_Id_Request_Head == static_cast<unsigned int>(-1)) {
            m_i_Id_Request_Head = 0;
        }
        ELSE {
            ++m_i_Id_Request_Head;
        }

        ro_Request_Context_List_Update.m_i_Id_Request = m_i_Id_Request_Head;

        C_Request_Context_List_Update * po_Request_Context_List_Update = spo_Request_Context_List_Update.get();

        po_Request_Context_List_Update->m_e_Requst_Type = ro_Request_Context_List_Update.m_e_Requst_Type;
        po_Request_Context_List_Update->m_i_Priority_Id = ro_Request_Context_List_Update.m_i_Priority_Id;
        po_Request_Context_List_Update->m_i_Id_Request = ro_Request_Context_List_Update.m_i_Id_Request;
        po_Request_Context_List_Update->m_function_On_Event_Phase = ro_Request_Context_List_Update.m_function_On_Event_Phase;
        po_Request_Context_List_Update->m_function_On_Event_Notify = ro_Request_Context_List_Update.m_function_On_Event_Notify;

        list<shared_ptr<C_Request_Data> > o_list_spo_Request_Data;
        switch (po_Request_Context_List_Update->m_e_Requst_Type) {
        case E_Data_Request_Type_Context_List_Update: {

            po_Request_Context_List_Update->m_string_Id_Party = ro_Request_Context_List_Update.m_string_Id_Party;
            po_Request_Context_List_Update->m_string_Id_Context = ro_Request_Context_List_Update.m_string_Id_Context;
            po_Request_Context_List_Update->m_string_Id_Category = ro_Request_Context_List_Update.m_string_Id_Category;
            po_Request_Context_List_Update->m_b_Clear = ro_Request_Context_List_Update.m_b_Clear;
            po_Request_Context_List_Update->m_b_Save_List_Context = ro_Request_Context_List_Update.m_b_Save_List_Context;

            // Begin.
            o_list_spo_Request_Data.push_back(
                shared_ptr<C_Request_Data>(
                    MACRO_New C_Request_Data_Context_List_Update_Begin(
                        po_Request_Context_List_Update
                    )
                )
            );

            // Clear.

            IF (ro_Request_Context_List_Update.m_b_Clear) {
                o_list_spo_Request_Data.push_back(
                    shared_ptr<C_Request_Data>(
                        MACRO_New C_Request_Data_Context_List_Update_Clear
                    )
                );
            }

            // Delete.

            while (!ro_Request_Context_List_Update.m_list_spo_Term_For_Delete.empty()) {
                o_list_spo_Request_Data.push_back(
                    shared_ptr<C_Request_Data>(
                        MACRO_New C_Request_Data_Context_List_Update_Delete(
                            ro_Request_Context_List_Update.m_list_spo_Term_For_Delete.front()
                        )
                    )
                );

                ro_Request_Context_List_Update.m_list_spo_Term_For_Delete.pop_front();
            }

            // Add.

            while (!ro_Request_Context_List_Update.m_list_spo_Term_For_Add.empty()) {
                o_list_spo_Request_Data.push_back(
                    shared_ptr<C_Request_Data>(
                        MACRO_New C_Request_Data_Context_List_Update_Add(
                            ro_Request_Context_List_Update.m_list_spo_Term_For_Add.front(),
                            po_Request_Context_List_Update
                        )
                    )
                );

                ro_Request_Context_List_Update.m_list_spo_Term_For_Add.pop_front();
            }

            // Commit.

            o_list_spo_Request_Data.push_back(
                shared_ptr<C_Request_Data>(
                    MACRO_New C_Request_Data_Context_List_Update_Commit(
                        spo_Request_Context_List_Update
                    )
                )
            );

            break;
        }
        case E_Data_Request_Type_Context_List_File_Save: {

            po_Request_Context_List_Update->m_string_Id_Party = ro_Request_Context_List_Update.m_string_Id_Party;
            po_Request_Context_List_Update->m_string_Id_Context = ro_Request_Context_List_Update.m_string_Id_Context;

            o_list_spo_Request_Data.push_back(
                shared_ptr<C_Request_Data>(
                    MACRO_New C_Request_Data_Context_List_File_Save(
                        spo_Request_Context_List_Update
                    )
                )
            );

            break;
        }
        case E_Data_Request_Type_Context_List_File_Delete: {

            po_Request_Context_List_Update->m_string_Id_Party = ro_Request_Context_List_Update.m_string_Id_Party;
            po_Request_Context_List_Update->m_string_Id_Context = ro_Request_Context_List_Update.m_string_Id_Context;

            o_list_spo_Request_Data.push_back(
                shared_ptr<C_Request_Data>(
                    MACRO_New C_Request_Data_Context_List_File_Delete(
                        spo_Request_Context_List_Update
                    )
                )
            );

            break;
        }
        case E_Data_Request_Type_Transcription_Id_Version_Update: {

            po_Request_Context_List_Update->m_string_Transcription_Id_Version = ro_Request_Context_List_Update.m_string_Transcription_Id_Version;
            o_list_spo_Request_Data.push_back(
                shared_ptr<C_Request_Data>(
                    MACRO_New C_Request_Data_Transcription_Id_Version_Update(
                        spo_Request_Context_List_Update
                    )
                )
            );

            break;
        }
        default: {
            MACRO_Log_Error
            return E_Result_Error;
        }
        }

        unsigned int i_Priority_Id = po_Request_Context_List_Update->m_i_Priority_Id;
        IF (0 == m_map_priority_id_and_list_spo_Request_Data.count(i_Priority_Id)) {
            m_map_priority_id_and_list_spo_Request_Data[i_Priority_Id]; // create new priority list context
        }

        list<shared_ptr<C_Request_Data> >& ro_list_spo_Request_Data = m_map_priority_id_and_list_spo_Request_Data[i_Priority_Id];
        ro_list_spo_Request_Data.insert(
            ro_list_spo_Request_Data.end(),
            o_list_spo_Request_Data.begin(),
            o_list_spo_Request_Data.end());
    }

    m_scpo_Thread_Data->Notify();

    return E_Result_Succeeded;
}

#if 0

E_Result
C_Mgr_Data::M_Fn_Context_Fuzzy_Compile
(
    C_Request_Context_Fuzzy_Compile & ro_Request_Context_Fuzzy_Compile
)
{
    MACRO_Log_Function

    vector<boost::shared_ptr<C_Term> > vector_spo_Term;
    vector<boost::shared_ptr<C_Term> >::size_type vector_spo_Term_Size = 0;

    {
        C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Request);

        // Accept request.

        shared_ptr<C_Request_Context_Fuzzy_Compile> spo_Request_Context_Fuzzy_Compile(
            MACRO_New C_Request_Context_Fuzzy_Compile
        );

        C_Request_Context_Fuzzy_Compile * po_Request_Context_Fuzzy_Compile = spo_Request_Context_Fuzzy_Compile.get();

        po_Request_Context_Fuzzy_Compile->m_string_Id_Party = ro_Request_Context_Fuzzy_Compile.m_string_Id_Party;
        po_Request_Context_Fuzzy_Compile->m_string_Id_Context = ro_Request_Context_Fuzzy_Compile.m_string_Id_Context;
        po_Request_Context_Fuzzy_Compile->m_function_On_Event_Phase = ro_Request_Context_Fuzzy_Compile.m_function_On_Event_Phase;
        po_Request_Context_Fuzzy_Compile->m_function_On_Event_Notify = ro_Request_Context_Fuzzy_Compile.m_function_On_Event_Notify;

        // Begin.
        m_list_spo_Request_Data.push_back(
            shared_ptr<C_Request_Data>(
                MACRO_New C_Request_Data_Context_Fuzzy_Compile_Begin(
                    po_Request_Context_Fuzzy_Compile
                )
            )
        );

        // Add Column.
        vector_spo_Term.clear();
        vector_spo_Term_Size = ro_Request_Context_Fuzzy_Compile.m_vector_spo_Term_Column.size();
        IF (0 == vector_spo_Term_Size) {
            m_list_spo_Request_Data.clear();
            MACRO_Log_Error            
            return E_Result_Error;
        }
        
        vector_spo_Term.assign(
            ro_Request_Context_Fuzzy_Compile.m_vector_spo_Term_Column.begin(),
            ro_Request_Context_Fuzzy_Compile.m_vector_spo_Term_Column.end());
        m_list_spo_Request_Data.push_back(
            shared_ptr<C_Request_Data>(
                MACRO_New C_Request_Data_Context_Fuzzy_Compile_Add_Column(
                    vector_spo_Term
                    )
                )
        );
        ro_Request_Context_Fuzzy_Compile.m_vector_spo_Term_Column.clear();

        // Add Entry
        while (!ro_Request_Context_Fuzzy_Compile.m_list_vector_spo_Term_Entry.empty()) {
            vector<boost::shared_ptr<C_Term> >& ro_vector_spo_Term = 
                ro_Request_Context_Fuzzy_Compile.m_list_vector_spo_Term_Entry.front();

            IF (vector_spo_Term_Size != ro_vector_spo_Term.size()) {
                m_list_spo_Request_Data.clear();
                MACRO_Log_Error            
                return E_Result_Error;
            }
                
            vector_spo_Term.clear();
            vector_spo_Term.assign(ro_vector_spo_Term.begin(), ro_vector_spo_Term.end());
            m_list_spo_Request_Data.push_back(
                shared_ptr<C_Request_Data>(
                    MACRO_New C_Request_Data_Context_Fuzzy_Compile_Add_Entry(
                        vector_spo_Term
                    )
                )
            );

            ro_Request_Context_Fuzzy_Compile.m_list_vector_spo_Term_Entry.pop_front();
        }

        // Run.

        m_list_spo_Request_Data.push_back(
            shared_ptr<C_Request_Data>(
                MACRO_New C_Request_Data_Context_Fuzzy_Compile_Run(
                    spo_Request_Context_Fuzzy_Compile
                )
            )
        );
    }
    
    m_scpo_Thread_Data->Notify();

    return E_Result_Succeeded;
}

#endif

E_Result
C_Mgr_Data::M_Fn_Suspend()
{
    MACRO_Log_Function

    C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Lock_Suspend);

    IF (!m_scpo_Lock_Mutex_Suspend) {
        m_scpo_Lock_Mutex_Suspend.reset(
            MACRO_New C_Lock_Mutex(m_o_Mutex_Suspend)
        );
    }

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Data::M_Fn_Resume()
{
    MACRO_Log_Function

    C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Lock_Suspend);

    IF ((bool)m_scpo_Lock_Mutex_Suspend) {
        m_scpo_Lock_Mutex_Suspend.reset();
    }

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Data::_M_Fn_Thread_Init()
{
    MACRO_Log_Function

    IF ((bool)m_scpo_Thread_Data) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_scpo_Thread_Data.reset(MACRO_New C_Thread_Data(*this));

#if defined(_WIN32) || defined(_WIN32_WCE)
#else
    m_scpo_Thread_Data->RegisterName("VR_ASR_DATA_THREAD");
#endif

    m_scpo_Thread_Data->StartRegistThread();

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Data::_M_Fn_Thread_Fina()
{
    MACRO_Log_Function

    IF ((bool)m_scpo_Thread_Data) {
        m_scpo_Thread_Data->StopThread();
        m_scpo_Thread_Data.reset();
    }

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Data::_M_Fn_Context_List_Update_Begin()
{
    MACRO_Log_Function

    E_Result e_Result;

    string string_Id_Party = m_spo_Request_Data_Curr->M_Fn_Get_string_Id_Party();
    Fn_Log("Context Request Party Id: " + string_Id_Party);

    C_Data_Party * po_Data_Party_Curr = NULL;
    po_Data_Party_Curr = m_ro_Engine_Concrete.M_Fn_Find_po_Data_Party(string_Id_Party);
    IF (NULL == po_Data_Party_Curr) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    string string_Id_Context = m_spo_Request_Data_Curr->M_Fn_Get_string_Id_Context();
    Fn_Log("Context Request Context Id: " + string_Id_Context);

    C_Data_Context* po_Data_Context_Curr = NULL;
    po_Data_Context_Curr = po_Data_Party_Curr->M_Fn_Find_po_Data_Context(string_Id_Context);
    IF (NULL == po_Data_Context_Curr) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_list_po_Data_Context.push_back(m_po_Data_Context_Curr);
    m_po_Data_Context_Curr = po_Data_Context_Curr;

    C_Request_Context_List_Update * po_Request_Context_List_Update = m_spo_Request_Data_Curr->M_Fn_Get_po_Request_Context_List_Update();

    IF (!po_Request_Context_List_Update) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    string string_Id_Category_Prev;
    string string_Id_Category_Curr = m_spo_Request_Data_Curr->M_Fn_Get_string_Id_Category();
    Fn_Log("Context Request Category Id: " + string_Id_Category_Curr);

    m_po_Data_CLC = m_ro_Engine_Concrete.M_Fn_Prepare_po_Data_CLC(string_Id_Category_Curr, string_Id_Category_Prev);
    IF (NULL == m_po_Data_CLC) {
        MACRO_Log_Error
        return E_Result_Error;
    }
    po_Request_Context_List_Update->m_string_Id_Category = string_Id_Category_Prev;

    e_Result = m_po_Data_Context_Curr->M_Fn_File_Delete();
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    e_Result = m_po_Data_Context_Curr->M_Fn_List_Update_Begin();
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    po_Request_Context_List_Update->m_function_On_Event_Phase(
        C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Proc_Begin)
    );

    m_o_Time_Data_Begin = C_Time::M_Fn_Get_Time();

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Data::_M_Fn_Context_List_Update_Clear()
{
    IF (!m_po_Data_Context_Curr) {
        return E_Result_Failed;
    }

    return m_po_Data_Context_Curr->M_Fn_List_Update_Clear();
}

E_Result
C_Mgr_Data::_M_Fn_Context_List_Update_Delete()
{
    IF (m_b_Stop) {
        return E_Result_Succeeded;
    }

    IF (!m_po_Data_Context_Curr) {
        return E_Result_Failed;
    }

    C_Term * po_Term = m_spo_Request_Data_Curr->M_Fn_Get_po_Term();

    IF (!po_Term) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return m_po_Data_Context_Curr->M_Fn_List_Update_Delete(*po_Term);
}

E_Result
C_Mgr_Data::_M_Fn_Context_List_Update_Add()
{
    IF (m_b_Stop) {
        return E_Result_Succeeded;
    }

    C_Request_Context_List_Update * po_Request_Context_List_Update = m_spo_Request_Data_Curr->M_Fn_Get_po_Request_Context_List_Update();

    IF (!po_Request_Context_List_Update) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (po_Request_Context_List_Update->m_b_Cancel) {
        return E_Result_Succeeded;
    }

    {
        C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Request);

        for (
            list<unsigned int>::iterator iterator_i_Id_Request_Cancel_Curr = m_list_i_Id_Request_Cancel.begin();
            iterator_i_Id_Request_Cancel_Curr != m_list_i_Id_Request_Cancel.end();
            ++iterator_i_Id_Request_Cancel_Curr
        ) {
            IF (po_Request_Context_List_Update->m_i_Id_Request == *iterator_i_Id_Request_Cancel_Curr) {
                po_Request_Context_List_Update->m_b_Cancel = true;
                m_list_i_Id_Request_Cancel.erase(iterator_i_Id_Request_Cancel_Curr);
                break;
            }
        }
    }

    IF (!m_po_Data_Context_Curr) {
        return E_Result_Failed;
    }

    IF (po_Request_Context_List_Update->m_b_Cancel)
    {
        return m_po_Data_Context_Curr->M_Fn_List_Update_Clear();
    }

    C_Term * po_Term = m_spo_Request_Data_Curr->M_Fn_Get_po_Term();

    IF (!po_Term) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (NULL == m_po_Data_CLC) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return m_po_Data_Context_Curr->M_Fn_List_Update_Add(*po_Term, *m_po_Data_CLC);
}

E_Result
C_Mgr_Data::_M_Fn_Context_List_Update_Commit()
{
    MACRO_Log_Function

    E_Result e_Result;

    C_Request_Context_List_Update * po_Request_Context_List_Update = NULL;
    po_Request_Context_List_Update = m_spo_Request_Data_Curr->M_Fn_Get_po_Request_Context_List_Update();
    IF (NULL == po_Request_Context_List_Update) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    C_Data_Context* po_Data_Context_Curr = m_po_Data_Context_Curr;
    IF (NULL == po_Data_Context_Curr) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    IF (m_list_po_Data_Context.empty()) {
        MACRO_Log_Error
        return E_Result_Failed;
    }
    m_po_Data_Context_Curr = m_list_po_Data_Context.front();
    m_list_po_Data_Context.pop_front();

    do {
        e_Result = po_Data_Context_Curr->M_Fn_List_Update_Commit();
        IF (E_Result_Succeeded != e_Result) {
            MACRO_Log_Error
            break;
        }

        unsigned int i_File_Size = 0;
        e_Result = po_Data_Context_Curr->M_Fn_File_Save(
            po_Request_Context_List_Update->m_b_Save_List_Context,
            i_File_Size);
        IF (E_Result_Succeeded != e_Result) {
            MACRO_Log_Error
            break;
        }

        po_Request_Context_List_Update->m_function_On_Event_Notify(
            C_Event_Notify_Impl_Context_File_Size(i_File_Size)
        );

    } while (0);

    IF (m_b_Stop || E_Result_Succeeded != e_Result) {
        po_Request_Context_List_Update->m_function_On_Event_Phase(
            C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Proc_Fail)
        );
    }
    ELSE {
        po_Request_Context_List_Update->m_function_On_Event_Phase(
            C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Proc_End)
        );
    }

    {
        C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Request);

        m_i_Id_Request_Tail = po_Request_Context_List_Update->m_i_Id_Request;
    }

    m_o_Time_Data_End = C_Time::M_Fn_Get_Time();

    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Log("Data Begin\t");
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Time(m_o_Time_Data_Begin);
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Enter();

    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Log("Data End\t");
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Time(m_o_Time_Data_End);
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Enter();

    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Log("Data Cost\t");
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Time(m_o_Time_Data_End - m_o_Time_Data_Begin);
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Enter();

    string string_Id_Category_Prev;
    m_po_Data_CLC = m_ro_Engine_Concrete.M_Fn_Prepare_po_Data_CLC(
        m_spo_Request_Data_Curr->M_Fn_Get_string_Id_Category(),
        string_Id_Category_Prev);
    IF (NULL == m_po_Data_CLC) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Data::_M_Fn_Context_List_File_Save()
{
    MACRO_Log_Function

    E_Result e_Result;

    C_Request_Context_List_Update * po_Request_Context_List_Update = NULL;
    po_Request_Context_List_Update = m_spo_Request_Data_Curr->M_Fn_Get_po_Request_Context_List_Update();
    IF (NULL == po_Request_Context_List_Update) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    do {

        string string_Id_Party = m_spo_Request_Data_Curr->M_Fn_Get_string_Id_Party();
        Fn_Log("Context Save Request Party Id: " + string_Id_Party);

        C_Data_Party * po_Data_Party_Curr = NULL;
        po_Data_Party_Curr = m_ro_Engine_Concrete.M_Fn_Find_po_Data_Party(string_Id_Party);
        IF (NULL == po_Data_Party_Curr) {
            MACRO_Log_Error
            break;
        }

        string string_Id_Context = m_spo_Request_Data_Curr->M_Fn_Get_string_Id_Context();
        Fn_Log("Context Save Request Context Id: " + string_Id_Context);

        C_Data_Context * po_Data_Context_Curr = NULL;
        po_Data_Context_Curr = po_Data_Party_Curr->M_Fn_Find_po_Data_Context(string_Id_Context);
        IF (NULL == po_Data_Context_Curr) {
            MACRO_Log_Error
            break;
        }

        unsigned int i_File_Size = 0;
        e_Result = po_Data_Context_Curr->M_Fn_File_Save(true, i_File_Size);
        IF (E_Result_Succeeded != e_Result) {
            MACRO_Log_Error
            break;
        }

        po_Request_Context_List_Update->m_function_On_Event_Phase(
            C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Proc_End)
        );

        return E_Result_Succeeded;

    } while (0);

    po_Request_Context_List_Update->m_function_On_Event_Notify(
       C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Context_List_File_Save_Failed)
    );

    po_Request_Context_List_Update->m_function_On_Event_Phase(
        C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Proc_End)
    );

    return E_Result_Failed;
}

E_Result
C_Mgr_Data::_M_Fn_Context_List_File_Delete()
{
    MACRO_Log_Function

    E_Result e_Result;

    C_Request_Context_List_Update * po_Request_Context_List_Update = NULL;
    po_Request_Context_List_Update = m_spo_Request_Data_Curr->M_Fn_Get_po_Request_Context_List_Update();
    IF (NULL == po_Request_Context_List_Update) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    do {

        string string_Id_Party = m_spo_Request_Data_Curr->M_Fn_Get_string_Id_Party();
        Fn_Log("Context Save Request Party Id: " + string_Id_Party);

        C_Data_Party * po_Data_Party_Curr = NULL;
        po_Data_Party_Curr = m_ro_Engine_Concrete.M_Fn_Find_po_Data_Party(string_Id_Party);
        IF (NULL == po_Data_Party_Curr) {
            MACRO_Log_Error
            break;
        }

        string string_Id_Context = m_spo_Request_Data_Curr->M_Fn_Get_string_Id_Context();
        Fn_Log("Context Save Request Context Id: " + string_Id_Context);

        C_Data_Context * po_Data_Context_Curr = NULL;
        po_Data_Context_Curr = po_Data_Party_Curr->M_Fn_Find_po_Data_Context(string_Id_Context);
        IF (NULL == po_Data_Context_Curr) {
            MACRO_Log_Error
            break;
        }

        e_Result = po_Data_Context_Curr->M_Fn_File_Delete();
        IF (E_Result_Succeeded != e_Result) {
            MACRO_Log_Error
            break;
        }

        po_Request_Context_List_Update->m_function_On_Event_Phase(
            C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Proc_End)
        );

        return E_Result_Succeeded;

    } while (0);

    po_Request_Context_List_Update->m_function_On_Event_Notify(
       C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Context_List_File_Delete_Failed)
    );

    po_Request_Context_List_Update->m_function_On_Event_Phase(
        C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Proc_End)
    );

    return E_Result_Failed;
}

E_Result
C_Mgr_Data::_M_Fn_Transcription_Id_Version_Update()
{
    MACRO_Log_Function

    E_Result e_Result;

    C_Request_Context_List_Update * po_Request_Context_List_Update = NULL;
    po_Request_Context_List_Update = m_spo_Request_Data_Curr->M_Fn_Get_po_Request_Context_List_Update();
    IF (!po_Request_Context_List_Update) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    int b_Transcription_Id_Version_Supported = false;
    e_Result = m_ro_Engine_Concrete.M_Fn_Data_CLC_Transcription_Id_Version_Update(
        po_Request_Context_List_Update->m_string_Transcription_Id_Version,
        b_Transcription_Id_Version_Supported);
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error

        po_Request_Context_List_Update->m_function_On_Event_Notify(
           C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Transcription_Id_Not_Support)
        );

        return E_Result_Error;
    }

    IF (b_Transcription_Id_Version_Supported) {
        po_Request_Context_List_Update->m_function_On_Event_Notify(
           C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Transcription_Id_Support)
        );
    }
    ELSE {
        po_Request_Context_List_Update->m_function_On_Event_Notify(
           C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Transcription_Id_Not_Support)
        );
    }

    return E_Result_Succeeded;
}

#if 0

E_Result
C_Mgr_Data::_M_Fn_Context_Fuzzy_Compile_Begin()
{
    MACRO_Log_Function

    E_Result e_Result;

    m_po_Data_Party_Curr = m_ro_Engine_Concrete.M_Fn_Find_po_Data_Party(
        m_spo_Request_Data_Curr->M_Fn_Get_string_Id_Party()
    );

    IF (!m_po_Data_Party_Curr) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_po_Data_Context_Curr = m_po_Data_Party_Curr->M_Fn_Find_po_Data_Context(
        m_spo_Request_Data_Curr->M_Fn_Get_string_Id_Context()
    );

    IF (!m_po_Data_Context_Curr) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    e_Result = m_po_Data_Context_Curr->M_Fn_Fuzzy_Compile_Begin();
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    C_Request_Context_Fuzzy_Compile * po_Request_Context_Fuzzy_Compile = m_spo_Request_Data_Curr->M_Fn_Get_po_Request_Context_Fuzzy_Compile();

    IF (!po_Request_Context_Fuzzy_Compile) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    po_Request_Context_Fuzzy_Compile->m_function_On_Event_Phase(
        C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Proc_Begin)
    );

    m_o_Time_Data_Begin = C_Time::M_Fn_Get_Time();

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Data::_M_Fn_Context_Fuzzy_Compile_Add_Column()
{
    vector<shared_ptr<C_Term> >* po_vector_spo_Term = m_spo_Request_Data_Curr->M_Fn_Get_Vector_po_Term();
    IF (NULL == po_vector_spo_Term) {
        MACRO_Log_Error
        return E_Result_Error;
    }
    
    return m_po_Data_Context_Curr->M_Fn_Fuzzy_Compile_Add_Column(*po_vector_spo_Term);
}

E_Result
C_Mgr_Data::_M_Fn_Context_Fuzzy_Compile_Add_Entry()
{
    vector<shared_ptr<C_Term> >* po_vector_spo_Term = m_spo_Request_Data_Curr->M_Fn_Get_Vector_po_Term();
    IF (NULL == po_vector_spo_Term) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return m_po_Data_Context_Curr->M_Fn_Fuzzy_Compile_Add_Entry(*po_vector_spo_Term);
}

E_Result
C_Mgr_Data::_M_Fn_Context_Fuzzy_Compile_Run()
{
    MACRO_Log_Function

    E_Result e_Result;
    e_Result = m_po_Data_Context_Curr->M_Fn_Fuzzy_Compile_Run();
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    C_Request_Context_Fuzzy_Compile * po_Request_Context_Fuzzy_Compile = m_spo_Request_Data_Curr->M_Fn_Get_po_Request_Context_Fuzzy_Compile();

    IF (!po_Request_Context_Fuzzy_Compile) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    po_Request_Context_Fuzzy_Compile->m_function_On_Event_Phase(
        C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Proc_End)
    );

    m_o_Time_Data_End = C_Time::M_Fn_Get_Time();

    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Log("Compile Begin\t");
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Time(m_o_Time_Data_Begin);
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Enter();

    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Log("Compile End\t");
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Time(m_o_Time_Data_End);
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Enter();

    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Log("Compile Cost\t");
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Time(m_o_Time_Data_End - m_o_Time_Data_Begin);
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Enter();
    
    return E_Result_Succeeded;
}

#endif

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
