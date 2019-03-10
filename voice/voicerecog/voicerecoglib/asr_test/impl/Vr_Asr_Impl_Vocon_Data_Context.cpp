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
#include "Vr_Asr_Impl_Fn_string.h"
#include "Vr_Asr_Impl_Vocon_Data_Context.h"
#include "Vr_Asr_Impl_Vocon_Data_Party.h"
#include "Vr_Asr_Impl_Vocon_Engine_Concrete.h"
#include "Vr_Asr_Impl_Vocon_Fn_Callback_FuzzyMatchCtxCpl_Error.h"
#include "Vr_Asr_Impl_Vocon_Heap.h"
#include "Vr_Asr_Impl_Fn_Dir_File.h"
#include "Vr_Asr_Impl_Event_Init_Notify_Impl.h"
#include "Vr_Asr_Impl_Vocon_Data_Transcription.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

C_Data_Context::~C_Data_Context()
{
}

C_Data_Context::C_Data_Context
(
    C_Data_Party & ro_Data_Party,
    string const & rstring_Id_Context,
    string const & rstring_Type_Context,
    string const & rstring_Data_File_Context,
    list<pair<string, string> > & rlist_pair_string_Id_Slot_string_Id_Context
)
: m_ro_Data_Party(ro_Data_Party)
, m_o_LH_OBJECT_Context(G_o_Vocon().M_Fn_LH_GetNullObj())
, m_string_Id_Context(rstring_Id_Context)
, m_string_Type_Context(rstring_Type_Context)
, m_string_Data_File_Context(rstring_Data_File_Context)
, m_string_File_Context_Fuzzy_Tmp()
, m_list_pair_string_Id_Slot_string_Id_Context()
, m_b_Obj_Created(false)
, m_b_Obj_Merged(false)
, m_b_Obj_Rec_Added(false)
, m_b_Obj_List_Updating(false)
, m_b_Obj_List_Save_File(true)
, m_b_Slot_Added(false)
, m_vector_i_Column_Id()
, m_vector_i_Column_Attribute()
{
    m_list_pair_string_Id_Slot_string_Id_Context.swap(rlist_pair_string_Id_Slot_string_Id_Context);
}

int
C_Data_Context::M_Fn_Is_Ready()
{
    return true;
}

E_Result
C_Data_Context::M_Fn_Init()
{
    MACRO_Log_Function

    MACRO_Log_Error

    return E_Result_Error;
}

E_Result
C_Data_Context::M_Fn_Fina()
{
    MACRO_Log_Function

    MACRO_Log_Error

    return E_Result_Error;
}

E_Result
C_Data_Context::M_Fn_Load
(
    list<shared_ptr<C_Request_Activate> > & rlist_spo_Request_Activate
)
{
    MACRO_Log_Function

    MACRO_Log_Error

    return E_Result_Error;
}

E_Result
C_Data_Context::M_Fn_Unload()
{
    MACRO_Log_Function

    MACRO_Log_Error

    return E_Result_Error;
}

E_Result
C_Data_Context::M_Fn_Slot_Guest_Load()
{
    MACRO_Log_Function

    MACRO_Log_Error

    return E_Result_Error;
}

E_Result
C_Data_Context::M_Fn_Slot_Guest_Unload()
{
    MACRO_Log_Function

    MACRO_Log_Error

    return E_Result_Error;
}

E_Result
C_Data_Context::M_Fn_Merge_Host_Merge
(
    C_Data_Context * po_Data_Context_Merge_Guest
)
{
#ifdef MACRO_Feature_Reserved

    MACRO_Log_Function

    LH_ERROR o_LH_ERROR; // = LH_OK;
    LH_BOOL b_LH_CanMerge; //  = LH_FALSE;
    string string_Context_Merge_Log;

    IF (NULL == po_Data_Context_Merge_Guest) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    string_Context_Merge_Log = "    Context Merge, Host Context: " + m_string_Id_Context;
    Fn_Log(string_Context_Merge_Log);
    string_Context_Merge_Log = "    Context Merge, Guest Context: " + po_Data_Context_Merge_Guest->m_string_Id_Context;
    Fn_Log(string_Context_Merge_Log);

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_MergeCtxCanMergeCtx(
        m_o_LH_OBJECT_Context, 
        po_Data_Context_Merge_Guest->m_o_LH_OBJECT_Context,
        &b_LH_CanMerge
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (!b_LH_CanMerge) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_MergeCtxMergeCtx(
        m_o_LH_OBJECT_Context, 
        po_Data_Context_Merge_Guest->m_o_LH_OBJECT_Context
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

#endif

    m_b_Obj_Merged = true;

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::M_Fn_Merge_Host_Clear()
{
    MACRO_Log_Function

    IF (!m_b_Obj_Merged) {
        return E_Result_Succeeded;
    }

#ifdef MACRO_Feature_Reserved

    LH_ERROR o_LH_ERROR = G_o_Vocon().M_Fn_LH_MergeCtxClearMerges(m_o_LH_OBJECT_Context);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }
#endif

    m_b_Obj_Merged = false;

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::M_Fn_Rec_Add()
{
    MACRO_Log_Function

    LH_ERROR o_LH_ERROR; // = LH_OK;

    string string_Rec_Add_Log;

    string_Rec_Add_Log = "    Rec Add, Context: " + m_string_Id_Context;
    Fn_Log(string_Rec_Add_Log);

    IF (m_b_Obj_Rec_Added) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_RecAddCtx(
        m_ro_Data_Party.m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Rec(),
        m_o_LH_OBJECT_Context
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_b_Obj_Rec_Added = true;

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::M_Fn_Rec_Remove()
{
    MACRO_Log_Function

    LH_ERROR o_LH_ERROR; // = LH_OK;

    IF (!m_b_Obj_Rec_Added) {
        return E_Result_Succeeded;
    }

    m_b_Obj_Rec_Added = false;

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_RecRemoveCtx(
        m_ro_Data_Party.m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Rec(),
        m_o_LH_OBJECT_Context
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::M_Fn_List_Update_Begin()
{
    m_b_Obj_List_Updating = true;

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::M_Fn_List_Update_Clear()
{
    LH_ERROR o_LH_ERROR; // = LH_OK;

    IF (!G_o_Vocon().M_Fn_LH_ObjIsNull(m_o_LH_OBJECT_Context)) {
        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ObjClose(&m_o_LH_OBJECT_Context);
        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            m_o_LH_OBJECT_Context = G_o_Vocon().M_Fn_LH_GetNullObj();
        }
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_CreateEmptyListCtx(
        m_ro_Data_Party.m_ro_Engine_Concrete.M_Fn_Get_ro_LH_COMPONENT_Asr(),
        m_ro_Data_Party.m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Mod(),
        LH_LISTCTX_KEEP_STRINGS_KEEP_IDS,
        LH_USERID_RANGE_64,
        &m_o_LH_OBJECT_Context
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::M_Fn_List_Update_Delete(const C_Term& o_Term)
{
    LH_ERROR o_LH_ERROR; // = LH_OK;

    IF (o_Term.m_vector_string_Orthography.empty()) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    string string_Orthography = o_Term.m_vector_string_Orthography[0];

    IF (!Fn_Is_Utf8_String(string_Orthography)) {
        Fn_Print_Bin_C_String(string_Orthography.c_str());
        return E_Result_Succeeded;
    }

    LH_GRM_TERMINAL o_LH_GRM_TERMINAL;

    IF (string_Orthography.empty()) {
        o_LH_GRM_TERMINAL.szOrthography = " ";
    }
    else {
        o_LH_GRM_TERMINAL.szOrthography = string_Orthography.c_str();
    }

    o_LH_GRM_TERMINAL.userID.hi32 = o_Term.m_i_User_Data_Hi;
    o_LH_GRM_TERMINAL.userID.lo32 = o_Term.m_i_User_Data_Lo;
    o_LH_GRM_TERMINAL.pTranscriptions = NULL;
    o_LH_GRM_TERMINAL.nbrTranscriptions = 0;

    size_t nbrRemoved;

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ListCtxRemoveAlternative (
        m_o_LH_OBJECT_Context,
        &o_LH_GRM_TERMINAL,
        1,
        LH_BNF_MATCHTYPE_EXACT,
        &nbrRemoved
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::M_Fn_List_Update_Add
(
    const C_Term& o_Term,
    C_Data_CLC& ro_Data_CLC
)
{
    E_Result e_Result;

    IF (o_Term.m_vector_buffer_Transcriptions.empty()) {
        e_Result = _M_Fn_List_Update_Add_Gracenote_And_Text_Transcriptions(
            o_Term,
            ro_Data_CLC);
        IF (E_Result_Succeeded != e_Result) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        return E_Result_Succeeded;
    }

    e_Result =  _M_Fn_List_Update_Add_Userword_Transcriptions(o_Term);
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::M_Fn_List_Update_Commit()
{
    LH_ERROR o_LH_ERROR; // = LH_OK;

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ListCtxCommit(m_o_LH_OBJECT_Context);

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_b_Obj_List_Updating = false;

    return E_Result_Succeeded;
}

#if 0

E_Result
C_Data_Context::M_Fn_Fuzzy_Compile_Begin()
{
    MACRO_Log_Function
    
    E_Result e_Result;
    
    ST_ERROR o_ST_ERROR; // = ST_OK;
    
    LH_ERROR o_LH_ERROR; // = LH_OK;

    LH_OSTREAM_INTERFACE o_LH_OSTREAM_INTERFACE;  

    void * pvoid_Inst_oStream = NULL;

    stringstream stringstream_Log;

    size_t i_find_pos;
    
    string string_File_Suffix;
    
    i_find_pos = m_string_Data_File_Context.find_last_of('.');
    IF (string::npos == i_find_pos) {
        MACRO_Log_Error
        return E_Result_Error;
    }
    string_File_Suffix = std::string(m_string_Data_File_Context, i_find_pos + 1);
    m_string_File_Context_Fuzzy_Tmp = std::string(m_string_Data_File_Context, 0, i_find_pos);
    m_string_File_Context_Fuzzy_Tmp += "_tmp." + string_File_Suffix;

    e_Result = _M_Fn_Obj_Destroy();
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    o_ST_ERROR = G_o_Vocon().M_Fn_ST_CreateStreamWriterToFile(
        m_string_File_Context_Fuzzy_Tmp.c_str(),
        &o_LH_OSTREAM_INTERFACE,
        &pvoid_Inst_oStream);
    IF (o_ST_ERROR != ST_OK) {
        Fn_Log_ST_ERROR(o_ST_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_ro_Data_Party.m_ro_Engine_Concrete.m_o_LH_OBJECT_FmCtxCpl = G_o_Vocon().M_Fn_LH_GetNullObj();
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_CreateFuzzyMatchCtxCpl(
        m_ro_Data_Party.m_ro_Engine_Concrete.m_o_LH_COMPONENT_Asr, 
        &o_LH_OSTREAM_INTERFACE, 
        pvoid_Inst_oStream, 
        NULL, 
        m_ro_Data_Party.m_ro_Engine_Concrete.m_o_LH_OBJECT_Mod, 
        &(m_ro_Data_Party.m_ro_Engine_Concrete.m_o_LH_OBJECT_FmCtxCpl));
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }
    
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_FuzzyMatchCtxCplSetErrorCallback(
        m_ro_Data_Party.m_ro_Engine_Concrete.m_o_LH_OBJECT_FmCtxCpl, 
        Fn_Callback_FuzzyMatchCtxCpl_Error, 
        static_cast<void*>(this));
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    /* Create a data table */
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_FuzzyMatchCtxCplBeginTable(
        m_ro_Data_Party.m_ro_Engine_Concrete.m_o_LH_OBJECT_FmCtxCpl, 
        LH_FUZZYMATCHCTXCPL_TABLE_DATA);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }
    
    return E_Result_Succeeded;
}

E_Result
C_Data_Context::M_Fn_Fuzzy_Compile_Add_Column
(
    vector<shared_ptr<C_Term> > const & ro_vector_spo_Term
)
{
    LH_ERROR o_LH_ERROR; // = LH_OK;

    stringstream stringstream_Log;
    
    LH_FUZZYMATCHCTXCPL_STATUS o_LH_FUZZYMATCHCTXCPL_STATUS; 
    
    vector<boost::shared_ptr<C_Term> >::size_type vector_spo_Term_Size = ro_vector_spo_Term.size();
    m_vector_i_Column_Attribute.clear();
    m_vector_i_Column_Id.resize(vector_spo_Term_Size);
    for (
        vector<boost::shared_ptr<C_Term> >::size_type vector_spo_Term_Index = 0;
        vector_spo_Term_Index < vector_spo_Term_Size;
        ++vector_spo_Term_Index
    ) {
        C_Term& o_Term_Column = *(ro_vector_spo_Term[vector_spo_Term_Index].get());
        m_vector_i_Column_Attribute.push_back(o_Term_Column.m_i_User_Data_Lo);  
        o_LH_ERROR = G_o_Vocon().M_Fn_LH_FuzzyMatchCtxCplAddColumn(
            m_ro_Data_Party.m_ro_Engine_Concrete.m_o_LH_OBJECT_FmCtxCpl, 
            o_Term_Column.m_string_Orthography.c_str(), 
            o_Term_Column.m_i_User_Data_Lo, 
            &(m_vector_i_Column_Id[vector_spo_Term_Index]));
        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Error;
        }

    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_FuzzyMatchCtxCplCommitRow(
            m_ro_Data_Party.m_ro_Engine_Concrete.m_o_LH_OBJECT_FmCtxCpl, 
            &o_LH_FUZZYMATCHCTXCPL_STATUS);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }
    IF (LH_FUZZYMATCHCTXCPL_STATUS_OK != o_LH_FUZZYMATCHCTXCPL_STATUS) {
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "LH_FUZZYMATCHCTXCPL_STATUS: " << static_cast<int>(o_LH_FUZZYMATCHCTXCPL_STATUS);
        Fn_Log(stringstream_Log.str());
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::M_Fn_Fuzzy_Compile_Add_Entry
(
    vector<shared_ptr<C_Term> > const & ro_vector_spo_Term
)
{
    void* p_void = NULL;

    int b_Failed = false;
    
    LH_ERROR o_LH_ERROR; // = LH_OK;

    stringstream stringstream_Log;

    LH_GRM_TERMINAL o_LH_GRM_TERMINAL;
    
    LH_FUZZYMATCHCTXCPL_SPEAKABLEDATA* p_SpeakableData = NULL;

    LH_FUZZYMATCHCTXCPL_STATUS o_LH_FUZZYMATCHCTXCPL_STATUS; 

    vector<boost::shared_ptr<C_Term> >::size_type vector_spo_Term_Size = ro_vector_spo_Term.size();
    for (
        vector<boost::shared_ptr<C_Term> >::size_type vector_spo_Term_Index = 0;
        vector_spo_Term_Index < vector_spo_Term_Size;
        ++vector_spo_Term_Index
    ) {
        C_Term& o_Term_Entry_Data = *(ro_vector_spo_Term[vector_spo_Term_Index].get());

        IF (m_vector_i_Column_Attribute[vector_spo_Term_Index] & LH_FUZZYMATCHCTXCPL_COLUMN_INTEGER) {  // Add data to an integer column
            o_LH_ERROR = G_o_Vocon().M_Fn_LH_FuzzyMatchCtxCplSetIntegers(
                m_ro_Data_Party.m_ro_Engine_Concrete.m_o_LH_OBJECT_FmCtxCpl, 
                m_vector_i_Column_Id[vector_spo_Term_Index], 
                &(o_Term_Entry_Data.m_i_User_Data_Lo),
                1);
            IF (LH_OK != o_LH_ERROR) {
                Fn_Log_LH_ERROR(o_LH_ERROR);
                MACRO_Log_Error
                return E_Result_Error;
            }
        }
        ELSE IF (m_vector_i_Column_Attribute[vector_spo_Term_Index] & LH_FUZZYMATCHCTXCPL_COLUMN_SPEAKABLE) { // Add data to a speakable column
            p_void = m_ro_Data_Party.m_ro_Engine_Concrete.m_scpo_Heap->M_Fn_Malloc(
                sizeof(LH_FUZZYMATCHCTXCPL_SPEAKABLEDATA));
            IF (NULL == p_void) {
                MACRO_Log_Error
                return E_Result_Error;
            }
            p_SpeakableData = reinterpret_cast<LH_FUZZYMATCHCTXCPL_SPEAKABLEDATA*>(p_void);

            /* Get the transcriptions */
            o_LH_GRM_TERMINAL.szOrthography = o_Term_Entry_Data.m_string_Orthography.c_str();
            o_LH_ERROR = G_o_Vocon().M_Fn_LH_TransLookUpFetchTranscriptions (
                m_ro_Data_Party.m_ro_Engine_Concrete.m_o_LH_OBJECT_CLC,
                o_LH_GRM_TERMINAL.szOrthography,
                &(o_LH_GRM_TERMINAL.pTranscriptions),
                &(o_LH_GRM_TERMINAL.nbrTranscriptions));
            IF (LH_OK != o_LH_ERROR) {
                Fn_Log_LH_ERROR(o_LH_ERROR);
                MACRO_Log_Error
                return E_Result_Error;
            }

            IF (0 != o_LH_GRM_TERMINAL.nbrTranscriptions) {
                p_SpeakableData->szText = o_Term_Entry_Data.m_string_Orthography.c_str();
                p_SpeakableData->pTranscriptions = o_LH_GRM_TERMINAL.pTranscriptions;
                p_SpeakableData->nbrTranscriptions = o_LH_GRM_TERMINAL.nbrTranscriptions;
            }
            ELSE {
                Fn_Log("  No Transcription For : " + o_Term_Entry_Data.m_string_Orthography);
                b_Failed = true;
            }

            IF (!b_Failed) {
                o_LH_ERROR = G_o_Vocon().M_Fn_LH_FuzzyMatchCtxCplSetSpeakableStrings(
                    m_ro_Data_Party.m_ro_Engine_Concrete.m_o_LH_OBJECT_FmCtxCpl, 
                    m_vector_i_Column_Id[vector_spo_Term_Index], 
                    p_SpeakableData, 
                    1);
                IF (LH_OK != o_LH_ERROR) {
                    Fn_Log_LH_ERROR(o_LH_ERROR);
                    MACRO_Log_Error
                    return E_Result_Error;
                }
            }

            o_LH_ERROR = G_o_Vocon().M_Fn_LH_TransLookUpReturnTranscriptions (
                m_ro_Data_Party.m_ro_Engine_Concrete.m_o_LH_OBJECT_CLC,
                o_LH_GRM_TERMINAL.pTranscriptions);
            IF (LH_OK != o_LH_ERROR) {
                Fn_Log_LH_ERROR(o_LH_ERROR);
                MACRO_Log_Error
                return E_Result_Error;
            }

            m_ro_Data_Party.m_ro_Engine_Concrete.m_scpo_Heap->M_Fn_Free(p_void);
            p_void = NULL;
            p_SpeakableData = NULL;

            IF (b_Failed) {
                return E_Result_Succeeded;
            }
        }
        ELSE { /* Add data to a string column */
            p_void = m_ro_Data_Party.m_ro_Engine_Concrete.m_scpo_Heap->M_Fn_Calloc(
                0,
                o_Term_Entry_Data.m_string_Orthography.size() + 1);
            IF (NULL == p_void) {
                MACRO_Log_Error
                return E_Result_Error;
            }

            char* p_char = static_cast<char*>(p_void);
            memcpy(
                p_char, 
                o_Term_Entry_Data.m_string_Orthography.c_str(), 
                o_Term_Entry_Data.m_string_Orthography.size());
            
            o_LH_ERROR = G_o_Vocon().M_Fn_LH_FuzzyMatchCtxCplSetStrings(
                m_ro_Data_Party.m_ro_Engine_Concrete.m_o_LH_OBJECT_FmCtxCpl,
                m_vector_i_Column_Id[vector_spo_Term_Index], 
                const_cast<char const**>(&p_char), 
                1);
            IF (LH_OK != o_LH_ERROR) {
                Fn_Log_LH_ERROR(o_LH_ERROR);
                MACRO_Log_Error
                return E_Result_Error;
            }

            m_ro_Data_Party.m_ro_Engine_Concrete.m_scpo_Heap->M_Fn_Free(p_void);
            p_void = NULL;
            p_char = NULL;
        }
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_FuzzyMatchCtxCplCommitRow(
        m_ro_Data_Party.m_ro_Engine_Concrete.m_o_LH_OBJECT_FmCtxCpl, 
        &o_LH_FUZZYMATCHCTXCPL_STATUS);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (LH_FUZZYMATCHCTXCPL_STATUS_OK != o_LH_FUZZYMATCHCTXCPL_STATUS) {
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "LH_FUZZYMATCHCTXCPL_STATUS: " << static_cast<int>(o_LH_FUZZYMATCHCTXCPL_STATUS);
        Fn_Log(stringstream_Log.str());
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::M_Fn_Fuzzy_Compile_Run()
{
    MACRO_Log_Function

    int i_ret_value = 0;
    
    E_Result e_Result;
    
    LH_ERROR o_LH_ERROR; // = LH_OK;

    stringstream stringstream_Log;

    LH_FUZZYMATCHCTXCPL_STATUS o_LH_FUZZYMATCHCTXCPL_STATUS; 
    
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_FuzzyMatchCtxCplEndTable(
        m_ro_Data_Party.m_ro_Engine_Concrete.m_o_LH_OBJECT_FmCtxCpl, 
        &o_LH_FUZZYMATCHCTXCPL_STATUS);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (LH_FUZZYMATCHCTXCPL_STATUS_OK != o_LH_FUZZYMATCHCTXCPL_STATUS) {
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "LH_FUZZYMATCHCTXCPL_STATUS: " << static_cast<int>(o_LH_FUZZYMATCHCTXCPL_STATUS);
        Fn_Log(stringstream_Log.str());
        MACRO_Log_Error
        return E_Result_Error;
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_FuzzyMatchCtxCplRun(
        m_ro_Data_Party.m_ro_Engine_Concrete.m_o_LH_OBJECT_FmCtxCpl, 
        &o_LH_FUZZYMATCHCTXCPL_STATUS);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (LH_FUZZYMATCHCTXCPL_STATUS_OK != o_LH_FUZZYMATCHCTXCPL_STATUS) {
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "LH_FUZZYMATCHCTXCPL_STATUS: " << static_cast<int>(o_LH_FUZZYMATCHCTXCPL_STATUS);
        Fn_Log(stringstream_Log.str());
        MACRO_Log_Error
        return E_Result_Error;
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ObjClose(
        &(m_ro_Data_Party.m_ro_Engine_Concrete.m_o_LH_OBJECT_FmCtxCpl));
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_vector_i_Column_Attribute.clear();
    m_vector_i_Column_Id.clear();

    i_ret_value = Fn_File_Delete(m_string_Data_File_Context);
    IF (0 != i_ret_value) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    i_ret_value = Fn_File_Rename(m_string_File_Context_Fuzzy_Tmp, m_string_Data_File_Context);
    IF (0 != i_ret_value) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    e_Result = _M_Fn_Obj_Create_From_File();
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::M_Fn_On_Callback_Fuzzy_Compile_Error
(
    string const& ro_string_Error_Msg
)
{
    MACRO_Log_Function

    Fn_Log(ro_string_Error_Msg);
    MACRO_Log_Error
    
    return E_Result_Succeeded;
}

#endif

E_Result
C_Data_Context::M_Fn_File_Save(int b_File_Write, unsigned int& ri_File_Size)
{
    MACRO_Log_Function

    ST_ERROR o_ST_ERROR; // = ST_OK;
    LH_ERROR o_LH_ERROR; // = LH_OK;

    LH_OSTREAM_PARAMS o_LH_OSTREAM_PARAMS;
    LH_OSTREAM_INTERFACE o_LH_OSTREAM_INTERFACE;
    void * pvoid_Inst_Stream = NULL;

    C_File_Info o_File_Info;
    o_File_Info.m_b_File_Write = b_File_Write;
    pvoid_Inst_Stream = reinterpret_cast<void*>(&o_File_Info);

    o_ST_ERROR = G_o_Vocon().M_Fn_ST_CreateStreamWriterToFile_Crc(
        m_string_Data_File_Context.c_str(),
        &o_LH_OSTREAM_INTERFACE,
        o_File_Info
    );

    IF (ST_OK != o_ST_ERROR) {
        Fn_Log("    Failed to create stream-writer for file: " + m_string_Data_File_Context);
        MACRO_Log_Error
        return E_Result_Error;
    }
    ELSE {
        memset(&o_LH_OSTREAM_PARAMS, 0, sizeof(LH_OSTREAM_PARAMS));

        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ArchivableArchive(
            m_o_LH_OBJECT_Context,
            &o_LH_OSTREAM_INTERFACE,
            pvoid_Inst_Stream,
            &o_LH_OSTREAM_PARAMS
        );
        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Error;
        }
    }

    ri_File_Size = static_cast<unsigned int>(o_File_Info.m_i_File_Len);

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::M_Fn_File_Delete()
{
    MACRO_Log_Function

    Fn_Log("Context List File Delete: " + m_string_Data_File_Context);
    IF (!Fn_File_Access(m_string_Data_File_Context)) {
        Fn_Log("The file does not exist");
        return E_Result_Succeeded;
    }

    int i_ret = Fn_File_Delete(m_string_Data_File_Context);
    IF (0 != i_ret) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::_M_Fn_Obj_Create_From_File()
{
    MACRO_Log_Function

    ST_ERROR o_ST_ERROR; // = ST_OK;
    LH_ERROR o_LH_ERROR; // = LH_OK;

    string string_File_Context;

    IF (m_b_Obj_Created) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (!G_o_Vocon().M_Fn_LH_ObjIsNull(m_o_LH_OBJECT_Context)) {
        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ObjClose(&m_o_LH_OBJECT_Context);
        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            m_o_LH_OBJECT_Context = G_o_Vocon().M_Fn_LH_GetNullObj();
        }
    }

    LH_ISTREAM_INTERFACE o_LH_ISTREAM_INTERFACE;
    void * pvoid_Inst_Stream = NULL;
    boost::function<void(C_Event_Init_Notify const &)>& o_function_On_Event_Init_Notify =
        m_ro_Data_Party.m_ro_Engine_Concrete.m_function_On_Event_Init_Notify;

    IF (Fn_File_Access(m_string_Data_File_Context)) {
        string_File_Context = m_string_Data_File_Context;
    }
    ELSE {
        IF (("list" == m_string_Type_Context)
            || ("fuzzy" == m_string_Type_Context)) {

            IF (o_function_On_Event_Init_Notify) {
                o_function_On_Event_Init_Notify(
                    C_Event_Init_Notify_Impl(
                        C_Event_Init_Notify::E_Event_Init_Notify_Type_Context_Not_Exist,
                        m_string_Id_Context
                    )
                );
            }
        }
        ELSE {
            MACRO_Log_Error
        }
        return E_Result_Error;
    }

    Fn_Log("    Fn_ST_CreateStreamReaderFromFile: " + string_File_Context);
    IF (("list" == m_string_Type_Context)
        || ("fuzzy" == m_string_Type_Context)) {

        bool b_Context_Ok = false;
        (void)m_ro_Data_Party.m_ro_Engine_Concrete.M_Fn_AcMod_Check_Dependency(string_File_Context, b_Context_Ok);
        IF (!b_Context_Ok) {
            IF (o_function_On_Event_Init_Notify) {
                o_function_On_Event_Init_Notify(
                    C_Event_Init_Notify_Impl(
                        C_Event_Init_Notify::E_Event_Init_Notify_Type_Context_Invalid,
                        m_string_Id_Context
                    )
                );
            }
            Fn_File_Delete(string_File_Context);
            return E_Result_Error;
        }

        o_ST_ERROR = G_o_Vocon().M_Fn_ST_CreateStreamReaderFromFile_Crc(
            string_File_Context.c_str(),
            &o_LH_ISTREAM_INTERFACE,
            &pvoid_Inst_Stream
        );
        IF (ST_OK != o_ST_ERROR) {
            Fn_Log("    Failed to create stream-reader for file: " + string_File_Context);
            return E_Result_Error;
        }
    }
    ELSE {
        o_ST_ERROR = G_o_Vocon().M_Fn_ST_CreateStreamReaderFromFile(
            string_File_Context.c_str(),
            &o_LH_ISTREAM_INTERFACE,
            &pvoid_Inst_Stream
        );

        IF (ST_OK != o_ST_ERROR) {
            Fn_Log("    Path: " + string_File_Context);
            Fn_Log_ST_ERROR(o_ST_ERROR);
            MACRO_Log_Error
            return E_Result_Error;
        }
    }
    Fn_Log("    Fn_ST_CreateStreamReaderFromFile Finish");
    

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_CreateCtx(
        m_ro_Data_Party.m_ro_Engine_Concrete.M_Fn_Get_ro_LH_COMPONENT_Asr(),
        &o_LH_ISTREAM_INTERFACE,
        pvoid_Inst_Stream,
        NULL,
        m_ro_Data_Party.m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Mod(),
        &m_o_LH_OBJECT_Context
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    Fn_Log("    G_o_Vocon().M_Fn_LH_CreateCtx Finish");

#ifdef MACRO_Feature_Reserved

    IF (("fuzzy_host" == m_string_Type_Context)
        || ("fuzzy" == m_string_Type_Context)) {
        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ConfigSetParam(
            m_o_LH_OBJECT_Context,
            LH_CTX_PARAM_TSILENCE_FX,
            E_Param_Value_Default_Fx_FuzzyCtx_TSilence
        );

        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Failed;
        }

        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ConfigSetParam(
            m_o_LH_OBJECT_Context,
            LH_CTX_PARAM_TSILENCE,
            0
        );

        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Failed;
        }
    }

    IF ("fuzzy" == m_string_Type_Context) {
        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ConfigSetParam(
            m_o_LH_OBJECT_Context,
            LH_TABLECTX_PARAM_MAX_COLLAPSED_LISTSIZE,
            E_Param_Value_Default_TableCtx_Max_Collapsed_List_Size
        );

        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Failed;
        }

        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ConfigSetParam(
            m_o_LH_OBJECT_Context,
            LH_TABLECTX_PARAM_MAX_NBR_UNCOLLAPSED_ENTRIES,
            E_Param_Value_Default_TableCtx_Nbr_Uncollapsed_Entries
        );

        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Failed;
        }
    }

#endif

    m_b_Obj_Created = true;

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::_M_Fn_Obj_Create_List()
{
    MACRO_Log_Function

    LH_ERROR o_LH_ERROR; // = LH_OK;

    IF (m_b_Obj_Created) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (!G_o_Vocon().M_Fn_LH_ObjIsNull(m_o_LH_OBJECT_Context)) {
        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ObjClose(&m_o_LH_OBJECT_Context);
        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            m_o_LH_OBJECT_Context = G_o_Vocon().M_Fn_LH_GetNullObj();
        }
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_CreateEmptyListCtx(
        m_ro_Data_Party.m_ro_Engine_Concrete.M_Fn_Get_ro_LH_COMPONENT_Asr(),
        m_ro_Data_Party.m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Mod(),
        LH_LISTCTX_KEEP_STRINGS_KEEP_IDS,
        LH_USERID_RANGE_64,
        &m_o_LH_OBJECT_Context
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_b_Obj_Created = true;

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ListCtxCommit(
        m_o_LH_OBJECT_Context
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::_M_Fn_Obj_Create_Userword()
{
    MACRO_Log_Function

    LH_ERROR o_LH_ERROR; // = LH_OK;

    IF (m_b_Obj_Created) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (!G_o_Vocon().M_Fn_LH_ObjIsNull(m_o_LH_OBJECT_Context)) {
        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ObjClose(&m_o_LH_OBJECT_Context);
        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            m_o_LH_OBJECT_Context = G_o_Vocon().M_Fn_LH_GetNullObj();
        }
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_CreateUswCtx(
        m_ro_Data_Party.m_ro_Engine_Concrete.M_Fn_Get_ro_LH_COMPONENT_Asr(),
        m_ro_Data_Party.m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Mod(),
        &m_o_LH_OBJECT_Context
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_b_Obj_Created = true;

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::_M_Fn_Obj_Destroy()
{
    MACRO_Log_Function

    LH_ERROR o_LH_ERROR; // = LH_OK;

    IF (!m_b_Obj_Created) {
        return E_Result_Succeeded;
    }

    IF (!G_o_Vocon().M_Fn_LH_ObjIsNull(m_o_LH_OBJECT_Context)) {
        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ObjClose(&m_o_LH_OBJECT_Context);
        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            m_o_LH_OBJECT_Context = G_o_Vocon().M_Fn_LH_GetNullObj();
        }
    }

    m_b_Obj_Created = false;

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::_M_Fn_Obj_Slot_Host_Add()
{
    MACRO_Log_Function

    E_Result e_Result;

    LH_ERROR o_LH_ERROR; // = LH_OK;

    LH_HOSTCTX_STATUS o_LH_HOSTCTX_STATUS; 

    string string_Slot_Host_Add_Log;

    list<pair<string, string> >::iterator iterator_pair_string_Id_Slot_string_Id_Context_End = m_list_pair_string_Id_Slot_string_Id_Context.end();

    for (
        list<pair<string, string> >::iterator iterator_pair_string_Id_Slot_string_Id_Context_Curr = m_list_pair_string_Id_Slot_string_Id_Context.begin();
        iterator_pair_string_Id_Slot_string_Id_Context_Curr != iterator_pair_string_Id_Slot_string_Id_Context_End;
        ++iterator_pair_string_Id_Slot_string_Id_Context_Curr
    ) {
        string & rstring_Id_Slot = iterator_pair_string_Id_Slot_string_Id_Context_Curr->first;
        string & rstring_Id_Context = iterator_pair_string_Id_Slot_string_Id_Context_Curr->second;

        string_Slot_Host_Add_Log = "    Slot Host Add, Slot Id: " + rstring_Id_Slot;
        Fn_Log(string_Slot_Host_Add_Log);
        string_Slot_Host_Add_Log = "    Slot Host Add, Context Id: " + rstring_Id_Context;
        Fn_Log(string_Slot_Host_Add_Log);
        
        C_Data_Context * po_Data_Context = m_ro_Data_Party.M_Fn_Find_po_Data_Context(rstring_Id_Context);
        IF (!po_Data_Context) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        IF (!po_Data_Context->M_Fn_Is_Ready()) {
            continue;
        }

        e_Result = po_Data_Context->M_Fn_Slot_Guest_Load();
        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }
#if 0        
        std::string context_tmp = "<ude#artist>:<artist>,<ude#album>:<album>,<ude#song>:<song>";
        o_LH_ERROR = G_o_Vocon().M_Fn_LH_HostCtxSetCtx2(
            m_o_LH_OBJECT_Context,
            context_tmp.c_str(),
            po_Data_Context->m_o_LH_OBJECT_Context,
            &o_LH_HOSTCTX_STATUS
            );
#endif

        o_LH_ERROR = G_o_Vocon().M_Fn_LH_HostCtxSetCtx2(
            m_o_LH_OBJECT_Context,
            rstring_Id_Slot.c_str(),
            po_Data_Context->m_o_LH_OBJECT_Context,
            &o_LH_HOSTCTX_STATUS
        );

        IF (LH_HOSTCTX_STATUS_OK  != o_LH_HOSTCTX_STATUS) {
            Fn_Log("Failed to set guest context on the host! Slot Id: " + rstring_Id_Slot);
            MACRO_Log_Error
            return E_Result_Error;
        }

        po_Data_Context->m_b_Slot_Added = true;
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::_M_Fn_Obj_Slot_Host_Clear()
{
    MACRO_Log_Function

    E_Result e_Result;

    LH_ERROR o_LH_ERROR; // = LH_OK;

    list<pair<string, string> >::iterator iterator_pair_string_Id_Slot_string_Id_Context_End = m_list_pair_string_Id_Slot_string_Id_Context.end();

    for (
        list<pair<string, string> >::iterator iterator_pair_string_Id_Slot_string_Id_Context_Curr = m_list_pair_string_Id_Slot_string_Id_Context.begin();
        iterator_pair_string_Id_Slot_string_Id_Context_Curr != iterator_pair_string_Id_Slot_string_Id_Context_End;
        ++iterator_pair_string_Id_Slot_string_Id_Context_Curr
    ) {
        string & rstring_Id_Slot = iterator_pair_string_Id_Slot_string_Id_Context_Curr->first;
        string & rstring_Id_Context = iterator_pair_string_Id_Slot_string_Id_Context_Curr->second;

        C_Data_Context * po_Data_Context = m_ro_Data_Party.M_Fn_Find_po_Data_Context(rstring_Id_Context);
        IF (!po_Data_Context) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        IF (po_Data_Context->m_b_Slot_Added) {
            po_Data_Context->m_b_Slot_Added = false;
            o_LH_ERROR = G_o_Vocon().M_Fn_LH_HostCtxClearCtx(
                m_o_LH_OBJECT_Context,
                rstring_Id_Slot.c_str()
            );

            IF (LH_OK != o_LH_ERROR) {
                Fn_Log_LH_ERROR(o_LH_ERROR);
                MACRO_Log_Error
                return E_Result_Error;
            }
        }

        e_Result = po_Data_Context->M_Fn_Slot_Guest_Unload();
        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::_M_Fn_Get_Obj_Activatable_Rules
(
    set<string>& ro_set_string_Activatable_Rules
)
{
    MACRO_Log_Function

    LH_ERROR o_LH_ERROR; // = LH_OK;
    const char** ppc_Rule = NULL;
    size_t i_Rule_Count = 0;

    ro_set_string_Activatable_Rules.clear();

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ActivatableFetchRules(
        m_o_LH_OBJECT_Context,
        LH_RULETYPE_ACTIVATABLE,
        &ppc_Rule,
        &i_Rule_Count);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    for (size_t i_Rule_Index = 0; i_Rule_Index < i_Rule_Count; ++i_Rule_Index) {
        const char* pc_Rule = *(ppc_Rule + i_Rule_Index);
        ro_set_string_Activatable_Rules.insert(string(pc_Rule));
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ActivatableReturnRules(
        m_o_LH_OBJECT_Context,
        ppc_Rule);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::_M_Fn_Obj_Activate_Grammar
(
    list<shared_ptr<C_Request_Activate> > & rlist_spo_Request_Activate
)
{
    MACRO_Log_Function

    LH_ERROR o_LH_ERROR; // = LH_OK;

    int b_First = true;
    int b_Deactivate_all_then_activate = false;
    string string_Activate_Grammar_Log;

    set<string> o_set_string_Activatable_Rules;
    E_Result e_Result = _M_Fn_Get_Obj_Activatable_Rules(o_set_string_Activatable_Rules);
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    list<shared_ptr<C_Request_Activate> >::iterator iterator_spo_Request_Activate_End = rlist_spo_Request_Activate.end();

    for (
        list<shared_ptr<C_Request_Activate> >::iterator iterator_spo_Request_Activate_Curr = rlist_spo_Request_Activate.begin();
        iterator_spo_Request_Activate_Curr != iterator_spo_Request_Activate_End;
        ++iterator_spo_Request_Activate_Curr
    ) {

        shared_ptr<C_Request_Activate> & rspo_Request_Activate = *iterator_spo_Request_Activate_Curr;
        IF (!rspo_Request_Activate) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        string_Activate_Grammar_Log = "    Activate Grammar, Context: " + rspo_Request_Activate->m_string_Id_Context;
        Fn_Log(string_Activate_Grammar_Log);
        IF (m_string_Id_Context != rspo_Request_Activate->m_string_Id_Context) {
            string_Activate_Grammar_Log = "    Activate Grammar, Target Context: " + m_string_Id_Context;
            Fn_Log(string_Activate_Grammar_Log);
            continue;
        }

        IF (b_First) {

            b_First = false;

            b_Deactivate_all_then_activate = rspo_Request_Activate->m_b_Deactivate_all_then_activate;

            IF (b_Deactivate_all_then_activate) {

                string_Activate_Grammar_Log = "    Activate Grammar, Deactivate All Rules";
                Fn_Log(string_Activate_Grammar_Log);

                o_LH_ERROR = G_o_Vocon().M_Fn_LH_ActivatableDeactivateAllRules(
                    m_o_LH_OBJECT_Context
                );

                IF (LH_OK != o_LH_ERROR) {
                    Fn_Log_LH_ERROR(o_LH_ERROR);
                    MACRO_Log_Error
                    return E_Result_Error;
                }
            }
        }
        ELSE {
            IF (b_Deactivate_all_then_activate != rspo_Request_Activate->m_b_Deactivate_all_then_activate) {
                MACRO_Log_Error
                return E_Result_Error;
            }
        }

        list<std::string>::iterator iterator_string_Id_Rule_End = rspo_Request_Activate->m_list_string_Id_Rule.end();

        for (
            list<std::string>::iterator iterator_string_Id_Rule_Curr = rspo_Request_Activate->m_list_string_Id_Rule.begin();
            iterator_string_Id_Rule_Curr != iterator_string_Id_Rule_End;
            ++iterator_string_Id_Rule_Curr
        ) {
            IF (0 == o_set_string_Activatable_Rules.count(*iterator_string_Id_Rule_Curr)) {
                string_Activate_Grammar_Log = "    Activate Grammar, No Rule: " + *iterator_string_Id_Rule_Curr;
                Fn_Log(string_Activate_Grammar_Log);
                continue;
            }

            IF (b_Deactivate_all_then_activate) {

                string_Activate_Grammar_Log = "    Activate Grammar, Activate Rule: " + *iterator_string_Id_Rule_Curr;
                Fn_Log(string_Activate_Grammar_Log);

                o_LH_ERROR = G_o_Vocon().M_Fn_LH_ActivatableActivateRule(
                    m_o_LH_OBJECT_Context,
                    iterator_string_Id_Rule_Curr->c_str()
                );
            }
            ELSE {

                string_Activate_Grammar_Log = "    Activate Grammar, Deactivate Rule: " + *iterator_string_Id_Rule_Curr;
                Fn_Log(string_Activate_Grammar_Log);

                o_LH_ERROR = G_o_Vocon().M_Fn_LH_ActivatableDeactivateRule(
                    m_o_LH_OBJECT_Context,
                    iterator_string_Id_Rule_Curr->c_str()
                );
            }

            IF (LH_OK != o_LH_ERROR) {
                Fn_Log_LH_ERROR(o_LH_ERROR);
                MACRO_Log_Error
                return E_Result_Error;
            }
        }
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::_M_Fn_Obj_Activate_Field
(
    list<shared_ptr<C_Request_Activate> > & rlist_spo_Request_Activate
)
{
    MACRO_Log_Function

#ifdef MACRO_Feature_Reserved

    LH_ERROR o_LH_ERROR; // = LH_OK;

    int b_First = true;
    string string_Activate_Field_Log;

    list<shared_ptr<C_Request_Activate> >::iterator iterator_spo_Request_Activate_End = rlist_spo_Request_Activate.end();

    for (
        list<shared_ptr<C_Request_Activate> >::iterator iterator_spo_Request_Activate_Curr = rlist_spo_Request_Activate.begin();
        iterator_spo_Request_Activate_Curr != iterator_spo_Request_Activate_End;
        ++iterator_spo_Request_Activate_Curr
    ) {
        shared_ptr<C_Request_Activate>& rspo_Request_Activate = *iterator_spo_Request_Activate_Curr;
        IF (!rspo_Request_Activate) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        string_Activate_Field_Log = "    Activate Field, Context: " + rspo_Request_Activate->m_string_Id_Context;
        Fn_Log(string_Activate_Field_Log);
        IF (m_string_Id_Context != rspo_Request_Activate->m_string_Id_Context) {
            string_Activate_Field_Log = "    Activate Field, Target Context: " + m_string_Id_Context;
            Fn_Log(string_Activate_Field_Log);
            continue;
        }

        string_Activate_Field_Log = "    Activate Field, Start: " + rspo_Request_Activate->m_string_Field_Start;
        Fn_Log(string_Activate_Field_Log);
        string_Activate_Field_Log = "    Activate Field, Stop: " + rspo_Request_Activate->m_string_Field_Stop;
        Fn_Log(string_Activate_Field_Log);
        string_Activate_Field_Log = "    Activate Field, Activate From: " + rspo_Request_Activate->m_string_Field_Activate_From;
        Fn_Log(string_Activate_Field_Log);

        IF (!b_First) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        b_First = false;

        IF (rspo_Request_Activate->m_vector_i_User_Data_Lo.empty()) {
            o_LH_ERROR = G_o_Vocon().M_Fn_LH_FieldCtxSetStartStop(
                m_o_LH_OBJECT_Context, 
                rspo_Request_Activate->m_string_Field_Start.c_str(),
                rspo_Request_Activate->m_string_Field_Stop.c_str(),
                LH_FIELDCTX_ACTIVATEALL
                );

            IF (LH_OK != o_LH_ERROR) {
                Fn_Log_LH_ERROR(o_LH_ERROR);
                MACRO_Log_Error
                return E_Result_Error;
            }

            continue;
        }
        ELSE {
            o_LH_ERROR = G_o_Vocon().M_Fn_LH_FieldCtxSetStartStop(
                m_o_LH_OBJECT_Context, 
                rspo_Request_Activate->m_string_Field_Start.c_str(),
                rspo_Request_Activate->m_string_Field_Stop.c_str(),
                LH_FIELDCTX_DEACTIVATEALL
                );

            IF (LH_OK != o_LH_ERROR) {
                Fn_Log_LH_ERROR(o_LH_ERROR);
                MACRO_Log_Error
                return E_Result_Error;
            }
        }

        vector<LH_USERID64> vector_o_LH_USERID64;

        for (
            size_t i_Pos = 0;
            true;
            ++i_Pos
        ) {
            int b_End = true;

            LH_USERID64 o_LH_USERID64;

            o_LH_USERID64.hi32 = 0;

            IF (i_Pos < rspo_Request_Activate->m_vector_i_User_Data_Hi.size()) {
                o_LH_USERID64.hi32 = rspo_Request_Activate->m_vector_i_User_Data_Hi[i_Pos];
                b_End = false;
            }

            o_LH_USERID64.lo32 = 0;

            IF (i_Pos < rspo_Request_Activate->m_vector_i_User_Data_Lo.size()) {
                o_LH_USERID64.lo32 = rspo_Request_Activate->m_vector_i_User_Data_Lo[i_Pos];
                b_End = false;
            }

            IF (b_End) {
                break;
            }

            vector_o_LH_USERID64.push_back(o_LH_USERID64);
        }

        o_LH_ERROR = G_o_Vocon().M_Fn_LH_FieldCtxActivateUserId(
            m_o_LH_OBJECT_Context, 
            rspo_Request_Activate->m_string_Field_Start.c_str(),
            rspo_Request_Activate->m_string_Field_Activate_From.c_str(),
            &vector_o_LH_USERID64[0],
            vector_o_LH_USERID64.size()
        );

        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Error;
        }
    }

#endif

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::_M_Fn_Obj_Activate_Fuzzy
(
    list<shared_ptr<C_Request_Activate> > & rlist_spo_Request_Activate
)
{
    MACRO_Log_Function

#ifdef MACRO_Feature_Reserved

    LH_ERROR o_LH_ERROR; // = LH_OK;

    stringstream stringstream_Log;

    char const* p_c_Query = NULL;

    unsigned long i_Activation_Value = 0;

    C_Request_Activate* po_Request_Activate = NULL;

    LH_TABLECTX_ACTIVATESTATUS o_LH_TABLECTX_ACTIVATESTATUS;

    string string_Activate_Fuzzy_Log;

    list<shared_ptr<C_Request_Activate> >::iterator iterator_spo_Request_Activate_End = rlist_spo_Request_Activate.end();

    for (
        list<shared_ptr<C_Request_Activate> >::iterator iterator_spo_Request_Activate_Curr = rlist_spo_Request_Activate.begin();
        iterator_spo_Request_Activate_Curr != iterator_spo_Request_Activate_End;
        ++iterator_spo_Request_Activate_Curr
    ) {
        po_Request_Activate = (*iterator_spo_Request_Activate_Curr).get();
        IF (NULL == po_Request_Activate) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        string_Activate_Fuzzy_Log = "    Activate Fuzzy, Context: " + po_Request_Activate->m_string_Id_Context;
        Fn_Log(string_Activate_Fuzzy_Log);
        IF (m_string_Id_Context != po_Request_Activate->m_string_Id_Context) {
            string_Activate_Fuzzy_Log = "    Activate Fuzzy, Target Context: " + m_string_Id_Context;
            Fn_Log(string_Activate_Fuzzy_Log);
            continue;
        }

        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "    Activate Fuzzy, Value: " << po_Request_Activate->m_i_Value;
        Fn_Log(stringstream_Log.str());
        string_Activate_Fuzzy_Log = "    Activate Fuzzy, Query: " + po_Request_Activate->m_string_Query;
        Fn_Log(string_Activate_Fuzzy_Log);
        
        i_Activation_Value = po_Request_Activate->m_i_Value;
        IF (i_Activation_Value > 10000) {
            MACRO_Log_Error
            return E_Result_Error;
        }
        
        p_c_Query = po_Request_Activate->m_string_Query.c_str();
        IF (po_Request_Activate->m_string_Query.empty()) {
            p_c_Query = NULL;
        }
        
        o_LH_ERROR = G_o_Vocon().M_Fn_LH_TableCtxActivate(
            m_o_LH_OBJECT_Context,
            p_c_Query,
            i_Activation_Value,
            &o_LH_TABLECTX_ACTIVATESTATUS);
        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Error;
        }
        
        IF (LH_TABLECTX_ACTIVATESTATUS_OK != o_LH_TABLECTX_ACTIVATESTATUS) {
            stringstream_Log.clear();
            stringstream_Log.str("");
            stringstream_Log << "LH_TABLECTX_ACTIVATESTATUS: " << static_cast<int>(o_LH_TABLECTX_ACTIVATESTATUS);
            Fn_Log(stringstream_Log.str());
            MACRO_Log_Error
            return E_Result_Error;
        }
    }

#endif

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::_M_Fn_List_Update_Add_Transcriptions
(
    const string& ro_string_Orthography,
    unsigned int i_User_Id_Hi,
    unsigned int i_User_Id_Lo,
    LH_TRANSCRIPTION * p_LH_TRANSCRIPTIONs,
    size_t i_LH_TRANSCRIPTIONs_count
)
{
    string string_Orthography = ro_string_Orthography;
    IF (string_Orthography.empty()) {
        string_Orthography = " ";
    }

    IF (!Fn_Is_Utf8_String(string_Orthography)) {
        Fn_Print_Bin_C_String(string_Orthography.c_str());
        return E_Result_Succeeded;
    }

    stringstream stringstream_Log;
    IF (0 == i_LH_TRANSCRIPTIONs_count) {
        stringstream_Log << string_Orthography
                         << " have no transcription";
        Fn_Log(stringstream_Log.str());
        return E_Result_Succeeded;
    }

    LH_GRM_TERMINAL o_LH_GRM_TERMINAL;
    // o_LH_GRM_TERMINAL.type = LH_GRM_TERMINAL_TYPE_WORD; // vocon_4.11
    o_LH_GRM_TERMINAL.szOrthography = string_Orthography.c_str();
    o_LH_GRM_TERMINAL.userID.hi32 = i_User_Id_Hi;
    o_LH_GRM_TERMINAL.userID.lo32 = i_User_Id_Lo;
    o_LH_GRM_TERMINAL.pTranscriptions = p_LH_TRANSCRIPTIONs;
    o_LH_GRM_TERMINAL.nbrTranscriptions = i_LH_TRANSCRIPTIONs_count;

    LH_ERROR o_LH_ERROR;
    LH_TERMSEQ_STATUS o_LH_TERMSEQ_STATUS;
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ListCtxAddTermSeq(
        m_o_LH_OBJECT_Context,
        &o_LH_GRM_TERMINAL,
        1,
        &o_LH_TERMSEQ_STATUS
    );
    IF (LH_OK != o_LH_ERROR) {
        MACRO_Log_Error
        Fn_Log_LH_ERROR(o_LH_ERROR);
        stringstream_Log << "  Add Error: " << o_LH_GRM_TERMINAL.szOrthography;
        Fn_Log(stringstream_Log.str());
        return E_Result_Error;
    }

    IF (!o_LH_TERMSEQ_STATUS.added) {
        stringstream_Log << "  Add Failed: " << o_LH_GRM_TERMINAL.szOrthography;
        Fn_Log(stringstream_Log.str());
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::_M_Fn_List_Update_Add_Userword_Transcriptions
(
    const C_Term& o_Term
)
{
    scoped_array<LH_TRANSCRIPTION> sca_Transcriptions;

    vector<C_Buffer>::size_type Buffer_Transcriptions_Count = o_Term.m_vector_buffer_Transcriptions.size();
    IF (0 != Buffer_Transcriptions_Count) {
        sca_Transcriptions.reset(MACRO_New LH_TRANSCRIPTION[Buffer_Transcriptions_Count]);

        for (
            vector<C_Buffer>::size_type Buffer_Transcriptions_Index = 0;
            Buffer_Transcriptions_Index != Buffer_Transcriptions_Count;
            ++Buffer_Transcriptions_Index
        ) {
            const C_Buffer& tmp_Buffer = o_Term.m_vector_buffer_Transcriptions[Buffer_Transcriptions_Index];
            sca_Transcriptions[Buffer_Transcriptions_Index].type = LH_TRANSTYPE_USERWORD;
            sca_Transcriptions[Buffer_Transcriptions_Index].pData = tmp_Buffer.m_sai_Buffer.get();
            sca_Transcriptions[Buffer_Transcriptions_Index].size = tmp_Buffer.m_i_Size;
        }
    }

    IF (o_Term.m_vector_string_Orthography.empty()) {
        Fn_Log("the user word orthography vector is empty!");
        return E_Result_Succeeded;
    }

    E_Result e_Result = _M_Fn_List_Update_Add_Transcriptions(
        o_Term.m_vector_string_Orthography[0],
        o_Term.m_i_User_Data_Hi,
        o_Term.m_i_User_Data_Lo,
        sca_Transcriptions.get(),
        Buffer_Transcriptions_Count);
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Context::_M_Fn_List_Update_Add_Gracenote_And_Text_Transcriptions
(
    const C_Term& o_Term,
    C_Data_CLC& ro_Data_CLC
)
{
    E_Result e_Result;
    int b_Error = false;

    IF (o_Term.m_vector_string_Orthography.empty()) {
        Fn_Log("the orthography vector is empty!");
        return E_Result_Succeeded;
    }

    size_t i_Transcriptions_Count = 0;
    scoped_array<LH_TRANSCRIPTION> sca_Transcriptions;
    vector<pair<LH_TRANSCRIPTION*, size_t> > o_vec_pair_Text_Transcriptions;
    vector<pair<LH_TRANSCRIPTION*, size_t> > o_vec_pair_Gracenote_Transcriptions;
    C_Data_Transcription o_Data_Transcription;

    e_Result = o_Data_Transcription.M_Fn_Fetch_Gracenote_And_Text_Transcriptions(
        o_Term,
        ro_Data_CLC,
        o_vec_pair_Text_Transcriptions,
        o_vec_pair_Gracenote_Transcriptions,
        sca_Transcriptions,
        i_Transcriptions_Count);
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    e_Result = _M_Fn_List_Update_Add_Transcriptions(
        o_Term.m_vector_string_Orthography[0],
        o_Term.m_i_User_Data_Hi,
        o_Term.m_i_User_Data_Lo,
        sca_Transcriptions.get(),
        i_Transcriptions_Count
    );
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        b_Error = true;
    }

    e_Result = o_Data_Transcription.M_Fn_Return_Gracenote_And_Text_Transcriptions(
        o_Term,
        ro_Data_CLC,
        o_vec_pair_Text_Transcriptions,
        o_vec_pair_Gracenote_Transcriptions);
    IF (E_Result_Succeeded != e_Result) {
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
