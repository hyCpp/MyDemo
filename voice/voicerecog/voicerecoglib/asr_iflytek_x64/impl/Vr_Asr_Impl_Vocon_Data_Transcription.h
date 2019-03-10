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
 * @file Voice_Asr_Impl_Data_Transcription.h
 * @brief C_Data_Transcription class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Data_Phoneme_h__
#define __Vr_Asr_Impl_Data_Phoneme_h__

namespace N_Vr
{

namespace N_Asr
{

class C_Term;
class C_Buffer;

namespace N_Impl
{

namespace N_Vocon
{

class C_Data_CLC;
/**
 * @brief The C_Data_Transcription class
 *
 * C_Data_Transcription class
 */

class C_Data_Transcription
{
  public:

    explicit
    C_Data_Transcription(); // [F]Constructor of Default

    virtual
    ~C_Data_Transcription(); // [F]Destructor

    E_Result
    M_Fn_Get_Transcription
    (
        const C_Term& o_Term,
        C_Data_CLC& ro_Data_CLC,
        C_Buffer& o_Buffer_Transcription
    );

    E_Result
    M_Fn_Fetch_Gracenote_And_Text_Transcriptions
    (
        const C_Term& o_Term,
        C_Data_CLC& ro_Data_CLC,
        vector<pair<LH_TRANSCRIPTION*, size_t> >& ro_vec_pair_Text_Transcriptions,
        vector<pair<LH_TRANSCRIPTION*, size_t> >& ro_vec_pair_Gracenote_Transcriptions,
        scoped_array<LH_TRANSCRIPTION>& ro_sca_Transcriptions,
        size_t& ri_Transcriptions_Count
    );

    E_Result
    M_Fn_Return_Gracenote_And_Text_Transcriptions
    (
        const C_Term& o_Term,
        C_Data_CLC& ro_Data_CLC,
        vector<pair<LH_TRANSCRIPTION*, size_t> >& ro_vec_pair_Text_Transcriptions,
        vector<pair<LH_TRANSCRIPTION*, size_t> >& ro_vec_pair_Gracenote_Transcriptions
    );

  private:

    E_Result
    _M_Fn_Return_Gracenote_And_Text_Transcriptions
    (
        const C_Term& o_Term,
        C_Data_CLC& ro_Data_CLC,
        vector<pair<LH_TRANSCRIPTION*, size_t> >& ro_vec_pair_Text_Transcriptions,
        vector<pair<LH_TRANSCRIPTION*, size_t> >& ro_vec_pair_Gracenote_Transcriptions
    );

    void
    _M_Fn_Get_Gracenote_And_Text_Transcriptions
    (
        vector<pair<LH_TRANSCRIPTION*, size_t> >& ro_vec_pair_Transcriptions,
        size_t& i_Transcriptions_Index,
        scoped_array<LH_TRANSCRIPTION>& sca_Transcriptions
    );

    E_Result
    _M_Fn_Fetch_Gracenote_Transcriptions
    (
        const C_Term& o_Term,
        LH_OBJECT& ro_LH_OBJECT_CLC_Id_Trans,
        vector<pair<LH_TRANSCRIPTION*, size_t> >& ro_vec_pair_Transcriptions,
        size_t& i_Transcriptions_Count
    );

    E_Result
    _M_Fn_Return_Gracenote_Transcriptions
    (
        LH_OBJECT& ro_LH_OBJECT_CLC_Id_Trans,
        vector<pair<LH_TRANSCRIPTION*, size_t> >& ro_vec_pair_Transcriptions
    );

    E_Result
    _M_Fn_Fetch_Text_Transcriptions
    (
        const C_Term& o_Term,
        LH_OBJECT& ro_LH_OBJECT_CLC_Lex,
        vector<pair<LH_TRANSCRIPTION*, size_t> >& ro_vec_pair_Transcriptions,
        size_t& i_Transcriptions_Count
    );

    E_Result
    _M_Fn_Return_Text_Transcriptions
    (
        LH_OBJECT& ro_LH_OBJECT_CLC_Lex,
        vector<pair<LH_TRANSCRIPTION*, size_t> >& ro_vec_pair_Transcriptions
    );


    C_Data_Transcription // [F]Constructor: Copy
    (
        C_Data_Transcription const & // [I]
    );

    C_Data_Transcription & // [R]
    operator = // [F]Operator: =
    (
        C_Data_Transcription const & // [I]
    );
};

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
