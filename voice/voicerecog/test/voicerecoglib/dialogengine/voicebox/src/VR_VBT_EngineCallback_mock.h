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
 * @file VR_VBT_EngineCallback_mock.h
 * @brief Mock class for IVECIEngineClient.
 *
 *
 * @attention used for C++ only.
 */

#ifndef CXX_VR_VBT_ENGINECALLBACK_MOCK_H
#define CXX_VR_VBT_ENGINECALLBACK_MOCK_H

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

#include <string>
#include "gmock/gmock.h"
#include "VR_VoiceBoxEngineCallback.h"

typedef std::pair<std::string, std::string>  pairAgentTransId;
/**
* MockVR_VBT_EngineCallback
*
* The class is a mock class for VR_VoiceBoxEngineCallback.
*/
class MockVR_VBT_EngineCallback : public VR_VoiceBoxEngineCallback
{
public:
    // Mock methods
    MOCK_METHOD1(OnRecognized, void(const std::string& message));
    MOCK_METHOD1(SetUpdateGammarFlg, void(bool bFinishUpdate));
    MOCK_METHOD1(OnInfoQueryMessage, void(const std::string& message));
    MOCK_METHOD1(OnBeep, void(const VR_BeepType& beepType));
    MOCK_METHOD1(OnBeep, void(const std::string& message));
    MOCK_METHOD2(OnSpeak, void(IVECITransaction* pcTrans,
                const std::string& message));
    MOCK_METHOD1(OnRequest, void(const std::string& message));
    MOCK_METHOD1(OnPlayTone, void(const std::string& message));
    MOCK_METHOD1(setActionResultValue, void(const std::string& message));
    MOCK_METHOD3(InsertMusicUpdateGrammar, void(const std::string& strTransId,
                const std::string& strSourceId, const std::string& bLast));
    MOCK_METHOD2(ConfirmMusicUpdateGrammar, void(const std::string& strTransId,
                const bool bGrammarErrorCode));
    MOCK_METHOD1(GetAudioConnected, std::string(const std::string& strAudioName));
    MOCK_METHOD0(GetAudioConnected, std::string());

    MOCK_METHOD1(ChangeAudioSourceName, void(std::string& strAudioName));

    MOCK_METHOD2(GetPromptByScreenId, void(const std::string& strScreenId,
                std::string& strPrompt));
    MOCK_CONST_METHOD0(GetUpdateMapState, bool());
    MOCK_METHOD4(SendGrammarResult, void(const std::string& strOp, const std::string& strAgent,
                const std::string& strGrammarId, const std::string& strErrCode));
    MOCK_METHOD3(SetActiveSouceTrans, void(const std::string& strTransId, const std::string& strOp, const std::string& strSouceId));
    MOCK_METHOD3(GetOpActiveSouce, void(const std::string& strTransId, std::string& strOp, std::string& strActiveSource));
    MOCK_METHOD1(SetNBestScreenFlg, void(const bool& bNBestFlg));
    MOCK_METHOD0(GetNBestScreenFlg, bool());
    MOCK_METHOD0(IsAppsInstalled, bool());
    MOCK_METHOD0(IsAppsEnabled, bool());
    MOCK_METHOD0(GetPhonBookDBPath, std::string());
    MOCK_METHOD0(GetMDActiveSourceId, std::string());
    MOCK_METHOD1(SetGrammarInitSourceId, void(const std::string& strGrammarInitSourceId));
    MOCK_METHOD0(ResetGrammarUpdate, bool());
    MOCK_METHOD0(GetEngineStatus, bool());
    MOCK_METHOD0(GetSendMessageAvailableState, bool());
    MOCK_METHOD2(SetCurrentTransaction, void(const std::string& strAgent, const std::string& strTransId));
    MOCK_METHOD0(GetCurrentTransaction, pairAgentTransId());
    MOCK_METHOD1(SetWaitResumeGrammar, void(bool bWaitResumeGrammar));
    MOCK_METHOD4(SendVRState, void(const std::string& strState, const std::string& strPrompt,
                const std::string& strNBestScreen, const std::string& strResult));
    MOCK_METHOD0(GetRouteStatus, bool());
    MOCK_METHOD0(GetCultureName, std::string());
    MOCK_METHOD1(NotifyActionClass, void(const std::string& strActionClass));
    MOCK_METHOD1(PostCatalogStatus, void(const std::string& strCatalogStatus));
    MOCK_METHOD1(PushFrontMessage, void(const std::string& strCatalogMsg));
    MOCK_METHOD1(SetPhoneDataSyncTransId, void(const std::string& strDataSyncTransId));
    MOCK_METHOD0(GetPhoneDataSyncTransId, std::string());
    MOCK_METHOD0(IsBeingVRSession, bool());
    MOCK_METHOD1(SetPhoneReloadDataFlg, void(bool bPhonePrepareData));
    MOCK_METHOD1(SetNeedGrammarResult, void(bool bNeedGrammarResult));
    MOCK_METHOD0(IsHVACFahrenheit, bool());
    MOCK_METHOD2(SendRequestGrammar, void(const std::string& strAgentName, const std::string& strGrammarID));
    MOCK_METHOD2(SendDEStatus, void(const std::string& strType, const std::string& strStatus));
    MOCK_METHOD0(NotifyPhoneGrammarFinished, void());
};

#endif  // CXX_VR_VBT_ENGINECALLBACK_MOCK_H
/* EOF */
