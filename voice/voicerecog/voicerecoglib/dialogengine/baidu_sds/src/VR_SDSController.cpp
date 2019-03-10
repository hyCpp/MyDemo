/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are NOT permitted except as agreed by
 * Suntec Software(Shanghai) Co., Ltd.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

#include "VR_SDSController.h"
#include "VR_SDSEvent.h"
#include "VR_Log.h"
#include "VR_ConfigureIF.h"
#include "VR_BaiduImpl.h"
#include "BL_AutoSync.h"
#include "VR_SDSAbstractState.h"
#include "VR_SpeechCommandObserver.h"

using namespace std;

BL_SyncObject   VR_SDSController::m_ActionIDSync;   ///< Sync object
VR_SDSController::VR_SDSController()
    : m_listener(nullptr)
    , m_controller(nullptr)
    , m_bInitialized(false)
    , m_iCurReqId(0)
    , m_iPictureId(0)
    , m_bSamemusicScreen(false)
    , m_bStartByKeyword(false)
    , m_bStartByPTT(false)
    , m_strCurrentSdsStatus("")
    , m_bIsPrompting(false)
    , m_iRecognizeFailCount(0)
{
}

bool VR_SDSController::Initialize(VR_BaiduSDSListener* listerner,
                             shared_ptr<VR_ControllerBase> controller)
{
    VR_LOGD_FUNC();

    m_listener = listerner;
    m_controller = controller;
    m_pStateMachine = make_shared<VR_SDSStateMachine>(m_controller);

    // Create the aciton-function map
    m_mapMsgHandler.clear();

    // m_mapMsgHandler.insert(std::make_pair("startedNotify", &VR_SDSController::OnDMStartedNotify));
    // m_mapMsgHandler.insert(std::make_pair("startAgent", &VR_SDSController::StartDialog));
    // m_mapMsgHandler.insert(std::make_pair("cancel", &VR_SDSController::Cancel));
    // m_mapMsgHandler.insert(std::make_pair("playBeep", &VR_SDSController::OnBeepDone));
    // m_mapMsgHandler.insert(std::make_pair("audioFile", &VR_SDSController::OnAudioFile));
    // m_mapMsgHandler.insert(std::make_pair("AudioInOpe", &VR_SDSController::OnAudioInOpe));
    // m_mapMsgHandler.insert(std::make_pair("closeSuggestSession", &VR_SDSController::OnCloseSuggestSession));
    m_mapMsgHandler.insert(std::make_pair("playTts", &VR_SDSController::OnPlayTtsEnd));

    // register event to baidu engine
    m_mapEngineCallback.insert(make_pair("recognizeResult", &VR_SDSController::OnRecognizeResult));
    m_mapEngineCallback.insert(make_pair("timeout", &VR_SDSController::OnTimeout));

    m_pEngine = make_shared<VR_BaiduImpl>();
    m_pEngine->Initialize(m_controller.get());

    m_pStateMachine->Init();

    // const std::string InputFolder = "/data/test/inputs";
    // dynamic_cast<VR_AsrEngineIF*>
    // m_speechcommandobserver = std::make_shared<SphinxS::VR_SpeechCommandObserver>(m_pEngine.get());
    // m_speechdetector = SphinxS::SphinxSpeechDetector::create(InputFolder, {"BIDUBIDU"}, {m_speechcommandobserver});

    m_bInitialized = true;

    // if (m_speechdetector) {
    //     VR_LOGE("create speechDetector Failed\n");
    //     return true;
    // }
    // else {
    //     return false;
    // }
    return true;
}

void VR_SDSController::UnInitialize()
{
    VR_LOGD_FUNC();
    m_mapMsgHandler.clear();
    m_mapEngineCallback.clear();
    m_pEngine->UnInitialize();
}

bool VR_SDSController::Start()
{
    VR_LOGD_FUNC();
    m_pStateMachine->Start();
    // m_pEngine->Start();
    return true;
}

void VR_SDSController::Stop()
{
    m_pStateMachine->Stop();
    // m_pEngine->Stop();
    VR_LOGD_FUNC();
}


void VR_SDSController::ProcessMessage(const string& message, int actionSeqId)
{
    VR_LOGD_FUNC();

    VR_XmlParser parser(message);
    std::string strActionKey = parser.getXmlKey();

    VR_LOGD("message:%s", message.c_str());
    VR_LOGD("strActionKey:%s", strActionKey.c_str());

    bool isSpeaking = ("SPEAKING" == m_strCurrentSdsStatus);

    if ("IDLE" == message) {
        SendVRState("server", "idle");
        m_strCurrentSdsStatus = "IDLE";
        m_pEngine->stopRecordMicData();
        if (!m_bIsPrompting && !isSpeaking) {
            if (m_listener) {
                m_listener->onDialogStateChanged(VR_DialogStateBaidu::IDLE);
            }
            else {
                VR_LOGD("Listener point not correct");
            }
        }
        return;
    }
    else if ("LISTENING" == message) {
        m_strCurrentSdsStatus = "LISTENING";
        VR_LOGD("Current status is listening, whether prompting? [%s]", (m_bIsPrompting ? "TRUE" : "FALSE"));
        if (!m_bIsPrompting && !isSpeaking) {
            SendVRState("server", "listening");
            m_pEngine->startRecordMicData();
            if (m_listener) {
                m_listener->onDialogStateChanged(VR_DialogStateBaidu::LISTENING);
            }
            else {
                VR_LOGD("Listener point not correct");
            }
        }
        return;
    }
    else if ("THINKING" == message) {
        SendVRState("server", "processing");
        m_strCurrentSdsStatus = "THINKING";
        if (m_listener) {
            m_listener->onDialogStateChanged(VR_DialogStateBaidu::THINKING);
        }
        else {
            VR_LOGD("Listener point not correct");
        }
        return;
    }
    else if ("SPEAKING" == message) {
        // std::string strMsg = "<action agent=\"system\" op=\"awakewithoutstart\"></action>";
        // OnRequestAction(strMsg);
        SendVRState("server", "playing");
        m_strCurrentSdsStatus = "SPEAKING";
        if (m_listener) {
            m_listener->onDialogStateChanged(VR_DialogStateBaidu::SPEAKING);
        }
        else {
            VR_LOGD("Listener point not correct");
        }
        return;
    }
    else if ("keyWordDetected" == message) {
        // std::string strMsg = "<action agent=\"system\" op=\"awake\"></action>";
        // OnRequestAction(strMsg);

        // start the state machine
        // VR_SDSEvent* pcEvent = new VR_SDSEvent(VR_SDS_EVENT_TYPE_USR, VR_SDS_EVENT_ID_MIC);
        // m_pStateMachine->PostRequest(pcEvent);

        m_iRecognizeFailCount = 0;
        m_pEngine->stopRecordMicData();
        VR_SDSAbstractState* pcState = m_pStateMachine->GetState(VR_SDSSERVER_STATE_TYPE_START);
        VR_LOGD("Change state to pre start state");
        m_pStateMachine->ChangeStateTo(pcState);

        // m_bStartByKeyword = true;
        return;
    }
    else if ("CONNECTED" == message) {
        SendDEStatus("engine", "0");
        return;
    }
    else if ("DISCONNECTED" == message) {
        SendDEStatus("engine", "1");
        return;
    }
    else if ("PLAYING" == message) {
        SendPlayStateToDM("open");
        SendPlayStateToUI(message);
    }
    else if ("PAUSED" == message) {
        SendPlayStateToDM("close");
        SendPlayStateToUI(message);
    }
    else if ("STOPPED" == message) {
        SendPlayStateToDM("close");
        SendPlayStateToUI(message);
    }
    else if ("retry" == message) {
        const int WAIT_INTERVEL = 5000000; // 5s
        usleep(WAIT_INTERVEL);
        NotifyEngineAudioOpen();
    }
    else if ("dial" == strActionKey) {
        VR_LOGD("Dialing phone...");
        OnRequestAction(message);
        return;
    }
    else if ("showPhoneCall" == strActionKey) {
        VR_LOGD("Showing dialing view...");
        OnRequestAction(message);
        return;
    }
    else if ("playTts" == strActionKey && parser.isAction()) {
        VR_LOGD("Play TTS...");
        m_bIsPrompting = true;
        OnRequestAction(message);

        return;
    }
    else if ("ScreenDisplay" == strActionKey) {
        VR_LOGD("ScreenDisplay ...");
        OnRequestAction(message);
        return;
    }
    else if ("showPOI" == strActionKey) {
        VR_LOGD("showPOI ...");
        OnRequestAction(message);
        return;
    }
    else if ("HMI_ShowList" == strActionKey) {
        VR_LOGD("HMI_ShowList ...");
        OnRequestAction(message);
        return;
    }
    else if ("Navi_ToDestination" == strActionKey) {
        VR_LOGD("calculateRoute ...");
        OnRequestAction(message);
        return;       
    }
    else if ("Navi_ToDestinationWithViaPoint" == strActionKey) {
        VR_LOGD("calculateRoute with waypoint...");
        OnRequestAction(message);
        return;
    }
    else {
        //OnRequestAction(message);
    }

    // Dispatch the messages
    VoiceMap<std::string, MessageHandler>::const_iterator iterMap = m_mapMsgHandler.find(strActionKey);
    if (m_mapMsgHandler.cend() != iterMap) {
        if (NULL != iterMap->second) {
            (this->*(iterMap->second))(parser);
        }
    }

    // Baidu engine callback
    auto itr = m_mapEngineCallback.find(strActionKey);
    if (itr != m_mapEngineCallback.end()) {
        if (nullptr != itr->second) {
            string keyword = parser.getValueByKey("value");
            (this->*(itr->second))(keyword);
        }
    }
}

int VR_SDSController::OnRequestAction(const std::string& strMsg)
{
    VR_LOGD_FUNC();

    BL_AutoSync cAutoSync(m_ActionIDSync);

    static const int VR_BAIDU_SDS_MAX_REQUEST_ID = 100000;
    int iID = m_iCurReqId + 2 * VR_BAIDU_SDS_MAX_REQUEST_ID; // id should large than 200000
    ++m_iCurReqId;

    if (m_iCurReqId >= VR_BAIDU_SDS_MAX_REQUEST_ID) {
        m_iCurReqId = 1;
    }

    if (m_listener) {
        VR_LOGD("OnRequestAction ID: %d", iID);
        m_listener->OnRequestAction(strMsg, iID);
    }

    return iID;
}

void VR_SDSController::SendVRState(const std::string& engineType, const std::string& state)
{
    VR_LOGD_FUNC();

    std::string strVRStateMsg = "<display agent=\"Common\" content=\"VRState\">";
    strVRStateMsg.append("<engineType>");
    strVRStateMsg.append(engineType);
    strVRStateMsg.append("</engineType>");
    strVRStateMsg.append("<state>");
    strVRStateMsg.append(state);
    strVRStateMsg.append("</state>");
    strVRStateMsg.append("<tag>Voice Control</tag>");
    strVRStateMsg.append("<prompt />");
    strVRStateMsg.append("<result />");
    strVRStateMsg.append("</display>");

    // OnRequestAction(strVRStateMsg);
}

std::string& VR_SDSController::replace_all_distinct(std::string& str, const std::string& old_value, const std::string& new_value)
{
    for(std::string::size_type pos(0); pos != std::string::npos; pos += new_value.length()) {
        if((pos = str.find(old_value, pos)) == std::string::npos) {
            break;
        }

        str.replace(pos, old_value.length(), new_value);
    }
    return str;
}

void VR_SDSController::SendDEStatus(const std::string& strType, const std::string& strStatus)
{
    VR_LOGD_FUNC();

    std::string strEventResult = "<action agent=\"destatus\" op=\"notifyResourceState\">";
    strEventResult.append("<resourceStateType>");
    strEventResult.append(strType);
    strEventResult.append("</resourceStateType>");
    strEventResult.append("<resourceState>");
    strEventResult.append(strStatus);
    strEventResult.append("</resourceState>");
    strEventResult.append("</action>");

    // OnRequestAction(strEventResult);
}

void VR_SDSController::SendPlayStateToUI(const std::string& message)
{
    VR_LOGD_FUNC();

    std::string strDisplay = "<display agent=\"Common\" content=\"ScreenDisplay\"><agent>music</agent><content>music_state</content><playState>";
    strDisplay.append(message);
    strDisplay.append("</playState></display>");

    VR_LOGI("strDisplay : %s", strDisplay.c_str());
    OnRequestAction(strDisplay);
}

void VR_SDSController::SendPlayStateToDM(const std::string& message)
{
    VR_LOGD_FUNC();

    std::string strAction = "<action agent=\"session\" op=\"";
    strAction.append(message);
    strAction.append("\"></action>");

    VR_LOGI("strAction : %s", strAction.c_str());
    OnRequestAction(strAction);
}

void VR_SDSController::StartDialog(VR_XmlParser& parser)
{
    VR_LOGD_FUNC();

    // m_bStartByPTT = true;
    // if (m_pEngine) {
    //     VR_LOGD("Receive start dialog event");
    //     m_pEngine->startRecordMicData();
    // }

    // // Get the statename from the XML message
    // std::string strStateName = parser.getValueByKey("agent");

    // std::string strResult = "<event-result name=\"startAgent\" errcode=\"\"><agent>";
    // strResult.append(strStateName);
    // strResult.append("</agent> </event-result>");
    // // OnRequestAction(strResult);
}

void VR_SDSController::StartDialogIF()
{
    VR_LOGD_FUNC();
    m_iRecognizeFailCount = 0;
    VR_SDSEvent* pcEvent = new VR_SDSEvent(VR_SDS_EVENT_TYPE_USR, VR_SDS_EVENT_ID_MIC);
    m_pStateMachine->PostRequest(pcEvent);
}

int VR_SDSController::onAudioInData(int16_t* buffer, int32_t size)
{
    return m_pEngine->onAudioInData(buffer, size);
    // if (m_speechdetector) {
    //     return m_speechdetector->OnAudioInData(buffer, size);
    // }
    // else {
    //     return -1;
    // }
}


void VR_SDSController::StopDialog()
{
    VR_LOGD_FUNC();
    StopTTS();
    VR_SDSAbstractState* pcState = m_pStateMachine->GetState(VR_SDSSERVER_STATE_TYPE_IDLE);
    m_pStateMachine->ChangeStateTo(pcState);
}

// Cancel the current voice recognition session
void VR_SDSController::Cancel(VR_XmlParser& parser)
{
    VR_LOGD_FUNC();

    // std::string option = parser.getValueByKey("option");
    // VR_LOGI("the option of cancel is %s", option.c_str());

    // std::string cancelResult;
    // if (option.empty()) {
    //     cancelResult = "<event-result name=\"cancel\"/>";
    // }
    // else {
    //     cancelResult = "<event-result name=\"cancel\" option=\"" + option +"\"/>";
    // }

    // m_pEngine->stopRecordMicData();
    // VR_SDSAbstractState* pcState = m_pStateMachine->GetState(VR_SDSSERVER_STATE_TYPE_IDLE);
    // m_pStateMachine->ChangeStateTo(pcState);

    // OnRequestAction(cancelResult);
    // SendVRState("server", "idle");
}

void VR_SDSController::OnDMStartedNotify(VR_XmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strStartFinish = "<action agent=\"destatus\" op=\"notifyStartFinish\"></action>";
    // OnRequestAction(strStartFinish);

    if (m_bInitialized) {
        VR_LOGI("Dialog engine server is ready");
        SendDEStatus("init", "0");
        SendDEStatus("engine", "0");
    }
    else {
        VR_LOGI("Dialog engine is NOT ready");
        SendDEStatus("init", "1");
        SendDEStatus("engine", "1");
    }
}

void VR_SDSController::PlayBeep(const std::string& strBeepFileName)
{
    VR_LOGD_FUNC();

    std::string strBeepPath = VR_ConfigureIF::Instance()->getDataPath() + "beep/" + strBeepFileName;
    std::string strPlayBeep = "<action agent=\"prompt\" op=\"playBeep\"><beepFile>";
    strPlayBeep.append(strBeepPath);
    strPlayBeep.append("</beepFile></action>");

    OnRequestAction(strPlayBeep);
}

void VR_SDSController::OnBeepDone(VR_XmlParser& parser)
{
    VR_LOGD_FUNC();
}

void VR_SDSController::OnAudioFile(VR_XmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strPath = parser.getValueByKey("path");
    VR_LOGI("strPath: %s", strPath.c_str());
    std::string strResult = "<event-result name=\"audioFile\"><path>";
    strResult.append(strPath);
    strResult.append("</path> </event-result>");
    OnRequestAction(strResult);

//    if (m_pAlexaImpl) {
//        m_pAlexaImpl->OnPlayAudioFile(strPath);
//    }
}

void VR_SDSController::OnCloseSuggestSession(VR_XmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strMsg = "<action agent=\"system\" op=\"quit\"></action>";
    OnRequestAction(strMsg);
}

void VR_SDSController::OnAudioInOpe(VR_XmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strOperate = parser.getValueByKey("operate");
    VR_LOGI("strOperate: %s", strOperate.c_str());
    if ("open" == strOperate) {
        NotifyEngineAudioOpen();
    }
    else if ("close" == strOperate) {
        NotifyAudioInClose();
    }
    else {

    }

    std::string strResult = "<event-result name=\"AudioInOpe\"><operate>";
    strResult.append(strOperate);
    strResult.append("</operate></event-result>");
    OnRequestAction(strResult);
}

void VR_SDSController::NotifyEngineAudioOpen()
{
    VR_LOGD_FUNC();

    if (nullptr != m_pEngine) {
        m_pEngine->Start();
    }
}

void VR_SDSController::NotifyAudioInClose()
{
    VR_LOGD_FUNC();
}


void VR_SDSController::OnRecognizeResult(const string& queryWord)
{
    if (!queryWord.empty()) {
        m_pEngine->stopRecordMicData();
        VR_SDSEvent* pcEvent = new VR_SDSEvent(VR_SDS_EVENT_TYPE_SYSTEM,
                                               VR_SDS_EVENT_ID_VR_END);

        int length = queryWord.length();

        VR_LOGD("Query word is: %s", queryWord.c_str());
        string word = queryWord.substr(0, length - 3);
        pcEvent->SetText(word);
        m_pStateMachine->SetUserData((word));
        m_pStateMachine->PostRequest(pcEvent);
    }
    else {
        if ("THINKING" == m_strCurrentSdsStatus) {
            m_strCurrentSdsStatus = "LISTENING";
            ProcessMessage("LISTENING");
        }

    //     VR_LOGD("m_iRecognizeFailCount = %d", m_iRecognizeFailCount);
    //     ++m_iRecognizeFailCount;

    //     if (0 == m_iRecognizeFailCount % 1) {
    //         m_bIsPrompting = true;
    //         PlayTTS("未能听清，请重试");
    //         ProcessMessage("LISTENING", -1);
    //     }
    //     if (0 == m_iRecognizeFailCount % 2) {
    //         m_bIsPrompting = true;
    //         PlayTTS("很抱歉，请重试");
    //         ProcessMessage("LISTENING", -1);
    //     }
    //     if (0 == m_iRecognizeFailCount % 3) {
    //         m_iRecognizeFailCount = 0;
    //         m_bIsPrompting = true;
    //         PlayTTS("不能理解您的意图，会话结束");
    //         VR_SDSAbstractState* pcState = m_pStateMachine->GetState(VR_SDSSERVER_STATE_TYPE_IDLE);
    //         m_pStateMachine->ChangeStateTo(pcState);
    //     }
    }
}

void VR_SDSController::OnTimeout(const string& queryWord)
{
    m_pEngine->stopRecordMicData();
    m_bIsPrompting = true;
    // 未能检测到语音输入，
    PlayTTS("会话结束");
    VR_SDSAbstractState* pcState = m_pStateMachine->GetState(VR_SDSSERVER_STATE_TYPE_IDLE);
    m_pStateMachine->ChangeStateTo(pcState);
}

void VR_SDSController::OnPlayTtsEnd(VR_XmlParser& parser)
{
    VR_LOGD_FUNC();

    VR_LOGD("Receive play end msg, current status is [%s]", m_strCurrentSdsStatus.c_str());
    m_bIsPrompting = false;
    if ("LISTENING" == m_strCurrentSdsStatus) {
        SendVRState("server", "listening");
        m_pEngine->startRecordMicData();
        if (m_listener) {
            m_listener->onDialogStateChanged(VR_DialogStateBaidu::LISTENING);
        }
    }
    if ("IDLE" == m_strCurrentSdsStatus) {
        if (m_listener) {
            m_listener->onDialogStateChanged(VR_DialogStateBaidu::IDLE);
        }
    }
    // if ("SPEAKING" == m_strCurrentSdsStatus) {
    //     if (m_listener) {
    //         m_listener->onDialogStateChanged(VR_DialogStateBaidu::LISTENING);
    //     }
    // }
    return;
}

void VR_SDSController::PlayTTS(const std::string& strTtsContent)
{
    if ("" == strTtsContent) {
        return;
    }

    std::string strAction = "<action agent = \"prompt\" op = \"playTts\">";
    strAction += "<language>zh-cn</language>";
    strAction += "<text>" + strTtsContent + "</text>";
    strAction += "<bargein>false</bargein>";
    strAction += "</action>";

    OnRequestAction(strAction);

}

void VR_SDSController::StopTTS()
{
    std::string strAction = "<action agent = \"prompt\" op = \"stopTts\">";
    strAction += "</action>";
    OnRequestAction(strAction);
    return;
}

/* EOF */