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
#include "Vr_Asr_Impl_Vocon_Fn_Get_ro_LH_AUDIOIN_INTERFACE.h"
#include "Vr_Asr_Impl_Event_Phase_Impl.h"
#include "Vr_Asr_Impl_Event_Notify_Impl.h"
#include "Vr_Asr_Impl_Event_Notify_Impl_Signal_Level.h"
#include "Vr_Asr_Impl_Event_Result_Impl.h"
#include "Vr_Asr_Impl_Vocon_Result.h"
#include "Vr_Asr_Impl_Vocon_Data_Node_Result.h"
#include "Vr_Asr_Impl_Vocon_Data_Tag.h"
#include "Vr_Asr_Impl_Vocon_Data_Rule.h"
#include "Vr_Asr_Impl_Vocon_Data_Party.h"
#include "Vr_Asr_Impl_Vocon_Thread_Rec.h"
#include "Vr_Asr_Impl_Vocon_Mgr_Rec.h"
#include "Vr_Asr_Impl_Vocon_Engine_Concrete.h"
#include "Vr_Asr_Impl_Vocon_Data_Context.h"
#include "Vr_Asr_Impl_Vocon_Fn_Callback_LH_Json_Error.h"


namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

C_Mgr_Rec::~C_Mgr_Rec()
{
    MACRO_Log_Function
}

C_Mgr_Rec::C_Mgr_Rec
(
    C_Engine_Concrete & ro_Engine_Concrete
)
: m_ro_Engine_Concrete(ro_Engine_Concrete)
, m_po_Data_Party_Using(NULL)
, m_spstring_Result_XML()
, m_buffer_Userword_Transcription()
, m_buffer_Userword_Audio()
, m_o_Mutex_State()
, m_e_State_Rec(E_State_Rec_Idle_Ack)
, m_scpo_Request_Recognize()
, m_scpo_Thread_Rec()
, m_vector_spo_Result_Hypothesis()
, m_b_VOD_BOS(false)
, m_b_VOD_TSF(false)
, m_b_VOD_TSR(false)
, m_i_Rec_Cnt(0)
, m_b_UserWord_Accepted(false)
, m_o_LH_OBJECT_UserWord_Result(G_o_Vocon().M_Fn_LH_GetNullObj())
, m_o_Time_VOD_BOS()
, m_o_Time_VOD_TSF()
, m_o_Time_VOD_TSR()
, m_o_Time_Result()
, m_o_Time_Begin()
, m_o_Time_End()
, m_i_Audio_level(0)
{
    MACRO_Log_Function
}

E_Result
C_Mgr_Rec::M_Fn_Init()
{
    MACRO_Log_Function

    E_Result e_Result;

    e_Result = _M_Fn_Thread_Init();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::M_Fn_Fina()
{
    MACRO_Log_Function

    E_Result e_Result;

    e_Result = _M_Fn_Thread_Fina();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    return E_Result_Succeeded;
}

void
C_Mgr_Rec::M_Fn_On_Thread_Notified()
{
    MACRO_Log_Function

    E_Result e_Result;

    {
        C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_State);

        IF (m_e_State_Rec == E_State_Rec_Run_Req) {

            m_e_State_Rec = E_State_Rec_Run_Ack;
        }
        ELSE
        IF (m_e_State_Rec == E_State_Rec_Idle_Req) {

            // Do nothing.
        }
        ELSE {

            stringstream stringstream_Log;
            stringstream_Log << "    State: " << static_cast<int>(m_e_State_Rec);
            Fn_Log(stringstream_Log.str());
            MACRO_Log_Error
            return;
        }
    }

    IF (!m_ro_Engine_Concrete.m_b_Init) {
        MACRO_Log_Error
        return;
    }

    IF (m_ro_Engine_Concrete.m_b_Error) {
        MACRO_Log_Error
        return;
    }

    IF (!m_scpo_Request_Recognize) {
        MACRO_Log_Error
        return;
    }

    m_scpo_Request_Recognize->m_function_On_Event_Phase(
        C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Proc_Begin)
    );

    m_o_Time_Begin = C_Time::M_Fn_Get_Time();

    m_b_VOD_BOS = false;
    m_b_VOD_TSF = false;
    m_b_VOD_TSR = false;

    m_ro_Engine_Concrete.M_Fn_Data_Suspend();

    e_Result = _M_Fn_Proc();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        m_ro_Engine_Concrete.m_b_Error = true;
    }

    m_ro_Engine_Concrete.M_Fn_Data_Resume();

    m_o_Time_End = C_Time::M_Fn_Get_Time();

    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Log("");
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Time(m_o_Time_Begin);
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Tab();

    IF (m_b_VOD_BOS) {
        m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Time(m_o_Time_VOD_BOS - m_o_Time_Begin);
    }
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Tab();

    IF (m_b_VOD_TSF) {
        m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Time(m_o_Time_VOD_TSF - m_o_Time_Begin);
    }
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Tab();

    IF (m_b_VOD_TSR) {
        m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Time(m_o_Time_VOD_TSR - m_o_Time_Begin);
    }
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Tab();

    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Time(m_o_Time_Result - m_o_Time_Begin);
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Tab();

    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Time(m_o_Time_End - m_o_Time_Begin);
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Enter();

    {
        C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_State);

        IF ((m_e_State_Rec != E_State_Rec_Run_Ack) && (m_e_State_Rec != E_State_Rec_Idle_Req)) {
            stringstream stringstream_Log;
            stringstream_Log << "    State: " << static_cast<int>(m_e_State_Rec);
            Fn_Log(stringstream_Log.str());
            MACRO_Log_Error
        }

        m_e_State_Rec = E_State_Rec_Idle_Ack;
    }

    m_scpo_Request_Recognize->m_function_On_Event_Phase(
        C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Proc_End)
    );

    IF (10 == m_i_Rec_Cnt++) {
        m_ro_Engine_Concrete.M_Fn_Write_Speaker_Data(false);
    }
}

E_Result
C_Mgr_Rec::M_Fn_On_Callback_Advance
(
    LH_BOOL * po_LH_BOOL_Continue
)
{
    {
        C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_State);

        *po_LH_BOOL_Continue = (m_e_State_Rec != E_State_Rec_Idle_Req) ? LH_TRUE : LH_FALSE;
    }

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::M_Fn_On_Callback_Event
(
    unsigned long i_Type,
    LH_TIME o_LH_TIME
)
{
    LH_ERROR o_LH_ERROR; // = LH_OK;

    stringstream stringstream_Log;
    stringstream_Log << " at time " << o_LH_TIME << "(ms)";

    string string_At_Time(stringstream_Log.str());

    // Phase

    IF (i_Type & LH_AUDIOCHAIN_EVENT_BOS) {
        Fn_Log("    LH_AUDIOCHAIN_EVENT_BOS" + string_At_Time);
        m_scpo_Request_Recognize->m_function_On_Event_Phase(
            C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Speech_Begin_Fx)
        );

        m_o_Time_VOD_BOS = C_Time::M_Fn_Get_Time();
        m_b_VOD_BOS = true;
    }

    IF (i_Type & LH_AUDIOCHAIN_EVENT_TS_FX) {
        Fn_Log("    LH_AUDIOCHAIN_EVENT_TS_FX" + string_At_Time);
        m_scpo_Request_Recognize->m_function_On_Event_Phase(
            C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Speech_End_Fx)
        );

        m_o_Time_VOD_TSF = C_Time::M_Fn_Get_Time();
        m_b_VOD_TSF = true;
    }

    IF (i_Type & LH_AUDIOCHAIN_EVENT_TS_REC) {
        Fn_Log("    LH_AUDIOCHAIN_EVENT_TS_REC" + string_At_Time);
        m_scpo_Request_Recognize->m_function_On_Event_Phase(
            C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Speech_End_Rec)
        );

        m_o_Time_VOD_TSR = C_Time::M_Fn_Get_Time();
        m_b_VOD_TSR = true;
    }

    // Notify

    IF (i_Type & LH_AUDIOCHAIN_EVENT_SSE_NOT_CONVERGED) {
        Fn_Log("    LH_AUDIOCHAIN_EVENT_SSE_NOT_CONVERGED." + string_At_Time);
        m_scpo_Request_Recognize->m_function_On_Event_Notify(
            C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Vocon_SSE_Not_Converged)
        );
    }

    IF (i_Type & LH_AUDIOCHAIN_EVENT_SSE_CONVERGED) {
        Fn_Log("    LH_AUDIOCHAIN_EVENT_SSE_CONVERGED." + string_At_Time);
        m_scpo_Request_Recognize->m_function_On_Event_Notify(
            C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Vocon_SSE_Converged)
        );
    }

    // Notify Fx ABNORMCOND

    IF (i_Type & LH_AUDIOCHAIN_EVENT_FX_ABNORMCOND) {
        LH_FX_ABNORMCOND o_LH_FX_ABNORMCOND;

        Fn_Log("    LH_AUDIOCHAIN_EVENT_FX_ABNORMCOND" + string_At_Time);

        o_LH_ERROR = G_o_Vocon().M_Fn_LH_FxGetAbnormCondition(
            m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Rec(),
            &o_LH_FX_ABNORMCOND
        );

        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Error;
        }

        switch (o_LH_FX_ABNORMCOND) {
        case LH_FX_BADSNR: {
            Fn_Log("    Abnormal condition: LH_FX_BADSNR.");
            m_scpo_Request_Recognize->m_function_On_Event_Notify(
                C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Bad_SNR)
            );
            break;
        }
        case LH_FX_OVERLOAD: {
            Fn_Log("    Abnormal condition: LH_FX_OVERLOAD.");
            m_scpo_Request_Recognize->m_function_On_Event_Notify(
                C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Overload)
            );
            break;
        }
        case LH_FX_TOOQUIET: {
            Fn_Log("  Abnormal condition: LH_FX_TOOQUIET.");
            m_scpo_Request_Recognize->m_function_On_Event_Notify(
                C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Too_Quiet)
            );
            break;
        }
        case LH_FX_NOSIGNAL: {
            Fn_Log("    Abnormal condition: LH_FX_NOSIGNAL.");
            m_scpo_Request_Recognize->m_function_On_Event_Notify(
                C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_No_Signal)
            );
            break;
        }
        case LH_FX_NOLEADINGSILENCE: {
            Fn_Log("    Abnormal condition: LH_FX_NOLEADINGSILENCE.");
            m_scpo_Request_Recognize->m_function_On_Event_Notify(
                C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_No_Leading_Silence)
            );
            break;
        }
        case LH_FX_POORMIC: {
            Fn_Log("    Abnormal condition: LH_FX_POORMIC.");
            m_scpo_Request_Recognize->m_function_On_Event_Notify(
                C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Poor_Mic)
            );
            break;
        }
        default: {
            Fn_Log("    Abnormal condition: Unknown.");
            m_scpo_Request_Recognize->m_function_On_Event_Notify(
                C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Abnormal_Unknown)
            );
            break;
        }
        }
    }

    // Notify Fx TIMER

    IF (i_Type & LH_AUDIOCHAIN_EVENT_FX_TIMER) {
        LH_FX_SIGNAL_LEVELS o_LH_FX_SIGNAL_LEVELS;

        o_LH_ERROR = G_o_Vocon().M_Fn_LH_FxGetSignalLevels(
            m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Rec(),
            &o_LH_FX_SIGNAL_LEVELS
        );

        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Error;
        }

        IF ((o_LH_FX_SIGNAL_LEVELS.energy > 1800)
            || (o_LH_FX_SIGNAL_LEVELS.energy < -7200)) {
            Fn_Log("    audio energy out of range!");
            MACRO_Log_Error
        }
        
        o_LH_FX_SIGNAL_LEVELS.energy = (o_LH_FX_SIGNAL_LEVELS.energy + 7200) / 90;
        IF (o_LH_FX_SIGNAL_LEVELS.energy != m_i_Audio_level) {
            m_i_Audio_level = o_LH_FX_SIGNAL_LEVELS.energy;
            m_scpo_Request_Recognize->m_function_On_Event_Notify(
                C_Event_Notify_Impl_Signal_Level(
                  o_LH_FX_SIGNAL_LEVELS.energy,
                  o_LH_FX_SIGNAL_LEVELS.SNR
                )
            );
        }
    }

    // Result

    IF (i_Type & LH_AUDIOCHAIN_EVENT_RESULT) {
        Fn_Log("    Result" + string_At_Time);

        m_o_Time_Result = C_Time::M_Fn_Get_Time();

        E_Result e_Result;

        e_Result = _M_Fn_Proc_On_Result();
        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }
    }

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::M_Fn_Start
(
    C_Request_Recognize & ro_Request_Recognize
)
{
    MACRO_Log_Function

    {
        C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_State);

        IF (m_e_State_Rec != E_State_Rec_Idle_Ack) {
            stringstream stringstream_Log;
            stringstream_Log << "    State: " << static_cast<int>(m_e_State_Rec);
            Fn_Log(stringstream_Log.str());
            MACRO_Log_Error
            return E_Result_Failed;
        }

        m_e_State_Rec = E_State_Rec_Run_Req;
    }

    m_scpo_Request_Recognize.reset(
        MACRO_New C_Request_Recognize(
            ro_Request_Recognize
        )
    );

    m_scpo_Thread_Rec->Notify();

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::M_Fn_Cancel()
{
    MACRO_Log_Function

    {
        C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_State);

        IF (m_e_State_Rec == E_State_Rec_Idle_Ack) {
            return E_Result_Succeeded;
        }

        m_e_State_Rec = E_State_Rec_Idle_Req;
    }

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::_M_Fn_Thread_Init()
{
    MACRO_Log_Function

    IF ((bool)m_scpo_Thread_Rec) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_scpo_Thread_Rec.reset(MACRO_New C_Thread_Rec(*this));

#if defined(_WIN32) || defined(_WIN32_WCE)
#else
    m_scpo_Thread_Rec->RegisterName("VR_ASR_REC_THREAD");
#endif

    m_scpo_Thread_Rec->StartRegistThread();

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::_M_Fn_Thread_Fina()
{
    MACRO_Log_Function

    IF ((bool)m_scpo_Thread_Rec) {
        m_scpo_Thread_Rec->StopThread();
        m_scpo_Thread_Rec.reset();
    }

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::_M_Fn_Proc()
{
    MACRO_Log_Function

    E_Result e_Result = E_Result_Succeeded;
    E_Result e_Result_Unset_Param = E_Result_Succeeded;
    E_Result e_Result_Dialog_UnLoad = E_Result_Succeeded;

    m_po_Data_Party_Using = m_ro_Engine_Concrete.M_Fn_Find_po_Data_Party(
        m_scpo_Request_Recognize->m_string_Id_Party
    );

    IF (!m_po_Data_Party_Using) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    map<string, pair<unsigned long, long> > o_map_Fx_Param;
    map<string, pair<unsigned long, long> > o_map_Origin_Fx_Param;
    map<string, pair<unsigned long, long> > o_map_Rec_Param;
    map<string, pair<unsigned long, long> > o_map_Origin_Rec_Param;

    e_Result = m_po_Data_Party_Using->M_Fn_Get_Dialog_Fx_And_Rec_Param(
        *m_scpo_Request_Recognize,
        o_map_Fx_Param,
        o_map_Rec_Param);
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    e_Result = _M_Fn_Set_Fx_And_Rec_Param(
        m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Rec(),
        m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Rec(),
        o_map_Fx_Param,
        o_map_Origin_Fx_Param,
        o_map_Rec_Param,
        o_map_Origin_Rec_Param);
    if (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    string string_Dialog_Log = "    Recognize Dialog: " + m_scpo_Request_Recognize->m_string_Id_Dialog;
    Fn_Log(string_Dialog_Log);

    e_Result = m_po_Data_Party_Using->M_Fn_Dialog_Load(
        *m_scpo_Request_Recognize
    );

    do {
        IF (m_scpo_Request_Recognize->m_b_Context_Check) {
            IF (e_Result != E_Result_Succeeded) {
                MACRO_Log_Error
                m_scpo_Request_Recognize->m_function_On_Event_Phase(
                    C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Context_Error)
                );
            }
            ELSE {
                m_scpo_Request_Recognize->m_function_On_Event_Phase(
                    C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Context_OK)
                );
            }

            break;
        }

        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            break;
        }

        e_Result = _M_Fn_Proc_Rec();
        if (E_Result_Succeeded != e_Result) {
            MACRO_Log_Error
            break;
        }

    } while (0);

    e_Result_Dialog_UnLoad = m_po_Data_Party_Using->M_Fn_Dialog_Unload(
        *m_scpo_Request_Recognize
    );

    e_Result_Unset_Param = _M_Fn_Set_Fx_And_Rec_Param(
        m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Rec(),
        m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Rec(),
        o_map_Origin_Fx_Param,
        o_map_Fx_Param,
        o_map_Origin_Rec_Param,
        o_map_Rec_Param);

    IF ((e_Result != E_Result_Succeeded)
        || (e_Result_Unset_Param != E_Result_Succeeded)
        || (e_Result_Dialog_UnLoad != E_Result_Succeeded)) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::_M_Fn_Proc_Rec()
{
    MACRO_Log_Function

    E_Result e_Result = E_Result_Succeeded;

    LH_ERROR o_LH_ERROR; // = LH_OK;

    m_scpo_Request_Recognize->m_function_On_Event_Phase(
        C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Listen_Begin)
    );

    do {

        IF (m_scpo_Request_Recognize->m_b_Barge_In) {

            void * pvoid_Data_Type_Audio_SSE = NULL;
            size_t i_Size_Data_Type_Audio_SSE = 0;

            m_scpo_Request_Recognize->m_spo_Audio_In->M_Fn_Get_Data_Type_Audio_SSE
            (
                &pvoid_Data_Type_Audio_SSE,
                &i_Size_Data_Type_Audio_SSE
            );

            IF (!pvoid_Data_Type_Audio_SSE) {
                MACRO_Log_Error
                e_Result = E_Result_Error;
                break;
            }

#ifdef MACRO_Feature_Reserved

            IF (!G_o_Vocon().M_Fn_LH_ObjIsNull(m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Type_Audio_SSE())) {
                o_LH_ERROR = G_o_Vocon().M_Fn_LH_ObjClose(&m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Type_Audio_SSE());

                IF (LH_OK != o_LH_ERROR) {
                    Fn_Log_LH_ERROR(o_LH_ERROR);
                    MACRO_Log_Error
                    e_Result = E_Result_Error;
                    break;
                }
            }

            o_LH_ERROR = G_o_Vocon().M_Fn_LH_CreateAudioTypeFromSSE(
                m_ro_Engine_Concrete.M_Fn_Get_ro_LH_COMPONENT_Asr(),
                pvoid_Data_Type_Audio_SSE,
                i_Size_Data_Type_Audio_SSE,
                &m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Type_Audio_SSE()
            );

            IF (LH_OK != o_LH_ERROR) {
                Fn_Log_LH_ERROR(o_LH_ERROR);
                MACRO_Log_Error
                e_Result = E_Result_Error;
                break;
            }

            o_LH_ERROR = G_o_Vocon().M_Fn_LH_SingleThreadRecSetAudioType (
                m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Rec(),
                m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Type_Audio_SSE()
            );

            IF (LH_OK != o_LH_ERROR) {
                Fn_Log_LH_ERROR(o_LH_ERROR);
                MACRO_Log_Error
                e_Result = E_Result_Error;
                break;
            }

            o_LH_ERROR = G_o_Vocon().M_Fn_LH_SingleThreadRecControlTimeOut(
                m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Rec(),
                LH_AUDIOSOURCE_TIMEOUT_BARGEIN
            );

            IF (LH_OK != o_LH_ERROR) {
                Fn_Log_LH_ERROR(o_LH_ERROR);
                MACRO_Log_Error
                e_Result = E_Result_Error;
                break;
            }

#endif

        }
        ELSE {

            o_LH_ERROR = G_o_Vocon().M_Fn_LH_SingleThreadRecSetAudioType (
                m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Rec(),
                m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Type_Audio_Sample()
            );

            IF (LH_OK != o_LH_ERROR) {
                Fn_Log_LH_ERROR(o_LH_ERROR);
                MACRO_Log_Error
                e_Result = E_Result_Error;
                break;
            }

            o_LH_ERROR = G_o_Vocon().M_Fn_LH_SingleThreadRecControlTimeOut(
                m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Rec(),
                LH_AUDIOSOURCE_TIMEOUT_ENABLE
            );

            IF (LH_OK != o_LH_ERROR) {
                Fn_Log_LH_ERROR(o_LH_ERROR);
                MACRO_Log_Error
                e_Result = E_Result_Error;
                break;
            }
        }

        o_LH_ERROR = G_o_Vocon().M_Fn_LH_SingleThreadRecProcess(
            m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Rec(),
            &Fn_Get_ro_LH_AUDIOIN_INTERFACE(),
            m_scpo_Request_Recognize->m_spo_Audio_In.get()
        );

        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            e_Result = E_Result_Error;
            break;
        }

        IF (!m_b_UserWord_Accepted) {
            break;
        }
        m_b_UserWord_Accepted = false;

        e_Result = _M_Fn_Proc_On_Result_Userword_Deal();
        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            break;
        }
    } while (0);

    m_scpo_Request_Recognize->m_function_On_Event_Phase(
        C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Listen_End)
    );

    return e_Result;
}

E_Result
C_Mgr_Rec::_M_Fn_Proc_On_Result()
{
    MACRO_Log_Function

    E_Result e_Result;

    LH_ERROR o_LH_ERROR; // = LH_OK;

    LH_OBJECT o_LH_OBJECT_Result = G_o_Vocon().M_Fn_LH_GetNullObj();

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_RecCreateResult(
        m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Rec(),
        &o_LH_OBJECT_Result
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    LH_BOOL o_LH_BOOL_Is_NBest_Result = LH_FALSE;
    LH_BOOL o_LH_BOOL_Is_Json_NBest_Result = LH_FALSE;
    LH_BOOL o_LH_BOOL_Is_Userword_Result = LH_FALSE;

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ObjHasInterface(
        o_LH_OBJECT_Result,
        LH_IID_IUSWRESULT,
        &o_LH_BOOL_Is_Userword_Result
    );
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ObjHasInterface(
        o_LH_OBJECT_Result,
        LH_IID_INBESTRESULT,
        &o_LH_BOOL_Is_NBest_Result);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (o_LH_BOOL_Is_Userword_Result) {
        e_Result = _M_Fn_Proc_On_Result_Userword(
            o_LH_OBJECT_Result
        );
        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
        }

        IF (m_b_UserWord_Accepted) {
            --(m_ro_Engine_Concrete.m_i_Context_Count);
            return E_Result_Succeeded;
        }
    }
    ELSE IF (o_LH_BOOL_Is_NBest_Result) {

        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ObjHasInterface(
            o_LH_OBJECT_Result,
            LH_IID_IJSONPULLPARSER,
            &o_LH_BOOL_Is_Json_NBest_Result);
        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Error;
        }

        IF (o_LH_BOOL_Is_Json_NBest_Result) {
            e_Result = _M_Fn_Proc_On_Result_Json_NBest(o_LH_OBJECT_Result);
            IF (e_Result != E_Result_Succeeded) {
                MACRO_Log_Error
                return E_Result_Error;
            }
        }
        ELSE {
                MACRO_Log_Error
                return E_Result_Error;
        }
    }

#ifdef MACRO_Feature_Reserved

    ELSE {

        char* pc_Json_Result = 0;

        G_o_Vocon().M_Fn_LH_JSONParserFetchString(o_LH_OBJECT_Result, &pc_Json_Result);

        Fn_Log(std::string(pc_Json_Result));

        G_o_Vocon().M_Fn_LH_JSONParserReturnString(o_LH_OBJECT_Result, pc_Json_Result);

        return E_Result_Succeeded;

    }

#endif

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ObjClose(
        &o_LH_OBJECT_Result
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (m_ro_Engine_Concrete.m_i_Context_Count > 1) {
        --(m_ro_Engine_Concrete.m_i_Context_Count);
        return E_Result_Succeeded;
    }
    ELSE IF (0 == m_ro_Engine_Concrete.m_i_Context_Count) {
        MACRO_Log_Error
        return E_Result_Error;
    }
    ELSE {
    }

    IF (m_b_UserWord_Accepted) {
        m_vector_spo_Result_Hypothesis.clear();
        return E_Result_Succeeded;
    }

    pugi::xml_document o_xmlDoc;
    pugi::xml_node decl = o_xmlDoc.prepend_child(pugi::node_declaration);
    decl.append_attribute("version") = "1.0";
    decl.append_attribute("encoding") = "UTF-8";
    pugi::xml_node o_RootNode = o_xmlDoc.append_child("result");

    std::sort(m_vector_spo_Result_Hypothesis.begin(), m_vector_spo_Result_Hypothesis.end(), Result_Hypothesis_Compare);

    vector<shared_ptr<C_Result_Hypothesis> >::reverse_iterator riterator_vector_spo_Result_Hypothesis_End = m_vector_spo_Result_Hypothesis.rend();
    for (
        vector<shared_ptr<C_Result_Hypothesis> >::reverse_iterator riterator_vector_spo_Result_Hypothesis = m_vector_spo_Result_Hypothesis.rbegin();
        riterator_vector_spo_Result_Hypothesis != riterator_vector_spo_Result_Hypothesis_End;
        ++riterator_vector_spo_Result_Hypothesis
    ) {
        IF (!(*riterator_vector_spo_Result_Hypothesis)) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        C_Result_Hypothesis& ro_Result_Hypothesis = *(*riterator_vector_spo_Result_Hypothesis);
        e_Result = _M_Fn_Proc_On_Result_NBest_Hypothesis_XML(
            ro_Result_Hypothesis,
            o_RootNode
        );

        IF (E_Result_Succeeded != e_Result) {
            MACRO_Log_Error
            return e_Result;
        }
    }
    m_vector_spo_Result_Hypothesis.clear();

    C_Event_Result_Impl o_Event_Result_Impl;

    stringstream sstringXml;
    o_xmlDoc.save(sstringXml, "    ");

    m_spstring_Result_XML.reset(MACRO_New string);
    *m_spstring_Result_XML = sstringXml.str();

    stringstream stringstream_Log;
    stringstream_Log.str(*m_spstring_Result_XML);
    string string_Log_Line;

    Fn_Log("");

    while (true) {
        std::getline(stringstream_Log, string_Log_Line);
        IF (!stringstream_Log) {
            break;
        }
        Fn_Log(string_Log_Line);
    }

    Fn_Log("");

    o_Event_Result_Impl.m_spstring_Result_XML = m_spstring_Result_XML;

    m_scpo_Request_Recognize->m_function_On_Event_Result(
        o_Event_Result_Impl
    );

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::_M_Fn_Proc_On_Result_Userword
(
    LH_OBJECT & ro_LH_OBJECT_Result
)
{
    MACRO_Log_Function

    LH_ERROR o_LH_ERROR; // = LH_OK;

    IF ((0 != m_buffer_Userword_Transcription.m_i_Size)
        || (0 != m_buffer_Userword_Audio.m_i_Size)) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    LH_USW_ACCEPTSTATUS o_LH_USW_ACCEPTSTATUS = LH_USW_ACCEPTSTATUS_NOT_TRAINED;

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_UswResultGetAcceptanceStatus(
        ro_LH_OBJECT_Result,
        &o_LH_USW_ACCEPTSTATUS
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    switch (o_LH_USW_ACCEPTSTATUS) {
    case LH_USW_ACCEPTSTATUS_NOT_TRAINED: {
        Fn_Log("    Userword training state: LH_USW_ACCEPTSTATUS_NOT_TRAINED");
        m_scpo_Request_Recognize->m_function_On_Event_Notify(
            C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Not_Trained)
        );
        break;
    }
    case LH_USW_ACCEPTSTATUS_TOO_SHORT: {
        Fn_Log("    Userword training state: LH_USW_ACCEPTSTATUS_TOO_SHORT");
        m_scpo_Request_Recognize->m_function_On_Event_Notify(
            C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Too_Short)
        );
        break;
    }
    case LH_USW_ACCEPTSTATUS_ACCEPTED: {
        Fn_Log("    Userword training state: LH_USW_ACCEPTSTATUS_ACCEPTED");
        memcpy(&m_o_LH_OBJECT_UserWord_Result, &ro_LH_OBJECT_Result, sizeof(LH_OBJECT));
        m_b_UserWord_Accepted = true;
        break;
    }
    default: {
        break;
    }
    }

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::_M_Fn_Proc_On_Result_Userword_Check
(
    LH_OBJECT & ro_LH_OBJECT_Result,
    bool & rb_Voicetag_Alike
)
{
    C_Data_Party * po_Data_Party_Curr = m_ro_Engine_Concrete.M_Fn_Find_po_Data_Party(
        m_scpo_Request_Recognize->m_string_Id_Party
    );
    IF (NULL == po_Data_Party_Curr) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    C_Data_Context * po_Data_Context_Curr = po_Data_Party_Curr->M_Fn_Find_po_Data_Context(
        m_scpo_Request_Recognize->m_string_Voicetag_Context_Id
    );
    IF (NULL == po_Data_Context_Curr) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    LH_ERROR o_LH_ERROR;
    LH_OBJECT o_LH_OBJECT_NBest_Result = G_o_Vocon().M_Fn_LH_GetNullObj();
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_UswResultCreateNBestResultFromConfusion(
        ro_LH_OBJECT_Result,
        po_Data_Context_Curr->m_o_LH_OBJECT_Context,
        &o_LH_OBJECT_NBest_Result
    );
    IF (LH_OK != o_LH_ERROR) {
        MACRO_Log_Error
        return E_Result_Error;
    }


    unsigned long * p_Scores = NULL;
    size_t i_NbrScores = 0;
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_NBestResultBorrowScores(
        o_LH_OBJECT_NBest_Result,
        &p_Scores,
        &i_NbrScores);
    IF (LH_OK != o_LH_ERROR) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    rb_Voicetag_Alike = false;

    stringstream stringstream_Log;
    stringstream_Log << "voicetag score: ";
    size_t i_Scores_Index = 0;
    while (i_Scores_Index < i_NbrScores) {
        unsigned long i_score = p_Scores[i_Scores_Index];
        IF (i_score > m_scpo_Request_Recognize->m_i_Voicetag_Valid_Score) {
            rb_Voicetag_Alike = true;
        }

        stringstream_Log << i_score << " ";

        ++i_Scores_Index;
    }
    Fn_Log(stringstream_Log.str());

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::_M_Fn_Proc_On_Result_Userword_Deal()
{
    E_Result e_Result;
    bool b_Voicetag_Alike = true;

    e_Result = _M_Fn_Proc_On_Result_Userword_Check(
        m_o_LH_OBJECT_UserWord_Result,
        b_Voicetag_Alike);
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    C_Event_Result_Impl o_Event_Result_Impl;

    IF (b_Voicetag_Alike) {
        m_scpo_Request_Recognize->m_function_On_Event_Notify(
            C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Voicetag_Alike)
        );
    }
    ELSE {
        m_scpo_Request_Recognize->m_function_On_Event_Notify(
            C_Event_Notify_Impl(C_Event_Notify::E_Event_Notify_Type_Voicetag_Not_Alike)
        );

        e_Result = _M_Fn_Proc_On_Result_Userword_Transcription_Audio(m_o_LH_OBJECT_UserWord_Result);
        IF (E_Result_Succeeded != e_Result) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        o_Event_Result_Impl.m_buffer_Userword_Transcription = m_buffer_Userword_Transcription;
        o_Event_Result_Impl.m_buffer_Userword_Audio = m_buffer_Userword_Audio;
    }

    G_o_Vocon().M_Fn_LH_ObjClose(&m_o_LH_OBJECT_UserWord_Result);

    m_scpo_Request_Recognize->m_function_On_Event_Result(o_Event_Result_Impl);

    m_buffer_Userword_Transcription.m_sai_Buffer.reset();
    m_buffer_Userword_Transcription.m_i_Size = 0;

    m_buffer_Userword_Audio.m_sai_Buffer.reset();
    m_buffer_Userword_Audio.m_i_Size = 0;

    return E_Result_Succeeded;
}


E_Result
C_Mgr_Rec::_M_Fn_Proc_On_Result_Userword_Transcription_Audio
(
    LH_OBJECT & ro_LH_OBJECT_Result
)
{
    MACRO_Log_Function

    LH_ERROR o_LH_ERROR;

    LH_TRANSCRIPTION * po_LH_TRANSCRIPTION = NULL;
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_UswResultFetchTranscription(
        ro_LH_OBJECT_Result,
        &po_LH_TRANSCRIPTION
    );
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (!po_LH_TRANSCRIPTION) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_buffer_Userword_Transcription.m_i_Size = po_LH_TRANSCRIPTION->size;
    m_buffer_Userword_Transcription.m_sai_Buffer.reset(
        MACRO_New unsigned char[po_LH_TRANSCRIPTION->size]);
    IF (!m_buffer_Userword_Transcription.m_sai_Buffer) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    memcpy(
        m_buffer_Userword_Transcription.m_sai_Buffer.get(),
        po_LH_TRANSCRIPTION->pData,
        po_LH_TRANSCRIPTION->size
    );

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_UswResultReturnTranscription(
        ro_LH_OBJECT_Result,
        po_LH_TRANSCRIPTION
    );
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    LH_BOOL o_LH_BOOL_Has_Audio_Buffer = LH_FALSE;
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_UswResultHasAudioBuffer(
        ro_LH_OBJECT_Result,
        &o_LH_BOOL_Has_Audio_Buffer
    );
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (o_LH_BOOL_Has_Audio_Buffer) {
        unsigned long i_Sampling_Frequency = 0;
        unsigned char * pi_Buffer_Audio = NULL;
        size_t i_Size_Buffer_Audio = 0;

        o_LH_ERROR = G_o_Vocon().M_Fn_LH_UswResultFetchAudioBuffer(
            ro_LH_OBJECT_Result,
            &i_Sampling_Frequency,
            reinterpret_cast<void **>(&pi_Buffer_Audio),
            &i_Size_Buffer_Audio
        );
        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Error;
        }

        m_buffer_Userword_Audio.m_i_Size = i_Size_Buffer_Audio;
        m_buffer_Userword_Audio.m_sai_Buffer.reset(
            MACRO_New unsigned char[i_Size_Buffer_Audio]);
        IF (!m_buffer_Userword_Audio.m_sai_Buffer) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        memcpy(
            m_buffer_Userword_Audio.m_sai_Buffer.get(),
            pi_Buffer_Audio,
            i_Size_Buffer_Audio
        );

        o_LH_ERROR = G_o_Vocon().M_Fn_LH_UswResultReturnAudioBuffer(
            ro_LH_OBJECT_Result,
            pi_Buffer_Audio
        );
        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Error;
        }
    }

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::_M_Fn_Proc_On_Result_Json_NBest
(
    LH_OBJECT & ro_LH_OBJECT_Result
)
{
    MACRO_Log_Function

    LH_ERROR o_LH_ERROR;
    LH_OBJECT o_LH_OBJECT_Cursor = G_o_Vocon().M_Fn_LH_GetNullObj();
    LH_BOOL o_LH_BOOL_Has_Next;

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONParserSetErrorCallback(
        ro_LH_OBJECT_Result,
        Fn_Callback_LH_Json_Error,
        NULL);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONPullParserCreateCursor(ro_LH_OBJECT_Result, &o_LH_OBJECT_Cursor);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONCursorHasNext(o_LH_OBJECT_Cursor, &o_LH_BOOL_Has_Next);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONCursorNext(o_LH_OBJECT_Cursor);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    LH_BOOL o_LH_BOOL_Found;
    char* pc_Json_Value = NULL;
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONCursorObjectBorrowStringValue(
        o_LH_OBJECT_Cursor,
        LH_NBEST_JSON_KEY_RESULT_TYPE,
        &pc_Json_Value,
        &o_LH_BOOL_Found);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF ((LH_TRUE != o_LH_BOOL_Found)
        || (strlen(pc_Json_Value) != strlen(LH_NBEST_JSON_VALUE_NBEST))
        || (0 != strncmp(pc_Json_Value, LH_NBEST_JSON_VALUE_NBEST, strlen(pc_Json_Value)))) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    size_t i_Hypothesis_Id = 0;
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONCursorAdvanceTo(
        o_LH_OBJECT_Cursor,
        LH_NBEST_JSON_KEY_HYPOTHESES,
        &i_Hypothesis_Id,
        &o_LH_BOOL_Found);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (LH_FALSE == o_LH_BOOL_Found) {
        return E_Result_Succeeded;
    }

    size_t i_Hypothesis_Count = 0;
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONCursorArrayGetSize(o_LH_OBJECT_Cursor, &i_Hypothesis_Count);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    Fn_Log("");

    stringstream stringstream_Log;
    stringstream_Log.clear();
    stringstream_Log.str("");
    stringstream_Log << "Hypotheses number: " << i_Hypothesis_Count;
    Fn_Log(stringstream_Log.str());

    for (
        size_t i_Hypothesis_Index = 0;
        i_Hypothesis_Index < i_Hypothesis_Count;
        ++i_Hypothesis_Index
    ) {
        o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONCursorAdvanceToNextArrayItem(o_LH_OBJECT_Cursor, i_Hypothesis_Id);
        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Error;
        }

        E_Result e_Result = _M_Fn_Proc_On_Result_Json_NBest_Hypothesis(
            o_LH_OBJECT_Cursor,
            i_Hypothesis_Index);
        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ObjClose(&o_LH_OBJECT_Cursor);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    Fn_Log("");

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::_M_Fn_Proc_On_Result_Json_NBest_Hypothesis
(
    LH_OBJECT o_LH_OBJECT_Cursor,
    size_t i_Hypothesis_Index
)
{
    LH_ERROR o_LH_ERROR; // = LH_OK;

    shared_ptr<C_Result_Hypothesis> spo_Result_Hypothesis(MACRO_New C_Result_Hypothesis);
    IF (!spo_Result_Hypothesis) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    stringstream stringstream_Log;

    string string_Indent = "    ";

    Fn_Log("");

    stringstream_Log.clear();
    stringstream_Log.str("");
    stringstream_Log << string_Indent << "Hypotheses: " << i_Hypothesis_Index;
    Fn_Log(stringstream_Log.str());

    string_Indent += "    ";

    // Start Rule.
    char * pi_Rule_Start = NULL;
    LH_BOOL o_LH_BOOL_Found;
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONCursorObjectBorrowStringValue(
        o_LH_OBJECT_Cursor,
        LH_NBEST_JSON_KEY_START_RULE,
        &pi_Rule_Start,
        &o_LH_BOOL_Found);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF ((LH_TRUE == o_LH_BOOL_Found)
        && (NULL != pi_Rule_Start)) {
        spo_Result_Hypothesis->m_string_Rule_Start = pi_Rule_Start;
    }

    stringstream_Log.clear();
    stringstream_Log.str("");
    stringstream_Log << string_Indent << "Start Rule: " << spo_Result_Hypothesis->m_string_Rule_Start;
    Fn_Log(stringstream_Log.str());

    // Confidence.
    signed int i_Confidence = 0;
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONCursorObjectGetNumberValue(
        o_LH_OBJECT_Cursor,
        LH_NBEST_JSON_KEY_CONF,
        &i_Confidence,
        &o_LH_BOOL_Found);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (LH_TRUE == o_LH_BOOL_Found) {
        spo_Result_Hypothesis->m_i_Confidence = static_cast<unsigned long>(i_Confidence);
    }

    stringstream_Log.clear();
    stringstream_Log.str("");
    stringstream_Log << string_Indent << "Confidence: " << spo_Result_Hypothesis->m_i_Confidence;
    Fn_Log(stringstream_Log.str());

    stringstream_Log.clear();
    stringstream_Log.str("");
    stringstream_Log << string_Indent << "Items: ";
    Fn_Log(stringstream_Log.str());

    list<C_Result_Item *> list_po_Result_Item_Stack;

    {
        shared_ptr<C_Result_Item> spo_Result_Item_Add(MACRO_New C_Result_Item);

        C_Result_Item & ro_Result_Item_Add = *spo_Result_Item_Add;

        ro_Result_Item_Add.m_b_Is_Slot = true;
        ro_Result_Item_Add.m_i_Confidence = spo_Result_Hypothesis->m_i_Confidence;

        spo_Result_Hypothesis->m_spo_Result_Item_Tree = spo_Result_Item_Add;

        list_po_Result_Item_Stack.push_back(&ro_Result_Item_Add);
    }

    LH_BOOL o_LH_BOOL_Has_Next;
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONCursorHasNext(o_LH_OBJECT_Cursor, &o_LH_BOOL_Has_Next);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (LH_FALSE == o_LH_BOOL_Has_Next) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONCursorNext(o_LH_OBJECT_Cursor);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    LH_JSONCURSOR_TYPE o_LH_JSONCURSOR_TYPE;
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONCursorGetType(o_LH_OBJECT_Cursor, &o_LH_JSONCURSOR_TYPE);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (LH_JSONCURSOR_TYPE_START_ARRAY != o_LH_JSONCURSOR_TYPE) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    string_Indent += "    ";

    unsigned int i_Item_Index = 0;
    E_Result e_Result = _M_Fn_Proc_On_Result_Json_NBest_Array(
        o_LH_OBJECT_Cursor,
        spo_Result_Hypothesis,
        list_po_Result_Item_Stack,
        string_Indent,
        i_Item_Index);
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    string_Indent = string_Indent.substr(4);

    m_vector_spo_Result_Hypothesis.push_back(spo_Result_Hypothesis);

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::_M_Fn_Proc_On_Result_Json_NBest_Array
(
    LH_OBJECT & ro_LH_OBJECT_Cursor,
    shared_ptr<C_Result_Hypothesis>& rspo_Result_Hypothesis,
    list<C_Result_Item *>& ro_list_po_Result_Item_Stack,
    string& ro_string_Indent,
    unsigned int& ri_Item_Index
)
{
    E_Result e_Result;
    LH_ERROR o_LH_ERROR; // = LH_OK;

    size_t i_Items_Count = 0;
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONCursorArrayGetSize(ro_LH_OBJECT_Cursor, &i_Items_Count);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    LH_BOOL o_LH_BOOL_Has_Next;
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONCursorHasNext(ro_LH_OBJECT_Cursor, &o_LH_BOOL_Has_Next);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    size_t i_Items_Index = 0;
    LH_JSONCURSOR_TYPE o_LH_JSONCURSOR_TYPE = LH_JSONCURSOR_TYPE_NONE;
    LH_JSONCURSOR_TYPE o_LH_JSONCURSOR_TYPE_Last;
    int b_Tag = false;
    int b_Cursor_End = false;
    while (LH_TRUE == o_LH_BOOL_Has_Next) {

        o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONCursorNext(ro_LH_OBJECT_Cursor);
        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Error;
        }

        o_LH_JSONCURSOR_TYPE_Last = o_LH_JSONCURSOR_TYPE;
        o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONCursorGetType(ro_LH_OBJECT_Cursor, &o_LH_JSONCURSOR_TYPE);
        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Error;
        }

        switch (o_LH_JSONCURSOR_TYPE) {
        case LH_JSONCURSOR_TYPE_START_OBJECT: {

            b_Tag = false;
            ++i_Items_Index;

            e_Result = _M_Fn_Proc_On_Result_Json_NBest_Object(
                ro_LH_OBJECT_Cursor,
                rspo_Result_Hypothesis,
                ro_list_po_Result_Item_Stack,
                ro_string_Indent,
                ri_Item_Index,
                b_Tag);
            IF (E_Result_Succeeded != e_Result) {
                MACRO_Log_Error
                return E_Result_Error;
            }

            break;
        }
        case LH_JSONCURSOR_TYPE_END_OBJECT: {

            IF (i_Items_Index >= i_Items_Count) {
                b_Cursor_End = true;
                break;
            }

            IF (ro_list_po_Result_Item_Stack.size() <= 1) {
                break;
            } 

            IF (LH_JSONCURSOR_TYPE_START_OBJECT != o_LH_JSONCURSOR_TYPE_Last) {
                break;
            }

            IF (b_Tag) {
                ro_list_po_Result_Item_Stack.pop_back();
            }

            break;
        }
        case LH_JSONCURSOR_TYPE_START_ARRAY: {

            ro_string_Indent += "    ";

            e_Result = _M_Fn_Proc_On_Result_Json_NBest_Array(
                ro_LH_OBJECT_Cursor,
                rspo_Result_Hypothesis,
                ro_list_po_Result_Item_Stack,
                ro_string_Indent,
                ri_Item_Index);
            IF (E_Result_Succeeded != e_Result) {
                MACRO_Log_Error
                return E_Result_Error;
            }

            break;
        }
        case LH_JSONCURSOR_TYPE_END_ARRAY: {
            ro_string_Indent = ro_string_Indent.substr(4);

            IF (ro_list_po_Result_Item_Stack.empty()) {
                MACRO_Log_Error
                return E_Result_Error;
            }
            ro_list_po_Result_Item_Stack.pop_back();

            break;
        }
        case LH_JSONCURSOR_TYPE_NONE: {
            MACRO_Log_Error
            return E_Result_Error;
        }
        default: {
            MACRO_Log_Error
            return E_Result_Error;
        }
        }

        IF (b_Cursor_End) {
            break;
        }

        o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONCursorHasNext(ro_LH_OBJECT_Cursor, &o_LH_BOOL_Has_Next);
        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Error;
        }
    }

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::_M_Fn_Proc_On_Result_Json_NBest_Object
(
    LH_OBJECT & ro_LH_OBJECT_Cursor,
    shared_ptr<C_Result_Hypothesis>& rspo_Result_Hypothesis,
    list<C_Result_Item *>& ro_list_po_Result_Item_Stack,
    string& ro_string_Indent,
    unsigned int& ri_Item_Index,
    int& rb_Tag
)
{
    LH_ERROR o_LH_ERROR; // = LH_OK;
    LH_BOOL o_LH_BOOL_Found;

    shared_ptr<C_Result_Item> spo_Result_Item_Add(MACRO_New C_Result_Item);
    IF (!spo_Result_Item_Add) {
        MACRO_Log_Error
        return E_Result_Error;
    }
    C_Result_Item & ro_Result_Item_Add = *spo_Result_Item_Add;

    char * pi_Item = NULL;
    signed int i_Item = 0;

    // be slot
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONCursorObjectBorrowStringValue(
        ro_LH_OBJECT_Cursor,
        LH_NBEST_JSON_KEY_TYPE,
        &pi_Item,
        &o_LH_BOOL_Found);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (LH_TRUE != o_LH_BOOL_Found) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF ((strlen(pi_Item) == strlen(LH_NBEST_JSON_VALUE_TAG))
        && (0 == strncmp(pi_Item, LH_NBEST_JSON_VALUE_TAG, strlen(pi_Item)))) {
        ro_Result_Item_Add.m_b_Is_Slot = true;
    }
    ELSE
    IF ((strlen(pi_Item) == strlen(LH_NBEST_JSON_VALUE_TERMINAL))
        && (0 == strncmp(pi_Item, LH_NBEST_JSON_VALUE_TERMINAL, strlen(pi_Item)))) {
        ro_Result_Item_Add.m_b_Is_Slot = false;
    }
    ELSE {
        MACRO_Log_Error
        return E_Result_Error;
    }

    // Confidence.
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONCursorObjectGetNumberValue(
        ro_LH_OBJECT_Cursor,
        LH_NBEST_JSON_KEY_CONF,
        &i_Item,
        &o_LH_BOOL_Found);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (LH_TRUE == o_LH_BOOL_Found) {
        ro_Result_Item_Add.m_i_Confidence = static_cast<unsigned long>(i_Item);
    }

    // Score.
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONCursorObjectGetNumberValue(
        ro_LH_OBJECT_Cursor,
        LH_NBEST_JSON_KEY_SCORE,
        &i_Item,
        &o_LH_BOOL_Found);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (LH_TRUE == o_LH_BOOL_Found) {
        ro_Result_Item_Add.m_i_Score = static_cast<unsigned long>(i_Item);
    }

    // User Id Hi.
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONCursorObjectGetNumberValue(
        ro_LH_OBJECT_Cursor,
        LH_NBEST_JSON_KEY_USERID_HI32,
        &i_Item,
        &o_LH_BOOL_Found);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (LH_TRUE == o_LH_BOOL_Found) {
        ro_Result_Item_Add.m_o_Result_Id_User.m_i_Hi = static_cast<unsigned int>(i_Item);
    }

    // User Id Lo.
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONCursorObjectGetNumberValue(
        ro_LH_OBJECT_Cursor,
        LH_NBEST_JSON_KEY_USERID_LO32,
        &i_Item,
        &o_LH_BOOL_Found);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (LH_TRUE == o_LH_BOOL_Found) {
        ro_Result_Item_Add.m_o_Result_Id_User.m_i_Lo = static_cast<unsigned int>(i_Item);
    }

    // name or terminal
    IF (ro_Result_Item_Add.m_b_Is_Slot) {
        o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONCursorObjectBorrowStringValue(
            ro_LH_OBJECT_Cursor,
            LH_NBEST_JSON_KEY_NAME,
            &pi_Item,
            &o_LH_BOOL_Found);
    }
    ELSE {
        o_LH_ERROR = G_o_Vocon().M_Fn_LH_JSONCursorObjectBorrowStringValue(
            ro_LH_OBJECT_Cursor,
            LH_NBEST_JSON_KEY_ORTHOGRAPHY,
            &pi_Item,
            &o_LH_BOOL_Found);
    }
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (LH_TRUE == o_LH_BOOL_Found) {
        ro_Result_Item_Add.m_string_Name = std::string(pi_Item);
    }

    stringstream stringstream_Log;
    stringstream_Log << ro_string_Indent;
    stringstream_Log << "Item: " << ri_Item_Index++ << std::right << "; ";
    stringstream_Log << "Confidence: " << std::setw(5) << std::setfill('0') << ro_Result_Item_Add.m_i_Confidence << "; ";
    stringstream_Log << "Score: " << std::setw(7) << std::setfill('0') << ro_Result_Item_Add.m_i_Score << "; ";
    stringstream_Log << "Id:";
    stringstream_Log << " 0x" << std::hex << std::setw(8) << std::setfill('0') << ro_Result_Item_Add.m_o_Result_Id_User.m_i_Hi << std::dec;
    stringstream_Log << " 0x" << std::hex << std::setw(8) << std::setfill('0') << ro_Result_Item_Add.m_o_Result_Id_User.m_i_Lo << std::dec;
    stringstream_Log << "; ";
    stringstream_Log << "Name: " << ro_Result_Item_Add.m_string_Name;

    Fn_Log(stringstream_Log.str());

    ro_list_po_Result_Item_Stack.back()->m_vector_spo_Result_Item_Tree.push_back(spo_Result_Item_Add);

    IF (ro_Result_Item_Add.m_b_Is_Slot) {
        rb_Tag = true;
        rspo_Result_Hypothesis->m_vector_spo_Result_Item_Flat_Slot.push_back(spo_Result_Item_Add);
        ro_list_po_Result_Item_Stack.push_back(&ro_Result_Item_Add);
    }

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::_M_Fn_Proc_On_Result_NBest_Hypothesis_XML
(
    C_Result_Hypothesis & ro_Result_Hypothesis,
    pugi::xml_node & o_RootNode
)
{
    E_Result e_Result;

    list<shared_ptr<C_Data_Node_Result> > * plist_spo_Data_Node_Result = NULL;

    Fn_Log("");
    
    C_Data_Rule * po_Data_Rule = m_po_Data_Party_Using->M_Fn_Find_po_Data_Rule(
        ro_Result_Hypothesis.m_string_Rule_Start
    );
    IF (po_Data_Rule) {
        plist_spo_Data_Node_Result = &po_Data_Rule->m_list_spo_Data_Node_Result;
    }
    ELSE {
        IF (ro_Result_Hypothesis.m_vector_spo_Result_Item_Flat_Slot.empty()) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        C_Data_Tag * po_Data_Tag = m_po_Data_Party_Using->M_Fn_Find_po_Data_Tag(
            ro_Result_Hypothesis.m_vector_spo_Result_Item_Flat_Slot.front()->m_o_Result_Id_User.m_i_Lo
        );
        IF (!po_Data_Tag) {
            MACRO_Log_Error
            return E_Result_Error;
        }
        ELSE {
            plist_spo_Data_Node_Result = &po_Data_Tag->m_list_spo_Data_Node_Result;
        }
    }

    IF (!plist_spo_Data_Node_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    e_Result = _M_Fn_Proc_On_Result_NBest_Hypothesis_XML_Nodes(
        ro_Result_Hypothesis,
        *plist_spo_Data_Node_Result,
        o_RootNode
    );

    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::_M_Fn_Proc_On_Result_NBest_Hypothesis_XML_Nodes
(
    C_Result_Hypothesis & ro_Result_Hypothesis,
    list<shared_ptr<C_Data_Node_Result> > & rlist_spo_Data_Node_Result,
    pugi::xml_node & o_RootNode
)
{
#ifdef MACRO_Feature_Reserved

    E_Result e_Result;
    string string_Indent;
    stringstream stringstream_Log;

    list<shared_ptr<C_Data_Node_Result> >::iterator iterator_spo_Data_Node_Result_End = rlist_spo_Data_Node_Result.end();

    for (
        list<shared_ptr<C_Data_Node_Result> >::iterator iterator_spo_Data_Node_Result_Curr = rlist_spo_Data_Node_Result.begin();
        iterator_spo_Data_Node_Result_Curr != iterator_spo_Data_Node_Result_End;
        ++iterator_spo_Data_Node_Result_Curr
    ) {
        shared_ptr<C_Data_Node_Result> rspo_Data_Node_Result = *iterator_spo_Data_Node_Result_Curr;
        IF (!rspo_Data_Node_Result) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        C_Data_Node_Result & ro_Data_Node_Result = *rspo_Data_Node_Result;

        int b_Optional = ro_Data_Node_Result.m_b_Optional;

        string string_Name = ro_Data_Node_Result.m_string_Name;
        string string_Value;
        unsigned int i_Conf = 0;

        IF ("hypothesis" == string_Name) {
            string_Indent = "    ";
        }
        ELSE IF ("intent" == string_Name) {
            string_Indent = "        ";
        }
        ELSE {  // for node
            string_Indent = "            ";
        }
        
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << string_Indent
                         << "Node_Name: " << string_Name
                         << ", Type_Tag: " << static_cast<int>(ro_Data_Node_Result.m_e_Type_Tag)
                         << ", Type_Terminal: " << static_cast<int>(ro_Data_Node_Result.m_e_Type_Terminal)
                         << ", Type_Value: " << static_cast<int>(ro_Data_Node_Result.m_e_Type_Value)
                         << ", Pos_Tag: " << ro_Data_Node_Result.m_i_Pos_Tag
                         << ", Code_Tag:  Min(" << ro_Data_Node_Result.m_i_Code_Tag_Min << "), Max(" << ro_Data_Node_Result.m_i_Code_Tag_Max << ")"
                         << ", Pos_Terminal:  " << ro_Data_Node_Result.m_i_Pos_Terminal;
        Fn_Log(stringstream_Log.str());
        
        // Find Tag.

        C_Result_Item * po_Result_Item_Tag = NULL;

        switch (ro_Data_Node_Result.m_e_Type_Tag) {
        case C_Data_Node_Result::E_Type_Tag_None: {

            po_Result_Item_Tag = ro_Result_Hypothesis.m_spo_Result_Item_Tree.get();

            break;
        }
        case C_Data_Node_Result::E_Type_Tag_By_Pos: {

            po_Result_Item_Tag = ro_Result_Hypothesis.Get_po_Result_Item_Tag_By_Pos(
                ro_Data_Node_Result.m_i_Pos_Tag
            );

            break;
        }
        case C_Data_Node_Result::E_Type_Tag_By_Code: {

            po_Result_Item_Tag = ro_Result_Hypothesis.Get_po_Result_Item_Tag_By_Code(
                ro_Data_Node_Result.m_i_Code_Tag_Min,
                ro_Data_Node_Result.m_i_Code_Tag_Max
            );

            break;
        }
        default: {

            MACRO_Log_Error
            return E_Result_Error;
        }
        }

        // Find Terminal.

        C_Result_Item * po_Result_Item_Terminal = NULL;

        switch (ro_Data_Node_Result.m_e_Type_Terminal) {
        case C_Data_Node_Result::E_Type_Terminal_None: {

            break;
        }
        case C_Data_Node_Result::E_Type_Terminal_By_Pos: {

            IF (po_Result_Item_Tag) {

                size_t i_Pos_Terminal_Curr = 0;

                po_Result_Item_Terminal = po_Result_Item_Tag->Get_po_Result_Item_Terminal_By_Pos(
                    ro_Data_Node_Result.m_i_Pos_Terminal,
                    i_Pos_Terminal_Curr
                );
            }

            break;
        }
        case C_Data_Node_Result::E_Type_Terminal_By_Code: {

            IF (po_Result_Item_Tag) {

                po_Result_Item_Terminal = po_Result_Item_Tag->Get_po_Result_Item_Terminal_By_Code(
                    ro_Data_Node_Result.m_i_Code_Terminal_Min,
                    ro_Data_Node_Result.m_i_Code_Terminal_Max
                );
            }

            break;
        }
        default: {

            MACRO_Log_Error
            return E_Result_Error;
        }
        }

        // Get Value.

        switch (ro_Data_Node_Result.m_e_Type_Value) {
        case C_Data_Node_Result::E_Type_Value_None: {

            break;
        }
        case C_Data_Node_Result::E_Type_Value_Defined: {

            string_Value = ro_Data_Node_Result.m_string_Value_Defined;

            i_Conf = ro_Result_Hypothesis.m_i_Confidence;

            break;
        }
        case C_Data_Node_Result::E_Type_Value_Tag_Name: {

            IF (po_Result_Item_Tag) {

                C_Data_Tag * po_Data_Tag = m_po_Data_Party_Using->M_Fn_Find_po_Data_Tag(
                    po_Result_Item_Tag->m_o_Result_Id_User.m_i_Lo
                );
                IF (!po_Data_Tag) {
                    MACRO_Log_Error
                    return E_Result_Error;
                }

                string_Value = po_Data_Tag->m_string_Id_Tag;

                i_Conf = po_Result_Item_Tag->m_i_Confidence;
            }
            ELSE
            IF (!b_Optional) {

                MACRO_Log_Error
                return E_Result_Error;
            }

            break;
        }
        case C_Data_Node_Result::E_Type_Value_Tag_User_Id: {

            IF (po_Result_Item_Tag) {

                stringstream stringstream_Value;

                stringstream_Value << po_Result_Item_Tag->m_o_Result_Id_User.m_i_Lo;

                string_Value = stringstream_Value.str();

                i_Conf = po_Result_Item_Tag->m_i_Confidence;
            }
            ELSE
            IF (!b_Optional) {

                MACRO_Log_Error
                return E_Result_Error;
            }

            break;
        }
        case C_Data_Node_Result::E_Type_Value_Tag_Orthography: {

            IF (po_Result_Item_Tag) {

                string_Value += po_Result_Item_Tag->Get_string_Terminal_Orthography();

                i_Conf = po_Result_Item_Tag->m_i_Confidence;
            }
            ELSE
            IF (!b_Optional) {

                MACRO_Log_Error
                return E_Result_Error;
            }

            break;
        }
        case C_Data_Node_Result::E_Type_Value_Tag_Eval: {

            IF (po_Result_Item_Tag) {
            
                string string_Value_Tmp = po_Result_Item_Tag->Get_string_Eval();
                
                string_Value += _M_Fn_Get_string_Eval_Result(string_Value_Tmp);

                i_Conf = po_Result_Item_Tag->m_i_Confidence;
            }
            ELSE
            IF (!b_Optional) {

                MACRO_Log_Error
                return E_Result_Error;
            }

            break;
        }
        case C_Data_Node_Result::E_Type_Value_Terminal_Name: {

            break;
        }
        case C_Data_Node_Result::E_Type_Value_Terminal_User_Id: {

            IF (po_Result_Item_Terminal) {

                stringstream stringstream_Value;

                stringstream_Value << po_Result_Item_Terminal->m_o_Result_Id_User.m_i_Lo;

                string_Value = stringstream_Value.str();

                i_Conf = po_Result_Item_Terminal->m_i_Confidence;
            }
            ELSE
            IF (!b_Optional) {

                MACRO_Log_Error
                return E_Result_Error;
            }

            break;
        }
        case C_Data_Node_Result::E_Type_Value_Terminal_Orthography: {

            IF (po_Result_Item_Terminal) {

                string_Value = po_Result_Item_Terminal->Get_string_Terminal_Orthography();

                i_Conf = po_Result_Item_Terminal->m_i_Confidence;
            }
            ELSE
            IF (!b_Optional) {

                MACRO_Log_Error
                return E_Result_Error;
            }

            break;
        }
        default: {

            MACRO_Log_Error
            return E_Result_Error;
        }
        }

#if 1 // for PreCV milestone

        pugi::xml_node o_CurrNode;
        string string_To_Modify = "value";
        IF (string_To_Modify == string_Name) {
        
            string_Name = "number";
            
            pugi::xml_node o_CurrNode_Add = o_RootNode.append_child("node");
            
            o_CurrNode_Add.append_attribute("name") = "intent";
            o_CurrNode_Add.append_attribute("value") = "CMN2034_house_number";
            o_CurrNode_Add.append_attribute("confidence") = i_Conf;

            o_CurrNode = o_CurrNode_Add.append_child("node");
        }
        ELSE {
            o_CurrNode = o_RootNode.append_child("node");
        }

        o_CurrNode.append_attribute("name") = string_Name.c_str();
        o_CurrNode.append_attribute("value") = string_Value.c_str();
        o_CurrNode.append_attribute("confidence") = i_Conf;

        e_Result = _M_Fn_Proc_On_Result_NBest_Hypothesis_XML_Nodes(
            ro_Result_Hypothesis,
            ro_Data_Node_Result.m_list_spo_Data_Node_Result,
            o_CurrNode
        );

        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }

#else // original code        

        pugi::xml_node o_CurrNode = o_RootNode.append_child("node");
        o_CurrNode.append_attribute("name") = string_Name.c_str();
        o_CurrNode.append_attribute("value") = string_Value.c_str();
        o_CurrNode.append_attribute("confidence") = i_Conf;

        e_Result = _M_Fn_Proc_On_Result_NBest_Hypothesis_XML_Nodes(
            ro_Result_Hypothesis,
            ro_Data_Node_Result.m_list_spo_Data_Node_Result,
            o_CurrNode
        );

        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }

#endif

    }

#endif

    return E_Result_Succeeded;
}

string
C_Mgr_Rec::_M_Fn_Get_string_Eval_Result
(
    string const& ro_string_Express
)
{
    string string_express = ro_string_Express;

#ifdef MACRO_Feature_Reserved

    size_t i_bracket_l_pos = string::npos;
    size_t i_bracket_r_pos = string::npos;
    
    for (;;) {
        i_bracket_l_pos = string_express.find_first_of('{');
        i_bracket_r_pos = string_express.find_first_of('}');
        IF ((string::npos == i_bracket_l_pos)
            || (string::npos == i_bracket_r_pos)) {
            break;
        }

        IF (i_bracket_r_pos < i_bracket_l_pos) {
            break;
        }

        string_express = string(string_express, 0, i_bracket_l_pos)
                       + _M_Fn_Calculate_Expression(string(string_express, i_bracket_l_pos + 1, i_bracket_r_pos -i_bracket_l_pos - 1))
                       + string(string_express, i_bracket_r_pos + 1);
    }

#endif

    return string_express;
}

#ifdef MACRO_Feature_Reserved

void 
Fn_Handle_Exception
(
    ExprEval::Exception &expect
)
{
    std::string string_err;
    switch (expect.GetType()) {
    case ExprEval::Exception::Type_SyntaxException:
        string_err = "Syntax Error";
        Fn_Log(string_err.c_str());
        break;
    case ExprEval::Exception::Type_EmptyExpressionException:
        string_err = "Empty Expression";
        Fn_Log(string_err.c_str());
        break;
    case ExprEval::Exception::Type_NotFoundException:
        string_err = "Not Found Expression";
        Fn_Log(string_err.c_str());
        break;
    case ExprEval::Exception::Type_AlreadyExistsException:
        string_err = "Already Exists Expression";
        Fn_Log(string_err.c_str());
        break;
    case ExprEval::Exception::Type_NullPointerException:
        string_err = "Null Pointer Expression";
        Fn_Log(string_err.c_str());
        break;
    case ExprEval::Exception::Type_MathException:
        string_err = "Math Expression";
        Fn_Log(string_err.c_str());
        break;
    case ExprEval::Exception::Type_DivideByZeroException:
        string_err = "Divide By Zero Expression";
        Fn_Log(string_err.c_str());
        break;
    case ExprEval::Exception::Type_NoValueListException:
        string_err = "No Value List Expression";
        Fn_Log(string_err.c_str());
        break;
    case ExprEval::Exception::Type_NoFunctionListException:
        string_err = "No Function List Expression";
        Fn_Log(string_err.c_str());
        break;
    case ExprEval::Exception::Type_AbortException: 
        string_err = "Abort Expression";
        Fn_Log(string_err.c_str());
        break;
    case ExprEval::Exception::Type_UnknownTokenException:
        string_err = "Unknown Token Expression";
        Fn_Log(string_err.c_str());
        break;
    case ExprEval::Exception::Type_InvalidArgumentCountException:
        string_err = "Invalid Argument Count Expression";
        Fn_Log(string_err.c_str());
        break;
    case ExprEval::Exception::Type_ConstantAssignException:
        string_err = "Constant Assign Expression";
        Fn_Log(string_err.c_str());
        break;
    case ExprEval::Exception::Type_ConstantReferenceException: 
        string_err = "Constant Reference Expression";
        Fn_Log(string_err.c_str());
        break;
    case ExprEval::Exception::Type_UnmatchedParenthesisException:
        string_err = "Unmatched Parenthesis Expression";
        Fn_Log(string_err.c_str());
        break;
    default:
        string_err = "Other Error";
        Fn_Log(string_err.c_str());
        break;        
    }
}

#endif

string
C_Mgr_Rec::_M_Fn_Calculate_Expression
(
    string const& ro_string_Express
)
{
    int i_sprintf_len = 0;
    char ac_result[MAX_PATH] = { 0 };

#ifdef MACRO_Feature_Reserved

    string string_result = "result";
    string string_express = string_result + "=" + ro_string_Express;
    double d_result_value = 0.0;
    
    try
    {
        ExprEval::ValueList o_ValueList;
        ExprEval::FunctionList o_FunctionList;
        ExprEval::Expression o_Expression;

        o_ValueList.AddDefaultValues();
        o_FunctionList.AddDefaultFunctions();

        o_Expression.SetValueList(&o_ValueList);
        o_Expression.SetFunctionList(&o_FunctionList);
        o_Expression.Parse(string_express);
        o_Expression.Evaluate();

        ExprEval::ValueList::size_type result_index;
        ExprEval::ValueList::size_type result_count = o_ValueList.Count();
        for (result_index = 0; result_index < result_count; ++result_index) {
        
            o_ValueList.Item(result_index, &string_result, &d_result_value);
            IF ("result" == string_result) {
                break;
            }
        }
    }
    catch (ExprEval::Exception &exception)
    {
        Fn_Handle_Exception(exception);
        MACRO_Log_Error
        return string("0");
    }

    i_sprintf_len = sprintf(ac_result, "%d", static_cast<int>(d_result_value));
    IF (i_sprintf_len < 0) {
        MACRO_Log_Error
        return string("0");
    }

#endif

    return string(ac_result, i_sprintf_len);
}

E_Result
C_Mgr_Rec::_M_Fn_Set_Obj_Param
(
    LH_OBJECT& ro_LH_OBJECT,
    map<string, pair<unsigned long, long> >& ro_map_Param,
    map<string, pair<unsigned long, long> >& ro_map_Origin_Param
)
{
    MACRO_Log_Function

    LH_ERROR o_LH_ERROR;

    ro_map_Origin_Param.clear();

    for (
        map<string, pair<unsigned long, long> >::iterator param_Iter = ro_map_Param.begin();
        param_Iter != ro_map_Param.end();
        ++param_Iter
    ) {
        const string & string_Param_Name = param_Iter->first;
        pair<unsigned long, long> & pair_Param = param_Iter->second;

        stringstream o_stringstream_Log;
        o_stringstream_Log << "Set Param: "
                           << string_Param_Name << "("
                           << pair_Param.first << "), Value:"
                           << pair_Param.second;
        Fn_Log(o_stringstream_Log.str());

        long i_Origin_Param = 0;
        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ConfigGetParam(
            ro_LH_OBJECT,
            pair_Param.first,
            &i_Origin_Param);
        IF (LH_OK != o_LH_ERROR) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        ro_map_Origin_Param[string_Param_Name] = std::make_pair(pair_Param.first, i_Origin_Param);

        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ConfigSetParam(
            ro_LH_OBJECT,
            pair_Param.first,
            pair_Param.second);
        IF (LH_OK != o_LH_ERROR) {
            MACRO_Log_Error
            return E_Result_Error;
        }
    }

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::_M_Fn_Set_Fx_And_Rec_Param
(
    LH_OBJECT& ro_LH_OBJECT_Fx,
    LH_OBJECT& ro_LH_OBJECT_Rec,
    map<string, pair<unsigned long, long> >& ro_map_Fx_Param,
    map<string, pair<unsigned long, long> >& ro_map_Origin_Fx_Param,
    map<string, pair<unsigned long, long> >& ro_map_Rec_Param,
    map<string, pair<unsigned long, long> >& ro_map_Origin_Rec_Param
)
{
    MACRO_Log_Function

    E_Result e_Result;

    e_Result = _M_Fn_Set_Obj_Param(
        ro_LH_OBJECT_Fx,
        ro_map_Fx_Param,
        ro_map_Origin_Fx_Param);
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    e_Result = _M_Fn_Set_Obj_Param(
        ro_LH_OBJECT_Rec,
        ro_map_Rec_Param,
        ro_map_Origin_Rec_Param);
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
