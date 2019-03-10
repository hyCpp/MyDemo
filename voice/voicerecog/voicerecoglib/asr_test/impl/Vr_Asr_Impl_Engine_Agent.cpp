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
#include "Vr_Asr_Impl_Engine_Agent.h"
#include "Vr_Asr_Impl_Fn_Get_string_Path.h"

#ifdef _NUANCE
#include "Vr_Asr_Impl_Vocon_Engine_Concrete.h"
#include "Vr_Asr_Impl_Vocon_Via_Shared_Engine_Concrete.h"
#endif

#ifdef _IFLYTEK
#include "Vr_Asr_Impl_IFlyTek_Engine_Concrete.h"
#endif

#ifdef _KALDI
#include "Vr_Asr_Impl_Kaldi_Engine_Concrete.h"
#endif


namespace N_Vr
{

namespace N_Asr
{

// iflytek language
string const g_string_Iss_Sr_Acous_Lang_Mandarin = "mandarin";
string const g_string_Iss_Sr_Acous_Lang_English = "english";
string const g_string_Iss_Sr_Acous_Lang_Cantonese = "cantonese";

// iflytek mode
string const g_string_Iss_Sr_Mode_Cloud = "cloud";
string const g_string_Iss_Sr_Mode_Local = "local";
string const g_string_Iss_Sr_Mode_Mix = "mix";

static bool sg_b_Engine_Exist = false;

namespace N_Impl
{

C_Engine_Agent::~C_Engine_Agent()
{
    MACRO_Log_Function

    C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Interface);

    _M_Fn_Language_Fina();

    sg_b_Engine_Exist = false;
}

C_Engine_Agent::C_Engine_Agent()
: m_e_Type_Engine(E_Type_Engine_None)
, m_set_string_Id_Party()
, m_function_On_Event_Init_Notify()
, m_scpo_Engine_Base()
, m_po_Vocon_LH_COMPONENT_Asr(NULL)
, m_po_Vocon_LH_OBJECT_Mod_Ac(NULL)
, m_o_Mutex_Interface()
{
    MACRO_Log_Function

    C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Interface);
}

E_Result
C_Engine_Agent::Language_Change
(
    string const & rstring_Id_Language,
    bool b_User_Data_Del
)
{
    MACRO_Log_Function

    C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Interface);

    E_Result e_Result;

    e_Result = _M_Fn_Language_Fina();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    if (b_User_Data_Del) {
        e_Result = _M_Fn_Clear_User_Data();
        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }
    }

    return _M_Fn_Language_Init(rstring_Id_Language);
}

E_Result
C_Engine_Agent::Language_Change
(
    C_Request_Language_Change const & ro_Request_Language_Change,
    bool b_User_Data_Del
)
{
    MACRO_Log_Function

    C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Interface);

    E_Result e_Result;

    e_Result = _M_Fn_Language_Fina();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    if (b_User_Data_Del) {
        e_Result = _M_Fn_Clear_User_Data();
        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }
    }

    // m_e_Type_Engine = ro_Request_Language_Change.m_e_Type_Engine;
    m_po_Vocon_LH_COMPONENT_Asr = ro_Request_Language_Change.m_po_Vocon_LH_COMPONENT_Asr;
    m_po_Vocon_LH_OBJECT_Mod_Ac = ro_Request_Language_Change.m_po_Vocon_LH_OBJECT_Mod_Ac;
    m_function_On_Event_Init_Notify = ro_Request_Language_Change.m_function_On_Event_Init_Notify;

    return _M_Fn_Language_Init(ro_Request_Language_Change.m_string_Id_Language);
}

E_Result
C_Engine_Agent::Party_Load
(
    string const & rstring_Id_Party
)
{
    MACRO_Log_Function

    C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Interface);

    E_Result e_Result;

    set<string>::iterator iterator_string_Id_Party_End = m_set_string_Id_Party.end();

    set<string>::iterator iterator_string_Id_Party_Find = m_set_string_Id_Party.find(rstring_Id_Party);
    IF (iterator_string_Id_Party_Find != m_set_string_Id_Party.end()) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    m_set_string_Id_Party.insert(rstring_Id_Party);

    IF ((bool)m_scpo_Engine_Base) {
        e_Result = m_scpo_Engine_Base->M_Fn_Party_Load(rstring_Id_Party);
        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Agent::Party_Unload
(
    string const & rstring_Id_Party
)
{
#if 0
    MACRO_Log_Function

    C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Interface);

    E_Result e_Result;

    IF (m_scpo_Engine_Base) {
        e_Result = m_scpo_Engine_Base->M_Fn_Party_Unload(rstring_Id_Party);
        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }
    }

    set<string>::iterator iterator_string_Id_Party_End = m_set_string_Id_Party.end();

    set<string>::iterator iterator_string_Id_Party_Find = m_set_string_Id_Party.find(rstring_Id_Party);
    IF (iterator_string_Id_Party_Find == m_set_string_Id_Party.end()) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    m_set_string_Id_Party.erase(rstring_Id_Party);
#endif
    return E_Result_Succeeded;
}

E_Result
C_Engine_Agent::Param_Set
(
    C_Request_Param_Set & ro_Request_Param_Set
)
{
    MACRO_Log_Function

    C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Interface);

    IF (!m_scpo_Engine_Base) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    return m_scpo_Engine_Base->M_Fn_Param_Set(ro_Request_Param_Set);
}

E_Result
C_Engine_Agent::Context_List_Update_Sync
(
    C_Request_Context_List_Update & ro_Request_Context_List_Update
)
{
    MACRO_Log_Function

    C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Interface);

    IF ((!m_scpo_Engine_Base)
        || (!ro_Request_Context_List_Update.m_function_On_Event_Phase)
        || (!ro_Request_Context_List_Update.m_function_On_Event_Notify)
    ) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    m_function_On_Event_Phase = ro_Request_Context_List_Update.m_function_On_Event_Phase;

    ro_Request_Context_List_Update.m_function_On_Event_Phase = bind(
        &C_Engine_Agent::_M_Fn_On_Event_Phase,
        this,
        _1
    );
    
    m_o_Semaphore.Reset();

    E_Result e_Result = m_scpo_Engine_Base->M_Fn_Data_Context_List_Update(ro_Request_Context_List_Update);
    IF (e_Result != E_Result_Succeeded) {

        ro_Request_Context_List_Update.m_function_On_Event_Phase = m_function_On_Event_Phase;

        return e_Result;
    }

    m_o_Semaphore.Wait();

    ro_Request_Context_List_Update.m_function_On_Event_Phase = m_function_On_Event_Phase;

    return E_Result_Succeeded;
}

E_Result
C_Engine_Agent::Context_List_Update
(
    C_Request_Context_List_Update & ro_Request_Context_List_Update
)
{
#ifdef MACRO_Dynamic_Debug
    MACRO_Log_Function

    C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Interface);

    IF ((!m_scpo_Engine_Base)
        || (!ro_Request_Context_List_Update.m_function_On_Event_Phase)
        || (!ro_Request_Context_List_Update.m_function_On_Event_Notify)
    ) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    E_Result e_Result = m_scpo_Engine_Base->M_Fn_Data_Context_List_Update(ro_Request_Context_List_Update);
    IF (e_Result != E_Result_Succeeded) {
        return e_Result;
    }
#endif
    return E_Result_Succeeded;
}


#if 0

E_Result
C_Engine_Agent::Context_Fuzzy_Compile
(
    C_Request_Context_Fuzzy_Compile & ro_Request_Context_Fuzzy_Compile
)
{
    MACRO_Log_Function

    C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Interface);

    IF ((!m_scpo_Engine_Base)
        || (!ro_Request_Context_Fuzzy_Compile.m_function_On_Event_Phase)
        || (!ro_Request_Context_Fuzzy_Compile.m_function_On_Event_Notify)
    ) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    return m_scpo_Engine_Base->M_Fn_Data_Context_Fuzzy_Compile(ro_Request_Context_Fuzzy_Compile);
}

#endif

E_Result
C_Engine_Agent::Recognize_Start_Sync
(
    C_Request_Recognize & ro_Request_Recognize
)
{
    MACRO_Log_Function

    C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Interface);

    IF ((!m_scpo_Engine_Base)
        || (!ro_Request_Recognize.m_spo_Audio_In)
        || (!ro_Request_Recognize.m_function_On_Event_Phase)
        || (!ro_Request_Recognize.m_function_On_Event_Notify)
        || (!ro_Request_Recognize.m_function_On_Event_Result)
    ) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    m_function_On_Event_Phase = ro_Request_Recognize.m_function_On_Event_Phase;

    ro_Request_Recognize.m_function_On_Event_Phase = bind(
        &C_Engine_Agent::_M_Fn_On_Event_Phase,
        this,
        _1
    );

    m_o_Semaphore.Reset();

    E_Result e_Result = m_scpo_Engine_Base->M_Fn_Rec_Start(ro_Request_Recognize);
    IF (e_Result != E_Result_Succeeded) {

        ro_Request_Recognize.m_function_On_Event_Phase = m_function_On_Event_Phase;

        return e_Result;
    }
    
    m_o_Semaphore.Wait();

    ro_Request_Recognize.m_function_On_Event_Phase = m_function_On_Event_Phase;

    return E_Result_Succeeded;
}

E_Result
C_Engine_Agent::Recognize_Start
(
    C_Request_Recognize & ro_Request_Recognize
)
{
#ifdef MACRO_Dynamic_Debug
    MACRO_Log_Function

    C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Interface);

    IF ((!m_scpo_Engine_Base)
        || (!ro_Request_Recognize.m_spo_Audio_In)
        || (!ro_Request_Recognize.m_function_On_Event_Phase)
        || (!ro_Request_Recognize.m_function_On_Event_Notify)
        || (!ro_Request_Recognize.m_function_On_Event_Result)
    ) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    E_Result e_Result = m_scpo_Engine_Base->M_Fn_Rec_Start(ro_Request_Recognize);
    IF (e_Result != E_Result_Succeeded) {
        return e_Result;
    }
    
#endif

    return E_Result_Succeeded;
}

E_Result
C_Engine_Agent::Recognize_Cancel()
{
    MACRO_Log_Function

    C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Interface);

    IF (!m_scpo_Engine_Base) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    return m_scpo_Engine_Base->M_Fn_Rec_Cancel();
}

E_Result
C_Engine_Agent::Get_Transcriptions
(
    C_Term& ro_Term
)
{
    MACRO_Log_Function

    C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Interface);

    IF (!m_scpo_Engine_Base) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    return m_scpo_Engine_Base->M_Fn_Get_Transcriptions(ro_Term);
}

E_Result
C_Engine_Agent::_M_Fn_Language_Init
(
    string const & rstring_Id_Language
)
{
    MACRO_Log_Function

    IF ((bool)m_scpo_Engine_Base) {
        return E_Result_Failed;
    }

    switch (m_e_Type_Engine) {
#ifdef _NUANCE
    case E_Type_Engine_Vocon:
        {
            m_scpo_Engine_Base.reset(
                MACRO_New N_Vocon::C_Engine_Concrete(
                    *this,
                    rstring_Id_Language,
                    m_function_On_Event_Init_Notify
                )
            );

            break;
        }
    case E_Type_Engine_Vocon_Via_Shared:
        {
            m_scpo_Engine_Base.reset(
                MACRO_New N_Vocon::N_Via_Shared::C_Engine_Concrete(
                    *this,
                    rstring_Id_Language,
                    m_po_Vocon_LH_COMPONENT_Asr,
                    m_po_Vocon_LH_OBJECT_Mod_Ac,
                    m_function_On_Event_Init_Notify
                )
            );

            break;
        }
#endif
    case E_Type_Engine_iFlytek:
    {
#ifdef _IFLYTEK
            m_scpo_Engine_Base.reset(
                MACRO_New N_IFlyTek::C_Engine_Concrete(
                *this,
                rstring_Id_Language,
                m_function_On_Event_Init_Notify,
                m_string_Center_Vr_Url
                )
            );
#endif
            break;
    }
    case E_Type_Engine_Kaldi:
    {
#ifdef _KALDI
            m_scpo_Engine_Base.reset(
                MACRO_New N_Kaldi::C_Engine_Concrete(
                *this,
                rstring_Id_Language,
                m_function_On_Event_Init_Notify
                )
            );
#endif
            break;
    }
    default:
        {
            MACRO_Log_Error
            return E_Result_Failed;
        }
    }

    E_Result e_Result = m_scpo_Engine_Base->M_Fn_Init();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Agent::_M_Fn_Language_Fina()
{
    MACRO_Log_Function

    IF (!m_scpo_Engine_Base) {
        return E_Result_Succeeded;
    }

    E_Result e_Result = m_scpo_Engine_Base->M_Fn_Fina();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    m_scpo_Engine_Base.reset();

    return E_Result_Succeeded;
}

void
C_Engine_Agent::_M_Fn_On_Event_Phase
(
    C_Event_Phase const & ro_Event_Phase
)
{
    IF ((ro_Event_Phase.Get_Event_Phase_Type() == C_Event_Phase::E_Event_Phase_Type_Proc_End)
        || (ro_Event_Phase.Get_Event_Phase_Type() == C_Event_Phase::E_Event_Phase_Type_Proc_Fail)) {

        m_o_Semaphore.Notify();
    }

    m_function_On_Event_Phase(ro_Event_Phase);
}

E_Result
C_Engine_Agent::_M_Fn_Clear_User_Data()
{
    MACRO_Log_Function

    C_Dir o_Dir;
    string string_User_Path;

    string_User_Path = G_o_Get_Path().M_Fn_Get_string_Path_Usr();
    Fn_Log(string_User_Path);
    CL_BOOL o_BOOL = o_Dir.RemoveDir(string_User_Path.c_str(), CL_FALSE);
    IF (CL_TRUE != o_BOOL) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    return E_Result_Succeeded;
}

} // name N_Impl

C_Engine::~C_Engine()
{
}

C_Engine::C_Engine()
{
}

C_Engine *
C_Engine::Factory
(
    C_Request_Factory & ro_Request_Factory
)
{
    E_Result e_Result;

    N_Impl::C_Engine_Agent * apo_Engine_Agent = MACRO_New N_Impl::C_Engine_Agent();

    apo_Engine_Agent->m_e_Type_Engine = ro_Request_Factory.m_e_Type_Engine;
    apo_Engine_Agent->m_po_Vocon_LH_COMPONENT_Asr = ro_Request_Factory.m_po_Vocon_LH_COMPONENT_Asr;
    apo_Engine_Agent->m_po_Vocon_LH_OBJECT_Mod_Ac = ro_Request_Factory.m_po_Vocon_LH_OBJECT_Mod_Ac;

    apo_Engine_Agent->m_function_On_Event_Init_Notify = ro_Request_Factory.m_function_On_Event_Init_Notify;
    apo_Engine_Agent->m_string_Center_Vr_Url = ro_Request_Factory.m_string_Center_Vr_Url;


    e_Result = apo_Engine_Agent->Party_Load("Origin");
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        delete apo_Engine_Agent;
        apo_Engine_Agent = NULL;
        RETURN_NULL;
    }

    e_Result = apo_Engine_Agent->Language_Change(ro_Request_Factory.m_string_Id_Language, false);
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        delete apo_Engine_Agent;
        apo_Engine_Agent = NULL;
        RETURN_NULL;
    }

    sg_b_Engine_Exist = true;

    return apo_Engine_Agent;
}

C_Engine *
C_Engine::Factory
(
    string const & rstring_Id_Language
)
{
    E_Result e_Result;

    N_Impl::C_Engine_Agent * apo_Engine_Agent = MACRO_New N_Impl::C_Engine_Agent();

    apo_Engine_Agent->m_e_Type_Engine = E_Type_Engine_Vocon;

    e_Result = apo_Engine_Agent->Party_Load("Origin");
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        delete apo_Engine_Agent;
        apo_Engine_Agent = NULL;
        RETURN_NULL;
    }

    e_Result = apo_Engine_Agent->Language_Change(rstring_Id_Language, false);
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        delete apo_Engine_Agent;
        apo_Engine_Agent = NULL;
        RETURN_NULL;
    }

    sg_b_Engine_Exist = true;

    return apo_Engine_Agent;
}

E_Result
C_Engine::Clear_User_Data()
{
    MACRO_Log_Function

    C_Dir o_Dir;
    CL_BOOL o_BOOL = CL_FALSE;
    string string_User_Path;

    IF (sg_b_Engine_Exist) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    string_User_Path = N_Impl::G_o_Get_Path().M_Fn_Get_string_Path_Usr();
    N_Impl::Fn_Log(string_User_Path);
    o_BOOL = o_Dir.RemoveDir(string_User_Path.c_str(), CL_FALSE);
    IF (CL_TRUE != o_BOOL) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    return E_Result_Succeeded;
}

} // name N_Asr

} // name N_Vr

/* EOF */
