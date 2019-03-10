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
#include "Vr_Asr_Impl_Vocon_Fn_Get_ro_LH_HEAP_INTERFACE.h"
#include "Vr_Asr_Impl_Vocon_Fn_Callback_LH_Error.h"
#include "Vr_Asr_Impl_Vocon_Fn_Callback_LH_Rec_Event.h"
#include "Vr_Asr_Impl_Vocon_Fn_Callback_LH_Rec_Advance.h"
#include "Vr_Asr_Impl_Vocon_Heap.h"
#include "Vr_Asr_Impl_Vocon_Data_Party.h"
#include "Vr_Asr_Impl_Vocon_Mgr_Data.h"
#include "Vr_Asr_Impl_Vocon_Mgr_Rec.h"
#include "Vr_Asr_Impl_Vocon_Via_Shared_Engine_Concrete.h"
#include "Vr_Asr_Impl_Engine_Agent.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

namespace N_Via_Shared
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
    void * po_Vocon_LH_COMPONENT_Asr,
    void * po_Vocon_LH_OBJECT_Mod_Ac,
    boost::function<void(C_Event_Init_Notify const &)>& ro_function_On_Event_Init_Notify
)
: N_Vocon::C_Engine_Concrete(ro_Engine_Agent, rstring_Id_Language, ro_function_On_Event_Init_Notify)
, m_po_LH_COMPONENT_Asr(NULL)
, m_po_LH_OBJECT_Mod(NULL)
{
    MACRO_Log_Function

    IF (!po_Vocon_LH_COMPONENT_Asr || !po_Vocon_LH_OBJECT_Mod_Ac) {
        MACRO_Log_Error
    }

    m_po_LH_COMPONENT_Asr = reinterpret_cast<LH_COMPONENT *>(po_Vocon_LH_COMPONENT_Asr);
    m_po_LH_OBJECT_Mod = reinterpret_cast<LH_OBJECT *>(po_Vocon_LH_OBJECT_Mod_Ac);
}

LH_COMPONENT &
C_Engine_Concrete::M_Fn_Get_ro_LH_COMPONENT_Asr()
{
    return *m_po_LH_COMPONENT_Asr;
}

LH_OBJECT &
C_Engine_Concrete::M_Fn_Get_ro_LH_OBJECT_Mod()
{
    return *m_po_LH_OBJECT_Mod;
}

E_Result
C_Engine_Concrete::_M_Fn_Base_Init()
{
    MACRO_Log_Function

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Base_Fina()
{
    MACRO_Log_Function

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Asr_Init()
{
    MACRO_Log_Function

    E_Result e_Result;

    e_Result = _M_Fn_Asr_Mod_Init();
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

    IF (!G_o_Vocon().M_Fn_LH_ComponentIsNull(M_Fn_Get_ro_LH_COMPONENT_Asr())) {
        LH_BOOL o_LH_BOOL_Error;

        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ComponentIsInErrorState(
            M_Fn_Get_ro_LH_COMPONENT_Asr(),
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

            e_Result = _M_Fn_Asr_Mod_Fina();

            IF (e_Result != E_Result_Succeeded) {
                MACRO_Log_Error
                return e_Result;
            }
        }
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Asr_Mod_Init()
{
    MACRO_Log_Function

    LH_ERROR o_LH_ERROR; // = LH_OK;

    LH_ACMOD_INFO * po_LH_ACMOD_INFO = NULL;

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_AcModBorrowInfo(
        M_Fn_Get_ro_LH_OBJECT_Mod(),
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

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Pron_Init()
{
    MACRO_Log_Function

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Pron_Fina()
{
    MACRO_Log_Function

    return E_Result_Succeeded;
}

} // name N_Via_Shared

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */