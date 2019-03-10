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
 * @file VR_VBT_VECIEngineCommand_mock.h
 * @brief Mock class for IVECIEngineCommand.
 *
 *
 * @attention used for C++ only.
 */

#ifndef CXX_VR_VBT_VECIENGINECOMMAND_MOCK_H
#define CXX_VR_VBT_VECIENGINECOMMAND_MOCK_H

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "IVBTEngineClient.h"

/**
* MockVR_VBT_VECIEngineCommand
*
* The class is a mock class for IVECIEngineCommand.
*/
class MockVR_VBT_VECIEngineCommand : public IVECIEngineCommand
{
public:
    // Implementation for IUnknown's functions
    HRESULT QueryInterface(REFIID riid, void** ppvObj)
    {
        return S_OK;
    }

    VBT_ULONG AddRef()
    {
        return 1;
    }

    VBT_ULONG Release()
    {
        return 0;
    }

    // Mock methods
    MOCK_METHOD2(StartEngine, HRESULT(
                IVECITransaction **ppTrans,
                VBT_CSTR szCulture));
    MOCK_METHOD2(StopEngine, HRESULT(
                IVECITransaction **ppTrans,
                VBT_BOOL bImmediate));
    MOCK_METHOD2(RestartEngine, HRESULT(
                IVECITransaction **ppTrans,
                VBT_CSTR szCulture));
    MOCK_METHOD2(SendXmlMessage, HRESULT(
                IVECITransaction **ppTrans,
                VBT_CSTR szMessage));
    MOCK_METHOD6(SendCommand, HRESULT(
                IVECITransaction **ppTrans,
                VBT_CSTR szAgent,
                VBT_CSTR szCommand,
                VBT_BOOL bSilent,
                IVECIParameterSet *pCriteria,
                IVECIParameterSet *pContext));
    MOCK_METHOD3(SendAudio, HRESULT(
                IVECITransaction **ppTrans,
                VBT_CSTR szFileName,
                VBT_CSTR szFileType));
    MOCK_METHOD2(SendText, HRESULT(
                IVECITransaction **ppTrans,
                VBT_CSTR szText));
    MOCK_METHOD2(StartRecoSession, HRESULT(
                IVECITransaction **ppTrans,
                VBT_BOOL bContinuous));
    MOCK_METHOD4(StartRecoSessionWithBargeIn, HRESULT(
                IVECITransaction **ppTrans,
                VBT_BOOL bContinuous,
                VBT_ULONG pData,
                VBT_UINT uSize));
    MOCK_METHOD2(Cancel, HRESULT(
                IVECITransaction **ppTrans,
                VBT_BOOL bClearQueues));
    MOCK_METHOD1(CancelSession, HRESULT(IVECITransaction **ppTrans));
    MOCK_METHOD2(CancelSpecific, HRESULT(
                IVECITransaction **ppTrans,
                VBT_CSTR szPatterns));
    MOCK_METHOD2(Reset, HRESULT(
                IVECITransaction **ppTrans,
                VBT_BOOL bRemovePseudoRootContexts));
    MOCK_METHOD3(ClearContexts, HRESULT(
                IVECITransaction **ppTrans,
                VBT_BOOL bRemovePseudoRootContexts,
                VBT_BOOL bRemoveCachedContexts));
    MOCK_METHOD5(StartSpeak, HRESULT(
                IVECITransaction **ppTrans,
                VBT_CSTR szText,
                VBT_BOOL bWriteToFile,
                VBT_CSTR szAudioType,
                VBT_CSTR szLocation));
    MOCK_METHOD1(PauseSpeak, HRESULT(IVECITransaction **ppTrans));
    MOCK_METHOD1(ResumeSpeak, HRESULT(IVECITransaction **ppTrans));
    MOCK_METHOD4(SetPreference, HRESULT(
                IVECITransaction **ppTrans,
                VBT_CSTR szAgent,
                VBT_CSTR szName,
                VBT_CSTR szValue));
    MOCK_METHOD3(GetPreference, HRESULT(
                IVECITransaction **ppTrans,
                VBT_CSTR szAgent,
                VBT_CSTR szName));
    MOCK_METHOD4(SetSystemParameter, HRESULT(
                IVECITransaction **ppTrans,
                VBT_CSTR szSection,
                VBT_CSTR szParamName,
                VBT_CSTR szParamValue));
    MOCK_METHOD3(GetSystemParameter, HRESULT(
                IVECITransaction **ppTrans,
                VBT_CSTR szSection,
                VBT_CSTR szParamName));
    MOCK_METHOD4(SendContext, HRESULT(
                IVECITransaction **ppTrans,
                VBT_CSTR szAgent,
                VBT_CSTR szContext,
                IVECIParameterSet *pContextVars));
    MOCK_METHOD2(SendCachedContext, HRESULT(
                IVECITransaction **ppTrans,
                VBT_INT nCachedContextId));
    MOCK_METHOD3(RemoveContext, HRESULT(
                IVECITransaction **ppTrans,
                VBT_CSTR szAgent,
                VBT_CSTR szContext));
    MOCK_METHOD1(GetContext, HRESULT(IVECITransaction **ppTrans));
    MOCK_METHOD1(GetCultures, HRESULT(IVECITransaction **ppTrans));
    MOCK_METHOD1(GetAgents, HRESULT(IVECITransaction **ppTrans));
    MOCK_METHOD1(GetVersion, HRESULT(IVECITransaction **ppTrans));
    MOCK_METHOD2(Mute, HRESULT(
                IVECITransaction **ppTrans,
                VBT_BOOL bMuteOn));
    MOCK_METHOD6(SendRecognitionState, HRESULT(
                IVECITransaction **ppTrans,
                VBT_CSTR szStateName,
                VeciBoolParam enClearContexts,
                VBT_BOOL bPseudoRoot,
                IVECIParameterSet *pVariables,
                IVECIListItems *pOptionalContextList));
    MOCK_METHOD2(SelectListItem, HRESULT(
                IVECITransaction **ppTrans,
                VBT_ULONG ulSelectionIndex));
    MOCK_METHOD3(Back, HRESULT(
                IVECITransaction **ppTrans,
                VBT_BOOL bSpeak,
                IVECITransaction **ppPrevTrans));
    MOCK_METHOD3(Repeat, HRESULT(
                IVECITransaction **ppTrans,
                VBT_BOOL bSpeak,
                IVECITransaction **ppLastTrans));
    MOCK_METHOD2(Resume, HRESULT(
                IVECITransaction **ppTrans,
                VBT_BOOL bContinuous));
    MOCK_METHOD4(QueueWorkItem, HRESULT(
                IVECITransaction **ppTrans,
                VBT_ULONG ulItemId,
                IUnknown *pData,
                IVECIParameterSet *pParams));
    MOCK_METHOD0(BeepDone, HRESULT());
    MOCK_METHOD5(DriverActionReply, HRESULT(
                VBT_CSTR szAgentName,
                VBT_CSTR szActionType,
                IVECIParameterSet *pParams,
                VBT_CSTR szListType,
                IVECIListItems *pListItems));
    MOCK_METHOD3(DriverListReply, HRESULT(
                VBT_CSTR szAgentName,
                VBT_CSTR szListType,
                IVECIListItems *pItems));
    MOCK_METHOD1(DriverXmlReply, HRESULT(VBT_CSTR szMessage));
    MOCK_METHOD5(InstallDownloadableAgent, HRESULT(
                IVECITransaction **ppTrans,
                VBT_CSTR szInstallPath,
                VBT_CSTR szTempPath,
                VBT_CSTR szCulture,
                IVECIStringSet *pDAPFiles));
    MOCK_METHOD4(UninstallDownloadableAgent, HRESULT(
                IVECITransaction **ppTrans,
                VBT_CSTR szInstallPath,
                VBT_CSTR szCulture,
                IVECIStringSet *pAgentNames));
    MOCK_METHOD3(UninstallAllDownloadableAgent, HRESULT(
                IVECITransaction **ppTrans,
                VBT_CSTR szInstallPath,
                VBT_CSTR szCulture));
    MOCK_METHOD4(GetRecognitionStateHints, HRESULT(
                VBT_CSTR szStateName,
                VBT_INT nScreenId,
                IVECIListItems *pOptionalContextList,
                IVECIStringSet **ppHints));
    MOCK_METHOD2(LoadSpeakerProfile, HRESULT(
                IVECITransaction **ppTrans,
                VBT_CSTR strProfileFilename));
    MOCK_METHOD2(SaveSpeakerProfile, HRESULT(
                IVECITransaction **ppTrans,
                VBT_CSTR strProfileFilename));
    MOCK_METHOD1(ResetSpeakerProfile, HRESULT(IVECITransaction **ppTrans));
    MOCK_METHOD1(PreloadGrammar, HRESULT(IVECITransaction **ppTrans));
};

#endif  // CXX_VR_VBT_VECIENGINECOMMAND_MOCK_H
/* EOF */
