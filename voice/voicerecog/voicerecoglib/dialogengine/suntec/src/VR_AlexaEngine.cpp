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

/* Standard Header */
#include <unistd.h>
#include <sstream>

/* Suntec Header */
#ifndef VR_LOG_H
#    include "VR_Log.h"
#endif

#ifndef VR_XMLPARSER_H
#    include "VR_XmlParser.h"
#endif

#ifndef VR_CONFIGUREIF_H
#    include "VR_ConfigureIF.h"
#endif

#ifndef CXX_BL_AUTOSYNC_H
#   include "BL_AutoSync.h"
#endif

#ifndef VR_DIALOGENGINEIF_H
#    include "VR_DialogEngineIF.h"
#endif

#ifndef VR_CONTROLLERBASE_H
#    include "VR_ControllerBase.h"
#endif

#ifndef VR_ALEXAIMPL_H
#   include "VR_AlexaImpl.h"
#endif

#ifndef VR_GOOGLEIMPL_H
#   include "VR_GoogleImpl.h"
#endif

#ifndef VR_BAIDUIMPL_H
#   include "VR_BaiduImpl.h"
#endif

#ifndef VR_GOOGLEOBSERVER_H
#   include "VR_GoogleObserver.h"
#endif

#ifndef VR_BAIDUOBSERVER_H
#   include "VR_BaiduObserver.h"
#endif

#ifndef VR_ALEXAENGINE_H
#    include "VR_AlexaEngine.h"
#endif

#ifndef VR_ALEXAJSONPARSER_H
#    include "VR_AlexaJSonParser.h"
#endif

#ifndef VR_VOICEBOXXMLBUILDER_H
#    include "VR_VoiceBoxXmlBuilder.h"
#endif

#include <stdio.h>

#ifndef VR_PORTAUDIOMICWRAPPER_H
#    include "VR_PortAudioMicWrapper.h"
#endif

#ifndef VR_BAIDUSDS_IF_H
#    include "VR_BaiduSDS_IF.h"
#endif

#ifndef CXX_GOOASSPUBLIC_H
#    include "GooAssPublic.h"
#endif

#ifndef VR_DICTATIONMANAGER_H
#    include "VR_DictationManager.h"
#endif

/* Class static member defintion */
BL_SyncObject   VR_AlexaEngine::m_ActionIDSync;   ///< Sync object

// VR_AlexaEngine
VR_AlexaEngine::VR_AlexaEngine()
: m_pAlexaImpl(nullptr)
, m_pBaiduImpl(nullptr)
, m_pGoogleImpl(nullptr)
, m_listener(nullptr)
, m_controller(nullptr)
, m_bIntialized(false)
, m_iCurReqId(0)
, m_iPictureId(0)
, m_bSamemusicScreen(false)
, m_bStartByKeyword(false)
, m_bStartByPTT(false)
, m_strEngineValue("Alexa")
, m_googleObserver(nullptr)
, m_baiduObserver(nullptr)
, m_pGoogleAbstractSDS(nullptr)
, m_pBaiduSDS(nullptr)
, m_bIDLE(true)
, m_bAlexaSelected(true)
, m_bBaiduSelected(false)
, m_bGoogleSelected(false)
, m_audioMicrophone(nullptr)
, m_bSuggestion(false)
, m_dictationManager(nullptr)
{

}

bool
VR_AlexaEngine::Initialize(VR_DialogEngineListener* listerner, VR_ControllerBase* controller)
{
    VR_LOGD_FUNC();

    m_listener = listerner;
    m_controller = controller;

    // Create the aciton-function map
    m_mapMsgHandler.clear();
    m_mapAlexaMsgHandler.clear();
    m_mapMsgHandler.insert(std::make_pair("startedNotify", &VR_AlexaEngine::OnDMStartedNotify));
    m_mapMsgHandler.insert(std::make_pair("startAgent", &VR_AlexaEngine::StartDialog));
    m_mapMsgHandler.insert(std::make_pair("cancel", &VR_AlexaEngine::Cancel));
    m_mapMsgHandler.insert(std::make_pair("playBeep", &VR_AlexaEngine::OnBeepDone));
    m_mapMsgHandler.insert(std::make_pair("audioFile", &VR_AlexaEngine::OnAudioFile));
    m_mapMsgHandler.insert(std::make_pair("AudioInOpe", &VR_AlexaEngine::OnAudioInOpe));
    m_mapMsgHandler.insert(std::make_pair("closeSuggestSession", &VR_AlexaEngine::OnCloseSuggestSession));
    m_mapMsgHandler.insert(std::make_pair("keyWordDetected", &VR_AlexaEngine::OnkeyWordDetected));
    m_mapMsgHandler.insert(std::make_pair("setAwakenHints", &VR_AlexaEngine::SetAwakenHints));
    m_mapMsgHandler.insert(std::make_pair("setAECStatus", &VR_AlexaEngine::OnSetAECStatus));
    m_mapMsgHandler.insert(std::make_pair("Dictation", &VR_AlexaEngine::OnDictation));
    m_mapMsgHandler.insert(std::make_pair("DictationRecognizeResult", &VR_AlexaEngine::OnDictationRecognizeResult));
    // handle the alexa message
    m_mapAlexaMsgHandler.insert(std::make_pair("BodyTemplate2", &VR_AlexaEngine::OnBodyTemplate2));
    m_mapAlexaMsgHandler.insert(std::make_pair("SetTemperature", &VR_AlexaEngine::OnChangeTemperature));
    m_mapAlexaMsgHandler.insert(std::make_pair("TuneIn Live Radio", &VR_AlexaEngine::OnPlayRadio));
    m_mapAlexaMsgHandler.insert(std::make_pair("iHeartRadio Live Radio", &VR_AlexaEngine::OnPlayRadio));
    m_mapAlexaMsgHandler.insert(std::make_pair("NaviToDestination", &VR_AlexaEngine::OnNaviToDestination));
    m_mapAlexaMsgHandler.insert(std::make_pair("GetTravelTime", &VR_AlexaEngine::OnGetTravelTime));
    m_mapAlexaMsgHandler.insert(std::make_pair("ShowPhoneCall", &VR_AlexaEngine::OnShowPhoneCall));
    m_mapAlexaMsgHandler.insert(std::make_pair("MakePhoneCall", &VR_AlexaEngine::OnMakePhoneCall));
    m_mapAlexaMsgHandler.insert(std::make_pair("RoofControl", &VR_AlexaEngine::OnRoofControl));
    m_mapAlexaMsgHandler.insert(std::make_pair("AutoStarStop", &VR_AlexaEngine::OnAutoStarStop));
    m_mapAlexaMsgHandler.insert(std::make_pair("SetDestinationReq", &VR_AlexaEngine::OnSetDestinationReq));
    m_mapAlexaMsgHandler.insert(std::make_pair("Stop", &VR_AlexaEngine::OnStop));
    // m_mapAlexaMsgHandler.insert(std::make_pair("SetDestinationReq", &VR_AlexaEngine::OnSetDestinationReq));
    m_mapAlexaMsgHandler.insert(std::make_pair("FindPlace", &VR_AlexaEngine::OnFindPlace));
    m_mapAlexaMsgHandler.insert(std::make_pair("WeatherTemplate", &VR_AlexaEngine::OnWeatherTemplate));
    m_mapAlexaMsgHandler.insert(std::make_pair("ShowContactList", &VR_AlexaEngine::OnShowContactList));
    m_mapAlexaMsgHandler.insert(std::make_pair("ShowPhoneList", &VR_AlexaEngine::OnShowPhoneList));
    m_mapAlexaMsgHandler.insert(std::make_pair("DirveScore", &VR_AlexaEngine::OnDirveScore));

    m_bIntialized = true;

    return true;
}

void VR_AlexaEngine::UnInitialize()
{
    VR_LOGD_FUNC();
    m_mapMsgHandler.clear();
    m_mapAlexaMsgHandler.clear();
    if (NULL != m_dictationManager) {
        m_dictationManager->UnInitialize();
    }
}

std::string VR_AlexaEngine::getHints(const std::string& hintsParams)
{
    VR_LOGD_FUNC();
    return "";
}

bool VR_AlexaEngine::Start()
{
    VR_LOGD_FUNC();
    return true;
}

void VR_AlexaEngine::Stop()
{
    VR_LOGD_FUNC();
}

VR_AlexaEngine::~VR_AlexaEngine()
{
    if (NULL != m_pAlexaImpl) {
        delete m_pAlexaImpl;
        m_pAlexaImpl = NULL;
    }
    if (NULL != m_pGoogleImpl) {
        delete m_pGoogleImpl;
        m_pGoogleImpl = NULL;
    }
    if (NULL != m_pBaiduImpl) {
        delete m_pBaiduImpl;
        m_pBaiduImpl = NULL;
    }
}

std::string& VR_AlexaEngine::replace_all_distinct(std::string& str, const std::string& old_value, const std::string& new_value)
{
    for (std::string::size_type pos(0); pos != std::string::npos; pos += new_value.length()) {
        if ((pos = str.find(old_value, pos)) != std::string::npos) {
            str.replace(pos, old_value.length(), new_value);
        }
        else {
            break;
        }
    }
    return str;
}

void VR_AlexaEngine::ProcessMessage(const std::string& message, int actionSeqId)
{
    VR_LOGD_FUNC();

    VR_LOGI("actionSeqId is: %d", actionSeqId);
    const int BAIDUMSGID = 200000;
    if (BAIDUMSGID <= actionSeqId) {
        if (m_pBaiduSDS) {
            m_pBaiduSDS->onMessage(message, actionSeqId);
            VR_LOGI("Reply baidu action message successfully !");
        }
        return;
    }

    // using namespace std;
    // if (std::string::npos != message.find("Welcome to The Magic Door")) {
    //     VR_LOGI("write to file");
    //     // std::string temp = "nice to meet you";

    // }

    VR_AlexaJSonParser JParser;
    if (JParser.LoadJSonString(message)) {
        // FILE *fp;
        // fp = fopen("/data/alexalog.txt", "w");
        // if(fp ==NULL) {
        //     VR_LOGI("打开文件aa.txt失败");
        // }
        // fprintf(fp, message.c_str());
        // fclose(fp);
        std::string stringFromStream = message;

        stringFromStream = replace_all_distinct(stringFromStream, "\\", "");
        stringFromStream = replace_all_distinct(stringFromStream, "\"{", "{");
        stringFromStream = replace_all_distinct(stringFromStream, "}\"", "}");
        VR_LOGI("stringFromStream %s\n", stringFromStream.c_str());

        VR_AlexaJSonParser JTempParser;

        // if (JTempParser.LoadJSonString(stringFromStream)) {
        //     VR_LOGI("handle the JSon message");
        //     HandleAlexaMsg(JTempParser);
        //     return;
        // }

        if (JTempParser.LoadJSonString(stringFromStream)) {
            std::string strKeyValue = JParser.GetKeyValue();
            VR_LOGI("json key (%s)", strKeyValue.c_str());
            VoiceMap<std::string, AlexaMsgHandler>::const_iterator iterMap = m_mapAlexaMsgHandler.find(strKeyValue);
            if (m_mapAlexaMsgHandler.cend() != iterMap) {
                if (NULL != iterMap->second) {
                    (this->*(iterMap->second))(JParser);
                    return;
                }
            }
        }
    }

    VR_XmlParser parser(message);
    std::string strActionKey = parser.getXmlKey();

    if ("IDLE" == message) {
        m_bIDLE = true;
        // if (m_pAlexaImpl) {
        //     m_pAlexaImpl->StopAudioFile();
        // }

        // if IDLE, call stopDialog()
        if (("Google" == m_strEngineValue) && m_pGoogleImpl) {
            m_pGoogleImpl->ProcessMessage("stopGoogleDialog");
        }
        // if (("Baidu" == m_strEngineValue) && m_pBaiduImpl) {
        //     m_pBaiduImpl->ProcessMessage("stopBaiduDialog");
        // }

        // if IDLE, switch to write to alexa
        if (m_pAlexaImpl && (m_pAlexaImpl->GetAudioWrapper())) {
            m_pAlexaImpl->GetAudioWrapper()->SetEngineValue("Alexa");
        }

        // While the Dialog State is IDLE, set it to MicWrap so as to write to alexa to awake
        if (m_pGoogleImpl) {
            m_pGoogleImpl->SetDialogState("IDLE");
        }

        if (m_bStartByKeyword) {
            m_bStartByKeyword = false;
            std::string strMsg = "<action agent=\"system\" op=\"quit\"></action>";
            OnRequestAction(strMsg);
            return;
        }

        if (m_bStartByPTT) {
            m_bStartByPTT = false;
            std::string strQuitVRApp = "<action agent=\"system\" op=\"quit\"></action>";
            OnRequestAction(strQuitVRApp);
            return;
        }

        if (m_bSuggestion) {
            std::string strMsg = "<action agent=\"system\" op=\"quit\"></action>";
            OnRequestAction(strMsg);
        }

        SendVRState("server", "idle");
        VR_ConfigureIF::Instance()->setEngineCurrentStatus(VR_Engine_Idle);
        VR_LOGD("getEngineCurrentStatus [%d]", VR_ConfigureIF::Instance()->getEngineCurrentStatus());
        return;
    }
    else if ("LISTENING" == message) {
        m_bIDLE = false;
        if (m_pGoogleImpl) {
            m_pGoogleImpl->SetDialogState("LISTENING");
        }

        SendVRState("server", "listening");
        return;
    }
    else if ("THINKING" == message) {
        m_bIDLE = false;
        if (m_pGoogleImpl) {
            m_pGoogleImpl->SetDialogState("THINKING");
        }

        SendVRState("server", "processing");
        return;
    }
    else if ("SPEAKING" == message) {
        m_bIDLE = false;
        if (m_pGoogleImpl) {
            m_pGoogleImpl->SetDialogState("SPEAKING");
        }
        std::string strMsg = "<action agent=\"system\" op=\"awakewithoutstart\"></action>";
        OnRequestAction(strMsg);
        // if (m_bSuggestion) {
        //     VR_LOGI("suggestion");
        //     m_bStartDialog = true;
        // }
        SendVRState("server", "playing");
        return;
    }
    else if ("ERROR" == message) {
        if (m_pGoogleImpl) {
            m_pGoogleImpl->SetDialogState("ERROR");
        }
        // if error, restart the engine
        if (("Google" == m_strEngineValue) && m_pGoogleImpl) {
            m_pGoogleImpl->ProcessMessage("startGoogleDialog");
        }
        if (("Baidu" == m_strEngineValue) && m_pBaiduImpl) {
            m_pBaiduImpl->ProcessMessage("startBaiduDialog");
        }
    }
    else if ("CONNECTED" == message) {
        static bool bEngineStatus = false;
        if (!bEngineStatus) {
            bEngineStatus = true;
            SendDEStatus("engine", "0");
        }
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
        AudioInOpen();
    }
    else if ("dictation_processing" == message) {
        SendVRDictationState("server", "processing");
    }
    else {
        // do nothing
    }

    // Dispatch the messages
    VoiceMap<std::string, MessageHandler>::const_iterator iterMap = m_mapMsgHandler.find(strActionKey);
    if (m_mapMsgHandler.cend() != iterMap) {
        if (NULL != iterMap->second) {
            (this->*(iterMap->second))(parser);
        }
    }
    else {
        DummyProcess(parser);
    }
}

void VR_AlexaEngine::OnkeyWordDetected(VR_XmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strEngineValue = parser.getValueByKey("value");
    VR_LOGI("strEngineValue: %s", strEngineValue.c_str());

    if (("Alexa" == m_strEngineValue) && (!m_bIDLE)) {
        // if Alexa was awaked, ignore Google and Baidu keyword !
        if ("Alexa" != strEngineValue) {
            return;
        }
    }

    if (!m_bAlexaSelected && ("Alexa" == strEngineValue)) {
        return;
    }
    if (!m_bBaiduSelected && ("Baidu" == strEngineValue)) {
        return;
    }
    if (!m_bGoogleSelected  && ("Google" == strEngineValue)) {
        return;
    }

    m_bStartByKeyword = true;
    std::string strMsg = "<action agent=\"system\" op=\"awake\"></action>";
    OnRequestAction(strMsg);

    m_strEngineValue = strEngineValue;
    VR_LOGI("m_strEngineValue: %s", m_strEngineValue.c_str());

    if (m_pAlexaImpl && (m_pAlexaImpl->GetAudioWrapper())) {
        m_pAlexaImpl->GetAudioWrapper()->SetEngineValue(m_strEngineValue);
        m_pAlexaImpl->GetAudioWrapper()->SaveMicToFile(m_strEngineValue);
    }
}

void VR_AlexaEngine::SetAwakenHints(VR_XmlParser& parser)
{
    VR_LOGD_FUNC();
    VR_LOGI("alexa value is: %s", (parser.getValueByKey("alexa")).c_str());

    m_bAlexaSelected = (parser.getValueByKey("alexa") == "1") ? true : false;
    m_bBaiduSelected = (parser.getValueByKey("baidu") == "1") ? true : false;
    m_bGoogleSelected = (parser.getValueByKey("google") == "1") ? true : false;
}

void VR_AlexaEngine::OnSetAECStatus(VR_XmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strAECStatus = parser.getValueByKey("status");

    VR_LOGI("AECStatus: %s", strAECStatus.c_str());

    if (("0" == strAECStatus) && (NULL != m_audioMicrophone)) {
        m_audioMicrophone->setMode(AECOFF);
    }
    else if (("1" == strAECStatus) && (NULL != m_audioMicrophone)) {
        m_audioMicrophone->setMode(AECON);
    }
    else {

    }
}

int
VR_AlexaEngine::OnRequestAction(const std::string& strMsg)
{
    VR_LOGD_FUNC();

    BL_AutoSync cAutoSync(m_ActionIDSync);
    int iID = m_iCurReqId;
    ++m_iCurReqId;

    const int VR_VBT_MAX_REQUEST_ID = 100000;
    if (m_iCurReqId >= VR_VBT_MAX_REQUEST_ID) {
        m_iCurReqId = 1;
    }

    if (m_listener) {
        m_listener->OnRequestAction(strMsg, iID);
    }

    return iID;
}

void VR_AlexaEngine::SendDEStatus(const std::string& strType, const std::string& strStatus)
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

    OnRequestAction(strEventResult);
}

void VR_AlexaEngine::SendVRState(const std::string& engineType, const std::string& state)
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

    OnRequestAction(strVRStateMsg);
}

void VR_AlexaEngine::SendVRDictationState(const std::string& engineType, const std::string& state)
{
    VR_LOGD_FUNC();

    std::string strVRStateMsg = "<display agent=\"Common\" content=\"VRDictationState\">";
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

    OnRequestAction(strVRStateMsg);
}

void VR_AlexaEngine::SendPlayStateToUI(const std::string& message)
{
    VR_LOGD_FUNC();

    // if (m_bSamemusicScreen) {
    //     return;
    // }

    std::string strDisplay = "<display agent=\"Common\" content=\"ScreenDisplay\"><agent>music</agent><content>music_state</content><playState>";
    strDisplay.append(message);
    strDisplay.append("</playState></display>");

    VR_LOGI("strDisplay : %s", strDisplay.c_str());
    OnRequestAction(strDisplay);
}

void VR_AlexaEngine::SendPlayStateToDM(const std::string& message)
{
    VR_LOGD_FUNC();

    // if (m_bSamemusicScreen) {
    //     return;
    // }

    std::string strAction = "<action agent=\"session\" op=\"";
    strAction.append(message);
    strAction.append("\"></action>");

    VR_LOGI("strAction : %s", strAction.c_str());
    OnRequestAction(strAction);
}

void VR_AlexaEngine::DummyProcess(VR_XmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string message = parser.getXmlString();
    // Check the header contents
    const int HEADER_LEN = 150;
    std::string header = message.substr(0, HEADER_LEN);

    if (std::string::npos != header.find("<event ")) {
        std::string strEventResult;
        AddResultToNode(strEventResult, message, true);
        OnRequestAction(strEventResult);
    }
    else if (std::string::npos != header.find("<grammar_")) {
        std::string strGrammarResult;
        BuildGrammarResultXML(message, strGrammarResult);
        OnRequestAction(strGrammarResult);
    }
}

void VR_AlexaEngine::StartDialog(VR_XmlParser& parser)
{
    VR_LOGD_FUNC();

    m_bSuggestion  = false;

    if (!m_bStartByKeyword) {
        m_bStartByPTT = true;
        if (m_bGoogleSelected && ("Google" == m_strEngineValue) && m_pGoogleImpl) {
            m_pGoogleImpl->ProcessMessage("startGoogleDialog");
        }
        else if (m_bBaiduSelected && ("Baidu" == m_strEngineValue) && m_pBaiduImpl) {
            m_pBaiduImpl->ProcessMessage("startBaiduDialog");
        }
        else if (m_pAlexaImpl) {
            m_pAlexaImpl->ProcessMessage("tap");
        }
    }
    else {
        // The keyword should be identified here.
        if (("Alexa" == m_strEngineValue) && m_pAlexaImpl) {
            m_pAlexaImpl->ProcessMessage("tap");
        }
        if (("Google" == m_strEngineValue) && m_pGoogleImpl) {
            m_pGoogleImpl->ProcessMessage("startGoogleDialog");
        }
        if (("Baidu" == m_strEngineValue) && m_pBaiduImpl) {
            m_pBaiduImpl->ProcessMessage("startBaiduDialog");
        }
    }

    // std::string strActionKey = parser.getXmlKey();
    // Get the statename from the XML message
    std::string strStateName = parser.getValueByKey("agent");

    std::string strResult = "<event-result name=\"startAgent\" errcode=\"\"><agent>";
    strResult.append(strStateName);
    strResult.append("</agent> </event-result>");
    OnRequestAction(strResult);
}

void VR_AlexaEngine::OnDictation(VR_XmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strStatus = parser.getValueByKey("status");

    VR_LOGI("strStatus = %s", strStatus.c_str());

    if ((NULL != m_dictationManager) && ("start" == strStatus)) {
        m_dictationManager->SetDictationValue(true);
        SendVRDictationState("server", "listening");
    }

    std::string strStateName = parser.getValueByKey("status");

    std::string strResult = "<event-result name=\"Dictation\" errcode=\"\"><status>";
    strResult.append(strStateName);
    strResult.append("</status> </event-result>");
    OnRequestAction(strResult);
}

void VR_AlexaEngine::OnDictationRecognizeResult(VR_XmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strSuccess = parser.getValueByKey("recognizeSuccess");
    std::string strResult = parser.getValueByKey("recognizeResult");

    std::string strRecResult = "<display agent=\"Common\" content=\"dictationResult\"><recognizeSuccess>";
    strRecResult.append(strSuccess);
    strRecResult.append("</recognizeSuccess><recognizeResult>");
    strRecResult.append(strResult);
    strRecResult.append("</recognizeResult></display>");

    OnRequestAction(strRecResult);

    SendVRDictationState("server", "idle");
}

// Cancel the current voice recognition session
void VR_AlexaEngine::Cancel(VR_XmlParser& parser)
{
    VR_LOGD_FUNC();

    if (m_pAlexaImpl && (m_pAlexaImpl->GetAudioWrapper())) {
        m_pAlexaImpl->GetAudioWrapper()->SetEngineValue("Alexa");
    }

    if (m_bStartByKeyword) {
        m_bStartByKeyword = false;
        if (("Alexa" == m_strEngineValue) && m_pAlexaImpl) {
            m_pAlexaImpl->ProcessMessage("talkEnd");
        }

        if (("Google" == m_strEngineValue) && m_pGoogleImpl) {
            m_pGoogleImpl->ProcessMessage("stopGoogleDialog");
        }

        if (("Baidu" == m_strEngineValue) && m_pBaiduImpl) {
            m_pBaiduImpl->ProcessMessage("stopBaiduDialog");
        }

        std::string strMsg = "<action agent=\"system\" op=\"quit\"></action>";
        OnRequestAction(strMsg);
    }

    if (m_bStartByPTT) {
        m_bStartByPTT = false;
        if (m_bGoogleSelected && ("Google" == m_strEngineValue) && m_pGoogleImpl) {
            m_pGoogleImpl->ProcessMessage("stopGoogleDialog");
        }
        else if (m_bBaiduSelected && ("Baidu" == m_strEngineValue) && m_pBaiduImpl) {
            m_pBaiduImpl->ProcessMessage("stopBaiduDialog");
        }
        else if (m_pAlexaImpl) {
            m_pAlexaImpl->ProcessMessage("talkEnd");
        }

        std::string strQuitVRApp = "<action agent=\"system\" op=\"quit\"></action>";
        OnRequestAction(strQuitVRApp);
    }

    std::string option = parser.getValueByKey("option");
    VR_LOGI("the option of cancel is %s", option.c_str());

    std::string cancelResult;
    if (option.empty()) {
        cancelResult = "<event-result name=\"cancel\"/>";
    }
    else {
        cancelResult = "<event-result name=\"cancel\" option=\"" + option +"\"/>";
    }

    OnRequestAction(cancelResult);
    SendVRState("server", "idle");
    VR_ConfigureIF::Instance()->setEngineCurrentStatus(VR_Engine_Idle);
    VR_LOGD("getEngineCurrentStatus [%d]", VR_ConfigureIF::Instance()->getEngineCurrentStatus());
}

void VR_AlexaEngine::OnDMStartedNotify(VR_XmlParser& parser)
{
    VR_LOGD_FUNC();

    static bool bStartedNotifyJustOnce = false;
    if (!bStartedNotifyJustOnce) {
        bStartedNotifyJustOnce = true;

        std::string strStartFinish = "<action agent=\"destatus\" op=\"notifyStartFinish\"></action>";
        OnRequestAction(strStartFinish);

        if (!m_googleObserver) {
            m_googleObserver = std::make_shared<VR_GoogleObserver>(m_controller);
            if (!m_googleObserver) {
                VR_LOGI("Failed to create VR_GoogleObserver !");
                return;
            }
        }

        if (!m_pGoogleAbstractSDS) {
            m_pGoogleAbstractSDS = AbstractSDS::createGoogleSDS(m_googleObserver.get());
            VR_LOGI("createGoogleSDS successfully !");
            if (!m_pGoogleAbstractSDS) {
                VR_LOGI("Failed to create GoogleSDS !");
                return;
            }
        }

        if (!m_baiduObserver) {
            m_baiduObserver = std::make_shared<VR_BaiduObserver>(m_controller, m_listener);
            if (!m_baiduObserver) {
                VR_LOGI("Failed to create VR_BaiduObserver !");
                return;
            }
        }

         if (!m_dictationManager) {
            m_dictationManager = std::make_shared<VR_DictationManager>(m_controller);
            if (!m_dictationManager) {
                VR_LOGI("Failed to create m_dictationManager !");
                return;
            }
            m_dictationManager->Initialize();
        }

        if (!m_pBaiduSDS) {
            m_pBaiduSDS = VR_BaiduSDS::CreateBaiduSDS(m_baiduObserver.get());
            VR_LOGI("CreateBaiduSDS successfully !");
            if (!m_pBaiduSDS) {
                VR_LOGI("Failed to create BaiduSDS !");
                return;
            }
        }

        bool bAECStatusInit = false;
        VR_ConfigureIF* pcConfig = VR_ConfigureIF::Instance();
        if (NULL != pcConfig) {
            bAECStatusInit = pcConfig->getAECEngineStatus();
        }
        VR_LOGI("bAECStatusInit : %d", bAECStatusInit);
        AECMODE aecModeInit = (!bAECStatusInit) ? AECOFF : AECON;

        if (!m_audioMicrophone) {
            m_audioMicrophone = alexaClientSDK::sampleApp::AudioMicrophone::createAEC(aecModeInit);
            if (!m_audioMicrophone) {
                VR_LOGI("createAEC failed !");
                return;
            }
        }

        if (m_bIntialized) {
            VR_LOGI("Alexa engine is ready");
            SendDEStatus("init", "0");
        }
        else {
            VR_LOGI("Alexa engine is NOT ready");
            SendDEStatus("init", "1");
        }
    }
}

void VR_AlexaEngine::PlayBeep(const std::string& strBeepFileName)
{
    VR_LOGD_FUNC();

    std::string strBeepPath = VR_ConfigureIF::Instance()->getDataPath() + "beep/" + strBeepFileName;
    std::string strPlayBeep = "<action agent=\"prompt\" op=\"playBeep\"><beepFile>";
    strPlayBeep.append(strBeepPath);
    strPlayBeep.append("</beepFile></action>");

    OnRequestAction(strPlayBeep);
}

void VR_AlexaEngine::OnBeepDone(VR_XmlParser& parser)
{
    VR_LOGD_FUNC();
}

void VR_AlexaEngine::AddResultToNode(std::string& strDestxml, const std::string& strSourxml, bool isAppendErrCode)
{
    pugi::xml_document doc;
    if (!doc.load_string(strSourxml.c_str())) {
        VR_LOGE("Load xml failed");
        return;
    }

    pugi::xml_node node = doc.first_child();

    std::string nodename = node.name();
    nodename.append("-result");
    node.set_name(nodename.c_str());
    if (isAppendErrCode) {
        node.append_attribute("errcode") = "1";
    }

    std::ostringstream ss;
    node.print(ss);
    strDestxml = ss.str();
}

void VR_AlexaEngine::OnAudioFile(VR_XmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strPath = parser.getValueByKey("path");
    VR_LOGI("strPath: %s", strPath.c_str());
    std::string strResult = "<event-result name=\"audioFile\"><path>";
    strResult.append(strPath);
    strResult.append("</path> </event-result>");
    OnRequestAction(strResult);

    if (!m_bIDLE) {
        VR_LOGI("Ignore this suggestion because of not IDLE state !");
        return;
    }

    m_bSuggestion = true;

    if (m_pAlexaImpl) {
        m_pAlexaImpl->OnPlayAudioFile(strPath);
    }
}

void VR_AlexaEngine::OnCloseSuggestSession(VR_XmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strMsg = "<action agent=\"system\" op=\"quit\"></action>";
    OnRequestAction(strMsg);
}

void VR_AlexaEngine::OnAudioInOpe(VR_XmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strOperate = parser.getValueByKey("operate");
    VR_LOGI("strOperate: %s", strOperate.c_str());
    if ("open" == strOperate) {
        AudioInOpen();
    }
    else if ("close" == strOperate) {
        AudioInClose();
    }
    else {

    }

    std::string strResult = "<event-result name=\"AudioInOpe\"><operate>";
    strResult.append(strOperate);
    strResult.append("</operate></event-result>");
    OnRequestAction(strResult);
}

void VR_AlexaEngine::AudioInOpen()
{
    VR_LOGD_FUNC();

    if (!m_pAlexaImpl) {
        m_pAlexaImpl = VR_new VR_AlexaImpl();
        if (!m_pAlexaImpl) {
            return;
        }

        if (!m_pAlexaImpl->Initialize(m_controller, m_googleObserver, m_audioMicrophone)) {
            VR_LOGI("AlexaImpl engine initialize failed");
            m_pAlexaImpl->UnInitialize();
            delete m_pAlexaImpl;
            m_pAlexaImpl = nullptr;
            m_controller->PostMessage("retry");
            return;
        }
    }

    if (!m_pGoogleImpl) {
        m_pGoogleImpl = VR_new VR_GoogleImpl();
        if (!m_pGoogleImpl) {
            return;
        }

        if (!m_pGoogleImpl->Initialize(m_controller, m_pGoogleAbstractSDS, m_pAlexaImpl->GetAudioWrapper())) {
            VR_LOGI("GoogleImpl initialize failed");
            delete m_pGoogleImpl;
            m_pGoogleImpl = nullptr;
            // m_controller->PostMessage("retry");
            // return;
        }
    }

    if (!m_pBaiduImpl) {
        m_pBaiduImpl = VR_new VR_BaiduImpl();
        if (!m_pBaiduImpl) {
            return;
        }

        if (!m_pBaiduImpl->Initialize(m_controller, m_pBaiduSDS, m_pAlexaImpl->GetAudioWrapper())) {
            VR_LOGI("BaiduImpl initialize failed");
            delete m_pBaiduImpl;
            m_pBaiduImpl = nullptr;
            // m_controller->PostMessage("retry");
            // return;
        }
    }

    if (m_pAlexaImpl->GetAudioWrapper()) {
        m_pAlexaImpl->GetAudioWrapper()->SetDictationManager(m_dictationManager);
    }
}

void VR_AlexaEngine::AudioInClose()
{
    VR_LOGD_FUNC();
}

void VR_AlexaEngine::BuildGrammarResultXML(const std::string& strGrammarxml, std::string& strGrammarResultxml)
{
    VR_XmlParser parser(strGrammarxml);
    std::string strRootName = parser.getXmlKey();
    std::string strAgent = parser.getValueByKey("agent");
    std::string strGrammarID = parser.getValueByKey("grammarid");

    std::string strOp;
    if ("grammar_disactive" == strRootName) {
        strOp = "disactive";
    }
    else if ("grammar_active" == strRootName) {
        strOp = "active";
    }
    else {
        strOp = "grammar";
    }

    pugi::xml_document docXML;
    pugi::xml_node root_node = docXML.append_child("grammar_result");
    root_node.append_attribute("agent").set_value(strAgent.c_str());
    root_node.append_attribute("op").set_value(strOp.c_str());
    if ("" != strGrammarID) {
        root_node.append_attribute("grammarid").set_value(strGrammarID.c_str());
    }
    root_node.append_attribute("errcode").set_value("1");

    std::ostringstream ss;
    root_node.print(ss);
    strGrammarResultxml = ss.str();
}

void VR_AlexaEngine::OnBodyTemplate2(VR_AlexaJSonParser& JParser)
{
    VR_LOGD_FUNC();

    // VR_VoiceBoxXmlBuilder xmlBulder;

    // pugi::xml_node xmlNode = xmlBulder.buildDisplayElement("Common", "ScreenDisplay");
    // xmlBulder.buildGivenElement(xmlNode, "agent", "alexa_demo", "", "");
    // xmlBulder.buildGivenElement(xmlNode, "content", JParser.GetKeyValue().c_str(), "", "");
    // xmlBulder.buildGivenElement(xmlNode, "type", JParser.GetKeyValue().c_str(), "", "");
    // xmlBulder.buildGivenElement(xmlNode, "mainTitle", JParser.GetMainTitle().c_str(), "", "");
    // xmlBulder.buildGivenElement(xmlNode, "subTitle", JParser.GetSubTitle().c_str(), "", "");
    // xmlBulder.buildGivenElement(xmlNode, "textField", JParser.GetTextField().c_str(), "", "");

    // std::string strReply = xmlBulder.getXmlString();

    // OnRequestAction(strReply);

    std::string strURL;
    int iID = GetPictureId();
    VR_LOGI("iID : %d", iID);
    std::string strDisplayScreen = JParser.GetBodyTemplate2ReqXml(strURL, iID);
    OnRequestAction(strDisplayScreen);
    // std::string strDispicture = JParser.GetBodyTemplate2ReqPictureXml(strURL, iID);
    // OnRequestAction(strDispicture);
}

void VR_AlexaEngine::OnChangeTemperature(VR_AlexaJSonParser& JParser)
{
    VR_LOGD_FUNC();

    VR_VoiceBoxXmlBuilder xmlBulder;
    pugi::xml_node node = xmlBulder.buildStartActionElement("climate", "changeTemperature");
    (void)xmlBulder.buildGivenElement(node, "degree", JParser.GetTempTemperature().c_str(), "", "");
    std::string strReply = xmlBulder.getXmlString();
    OnRequestAction(strReply);
}

int VR_AlexaEngine::GetPictureId()
{
    VR_LOGD_FUNC();
    BL_AutoSync cAutoSync(m_ActionIDSync);

    int iID = m_iPictureId;
    ++m_iPictureId;

    const int MAX_PICTURE_ID = 100000;
    if (m_iPictureId >= MAX_PICTURE_ID) {
        m_iPictureId = 1;
    }

    return iID;
}

void VR_AlexaEngine::OnPlayRadio(VR_AlexaJSonParser& JParser)
{
    VR_LOGD_FUNC();

    if (m_strAudioItemId == JParser.GetAudioItemId()) {
        m_bSamemusicScreen = true;
        VR_LOGI("it is the same screen");
        return;
    }
    else {
        m_bSamemusicScreen = false;
        m_strAudioItemId = JParser.GetAudioItemId();
    }

    int iID = GetPictureId();

    std::string strMusicDisplayScreen = JParser.GetMusicDisplayXml(iID);
    // std::string strMusicDispicture = JParser.GetMusicDispictureXml(iID);

    OnRequestAction(strMusicDisplayScreen);
    // OnRequestAction(strMusicDispicture);
}

void VR_AlexaEngine::OnNaviToDestination(VR_AlexaJSonParser& JParser)
{
    VR_LOGD_FUNC();

    std::string strDisPlayScreen = JParser.GetRouteXml();
    OnRequestAction(strDisPlayScreen);
}

void VR_AlexaEngine::OnGetTravelTime(VR_AlexaJSonParser& JParser)
{
    // VR_LOGD_FUNC();

    // std::string strDisPlayScreen = JParser.GetTravelTimeXml();
    // OnRequestAction(strDisPlayScreen);
}

void VR_AlexaEngine::OnShowPhoneCall(VR_AlexaJSonParser& JParser)
{
    VR_LOGD_FUNC();

    std::string strDisPlayScreen = JParser.GetPhoneCallXml();
    OnRequestAction(strDisPlayScreen);
}

void VR_AlexaEngine::OnShowContactList(VR_AlexaJSonParser& JParser)
{
    VR_LOGD_FUNC();

    int iID = GetPictureId();
    std::string strId = std::to_string(iID);
    std::string strDisPlayScreen = JParser.GetContactXml(strId);
    OnRequestAction(strDisPlayScreen);
}

void VR_AlexaEngine::OnShowPhoneList(VR_AlexaJSonParser& JParser)
{
    VR_LOGD_FUNC();

    int iID = GetPictureId();
    std::string strId = std::to_string(iID);

    std::string strDisPlayScreen = JParser.GetPhoneListXml(strId);
    OnRequestAction(strDisPlayScreen);
}

void VR_AlexaEngine::OnMakePhoneCall(VR_AlexaJSonParser& JParser)
{
    VR_LOGD_FUNC();

    std::string strPhoneCall = JParser.GetMakeCallXml();
    OnRequestAction(strPhoneCall);
}

void VR_AlexaEngine::OnRoofControl(VR_AlexaJSonParser& JParser)
{
    VR_LOGD_FUNC();

    std::string strDisPlayScreen = JParser.GetRoofControlXml();
    OnRequestAction(strDisPlayScreen);
}

void VR_AlexaEngine::OnAutoStarStop(VR_AlexaJSonParser& JParser)
{
    VR_LOGD_FUNC();

    std::string strDisPlayScreen = JParser.GetAutoStarStopXml();
    OnRequestAction(strDisPlayScreen);
}

void VR_AlexaEngine::OnSetDestinationReq(VR_AlexaJSonParser& JParser)
{
    VR_LOGD_FUNC();

    std::string strURL;
    int iID = GetPictureId();
    VR_LOGI("iID : %d", iID);
    std::string strDisPlayScreen = JParser.GetDestinationReqXml(strURL, iID);
    OnRequestAction(strDisPlayScreen);

    // std::string strMapDispicture = JParser.GetDestinationReqPictureXml(strURL, iID);
    // OnRequestAction(strMapDispicture);
}

void VR_AlexaEngine::OnStop(VR_AlexaJSonParser& JParser)
{
    VR_LOGD_FUNC();

    std::string strQuitSuggestionScreen = "<action agent=\"suggestion\" op=\"quitScreen\"></action>";
    OnRequestAction(strQuitSuggestionScreen);
}

void VR_AlexaEngine::OnFindPlace(VR_AlexaJSonParser& JParser)
{
    VR_LOGD_FUNC();

    std::string strDisPlayScreen = JParser.GetPOIListXml();
    OnRequestAction(strDisPlayScreen);
}

void VR_AlexaEngine::OnWeatherTemplate(VR_AlexaJSonParser& JParser)
{
    VR_LOGD_FUNC();

    std::string strURL;
    int iID = GetPictureId();
    VR_LOGI("iID : %d", iID);
    std::string strDisWeatherScreen = JParser.GetWeatherTemplateReqXml(strURL, iID);
    OnRequestAction(strDisWeatherScreen);
    // std::string strWeatherDispicture = JParser.GetWeatherTemplateReqPictureXml(strURL, iID);
    // OnRequestAction(strWeatherDispicture);
}

void VR_AlexaEngine::OnDirveScore(VR_AlexaJSonParser& JParser)
{
    VR_LOGD_FUNC();

    std::string strScore = JParser.GetDirveScore();
    OnRequestAction(strScore);
}
/* EOF */
