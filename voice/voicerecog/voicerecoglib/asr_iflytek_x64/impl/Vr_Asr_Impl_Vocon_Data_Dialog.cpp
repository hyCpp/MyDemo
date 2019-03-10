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
#include "Vr_Asr_Impl_Vocon_Data_Context.h"
#include "Vr_Asr_Impl_Vocon_Data_Context_In_Dialog.h"
#include "Vr_Asr_Impl_Vocon_Data_Dialog.h"
#include "Vr_Asr_Impl_Vocon_Data_Party.h"
#include "Vr_Asr_Impl_Vocon_Engine_Concrete.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

C_Data_Dialog::~C_Data_Dialog()
{
}

C_Data_Dialog::C_Data_Dialog
(
    C_Data_Party & ro_Data_Party,
    string const & rstring_Id_Dialog,
    list<shared_ptr<C_Data_Context_In_Dialog> > & rlist_spo_Data_Context_In_Dialog,
    map<string, pair<unsigned long, long> >& ro_map_Fx_Param,
    map<string, pair<unsigned long, long> >& ro_map_Rec_Param
)
: m_ro_Data_Party(ro_Data_Party)
, m_string_Id_Dialog(rstring_Id_Dialog)
, m_list_spo_Data_Context_In_Dialog()
, m_map_Fx_Param(ro_map_Fx_Param)
, m_map_Rec_Param(ro_map_Rec_Param)
{
    m_list_spo_Data_Context_In_Dialog.swap(rlist_spo_Data_Context_In_Dialog);
}

E_Result
C_Data_Dialog::M_Fn_Load
(
    C_Request_Recognize & ro_Request_Recognize
)
{
    MACRO_Log_Function

    E_Result e_Result;

    C_Data_Context * po_Data_Context_Prev = NULL;

    m_ro_Data_Party.m_ro_Engine_Concrete.m_i_Context_Count = 0;

    list<shared_ptr<C_Request_Activate> > list_spo_Request_Activate = ro_Request_Recognize.m_list_spo_Request_Activate;

    list<shared_ptr<C_Data_Context_In_Dialog> >::iterator iterator_list_spo_Data_Context_In_Dialog_End = m_list_spo_Data_Context_In_Dialog.end();

    for (
        list<shared_ptr<C_Data_Context_In_Dialog> >::iterator iterator_list_spo_Data_Context_In_Dialog_Curr = m_list_spo_Data_Context_In_Dialog.begin();
        iterator_list_spo_Data_Context_In_Dialog_Curr != iterator_list_spo_Data_Context_In_Dialog_End;
        ++iterator_list_spo_Data_Context_In_Dialog_Curr
    ) {
        shared_ptr<C_Data_Context_In_Dialog> & rspo_Data_Context_In_Dialog = *iterator_list_spo_Data_Context_In_Dialog_Curr;
        IF (!rspo_Data_Context_In_Dialog) {
            MACRO_Log_Error;
            return E_Result_Error;
        }

        string & rstring_Id_Context = rspo_Data_Context_In_Dialog->m_string_Id_Context;

        C_Data_Context * po_Data_Context = m_ro_Data_Party.M_Fn_Find_po_Data_Context(rstring_Id_Context);
        IF (!po_Data_Context) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        IF ((bool)(rspo_Data_Context_In_Dialog->m_spo_Request_Activate)) {
            list_spo_Request_Activate.push_front(rspo_Data_Context_In_Dialog->m_spo_Request_Activate);
        }

        IF (!po_Data_Context->M_Fn_Is_Ready()) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        e_Result = po_Data_Context->M_Fn_Load(list_spo_Request_Activate);

        IF ((bool)(rspo_Data_Context_In_Dialog->m_spo_Request_Activate)) {
            list_spo_Request_Activate.pop_front();
        }

        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }

        e_Result = po_Data_Context->M_Fn_Set_Ctx_Param(rspo_Data_Context_In_Dialog->m_map_Ctx_Param);
        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }


        IF (rspo_Data_Context_In_Dialog->m_b_Is_Host_For_Merge) {

            IF (po_Data_Context_Prev) {

                e_Result = po_Data_Context_Prev->M_Fn_Rec_Add();
                IF (E_Result_Succeeded != e_Result) {
                    MACRO_Log_Error
                    return E_Result_Error;
                }

                ++(m_ro_Data_Party.m_ro_Engine_Concrete.m_i_Context_Count);
            }
            po_Data_Context_Prev = po_Data_Context;
        }
        ELSE {

            IF (!po_Data_Context_Prev) {
                MACRO_Log_Error
                return E_Result_Error;
            }

            e_Result = po_Data_Context_Prev->M_Fn_Merge_Host_Merge(po_Data_Context);
            IF (e_Result != E_Result_Succeeded) {
                MACRO_Log_Error
                return E_Result_Error;
            }
        }
    }

    IF (!po_Data_Context_Prev) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    e_Result = po_Data_Context_Prev->M_Fn_Rec_Add();
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    ++(m_ro_Data_Party.m_ro_Engine_Concrete.m_i_Context_Count);

    return E_Result_Succeeded;
}

E_Result
C_Data_Dialog::M_Fn_Unload()
{
    MACRO_Log_Function

    E_Result e_Result;

    list<shared_ptr<C_Data_Context_In_Dialog> >::iterator iterator_list_spo_Data_Context_In_Dialog_End = m_list_spo_Data_Context_In_Dialog.end();

    for (
        list<shared_ptr<C_Data_Context_In_Dialog> >::iterator iterator_list_spo_Data_Context_In_Dialog_Curr = m_list_spo_Data_Context_In_Dialog.begin();
        iterator_list_spo_Data_Context_In_Dialog_Curr != iterator_list_spo_Data_Context_In_Dialog_End;
        ++iterator_list_spo_Data_Context_In_Dialog_Curr
    ) {
        shared_ptr<C_Data_Context_In_Dialog> & rspo_Data_Context_In_Dialog = *iterator_list_spo_Data_Context_In_Dialog_Curr;
        IF (!rspo_Data_Context_In_Dialog) {
            MACRO_Log_Error;
            return E_Result_Error;
        }

        C_Data_Context * po_Data_Context = m_ro_Data_Party.M_Fn_Find_po_Data_Context(rspo_Data_Context_In_Dialog->m_string_Id_Context);
        IF (!po_Data_Context) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        IF (rspo_Data_Context_In_Dialog->m_b_Is_Host_For_Merge) {
            e_Result = po_Data_Context->M_Fn_Rec_Remove();
            IF (E_Result_Succeeded != e_Result) {
                MACRO_Log_Error
                return E_Result_Error;
            }

            e_Result = po_Data_Context->M_Fn_Merge_Host_Clear();
            IF (E_Result_Succeeded != e_Result) {
                MACRO_Log_Error
                return E_Result_Error;
            }
        }

        e_Result = po_Data_Context->M_Fn_Unload();

        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }
    }

    return E_Result_Succeeded;
}

map<string, pair<unsigned long, long> >
C_Data_Dialog::M_Fn_Get_Fx_Param()
{
    return m_map_Fx_Param;
}

map<string, pair<unsigned long, long> >
C_Data_Dialog::M_Fn_Get_Rec_Param()
{
    return m_map_Rec_Param;
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */