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
 * @file VR_AlexaEngine.h
 * @brief dialog engine class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef VR_ALEXAENGINE_H
#define VR_ALEXAENGINE_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/* Standard Header */
#include <string>
#include <memory>

/* Suntec Header */
#ifndef VR_DEF_H
#    include "VR_Def.h"
#endif

#ifndef VR_DEPROCESSOR_H
#    include "VR_DEProcessor.h"
#endif

#ifndef CXX_BL_SyncObject_H
#   include "BL_SyncObject.h"
#endif

/* Forward Declaration */

class VR_AlexaEngine;
class VR_DialogEngineListener;
class VR_Settings;
class VR_XmlParser;
class VR_AlexaImpl;
class VR_GoogleImpl;
class VR_BaiduImpl;
class VR_ControllerBase;
class VR_AlexaJSonParser;
class VR_GoogleObserver;
class VR_BaiduObserver;
class AbstractSDS;
class VR_BaiduSDS;
class VR_DictationManager;

#ifndef ALEXA_AUDIO_MICROPHONE_H_
#    include "AudioMicrophone.h"
#endif


// Define the action-function map
typedef void (VR_AlexaEngine::* MessageHandler)(VR_XmlParser&);
typedef void (VR_AlexaEngine::* AlexaMsgHandler)(VR_AlexaJSonParser&);

/**
 * @brief The VR_AlexaEngine class
 *
 * dialog controller class
 */
class VR_AlexaEngine : public VR_DEProcessor
{
public:
    VR_AlexaEngine();
    virtual ~VR_AlexaEngine();

    virtual bool Initialize(VR_DialogEngineListener* listerner, VR_ControllerBase* controller);
    virtual std::string getHints(const std::string& hintsParams);
    virtual bool Start();
    virtual void Stop();
    virtual void UnInitialize();

    /**
     * ProcessMessage
     *
     * Process Voice Recognition related messages
     *
     * @param none
     * @return none
     * @attention none
     * @see none
     */
    virtual void ProcessMessage(const std::string& message, int actionSeqId  = -1);

private:
    void DummyProcess(VR_XmlParser& parser);
    void StartDialog(VR_XmlParser& parser);
    void Cancel(VR_XmlParser& parser);
    void OnDMStartedNotify(VR_XmlParser& parser);
    void OnBeepDone(VR_XmlParser& parser);
    void OnAudioFile(VR_XmlParser& parser);
    void OnAudioInOpe(VR_XmlParser& parser);
    void OnCloseSuggestSession(VR_XmlParser& parser);
    void AudioInOpen();
    void AudioInClose();
    void OnkeyWordDetected(VR_XmlParser& parser);
    void SetAwakenHints(VR_XmlParser& parser);
    void OnSetAECStatus(VR_XmlParser& parser);
    void OnDictation(VR_XmlParser& parser);
    void OnDictationRecognizeResult(VR_XmlParser& parser);

    // handle the message of Alexa
    void OnBodyTemplate2(VR_AlexaJSonParser& JParser);
    void OnChangeTemperature(VR_AlexaJSonParser& JParser);
    void OnPlayRadio(VR_AlexaJSonParser& JParser);
    void OnNaviToDestination(VR_AlexaJSonParser& JParser);
    void OnGetTravelTime(VR_AlexaJSonParser& JParser);
    void OnShowPhoneCall(VR_AlexaJSonParser& JParser);
    void OnShowContactList(VR_AlexaJSonParser& JParser);
    void OnShowPhoneList(VR_AlexaJSonParser& JParser);
    void OnMakePhoneCall(VR_AlexaJSonParser& JParser);
    void OnRoofControl(VR_AlexaJSonParser& JParser);
    void OnAutoStarStop(VR_AlexaJSonParser& JParser);
    void OnSetDestinationReq(VR_AlexaJSonParser& JParser);
    void OnStop(VR_AlexaJSonParser& JParser);
    void OnFindPlace(VR_AlexaJSonParser& JParser);
    void OnWeatherTemplate(VR_AlexaJSonParser& JParser);
    void OnDirveScore(VR_AlexaJSonParser& JParser);

    void SendDEStatus(const std::string& strType, const std::string& strStatus);
    void SendVRDictationState(const std::string& engineType, const std::string& state);
    void SendVRState(const std::string& engineType, const std::string& state);
    int  OnRequestAction(const std::string& strMsg);

    void PlayBeep(const std::string& strBeepFileName);

    void AddResultToNode(std::string& strDestxml, const std::string& strSourxml, bool isAppendErrCode);
    void BuildGrammarResultXML(const std::string& strGrammarxml, std::string& strGrammarResultxml);

    std::string& replace_all_distinct(std::string& str, const std::string& old_value, const std::string& new_value);

    int GetPictureId();

    void SendPlayStateToDM(const std::string& message);
    void SendPlayStateToUI(const std::string& message);

private:
    VR_AlexaImpl*                                            m_pAlexaImpl;
    VR_GoogleImpl*                                           m_pGoogleImpl;
    VR_BaiduImpl*                                            m_pBaiduImpl;
    AbstractSDS*                                             m_pGoogleAbstractSDS;
    VR_BaiduSDS*                                             m_pBaiduSDS;
    VR_DialogEngineListener*                                 m_listener;
    VR_ControllerBase*                                       m_controller;
    bool                                                     m_bIntialized;
    std::string                                              m_strEngineValue;
    std::shared_ptr<VR_GoogleObserver>                       m_googleObserver;
    std::shared_ptr<VR_BaiduObserver>                        m_baiduObserver;
    std::shared_ptr<VR_DictationManager>                     m_dictationManager;
    alexaClientSDK::sampleApp::AudioMicrophone*              m_audioMicrophone;

    // Action-function map
    VoiceMap<std::string, MessageHandler>::type              m_mapMsgHandler;
    VoiceMap<std::string, AlexaMsgHandler>::type             m_mapAlexaMsgHandler;

    // Used for generating action ID
    int                                                      m_iCurReqId;
    int                                                      m_iPictureId;
    static BL_SyncObject                                     m_ActionIDSync;
    std::string                                              m_strAudioItemId;
    bool                                                     m_bSamemusicScreen;
    bool                                                     m_bStartByKeyword;
    bool                                                     m_bStartByPTT;
    bool                                                     m_bIDLE;
    bool                                                     m_bAlexaSelected;
    bool                                                     m_bBaiduSelected;
    bool                                                     m_bGoogleSelected;
    bool                                                     m_bSuggestion;
};

#endif
/* EOF */
