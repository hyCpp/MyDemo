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
 * @file Voice_Asr_Impl_Vocon.h
 * @brief C_Vocon class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Vocon_Vocon_h__
#define __Vr_Asr_Impl_Vocon_Vocon_h__

#ifdef _NUANCE

#include "vocon3200_base.h"
#include "vocon3200_asr.h"
#include "vocon3200_pron.h"
#include "vocon_ext_heap.h"
#include "vocon_ext_stream.h"
#include "vocon_ext_audioin.h"
#include "Vr_Asr_Impl_Vocon_Fn_Iostream.h"

#if defined(_WIN32) || defined(_WIN32_WCE)
#pragma comment(lib, "vocon3200_asr.lib")
#pragma comment(lib, "vocon3200_base.lib")
#pragma comment(lib, "vocon3200_g2p.lib")
#pragma comment(lib, "vocon3200_gram2.lib")
#pragma comment(lib, "vocon3200_gram.lib")
#pragma comment(lib, "vocon3200_pron.lib")
#pragma comment(lib, "vocon3200_sem.lib")
#pragma comment(lib, "vocon_ext_asr2sem.lib")
#pragma comment(lib, "vocon_ext_audioin.lib")
#pragma comment(lib, "vocon_ext_heap.lib")
#pragma comment(lib, "vocon_ext_stream.lib")
#else
#endif

#endif


namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

class C_Vocon;

extern scoped_ptr<C_Vocon> g_scp_o_Vocon;

inline
C_Vocon &
G_o_Vocon()
{
    return *g_scp_o_Vocon;
}

/**
 * @brief The C_Vocon class
 *
 * C_Vocon class
 */

class C_Vocon
{
  public:

    virtual
    ~C_Vocon(); // [F]Destructor

    explicit
    C_Vocon(); // [F]Constructor of Default

    virtual
    LH_COMPONENT
    M_Fn_LH_GetNullComponent
    (
        void
    )
    {
        return lh_GetNullComponent();
    }

    virtual
    LH_BOOL
    M_Fn_LH_ComponentIsNull
    (
        LH_COMPONENT hComponent
    )
    {
        return lh_ComponentIsNull(hComponent);
    }

    virtual
    LH_ERROR
    M_Fn_LH_ComponentIsInErrorState
    (
        LH_COMPONENT hComponent,
        LH_BOOL* pIsInErrorState
    )
    {
        return lh_ComponentIsInErrorState(hComponent, pIsInErrorState);
    }

    virtual
    LH_ERROR
    M_Fn_LH_ComponentTerminate
    (
        LH_COMPONENT* phComponent
    )
    {
        return lh_ComponentTerminate(phComponent);
    }

    virtual
    LH_OBJECT
    M_Fn_LH_GetNullObj
    (
        void
    )
    {
        return lh_GetNullObj();
    }

    virtual
    LH_BOOL
    M_Fn_LH_ObjIsNull
    (
        LH_OBJECT hObj
    )
    {
        return lh_ObjIsNull(hObj);
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
        return lh_ObjHasInterface(hObj, interfaceID, pHasInterface);
    }

    virtual
    LH_ERROR
    M_Fn_LH_ObjClose
    (
        LH_OBJECT* phObj
    )
    {
        return lh_ObjClose(phObj);
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
        return lh_ArchivableArchive(hArchivable, pOStreamInterface, pOStreamInst, pOStreamParams);
    }

    virtual
    LH_ERROR
    M_Fn_LH_JSONParserFetchString
    (
        LH_OBJECT hJSONParser,
        char** pszJSONString
    )
    {
        return lh_JSONParserFetchString(hJSONParser, pszJSONString);
    }

    virtual
    LH_ERROR
    M_Fn_LH_JSONParserReturnString
    (
        LH_OBJECT hJSONParser,
        char* szJSONString
    )
    {
        return lh_JSONParserReturnString(hJSONParser, szJSONString);
    }

    virtual
    LH_ERROR
    M_Fn_LH_ConfigGetParam
    (
        LH_OBJECT hConfig,
        unsigned long paramId,
        signed long* pParamValue
    )
    {
        return lh_ConfigGetParam(hConfig, paramId, pParamValue);
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
        return lh_ConfigSetParam(hConfig, paramId, paramValue);
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
        return lh_InitBase(pHeapInterface, pHeapInst, errorCallbackFn, pErrorCallbackInst, phCBase);
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
        return lh_CreateAcMod(hCAsr, pIStreamInterface, pIStreamInst, pIStreamParams, phAcMod);
    }

    virtual
    LH_ERROR
    M_Fn_LH_AcModBorrowInfo
    (
        LH_OBJECT hAcMod,
        struct LH_ACMOD_INFO_S** ppAcModInfo
    )
    {
        return lh_AcModBorrowInfo(hAcMod, ppAcModInfo);
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
        return lh_AcModSetSpeakerProfile(hAcMod, pIStreamInterface, pIStreamInst, pIStreamParams);
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
        return lh_AcModArchiveSpeakerProfile(hAcMod, pOStreamInterface, pOStreamInst, pOStreamParams);
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
        return lh_InitAsr(hCBase, pHeapInterface, pHeapInst, phCAsr);
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
        return lh_CreateAudioTypeSamples(hCAsr, sampleFreq, phAudioType);
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
        return lh_CreateAudioTypeFromSSE(hCAsr, pSSEAudioTypeData, dataSize, phAudioType);
    }

    virtual
    LH_ERROR
    M_Fn_LH_CreateFx
    (
        LH_COMPONENT hCAsr,
        LH_OBJECT* phFx
    )
    {
        return lh_CreateFx(hCAsr, phFx);
    }

    virtual
    LH_ERROR
    M_Fn_LH_FxGetAbnormCondition
    (
        LH_OBJECT hFx,
        LH_FX_ABNORMCOND *pAbnormCondition
    )
    {
        return lh_FxGetAbnormCondition(hFx, pAbnormCondition);
    }

    virtual
    LH_ERROR
    M_Fn_LH_FxGetSignalLevels
    (
        LH_OBJECT hFx,
        LH_FX_SIGNAL_LEVELS *pSignalLevels
    )
    {
        return lh_FxGetSignalLevels(hFx, pSignalLevels);
    }

    virtual
    LH_ERROR
    M_Fn_LH_FxConfigure
    (
        LH_OBJECT hFx,
        LH_OBJECT hFxConfig
    )
    {
        return lh_FxConfigure(hFx, hFxConfig);
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
        return lh_EnvAdaptReset(hObject, pIStreamInterface, pIStreamInst, pIStreamParams);
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
        return lh_EnvAdaptArchive(hObject, pOStreamInterface, pOStreamInst, pOStreamParams);
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
        return lh_CreateSingleThreadRec(hCAsr, pEventInterface, pEventInst, bufferSizeMs, hAudioType, phSingleThreadRec);
    }

    virtual
    LH_ERROR
    M_Fn_LH_SingleThreadRecControlTimeOut
    (
        LH_OBJECT hSingleThreadRec,
        LH_AUDIOSOURCE_TIMEOUTMODE TimeOutMode
    )
    {
        return lh_SingleThreadRecControlTimeOut(hSingleThreadRec, TimeOutMode);
    }

    virtual
    LH_ERROR
    M_Fn_LH_SingleThreadRecSetAudioType
    (
        LH_OBJECT hSingleThreadRec,
        LH_OBJECT hAudioType
    )
    {
        return lh_SingleThreadRecSetAudioType(hSingleThreadRec, hAudioType);
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
        return lh_SingleThreadRecProcess(hSingleThreadRec, pAudioInInterface, pAudioInInst);
    }

    virtual
    LH_ERROR
    M_Fn_LH_ActivatableActivateRule
    (
        LH_OBJECT hActivatable,
        const char* szRule
    )
    {
        return lh_ActivatableActivateRule(hActivatable, szRule);
    }

    virtual
    LH_ERROR
    M_Fn_LH_ActivatableDeactivateRule
    (
        LH_OBJECT hActivatable,
        const char* szRule
    )
    {
        return lh_ActivatableDeactivateRule(hActivatable, szRule);
    }

    virtual
    LH_ERROR
    M_Fn_LH_ActivatableDeactivateAllRules
    (
        LH_OBJECT hActivatable
    )
    {
        return lh_ActivatableDeactivateAllRules(hActivatable);
    }

    virtual
    LH_ERROR
    M_Fn_LH_RecAddCtx
    (
        LH_OBJECT hRec,
        LH_OBJECT hCtx
    )
    {
        return lh_RecAddCtx(hRec, hCtx);
    }

    virtual
    LH_ERROR
    M_Fn_LH_RecRemoveCtx
    (
        LH_OBJECT hRec,
        LH_OBJECT hCtx
    )
    {
        return lh_RecRemoveCtx(hRec, hCtx);
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
        return lh_HostCtxSetCtx2(hHostCtx, szSlotAssignments, hGuestCtx, pStatus);
    }

    virtual
    LH_ERROR
    M_Fn_LH_HostCtxClearCtx
    (
        LH_OBJECT hHostCtx,
        const char* szSlotName
    )
    {
        return lh_HostCtxClearCtx(hHostCtx, szSlotName);
    }

    virtual
    LH_ERROR
    M_Fn_LH_MergeCtxMergeCtx
    (
        LH_OBJECT hMergeCtx,
        LH_OBJECT hGuestCtx
    )
    {
        return lh_MergeCtxMergeCtx(hMergeCtx, hGuestCtx);
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
        return lh_MergeCtxCanMergeCtx(hMergeCtx, hGuestCtx, pCanMerge);
    }

    virtual
    LH_ERROR
    M_Fn_LH_MergeCtxClearMerges
    (
        LH_OBJECT hMergeCtx
    )
    {
        return lh_MergeCtxClearMerges(hMergeCtx);
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
        return lh_CreateCtx(hCAsr, pIStreamInterface, pIStreamInst, pIStreamParams, hAcMod, phCtx);
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
        return lh_CreateUswCtx(hCAsr, hAcMod, phUswCtx);
    }

    virtual
    LH_ERROR
    M_Fn_LH_RecCreateResult
    (
        LH_OBJECT hRec,
        LH_OBJECT *phResult
    )
    {
        return lh_RecCreateResult(hRec, phResult);
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
        return lh_FieldCtxSetStartStop(hFieldCtx, szStartField, szStopField, activationFlag);
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
        return lh_FieldCtxActivateUserId(hFieldCtx, szStartField, szActivateFromField, pUserIds, nbrUserIds);
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
        return lh_CreateEmptyListCtx(hCAsr, hAcMod, flags, range, phListCtx);
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
        return lh_ListCtxAddTermSeq(hListCtx, pTerminals, nbrTerminals, pStatus);
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
        return lh_ListCtxRemoveAlternative(hListCtx, pTerminals, nbrTerminals, matchType, pNbrMatched);
    }

    virtual
    LH_ERROR
    M_Fn_LH_ListCtxCommit
    (
        LH_OBJECT hListCtx
    )
    {
        return lh_ListCtxCommit(hListCtx);
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
        return lh_TableCtxActivate(hTableCtx, szQuery, activationValue, pStatus);
    }

    virtual
    LH_ERROR
    M_Fn_LH_UswResultGetAcceptanceStatus
    (
        LH_OBJECT hUswResult,
        LH_USW_ACCEPTSTATUS* pStatus
    )
    {
        return lh_UswResultGetAcceptanceStatus(hUswResult, pStatus);
    }

    virtual
    LH_ERROR
    M_Fn_LH_UswResultFetchTranscription
    (
        LH_OBJECT hUswResult,
        LH_TRANSCRIPTION **ppTranscription
    )
    {
        return lh_UswResultFetchTranscription(hUswResult, ppTranscription);
    }

    virtual
    LH_ERROR
    M_Fn_LH_UswResultReturnTranscription
    (
        LH_OBJECT hUswResult,
        LH_TRANSCRIPTION *pTranscription
    )
    {
        return lh_UswResultReturnTranscription(hUswResult, pTranscription);
    }

    virtual
    LH_ERROR
    M_Fn_LH_UswResultHasAudioBuffer
    (
        LH_OBJECT hUswResult,
        LH_BOOL* pHasAudioBuffer
    )
    {
        return lh_UswResultHasAudioBuffer(hUswResult, pHasAudioBuffer);
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
        return lh_UswResultFetchAudioBuffer(hUswResult, pFrequency, ppBuffer, pSize);
    }

    virtual
    LH_ERROR
    M_Fn_LH_UswResultReturnAudioBuffer
    (
        LH_OBJECT hUswResult,
        void*     pBuffer
    )
    {
        return lh_UswResultReturnAudioBuffer(hUswResult, pBuffer);
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
        return lh_InitPron(hCBase, pHeapInterface, pHeapInst, phCPron);
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
        return lh_CreateMLCLC(hCPron, pIStreamInterface, pIStreamInst, pIStreamParams, pTranscriptionsSpec, phCLC);
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
        return lh_CLCBorrowDomainCategories(hCLC, ppszDomainCategories, pNbrCategories);
    }

    virtual
    LH_ERROR
    M_Fn_LH_CLCFetchInputLanguages
    (
        LH_OBJECT  hCLC,
        char ***  ppszInputLanguages,
        size_t *  pNbrInputLanguages
    )
    {
        return lh_CLCFetchInputLanguages(hCLC, ppszInputLanguages, pNbrInputLanguages);
    }

    virtual
    LH_ERROR
    M_Fn_LH_CLCReturnInputLanguages
    (
        LH_OBJECT  hCLC,
        char **  pszInputLanguages
    )
    {
        return lh_CLCReturnInputLanguages(hCLC, pszInputLanguages);
    }

    virtual
    LH_ERROR
    M_Fn_LH_CLCSetCurrentDomainCategory
    (
        LH_OBJECT hCLC,
        const char * szCurrentDomainCategory
    )
    {
        return lh_CLCSetCurrentDomainCategory(hCLC, szCurrentDomainCategory);
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
        return lh_CreateLex(hCPron, pTranscriptionsSpec, phLex);
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
        return lh_LexSetG2P(hLex, hG2P, hCache);
    }

    virtual
    LH_ERROR
    M_Fn_LH_LexClearG2P
    (
        LH_OBJECT hLex
    )
    {
        return lh_LexClearG2P(hLex);
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
        return lh_TransLookUpFetchTranscriptions(hTransLookUp, szWord, ppTranscriptions, pNbrTranscriptions);
    }

    virtual
    LH_ERROR
    M_Fn_LH_TransLookUpReturnTranscriptions
    (
        LH_OBJECT hTransLookUp,
        LH_TRANSCRIPTION* pTranscriptions
    )
    {
        return lh_TransLookUpReturnTranscriptions(hTransLookUp, pTranscriptions);
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
        return ph_DlHeapCreate(pHeapParams, pHeapInterface, ppHeapInst);
    }

    virtual
    PH_ERROR
    M_Fn_PH_DlHeapClose
    (
        void** ppHeapInst
    )
    {
        return ph_DlHeapClose(ppHeapInst);
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
        return st_CreateStreamReaderFromFile(szFilename, pIStreamInterface, ppIStreamInst);
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
        return st_CreateStreamWriterToFile(szFilename, pOStreamInterface, ppOStreamInst);
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
        return Fn_ST_CreateStreamReaderFromFile(szFilename, pIStreamInterface, ppIStreamInst);
    }

    virtual
    ST_ERROR
    M_Fn_ST_CreateStreamWriterToFile_Crc
    (
        const char* szFilename,
        struct LH_OSTREAM_INTERFACE_S* pOStreamInterface,
        C_File_Info& ro_File_Info
    )
    {
        return Fn_ST_CreateStreamWriterToFile(szFilename, pOStreamInterface, ro_File_Info);
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
        return lh_CreateStreamInfo(hComp, pIStreamInterface, pIStreamInst, pIStreamParams, phStreamInfo);
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
        return lh_AcModCheckDependency(hAcMod, hStreamInfo, pStatus);
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
        return lh_IdTransLookupLoadData
                (
                    hIdTransLookup,
                    pIStreamInterface,
                    pIStreamInst,
                    pIStreamParams,
                    szCompatibilityString,
                    pStatus
                );
    }

    virtual
    LH_ERROR
    M_Fn_LH_IdTransLookupUnloadData(LH_OBJECT hIdTransLookup)
    {
        return lh_IdTransLookupUnloadData(hIdTransLookup);
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
        return lh_IdTransLookupFetchTranscriptions(hIdTransLookup, id, ppTrans, pNbrTrans);
    }

    virtual
    LH_ERROR
    M_Fn_LH_IdTransLookupReturnTranscriptions
    (
        LH_OBJECT hIdTransLookup,
        LH_TRANSCRIPTION * pTrans
    )
    {
        return lh_IdTransLookupReturnTranscriptions(hIdTransLookup, pTrans);
    }

    virtual
    LH_ERROR
    M_Fn_LH_UswResultCreateNBestResultFromConfusion
    (
        LH_OBJECT hUswResult,
        LH_OBJECT hCtx,
        LH_OBJECT* phNBestResult
    )
    {
        return lh_UswResultCreateNBestResultFromConfusion(hUswResult, hCtx, phNBestResult);
    }

    virtual
    LH_ERROR
    M_Fn_LH_ActivatableFetchRules
    (
        LH_OBJECT hActivatable,
        unsigned long ruleType,
        const char *** ppszRules,
        size_t * pNbrOfRules
    )
    {
        return lh_ActivatableFetchRules(hActivatable, ruleType, ppszRules, pNbrOfRules);
    }

    virtual
    LH_ERROR
    M_Fn_LH_ActivatableReturnRules
    (
        LH_OBJECT hActivatable,
        const char ** pszRules
    )
    {
        return lh_ActivatableReturnRules(hActivatable, pszRules);
    }

    virtual
    LH_ERROR
    M_Fn_LH_TransSpecFetchValue
    (
        LH_OBJECT hTransSpec,
        LH_TRANSCRIPTION_SPEC ** ppTranscriptionSpec
    )
    {
        return lh_TransSpecFetchValue(hTransSpec, ppTranscriptionSpec);
    }

    virtual
    LH_ERROR
    M_Fn_LH_TransSpecReturnValue
    (
        LH_OBJECT hTransSpec,
        LH_TRANSCRIPTION_SPEC * pTranscriptionSpec
    )
    {
        return lh_TransSpecReturnValue(hTransSpec, pTranscriptionSpec);
    }

    virtual
    LH_ERROR
    M_Fn_LH_NBestResultBorrowScores
    (
        LH_OBJECT  hNBestResult,
        unsigned long **  ppScores,
        size_t *  pNbrScores
    )
    {
        return lh_NBestResultBorrowScores(hNBestResult, ppScores, pNbrScores);
    }

    virtual
    LH_ERROR
    M_Fn_LH_JSONParserSetErrorCallback
    (
        LH_OBJECT hJSONParser,
        LH_JSON_ERROR_CALLBACK_FN pfParseErrorCallback,
        void *pParseErrorCallbackInst
    )
    {
        return lh_JSONParserSetErrorCallback(hJSONParser, pfParseErrorCallback, pParseErrorCallbackInst);
    }

    virtual
    LH_ERROR
    M_Fn_LH_JSONPullParserCreateCursor
    (
        LH_OBJECT hJSONParser,
        LH_OBJECT * phCursor
    )
    {
        return lh_JSONPullParserCreateCursor(hJSONParser, phCursor);
    }

    virtual
    LH_ERROR
    M_Fn_LH_JSONCursorHasNext
    (
        LH_OBJECT hJSONCursor,
        LH_BOOL * pHasNext
    )
    {
        return lh_JSONCursorHasNext(hJSONCursor, pHasNext);
    }

    virtual
    LH_ERROR
    M_Fn_LH_JSONCursorNext(LH_OBJECT hJSONCursor)
    {
        return lh_JSONCursorNext(hJSONCursor);
    }

    virtual
    LH_ERROR
    M_Fn_LH_JSONCursorGetType
    (
        LH_OBJECT hJSONCursor,
        LH_JSONCURSOR_TYPE * pType
    )
    {
        return lh_JSONCursorGetType(hJSONCursor, pType);
    }

    virtual
    LH_ERROR
    M_Fn_LH_JSONCursorAdvanceTo
    (
        LH_OBJECT hJSONCursor,
        const char *szKey,
        size_t *pIdentifier,
        LH_BOOL *pFound
    )
    {
        return lh_JSONCursorAdvanceTo(hJSONCursor, szKey, pIdentifier, pFound);
    }

    virtual
    LH_ERROR
    M_Fn_LH_JSONCursorAdvanceToNextArrayItem
    (
        LH_OBJECT hJSONCursor,
        size_t arrayIdentifier
    )
    {
        return lh_JSONCursorAdvanceToNextArrayItem(hJSONCursor, arrayIdentifier);
    }

    virtual
    LH_ERROR
    M_Fn_LH_JSONCursorArrayGetSize
    (
        LH_OBJECT hJSONCursor,
        size_t *pNbrItems
    )
    {
        return lh_JSONCursorArrayGetSize(hJSONCursor, pNbrItems);
    }

    virtual
    LH_ERROR
    M_Fn_LH_JSONCursorObjectGetNumberValue
    (
        LH_OBJECT hJSONCursor,
        const char *szKey,
        signed int *pNumber,
        LH_BOOL *pFound
    )
    {
        return lh_JSONCursorObjectGetNumberValue(hJSONCursor, szKey, pNumber, pFound);
    }

    virtual
    LH_ERROR
    M_Fn_LH_JSONCursorObjectBorrowStringValue
    (
        LH_OBJECT hJSONCursor,
        const char *szKey,
        char **pszString,
        LH_BOOL *pFound
    )
    {
        return lh_JSONCursorObjectBorrowStringValue(hJSONCursor, szKey, pszString, pFound);
    }
};

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
