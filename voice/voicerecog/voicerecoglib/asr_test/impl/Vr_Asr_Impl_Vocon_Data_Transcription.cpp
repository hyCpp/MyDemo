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
#include "Vr_Asr_Term.h"
#include "Vr_Asr_Buffer.h"
#include "Vr_Asr_Impl_Fn_string.h"
#include "Vr_Asr_Impl_Vocon_Data_CLC.h"
#include "Vr_Asr_Impl_Vocon_Data_Transcription.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

C_Data_Transcription::~C_Data_Transcription()
{
}

C_Data_Transcription::C_Data_Transcription()
{
}

E_Result
C_Data_Transcription::M_Fn_Get_Transcription
(
    const C_Term& o_Term,
    C_Data_CLC& ro_Data_CLC,
    C_Buffer& o_Buffer_Transcriptions
)
{
    E_Result e_Result;

    size_t i_Transcriptions_Count = 0;
    scoped_array<LH_TRANSCRIPTION> o_sca_Transcriptions;
    vector<pair<LH_TRANSCRIPTION*, size_t> > o_vec_pair_Text_Transcriptions;
    vector<pair<LH_TRANSCRIPTION*, size_t> > o_vec_pair_Gracenote_Transcriptions;

    e_Result = M_Fn_Fetch_Gracenote_And_Text_Transcriptions(
        o_Term,
        ro_Data_CLC,
        o_vec_pair_Text_Transcriptions,
        o_vec_pair_Gracenote_Transcriptions,
        o_sca_Transcriptions,
        i_Transcriptions_Count);
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    int b_Error = false;
    IF (0 != i_Transcriptions_Count) {
        LH_TRANSCRIPTION* po_LH_TRANSCRIPTIONs = o_sca_Transcriptions.get();

        o_Buffer_Transcriptions.m_sai_Buffer.reset(MACRO_New unsigned char[po_LH_TRANSCRIPTIONs->size]);
        IF (o_Buffer_Transcriptions.m_sai_Buffer) {
            memcpy(
                o_Buffer_Transcriptions.m_sai_Buffer.get(),
                po_LH_TRANSCRIPTIONs->pData,
                po_LH_TRANSCRIPTIONs->size);

            o_Buffer_Transcriptions.m_i_Size = po_LH_TRANSCRIPTIONs->size;
        }
        ELSE {
            MACRO_Log_Error
            b_Error = true;
        }
    }

    e_Result = M_Fn_Return_Gracenote_And_Text_Transcriptions(
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

E_Result
C_Data_Transcription::M_Fn_Fetch_Gracenote_And_Text_Transcriptions
(
    const C_Term& o_Term,
    C_Data_CLC& ro_Data_CLC,
    vector<pair<LH_TRANSCRIPTION*, size_t> >& ro_vec_pair_Text_Transcriptions,
    vector<pair<LH_TRANSCRIPTION*, size_t> >& ro_vec_pair_Gracenote_Transcriptions,
    scoped_array<LH_TRANSCRIPTION>& ro_sca_Transcriptions,
    size_t& ri_Transcriptions_Count
)
{
    E_Result e_Result;
    size_t i_Gracenote_Transcriptions_Count = 0;
    IF (o_Term.m_b_Use_Id_Trans) {
        int b_Support_Id_Trans_Lookup = ro_Data_CLC.M_Fn_Get_b_Id_Trans_Lookup();
        IF (!b_Support_Id_Trans_Lookup) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        LH_OBJECT& ro_LH_OBJECT_CLC_Id_Trans = ro_Data_CLC.M_Fn_Get_ro_LH_OBJECT_CLC_Id_Trans();
        IF (G_o_Vocon().M_Fn_LH_ObjIsNull(ro_LH_OBJECT_CLC_Id_Trans)) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        e_Result = _M_Fn_Fetch_Gracenote_Transcriptions(
            o_Term,
            ro_LH_OBJECT_CLC_Id_Trans,
            ro_vec_pair_Gracenote_Transcriptions,
            i_Gracenote_Transcriptions_Count);
        IF (E_Result_Succeeded != e_Result) {
            MACRO_Log_Error
            return E_Result_Error;
        }
    }

    size_t i_Text_Transcriptions_Count = 0;
    LH_OBJECT& ro_LH_OBJECT_CLC_Lex = ro_Data_CLC.M_Fn_Get_ro_LH_OBJECT_CLC_LEX();
    IF (G_o_Vocon().M_Fn_LH_ObjIsNull(ro_LH_OBJECT_CLC_Lex)) {
        MACRO_Log_Error
        return E_Result_Error;
    }
    e_Result = _M_Fn_Fetch_Text_Transcriptions(
        o_Term,
        ro_LH_OBJECT_CLC_Lex,
        ro_vec_pair_Text_Transcriptions,
        i_Text_Transcriptions_Count);
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    ri_Transcriptions_Count = i_Gracenote_Transcriptions_Count + i_Text_Transcriptions_Count;
    IF (0 == ri_Transcriptions_Count) {
        return E_Result_Succeeded;
    }

    ro_sca_Transcriptions.reset(MACRO_New LH_TRANSCRIPTION[ri_Transcriptions_Count]);
    IF (!ro_sca_Transcriptions) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    size_t i_Transcriptions_Index = 0;
    _M_Fn_Get_Gracenote_And_Text_Transcriptions(
        ro_vec_pair_Gracenote_Transcriptions,
        i_Transcriptions_Index,
        ro_sca_Transcriptions);

    _M_Fn_Get_Gracenote_And_Text_Transcriptions(
        ro_vec_pair_Text_Transcriptions,
        i_Transcriptions_Index,
        ro_sca_Transcriptions);

    return E_Result_Succeeded;
}

E_Result
C_Data_Transcription::M_Fn_Return_Gracenote_And_Text_Transcriptions
(
    const C_Term& o_Term,
    C_Data_CLC& ro_Data_CLC,
    vector<pair<LH_TRANSCRIPTION*, size_t> >& ro_vec_pair_Text_Transcriptions,
    vector<pair<LH_TRANSCRIPTION*, size_t> >& ro_vec_pair_Gracenote_Transcriptions
)
{
    E_Result e_Result;
    IF (o_Term.m_b_Use_Id_Trans) {
        LH_OBJECT& ro_LH_OBJECT_CLC_Id_Trans = ro_Data_CLC.M_Fn_Get_ro_LH_OBJECT_CLC_Id_Trans();
        e_Result = _M_Fn_Return_Gracenote_Transcriptions(
            ro_LH_OBJECT_CLC_Id_Trans,
            ro_vec_pair_Gracenote_Transcriptions);
        IF (E_Result_Succeeded != e_Result) {
            MACRO_Log_Error
            return E_Result_Error;
        }
    }

    LH_OBJECT& ro_LH_OBJECT_CLC_Lex = ro_Data_CLC.M_Fn_Get_ro_LH_OBJECT_CLC_LEX();
    e_Result = _M_Fn_Return_Text_Transcriptions(
        ro_LH_OBJECT_CLC_Lex,
        ro_vec_pair_Text_Transcriptions);
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}

void
C_Data_Transcription::_M_Fn_Get_Gracenote_And_Text_Transcriptions
(
    vector<pair<LH_TRANSCRIPTION*, size_t> >& ro_vec_pair_Transcriptions,
    size_t& i_Transcriptions_Index,
    scoped_array<LH_TRANSCRIPTION>& sca_Transcriptions
)
{
    typedef vector<pair<LH_TRANSCRIPTION*, size_t> >::iterator Vec_Pair_Iter;
    LH_TRANSCRIPTION* p_LH_TRANSCRIPTIONs = NULL;
    for (
        Vec_Pair_Iter iter = ro_vec_pair_Transcriptions.begin();
        iter != ro_vec_pair_Transcriptions.end();
        ++iter
    ) {
        size_t i_LH_TRANSCRIPTION_Index = 0;
        p_LH_TRANSCRIPTIONs = iter->first;
        while (i_LH_TRANSCRIPTION_Index < iter->second) {
            LH_TRANSCRIPTION& LH_TRANSCRIPTION_Curr = p_LH_TRANSCRIPTIONs[i_LH_TRANSCRIPTION_Index];
            sca_Transcriptions[i_Transcriptions_Index].type = LH_TRANSCRIPTION_Curr.type;
            sca_Transcriptions[i_Transcriptions_Index].pData = LH_TRANSCRIPTION_Curr.pData;
            sca_Transcriptions[i_Transcriptions_Index].size = LH_TRANSCRIPTION_Curr.size;

            ++i_Transcriptions_Index;
            ++i_LH_TRANSCRIPTION_Index;
        }
    }

    return;
}

E_Result
C_Data_Transcription::_M_Fn_Fetch_Gracenote_Transcriptions
(
    const C_Term& o_Term,
    LH_OBJECT& ro_LH_OBJECT_CLC_Id_Trans,
    vector<pair<LH_TRANSCRIPTION*, size_t> >& ro_vec_pair_Transcriptions,
    size_t& i_Transcriptions_Count
)
{
    LH_U64_STRUCT o_LH_U64_STRUCT_Trans;
    o_LH_U64_STRUCT_Trans.hi32 = o_Term.m_i_Id_Trans_Hi;
    o_LH_U64_STRUCT_Trans.lo32 = o_Term.m_i_Id_Trans_Lo;

    LH_TRANSCRIPTION* p_LH_TRANSCRIPTIONs = NULL;
    size_t i_LH_TRANSCRIPTIONs_count = 0;

    ro_vec_pair_Transcriptions.clear();
    i_Transcriptions_Count = 0;

    LH_ERROR o_LH_ERROR;
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_IdTransLookupFetchTranscriptions(
        ro_LH_OBJECT_CLC_Id_Trans,
        o_LH_U64_STRUCT_Trans,
        &p_LH_TRANSCRIPTIONs,
        &i_LH_TRANSCRIPTIONs_count
    );
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF ((0 != i_LH_TRANSCRIPTIONs_count)
        && (NULL != p_LH_TRANSCRIPTIONs)) {
        ro_vec_pair_Transcriptions.push_back(std::make_pair(p_LH_TRANSCRIPTIONs, i_LH_TRANSCRIPTIONs_count));
        i_Transcriptions_Count = i_LH_TRANSCRIPTIONs_count;
    }
    ELSE {
        stringstream stringstream_Log;
        stringstream_Log << "gracenote id hi(" << o_Term.m_i_Id_Trans_Hi
                         << ") lo(" << o_Term.m_i_Id_Trans_Lo
                         << "): no transcription";
        Fn_Log(stringstream_Log.str());
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Transcription::_M_Fn_Return_Gracenote_Transcriptions
(
    LH_OBJECT& ro_LH_OBJECT_CLC_Id_Trans,
    vector<pair<LH_TRANSCRIPTION*, size_t> >& ro_vec_pair_Transcriptions
)
{
    LH_ERROR o_LH_ERROR; // = LH_OK;
    typedef vector<pair<LH_TRANSCRIPTION*, size_t> >::iterator Vec_Pair_Iter;
    for (
        Vec_Pair_Iter iter = ro_vec_pair_Transcriptions.begin();
        iter != ro_vec_pair_Transcriptions.end();
        ++iter
    ) {
        o_LH_ERROR = G_o_Vocon().M_Fn_LH_IdTransLookupReturnTranscriptions(
            ro_LH_OBJECT_CLC_Id_Trans,
            iter->first
        );

        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Error;
        }
    }
    ro_vec_pair_Transcriptions.clear();

    return E_Result_Succeeded;
}

E_Result
C_Data_Transcription::_M_Fn_Fetch_Text_Transcriptions
(
    const C_Term& o_Term,
    LH_OBJECT& ro_LH_OBJECT_CLC_Lex,
    vector<pair<LH_TRANSCRIPTION*, size_t> >& ro_vec_pair_Transcriptions,
    size_t& i_Transcriptions_Count
)
{
    ro_vec_pair_Transcriptions.clear();
    i_Transcriptions_Count = 0;

    const vector<string>& vec_string_Orthography = o_Term.m_vector_string_Orthography;
    IF (vec_string_Orthography.empty()) {
        Fn_Log("the orthography vector is empty!");
        return E_Result_Succeeded;
    }

    LH_ERROR o_LH_ERROR; // = LH_OK;
    const char* p_Orthography = NULL;
    LH_TRANSCRIPTION* p_LH_TRANSCRIPTIONs = NULL;
    size_t i_LH_TRANSCRIPTIONs_Count = 0;

    for (
        vector<string>::const_iterator iter = vec_string_Orthography.begin();
        iter != vec_string_Orthography.end();
        ++iter
    ) {
        const string& string_Orthography = *iter;
        IF (!Fn_Is_Utf8_String(string_Orthography)) {
            Fn_Print_Bin_C_String(string_Orthography.c_str());
            continue;
        }

        IF (string_Orthography.empty()) {
            p_Orthography = " ";
        }
        else {
            p_Orthography = string_Orthography.c_str();
        }

        o_LH_ERROR = G_o_Vocon().M_Fn_LH_TransLookUpFetchTranscriptions(
            ro_LH_OBJECT_CLC_Lex,
            p_Orthography,
            &p_LH_TRANSCRIPTIONs,
            &i_LH_TRANSCRIPTIONs_Count
        );
        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Error;
        }

        IF ((0 != i_LH_TRANSCRIPTIONs_Count)
            && (NULL != p_LH_TRANSCRIPTIONs)) {
            i_Transcriptions_Count += i_LH_TRANSCRIPTIONs_Count;
            ro_vec_pair_Transcriptions.push_back(std::make_pair(p_LH_TRANSCRIPTIONs, i_LH_TRANSCRIPTIONs_Count));
        }
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Transcription::_M_Fn_Return_Text_Transcriptions
(
    LH_OBJECT& ro_LH_OBJECT_CLC_Lex,
    vector<pair<LH_TRANSCRIPTION*, size_t> >& ro_vec_pair_Transcriptions
)
{
    LH_ERROR o_LH_ERROR; // = LH_OK;

    typedef vector<pair<LH_TRANSCRIPTION*, size_t> >::iterator Vec_Pair_Iter;
    for (
        Vec_Pair_Iter iter = ro_vec_pair_Transcriptions.begin();
        iter != ro_vec_pair_Transcriptions.end();
        ++iter
    ) {
        o_LH_ERROR = G_o_Vocon().M_Fn_LH_TransLookUpReturnTranscriptions(
            ro_LH_OBJECT_CLC_Lex,
            iter->first
        );

        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Error;
        }
    }
    ro_vec_pair_Transcriptions.clear();

    return E_Result_Succeeded;
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
