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
 * @file Voice_Asr_Impl_Vocon_Mock.h
 * @brief C_Vocon_Mock class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Vocon_Vocon_Mock_h__
#define __Vr_Asr_Impl_Vocon_Vocon_Mock_h__

#include "Vr_Asr_Impl_Vocon_Vocon.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

/**
 * @brief The C_Vocon_Mock class
 *
 * C_Vocon_Mock class
 */

class C_Vocon_Mock : public C_Vocon
{
public:

    enum E_Param
    {
        E_Param_Begin = 0,
        E_Param_All_OK = E_Param_Begin,
        E_Param_LH_ComponentTerminat_ERROR,
        E_Param_LH_ComponentIsInErrorState_ERROR,
        E_Param_LH_ObjHasInterface_ERROR,
        E_Param_LH_ObjHasInterface_FALSE,
        E_Param_LH_ObjClose_ERROR,
        E_Param_LH_ArchivableArchive_ERROR,
        E_Param_LH_JSONParserFetchString_ERROR,
        E_Param_LH_JSONParserReturnString_ERROR,
        E_Param_LH_ConfigGetParam_ERROR,
        E_Param_LH_ConfigSetParam_ERROR,
        E_Param_LH_InitBase_ERROR,
        E_Param_LH_CreateAcMod_ERROR,
        E_Param_LH_AcModBorrowInfo_ERROR,
        E_Param_LH_AcModBorrowInfo_FALSE,
        E_Param_LH_AcModSetSpeakerProfile_ERROR,
        E_Param_LH_AcModArchiveSpeakerProfile_ERROR,
        E_Param_LH_AcModBorrowTranscriptionSpec_ERROR,
        E_Param_LH_InitAsr_ERROR,
        E_Param_LH_CreateAudioTypeSamples_ERROR,
        E_Param_LH_CreateAudioTypeFromSSE_ERROR,
        E_Param_LH_CreateFx_ERROR,
        E_Param_LH_FxGetAbnormCondition_ERROR,
        E_Param_LH_FxGetAbnormCondition_LH_FX_ABNORMNULL,
        E_Param_LH_FxGetAbnormCondition_LH_FX_BADSNR,
        E_Param_LH_FxGetAbnormCondition_LH_FX_OVERLOAD,
        E_Param_LH_FxGetAbnormCondition_LH_FX_TOOQUIET,
        E_Param_LH_FxGetAbnormCondition_LH_FX_NOSIGNAL,
        E_Param_LH_FxGetAbnormCondition_LH_FX_NOLEADINGSILENCE,
        E_Param_LH_FxGetAbnormCondition_LH_FX_POORMIC,
        E_Param_LH_FxGetSignalLevels_ERROR,
        E_Param_LH_FxConfigure_ERROR,
        E_Param_LH_EnvAdaptReset_ERROR,
        E_Param_LH_EnvAdaptArchive_ERROR,
        E_Param_LH_CreateSingleThreadRec_ERROR,
        E_Param_LH_SingleThreadRecControlTimeOut_ERROR,
        E_Param_LH_SingleThreadRecSetAudioType_ERROR,
        E_Param_LH_SingleThreadRecProcess_ERROR,
        E_Param_LH_ActivatableActivateRule_ERROR,
        E_Param_LH_ActivatableDeactivateRule_ERROR,
        E_Param_LH_ActivatableDeactivateAllRules_ERROR,
        E_Param_LH_NBestResultGetNbrHypotheses_ERROR,
        E_Param_LH_NBestResultBorrowConfLevels_ERROR,
        E_Param_LH_NBestResultBorrowStartRule_ERROR,
        E_Param_LH_NBestResultFetchHypothesis_ERROR,
        E_Param_LH_RecAddCtx_ERROR,
        E_Param_LH_RecRemoveCtx_ERROR,
        E_Param_LH_HostCtxSetCtx_ERROR,
        E_Param_LH_HostCtxSetCtx2_ERROR,
        E_Param_LH_HostCtxSetCtx2_FALSE,
        E_Param_LH_HostCtxClearCtx_ERROR,
        E_Param_LH_MergeCtxMergeCtx_ERROR,
        E_Param_LH_MergeCtxCanMergeCtx_ERROR,
        E_Param_LH_MergeCtxClearMerges_ERROR,
        E_Param_LH_CreateCtx_ERROR,
        E_Param_LH_CreateUswCtx_ERROR,
        E_Param_LH_RecCreateResult_ERROR,
        E_Param_LH_RecCreateResult_Usw,
        E_Param_LH_RecCreateResult_2,
        E_Param_LH_FieldCtxSetStartStop_ERROR,
        E_Param_LH_FieldCtxActivate_ERROR,
        E_Param_LH_FieldCtxActivateUserId_ERROR,
        E_Param_LH_CreateEmptyListCtx_ERROR,
        E_Param_LH_ListCtxAddTermSeq_ERROR,
        E_Param_LH_ListCtxRemoveAlternative_ERROR,
        E_Param_LH_ListCtxCommit_ERROR,
        E_Param_LH_TableCtxActivate_ERROR,
        E_Param_LH_CreateFuzzyMatchCtxCpl_ERROR,
        E_Param_LH_FuzzyMatchCtxCplBeginTable_ERROR,
        E_Param_LH_FuzzyMatchCtxCplEndTable_ERROR,
        E_Param_LH_FuzzyMatchCtxCplAddColumn_ERROR,
        E_Param_LH_FuzzyMatchCtxCplSetIntegers_ERROR,
        E_Param_LH_FuzzyMatchCtxCplSetStrings_ERROR,
        E_Param_LH_FuzzyMatchCtxCplSetSpeakableStrings_ERROR,
        E_Param_LH_FuzzyMatchCtxCplCommitRow_ERROR,
        E_Param_LH_FuzzyMatchCtxCplRun_ERROR,
        E_Param_LH_FuzzyMatchCtxCplSetErrorCallback_ERROR,
        E_Param_LH_UswResultGetAcceptanceStatus_ERROR,
        E_Param_LH_UswResultGetAcceptanceStatus_LH_USW_NULL,
        E_Param_LH_UswResultGetAcceptanceStatus_LH_USW_ACCEPTSTATUS_NOT_TRAINED,
        E_Param_LH_UswResultGetAcceptanceStatus_LH_USW_ACCEPTSTATUS_TOO_SHORT,
        E_Param_LH_UswResultFetchTranscription_ERROR,
        E_Param_LH_UswResultFetchTranscription_NULL,
        E_Param_LH_UswResultReturnTranscription_ERROR,
        E_Param_LH_UswResultHasAudioBuffer_ERROR,
        E_Param_LH_UswResultHasAudioBuffer_FALSE,
        E_Param_LH_UswResultFetchAudioBuffer_ERROR,
        E_Param_LH_UswResultReturnAudioBuffer_ERROR,
        E_Param_LH_InitPron_ERROR,
        E_Param_LH_CreateMLCLC_ERROR,
        E_Param_LH_CLCBorrowDomainCategories_ERROR,
        E_Param_LH_CLCBorrowSecondaryLanguages_ERROR,
        E_Param_LH_CLCSetCurrentDomainCategory_ERROR,
        E_Param_LH_CreateLex_ERROR,
        E_Param_LH_LexSetG2P_ERROR,
        E_Param_LH_LexClearG2P_ERROR,
        E_Param_LH_TransLookUpFetchTranscriptions_ERROR,
        E_Param_LH_TransLookUpReturnTranscriptions_ERROR,
        E_Param_PH_DlHeapCreate_ERROR,
        E_Param_PH_DlHeapClose_ERROR,
        E_Param_ST_CreateStreamReaderFromFile_ERROR,
        E_Param_ST_CreateStreamWriterToFile_ERROR,
        E_Param_ST_CreateStreamReaderFromFile_Crc_ERROR,
        E_Param_ST_CreateStreamWriterToFile_Crc_ERROR,
        E_Param_LH_CreateStreamInfo_ERROR,
        E_Param_LH_AcModCheckDependency_ERROR,
        E_Param_LH_IdTransLookupLoadData_ERROR,
        E_Param_LH_IdTransLookupUnloadData_ERROR,
        E_Param_LH_IdTransLookupFetchTranscriptions_ERROR,
        E_Param_LH_IdTransLookupReturnTranscriptions_ERROR,
        E_Param_End,
        E_Param_Size = E_Param_End - E_Param_Begin,
    };

    size_t i_Num_Trigger;
    size_t i_Pos_Trigger;

    vector<int> vector_b_Param;

    vector<unsigned long> m_vector_i_Confidence;

    vector<LH_HYPOTHESIS> m_vector_o_LH_HYPOTHESIS;

    vector<vector<LH_HYPOTHESIS_WORD_S>> m_vector_vector_o_LH_HYPOTHESIS_WORD_S;

    LH_AUDIOCHAINEVENT_INTERFACE m_o_LH_AUDIOCHAINEVENT_INTERFACE;

    void * m_pvoid_Inst_Event;

    virtual
    ~C_Vocon_Mock(); // [F]Destructor

    explicit
    C_Vocon_Mock(); // [F]Constructor of Default

    bool
    M_Fn_Check_Trigger
    (
        int i_Param
    )
    {
        if (vector_b_Param[i_Param]) {

            if (i_Pos_Trigger++ == i_Num_Trigger) {
                return true;
            }

            return false;
        }

        return false;
    }

    virtual
    LH_COMPONENT
    M_Fn_LH_GetNullComponent
    (
        void
    )
    {
        LH_COMPONENT o_LH_COMPONENT;

        o_LH_COMPONENT.pObj = NULL;
        o_LH_COMPONENT.magicNbr = 0;

        return o_LH_COMPONENT;
    }

    virtual
    LH_BOOL
    M_Fn_LH_ComponentIsNull
    (
        LH_COMPONENT hComponent
    )
    {
        if (hComponent.pObj) {
            return LH_FALSE;
        }
        else {
            return LH_TRUE;
        }
    }

    virtual
    LH_ERROR
    M_Fn_LH_ComponentIsInErrorState
    (
        LH_COMPONENT hComponent,
        LH_BOOL* pIsInErrorState
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_ComponentIsInErrorState_ERROR)) {
            return LH_E_INTERNAL;
        }

        if (hComponent.magicNbr) {
            *pIsInErrorState = LH_TRUE;
        }
        else {
            *pIsInErrorState = LH_FALSE;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_ComponentTerminate
    (
        LH_COMPONENT* phComponent
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_ComponentTerminat_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_OBJECT
    M_Fn_LH_GetNullObj
    (
        void
    )
    {
        LH_OBJECT o_LH_OBJECT;

        o_LH_OBJECT.pObj = NULL;
        o_LH_OBJECT.magicNbr = 0;

        return o_LH_OBJECT;
    }

    virtual
    LH_BOOL
    M_Fn_LH_ObjIsNull
    (
        LH_OBJECT hObj
    )
    {
        if (hObj.pObj) {
            return LH_FALSE;
        }
        else {
            return LH_TRUE;
        }
    }

    virtual
    LH_ERROR
    M_Fn_LH_ObjHasInterface
    (
        LH_OBJECT hObj,
        LH_INTERFACE_ID interfaceID,
        LH_BOOL* pHasInterface
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_ObjHasInterface_ERROR)) {
            return LH_E_INTERNAL;
        }

        if (interfaceID == LH_IID_IUSWRESULT) {
            if (hObj.magicNbr == 2) {
                *pHasInterface = LH_TRUE;
            }
            else {
                *pHasInterface = LH_FALSE;
            }

            return LH_OK;
        }
        else if (interfaceID == LH_IID_INBESTRESULT) {
            if (hObj.magicNbr == 1) {
                *pHasInterface = LH_TRUE;
            }
            else {
                *pHasInterface = LH_FALSE;
            }

            return LH_OK;
        }

        if (M_Fn_Check_Trigger(E_Param_LH_ObjHasInterface_FALSE)) {
            *pHasInterface = LH_FALSE;
        }
        else {
            *pHasInterface = LH_TRUE;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_ObjClose
    (
        LH_OBJECT* phObj
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_ObjClose_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_ArchivableArchive
    (
        LH_OBJECT hArchivable,
        LH_OSTREAM_INTERFACE* pOStreamInterface,
        void* pOStreamInst,
        LH_OSTREAM_PARAMS* pOStreamParams
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_ArchivableArchive_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_JSONParserFetchString
    (
        LH_OBJECT hJSONParser,
        char** pszJSONString
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_JSONParserFetchString_ERROR)) {
            return LH_E_INTERNAL;
        }

        static char * a = "JSON";

        * pszJSONString = a;

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_JSONParserReturnString
    (
        LH_OBJECT hJSONParser,
        char* szJSONString
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_JSONParserReturnString_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_ConfigGetParam
    (
        LH_OBJECT hConfig,
        unsigned long paramId,
        signed long *pParamValue
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_ConfigGetParam_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_ConfigSetParam
    (
        LH_OBJECT hConfig,
        unsigned long paramId,
        signed long paramValue
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_ConfigSetParam_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_InitBase
    (
        LH_HEAP_INTERFACE* pHeapInterface,
        void* pHeapInst,
        LH_ERROR_CALLBACK_FN errorCallbackFn,
        void* pErrorCallbackInst,
        LH_COMPONENT* phCBase
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_InitBase_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_CreateAcMod
    (
        LH_COMPONENT hCAsr,
        LH_ISTREAM_INTERFACE* pIStreamInterface,
        void* pIStreamInst,
        LH_ISTREAM_PARAMS* pIStreamParams,
        LH_OBJECT* phAcMod
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_CreateAcMod_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_AcModBorrowInfo
    (
        LH_OBJECT hAcMod,
        struct LH_ACMOD_INFO_S** ppAcModInfo
    )
    {
        static struct LH_ACMOD_INFO_S o_LH_ACMOD_INFO_S;
        if (M_Fn_Check_Trigger(E_Param_LH_AcModBorrowInfo_ERROR)) {
            return LH_E_INTERNAL;
        }

        if (M_Fn_Check_Trigger(E_Param_LH_AcModBorrowInfo_FALSE)) {
            *ppAcModInfo = NULL;
        }
        else {
            *ppAcModInfo = &o_LH_ACMOD_INFO_S;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_AcModSetSpeakerProfile
    (
        LH_OBJECT hAcMod,
        LH_ISTREAM_INTERFACE* pIStreamInterface,
        void* pIStreamInst,
        LH_ISTREAM_PARAMS* pIStreamParams
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_AcModSetSpeakerProfile_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_AcModArchiveSpeakerProfile
    (
        LH_OBJECT hAcMod,
        LH_OSTREAM_INTERFACE* pOStreamInterface,
        void* pOStreamInst,
        LH_OSTREAM_PARAMS* pOStreamParams
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_AcModArchiveSpeakerProfile_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_AcModBorrowTranscriptionSpec
    (
        LH_OBJECT hAcMod,
        LH_TRANSCRIPTION_SPEC ** ppTransSpec
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_AcModBorrowTranscriptionSpec_ERROR)) {
            return LH_E_INTERNAL;
        }

        * ppTransSpec = NULL;

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_InitAsr
    (
        LH_COMPONENT hCBase,
        LH_HEAP_INTERFACE* pHeapInterface,
        void* pHeapInst,
        LH_COMPONENT* phCAsr
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_InitAsr_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_CreateAudioTypeSamples
    (
        LH_COMPONENT hCAsr,
        unsigned long sampleFreq,
        LH_OBJECT* phAudioType
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_CreateAudioTypeSamples_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_CreateAudioTypeFromSSE
    (
        LH_COMPONENT hCAsr,
        const void *pSSEAudioTypeData,
        size_t dataSize,
        LH_OBJECT* phAudioType
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_CreateAudioTypeFromSSE_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_CreateFx
    (
        LH_COMPONENT hCAsr,
        LH_OBJECT* phFx
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_CreateFx_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_FxGetAbnormCondition
    (
        LH_OBJECT hFx,
        LH_FX_ABNORMCOND *pAbnormCondition
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_FxGetAbnormCondition_ERROR)) {
            return LH_E_INTERNAL;
        }

        if (M_Fn_Check_Trigger(E_Param_LH_FxGetAbnormCondition_LH_FX_ABNORMNULL)) {
            *pAbnormCondition = LH_FX_ABNORMNULL;
        }
        else if (M_Fn_Check_Trigger(E_Param_LH_FxGetAbnormCondition_LH_FX_BADSNR)) {
            *pAbnormCondition = LH_FX_BADSNR;
        }
        else if (M_Fn_Check_Trigger(E_Param_LH_FxGetAbnormCondition_LH_FX_OVERLOAD)) {
            *pAbnormCondition = LH_FX_OVERLOAD;
        }
        else if (M_Fn_Check_Trigger(E_Param_LH_FxGetAbnormCondition_LH_FX_TOOQUIET)) {
            *pAbnormCondition = LH_FX_TOOQUIET;
        }
        else if (M_Fn_Check_Trigger(E_Param_LH_FxGetAbnormCondition_LH_FX_NOSIGNAL)) {
            *pAbnormCondition = LH_FX_NOSIGNAL;
        }
        else if (M_Fn_Check_Trigger(E_Param_LH_FxGetAbnormCondition_LH_FX_NOLEADINGSILENCE)) {
            *pAbnormCondition = LH_FX_NOLEADINGSILENCE;
        }
        else if (M_Fn_Check_Trigger(E_Param_LH_FxGetAbnormCondition_LH_FX_POORMIC)) {
            *pAbnormCondition = LH_FX_POORMIC;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_FxGetSignalLevels
    (
        LH_OBJECT hFx,
        LH_FX_SIGNAL_LEVELS *pSignalLevels
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_FxGetSignalLevels_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_FxConfigure
    (
        LH_OBJECT hFx,
        LH_OBJECT hFxConfig
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_FxConfigure_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_EnvAdaptReset
    (
        LH_OBJECT hObject,
        LH_ISTREAM_INTERFACE* pIStreamInterface,
        void* pIStreamInst,
        LH_ISTREAM_PARAMS* pIStreamParams
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_EnvAdaptReset_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_EnvAdaptArchive
    (
        LH_OBJECT hObject,
        LH_OSTREAM_INTERFACE* pOStreamInterface,
        void* pOStreamInst,
        LH_OSTREAM_PARAMS* pOStreamParams
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_EnvAdaptArchive_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_CreateSingleThreadRec
    (
        LH_COMPONENT          hCAsr,
        LH_AUDIOCHAINEVENT_INTERFACE  * pEventInterface,
        void                * pEventInst,
        size_t                bufferSizeMs,
        LH_OBJECT             hAudioType,
        LH_OBJECT           * phSingleThreadRec
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_CreateSingleThreadRec_ERROR)) {
            return LH_E_INTERNAL;
        }

        m_o_LH_AUDIOCHAINEVENT_INTERFACE.pfevent = pEventInterface->pfevent;
        m_o_LH_AUDIOCHAINEVENT_INTERFACE.pfadvance = pEventInterface->pfadvance;

        m_pvoid_Inst_Event = pEventInst;

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_SingleThreadRecControlTimeOut
    (
        LH_OBJECT hSingleThreadRec,
        LH_AUDIOSOURCE_TIMEOUTMODE TimeOutMode
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_SingleThreadRecControlTimeOut_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_SingleThreadRecSetAudioType
    (
        LH_OBJECT hSingleThreadRec,
        LH_OBJECT hAudioType
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_SingleThreadRecSetAudioType_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_SingleThreadRecProcess
    (
        LH_OBJECT             hSingleThreadRec,
        LH_AUDIOIN_INTERFACE* pAudioInInterface,
        void                * pAudioInInst
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_SingleThreadRecProcess_ERROR)) {
            return LH_E_INTERNAL;
        }

        LH_BOOL o_LH_BOOL_Continue;

        m_o_LH_AUDIOCHAINEVENT_INTERFACE.pfadvance
        (
            m_pvoid_Inst_Event,
            &o_LH_BOOL_Continue
        );

        if (!o_LH_BOOL_Continue) {
            return LH_OK;
        }

        unsigned long i_Type =
            LH_AUDIOCHAIN_EVENT_BOS |
            LH_AUDIOCHAIN_EVENT_TS_FX |
            LH_AUDIOCHAIN_EVENT_TS_REC |
            LH_AUDIOCHAIN_EVENT_SSE_NOT_CONVERGED |
            LH_AUDIOCHAIN_EVENT_SSE_CONVERGED |
            LH_AUDIOCHAIN_EVENT_FX_ABNORMCOND |
            LH_AUDIOCHAIN_EVENT_FX_TIMER |
            LH_AUDIOCHAIN_EVENT_RESULT
            ;

        LH_TIME o_LH_TIME = 1;

        m_o_LH_AUDIOCHAINEVENT_INTERFACE.pfevent
        (
            m_pvoid_Inst_Event,
            i_Type,
            o_LH_TIME
        );

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_ActivatableActivateRule
    (
        LH_OBJECT hActivatable,
        const char* szRule
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_ActivatableActivateRule_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_ActivatableDeactivateRule
    (
        LH_OBJECT hActivatable,
        const char* szRule
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_ActivatableDeactivateRule_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_ActivatableDeactivateAllRules
    (
        LH_OBJECT hActivatable
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_ActivatableDeactivateAllRules_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_NBestResultGetNbrHypotheses
    (
        LH_OBJECT hNBestResult,
        size_t* pNbrHypotheses
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_NBestResultGetNbrHypotheses_ERROR)) {
            return LH_E_INTERNAL;
        }

        * pNbrHypotheses = m_vector_o_LH_HYPOTHESIS.size();

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_NBestResultBorrowConfLevels
    (
        LH_OBJECT hNBestResult,
        unsigned long** ppConfLevels,
        size_t* pNbrConfLevels
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_NBestResultBorrowConfLevels_ERROR)) {
            return LH_E_INTERNAL;
        }

        * pNbrConfLevels = m_vector_i_Confidence.size();
        if (* pNbrConfLevels > 0) {
            * ppConfLevels = &m_vector_i_Confidence[0];
        }
        else {
            * ppConfLevels = NULL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_NBestResultBorrowStartRule
    (
        LH_OBJECT hNBestResult,
        size_t hypoIdx,
        char** pszStartRule
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_NBestResultBorrowStartRule_ERROR)) {
            return LH_E_INTERNAL;
        }

        * pszStartRule = m_vector_o_LH_HYPOTHESIS[hypoIdx].szStartRule;

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_NBestResultFetchHypothesis
    (
        LH_OBJECT hNBestResult,
        size_t n,
        LH_HYPOTHESIS** ppHypothesis
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_NBestResultFetchHypothesis_ERROR)) {
            return LH_E_INTERNAL;
        }

        * ppHypothesis = &m_vector_o_LH_HYPOTHESIS[n];

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_RecAddCtx
    (
        LH_OBJECT hRec,
        LH_OBJECT hCtx
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_RecAddCtx_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_RecRemoveCtx
    (
        LH_OBJECT hRec,
        LH_OBJECT hCtx
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_RecRemoveCtx_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_HostCtxSetCtx
    (
        LH_OBJECT hHostCtx,
        const char* szSlotName,
        LH_OBJECT hGuestCtx
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_HostCtxSetCtx_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_HostCtxSetCtx2
    (
        LH_OBJECT hHostCtx,
        const char* szSlotAssignments,
        LH_OBJECT hGuestCtx,
        LH_HOSTCTX_STATUS* pStatus
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_HostCtxSetCtx2_ERROR)) {
            return LH_E_INTERNAL;
        }

        if (M_Fn_Check_Trigger(E_Param_LH_HostCtxSetCtx2_FALSE)) {
            *pStatus = LH_HOSTCTX_STATUS_CONTEXTNOTSET;
        }
        else {
            *pStatus = LH_HOSTCTX_STATUS_OK;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_HostCtxClearCtx
    (
        LH_OBJECT hHostCtx,
        const char* szSlotName
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_HostCtxClearCtx_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_MergeCtxMergeCtx
    (
        LH_OBJECT hMergeCtx,
        LH_OBJECT hGuestCtx
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_MergeCtxMergeCtx_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_MergeCtxCanMergeCtx
    (
        LH_OBJECT hMergeCtx,
        LH_OBJECT hGuestCtx,
        LH_BOOL *pCanMerge
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_MergeCtxCanMergeCtx_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_MergeCtxClearMerges
    (
        LH_OBJECT hMergeCtx
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_MergeCtxClearMerges_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_CreateCtx
    (
        LH_COMPONENT hCAsr,
        LH_ISTREAM_INTERFACE* pIStreamInterface,
        void* pIStreamInst,
        LH_ISTREAM_PARAMS* pIStreamParams,
        LH_OBJECT hAcMod,
        LH_OBJECT* phCtx
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_CreateCtx_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_CreateUswCtx
    (
        LH_COMPONENT hCAsr,
        LH_OBJECT hAcMod,
        LH_OBJECT* phUswCtx
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_CreateUswCtx_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_RecCreateResult
    (
        LH_OBJECT hRec,
        LH_OBJECT *phResult
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_RecCreateResult_ERROR)) {
            return LH_E_INTERNAL;
        }

        if (M_Fn_Check_Trigger(E_Param_LH_RecCreateResult_Usw)
            || M_Fn_Check_Trigger(E_Param_LH_UswResultGetAcceptanceStatus_ERROR)
            || M_Fn_Check_Trigger(E_Param_LH_UswResultGetAcceptanceStatus_LH_USW_NULL)
            || M_Fn_Check_Trigger(E_Param_LH_UswResultGetAcceptanceStatus_LH_USW_ACCEPTSTATUS_NOT_TRAINED)
            || M_Fn_Check_Trigger(E_Param_LH_UswResultGetAcceptanceStatus_LH_USW_ACCEPTSTATUS_TOO_SHORT)
            || M_Fn_Check_Trigger(E_Param_LH_UswResultFetchTranscription_ERROR)
            || M_Fn_Check_Trigger(E_Param_LH_UswResultFetchTranscription_NULL)
            || M_Fn_Check_Trigger(E_Param_LH_UswResultReturnTranscription_ERROR)
            || M_Fn_Check_Trigger(E_Param_LH_UswResultHasAudioBuffer_ERROR)
            || M_Fn_Check_Trigger(E_Param_LH_UswResultHasAudioBuffer_FALSE)
            || M_Fn_Check_Trigger(E_Param_LH_UswResultFetchAudioBuffer_ERROR)
            || M_Fn_Check_Trigger(E_Param_LH_UswResultReturnAudioBuffer_ERROR)
           ) {
            phResult->magicNbr = 2;
        }
        else {
            phResult->magicNbr = 1;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_FieldCtxSetStartStop
    (
        LH_OBJECT hFieldCtx,
        const char* szStartField,
        const char* szStopField,
        unsigned long activationFlag
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_FieldCtxSetStartStop_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_FieldCtxActivate
    (
        LH_OBJECT hFieldCtx,
        const char* szStartField,
        const char* szActivateFromField,
        LH_USERID* pUserIds,
        size_t nbrUserIds
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_FieldCtxActivate_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_FieldCtxActivateUserId
    (
        LH_OBJECT hFieldCtx,
        const char* szStartField,
        const char* szActivateFromField,
        LH_USERID64* pUserIds,
        size_t nbrUserIds
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_FieldCtxActivateUserId_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_CreateEmptyListCtx
    (
        LH_COMPONENT hCAsr,
        LH_OBJECT hAcMod,
        unsigned long flags,
        LH_USERID_RANGE range,
        LH_OBJECT* phListCtx
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_CreateEmptyListCtx_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_ListCtxAddTermSeq
    (
        LH_OBJECT hListCtx,
        LH_GRM_TERMINAL pTerminals[],
        size_t nbrTerminals,
        LH_TERMSEQ_STATUS* pStatus
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_ListCtxAddTermSeq_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_ListCtxRemoveAlternative
    (
        LH_OBJECT hListCtx,
        LH_GRM_TERMINAL pTerminals[],
        size_t nbrTerminals,
        LH_BNF_MATCHTYPE matchType,
        size_t* pNbrMatched
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_ListCtxRemoveAlternative_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_ListCtxCommit
    (
        LH_OBJECT hListCtx
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_ListCtxCommit_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_TableCtxActivate
    (
        LH_OBJECT hTableCtx,
        const char* szQuery,
        unsigned long activationValue,
        LH_TABLECTX_ACTIVATESTATUS* pStatus
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_TableCtxActivate_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_CreateFuzzyMatchCtxCpl
    (
        LH_COMPONENT hCAsr,
        LH_OSTREAM_INTERFACE* pOStreamInterface,
        void* pOStreamInst,
        LH_OSTREAM_PARAMS *pOStreamParams,
        LH_OBJECT hAcMod,
        LH_OBJECT* phFmCtxCpl
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_CreateFuzzyMatchCtxCpl_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_FuzzyMatchCtxCplBeginTable
    (
        LH_OBJECT hFmCtxCpl,
        unsigned int tableID
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_FuzzyMatchCtxCplBeginTable_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_FuzzyMatchCtxCplEndTable
    (
        LH_OBJECT hFmCtxCpl,
        LH_FUZZYMATCHCTXCPL_STATUS* pStatus
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_FuzzyMatchCtxCplEndTable_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_FuzzyMatchCtxCplAddColumn
    (
        LH_OBJECT hFmCtxCpl,
        const char* szColumnName,
        unsigned int columnAttributes,
        unsigned int* columnID
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_FuzzyMatchCtxCplAddColumn_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_FuzzyMatchCtxCplSetIntegers
    (
        LH_OBJECT hFmCtxCpl,
        unsigned int columnID,
        unsigned int* aValues,
        size_t nbrValues
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_FuzzyMatchCtxCplSetIntegers_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_FuzzyMatchCtxCplSetStrings
    (
        LH_OBJECT hFmCtxCpl,
        unsigned int columnID,
        const char** aszValue,
        size_t nbrValues
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_FuzzyMatchCtxCplSetStrings_ERROR)) {
            return LH_E_INTERNAL;
        }

        static char * a = "F";

        * aszValue = a;

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_FuzzyMatchCtxCplSetSpeakableStrings
    (
        LH_OBJECT hFmCtxCpl,
        unsigned int columnID,
        LH_FUZZYMATCHCTXCPL_SPEAKABLEDATA* aSpeakableData,
        size_t nbrTerminals
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_FuzzyMatchCtxCplSetSpeakableStrings_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_FuzzyMatchCtxCplCommitRow
    (
        LH_OBJECT hFmCtxCpl,
        LH_FUZZYMATCHCTXCPL_STATUS* pStatus
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_FuzzyMatchCtxCplCommitRow_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_FuzzyMatchCtxCplRun
    (
        LH_OBJECT hFmCtxCpl,
        LH_FUZZYMATCHCTXCPL_STATUS* pStatus
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_FuzzyMatchCtxCplRun_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_FuzzyMatchCtxCplSetErrorCallback
    (
        LH_OBJECT hFmCtxCpl,
        LH_FUZZYMATCHCTXCPL_ERROR_CALLBACK_FN pfErrorCallback,
        void *pErrorCallbackInst
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_FuzzyMatchCtxCplSetErrorCallback_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_UswResultGetAcceptanceStatus
    (
        LH_OBJECT hUswResult,
        LH_USW_ACCEPTSTATUS* pStatus
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_UswResultGetAcceptanceStatus_ERROR)) {
            return LH_E_INTERNAL;
        }

        if (M_Fn_Check_Trigger(E_Param_LH_UswResultGetAcceptanceStatus_LH_USW_NULL)) {
            *reinterpret_cast<int*>(pStatus) = 100;
        }
        else if (M_Fn_Check_Trigger(E_Param_LH_UswResultGetAcceptanceStatus_LH_USW_ACCEPTSTATUS_NOT_TRAINED)) {
            *pStatus = LH_USW_ACCEPTSTATUS_NOT_TRAINED;
        }
        else if (M_Fn_Check_Trigger(E_Param_LH_UswResultGetAcceptanceStatus_LH_USW_ACCEPTSTATUS_TOO_SHORT)) {
            *pStatus = LH_USW_ACCEPTSTATUS_TOO_SHORT;
        }
        else {
            *pStatus = LH_USW_ACCEPTSTATUS_ACCEPTED;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_UswResultFetchTranscription
    (
        LH_OBJECT hUswResult,
        LH_TRANSCRIPTION **ppTranscription
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_UswResultFetchTranscription_ERROR)) {
            return LH_E_INTERNAL;
        }

        if (M_Fn_Check_Trigger(E_Param_LH_UswResultFetchTranscription_NULL)) {
            return LH_OK;
        }

        static char a = 1;

        static LH_TRANSCRIPTION o_LH_TRANSCRIPTION;

        o_LH_TRANSCRIPTION.pData = &a;
        o_LH_TRANSCRIPTION.size = 1;

        *ppTranscription = &o_LH_TRANSCRIPTION;

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_UswResultReturnTranscription
    (
        LH_OBJECT hUswResult,
        LH_TRANSCRIPTION *pTranscription
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_UswResultReturnTranscription_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_UswResultHasAudioBuffer
    (
        LH_OBJECT hUswResult,
        LH_BOOL* pHasAudioBuffer
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_UswResultHasAudioBuffer_ERROR)) {
            return LH_E_INTERNAL;
        }

        if (M_Fn_Check_Trigger(E_Param_LH_UswResultHasAudioBuffer_FALSE)) {
            * pHasAudioBuffer = LH_FALSE;
            return LH_OK;
        }

        * pHasAudioBuffer = LH_TRUE;
        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_UswResultFetchAudioBuffer
    (
        LH_OBJECT hUswResult,
        unsigned long* pFrequency,
        void**    ppBuffer,
        size_t*   pSize
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_UswResultFetchAudioBuffer_ERROR)) {
            return LH_E_INTERNAL;
        }

        static char a;

        *ppBuffer = &a;
        *pSize = 1;

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_UswResultReturnAudioBuffer
    (
        LH_OBJECT hUswResult,
        void*     pBuffer
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_UswResultReturnAudioBuffer_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_InitPron
    (
        LH_COMPONENT hCBase,
        LH_HEAP_INTERFACE* pHeapInterface,
        void* pHeapInst,
        LH_COMPONENT* phCPron
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_InitPron_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_CreateMLCLC
    (
        LH_COMPONENT hCPron,
        LH_ISTREAM_INTERFACE* pIStreamInterface,
        void* pIStreamInst,
        LH_ISTREAM_PARAMS* pIStreamParams,
        LH_TRANSCRIPTION_SPEC *pTranscriptionsSpec,
        LH_OBJECT* phCLC
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_CreateMLCLC_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_CLCBorrowDomainCategories
    (
        LH_OBJECT hCLC,
        char*** ppszDomainCategories,
        size_t* pNbrCategories
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_CLCBorrowDomainCategories_ERROR)) {
            return LH_E_INTERNAL;
        }

        static char* a = "name";
        *ppszDomainCategories = &a;
        *pNbrCategories = 1;

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_CLCBorrowSecondaryLanguages
    (
        LH_OBJECT hCLC,
        char*** ppszSecondaryLanguages,
        size_t* pNbrSecondaryLanguages
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_CLCBorrowSecondaryLanguages_ERROR)) {
            return LH_E_INTERNAL;
        }

        static char* a = "name";
        *ppszSecondaryLanguages = &a;
        *pNbrSecondaryLanguages = 1;

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_CLCSetCurrentDomainCategory
    (
        LH_OBJECT hCLC,
        const char * szCurrentDomainCategory
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_CLCSetCurrentDomainCategory_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_CreateLex
    (
        LH_COMPONENT hCPron,
        LH_TRANSCRIPTION_SPEC *pTranscriptionsSpec,
        LH_OBJECT  *phLex
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_CreateLex_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_LexSetG2P
    (
        LH_OBJECT hLex,
        LH_OBJECT hG2P,
        LH_OBJECT hCache
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_LexSetG2P_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_LexClearG2P
    (
        LH_OBJECT hLex
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_LexClearG2P_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_TransLookUpFetchTranscriptions
    (
        LH_OBJECT hTransLookUp,
        const char* szWord,
        LH_TRANSCRIPTION** ppTranscriptions,
        size_t* pNbrTranscriptions
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_TransLookUpFetchTranscriptions_ERROR)) {
            return LH_E_INTERNAL;
        }

        static char a = 1;

        static LH_TRANSCRIPTION o_LH_TRANSCRIPTION;

        o_LH_TRANSCRIPTION.pData = &a;
        o_LH_TRANSCRIPTION.size = 1;

        *ppTranscriptions = &o_LH_TRANSCRIPTION;

        *pNbrTranscriptions = 1;

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_TransLookUpReturnTranscriptions
    (
        LH_OBJECT hTransLookUp,
        LH_TRANSCRIPTION* pTranscriptions
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_TransLookUpReturnTranscriptions_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    PH_ERROR
    M_Fn_PH_DlHeapCreate
    (
        PH_DLHEAP_PARAMS* pHeapParams,
        LH_HEAP_INTERFACE* pHeapInterface,
        void** ppHeapInst
    )
    {
        if (M_Fn_Check_Trigger(E_Param_PH_DlHeapCreate_ERROR)) {
            return PH_E_ARG;
        }

        * ppHeapInst = NULL;

        return PH_OK;
    }

    virtual
    PH_ERROR
    M_Fn_PH_DlHeapClose
    (
        void** ppHeapInst
    )
    {
        if (M_Fn_Check_Trigger(E_Param_PH_DlHeapClose_ERROR)) {
            return PH_E_ARG;
        }

        * ppHeapInst = NULL;

        return PH_OK;
    }

    virtual
    ST_ERROR
    M_Fn_ST_CreateStreamReaderFromFile
    (
        const char* szFilename,
        struct LH_ISTREAM_INTERFACE_S* pIStreamInterface,
        void** ppIStreamInst
    )
    {
        if (M_Fn_Check_Trigger(E_Param_ST_CreateStreamReaderFromFile_ERROR)) {
            return ST_E_ARG;
        }

        * ppIStreamInst = NULL;

        return ST_OK;
    }

    virtual
    ST_ERROR
    M_Fn_ST_CreateStreamWriterToFile
    (
        const char* szFilename,
        struct LH_OSTREAM_INTERFACE_S* pOStreamInterface,
        void** ppOStreamInst
    )
    {
        if (M_Fn_Check_Trigger(E_Param_ST_CreateStreamWriterToFile_ERROR)) {
            return ST_E_ARG;
        }

        * ppOStreamInst = NULL;

        return ST_OK;
    }

    virtual
    ST_ERROR
    M_Fn_ST_CreateStreamReaderFromFile_Crc
    (
        const char* szFilename,
        struct LH_ISTREAM_INTERFACE_S* pIStreamInterface,
        void** ppIStreamInst
    )
    {
        if (M_Fn_Check_Trigger(E_Param_ST_CreateStreamReaderFromFile_Crc_ERROR)) {
            return ST_E_ARG;
        }

        * ppIStreamInst = NULL;

        return ST_OK;
    }

    virtual
    ST_ERROR
    M_Fn_ST_CreateStreamWriterToFile_Crc
    (
        const char* szFilename,
        struct LH_OSTREAM_INTERFACE_S* pOStreamInterface,
        void** ppOStreamInst
    )
    {
        if (M_Fn_Check_Trigger(E_Param_ST_CreateStreamWriterToFile_Crc_ERROR)) {
            return ST_E_ARG;
        }

        * ppOStreamInst = NULL;

        return ST_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_CreateStreamInfo
    (
        LH_COMPONENT hComp,
        LH_ISTREAM_INTERFACE *pIStreamInterface,
        void *pIStreamInst,
        LH_ISTREAM_PARAMS *pIStreamParams,
        LH_OBJECT *phStreamInfo
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_CreateStreamInfo_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_AcModCheckDependency
    (
        LH_OBJECT hAcMod,
        LH_OBJECT hStreamInfo,
        LH_STREAMINFO_DEPENDENCY_STATUS *pStatus
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_AcModCheckDependency_ERROR)) {
            return LH_E_INTERNAL;
        }

        *pStatus = LH_STREAMINFO_DEPENDENCY_STATUS_OK;

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_IdTransLookupLoadData
    (
        LH_OBJECT hIdTransLookup,
        LH_ISTREAM_INTERFACE * pIStreamInterface,
        void * pIStreamInst,
        LH_ISTREAM_PARAMS * pIStreamParams,
        const char * szCompatibilityString,
        LH_IDLOOKUP_STATUS * pStatus
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_IdTransLookupLoadData_ERROR)) {
            return LH_E_INTERNAL;
        }

        pStatus->loaded = LH_FALSE;

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_IdTransLookupUnloadData(LH_OBJECT hIdTransLookup)
    {
        if (M_Fn_Check_Trigger(E_Param_LH_IdTransLookupUnloadData_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_IdTransLookupFetchTranscriptions
    (
        LH_OBJECT hIdTransLookup,
        const LH_U64_STRUCT id,
        LH_TRANSCRIPTION ** ppTrans,
        size_t * pNbrTrans
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_IdTransLookupFetchTranscriptions_ERROR)) {
            return LH_E_INTERNAL;
        }

        *ppTrans = NULL;
        *pNbrTrans = 0;

        return LH_OK;
    }

    virtual
    LH_ERROR
    M_Fn_LH_IdTransLookupReturnTranscriptions
    (
        LH_OBJECT hIdTransLookup,
        LH_TRANSCRIPTION * pTrans
    )
    {
        if (M_Fn_Check_Trigger(E_Param_LH_IdTransLookupReturnTranscriptions_ERROR)) {
            return LH_E_INTERNAL;
        }

        return LH_OK;
    }
};

extern C_Vocon_Mock g_o_Vocon_Mock;

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
