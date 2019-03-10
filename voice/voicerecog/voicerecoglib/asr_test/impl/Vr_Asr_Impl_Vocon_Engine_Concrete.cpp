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
#include "Vr_Asr_Impl_Fn_Get_string_Line_Fixed.h"
#include "Vr_Asr_Impl_Fn_Get_string_Path.h"
#include "Vr_Asr_Impl_Fn_Dir_File.h"
#include "Vr_Asr_Impl_Vocon_Fn_Get_ro_LH_HEAP_INTERFACE.h"
#include "Vr_Asr_Impl_Vocon_Fn_Callback_LH_Error.h"
#include "Vr_Asr_Impl_Vocon_Fn_Callback_LH_Rec_Event.h"
#include "Vr_Asr_Impl_Vocon_Fn_Callback_LH_Rec_Advance.h"
#include "Vr_Asr_Impl_Vocon_Heap.h"
#include "Vr_Asr_Impl_Vocon_Data_CLC.h"
#include "Vr_Asr_Impl_Vocon_Data_Party.h"
#include "Vr_Asr_Impl_Vocon_Mgr_Data.h"
#include "Vr_Asr_Impl_Vocon_Mgr_Rec.h"
#include "Vr_Asr_Impl_Vocon_Engine_Concrete.h"
#include "Vr_Asr_Impl_Engine_Agent.h"
#include "Vr_Asr_Impl_Fn_string.h"
#include "Vr_Asr_Impl_Vocon_Data_Transcription.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

C_Engine_Concrete::~C_Engine_Concrete()
{
    MACRO_Log_Function

    m_o_Log_File.M_Fn_Fina();
}

C_Engine_Concrete::C_Engine_Concrete
(
    C_Engine_Agent & ro_Engine_Agent,
    string const & rstring_Id_Language,
    boost::function<void(C_Event_Init_Notify const &)>& ro_function_On_Event_Init_Notify
)
: m_ro_Engine_Agent(ro_Engine_Agent)
, m_function_On_Event_Init_Notify(ro_function_On_Event_Init_Notify)
, m_b_Init(false)
, m_b_Error(false)
, m_scpo_Heap(MACRO_New C_Heap())
, m_scpo_Mgr_Data(MACRO_New C_Mgr_Data(*this))
, m_scpo_Mgr_Rec(MACRO_New C_Mgr_Rec(*this))
, m_o_Log_File()
, m_i_Context_Count(0)
, m_o_LH_COMPONENT_Base(G_o_Vocon().M_Fn_LH_GetNullComponent())
, m_o_LH_COMPONENT_Asr(G_o_Vocon().M_Fn_LH_GetNullComponent())
, m_o_LH_COMPONENT_Pron(G_o_Vocon().M_Fn_LH_GetNullComponent())
, m_o_LH_OBJECT_Mod(G_o_Vocon().M_Fn_LH_GetNullObj())
, m_o_LH_OBJECT_Rec(G_o_Vocon().M_Fn_LH_GetNullObj())
, m_o_LH_OBJECT_Type_Audio_Sample(G_o_Vocon().M_Fn_LH_GetNullObj())
, m_o_LH_OBJECT_Type_Audio_SSE(G_o_Vocon().M_Fn_LH_GetNullObj())
, m_o_LH_OBJECT_Fx(G_o_Vocon().M_Fn_LH_GetNullObj())
, m_o_LH_OBJECT_FmCtxCpl(G_o_Vocon().M_Fn_LH_GetNullObj())
, m_string_Id_Language(rstring_Id_Language)
, m_string_Path_Folder_Mod()
, m_string_Path_File_Mod()
, m_string_Path_File_CLC()
, m_string_Path_File_CLC_V()
, m_string_Path_File_CLC_M()
, m_string_Path_File_Id_Trans_Lookup()
, m_string_Path_Acmod_Session_Data()
, m_string_Path_Fx_Session_Data()
, m_string_Path_Speaker_Profile()
, m_list_spo_Data_CLC()
, m_map_string_po_Data_CLC()
, m_map_string_spo_Data_Party()
, m_i_Frequency_Sample(0)
, m_i_Frame_Shift(0)
{
    MACRO_Log_Function
}

LH_COMPONENT &
C_Engine_Concrete::M_Fn_Get_ro_LH_COMPONENT_Base()
{
    return m_o_LH_COMPONENT_Base;
}

LH_COMPONENT &
C_Engine_Concrete::M_Fn_Get_ro_LH_COMPONENT_Asr()
{
    return m_o_LH_COMPONENT_Asr;
}

LH_COMPONENT &
C_Engine_Concrete::M_Fn_Get_ro_LH_COMPONENT_Pron()
{
    return m_o_LH_COMPONENT_Pron;
}

LH_OBJECT &
C_Engine_Concrete::M_Fn_Get_ro_LH_OBJECT_Mod()
{
    return m_o_LH_OBJECT_Mod;
}

LH_OBJECT &
C_Engine_Concrete::M_Fn_Get_ro_LH_OBJECT_Rec()
{
    return m_o_LH_OBJECT_Rec;
}

LH_OBJECT &
C_Engine_Concrete::M_Fn_Get_ro_LH_OBJECT_Type_Audio_Sample()
{
    return m_o_LH_OBJECT_Type_Audio_Sample;
}

LH_OBJECT &
C_Engine_Concrete::M_Fn_Get_ro_LH_OBJECT_Type_Audio_SSE()
{
    return m_o_LH_OBJECT_Type_Audio_SSE;
}

LH_OBJECT &
C_Engine_Concrete::M_Fn_Get_ro_LH_OBJECT_Fx()
{
    return m_o_LH_OBJECT_Fx;
}

LH_OBJECT &
C_Engine_Concrete::M_Fn_Get_ro_LH_OBJECT_FmCtxCpl()
{
    return m_o_LH_OBJECT_FmCtxCpl;
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

    m_b_Error = true;

    m_string_Path_Folder_Mod = G_o_Get_Path().M_Fn_Get_string_Path_VData() + "Model/Language_" + m_string_Id_Language + '/';

    Fn_Log("    Folder Mod:" + m_string_Path_Folder_Mod);

    {
        std::ifstream ifstream_Info;
        string string_Path_File_Info = m_string_Path_Folder_Mod + "Info.dat";
        ifstream_Info.open(string_Path_File_Info.c_str());
        IF (!ifstream_Info.is_open()) {
            Fn_Log("    Failed to open file: "+ string_Path_File_Info);
            MACRO_Log_Error
            return E_Result_Failed;
        }

        // BOM

        char i_BOM;

        ifstream_Info >> i_BOM >> i_BOM >> i_BOM;

        string string_Line;

        Fn_Get_string_Line_Fixed(ifstream_Info, string_Line);

        // Data

        Fn_Get_string_Line_Fixed(ifstream_Info, string_Line);
        m_string_Path_File_Mod = m_string_Path_Folder_Mod + string_Line;
        Fn_Log("    File Mod:" + m_string_Path_File_Mod);

        Fn_Get_string_Line_Fixed(ifstream_Info, string_Line);
        m_string_Path_File_CLC = m_string_Path_Folder_Mod + string_Line;
        Fn_Log("    File CLC:" + m_string_Path_File_CLC);

        IF (ifstream_Info) {
            Fn_Get_string_Line_Fixed(ifstream_Info, string_Line);
            IF (!string_Line.empty()) {
                m_string_Path_File_CLC_V = m_string_Path_Folder_Mod + string_Line;
            }
            Fn_Log("    File CLC V:" + m_string_Path_File_CLC_V);
        }

        IF (ifstream_Info) {
            Fn_Get_string_Line_Fixed(ifstream_Info, string_Line);
            IF (!string_Line.empty()) {
                m_string_Path_File_CLC_M = m_string_Path_Folder_Mod + string_Line;
            }
            Fn_Log("    File CLC M:" + m_string_Path_File_CLC_M);
        }

        IF (ifstream_Info) {
            Fn_Get_string_Line_Fixed(ifstream_Info, string_Line);
            IF (!string_Line.empty()) {
                m_string_Path_File_Id_Trans_Lookup = m_string_Path_Folder_Mod + string_Line;
            }
            Fn_Log("    File Id Trans Lookup:" + m_string_Path_File_Id_Trans_Lookup);
        }

        ifstream_Info.close();
    }

    string string_Path_Folder_Adaption = G_o_Get_Path().M_Fn_Get_string_Path_Usr() + "Adaption/Language_" + m_string_Id_Language;
    IF (!Fn_File_Access(string_Path_Folder_Adaption)) {
        IF (0 != Fn_Directory_Create_Deep(string_Path_Folder_Adaption)) {
            MACRO_Log_Error
            return E_Result_Failed;
        }
    }
    
    Fn_Log("    Folder Usr:" + string_Path_Folder_Adaption);

    {
        // Data

        m_string_Path_Acmod_Session_Data = string_Path_Folder_Adaption + "/asd.dat";
        Fn_Log("    File Acmod Session Data:" + m_string_Path_Acmod_Session_Data);

        m_string_Path_Fx_Session_Data = string_Path_Folder_Adaption + "/fsd.dat";
        Fn_Log("    File Fx Session Data:" + m_string_Path_Fx_Session_Data);

        m_string_Path_Speaker_Profile = string_Path_Folder_Adaption + "/spd.dat";
        Fn_Log("    File Fx Speaker Profile:" + m_string_Path_Speaker_Profile);
    }

    e_Result = _M_Fn_Base_Init();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    e_Result = _M_Fn_Asr_Init();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    e_Result = _M_Fn_Pron_Init();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    e_Result = _M_Fn_Party_Init();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    e_Result = _M_Fn_Mgr_Init();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    m_o_Log_File.M_Fn_Init(G_o_Get_Path().M_Fn_Get_string_Path_Usr() + "Log.txt");
    m_o_Log_File.M_Fn_Log("Begin\tVOD_BOS\tVOD_TSF\tVOD_TSR\tResult\tEnd");
    m_o_Log_File.M_Fn_Enter();

    m_b_Error = false;

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::M_Fn_Fina()
{
    MACRO_Log_Function

    E_Result e_Result;

    IF (!m_b_Init) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    m_b_Init = false;

    m_b_Error = true;

    e_Result = M_Fn_Write_Speaker_Data(true);
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
    }

    e_Result = _M_Fn_Mgr_Fina();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
    }

    e_Result = _M_Fn_Party_Fina();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
    }

    e_Result = _M_Fn_Pron_Fina();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
    }

    e_Result = _M_Fn_Asr_Fina();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
    }

    e_Result = _M_Fn_Base_Fina();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
    }

    m_o_Log_File.M_Fn_Fina();

    m_b_Error = false;

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::M_Fn_Party_Load
(
    string const & rstring_Id_Party
)
{
#ifdef MACRO_Dynamic_Debug
    MACRO_Log_Function

    IF (!m_b_Init) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    IF (m_b_Error) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_b_Error = true;

    map<string, shared_ptr<C_Data_Party> >::iterator iterator_pair_string_spo_Data_Party_Find = m_map_string_spo_Data_Party.find(rstring_Id_Party);
    IF (iterator_pair_string_spo_Data_Party_Find != m_map_string_spo_Data_Party.end()) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    shared_ptr<C_Data_Party> spo_Data_Party(
        MACRO_New C_Data_Party(
            *this,
            rstring_Id_Party
        )
    );

    spo_Data_Party->M_Fn_Init();

    m_map_string_spo_Data_Party.insert(
        std::make_pair(
            rstring_Id_Party,
            spo_Data_Party
        )
    );

    m_b_Error = false;
#endif
    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::M_Fn_Party_Unload
(
    string const & rstring_Id_Party
)
{
#ifdef MACRO_Dynamic_Debug
    MACRO_Log_Function

    IF (!m_b_Init) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    IF (m_b_Error) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_b_Error = true;

    map<string, shared_ptr<C_Data_Party> >::iterator iterator_pair_string_spo_Data_Party_Find = m_map_string_spo_Data_Party.find(rstring_Id_Party);
    IF (iterator_pair_string_spo_Data_Party_Find == m_map_string_spo_Data_Party.end()) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    shared_ptr<C_Data_Party> & rspo_Data_Party = iterator_pair_string_spo_Data_Party_Find->second;
    IF (!rspo_Data_Party) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    rspo_Data_Party->M_Fn_Fina();

    m_map_string_spo_Data_Party.erase(iterator_pair_string_spo_Data_Party_Find);

    m_b_Error = false;
#endif
    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::M_Fn_Param_Set
(
    C_Request_Param_Set & ro_Request_Param_Set
)
{
    LH_ERROR o_LH_ERROR; // = LH_OK;

    IF (!m_b_Init) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    IF (m_b_Error) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    unsigned int i_Id_Param = 0;

    switch (ro_Request_Param_Set.m_e_Param_Type) {
    case E_Param_Type_Fx_Sensitivity: {

        i_Id_Param = LH_FX_PARAM_SENSITIVITY;
        break;
    }
    case E_Param_Type_Fx_Absolute_Threshold: {

        i_Id_Param = LH_FX_PARAM_ABSOLUTE_THRESHOLD;
        break;
    }
    case E_Param_Type_Fx_Minspeech: {

        i_Id_Param = LH_FX_PARAM_MINSPEECH;
        break;
    }
    case E_Param_Type_Fx_Timeout_Silence_Leading: {

        i_Id_Param = LH_FX_PARAM_TIMEOUT_LSILENCE;
        break;
    }
    case E_Param_Type_Fx_Timeout_Speech: {

        i_Id_Param = LH_FX_PARAM_TIMEOUT_SPEECH;
        break;
    }
    case E_Param_Type_Fx_Timeout_Silence_Trailing: {

        i_Id_Param = LH_FX_PARAM_TSILENCE;
        break;
    }
    case E_Param_Type_Fx_Event_Timer_Interval_Millis: {

        i_Id_Param = LH_FX_PARAM_EVENT_TIMER;
        break;
    }
    case E_Param_Type_Rec_Garbage: {

        i_Id_Param = LH_REC_PARAM_GARBAGE;
        break;
    }
    case E_Param_Type_Rec_Firstpass_Distapprox: {

        i_Id_Param = LH_REC_PARAM_FIRSTPASS_DISTAPPROX;
        break;
    }
    case E_Param_Type_Audio_Timeout: {

        i_Id_Param = LH_AUDIOSOURCE_PARAM_TIMEOUT;
        break;
    }
    default: {

        MACRO_Log_Error
        return E_Result_Failed;
    }
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ConfigSetParam(
        m_o_LH_OBJECT_Rec,
        i_Id_Param,
        ro_Request_Param_Set.m_i_Value
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::M_Fn_Data_Context_List_Update
(
    C_Request_Context_List_Update & ro_Request_Context_List_Update
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

    return m_scpo_Mgr_Data->M_Fn_Context_List_Update(ro_Request_Context_List_Update);
}

#if 0

E_Result
C_Engine_Concrete::M_Fn_Data_Context_Fuzzy_Compile
(
    C_Request_Context_Fuzzy_Compile & ro_Request_Context_Fuzzy_Compile
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

    return m_scpo_Mgr_Data->M_Fn_Context_Fuzzy_Compile(ro_Request_Context_Fuzzy_Compile);
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

    return m_scpo_Mgr_Rec->M_Fn_Start(ro_Request_Recognize);
}

E_Result
C_Engine_Concrete::M_Fn_Rec_Cancel()
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

    return m_scpo_Mgr_Rec->M_Fn_Cancel();
}

E_Result
C_Engine_Concrete::M_Fn_Data_Suspend()
{
    MACRO_Log_Function
#if 0
    IF (!m_b_Init) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    IF (m_b_Error) {
        MACRO_Log_Error
        return E_Result_Error;
    }
#endif
    return m_scpo_Mgr_Data->M_Fn_Suspend();
}

E_Result
C_Engine_Concrete::M_Fn_Data_Resume()
{
    MACRO_Log_Function
#if 0
     IF (!m_b_Init) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    IF (m_b_Error) {
        MACRO_Log_Error
        return E_Result_Error;
    }
#endif
    return m_scpo_Mgr_Data->M_Fn_Resume();
}

C_Data_CLC *
C_Engine_Concrete::M_Fn_Prepare_po_Data_CLC
(
    string const & rstring_Id,
    string& ro_string_Id_Prev
)
{
    MACRO_Log_Function

    Fn_Log("    Request: " + rstring_Id);

    map<string, C_Data_CLC * >::iterator iterator_pair_string_po_Data_CLC_Find;

    iterator_pair_string_po_Data_CLC_Find = m_map_string_po_Data_CLC.find(
        rstring_Id
    );

    C_Data_CLC * po_Data_CLC = NULL;
    string string_Id_Prev;
    IF (iterator_pair_string_po_Data_CLC_Find == m_map_string_po_Data_CLC.end()) {

        IF (!m_list_spo_Data_CLC.empty()) {

            po_Data_CLC = m_list_spo_Data_CLC.front().get();

            po_Data_CLC->M_Fn_Domain_Category_Set("normal", string_Id_Prev);
        }
    }
    ELSE {

        po_Data_CLC = iterator_pair_string_po_Data_CLC_Find->second;

        po_Data_CLC->M_Fn_Domain_Category_Set(rstring_Id, string_Id_Prev);
    }

    ro_string_Id_Prev = string_Id_Prev;

    return po_Data_CLC;
}

E_Result
C_Engine_Concrete::M_Fn_Data_CLC_Transcription_Id_Version_Update
(
    string const& ro_string_Transcription_Id_Version,
    int & b_Transcription_Id_Version_Supported
)
{
    MACRO_Log_Function

    b_Transcription_Id_Version_Supported = false;

    C_Data_CLC* po_Data_CLC = NULL;
    map<string, C_Data_CLC * >::reverse_iterator clc_iter = m_map_string_po_Data_CLC.rbegin();
    while (clc_iter != m_map_string_po_Data_CLC.rend()) {
        po_Data_CLC = clc_iter->second;
        IF (NULL == po_Data_CLC) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        IF (po_Data_CLC->M_Fn_Get_b_Id_Trans_Supported()) {
            break;
        }

        ++clc_iter;
    }

    IF (clc_iter == m_map_string_po_Data_CLC.rend()) {
        Fn_Log("No Transcription Id File");
        return E_Result_Succeeded;
    }

    return po_Data_CLC->M_Fn_Id_Trans_Version_Update(ro_string_Transcription_Id_Version, b_Transcription_Id_Version_Supported);
}

C_Data_Party *
C_Engine_Concrete::M_Fn_Find_po_Data_Party
(
    string const & rstring_Id_Party
)
{
    map<string, shared_ptr<C_Data_Party> >::iterator iterator_pair_string_spo_Data_Party_Find;
    iterator_pair_string_spo_Data_Party_Find = m_map_string_spo_Data_Party.find(
        rstring_Id_Party
    );

    IF (iterator_pair_string_spo_Data_Party_Find == m_map_string_spo_Data_Party.end()) {
        MACRO_Log_Error
        RETURN_NULL;
    }

    shared_ptr<C_Data_Party> & rspo_Data_Party = iterator_pair_string_spo_Data_Party_Find->second;

    return rspo_Data_Party.get();
}

E_Result
C_Engine_Concrete::M_Fn_Write_Speaker_Data(int b_Fina)
{
    MACRO_Log_Function

    LH_ERROR o_LH_ERROR; // = LH_OK;
    ST_ERROR o_ST_ERROR; // = ST_OK;
    LH_OSTREAM_INTERFACE o_LH_OSTREAM_INTERFACE; // The stream interface.
    void * pvoid_Instance_Stream = NULL; // Pointer to the stream.
    LH_OSTREAM_PARAMS o_LH_OSTREAM_PARAMS;
    int b_Write_File_Need = false;

    C_File_Info o_File_Info;
    o_File_Info.m_b_File_Write = true;
    pvoid_Instance_Stream = reinterpret_cast<void*>(&o_File_Info); // Pointer to the stream.

    IF (b_Fina || !Fn_File_Access(m_string_Path_Fx_Session_Data)) {
        b_Write_File_Need = true;
    }

    IF (!m_string_Path_Fx_Session_Data.empty()
        && b_Write_File_Need
        && (LH_TRUE != G_o_Vocon().M_Fn_LH_ObjIsNull(m_o_LH_OBJECT_Fx))) {
        o_ST_ERROR = G_o_Vocon().M_Fn_ST_CreateStreamWriterToFile_Crc(
            m_string_Path_Fx_Session_Data.c_str(),
            &o_LH_OSTREAM_INTERFACE,
            o_File_Info);
        IF (ST_OK != o_ST_ERROR) {
            Fn_Log("    Failed to create stream-writer for file: " + m_string_Path_Fx_Session_Data);
        }
        ELSE {
            memset(&o_LH_OSTREAM_PARAMS, 0, sizeof(LH_OSTREAM_PARAMS));
            o_LH_ERROR =  G_o_Vocon().M_Fn_LH_EnvAdaptArchive(
                m_o_LH_OBJECT_Fx,
                &o_LH_OSTREAM_INTERFACE,
                pvoid_Instance_Stream,
                &o_LH_OSTREAM_PARAMS);
            IF (LH_OK != o_LH_ERROR) {
                Fn_Log_LH_ERROR(o_LH_ERROR);
                MACRO_Log_Error
                return E_Result_Failed;
            }
        }
    }

    b_Write_File_Need = false;
    IF (b_Fina || !Fn_File_Access(m_string_Path_Speaker_Profile)) {
        b_Write_File_Need = true;
    }

    IF (!m_string_Path_Speaker_Profile.empty()
        && b_Write_File_Need
        && (LH_TRUE != G_o_Vocon().M_Fn_LH_ObjIsNull(m_o_LH_OBJECT_Mod))) {
        o_ST_ERROR = G_o_Vocon().M_Fn_ST_CreateStreamWriterToFile_Crc(
            m_string_Path_Speaker_Profile.c_str(),
            &o_LH_OSTREAM_INTERFACE,
            o_File_Info);
        IF (ST_OK != o_ST_ERROR) {
            Fn_Log("    Failed to create stream-writer for file: " + m_string_Path_Speaker_Profile);
        }
        ELSE {
            memset(&o_LH_OSTREAM_PARAMS, 0, sizeof(LH_OSTREAM_PARAMS));
            o_LH_ERROR =  G_o_Vocon().M_Fn_LH_AcModArchiveSpeakerProfile(
                m_o_LH_OBJECT_Mod,
                &o_LH_OSTREAM_INTERFACE,
                pvoid_Instance_Stream,
                &o_LH_OSTREAM_PARAMS);
            IF (LH_OK != o_LH_ERROR) {
                Fn_Log_LH_ERROR(o_LH_ERROR);
                MACRO_Log_Error
                return E_Result_Failed;
            }
        }
    }

    b_Write_File_Need = false;
    IF (b_Fina || !Fn_File_Access(m_string_Path_Acmod_Session_Data)) {
        b_Write_File_Need = true;
    }

    IF (!m_string_Path_Acmod_Session_Data.empty()
        && b_Write_File_Need
        && (LH_TRUE != G_o_Vocon().M_Fn_LH_ObjIsNull(m_o_LH_OBJECT_Mod))) {
        o_ST_ERROR = G_o_Vocon().M_Fn_ST_CreateStreamWriterToFile_Crc(
            m_string_Path_Acmod_Session_Data.c_str(),
            &o_LH_OSTREAM_INTERFACE,
            o_File_Info);
        IF (ST_OK != o_ST_ERROR) {
            Fn_Log("    Failed to create stream-writer for file: " + m_string_Path_Acmod_Session_Data);
        }
        ELSE {
            memset(&o_LH_OSTREAM_PARAMS, 0, sizeof(LH_OSTREAM_PARAMS));
            o_LH_ERROR =  G_o_Vocon().M_Fn_LH_EnvAdaptArchive(
                m_o_LH_OBJECT_Mod,
                &o_LH_OSTREAM_INTERFACE,
                pvoid_Instance_Stream,
                &o_LH_OSTREAM_PARAMS);
            IF (LH_OK != o_LH_ERROR) {
                Fn_Log_LH_ERROR(o_LH_ERROR);
                MACRO_Log_Error
                return E_Result_Failed;
            }
        }
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::M_Fn_AcMod_Check_Dependency
(
    string& string_File_Path,
    bool& b_Acmod_Match
)
{
    LH_ERROR o_LH_ERROR;
    ST_ERROR o_ST_ERROR;
    LH_ISTREAM_INTERFACE o_LH_ISTREAM_INTERFACE;
    void * pvoid_Inst_Stream = NULL;
    LH_STREAMINFO_DEPENDENCY_STATUS o_LH_STREAMINFO_DEPENDENCY_STATUS;
    LH_OBJECT o_LH_OBJECT_StreamInfo = G_o_Vocon().M_Fn_LH_GetNullObj();

    b_Acmod_Match = false;

    o_ST_ERROR = G_o_Vocon().M_Fn_ST_CreateStreamReaderFromFile_Crc(
        string_File_Path.c_str(),
        &o_LH_ISTREAM_INTERFACE,
        &pvoid_Inst_Stream
    );
    IF (ST_OK != o_ST_ERROR) {
        Fn_Log("    The file is damaged!");
        return E_Result_Error;
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_CreateStreamInfo(
        M_Fn_Get_ro_LH_COMPONENT_Base(),
        &o_LH_ISTREAM_INTERFACE,
        pvoid_Inst_Stream,
        NULL,
        &o_LH_OBJECT_StreamInfo
    );
    IF (LH_OK != o_LH_ERROR) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_AcModCheckDependency(
        M_Fn_Get_ro_LH_OBJECT_Mod(),
        o_LH_OBJECT_StreamInfo,
        &o_LH_STREAMINFO_DEPENDENCY_STATUS
    );
    IF (LH_OK != o_LH_ERROR) {
        MACRO_Log_Error
        G_o_Vocon().M_Fn_LH_ObjClose(&o_LH_OBJECT_StreamInfo);
        return E_Result_Error;
    }

    IF (LH_STREAMINFO_DEPENDENCY_STATUS_OK == o_LH_STREAMINFO_DEPENDENCY_STATUS) {
        b_Acmod_Match = true;
    }
    ELSE {
        Fn_Log("    The file does not match the acmod!");
    }

    G_o_Vocon().M_Fn_LH_ObjClose(&o_LH_OBJECT_StreamInfo);

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::M_Fn_Get_Transcriptions
(
    C_Term& ro_Term
)
{
    MACRO_Log_Function

    C_Buffer o_Transcription_Buffer;
    ro_Term.m_vector_buffer_Transcriptions.clear();

    M_Fn_Data_Suspend();

    E_Result e_Result = _M_Fn_Get_Transcriptions(ro_Term, o_Transcription_Buffer);
    IF (E_Result_Succeeded == e_Result) {
        ro_Term.m_vector_buffer_Transcriptions.push_back(o_Transcription_Buffer);
    }
    ELSE {
        MACRO_Log_Error
    }

    M_Fn_Data_Resume();

    return e_Result;
}

E_Result
C_Engine_Concrete::_M_Fn_Base_Init()
{
    MACRO_Log_Function

    LH_ERROR o_LH_ERROR; // = LH_OK;
    PH_ERROR o_PH_ERROR; // = PH_OK;

    memset(&m_scpo_Heap->m_o_LH_HEAP_INTERFACE, 0, sizeof(LH_HEAP_INTERFACE));

    o_PH_ERROR = G_o_Vocon().M_Fn_PH_DlHeapCreate(
        NULL,
        &m_scpo_Heap->m_o_LH_HEAP_INTERFACE,
        &m_scpo_Heap->m_pvoid_Instance
    );

    IF (PH_OK != o_PH_ERROR) {
        Fn_Log_PH_ERROR(o_PH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    m_o_LH_COMPONENT_Base = G_o_Vocon().M_Fn_LH_GetNullComponent();

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_InitBase(
        &Fn_Get_ro_LH_HEAP_INTERFACE(),
        m_scpo_Heap.get(),
        Fn_Callback_LH_Error,
        NULL,
        &m_o_LH_COMPONENT_Base
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Base_Fina()
{
    MACRO_Log_Function

    LH_ERROR o_LH_ERROR; // = LH_OK;
    PH_ERROR o_PH_ERROR; // = PH_OK;

    IF (!G_o_Vocon().M_Fn_LH_ComponentIsNull(m_o_LH_COMPONENT_Base)) {
        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ComponentTerminate(&m_o_LH_COMPONENT_Base);

        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Failed;
        }

        m_o_LH_COMPONENT_Base = G_o_Vocon().M_Fn_LH_GetNullComponent();
    }

    o_PH_ERROR = G_o_Vocon().M_Fn_PH_DlHeapClose(&m_scpo_Heap->m_pvoid_Instance);

    IF (PH_OK != o_PH_ERROR) {
        Fn_Log_PH_ERROR(o_PH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Asr_Init()
{
    MACRO_Log_Function

    E_Result e_Result;

    LH_ERROR o_LH_ERROR; // = LH_OK;

    // ASR component.

    m_o_LH_COMPONENT_Asr = G_o_Vocon().M_Fn_LH_GetNullComponent();

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_InitAsr (
        m_o_LH_COMPONENT_Base,
        &Fn_Get_ro_LH_HEAP_INTERFACE(),
        m_scpo_Heap.get(),
        &m_o_LH_COMPONENT_Asr
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    e_Result = _M_Fn_Asr_Mod_Init();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    e_Result = _M_Fn_Asr_Fx_Init();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    e_Result = _M_Fn_Asr_Rec_Init();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Asr_Fina()
{
    MACRO_Log_Function

    E_Result e_Result;

    LH_ERROR o_LH_ERROR; // = LH_OK;

    IF (!G_o_Vocon().M_Fn_LH_ComponentIsNull(m_o_LH_COMPONENT_Asr)) {
        LH_BOOL o_LH_BOOL_Error;

        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ComponentIsInErrorState(
            m_o_LH_COMPONENT_Asr,
            &o_LH_BOOL_Error
        );

        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Failed;
        }

        IF (!o_LH_BOOL_Error) {
            e_Result = _M_Fn_Asr_Rec_Fina();

            IF (e_Result != E_Result_Succeeded) {
                MACRO_Log_Error
                return e_Result;
            }

            e_Result = _M_Fn_Asr_Fx_Fina();
            
            IF (e_Result != E_Result_Succeeded) {
                MACRO_Log_Error
                return e_Result;
            }

            e_Result = _M_Fn_Asr_Mod_Fina();

            IF (e_Result != E_Result_Succeeded) {
                MACRO_Log_Error
                return e_Result;
            }
        }

        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ComponentTerminate(&m_o_LH_COMPONENT_Asr);

        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Failed;
        }
        m_o_LH_COMPONENT_Asr = G_o_Vocon().M_Fn_LH_GetNullComponent();
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Asr_Mod_Init()
{
    MACRO_Log_Function

    ST_ERROR o_ST_ERROR; // = ST_OK;
    LH_ERROR o_LH_ERROR; // = LH_OK;

    // Stream.

    LH_ISTREAM_INTERFACE o_LH_ISTREAM_INTERFACE; // The stream interface.

    void * pvoid_Instance_Stream = NULL; // Pointer to the stream.

    LH_ISTREAM_PARAMS o_LH_ISTREAM_PARAMS;

    o_ST_ERROR = G_o_Vocon().M_Fn_ST_CreateStreamReaderFromFile(
        m_string_Path_File_Mod.c_str(),
        &o_LH_ISTREAM_INTERFACE,
        &pvoid_Instance_Stream
    );

    IF (ST_OK != o_ST_ERROR) {
        Fn_Log("    Path: " + m_string_Path_File_Mod);
        Fn_Log_ST_ERROR(o_ST_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // AcMod object.

    m_o_LH_OBJECT_Mod = G_o_Vocon().M_Fn_LH_GetNullObj();

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_CreateAcMod(
        M_Fn_Get_ro_LH_COMPONENT_Asr(),
        &o_LH_ISTREAM_INTERFACE,
        pvoid_Instance_Stream,
        NULL,
        &m_o_LH_OBJECT_Mod
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    _M_Fn_Check_Speaker_Data();

    IF (!m_string_Path_Acmod_Session_Data.empty() && Fn_File_Access(m_string_Path_Acmod_Session_Data)) {
        o_ST_ERROR = G_o_Vocon().M_Fn_ST_CreateStreamReaderFromFile_Crc(
            m_string_Path_Acmod_Session_Data.c_str(),
            &o_LH_ISTREAM_INTERFACE,
            &pvoid_Instance_Stream);
        IF (ST_OK != o_ST_ERROR) {
            Fn_Log("    Failed to create stream-reader for file: " + m_string_Path_Acmod_Session_Data);
        }
        ELSE {
            memset(&o_LH_ISTREAM_PARAMS, 0, sizeof(LH_ISTREAM_PARAMS));
            o_LH_ERROR = G_o_Vocon().M_Fn_LH_EnvAdaptReset(
                m_o_LH_OBJECT_Mod,
                &o_LH_ISTREAM_INTERFACE,
                pvoid_Instance_Stream,
                &o_LH_ISTREAM_PARAMS);
            IF (LH_OK != o_LH_ERROR) {
                Fn_File_Delete(m_string_Path_Acmod_Session_Data);
                Fn_Log_LH_ERROR(o_LH_ERROR);
                MACRO_Log_Error
                return E_Result_Failed;
            }
        }
    }

    IF (!m_string_Path_Speaker_Profile.empty() && Fn_File_Access(m_string_Path_Speaker_Profile)) {
        o_ST_ERROR = G_o_Vocon().M_Fn_ST_CreateStreamReaderFromFile_Crc(
            m_string_Path_Speaker_Profile.c_str(),
            &o_LH_ISTREAM_INTERFACE,
            &pvoid_Instance_Stream);
        IF (ST_OK != o_ST_ERROR) {
            Fn_Log("    Failed to create stream-reader for file: " + m_string_Path_Speaker_Profile);
        }
        ELSE {
            memset(&o_LH_ISTREAM_PARAMS, 0, sizeof(LH_ISTREAM_PARAMS));
            o_LH_ERROR = G_o_Vocon().M_Fn_LH_AcModSetSpeakerProfile(
                m_o_LH_OBJECT_Mod,
                &o_LH_ISTREAM_INTERFACE,
                pvoid_Instance_Stream,
                &o_LH_ISTREAM_PARAMS);
            IF (LH_OK != o_LH_ERROR) {
                Fn_File_Delete(m_string_Path_Speaker_Profile);
                Fn_Log_LH_ERROR(o_LH_ERROR);
                MACRO_Log_Error
                return E_Result_Failed;
            }
        }
    }

    // Retrieve some information from the AcMod object.

    LH_ACMOD_INFO * po_LH_ACMOD_INFO = NULL;

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_AcModBorrowInfo(
        m_o_LH_OBJECT_Mod,
        &po_LH_ACMOD_INFO
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    IF (!po_LH_ACMOD_INFO) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    m_i_Frequency_Sample = po_LH_ACMOD_INFO->sampleFrequency;
    m_i_Frame_Shift = po_LH_ACMOD_INFO->frameShift * m_i_Frequency_Sample / 1000;

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Asr_Mod_Fina()
{
    MACRO_Log_Function

    LH_ERROR o_LH_ERROR; // = LH_OK;

    IF (G_o_Vocon().M_Fn_LH_ObjIsNull(m_o_LH_OBJECT_Mod)) {
        return E_Result_Succeeded;
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ObjClose(&m_o_LH_OBJECT_Mod);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Asr_Fx_Init()
{
    MACRO_Log_Function

    LH_ERROR o_LH_ERROR; // = LH_OK;
    ST_ERROR o_ST_ERROR; // = ST_OK;

    // Stream.

    LH_ISTREAM_INTERFACE o_LH_ISTREAM_INTERFACE; // The stream interface.

    void * pvoid_Instance_Stream = NULL; // Pointer to the stream.

    LH_ISTREAM_PARAMS o_LH_ISTREAM_PARAMS;

    m_o_LH_OBJECT_Fx = G_o_Vocon().M_Fn_LH_GetNullObj();

    o_LH_ERROR =  G_o_Vocon().M_Fn_LH_CreateFx(
        M_Fn_Get_ro_LH_COMPONENT_Asr(),
        &(m_o_LH_OBJECT_Fx));
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }
    
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_FxConfigure(
        m_o_LH_OBJECT_Fx,
        M_Fn_Get_ro_LH_OBJECT_Mod());
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    IF (!m_string_Path_Fx_Session_Data.empty() && Fn_File_Access(m_string_Path_Fx_Session_Data)) {
        o_ST_ERROR = G_o_Vocon().M_Fn_ST_CreateStreamReaderFromFile_Crc(
            m_string_Path_Fx_Session_Data.c_str(),
            &o_LH_ISTREAM_INTERFACE,
            &pvoid_Instance_Stream);
        IF (ST_OK != o_ST_ERROR) {
            Fn_Log("    Failed to creater stream-reader for file: " + m_string_Path_Fx_Session_Data);
        }
        ELSE {
            memset(&o_LH_ISTREAM_PARAMS, 0, sizeof(LH_ISTREAM_PARAMS));
            o_LH_ERROR = G_o_Vocon().M_Fn_LH_EnvAdaptReset(
                m_o_LH_OBJECT_Fx,
                &o_LH_ISTREAM_INTERFACE,
                pvoid_Instance_Stream,
                &o_LH_ISTREAM_PARAMS);
            IF (LH_OK != o_LH_ERROR) {
                Fn_File_Delete(m_string_Path_Fx_Session_Data);
                Fn_Log_LH_ERROR(o_LH_ERROR);
                MACRO_Log_Error
                return E_Result_Failed;
            }
        }
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Asr_Fx_Fina()
{
    MACRO_Log_Function

    LH_ERROR o_LH_ERROR; // = LH_OK;

    IF (G_o_Vocon().M_Fn_LH_ObjIsNull(m_o_LH_OBJECT_Fx)) {
        return E_Result_Succeeded;
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ObjClose(&m_o_LH_OBJECT_Fx);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }
    
    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Asr_Rec_Init()
{
    MACRO_Log_Function

    LH_ERROR o_LH_ERROR; // = LH_OK;

    m_o_LH_OBJECT_Type_Audio_Sample = G_o_Vocon().M_Fn_LH_GetNullObj();

    LH_AUDIOCHAINEVENT_INTERFACE o_LH_AUDIOCHAINEVENT_INTERFACE;
    o_LH_AUDIOCHAINEVENT_INTERFACE.pfevent = Fn_Callback_LH_Rec_Event;
    o_LH_AUDIOCHAINEVENT_INTERFACE.pfadvance = Fn_Callback_LH_Rec_Advance;

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_CreateAudioTypeSamples(
        M_Fn_Get_ro_LH_COMPONENT_Asr(),
        m_i_Frequency_Sample,
        &m_o_LH_OBJECT_Type_Audio_Sample
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    m_o_LH_OBJECT_Rec = G_o_Vocon().M_Fn_LH_GetNullObj();

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_CreateSingleThreadRec(
        M_Fn_Get_ro_LH_COMPONENT_Asr(),
        &o_LH_AUDIOCHAINEVENT_INTERFACE,
        this,
        3000,
        m_o_LH_OBJECT_Type_Audio_Sample,
        &m_o_LH_OBJECT_Rec
    );
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // Set param: LH_FX_PARAM_START_ENABLE

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ConfigSetParam(
        m_o_LH_OBJECT_Rec,
        LH_FX_PARAM_START_ENABLE,
        LH_TRUE
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // Set param: LH_FX_PARAM_SENSITIVITY

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ConfigSetParam(
        m_o_LH_OBJECT_Rec,
        LH_FX_PARAM_SENSITIVITY,
        E_Param_Value_Default_Fx_Sensitivity
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // Set param: LH_FX_PARAM_ABSOLUTE_THRESHOLD

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ConfigSetParam(
        m_o_LH_OBJECT_Rec,
        LH_FX_PARAM_ABSOLUTE_THRESHOLD,
        E_Param_Value_Default_Fx_Absolute_Threshold
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // Set param: LH_FX_PARAM_MINSPEECH

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ConfigSetParam(
        m_o_LH_OBJECT_Rec,
        LH_FX_PARAM_MINSPEECH,
        E_Param_Value_Default_Fx_Minspeech
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // Set param: LH_FX_PARAM_TIMEOUT_LSILENCE

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ConfigSetParam(
        m_o_LH_OBJECT_Rec,
        LH_FX_PARAM_TIMEOUT_LSILENCE,
        E_Param_Value_Default_Fx_Timeout_Silence_Leading
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // Set param: LH_FX_PARAM_TIMEOUT_SPEECH

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ConfigSetParam(
        m_o_LH_OBJECT_Rec,
        LH_FX_PARAM_TIMEOUT_SPEECH,
        E_Param_Value_Default_Fx_Timeout_Speech
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // Set param: LH_FX_PARAM_TSILENCE

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ConfigSetParam(
        m_o_LH_OBJECT_Rec,
        LH_FX_PARAM_TSILENCE,
        E_Param_Value_Default_Fx_Timeout_Silence_Trailing
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // Set param: LH_FX_PARAM_ADAPTATION

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ConfigSetParam(
        m_o_LH_OBJECT_Rec,
        LH_FX_PARAM_ADAPTATION,
        LH_FX_FAST_ADAPTATION
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // Set param: LH_FX_PARAM_EVENT_TIMER

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ConfigSetParam(
        m_o_LH_OBJECT_Rec,
        LH_FX_PARAM_EVENT_TIMER,
        E_Param_Value_Default_Fx_Event_Timer_Interval_Millis
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // Set param: LH_REC_PARAM_GARBAGE

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ConfigSetParam(
        m_o_LH_OBJECT_Rec,
        LH_REC_PARAM_GARBAGE,
        E_Param_Value_Default_Rec_Garbage
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // Set param: LH_REC_PARAM_FIRSTPASS_DISTAPPROX

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ConfigSetParam(
        m_o_LH_OBJECT_Rec,
        LH_REC_PARAM_FIRSTPASS_DISTAPPROX,
        E_Param_Value_Default_Rec_Firstpass_Distapprox
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // Set param: LH_REC_PARAM_SPEAKER_ADAPTATION

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ConfigSetParam(
        m_o_LH_OBJECT_Rec,
        LH_REC_PARAM_SPEAKER_ADAPTATION,
        LH_REC_SPEAKER_PROFILE_AUTOMATIC
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // Set param: LH_AUDIOSOURCE_PARAM_TIMEOUT

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ConfigSetParam(
        m_o_LH_OBJECT_Rec,
        LH_AUDIOSOURCE_PARAM_TIMEOUT,
        E_Param_Value_Default_Audio_Timeout
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Asr_Rec_Fina()
{
    MACRO_Log_Function

    LH_ERROR o_LH_ERROR; // = LH_OK;

    IF (!G_o_Vocon().M_Fn_LH_ObjIsNull(m_o_LH_OBJECT_Rec)) {
        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ObjClose(&m_o_LH_OBJECT_Rec);

        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Failed;
        }
    }

    IF (!G_o_Vocon().M_Fn_LH_ObjIsNull(m_o_LH_OBJECT_Type_Audio_SSE)) {
        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ObjClose(&m_o_LH_OBJECT_Type_Audio_SSE);

        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Failed;
        }
    }

    IF (!G_o_Vocon().M_Fn_LH_ObjIsNull(m_o_LH_OBJECT_Type_Audio_Sample)) {
        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ObjClose(&m_o_LH_OBJECT_Type_Audio_Sample);

        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Failed;
        }
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Pron_Init()
{
    MACRO_Log_Function

    E_Result e_Result;

    LH_ERROR o_LH_ERROR; // = LH_OK;

    // Pron component.

    m_o_LH_COMPONENT_Pron = G_o_Vocon().M_Fn_LH_GetNullComponent();

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_InitPron(
        m_o_LH_COMPONENT_Base,
        &Fn_Get_ro_LH_HEAP_INTERFACE(),
        m_scpo_Heap.get(),
        &m_o_LH_COMPONENT_Pron
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    e_Result = _M_Fn_Pron_CLC_Init();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Pron_Fina()
{
    MACRO_Log_Function

    E_Result e_Result;

    LH_ERROR o_LH_ERROR; // = LH_OK;

    IF (!G_o_Vocon().M_Fn_LH_ComponentIsNull(m_o_LH_COMPONENT_Pron)) {
        LH_BOOL o_LH_BOOL_Error;

        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ComponentIsInErrorState(
            m_o_LH_COMPONENT_Pron,
            &o_LH_BOOL_Error
        );

        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Failed;
        }

        IF (!o_LH_BOOL_Error) {
            e_Result = _M_Fn_Pron_CLC_Fina();
            IF (e_Result != E_Result_Succeeded) {
                MACRO_Log_Error
                return e_Result;
            }
        }

        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ComponentTerminate(&m_o_LH_COMPONENT_Pron);

        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Failed;
        }

        m_o_LH_COMPONENT_Pron = G_o_Vocon().M_Fn_LH_GetNullComponent();
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Pron_CLC_Init()
{
    MACRO_Log_Function

    E_Result e_Result;

    m_map_string_po_Data_CLC.clear();

    m_list_spo_Data_CLC.clear();

    m_list_spo_Data_CLC.push_back(
        shared_ptr<C_Data_CLC>(
            MACRO_New C_Data_CLC(
                *this,
                "cfg",
                m_string_Path_File_CLC
            )
        )
    );

    IF (Fn_File_Access(m_string_Path_File_CLC_V)) {

        m_list_spo_Data_CLC.push_back(
            shared_ptr<C_Data_CLC>(
                MACRO_New C_Data_CLC(
                    *this,
                    "vadvde",
                    m_string_Path_File_CLC_V
                )
            )
        );
    }

    IF (Fn_File_Access(m_string_Path_File_CLC_M)) {

        m_list_spo_Data_CLC.push_back(
            shared_ptr<C_Data_CLC>(
                MACRO_New C_Data_CLC(
                    *this,
                    "mp3",
                    m_string_Path_File_CLC_M
                )
            )
        );
    }

    // Grace note for music title

    IF (Fn_File_Access(m_string_Path_File_Id_Trans_Lookup)) {

        IF (!m_list_spo_Data_CLC.empty()) {

            shared_ptr<C_Data_CLC> & rspo_Data_CLC = m_list_spo_Data_CLC.back();

            IF (rspo_Data_CLC) {

                rspo_Data_CLC->M_Fn_Set_Path_File_Id_Trans_Lookup(m_string_Path_File_Id_Trans_Lookup);
            }
        }
    }

    for (
        list<shared_ptr<C_Data_CLC> >::reverse_iterator iterator_spo_Data_CLC_Curr = m_list_spo_Data_CLC.rbegin();
        iterator_spo_Data_CLC_Curr != m_list_spo_Data_CLC.rend();
        ++iterator_spo_Data_CLC_Curr
    ) {

        shared_ptr<C_Data_CLC> & rspo_Data_CLC = *iterator_spo_Data_CLC_Curr;

        e_Result = rspo_Data_CLC->M_Fn_Init();
        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Pron_CLC_Fina()
{
    MACRO_Log_Function

    for (
        list<shared_ptr<C_Data_CLC> >::iterator iterator_spo_Data_CLC_Curr = m_list_spo_Data_CLC.begin();
        iterator_spo_Data_CLC_Curr != m_list_spo_Data_CLC.end();
        ++iterator_spo_Data_CLC_Curr
    ) {

        shared_ptr<C_Data_CLC> & rspo_Data_CLC = *iterator_spo_Data_CLC_Curr;

        rspo_Data_CLC->M_Fn_Fina();
    }

    m_map_string_po_Data_CLC.clear();

    m_list_spo_Data_CLC.clear();

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Party_Init()
{
    MACRO_Log_Function

    E_Result e_Result;

    set<string>::iterator iterator_string_Id_Party_End = m_ro_Engine_Agent.m_set_string_Id_Party.end();

    for (
        set<string>::iterator iterator_string_Id_Party_Curr = m_ro_Engine_Agent.m_set_string_Id_Party.begin();
        iterator_string_Id_Party_Curr != iterator_string_Id_Party_End;
        ++iterator_string_Id_Party_Curr
    ) {
        string const & rstring_Id_Party = *iterator_string_Id_Party_Curr;

        map<string, shared_ptr<C_Data_Party> >::iterator iterator_pair_string_spo_Data_Party_Find = m_map_string_spo_Data_Party.find(rstring_Id_Party);
        IF (iterator_pair_string_spo_Data_Party_Find != m_map_string_spo_Data_Party.end()) {
            return E_Result_Failed;
        }

        shared_ptr<C_Data_Party> spo_Data_Party(
            MACRO_New C_Data_Party(
                *this,
                rstring_Id_Party
            )
        );

        e_Result = spo_Data_Party->M_Fn_Init();
        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        m_map_string_spo_Data_Party.insert(
            std::make_pair(
                rstring_Id_Party,
                spo_Data_Party
            )
        );
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Party_Fina()
{
    MACRO_Log_Function

    map<string, shared_ptr<C_Data_Party> >::iterator iterator_pair_string_spo_Data_Party_End = m_map_string_spo_Data_Party.end();

    for (
        map<string, shared_ptr<C_Data_Party> >::iterator iterator_pair_string_spo_Data_Party_Curr = m_map_string_spo_Data_Party.begin();
        iterator_pair_string_spo_Data_Party_Curr != iterator_pair_string_spo_Data_Party_End;
        ++iterator_pair_string_spo_Data_Party_Curr
    ) {
        shared_ptr<C_Data_Party> & rspo_Data_Party = iterator_pair_string_spo_Data_Party_Curr->second;
        IF (!rspo_Data_Party) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        rspo_Data_Party->M_Fn_Fina();
    }

    m_map_string_spo_Data_Party.clear();

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Mgr_Init()
{
    MACRO_Log_Function

    m_scpo_Mgr_Data->M_Fn_Init();

    m_scpo_Mgr_Rec->M_Fn_Init();

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Mgr_Fina()
{
    MACRO_Log_Function

    m_scpo_Mgr_Rec->M_Fn_Fina();

    m_scpo_Mgr_Data->M_Fn_Fina();

    return E_Result_Succeeded;
}

void
C_Engine_Concrete::_M_Fn_Check_Speaker_Data()
{
    bool b_Acmod_Data_Match = false;
    bool b_Acmod_Speaker_Match = false;
    IF (Fn_File_Access(m_string_Path_Acmod_Session_Data)) {
        (void)M_Fn_AcMod_Check_Dependency(m_string_Path_Acmod_Session_Data, b_Acmod_Data_Match);
    }

    IF (Fn_File_Access(m_string_Path_Speaker_Profile)) {
        (void)M_Fn_AcMod_Check_Dependency(m_string_Path_Speaker_Profile, b_Acmod_Speaker_Match);
    }

    IF (!b_Acmod_Data_Match || !b_Acmod_Speaker_Match) {
        Fn_File_Delete(m_string_Path_Acmod_Session_Data);
        Fn_File_Delete(m_string_Path_Speaker_Profile);
        Fn_File_Delete(m_string_Path_Fx_Session_Data);
    }
}

E_Result
C_Engine_Concrete::_M_Fn_Get_Transcriptions
(
    C_Term& ro_Term,
    C_Buffer& ro_Buffer_Transcription
)
{
    MACRO_Log_Function

    E_Result e_Result;

    string string_Id_Category_Prev;
    string string_Id_Category_Curr = ro_Term.m_string_Trans_Category;
    C_Data_CLC * po_Data_CLC = M_Fn_Prepare_po_Data_CLC(
        string_Id_Category_Curr,
        string_Id_Category_Prev);
    IF (NULL == po_Data_CLC) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    int b_Error = false;
    C_Data_Transcription o_Data_Transcription;
    e_Result = o_Data_Transcription.M_Fn_Get_Transcription(
        ro_Term,
        *po_Data_CLC,
        ro_Buffer_Transcription);
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        b_Error = true;
    }

    string_Id_Category_Curr = string_Id_Category_Prev;
    po_Data_CLC = M_Fn_Prepare_po_Data_CLC(
        string_Id_Category_Curr,
        string_Id_Category_Prev);
    IF (NULL == po_Data_CLC) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return b_Error ? E_Result_Error : E_Result_Succeeded;
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
