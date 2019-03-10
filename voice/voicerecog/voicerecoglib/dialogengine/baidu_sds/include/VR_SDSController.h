#ifndef VR_SDSCONTROLLER_H
#define VR_SDSCONTROLLER_H

#include <string>
#include "VR_DEProcessor.h"
#include "VR_BaiduSDS_IF.h"
#include "VR_XmlParser.h"
#include "VR_ControllerBase.h"
#include "VR_SDSParserConcrete.h"
#include "VR_SDSStateMachine.h"
#include "VR_AsrEngineIF.h"
#include "BL_SyncObject.h"

#include "observerInterface/SpeechCommandObserverInterface.h"
#include "SpeechCommandObserver.h"
#include "SphinxSpeechDetector.h"

using namespace SphinxS;

class VR_SDSController : public VR_DEProcessor
{
public:
    VR_SDSController();
    bool Initialize(VR_BaiduSDSListener* listerner,
                    std::shared_ptr<VR_ControllerBase> controller);
    void UnInitialize();
    bool Start();
    void Stop();

    void ProcessMessage(const std::string& message, int actionSeqId = -1);

    void StartDialogIF();
    void StopDialog();

    int onAudioInData(int16_t* buffer, int32_t size);

private:
    void StartDialog(VR_XmlParser& parser);
    void Cancel(VR_XmlParser& parser);
    void OnDMStartedNotify(VR_XmlParser& parser);
    void OnBeepDone(VR_XmlParser& parser);
    void OnAudioFile(VR_XmlParser& parser);
    void OnAudioInOpe(VR_XmlParser& parser);
    void OnCloseSuggestSession(VR_XmlParser& parser);
    void NotifyEngineAudioOpen();
    void NotifyAudioInClose();
    void OnRecognizeResult(const std::string& queryWord);
    void OnTimeout(const std::string& queryWord);

    void SendDEStatus(const std::string& strType, const std::string& strStatus);
    void SendVRState(const std::string& engineType, const std::string& state);
    int  OnRequestAction(const std::string& strMsg);
    void PlayBeep(const std::string& strBeepFileName);
    void OnPlayTtsEnd(VR_XmlParser&);

    void PlayTTS(const std::string& strTtsContent);
    void StopTTS();

    std::string& replace_all_distinct(std::string& str, const std::string& old_value, const std::string& new_value);

    void SendPlayStateToDM(const std::string& message);
    void SendPlayStateToUI(const std::string& message);

    typedef void (VR_SDSController::* MessageHandler)(VR_XmlParser&);
    typedef void (VR_SDSController::* ServerMsgHandler)(const std::string&);

private:
    // Action-function map
    std::map<std::string, MessageHandler>           m_mapMsgHandler;
    // Callback for baidu engine when there are recognized result
    std::map<std::string, ServerMsgHandler>         m_mapEngineCallback;
    // baidu engine instance
    std::shared_ptr<VR_AsrEngineIF>                 m_pEngine;
    // State machine
    std::shared_ptr<VR_SDSStateMachine>             m_pStateMachine;

    // Sphinx Observer and SpeechDetector
    std::shared_ptr<alexaClientSDK::avsCommon::sdkInterfaces::SpeechCommandObserverInterface> m_speechcommandobserver;
    std::unique_ptr<SphinxSpeechDetector> m_speechdetector;

    VR_BaiduSDSListener* m_listener;
    std::shared_ptr<VR_ControllerBase>       m_controller;
    bool                     m_bInitialized;

    // Used for generating action ID
    int                      m_iCurReqId;
    int                      m_iPictureId;
    static BL_SyncObject     m_ActionIDSync;
    std::string              m_strAudioItemId;
    bool                     m_bSamemusicScreen;
    bool                     m_bStartByKeyword;
    bool                     m_bStartByPTT;

    std::string              m_strCurrentSdsStatus;
    bool                     m_bIsPrompting;

    int                      m_iRecognizeFailCount;
};

#endif // VR_SDSCONTROLLER_H
