/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
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
#include "stdafx.h"

#include <memory>
#include <map>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include "pugixml.hpp"

#include "VR_EventSenderIF.h"
#include "EV_Define.h"
#include "EV_EventSender.h"
#include "eventmanger/VR_EngineEventSenderIF.h"

#include "VR_RecogTask.h"
#include "VR_TaskQueue.h"
#include "VR_DialogManger.h"
#include "VR_SessionManger.h"
#include "VR_DialogEngineIF.h"
#include "VR_DialogEngineListener.h"
#include "VR_DialogEngineFactory.h"
#include "VR_DailogEngineMessageBuilder.h"
#include "VR_ConfigureIF.h"
#include "VR_ConfigureDEF.h"
#include "VR_UIStartingCondition.h"
#include "VR_NCTutorialManager.h"
#include "action/VR_VehicleInfoProxy.h"
#include "VR_ContentProvider.h"
#include "VR_VoiceTslApp.h"
#include "VR_SmartAgentHandler.h"
#include "VR_VoiceTagHandler.h"
#include "VR_TaskFactoryIF.h"
#include "VR_DictationStatusCheck.h"
#include "VR_VoiceTagTTS.h"
#include "VR_PersonDataEventHandler.h"
#include "VR_PhoneCallResultHandler.h"
#include "VR_NaviStatusMoniter.h"
#include "VR_RunableThread.h"
#include "VR_TtsStatusMoniter.h"
#include "VR_JsonParse.h"
#include "VR_VehicleInfoManager.h"
#include "cutils/properties.h"
#include "BL_AplInfo.h"

#include "VR_UserManager.h"
#include "VR_StockInfoManager.h"
#include "VR_CMInfoManager.h"
#include "VR_SpotlightSearchTask.h"
#include "VR_StartServiceHandler.h"
#include "VR_NaviStatusManager.h"
#include "VR_TunerStatusManager.h"
#include "MediaInfoProvider/NMMediaInfoHelper.h"
#include "VR_TransformXmlToXml.h"
#include "VR_NCDrvInfoProxy.h"

#include "navi/Voice/VoiceRecog/VrNotifySessionClosed.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrStartRecognition.pb.h"
#include "navi/Voice/VoiceRecog/RespVrStartRecognitionResult.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrCancelRecognition.pb.h"
#include "navi/Voice/VoiceRecog/RespVrCancelRecognitionResult.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrCommonProperty.pb.h"
#include "navi/Voice/VoiceRecog/RespVrCommonPropertyResult.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrPVRFlag.pb.h"
#include "navi/Voice/VoiceRecog/RespVrPVRFlagResult.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrReqOperation.pb.h"
#include "navi/Voice/VoiceRecog/RespVrReqOperationResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMTaskMessage.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyCommonProperty.pb.h"
#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrTSL.pb.h"
#include "navi/Voice/TTS/TtsRespPlayState.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyRecogResult.pb.h"
#include "navi/Voice/TTS/TtsStop.pb.h"

#include "navi/Voice/VoiceRecog/RequestSDLVr.pb.h"
#include "navi/Voice/VoiceRecog/RespSDLVrResult.pb.h"

#include "navi/Voice/VoiceRecog/RequestMMVr.pb.h"
#include "navi/Voice/VoiceRecog/RespMMVrResult.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyiAutoProxy.pb.h"
#include "navi/Voice/VoiceRecog/RequestVriAutoProxy.pb.h"
#include "navi/Voice/VoiceRecog/VrAirconditionStatus.pb.h"

#include "navi/NaviInfo/NaviInfoBase.pb.h"
#include "navi/NaviInfo/NaviInfoCancelRoute.pb.h"
#include "navi/NaviInfo/NaviInfoPoiHistory.pb.h"
#include "navi/NaviInfo/NaviInfoPoiHome.pb.h"
#include "navi/NaviInfo/NaviInfoPoiKeywordQuery.pb.h"
#include "navi/NaviInfo/NaviInfoPoiKeywordQueryResult.pb.h"
#include "navi/NaviInfo/NaviInfoPoiNearByGivenAddress.pb.h"
#include "navi/NaviInfo/NaviInfoPoiNearByQuery.pb.h"
#include "navi/NaviInfo/NaviInfoPoiPassPointSearch.pb.h"
#include "navi/NaviInfo/NaviInfoPoiPassPointTypeSearch.pb.h"
#include "navi/NaviInfo/NaviInfoPoiTime.pb.h"
#include "navi/NaviInfo/NaviInfoSetScaleLevel.pb.h"
#include "navi/NaviInfo/NaviInfoSetShowState.pb.h"
#include "navi/NaviInfo/NaviInfoStartNavigation.pb.h"
#include "navi/NaviInfo/NaviInfoPoiAddressBook.pb.h"

#include   "BL_AplInfo.h"

#if defined(VR_DM_TARGET_VRUNIT)
#include "navi/Path/ReqPathPointList.pb.h"
#endif

#ifndef NCFILE_H
#include "ncore/NCFile.h"
#endif

#ifndef NCDIR_H
#include "ncore/NCDir.h"
#endif

#ifndef NCPOWERSTATUS_H
#include "system/NCPowerStatus.h"
#endif

#include "BL_File.h"
#include "BL_DataStorage.h"
#include "CL_AbstractFile.h"
#include "system/NCEnvironment.h"
#include "VR_MusicSpotLightUpdater.h"

#include "VR_NIMusicShareProxy.h"
#include "MediaDataProviderIF/include/NMMediaDataProviderIF.h"

#include "VR_XmlStrParse.h"
#include "VR_VehicleInfoManager.h"
#include "VR_Common_DeviceDef.h"
#include "VR_NaviMessageHandler.h"

#include <sstream>

using namespace navi::VoiceRecog;
using namespace nutshell::tutorial;
using namespace nutshell;
using namespace navi::TTS;
using namespace nutshell::media;
using namespace navi::NaviInfo;

#if defined(VR_DM_TARGET_VRUNIT)
    using namespace navi::Path;
#endif

const  int           VR_DATA_VERSION_SIZE   =  17;
const  std::string   VR_EVENTHANDLER_THREAD = "VR_Voice_EventHandler";

const  char*         MMVR_GRAMMAR_DYNAMIC_XML = "<grammar_dynamic agent=\"SDL\"></grammar_dynamic>";
const  char*         MMVR_NODE_AGENT          = "grammar_dynamic";
const  char*         MMVR_NODE_CATEGORY       = "category";
const  char*         MMVR_CATEGORY_ATTR_NAME  = "name";
const  char*         MMVR_CATEGORY_ATTR_VALUE = "default";
const  char*         MMVR_NODE_FORMAL         = "formal";
const  char*         MMVR_FORMAL_ATTR_ID      = "id";
const  char*         MMVR_FORMAL_ATTR_NAME    = "name";



/**
 * @brief The VR_EngineCallBack class
 *
 * class declaration
 */
class VR_EngineCallBack : public VR_DialogEngineListener
{
public:
    VR_EngineCallBack()
        : m_TaskID(-1)
        , m_spVR_EngineEventSenderIF(VR_EngineEventSenderIF::GetSender())
    {
        VR_LOGD_FUNC();
    }

    void SetTaskID(int id)
    {
        VR_LOGI("task id = %d", id);
        m_TaskID = id;
    }

    virtual void OnRequestAction(const std::string& message, int seqId)
    {
        VR_LOGI("msg=[%s], seqId=%d", (message.c_str()), seqId);
        m_spVR_EngineEventSenderIF->SendEvent(m_TaskID, seqId, message);
    }

    virtual void OnStarted()
    {
        VR_LOGD_FUNC();
    }

    virtual void OnStopped()
    {
        VR_LOGD_FUNC();
    }

protected:
    int                         m_TaskID;
    spVR_EngineEventSenderIF    m_spVR_EngineEventSenderIF;
};

///
/// VR_DialogManger
///
VR_DialogManger::VR_DialogManger(const spVR_SessionManger pVR_SessionManger)
    : m_spEngineListener(VR_new VR_EngineCallBack())
    , m_spTaskQueue(VR_new VR_TaskQueue())
    , m_sessionTaskMap()
    , m_spEventSenderIF(VR_EventSenderIF::GetSender())
//    , m_spTutorialManager(VR_new VR_NCTutorialManager)
    , m_spSessionManger(pVR_SessionManger)
//    , m_spDictationStatus(VR_new VR_DictationStatusCheck(m_spEventSenderIF))
//    , m_bVDataUpdateError(false)
    , m_spMusicShareProxy(VR_new VR_NIMusicShareProxy(PROXY_NAME))
    , m_sVrServiceStatus(VR_STATUS_DISABLE_XML)
    , m_spVehiInfoManager(VR_new VR_VehicleInfoManager())
    , m_spDrvInfoProxy(VR_new VR_NCDrvInfoProxy("nutshell.drvasst.drvinfo"))
    , m_bNetworkStatus(false)
{
    GetVUIWorkRootPath();
    VR_LOGD_FUNC();
#ifdef HAVE_NUTSHELL_OS
    m_spEventHandlerThread = spVR_RunableThread(VR_new  VR_RunableThread(VR_EVENTHANDLER_THREAD.c_str(), false));
#endif
    m_spTaskFactory = VR_TaskFactoryIF::getTaskFactory();

    // create engineer
    VR_ContentProvider vr_cp;
    VR_String strEngineType = vr_cp.GetContentProviderValue(VR_IAUTO_VR_ENGINETYPE);
    VR_LOGI("strEngineType = [%s]", strEngineType.c_str());
#ifdef HAVE_NUTSHELL_OS
    if ("Nuance" == strEngineType) {
        m_spEngineIF = spVR_DialogEngineIF(VR_DialogEngineFactory::CreateDialogEngine(true, strEngineType));
    }
    else {
        m_spEngineIF = spVR_DialogEngineIF(VR_DialogEngineFactory::CreateDialogEngine(false, strEngineType));
    }
#else
    m_spEngineIF = spVR_DialogEngineIF(VR_DialogEngineFactory::CreateDialogEngine(false, strEngineType));
#endif
    m_spSessionManger->SetDialogEngine(m_spEngineIF);
    m_spUIStartCondition = spVR_UIStartingCondition(VR_new VR_UIStartingCondition(m_spEngineIF, m_spEventSenderIF));
    m_spSpotLightSearch  = spVR_SpotlightSearchTask(VR_new VR_SpotlightSearchTask(m_spMusicShareProxy));
    m_spSpotLightUpdater = spVR_MusicSpotLightUpdater(VR_new  VR_MusicSpotLightUpdater());
    m_spStartServiceHandler = spVR_StartServiceHandler(VR_new VR_StartServiceHandler(m_spEngineIF, m_spEventSenderIF));
    m_spVoiceRecogStatus = spVR_VoiceRecogStatus(VR_new  VR_VoiceRecogStatus(m_spEngineIF));
    m_spNaviStatusManager = spVR_NaviStatusManager(VR_new VR_NaviStatusManager());
    m_spTunerStatusManager = spVR_TunerStatusManager(VR_new VR_TunerStatusManager());
    m_mediaDataProvider = spNMMediaDataProviderIF(VR_new NMMediaDataProviderIF());

    m_spNaviMsgHandler = spVR_NaviMessageHandler(VR_new VR_NaviMessageHandler());
    m_spSMSHandler = spVR_SMSHandler(VR_new VR_SMSHandler());
    m_pVRUserManager = VR_new VR_UserManager();
    m_pVrStockInfoManager = VR_new VR_StockInfoManager();
    m_pVRUserManager->AddUserFunc(m_pVrStockInfoManager);
    m_pVRCMInfoManager = VR_new VR_CMInfoManager();
}

VR_DialogManger::~VR_DialogManger()
{
    VR_LOGD_FUNC();

    delete m_pVrStockInfoManager;
    m_pVrStockInfoManager = nullptr;

    delete m_pVrStockInfoManager;
    m_pVrStockInfoManager = nullptr;

    delete m_pVRCMInfoManager;
    m_pVRCMInfoManager = nullptr;
}

void VR_DialogManger::Initialize()
{
    RemoveVBTFlagFile();

    // set VR Server URL
//    std::string strHybridVRUrl = m_spStartGBookHandler->getServerVRUrl();
//    VR_ConfigureIF::Instance()->setHybridVRUrl(strHybridVRUrl);

    // init DE
#ifdef HAVE_NUTSHELL_OS
    VR_Settings settings;
    std::string strVRVersion;
    ConfigureVRDESetting(settings, strVRVersion);
    VR_LOGD("strVRVersion = [%s]", strVRVersion.c_str());

    bool bInitResult = m_spEngineIF->Initialize(m_spEngineListener.get(), settings);
    if (bInitResult) {
        BL_DataStorage   dataStorage;
        int  nDataLength = strVRVersion.length();
        int  nSize  = (nDataLength >= VR_DATA_VERSION_SIZE ? VR_DATA_VERSION_SIZE : nDataLength);
        dataStorage.PutValue(BL_DATA_KEY_VR_VRVERSION, strVRVersion.c_str(), nSize == 0 ? 1 : nSize);
        if (!strVRVersion.empty()) {
            nutshell::NCEnvironment  env;
            NCString  ncVRVersion(strVRVersion.c_str());
            env.update("update/vrversion", ncVRVersion);
        }
    }
#elif HAVE_LINUX_NATIVEX86
    VR_Settings settings;
    std::string strVRVersion;
    ConfigureVRDESetting(settings, strVRVersion);
    VR_LOGD("strVRVersion = [%s]", strVRVersion.c_str());

    m_spEngineIF->Initialize(m_spEngineListener.get(), settings);

#endif
    m_spUIStartCondition->InitializeData();
    m_spStartServiceHandler->Initialize();
    m_mediaDataProvider->onInit();
}

void VR_DialogManger::Start()
{
    VR_LOGD_FUNC();

    m_spEngineIF->Start();
    m_mediaDataProvider->onStart();

    BL_AplInfo::UpdateLazyServicReady();
}

void VR_DialogManger::CheckStatus()
{
    VR_LOGD_FUNC();
    m_spUIStartCondition->NotifyAllData();
    m_spSpotLightUpdater->RequestFileListCount();
//    m_spMusicShareProxy->Initialize();
    m_spSpotLightSearch->ReqModuleStatus();

    VrNotifyiAutoProxy* notifyMessage = new VrNotifyiAutoProxy;
    notifyMessage->set_funccode(VrNotifyiAutoProxy_VrFunc_NotifyServiceReady);
    notifyMessage->mutable_serviceready();
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFIAUTOPROXY, notifyMessage);

    SessionBlinkReset();
    NotifyMicrophoneStatusToFC();
    NotifyMicrophoneStatusToVR();
    QueryAirconditionStatus();
    m_spTunerStatusManager->CheckTunerStationFile();
    m_pVRUserManager->Initialize(); // Tips: after onWake invokes
    m_spSessionManger->OpenMic();
}

void VR_DialogManger::Stop()
{
    VR_LOGD_FUNC();
    m_spEngineIF->Stop();
    m_mediaDataProvider->onStop();
}

void VR_DialogManger::Destory()
{
    VR_LOGD_FUNC();
    m_spEngineIF->UnInitialize();
    m_mediaDataProvider->onDeinit();
}

void VR_DialogManger::OnReqestStartChangeLanguage(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
}

void VR_DialogManger::NotifyMicrophoneStatusToFC()
{
    VR_ContentProvider vr_cp;
    std::string strValue = vr_cp.GetContentProviderValue(VR_MICROPHONE_URL);
    VR_String strXml = "";

    if ("valid" == strValue) {
       strXml = VR_MICROPHONE_CONNECTED_XML;
    }
    else {
       strXml = VR_MICROPHONE_DISCONNECTED_XML;
    }

    strXml = VR_MICROPHONE_CONNECTED_XML; // todo to delete
    std::unique_ptr<VrNotifyCommonProperty> noitfyProperty(VrNotifyCommonProperty::default_instance().New());
    noitfyProperty->set_sender("NATP");
    noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyVRUpdateData);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());
}

void VR_DialogManger::NotifyMicrophoneStatusToVR()
{
    VR_ContentProvider vr_cp;
    std::string strValue = vr_cp.GetContentProviderValue(VR_MICROPHONE_URL);
    VR_String strXml = "";

    if ("valid" == strValue) {
       strXml = VR_MICROPHONE_CONNECTED_XML;
    }
    else {
       strXml = VR_MICROPHONE_DISCONNECTED_XML;
    }

    strXml = VR_MICROPHONE_CONNECTED_XML; // todo to delete
    VR_XmlStrParse xmlStrParse;
    if (xmlStrParse.LoadString(strXml)) {
        xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
        VR_String strMsg = xmlStrParse.TransferXMLtoString();
        SendReqToNATP(strMsg);
    }
    else {
        VR_LOGD("LoadString [%s] error", strXml.c_str());
    }
}

void VR_DialogManger::NotifyInternetStatusToVR()
{
    std::string strStatus = GetInternetStatus();
    VR_String strXml = "";
    if ("connected" == strStatus) {
        strXml = VR_NET_CONNECTED_XML;
    }
    else {
        strXml = VR_NET_DISCONNECTED_XML;
    }

    VR_XmlStrParse xmlStrParse;
    if (xmlStrParse.LoadString(strXml)) {
        xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
        VR_String strMsg = xmlStrParse.TransferXMLtoString();
        SendReqToNATP(strMsg);
    }
    else {
        VR_LOGD("LoadString [%s] error", strXml.c_str());
    }

}

void VR_DialogManger::RespLanguageStatusToVR(VR_String &strId)
{
    VR_String strXml = VR_LANGUAGE_XML;

    // get system language
    VR_ContentProvider vr_cp;
    VR_String strSystemLanguage = vr_cp.GetContentProviderValue(VR_IAUTO_VR_LANGUAGE);
    VR_LOGI("strSystemLanguage = [%s]", strSystemLanguage.c_str());

    VR_XmlStrParse xmlStrParse;
    if (xmlStrParse.LoadString(strXml)) {
        xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
        xmlStrParse.setTextTo(VR_XPATH_ACTION_ID, strId);
        xmlStrParse.setTextTo(VR_XPATH_SYSTEM_LANGUAGE, strSystemLanguage);
        VR_String strMsg = xmlStrParse.TransferXMLtoString();
        SendReqToNATP(strMsg);
    }
    else {
        VR_LOGD("LoadString [%s] error", strXml.c_str());
    }
}

void VR_DialogManger::NotifyLanguageStatusToVR()
{
    VR_String strXml = VR_LANGUAGE_NOTIFY_XML;

    // get system language
    VR_ContentProvider vr_cp;
    VR_String strSystemLanguage = vr_cp.GetContentProviderValue(VR_IAUTO_VR_LANGUAGE);
    VR_LOGI("strSystemLanguage = [%s]", strSystemLanguage.c_str());

    VR_XmlStrParse xmlStrParse;
    if (xmlStrParse.LoadString(strXml)) {
        xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
        xmlStrParse.setTextTo(VR_XPATH_SYSTEM_LANGUAGE, strSystemLanguage);
        VR_String strMsg = xmlStrParse.TransferXMLtoString();
        SendReqToNATP(strMsg);
    }
    else {
        VR_LOGD("LoadString [%s] error", strXml.c_str());
    }
}

void VR_DialogManger::NotifyAwakenWord()
{
    VR_String strXml = VR_AWAKEN_WORD_NOTIFY_XML;

    // get system language
    VR_ContentProvider vr_cp;
    VR_String strAwakenWord = vr_cp.GetContentProviderValue(VR_IAUTO_VR_AWAKENWORD);
    VR_LOGI("strAwakenWord = [%s]", strAwakenWord.c_str());

    VR_XmlStrParse xmlStrParse;
    if (xmlStrParse.LoadString(strXml)) {
        xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
        xmlStrParse.setTextTo(VR_XPATH_WUW_WORD, strAwakenWord);
        VR_String strMsg = xmlStrParse.TransferXMLtoString();
        SendReqToNATP(strMsg);
    }
    else {
        VR_LOGD("LoadString [%s] error", strXml.c_str());
    }
}

void VR_DialogManger::NotifyWuwSwitch()
{
    VR_LOGD_FUNC();

    // get wuw switch
    VR_ContentProvider vr_cp;
    VR_String value = vr_cp.GetContentProviderValue(VR_IAUTO_VR_WUWSWITCH);
    VR_LOGI("value = [%s]", value.c_str());

    VR_XmlStrParse parser;
    bool res = parser.LoadString(VR_AWAKEN_WUWSWITCH_NOTIFY_XML);
    if (!res) {
        VR_LOGD("load failed");
        return;
    }

    parser.setTextTo("//id", std::to_string(GetActionId()));
    if ("ON" == value) {
        parser.setTextTo("//value", "OFF");
    }
    else if ("OFF" == value) {
        parser.setTextTo("//value", "ON");
    }
    else {
        VR_LOGD("unknown attr");
        return;
    }

    VR_String mesg;
    parser.GetXmlString(mesg);

    SendReqToNATP(mesg);
}

void VR_DialogManger::RespAwakenWord(VR_String &strId)
{
    VR_String strXml = VR_AWAKEN_WORD_RESP_XML;

    // get system language
    VR_ContentProvider vr_cp;
    VR_String strAwakenWord = vr_cp.GetContentProviderValue(VR_IAUTO_VR_AWAKENWORD);
    VR_LOGI("strAwakenWord = [%s]", strAwakenWord.c_str());

    VR_XmlStrParse xmlStrParse;
    if (xmlStrParse.LoadString(strXml)) {
        xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
        xmlStrParse.setTextTo(VR_XPATH_ACTION_ID, strId);
        xmlStrParse.setTextTo(VR_XPATH_AWAKEN_WORD, strAwakenWord);
        VR_String strMsg = xmlStrParse.TransferXMLtoString();
        SendReqToNATP(strMsg);
    }
    else {
        VR_LOGD("LoadString [%s] error", strXml.c_str());
    }
}

void VR_DialogManger::QueryAirconditionStatus()
{
    m_spVehiInfoManager->QueryAirconditionStatus();
}

void VR_DialogManger::ReplyDataPathToVR(VR_String &strId)
{
    VR_String strXml = VR_DATA_PATH_XML;

    // get data path
    VR_String strWorkPath = GetVUIWorkRootPath();
    VR_LOGI("strWorkPath=[%s]", strWorkPath.c_str());

    VR_XmlStrParse xmlStrParse;
    if (xmlStrParse.LoadString(strXml)) {
        xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
        xmlStrParse.setTextTo(VR_XPATH_ACTION_ID, strId);
        xmlStrParse.setTextTo(VR_XPATH_WORK_PATH, strWorkPath);
        VR_String strMsg = xmlStrParse.TransferXMLtoString();
        SendReqToNATP(strMsg);
    }
    else {
        VR_LOGD("LoadString [%s] error", strXml.c_str());
    }
}

void VR_DialogManger::ReplyMicrophoneStatusToVR(VR_String &strId)
{
    VR_ContentProvider vr_cp;
    std::string strValue = vr_cp.GetContentProviderValue(VR_MICROPHONE_URL);
    VR_String strXml = "";

    if ("valid" == strValue) {
       strXml = VR_MICROPHONE_CONNECTED_ACTION_XML;
    }
    else {
       strXml = VR_MICROPHONE_DISCONNECTED_ACTION_XML;
    }

    strXml = VR_MICROPHONE_CONNECTED_ACTION_XML; // todo to delete
    VR_XmlStrParse xmlStrParse;
    if (xmlStrParse.LoadString(strXml)) {
        xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
        xmlStrParse.setTextTo(VR_XPATH_ACTION_ID, strId);
        VR_String strMsg = xmlStrParse.TransferXMLtoString();
        SendReqToNATP(strMsg);
    }
    else {
        VR_LOGD("LoadString [%s] error", strXml.c_str());
    }
}

void VR_DialogManger::ReplyInternetStatusToVR(VR_String &strId)
{
    std::string strStatus = GetInternetStatus();
    VR_String strXml = "";
    if ("connected" == strStatus) {
        strXml = VR_NET_CONNECTED_ACTION_XML;
    }
    else {
        strXml = VR_NET_DISCONNECTED_ACTION_XML;
    }

    VR_XmlStrParse xmlStrParse;
    if (xmlStrParse.LoadString(strXml)) {
        xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
        xmlStrParse.setTextTo(VR_XPATH_ACTION_ID, strId);
        VR_String strMsg = xmlStrParse.TransferXMLtoString();
        SendReqToNATP(strMsg);
    }
    else {
        VR_LOGD("LoadString [%s] error", strXml.c_str());
    }
}

void VR_DialogManger::QueryNaviAddrLocation(VR_String &strId, VR_String &addrType)
{
    VR_LOGD_FUNC();
    m_spNaviStatusManager->QueryNaviAddrLocation(strId, addrType);
}

void VR_DialogManger::ReplyNaviStatusToVR(VR_String &strId)
{
    VR_LOGD_FUNC();
    m_spNaviStatusManager->replyNaviStatusToVR(strId);
}

void VR_DialogManger::ReplyVehicleRainyStateToVR(VR_String &strId)
{
    VR_LOGD_FUNC();
    VR_String strXml = VR_VEHICLE_RAINY_STATUS_RESP_XML;
    VR_XmlStrParse xmlStrParse;
    if (!xmlStrParse.LoadString(strXml)) {
        VR_LOGD("LoadString [%s] error", strXml.c_str());
        return;
    }

    xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
    xmlStrParse.setTextTo(VR_XPATH_ACTION_ID, strId);
    VR_String strMsg = xmlStrParse.TransferXMLtoString();
    SendReqToNATP(strMsg);
}

VR_String VR_DialogManger::GetInternetStatus()
{
    VR_LOGD_FUNC();
    nutshell::NCEnvironment environment;
    nutshell::NCString ncStrValue;
    nutshell::NCString ncStrKey("internet/status");

    environment.get(ncStrKey, ncStrValue);

    return ncStrValue.getString();
}

void VR_DialogManger::setNetWorkStatus(bool status)
{
    VR_LOGD_FUNC();
    m_bNetworkStatus = status;
}

VR_String VR_DialogManger::MusicChangeSource(VR_String &strId, bool incNet)
{
    VR_LOGD_FUNC();
    VR_String strXml = VR_MEDIA_PLAY_LOCAL_MUSIC_XML;
    VR_XmlStrParse xmlStrParse;
    if (!xmlStrParse.LoadString(strXml)) {
        VR_LOGD("LoadString [%s] error", strXml.c_str());
        return "";
    }
    xmlStrParse.setTextTo(VR_XPATH_ID, strId);
    bool bSuccess = false;
    do {
        if (VR_ConfigureIF::Instance()->getMediaConnectStatus(usb_connect)) {
            xmlStrParse.setTextTo(VR_MEDIA_SONG_NAME, VR_MEDIA_SOUECE_NAME_USB);
            xmlStrParse.setTextTo(VR_MEDIA_SONG_SOURCE_ID, VR_MEDIA_SOUECE_ID_USB);
            bSuccess = true;
            break;
        }
        if (VR_ConfigureIF::Instance()->getMediaConnectStatus(ipod_connect)) {
            xmlStrParse.setTextTo(VR_MEDIA_SONG_NAME, VR_MEDIA_SOUECE_NAME_IPOD);
            xmlStrParse.setTextTo(VR_MEDIA_SONG_SOURCE_ID, VR_MEDIA_SOUECE_ID_IPOD);
            bSuccess = true;
            break;
        }
        if (VR_ConfigureIF::Instance()->getMediaConnectStatus(btaudio_connect)) {
            xmlStrParse.setTextTo(VR_MEDIA_SONG_NAME, VR_MEDIA_SOUECE_NAME_BTAUDIO);
            xmlStrParse.setTextTo(VR_MEDIA_SONG_SOURCE_ID, VR_MEDIA_SOUECE_ID_BTAUDIO);
            bSuccess = true;
            break;
        }
        if (GetInternetStatus() == "connected" && incNet) {
            xmlStrParse.setTextTo(VR_MEDIA_SONG_NAME, VR_MEDIA_SOUECE_NAME_NETWORK);
            xmlStrParse.setTextTo(VR_MEDIA_SONG_SOURCE_ID, VR_MEDIA_SOUECE_ID_NETWORK);
            bSuccess = true;
            break;
        }
    } while (0);

    if (bSuccess) {
        VR_String strMsg = xmlStrParse.TransferXMLtoString();
        return strMsg;
    }
    else {
        strXml = VR_MEDIA_PLAY_LOCAL_MUSIC_PESP_XML;
        if (!xmlStrParse.LoadString(strXml)) {
            VR_LOGD("LoadString [%s] error", strXml.c_str());
            return "";
        }
        xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
        xmlStrParse.setTextTo(VR_XPATH_ACTION_ID, strId);
        VR_String strMsg = xmlStrParse.TransferXMLtoString();
        SendReqToNATP(strMsg);
        return "";
    }
}

VR_String VR_DialogManger::MusicPlayFavorites(VR_String &strId)
{
    VR_LOGD_FUNC();
    VR_String strXml = VR_MEDIA_PLAY_LOCAL_MUSIC_XML;
    VR_XmlStrParse xmlStrParse;
    if (!xmlStrParse.LoadString(strXml)) {
        VR_LOGD("LoadString [%s] error", strXml.c_str());
        return "";
    }
    xmlStrParse.setTextTo(VR_XPATH_ID, strId);
    xmlStrParse.setTextTo(VR_MEDIA_SONG_TYPE, VR_MEDIA_SONG_TYPE_FAVORIATES);
    bool bSuccess = false;
    do {
        if (VR_ConfigureIF::Instance()->getMediaConnectStatus(usb_connect)) {
            xmlStrParse.setTextTo(VR_MEDIA_SONG_SOURCE_ID, VR_MEDIA_SOUECE_ID_USB);
            bSuccess = true;
            break;
        }
        if (GetInternetStatus() == "connected") {
            VR_LOGD("GetInternetStatus   usb_connect");
            xmlStrParse.setTextTo(VR_MEDIA_SONG_SOURCE_ID, VR_MEDIA_SOUECE_ID_NETWORK);
            bSuccess = true;
            break;
        }
    } while (0);

    if (bSuccess) {
        VR_String strMsg = xmlStrParse.TransferXMLtoString();
        return strMsg;
    }
    else {
        strXml = VR_MEDIA_PLAY_LOCAL_MUSIC_PESP_XML;
        if (!xmlStrParse.LoadString(strXml)) {
            VR_LOGD("LoadString [%s] error", strXml.c_str());
            return "";
        }
        xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
        xmlStrParse.setTextTo(VR_XPATH_ACTION_ID, strId);
        xmlStrParse.setTextTo(VR_MEDIA_ERROR_NO, VR_MEDIA_RESULT_ERROR);
        VR_String strMsg = xmlStrParse.TransferXMLtoString();
        SendReqToNATP(strMsg);
        return "";
    }
}

void VR_DialogManger::MusicPlayOnline(VR_String &strXml)
{
    VR_LOGD_FUNC();

    VR_XmlStrParse xmlStrParse;
    if (!xmlStrParse.LoadString(strXml)) {
        VR_LOGD("LoadString [%s] error", strXml.c_str());
        return;
    }

    XmlMapVector songList = xmlStrParse.GetXMLListValue(VR_MEDIA_MUSIC_INFO);

    VoiceList<NMFileInfo>::type fileList;

    if (!songList.empty()) {
        for (XmlMap map : songList) {
            NMFileInfo fileInfo;
            VR_String strUrlSong = map[VR_MEDIA_NODE_URLSONG].c_str();
            fileInfo.uri = strUrlSong.c_str();
            fileInfo.tags.artwork = map[VR_MEDIA_NODE_URLPHONE].c_str();
            fileInfo.tags.artist = map[VR_MEDIA_NODE_ARTISTNAME].c_str();
            fileInfo.tags.genre = map[VR_MEDIA_NODE_TYPENAME].c_str();
            fileInfo.title = map[VR_MEDIA_NODE_SONGNAME].c_str();
            size_t nPos = strUrlSong.rfind("/");
            if (nPos != std::string::npos) {
                fileInfo.name = strUrlSong.substr(strUrlSong.rfind("/") + 1).c_str();
                fileInfo.path = strUrlSong.substr(0, strUrlSong.rfind("/")).c_str();
            }
            else {
                fileInfo.name = strUrlSong.c_str();
            }
            fileInfo.type = AUDIO;
            fileInfo.device = "nuance";
            fileInfo.deviceUuid = "nuance";
            fileList.push_back(fileInfo);
        }
        m_mediaDataProvider->insertFileData(fileList, true);
    }
    else {
        VR_LOGE("musicList is empty");
    }
}

void VR_DialogManger::OnMessage(const VrActionFC2DM &proto)
{
    VrDMActionType actionType = proto.vr_dm_action_type();
    if (ActionType_TTS_COMMAND_RESULT != actionType) {
        VR_LOGD("recive reply from fc actiontype:%d", actionType);
        return;
    }

    if (!proto.has_tts_result()) {
        VR_LOGD("proto.has_tts_result not exist");
        return;
    }

    // notify tutorial tts playstate
    navi::TTS::TtsRespPlayState ttsPlayState;
    ttsPlayState.set_sender(VR_NCTUTORIAL_TTS_PLAY_SENDER_NAME);
    ttsPlayState.set_reqid(proto.id());
    switch (proto.tts_result().func_code()) {
    case TTSProcessType_Play:
    {
        ttsPlayState.set_state(navi::TTS::TtsRespPlayState_TTS_PLAY_STATE_TTS_PLAYER_START);
        break;
    }
    default:
    {
        ttsPlayState.set_state(navi::TTS::TtsRespPlayState_TTS_PLAY_STATE_TTS_PLAYER_STOPED);
        break;
    }
    }
    // ttsPlayState.set_state(proto.tts_result().func_code());
    // m_spTutorialManager->NotifyPlayMediaEnd(&ttsPlayState);
}

void VR_DialogManger::OnMsgBody(const VR_String& strMsgBody)
{
    m_spSMSHandler->ReponseMsgBody(strMsgBody);
}

void VR_DialogManger::OnReqestStartDialog(const EV_EventContext &context, const VR_ProtoMessageBase &msg, SessionChecker checker)
{
    VR_LOGP("StartDialog");
    const RequestVrStartRecognition &aVrReqStartRecognition = (const RequestVrStartRecognition &)msg;

    const VR_String& session = aVrReqStartRecognition.session();
    int enum_type = aVrReqStartRecognition.type(); // ::navi::VoiceRecog::RequestVrStartRecognition_VrDomainType

    VR_LOGI("checker(session)=[%d]", checker(session));
    if (checker(session)) {
        m_spSessionManger->SetStartAgent(true);
        spVR_DialogTaskBase dialogTask(VR_new VR_RecogTask(enum_type, session, context, m_spEngineIF));
        boost::dynamic_pointer_cast<VR_EngineCallBack>(m_spEngineListener)->SetTaskID(dialogTask->GetTaskID());
        m_spTaskQueue->PushTask(dialogTask);
        AddTask2Session(session, dialogTask);
    }
    else {
        std::unique_ptr<RespVrStartRecognitionResult> replyMsg(RespVrStartRecognitionResult::default_instance().New());
        replyMsg->set_session(session);
        replyMsg->set_taskid(VR_RecogTask::INVALID);
        replyMsg->set_result(VrOpeResult_Undefined);
        m_spEventSenderIF->Reply(context, replyMsg.release());
    }
}

void VR_DialogManger::OnReqestCancleDialog(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    const RequestVrCancelRecognition &aRequestVrCancelRecognition = (const RequestVrCancelRecognition &)msg;
    int id = aRequestVrCancelRecognition.taskid();
    spVR_DialogTaskBase dialogTask = m_spTaskQueue->FindTaskByID(id);
    VR_LOGI("dialogTask.get()=[%p]", dialogTask.get());
    if (NULL != dialogTask.get()) {
        dialogTask->Cancel(context);
        boost::dynamic_pointer_cast<VR_EngineCallBack>(m_spEngineListener)->SetTaskID(-1);
    }
    else {
        std::unique_ptr<RespVrCancelRecognitionResult> replyMsg(RespVrCancelRecognitionResult::default_instance().New());
        replyMsg->set_taskid(id);
        replyMsg->set_result(VrOpeResult_Undefined);
        m_spEventSenderIF->Reply(context, replyMsg.release());
    }
}

void VR_DialogManger::OnReqestUserOperation(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    const RequestVrReqOperation &aRequestVrReqOperation = (const RequestVrReqOperation &)msg;
    const int       id          = aRequestVrReqOperation.taskid();
    const VR_String operation   = aRequestVrReqOperation.operation();
    VR_LOGI("operation=[%s]", operation.c_str());

    // spec deal with pvr flag ....
    if (operation == "set_pvr_flag") {

        VR_LOGI("get set_pvr_flag proto, and update pvrflag,,,,");
        // reply fc the msg ok...
        std::unique_ptr<RespVrReqOperationResult> replyMsg(RespVrReqOperationResult::default_instance().New());
        replyMsg->set_taskid(id);
        replyMsg->set_result(VrOpeResult_OK);
        m_spEventSenderIF->Reply(context, replyMsg.release());

#ifdef HAVE_NUTSHELL_OS
        // updatepvrflag
        VR_ContentProvider cp;
        cp.UpdateContentProviderValue("content://settings/system", "voicesetting_pvr", "off");
#endif
        return; // pvrflag end
    }
    else if (operation == "Debug_Input_On") {
        VR_LOGI("operation == Debug_Input_On");
        ReplyRequestDebuginfo(context);
        VR_ConfigureIF::Instance()->setInputWavOption(true);
        const VR_String debugMenuXML = VR_DialogEngineMessageBuilder::CreateDebugMenuMsg("input", 1);
        m_spEngineIF->SendMessage(debugMenuXML);
        return;
    }
    else if (operation == "Debug_Input_Off") {
        VR_LOGI("operation == Debug_Input_Off");
        ReplyRequestDebuginfo(context);
        VR_ConfigureIF::Instance()->setInputWavOption(false);
        const VR_String debugMenuXML = VR_DialogEngineMessageBuilder::CreateDebugMenuMsg("input", 0);
        m_spEngineIF->SendMessage(debugMenuXML);
        return;
    }
    else if (operation == "Debug_Output_On") {
        VR_LOGI("operation == Debug_Output_On");
        ReplyRequestDebuginfo(context);
        VR_ConfigureIF::Instance()->setOutputWavOption(true);
        const VR_String debugMenuXML = VR_DialogEngineMessageBuilder::CreateDebugMenuMsg("output", 1);
        m_spEngineIF->SendMessage(debugMenuXML);
        return;
    }
    else if (operation == "Debug_Output_Off") {
        VR_LOGI("operation == Debug_Output_Off");
        ReplyRequestDebuginfo(context);
        VR_ConfigureIF::Instance()->setOutputWavOption(false);
        const VR_String debugMenuXML = VR_DialogEngineMessageBuilder::CreateDebugMenuMsg("output", 0);
        m_spEngineIF->SendMessage(debugMenuXML);
        return;
    }
    else if (operation == "ptt_hard_key_press_special_in_endtaskstatus") {
        VR_String strEndTask = VR_DialogEngineMessageBuilder::UsrOperationMsg("endTask");
        m_spEngineIF->SendMessage(strEndTask);
        return;
    }
    else if (operation == "hard_key_cancel_vr") {
        VR_String strCancelVR = VR_DialogEngineMessageBuilder::UsrOperationMsg("hardKeyCanceVR");
        m_spEngineIF->SendMessage(strCancelVR);
        return;
    }
    else if ((operation == "tutorials") || (operation == "voice_training") || (operation == "setting_voice_training_start")) {
        VR_LOGI("operation == tutorials || operation == voice_training || operation == setting_voice_training_start");
        std::unique_ptr<RespVrReqOperationResult> replyMsg(RespVrReqOperationResult::default_instance().New());
        replyMsg->set_taskid(id);
        replyMsg->set_result(VrOpeResult_OK);
        m_spEventSenderIF->Reply(context, replyMsg.release());

        const VR_String operationXML = VR_DialogEngineMessageBuilder::UsrOperationMsg(operation);
        m_spEngineIF->SendMessage(operationXML);
        return;
    }
    spVR_DialogTaskBase dialogTask = m_spTaskQueue->FindTaskByID(id);
    VR_LOGI("dialogTask.get()=[%p]", dialogTask.get());
    if (NULL != dialogTask.get()) {
        dialogTask->UserOperation(context, operation);
    }
    else {
        std::unique_ptr<RespVrReqOperationResult> replyMsg(RespVrReqOperationResult::default_instance().New());
        replyMsg->set_taskid(id);
        replyMsg->set_result(VrOpeResult_Undefined);
        m_spEventSenderIF->Reply(context, replyMsg.release());
    }

}

void VR_DialogManger::OnReqestCommonProperty(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const RequestVrCommonProperty &aVrReqCommonProperty = (const RequestVrCommonProperty &)msg;

    std::unique_ptr<RespVrCommonPropertyResult> replyMsg(RespVrCommonPropertyResult::default_instance().New());
    replyMsg->set_sender(aVrReqCommonProperty.sender());
    replyMsg->set_taskid(aVrReqCommonProperty.taskid());

    RequestVrCommonProperty* requestCommProperty = const_cast<RequestVrCommonProperty*>(&aVrReqCommonProperty);
    RequestVrCommonProperty_VrRequestCommonFunc requestType = aVrReqCommonProperty.funccode();
    switch (requestType) {
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqFileListCount:
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqFileListData:
        {
            if (aVrReqCommonProperty.has_reqfilelistcount()) {
                m_spSpotLightUpdater->OnRequestCommonProerty(context, aVrReqCommonProperty);
            }
            else if (aVrReqCommonProperty.has_reqfilelistdata()) {
                m_spSpotLightUpdater->OnRequestCommonProerty(context, aVrReqCommonProperty);
            }
            else {
                VR_LOGE("file list is abnormal");
            }
        }
        break;
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqKeyWordsSearchInfo:
        {
            if (aVrReqCommonProperty.has_reqkeywordssearchinfo()) {
                VR_LOGD("aVrReqCommonProperty.has_reqkeywordssearchinfo()");
                const  VrReqKeyWordsSearchInfo&  searchinfo = aVrReqCommonProperty.reqkeywordssearchinfo();
                m_spSpotLightSearch->OnRequestCommonProperty(context, searchinfo);
            }
            else {
               VR_LOGE("reqkeywordssearchinfo is abnormal");
            }
        }
        break;
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqCancelSearchFor:
        {
            if (aVrReqCommonProperty.has_reqcancelsearchfor()) {
                VR_LOGD("aVrReqCommonProperty.has_reqcancelsearchfor()");
                const  VrReqCancelSearchFor&  cancelSearch = aVrReqCommonProperty.reqcancelsearchfor();
                m_spSpotLightSearch->OnRequestCancelSearchResult(context, cancelSearch);
            }
            else {
                VR_LOGE("VrReqCancelSearchFor is abnormal");
            }
        }
        break;
    case RequestVrCommonProperty_VrRequestCommonFunc_GetHints:
        {
            VR_LOGP("DM : Revcive gethints request case:215-1 215-1");
            if (aVrReqCommonProperty.has_gethints()) {
                VR_LOGI("aVrReqCommonProperty.has_gethints()");
                VrGetHintsResult* hintsResult =  replyMsg->mutable_gethintsresult();
                std::string abc("hints");
                hintsResult->set_hints(abc);
                m_spEventSenderIF->Reply(context, replyMsg.release());

                int hintType = aVrReqCommonProperty.gethints().type();
                VR_String hintsParam = VR_DialogEngineMessageBuilder::CreateGetHintsDialogMsg(hintType);
                const std::string strContent = m_spEngineIF->getHints(hintsParam);
                VR_LOGD("strContent = [%s]", strContent.c_str());
                std::unique_ptr<VrNotifyRecogResult> protoMsg(VrNotifyRecogResult::default_instance().New());
                protoMsg->set_sender("vr_s");
                protoMsg->set_taskid(aVrReqCommonProperty.taskid());
                protoMsg->set_recogresult(strContent);
                EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYRECOGRESULT, protoMsg.release());
                VR_LOGP("DM: GetHintsResult From DE Case:215-1 215-2");
            }
            else {
                VR_LOGE("Gethints is abnormal");
            }
        }
        return;
    case RequestVrCommonProperty_VrRequestCommonFunc_GetAgentActivation:
        {
            if (aVrReqCommonProperty.has_getagentactivation()) {
                VR_LOGI("aVrReqCommonProperty.has_getagentactivation()");
                VrGetAgentActivationResult * agentActivationResult = replyMsg->mutable_getagentactivationresult();
                agentActivationResult->set_agentactivation("agentactivation");
            }
            else {
                VR_LOGE("GetAgentActivation is abnormal");
            }
        }
        break;
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqVoiceTagStatus:
        {
        }
        return;
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqStartConditions:
        {
            if (aVrReqCommonProperty.has_reqstartcondition()) {
                VR_LOGI("aVrReqCommonProperty.has_reqstartcondition()");
                m_spUIStartCondition->OnReqestCommonProperty(context, msg);
                m_spSpotLightUpdater->RequestFileListCount();
         //       this->UpdateErrorONS();
            }
            else {
                VR_LOGE("GetReqStartCondition is abnormal");
            }
            return; // if reqstartcondition case, we will go to uistartcondition class .
        }
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqServiceStatus:
        {
            // for servicestatus
            if (aVrReqCommonProperty.has_reqservicestatus()) {
                VR_LOGP("DM:Recive ServiceStatus Requst");
                m_spSessionManger->CheckAudioSource();
       //         m_spStartServiceHandler->OnReqestCommonProperty(context, msg);
            }
            else {
                VR_LOGI("aVrReqCommonProperty.has_reqservicestatus error..");
            }
            return;
        }
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqHybirdCommuConnectStatus :
        {
            // for hybirdcommu connect status
            if (aVrReqCommonProperty.has_hybirdcommuconnectstatus()) {
      //          m_spStartGBookHandler->OnReqestCommonProperty(context, msg);
            }
            else {
                VR_LOGD("aVrReqCommonProperty.has_hybirdcommuconnectstatus() error");
            }
            return;
        }
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqServerStatus:
        {
            // this is only for test, the interface cannot define,,,,
            // serverstatus ....
            if (aVrReqCommonProperty.has_reqserverstatus()) {
                VR_LOGI("aVrReqCommonProperty.has_reqserverstatus()");
                VrReqServerStatusResult* serverStatus = replyMsg->mutable_reqserverstatusresult();
                VrReqServerStatusResult_VrServerStatus sStatus = VrReqServerStatusResult_VrServerStatus_Unavaiable;
                VR_ConfigureIF* configIF = VR_ConfigureIF::Instance();
                if (configIF) {
                    bool bHybridVRFlag = configIF->getHybridVRFlag();
                    if (bHybridVRFlag) {
                        VR_LOGI("status = VrReqServerStatusResult_VrServerStatus_Avaiable");
                        sStatus = VrReqServerStatusResult_VrServerStatus_Avaiable;
                    }
                }
                serverStatus->set_status(sStatus);
            }
            else {
                VR_LOGI("aVrReqCommonProperty.has_reqserverstatus error...");
            }
            break;
        }

    case RequestVrCommonProperty_VrRequestCommonFunc_ReqPlayMedia:
        {
//            m_spTutorialManager->StartPlayMedia(requestCommProperty->mutable_reqplaymedia(),
//                                                replyMsg->mutable_reqplaymediaresult());
        }
        break;
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqStopMedia:
        {
//            m_spTutorialManager->StopPlayMedia(requestCommProperty->mutable_reqstopmedia(),
//                                               replyMsg->mutable_reqstopmediaresult());
        }
        break;
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqMediaListData:
        {
//            m_spTutorialManager->LoadTutorialItemsList(replyMsg->mutable_reqmedialistdataresult());
        }
        break;
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqMediaPlayInfo:
        {
//            m_spTutorialManager->LoadTutorialSingleItemInfo(requestCommProperty->mutable_reqmediaplayinfo(),
//                                                            replyMsg->mutable_reqmediaplayinforesult());
        }
        break;
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqStartTutorialsMode:
        {
            if (m_spSessionManger->IsTutorialSessionClosed()) {
                replyMsg->mutable_reqstarttutorialsmoderesult()->set_starttype(0);
            }
            else {
                replyMsg->mutable_reqstarttutorialsmoderesult()->set_starttype(1);
            }
        }
        break;
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqCancelTutorialsMode:
        {
//            m_spTutorialManager->CancelTutorialsMode(requestCommProperty->mutable_reqcanceltutorialsmode(),
//                                                     replyMsg->mutable_reqcanceltutorialsmoderesult());
        }
        break;
    case RequestVrCommonProperty_VrRequestCommonFunc_DisplayStatus:
        {
            if (aVrReqCommonProperty.has_displaystatus()) {
                VR_LOGI("aVrReqCommonProperty.has_displaystatus");
                std::string cCommStatus = aVrReqCommonProperty.displaystatus().status();
                m_spEngineIF->SendMessage(VR_DialogEngineMessageBuilder::PvrTTSRequestMsg(cCommStatus));
                VrDisplayStatusResult* displayStatusresult =  replyMsg->mutable_displaystatusresult();
                std::string result("ok");
                displayStatusresult->set_result(result);
            }
            else {
                VR_LOGE("DisplayStatus is abnormal");
            }
        }
        break;
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqStartDictation:
        {
            if (aVrReqCommonProperty.has_reqstartdictation()) {
                VR_LOGI("aVrReqCommonProperty.has_reqstartdictation");
      //          m_spVR_DictationTask->Start(context, msg);
            }
            else {
                VR_LOGE("ReqStartDictation is abnormal");
            }
        }
        break;
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqDictationStatus:
        {
            if (aVrReqCommonProperty.has_dictationstatus()) {
                VR_LOGI("aVrReqCommonProperty.has_dictationstatus");
      //          m_spDictationStatus->OnReqestCommonProperty(context, msg);
            }
            else {
                VR_LOGE("ReqDictationstatus is abnormal");
            }
            return;
        }
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqPlayVoiceTagTTS:
        {
            if (aVrReqCommonProperty.has_reqplayvoicetagtts()) {
                VR_LOGI("aVrReqCommonProperty.has_reqplayvoicetagtts");
      //          m_spVoiceTagTTS->Start(context, msg);
            }
            else {
                VR_LOGE("ReqPlayVoiceTagTTS is abnormal");
            }
            return;
        }
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqIsVoiceSettingAvailable:
        {
            if (aVrReqCommonProperty.has_reqisvoicesettingavailable()) {
            }
            else {
                VR_LOGE("has_reqisvoicesettingavailable is abnormal");
            }
        }
        break;
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqServiceInfo:
        {
            if (aVrReqCommonProperty.has_reqserviceinfo()) {
                VR_LOGI("aVrReqCommonProperty.has_reqserviceinfo");
                m_spSessionManger->CheckAudioSource();
         //       m_spStartServiceHandler->OnReqestVRStatusWithHints(context, aVrReqCommonProperty.reqserviceinfo());
            }
            else {
                VR_LOGE("has_reqserviceinfo is abnormal");
            }

            return;
        }
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqServiceIdleStatus:
        {
            if (aVrReqCommonProperty.has_reqserviceidlestatus()) {
   //             m_spStartServiceHandler->OnReqestVRServiceIdleStatus(context, aVrReqCommonProperty.reqserviceidlestatus());
            }
            else {
                VR_LOGE("has_reqserviceidlestatus is abnormal");
            }
            return;
        }
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqDomainTypeHints:
        {
            OnRequestDomainTypeHints(context, aVrReqCommonProperty);
            break;
        }
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqUpdateHintsActionResultToVR:
        {
            OnRequestUpdateHintsActionResultToVR(context, aVrReqCommonProperty);
            break;
        }
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqStartVUI:
        {
            VR_ContentProvider vr_cp;
            VR_String strEngineType = vr_cp.GetContentProviderValue(VR_IAUTO_VR_ENGINETYPE);
            VR_LOGI("strEngineType = [%s]", strEngineType.c_str());
            if ("Nuance" != strEngineType) {
                if (!VR_ConfigureIF::Instance()->setEngineCurrentStatus(VR_Engine_Alexa_Activtity)) {
                    VR_LOGD("setEngineCurrentStatus(VR_Engine_Alexa_Activtity) failed!");
                    return;
                }
                else {
                     VR_LOGD("setEngineCurrentStatus(VR_Engine_Alexa_Activtity) success!");
                }
                if (!aVrReqCommonProperty.reqstartvui().has_domaintypeinfo()) {
                    return;
                }
                if (VR_String::npos != aVrReqCommonProperty.reqstartvui().domaintypeinfo().find("notifyLongPTTPressed")) {
                    exit(-1);
                }
                else {
                    OnStartVRNotify();
                }
            }
            else {
                OnRequestStartVUI(context, aVrReqCommonProperty);
            }
            break;
        }
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqStartVRDiagActionResultToVR:
        {
            OnRequestStartVRDiagActionResultToVR(context, aVrReqCommonProperty);
            break;
        }
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqStopVRDiag:
        {
            VR_ContentProvider vr_cp;
            VR_String strEngineType = vr_cp.GetContentProviderValue(VR_IAUTO_VR_ENGINETYPE);
            VR_LOGI("strEngineType = [%s]", strEngineType.c_str());
            if ("Nuance" != strEngineType) {
                const VR_String strMsg = "<event name=\"cancel\" option=\"allQuit\"></event>";
                m_spEngineIF->SendMessage(strMsg);
            }
            else {
                OnRequestStopVRDiag(context, aVrReqCommonProperty);
            }
            break;
        }
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqStopVRDiagActionResultToVR:
        {
            OnRequestStopVRDiagActionResultToVR(context, aVrReqCommonProperty);
            break;
        }
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqVrServiceStatus:
        {
            OnRequestVrServiceStatus(context, aVrReqCommonProperty);
            break;
        }
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqCommonAcitonResult:
        {
            OnRequestCommonActionResult(context, aVrReqCommonProperty);
            break;
        }
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqWeatherInfo:
        {
            OnNotifyWeatherScreenChangedToNATP(aVrReqCommonProperty);
            break;
        }
    case RequestVrCommonProperty_VrRequestCommonFunc_NotifyUserManualInfo:
        {
            if (!aVrReqCommonProperty.has_notifyuserinfo()) {
                VR_LOGD("has not user info");
                return;
            }

            m_pVRUserManager->HandleUserManual(&aVrReqCommonProperty.notifyuserinfo());
            return;
        }
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqSendJsonStrToAlexa:
        {
            bool bRet = false;
            VR_String strSender;
            VR_INT32 nSenderId = -1;
            VR_String strMsg;
            if (aVrReqCommonProperty.has_reqsendjsonstrtoalexa()) {
                strMsg = aVrReqCommonProperty.reqsendjsonstrtoalexa().jsonstring();
                strSender = aVrReqCommonProperty.reqsendjsonstrtoalexa().sender();
                nSenderId = aVrReqCommonProperty.reqsendjsonstrtoalexa().senderid();
                VR_LOGI("strMsg=[%s]", strMsg.c_str());
                VR_JsonParse aJsonParse;
                VR_String strRet =  aJsonParse.ParseJson(strMsg);
                if (!strRet.empty()) {
                    bRet = true;
                    m_spSessionManger->OpenChannel(strRet);
                }
            }
            else {
                VR_LOGI("aVrReqCommonProperty.has_reqsendjsonstrtoalexa error..");
            }
            VrReqSendJsonStrToAlexVRResult *pVrReqSendJsonStrToAlexVRResult = replyMsg->mutable_reqsendjsonstrtoalexvrresult();
            pVrReqSendJsonStrToAlexVRResult->set_sender(strSender);
            pVrReqSendJsonStrToAlexVRResult->set_senderid(nSenderId);
            pVrReqSendJsonStrToAlexVRResult->set_result(bRet);
            m_spEventSenderIF->Reply(context, replyMsg.release());

            return;
        }
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqDrivingAssistantInfo:
        {
            VR_LOGI("DrvInfo_iAutoLinkReqDrvInfo");
            m_spDrvInfoProxy->reqDrivingAssistant();
            return;
        }
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqDictationFuncStatus:
        {
            VR_LOGI("ReqDictationFuncStatus");

            std::unique_ptr<VrNotifyCommonProperty> noitfyProperty(VrNotifyCommonProperty::default_instance().New());
            noitfyProperty->set_sender("fc");
            noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyDictationVrServiceStatus);

            if (m_bNetworkStatus) {
                noitfyProperty->mutable_vrservicestatus()->set_status(VrNotifyDictationVrServiceStatus_VrDictationVrServiceStatus_Idle);
            }
            else {
                noitfyProperty->mutable_vrservicestatus()->set_status(VrNotifyDictationVrServiceStatus_VrDictationVrServiceStatus_Invalid);
            }

            EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());

            return;
        }
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqStartDictationFunc:
        {
            VR_LOGD("ReqStartDictationFunc");
            VR_LOGD("getEngineCurrentStatus [%d]", VR_ConfigureIF::Instance()->getEngineCurrentStatus());

            if (VR_ConfigureIF::Instance()->setEngineCurrentStatus(VR_Engine_Dictation_Activtity)) {
                m_spEngineIF->SendMessage("<event name=\"Dictation\"><status>start</status></event>");
            }
            return;
        }
    case RequestVrCommonProperty_VrRequestCommonFunc_ReqCancelDictationFunc:
        {
            VR_LOGI("ReqCancelDictationFunc");
            m_spEngineIF->SendMessage("<event name=\"Dictation\"><status>cancel</status></event>");
            return;
        }
    default:
        break;
    }
    // m_spEventSenderIF->Reply(context, replyMsg.release());
}

void VR_DialogManger::OnReqestVrPVRFlag(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const RequestVrPVRFlag &aVrPVRFlag = (const RequestVrPVRFlag &)msg;

    std::unique_ptr<RespVrPVRFlagResult> replyMsg(RespVrPVRFlagResult::default_instance().New());
    replyMsg->set_sender(aVrPVRFlag.sender());
    replyMsg->set_flag(false);
    m_spEventSenderIF->Reply(context, replyMsg.release());
}

void VR_DialogManger::OnDEReplyMessage(const VR_ProtoMessageBase &msg)
{
    const VrEngineControlMessage& engineMessage = (const VrEngineControlMessage&)msg;
    VrDMTaskMessageType  engineType  = engineMessage.type();

    if (engineType == DMTaskMessageType_StartCondition) {
        m_spUIStartCondition->OnMessage(engineMessage.startcondition_comfirm());
        return;
    }
    else if (engineType == DMTaskMessageType_TransmitDataToTSLNotify) {
   //     m_spVR_VoiceTslApp->OnMessage(engineMessage.transmitdatatotslnotify());
        return;
    }
    else if (engineType == DMTaskMessageType_StartService) {
 //       m_spStartServiceHandler->OnMessage(engineMessage.start_service_conditon());
        return;
    }
    else if (engineType == DMTaskMessageType_UnitStatusResult) {
 //       m_spStartServiceHandler->OnMessage(engineMessage.unitstatusnotify());
        return;
    }
    else if (engineType == DMTaskMessageType_StartStatusResult) {
        m_spStartServiceHandler->OnMessage(engineMessage.startstatusresult());

        if (Micphone_Status == engineMessage.startstatusresult().starttype()) {
            NotifyMicrophoneStatusToFC();
            NotifyMicrophoneStatusToVR();
        }
        else if (Internet_Status == engineMessage.startstatusresult().starttype()) {
            NotifyInternetStatusToVR();
        }
        else if (Engine_Updating == engineMessage.startstatusresult().starttype()) {
            if (!engineMessage.startstatusresult().startreuslt()) {
                m_spSessionManger->OnEngineInitedMsg();
            }
        }

        return;
    }
    else if (engineType == DMTaskMessageType_SmartAgent) {
   //     m_spSmartAgentHandler->OnMessage(engineMessage.smartagentresult());
        return;
    }
    else if (engineType == DMTaskMessageType_InitPersonalData) {
  //      m_spPersonalDataHandler->OnMessage(engineMessage.personalresult());
        return;
    }
    else if (engineType == DMTaskMessageType_VoiceTag) {
  //      m_spVoiceTagHandler->OnMessage(engineMessage.syncvoicetagresult());
        return;
    }
    else if (engineType == DMTaskMessageType_GetVoiceTagInfo) {
  //      m_spVoiceTagHandler->OnMessage(engineMessage.getvoicetaginfo());
        return;
    }
    else if (engineType == DMTaskMessageType_AvcLanDicResp) {
   //     m_spVR_DictationTask->OnMessage(engineMessage.avclan_dictation_response());
        return;
    }
    else if (engineType == DMTaskMessageType_AvcLanDicResult) {
    //    m_spVR_DictationTask->OnMessage(engineMessage.avclan_dictation_result());
        return;
    }
    else if (engineType == DMTaskMessageType_GViewMessage) {
    //    m_spGViewerProxy->OnMessage(engineMessage.gviewnofitymessage());
        return;
    }
    else if (engineType == DMTaskMessageType_CancelEnd) {
        VrCancelOptionType eCancelOptionType = engineMessage.cancel_end().cancel_option();
        if (m_spSessionManger) {
            m_spSessionManger->CloseChannel();
        }
        // usb cancel end
        if ((VrCancelOptionType_Default != eCancelOptionType) && (VrCancelOptionType_CloseSession != eCancelOptionType)) {
//           m_spUsbMoniter->OnMessage(engineMessage.cancel_end());
        }
        else {
            return;
        }
    }
    else if (engineType == DMTaskMessageType_GBookUserType) {
  //      m_spStartGBookHandler->OnMessage(engineMessage.gbookuserresult());
        return;
    }
    else if (engineType == DMTaskMessageType_VBTLogSwitchType) {
  //      m_spUsbMoniter->OnMessage(engineMessage.vbtswitchresult());
        return;
    }
    else if (engineType == DMTaskMessageType_DiagEventResult) {
 //       m_spUsbMoniter->OnMessage(engineMessage.diageventresult());
        m_spSpotLightUpdater->ClearUsbData();
        return;
    }
    else if (engineType == DMTaskMessageType_NaviCrashNotify) {
   //     m_spNaviStatusMoniter->OnMessage(engineMessage.navicrashnotify());
        return;
    }
    else if (engineType == DMTaskMessageType_PhoneCallResult) {
    //    m_spPhoneCallResult->OnMessage(engineMessage.phonecallresult());
        return;
    }
    else if (engineType == DMTaskMessageType_CheckAppListExist) {
   //     m_spVR_VoiceTslApp->OnMessage(engineMessage.checkapplist());
        return;
    }

    spVR_DialogTaskBase dialogTask = m_spTaskQueue->FindTopTask();
    VR_LOGI("dialogTask.get()=[%p]", dialogTask.get());
    if (dialogTask.get()) {
        dialogTask->OnMessage(msg);
    }

}

/************************* Manual VR****************************************/
void  VR_DialogManger::OnMMVRRequestVRServiceStatus(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_spStartServiceHandler->OnMMVRRequestVRServiceStatus(context, msg);
}

void  VR_DialogManger::OnMMVRRegistCommands(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const VrRegistMMVrCommands& registCommands = (const VrRegistMMVrCommands&)msg;
    const std::string& strSession = registCommands.session();
    const MMVrCommandInfoList& list = registCommands.commandslist();
    int nCommandS = list.command_size();
    if (nCommandS <= 0) {
        VR_LOGD("regist mmvr command size is zero.");
        return;
    }
    pugi::xml_document doc;
    pugi::xml_parse_result result;
    do {
        result = doc.load_string(MMVR_GRAMMAR_DYNAMIC_XML);
        if (pugi::status_ok != result.status) {
            break;
        }
        pugi::xml_node agentNode = doc.child(MMVR_NODE_AGENT);
        if (agentNode.empty()) {
            break;
        }
        pugi::xml_node categoryNode = agentNode.append_child(MMVR_NODE_CATEGORY);
        if (categoryNode.empty()) {
            break;
        }
        categoryNode.append_attribute(MMVR_CATEGORY_ATTR_NAME) = MMVR_CATEGORY_ATTR_VALUE;
        for (int n = 0; n < nCommandS; ++n) {
            pugi::xml_node formalNode = categoryNode.append_child(MMVR_NODE_FORMAL);
            if (formalNode.empty()) {
                continue;
            }
            formalNode.append_attribute(MMVR_FORMAL_ATTR_ID) = (n + 1);
            formalNode.append_attribute(MMVR_FORMAL_ATTR_NAME) = list.command(n).c_str();
        }
    } while (0);

    std::stringstream  ss;
    doc.print(ss);
    std::string strCommands = ss.str();
    m_spEngineIF->SendMessage(strCommands);

    std::unique_ptr<RespMMVrResult>  MMVRResult(RespMMVrResult::default_instance().New());
    MMVRResult->set_sender("dm");
    MMVRResult->set_funccode(VrMMFunc_RegistVrCommands);
    VrRegistMMVrCommandsResult* commandResult = MMVRResult->mutable_registmmvrcommandsresult();
    commandResult->set_session(strSession);
    commandResult->set_operresult(VrOpeResult_OK);
    m_spEventSenderIF->Reply(context, MMVRResult.release());
}

void  VR_DialogManger::OnMMVRRequestStartDialog(const EV_EventContext &context,
                                                const VR_ProtoMessageBase &msg,
                                                SessionChecker checker)
{
    VR_LOGD_FUNC();
    const VrStartMMVoiceRecog& aReqStartRecog = (const VrStartMMVoiceRecog&)msg;

    const VR_String& session = aReqStartRecog.session();
    int enum_type = aReqStartRecog.domaintype();
    int screen_type = aReqStartRecog.screentype();
    const VR_String& strprompt = aReqStartRecog.prompt();

    VR_LOGD("checker(session)=[%d]", checker(session));
    if (checker(session)) {
        m_spSessionManger->SetStartAgent(true);
        spVR_DialogTaskBase dialogTask(VR_new  MMVR_RecogTask(enum_type, session, context, m_spEngineIF, strprompt, screen_type));
        boost::dynamic_pointer_cast<VR_EngineCallBack>(m_spEngineListener)->SetTaskID(dialogTask->GetTaskID());
        m_spTaskQueue->PushTask(dialogTask);
        AddTask2Session(session, dialogTask);
    }
    else {
        std::unique_ptr<RespMMVrResult>  MMVRResult(RespMMVrResult::default_instance().New());
        MMVRResult->set_sender("dm");
        MMVRResult->set_funccode(VrMMFunc_StartVoiceRecog);
        VrStartMMVoiceRecogResult* startVoiceRecog = MMVRResult->mutable_startmmvoicerecogresult();
        startVoiceRecog->set_taskid(VR_RecogTask::INVALID);
        startVoiceRecog->set_session(session);
        startVoiceRecog->set_operresult(VrOpeResult_Undefined);
        m_spEventSenderIF->Reply(context, MMVRResult.release());
    }

}

void  VR_DialogManger::OnMMVRRequestCancelDialog(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const VrCancelMMVoiceRecog& aReqCancelRecog = (const VrCancelMMVoiceRecog&)msg;
    int taskid = aReqCancelRecog.taskid();
    spVR_DialogTaskBase task = m_spTaskQueue->FindTaskByID(taskid);
    if (NULL != task.get()) {
        task->Cancel(context);
        boost::dynamic_pointer_cast<VR_EngineCallBack>(m_spEngineListener)->SetTaskID(-1);
    }
    else {
        std::unique_ptr<RespMMVrResult>  MMVRResult(RespMMVrResult::default_instance().New());
        MMVRResult->set_sender("dm");
        MMVRResult->set_funccode(VrMMFunc_CancelVoiceRecog);
        VrCancelMMVoiceRecogResult* cancelVoiceRecog = MMVRResult->mutable_cancelmmvoicerecogresult();
        cancelVoiceRecog->set_taskid(taskid);
        cancelVoiceRecog->set_operresult(VrOpeResult_Undefined);
        m_spEventSenderIF->Reply(context, MMVRResult.release());
    }
}

/************************** SDL VR *****************************************************/
void  VR_DialogManger::OnSDLVRRequestVRServiceStatus(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_spStartServiceHandler->OnSDLVRRequestVRServiceStatus(context, msg);
}

void  VR_DialogManger::OnSDLVRRequestStartDialog(const EV_EventContext &context,
                                                 const VR_ProtoMessageBase &msg,
                                                 SessionChecker checker)
{
    VR_LOGD_FUNC();
//    VR_LOGP("StartDialog");
    const SDLVrStartSDLVoiceRecog& startVoiceRecog = (const SDLVrStartSDLVoiceRecog&)msg;

    const VR_String& session = startVoiceRecog.session();
    int enum_type = startVoiceRecog.domaintype(); // ::navi::VoiceRecog::RequestVrStartRecognition_VrDomainType
    int timeout = startVoiceRecog.timeout();
    const VR_String& prompt = startVoiceRecog.prompt();

    VR_LOGI("checker(session)=[%d]", checker(session));
    if (checker(session)) {
        m_spSessionManger->SetStartAgent(true);
        spVR_DialogTaskBase dialogTask(VR_new SDLVR_RecogTask(enum_type, session, context, m_spEngineIF, timeout, prompt));
        boost::dynamic_pointer_cast<VR_EngineCallBack>(m_spEngineListener)->SetTaskID(dialogTask->GetTaskID());
        m_spTaskQueue->PushTask(dialogTask);
        AddTask2Session(session, dialogTask);
    }
    else {
        std::unique_ptr<RespSDLVrResult>  SDLVRResult(RespSDLVrResult::default_instance().New());
        SDLVRResult->set_sender("dm");
        SDLVRResult->set_funccode(SDLVrFunc_StartSDLVoiceRecog);
        SDLVrStartSDLVoiceRecogResult* startVoiceRecog = SDLVRResult->mutable_startsdlvoicerecogresult();
        startVoiceRecog->set_session(session);
        startVoiceRecog->set_taskid(VR_RecogTask::INVALID);
        startVoiceRecog->set_operresult(VrOpeResult_Undefined);
        m_spEventSenderIF->Reply(context, SDLVRResult.release());
    }
}

void  VR_DialogManger::OnSDLVRRequestCancelDialog(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const SDLVrCancelSDLVoiceRecog& aReqCancelVoice = (const SDLVrCancelSDLVoiceRecog&)msg;
    int id = aReqCancelVoice.taskid();
    spVR_DialogTaskBase dialogTask = m_spTaskQueue->FindTaskByID(id);
    VR_LOGI("dialogTask.get()=[%p]", dialogTask.get());
    if (NULL != dialogTask.get()) {
        dialogTask->Cancel(context);
        boost::dynamic_pointer_cast<VR_EngineCallBack>(m_spEngineListener)->SetTaskID(-1);
    }
    else {
        std::unique_ptr<RespSDLVrResult>  SDLVRResult(RespSDLVrResult::default_instance().New());
        SDLVRResult->set_sender("dm");
        SDLVRResult->set_funccode(SDLVrFunc_CancelSDLVoiceRecog);
        SDLVrCancelSDLVoiceRecogResult* cancleVoiceRecog = SDLVRResult->mutable_cancelsdlvoicerecogresult();
        cancleVoiceRecog->set_taskid(id);
        cancleVoiceRecog->set_operresult(VrOpeResult_Undefined);
        m_spEventSenderIF->Reply(context, SDLVRResult.release());
    }
}

void VR_DialogManger::OnRequestVriAutoProxy(const VR_String& strXml)
{
    VR_LOGD_FUNC();

    VR_XmlStrParse xmlStrParse;
    if (!xmlStrParse.LoadString(strXml)) {
        VR_LOGE("load_string error");
        return;
    }

    std::unique_ptr<VrNotifyCommonProperty> noitfyProperty(VrNotifyCommonProperty::default_instance().New());
    noitfyProperty->set_sender("NATP");

    VR_String strType = xmlStrParse.GetXMLValue(VR_XPATH_TYPE);
    VR_String strId = xmlStrParse.GetXMLValue(VR_XPATH_QUERY_ID);
    VoiceMap<VR_String, emVriAutoProxyType>::const_iterator iter = g_iAutoRouter.find(strType);
    if (iter == g_iAutoRouter.end()) {
        VR_LOGE("Type[%s] don't exist!", strType.c_str());
        return;
    }
    VR_LOGD("VriAutoProxyType is [%s],xml is [%s]", strType.c_str(), strXml.c_str());

#ifdef VR_DM_TARGET_WUKONG
    switch (iter->second) {
    case VriAutoProxyType_Navi_ToDestination:
    case VriAutoProxyType_Navi_SetMode:
    case VriAutoProxyType_Navi_GetAddrLocation:
    case VriAutoProxyType_Navi_DistTimeToAddr:
    case VriAutoProxyType_Navi_GetNaviStatus:
    case VriAutoProxyType_Navi_SetRouteOption:
    {
        m_spNaviMsgHandler->OnRequestVriAutoProxy(strXml);
        return;
    }
    default:
    {
        VR_LOGD("type %d in wukong temp do not add function!");
        break;
    }
    }

#endif

    switch (iter->second) {
    case VriAutoProxyType_HMI_ShowList: {
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyUpdateHintsAction);
        noitfyProperty->mutable_updatehintsaction()->set_hintscontent(strXml);

        VR_String strScenario = xmlStrParse.GetXMLValue(VR_WEATHER_PARAMS_SCENARIO);
        if (VR_WEATHER_WEATHER_INFO == strScenario) {
            NotifyUpdateWeatherData(strXml);
        }

        VR_String val;
        bool res = xmlStrParse.GetNodeText("//HMI_CM_TYPE", val);
        if (!res) {
            break;
        }

        if ("CM_INQUIRE_HOW" != val) {
            break;
        }

        xmlStrParse.GetNodeText("//HMI_CM_CATEGORY", val);
        m_pVRCMInfoManager->onCarManualHowStart(val);
        break;
    }
    case VriAutoProxyType_StartVRDiag: {
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyStartVRDiagAction);
        noitfyProperty->mutable_startvrdiagaction()->set_actioncontent(strXml);
        break;
    }
    case VriAutoProxyType_System_SetState: {
        m_sVrServiceStatus = strXml;
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyVRUpdateData);
        noitfyProperty->mutable_vrupdatedata()->set_updatedata(strXml);

        VR_String state;
        VR_String action;
        xmlStrParse.GetNodeText("//system_state", state);
        xmlStrParse.GetNodeText("//system_state_action", action);

        if ("SYSTEM_STATE_DIALOG" == state && "SYSTEM_STATE_END" == action) {
            m_pVRCMInfoManager->onCarManualHowEnd();
        }
        break;
    }
    case VriAutoProxyType_System_SetSoundLevel: {
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyVRUpdateData);
        noitfyProperty->mutable_vrupdatedata()->set_updatedata(strXml);
        break;
    }
    case VriAutoProxyType_StopVRDiag: {
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyStopVRDiagAction);
        noitfyProperty->mutable_stopvrdiagaction()->set_actioncontent(strXml);
        break;
    }
    case VriAutoProxyType_HMI_SetVideo:
    case VriAutoProxyType_VUICommonAction:
    case VriAutoProxyType_Navi_ToDestination:
    case VriAutoProxyType_Navi_SetRouteOption:
    case VriAutoProxyType_Navi_SetViaPoint:
    case VriAutoProxyType_Navi_SetMode:
    case VriAutoProxyType_Navi_SetPresetAddress:
    case VriAutoProxyType_Media_SetStatus:
    case VriAutoProxyType_Media_PlayOnlineAudio:
    case VriAutoProxyType_Phone_MakeCall:
    case VriAutoProxyType_Phone_CallOperate:
    case VriAutoProxyType_Vehicle_SetFanSpeed:
    case VriAutoProxyType_Vehicle_TurnACOnOff:
    case VriAutoProxyType_Vehicle_SetTemperature:
    case VriAutoProxyType_Vehicle_Control:
    case VriAutoProxyType_VB_ResetVehiSetting:
    case VriAutoProxyType_VbUser_IdentifyUser:
    case VriAutoProxyType_VbUser_Verify_VBName:
    case VriAutoProxyType_SMS_SendSMS:
    case VriAutoProxyType_VbUser_SetCurrentUser: {
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyCommonAction);
        noitfyProperty->mutable_vuicommonaction()->set_action(strXml);
        break;
    }
    case VriAutoProxyType_VbUser_SaveUser: {
        VR_String vp_id;
        xmlStrParse.GetNodeText("//user_id", vp_id);

        bool IsExist = false;
        bool res = m_pVRUserManager->QueryUserExistsStatus(vp_id, IsExist);
        if (!res) {
            ReplyAddUserError(strId);
            VR_LOGD("error happened");
            return;
        }
        xmlStrParse.AddNode("//params", "rename", IsExist ? "true" : "false");

        VR_String xml;
        xmlStrParse.GetXmlString(xml);
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyCommonAction);
        noitfyProperty->mutable_vuicommonaction()->set_action(xml);
        break;
    }
    case VriAutoProxyType_Media_PlayOnlineMusic: {
        VR_String strMsg = strXml;
        MusicPlayOnline(strMsg);
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyCommonAction);
        noitfyProperty->mutable_vuicommonaction()->set_action(strXml);
        break;
    }
    case VriAutoProxyType_Media_PlayLocalMusic: {
        VR_String strType = xmlStrParse.GetXMLValue(VR_MEDIA_SONG_TYPE);
        VR_String strMsg = strXml;
        if (VR_MEDIA_SONG_TYPE_MUSIC == strType) {
            strMsg = MusicChangeSource(strId);
            if (strMsg.empty()) {
                return;
            }
        }
        else if (VR_MEDIA_SONG_LOCAL_MUSIC == strType) {
            strMsg = MusicChangeSource(strId, false);
            if (strMsg.empty()) {
                return;
            }
        }
        else if (VR_MEDIA_SONG_TYPE_FAVORIATES == strType) {
            strMsg = MusicPlayFavorites(strId);
            if (strMsg.empty()) {
                return;
            }
        }
        else if (VR_MEDIA_SONG_TYPE_RECENT_SRC == strType) {
            if (!IsSourceSupportResume()) {
                ReplyMusicPlayError(strId);
                return;
            }
        }

        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyCommonAction);
        noitfyProperty->mutable_vuicommonaction()->set_action(strMsg);
        break;
    }
    case VriAutoProxyType_Navi_DistTimeToAddr: {
        VR_String queryType = xmlStrParse.GetXMLValue(VR_NAVI_QUERY_TYPE);
        VR_LOGD("queryType is [%s]", queryType.c_str());
        if (VR_NAVI_QUERY_DESTINATION == queryType) {
            m_spNaviStatusManager->ReplyNaviTimeToAddrToVR(strId);
        }
        else if (VR_NAVI_QUERY_POI == queryType) {
            m_spNaviStatusManager->QueryNaviToDestinationFromFC(strXml);
        }
        else {
            m_spNaviStatusManager->ReplyDestTimeToAddrErrorToVR(strId);
        }
        return;
    }
    case VriAutoProxyType_Navi_SrchCategoryByAddr: {
        m_spNaviStatusManager->QueryCategoryDestinationFromFC(strXml);
        return;
    }
    case VriAutoProxyType_Navi_GetAddrLocation: {
        VR_String addrType = xmlStrParse.GetXMLValue(VR_NAVI_ADDR_TYPE);
        VR_LOGD("AddrType is [%s]", addrType.c_str());
        QueryNaviAddrLocation(strId, addrType);
        return;
    }
    case VriAutoProxyType_Navi_GetNaviStatus: {
        ReplyNaviStatusToVR(strId);
        return;
    }
    case VriAutoProxyType_QueryMicInfo: {
        // todo
        return;
    }
    case VriAutoProxyType_QueryMicStatus: {
        ReplyMicrophoneStatusToVR(strId);
        return;
    }
    case VriAutoProxyType_QueryInternetStatus: {
        ReplyInternetStatusToVR(strId);
        return;
    }
    case VriAutoProxyType_System_GetLanguage: {
        RespLanguageStatusToVR(strId);
        return;
    }
    case VriAutoProxyType_System_GetDataPath: {
        ReplyDataPathToVR(strId);
        return;
    }
    case VriAutoProxyType_Tuner_GetStatus: {
        m_spTunerStatusManager->ReplyTunerGetStatus(strId);
        return;
    }
    case VriAutoProxyType_Tuner_PlayControl: {
        m_spTunerStatusManager->QueryTunerPlayControl(strXml);
        return;
    }
    case VriAutoProxyType_SET_AWAKEN_WORD: {
        RespAwakenWord(strId);
        return;
    }
    case VriAutoProxyType_Natp_Started: {
        NotifyInternetStatusToVR();
        NotifyLanguageStatusToVR();
        NotifyWuwSwitch();
#ifdef VR_DM_TARGET_MUIFA
        m_spNaviStatusManager->NotifyRouteOptionChanged();
        m_spNaviStatusManager->OnMessageNaviLocInfoUpdate();
#endif
        break;
    }
    case VriAutoProxyType_Vehicle_QueryACStatus: {
        m_spVehiInfoManager->ReplyVehicleACStatusToVR(strId);
        return;
    }
    case VriAutoProxyType_Vehicle_GetSunroofStatus: {
        m_spVehiInfoManager->ReplyVehicleSunroofStatusToVR(strId);
        return;
    }
    case VriAutoProxyType_Vehicle_GetCurSpeed: {
        m_spVehiInfoManager->ReplyVehicleCurSpeedToVR(strId);
        return;
    }
    case VriAutoProxyType_Vehicle_CheckRainyState: {
        ReplyVehicleRainyStateToVR(strId);
        return;
    }
    case VriAutoProxyType_Stock_GetFavorite: {
        m_pVrStockInfoManager->GetFavoriteStocksXml(strXml);
        return;
    }
    case VriAutoProxyType_Stock_SetFavorite: {
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyCommonAction);
        noitfyProperty->mutable_vuicommonaction()->set_action(strXml);
        break;
    }
    case VriAutoProxyType_SMS_GetMsg: {
        m_spSMSHandler->GetSMSMessage(strXml);
        return;
    }
    case VriAutoProxyType_CM_Inquire: {
        m_pVRCMInfoManager->onRequestCarManualInquire(strXml);
        return;
    }
    case VriAutoProxyType_GetFuelConsumption: {
        m_spVehiInfoManager->ReplyVehicleFuelConsumption(strId);
        return;
    }
    case VriAutoProxyType_GetMileageLeft: {
        m_spVehiInfoManager->ReplyVehicleMileAgeLeft(strId);
        return;
    }
    case VriAutoProxyType_GetMileagePeriod: {
        m_spVehiInfoManager->ReplyVehicleMileAgePeriod(strId);
        return;
    }
    case VriAutoProxyType_GetNextMaintenanceInfo: {
        m_spVehiInfoManager->ReplyVehicleNextMaintanceInfo(strId);
        return;
    }
    case VriAutoProxyType_GetWindowsStatus: {
        m_spVehiInfoManager->ReplyVehicleWindowsStatus(strId);
        break;
    }
    default: {
        VR_LOGE("strType:[%s] is out of VriAutoProxyType", strType.c_str());
        return;
    }
    }

    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());
}

void VR_DialogManger::OnUpdateAircondStatus(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_spVehiInfoManager->UpdateAircondStatus(msg);
}

void VR_DialogManger::OnUpdateVehicleStatus(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_spVehiInfoManager->UpdateVehicleStatus(msg);

    m_pVRCMInfoManager->UpdateVehicleInfoStatus(msg);
}

void  VR_DialogManger::NotifyVrServiceStatus()
{
    VR_LOGD_FUNC();

    std::unique_ptr<VrNotifyCommonProperty> noitfyProperty(VrNotifyCommonProperty::default_instance().New());
    noitfyProperty->set_sender("NATP");
    noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyVRUpdateData);
    noitfyProperty->mutable_vrupdatedata()->set_updatedata(m_sVrServiceStatus);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());
}

void  VR_DialogManger::OnStartMessage(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    const  VrAgentActivation& agentActivation = (const VrAgentActivation&)msg;
    VrAgentActivationType type = agentActivation.type();
    if (type == VrAgentActivationType_ComfirmReq) {
        m_spSessionManger->CheckAudioSource();
    }
 //   m_spStartServiceHandler->OnStartMessage(msg);
}

void  VR_DialogManger::OnReceiveMessage(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

//    m_spVoiceRecogStatus->OnRecevieMessage(msg);
//    m_spStartServiceHandler->OnReceiveMessage(msg);
}

spVR_DialogEngineIF VR_DialogManger::GetEngine()
{
    VR_LOGD_FUNC();
    return m_spEngineIF;
}

void VR_DialogManger::OnSessionClosed(const VR_ProtoMessageBase &msg)
{
    VrNotifySessionClosed &aNotifyMsg = (VrNotifySessionClosed &)msg;
    VR_LOGI("aNotifyMsg.has_session()=[%d]", aNotifyMsg.has_session());
    if (aNotifyMsg.has_session()) {
        const std::string session = aNotifyMsg.session();
        typedef VR_SessionTaskMap::iterator MapItr;
        typedef VR_SessionTaskMap::value_type MapElement;
        std::pair<MapItr, MapItr> result = m_sessionTaskMap.equal_range(session);
        std::for_each(result.first, result.second,
                      ([](MapElement& element) {
                          spVR_DialogTaskBase task = element.second.lock();
                          if (task) {
                              task->Cancel(*VR_EventSenderIF::CopyEventContext(NULL));
                          }
                      }));
        m_sessionTaskMap.erase(result.first, result.second);
    }
}

void VR_DialogManger::OnRequestVrTsl(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
//    m_spVR_VoiceTslApp->OnRequestVrTsl(context, msg);
    const RequestVrTSL& requestVrTsl = (const RequestVrTSL&)msg;
    VrTSLFunc funccode = requestVrTsl.funccode();
    if (VrTSLFunc_AppsInstallingState == funccode) {
 //       m_spStartServiceHandler->RequestVrTslState(context, msg);
    }
}

void   VR_DialogManger::OnRequestVoiceTagStatus(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
 //   m_spStartServiceHandler->OnReqestVoiceTagStatus(context, msg);
}

void VR_DialogManger::OnNotifyUICrash(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
 //   m_spStartServiceHandler->NotifyUICrash(msg);
    m_sVrServiceStatus = VR_STATUS_DISABLE_XML;
    NotifyVrServiceStatus();
}

void VR_DialogManger::TransforLanguage()
{
    VR_LOGD_FUNC();

    m_spUIStartCondition->TransforLanguage();
}

void VR_DialogManger::OnNotifySettingStatus()
{
    VR_LOGD_FUNC();
//    m_spStartServiceHandler->OnNotifyVoiceSettingAvailable();
//    m_spStartServiceHandler->OnNotifyTslServiceAvailable();
//    m_spStartServiceHandler->OnNotifyVoiceTagAvailable();
}

void  VR_DialogManger::OnMessageWeatherDataServiceRes(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
//    m_spWeatherDataService->OnMessage(msg);

}

void  VR_DialogManger::OnMessageWeatherDataServiceUpdate(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
//    m_spWeatherDataService->OnMessage(msg);
}

void  VR_DialogManger::OnMessageSpotlightSearchResult(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_spSpotLightSearch->OnMessage(msg);
}

void   VR_DialogManger::OnMessageSearchModuleStatus(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_spSpotLightSearch->OnMessageSearchStatus(msg);
}

void VR_DialogManger::OnMessageNaviStatusUpdate(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_spNaviStatusManager->OnMessageNaviStatusUpdate(msg);
}

void VR_DialogManger::OnMessageNaviLocInfoUpdate(const VR_ProtoMessageBase &msg)
{
    m_spNaviStatusManager->OnMessageNaviLocInfoUpdate();
}

void VR_DialogManger::OnMessageNaviInfoResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_spNaviMsgHandler->OnMessageNaviResp(msg);
}

bool VR_DialogManger::OnStartVRNotify()
{
    VR_LOGD_FUNC();

    bool bRet = false;
    if (m_spStartServiceHandler->CheckAwakenStatus() && (VR_SessionManger::VR_SourceStatus_Idle == m_spSessionManger->GetSourceStatus())) {
        m_spSessionManger->OpenChannel();
        bRet = true;
    }
    else {
        VR_LOGI("status is unavailable");
    }

    return bRet;
}

void VR_DialogManger::OnVrStartResult(const bool bRet)
{
    VR_LOGI("bRet=[%d]", bRet);
    if (bRet) {
        VR_String strState = "SYSTEM_STATE_DIALOG";
        VR_String strStateAction = "SYSTEM_STATE_BEGIN";
        VR_String strMsg = VR_TransformXmlToXml().CreateStateXML(0, strState, strStateAction);
        std::unique_ptr<VrNotifyCommonProperty> noitfyProperty(VrNotifyCommonProperty::default_instance().New());
        noitfyProperty->set_sender("Alexa");
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyVRUpdateData);
        noitfyProperty->mutable_startvrdiagaction()->set_actioncontent(strMsg);

        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());
    }
    else {
        m_spSessionManger->CloseChannel();
    }
}

void VR_DialogManger::OnReqDrvInfoMessage()
{
    VR_LOGD("DrvInfo_OnReqDrvInfoMessage");
    m_spDrvInfoProxy->reqDrivingAssistant();
}

void VR_DialogManger::OnReqDrvInfoMessage(const VR_String& strValue)
{
    VR_LOGI("strValue is %s", strValue.c_str());

    m_spDrvInfoProxy->setDrvInfoState(VR_NCDrvInfoProxy::DrvInfoState::State_User_Command);
    int days = 0;
    if ("week" == strValue) {
        days = 7;
    }
    if ("month" == strValue) {
        days = 30;
    }
    if ("year" == strValue) {
        days = 365;
    }
    m_spDrvInfoProxy->reqDrivingAssistantByUser(days * 24);
}

void VR_DialogManger::OnMessageCategySearchInfo(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_spNaviStatusManager->ReplyNaviSrchCategoryToVR(msg);
}


void VR_DialogManger::AddTask2Session(const VR_String &session, wpVR_DialogTaskBase task)
{
    VR_LOGD_FUNC();
    m_sessionTaskMap.insert(std::make_pair(session, task));
}

void VR_DialogManger::ReplyRequestDebuginfo(const EV_EventContext &context)
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespVrReqOperationResult> replyMsg(RespVrReqOperationResult::default_instance().New());
    replyMsg->set_result(VrOpeResult_OK);
    m_spEventSenderIF->Reply(context, replyMsg.release());
}

void VR_DialogManger::SessionBlinkReset()
{
    VR_String keyName = "local.navi.VR.startstatus";
    VR_String strDefault = "firststart";
    char keyValue[PROPERTY_VALUE_MAX] = { 0 };
    property_get(keyName.c_str(), keyValue, strDefault.c_str());
    VR_LOGI("keyValue : [%s]", keyValue);
    VR_String strValue = keyValue;
    if (strValue != strDefault) {
        VRSessionBlinkReset();
    }
}

void VR_DialogManger::VRSessionBlinkReset()
{
    VR_LOGD_FUNC();

    // set session_using
    std::unique_ptr<VrLoopBack>    loopMessage(VrLoopBack::default_instance().New());
    loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
    VrEngineControlMessage*  controlMessage = loopMessage->mutable_m_controlmessage();
    controlMessage->set_type(DMTaskMessageType_StartStatusResult);
    controlMessage->mutable_startstatusresult()->set_starttype(Session_Using);
    controlMessage->mutable_startstatusresult()->set_startreuslt(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());

    m_spSessionManger->GetCurAudioSource();
}

void  VR_DialogManger::DoDiagBlinkReset()
{
    VR_LOGD_FUNC();
}

void VR_DialogManger::OnRequestDomainTypeHints(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    const RequestVrCommonProperty& commonProperty = (const RequestVrCommonProperty&)msg;
    do {
        if (!commonProperty.has_reqdomaintypehints()) {
            VR_LOGE("has_reqdomaintypehints is abnormal");
            break;
        }
        if (!commonProperty.reqdomaintypehints().has_domaintypeinfo()) {
            VR_LOGE("has_domaintypeinfo is abnormal");
            break;
        }
        VR_String strXml = commonProperty.reqdomaintypehints().domaintypeinfo();
        if (strXml.empty()) {
            VR_LOGE("domaintypeinfo is empty");
            break;
        }
        ReplyDomainTypeHints(context, commonProperty, true);
        VR_ContentProvider vr_cp;
        VR_String strEngineType = vr_cp.GetContentProviderValue(VR_IAUTO_VR_ENGINETYPE);
        VR_LOGI("strEngineType = [%s]", strEngineType.c_str());
        if ("Nuance" == strEngineType) {
            NotifyVrServiceStatus();
        }

        VR_XmlStrParse xmlStrParse;

        if (xmlStrParse.LoadString(strXml)) {
            xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
            VR_String strMsg = xmlStrParse.TransferXMLtoString();
            SendReqToNATP(strMsg);
            return;
        }
        else {
            VR_LOGD("LoadString [%s] error", strXml.c_str());
        }

    } while (0);

    ReplyDomainTypeHints(context, commonProperty, false);
    return;
}

void VR_DialogManger::OnRequestUpdateHintsActionResultToVR(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    const RequestVrCommonProperty& commonProperty = (const RequestVrCommonProperty&)msg;
    do {
        if (!commonProperty.has_requpdatehintsactionresulttovr()) {
            VR_LOGE("has_requpdatehintsactionresulttovr is abnormal");
            break;
        }
        if (!commonProperty.requpdatehintsactionresulttovr().has_actionresult()) {
            VR_LOGE("has_actionresult is abnormal");
            break;
        }
        VR_String strXml = commonProperty.requpdatehintsactionresulttovr().actionresult();
        if (strXml.empty()) {
            VR_LOGE("actionresult is empty");
            break;
        }
        ReplyUpdateHintsActionResultToVR(context, commonProperty, true);
        VR_XmlStrParse xmlStrParse;

        if (xmlStrParse.LoadString(strXml)) {
            xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
            VR_String strMsg = xmlStrParse.TransferXMLtoString();
            SendReqToNATP(strMsg);
            return;
        }
        else {
            VR_LOGD("LoadString [%s] error", strXml.c_str());
        }
    } while (0);

    ReplyUpdateHintsActionResultToVR(context, commonProperty, false);
    return;
}

void VR_DialogManger::OnRequestStartVUI(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    const RequestVrCommonProperty& commonProperty = (const RequestVrCommonProperty&)msg;
    do {
        if (!commonProperty.has_reqstartvui()) {
            VR_LOGE("has_reqstartvui is abnormal");
            break;
        }
        if (!commonProperty.reqstartvui().has_domaintypeinfo()) {
            VR_LOGE("has_domaintypeinfo is abnormal");
            break;
        }
        VR_String strXml = commonProperty.reqstartvui().domaintypeinfo();
        if (strXml.empty()) {
            VR_LOGE("domaintypeinfo is empty");
            break;
        }
        ReplyStartVUI(context, commonProperty, true);



        VR_XmlStrParse xmlStrParse;

        if (xmlStrParse.LoadString(strXml)) {
            xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
            VR_String strMsg = xmlStrParse.TransferXMLtoString();
            SendReqToNATP(strMsg);
            return;
        }
        else {
            VR_LOGD("LoadString [%s] error", strXml.c_str());
        }
    } while (0);

    ReplyStartVUI(context, commonProperty, false);
    return;
}

void VR_DialogManger::OnRequestStartVRDiagActionResultToVR(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    const RequestVrCommonProperty& commonProperty = (const RequestVrCommonProperty&)msg;
    do {
        if (!commonProperty.has_reqstartvrdiagactionresulttovr()) {
            VR_LOGE("has_reqstartvrdiagactionresulttovr is abnormal");
            break;
        }
        if (!commonProperty.reqstartvrdiagactionresulttovr().has_actionresult()) {
            VR_LOGE("has_actionresult is abnormal");
            break;
        }
        VR_String strXml = commonProperty.reqstartvrdiagactionresulttovr().actionresult();
        if (strXml.empty()) {
            VR_LOGE("actionresult is empty");
            break;
        }
        ReplyStartVRDiagActionResultToVR(context, commonProperty, true);
        VR_XmlStrParse xmlStrParse;

        if (xmlStrParse.LoadString(strXml)) {
            xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
            VR_String strMsg = xmlStrParse.TransferXMLtoString();
            SendReqToNATP(strMsg);
            return;
        }
        else {
            VR_LOGD("LoadString [%s] error", strXml.c_str());
        }
    } while (0);

    ReplyStartVRDiagActionResultToVR(context, commonProperty, false);
    return;
}

void VR_DialogManger::OnRequestStopVRDiag(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    const RequestVrCommonProperty& commonProperty = (const RequestVrCommonProperty&)msg;
    do {
        if (!commonProperty.has_reqstopvrdiag()) {
            VR_LOGE("has_reqstopvrdiag is abnormal");
            break;
        }
        if (!commonProperty.reqstopvrdiag().has_stopvrdiaginfo()) {
            VR_LOGE("has_stopvrdiaginfo is abnormal");
            break;
        }
        VR_String strXml = commonProperty.reqstopvrdiag().stopvrdiaginfo();
        if (strXml.empty()) {
            VR_LOGE("stopvrdiaginfo is empty");
            break;
        }
        ReplyStopVRDiag(context, commonProperty, true);

        VR_XmlStrParse xmlStrParse;

        if (xmlStrParse.LoadString(strXml)) {
            xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
            VR_String strMsg = xmlStrParse.TransferXMLtoString();
            SendReqToNATP(strMsg);
            return;
        }
        else {
            VR_LOGD("LoadString [%s] error", strXml.c_str());
        }
    } while (0);

    ReplyStopVRDiag(context, commonProperty, false);
    return;
}

void VR_DialogManger::OnRequestStopVRDiagActionResultToVR(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    const RequestVrCommonProperty& commonProperty = (const RequestVrCommonProperty&)msg;
    do {
        if (!commonProperty.has_reqstopvrdiagactionresulttovr()) {
            VR_LOGE("has_reqstopvrdiagactionresulttovr is abnormal");
            break;
        }
        if (!commonProperty.reqstopvrdiagactionresulttovr().has_actionresult()) {
            VR_LOGE("has_actionresult is abnormal");
            break;
        }
        VR_String strXml = commonProperty.reqstopvrdiagactionresulttovr().actionresult();
        if (strXml.empty()) {
            VR_LOGE("actionresult is empty");
            break;
        }
        ReplyStopVRDiagActionResultToVR(context, commonProperty, true);
        VR_XmlStrParse xmlStrParse;

        if (xmlStrParse.LoadString(strXml)) {
            xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
            VR_String strMsg = xmlStrParse.TransferXMLtoString();
            SendReqToNATP(strMsg);
            return;
        }
        else {
            VR_LOGD("LoadString [%s] error", strXml.c_str());
        }
    } while (0);

    ReplyStopVRDiagActionResultToVR(context, commonProperty, false);
    return;
}

void VR_DialogManger::OnRequestVrServiceStatus(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    const RequestVrCommonProperty& commonProperty = (const RequestVrCommonProperty&)msg;

    if (commonProperty.has_reqvrservicestatus()) {
        ReplyVrServiceStatus(context, commonProperty, true);
        NotifyVrServiceStatus();
        NotifyMicrophoneStatusToFC();
    }
    else {
        VR_LOGE("has_reqvrservicestatus is abnormal");
        ReplyVrServiceStatus(context, commonProperty, false);
    }
    return;
}

void VR_DialogManger::OnRequestCommonActionResult(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    const RequestVrCommonProperty& commonProperty = (const RequestVrCommonProperty&)msg;

    if (!commonProperty.has_reqcommonactionresult()) {
        VR_LOGE("has_reqvrservicestatus is abnormal");
        return;
    }

    VR_String strXml = commonProperty.reqcommonactionresult().action();
    VR_XmlStrParse parser;
    bool res = parser.LoadString(strXml);
    if (!res) {
        VR_LOGD("LoadString [%s] error", strXml.c_str());
        return;
    }

// #ifdef VR_DM_TARGET_WUKONG
//     VR_String strType = parser.GetXMLValue(VR_XPATH_TYPE);
//     VR_LOGD("strType [%s]", strType.c_str());
//     VoiceMap<VR_String, emVriAutoProxyType>::const_iterator iter = g_iAutoRouter.find(strType);
//     switch (iter->second) {
//     case VriAutoProxyType_Navi_WukongListData:
//     {
//         m_spNaviMsgHandler->OnRequestVriAutoProxy(strXml);
//         return;
//     }
//     default:
//     {
//         VR_LOGD("type %d in wukong temp do not add function!");
//         break;
//     }
//     }
// #endif

    VR_String content;
    res = parser.GetAttributeValue("//event", "name", content);
    if (res) {
        if (content.compare("setAECStatus") == 0) {
            VR_String engineAECstatus = parser.GetXMLValue("status");
            VR_LOGD("iauto3 setAECStatus [%s]", engineAECstatus.c_str());

            if (atoi(engineAECstatus.c_str())) {
                VR_ConfigureIF::Instance()->setAECEngineStatus(true);
            }
            else {
                VR_ConfigureIF::Instance()->setAECEngineStatus(false);
            }

            m_spEngineIF->SendMessage(strXml);
            return;
        }
        else if (content.compare("setAwakenHints") == 0) {
            VR_LOGD("setAwakeHints success [%s]", strXml.c_str());
            m_spEngineIF->SendMessage(strXml);
            return;
        }
        else if (content.compare("uploadAwakenAudio") == 0) {
            strXml = "<event name = \"deleteAwakenAudio\"><event>";
            VR_LOGD("deleteAwakenAudio success [%s]", strXml.c_str());
            m_spEngineIF->SendMessage(strXml);
            return;
        }
    }

    res = parser.GetNodeText("//notification/type[text()='System_DelFavoriteStock']", content);
    if (res) {
        bool state = m_pVrStockInfoManager->DelFavoriteStockInfo(strXml);

        VR_String stock_id;
        VR_String stock_name;
        parser.GetNodeText("//stock_name", stock_name);
        parser.GetNodeText("//stock_id", stock_id);

        VR_XmlStrParse parsertmp;
        parsertmp.LoadString(VR_STOCK_QUERYFAVORITESTE_RESP_XML);
        parsertmp.AddNode("//params", "stock_id", stock_id);
        parsertmp.AddNode("//params", "stock_name", stock_name);
        if (state) {
            parsertmp.SetNodeText("//state", "OK");
        }
        else {
            parsertmp.SetNodeText("//state", "NG");
        }

        parsertmp.GetXmlString(content);
        VR_LOGD("content: %s", content.c_str());
        std::unique_ptr<VrNotifyCommonProperty> noitfyProperty(VrNotifyCommonProperty::default_instance().New());
        noitfyProperty->set_sender("vr");
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyCommonAction);
        noitfyProperty->mutable_vuicommonaction()->set_action(content);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());
        return;
    }

    res = parser.GetNodeText("//notification/type[text()='System_AddFavoriteStock']", content);
    if (res) {
        bool state = m_pVrStockInfoManager->AddFavoriteStockInfo(strXml);

        VR_String stock_id;
        VR_String stock_name;
        parser.GetNodeText("//stock_name", stock_name);
        parser.GetNodeText("//stock_id", stock_id);

        VR_XmlStrParse parsertmp;
        parsertmp.LoadString(VR_STOCK_QUERYFAVORITESTE_RESP_XML);
        parsertmp.AddNode("//params", "stock_id", stock_id);
        parsertmp.AddNode("//params", "stock_name", stock_name);
        if (state) {
            parsertmp.SetNodeText("//state", "OK");
        }
        else {
            parsertmp.SetNodeText("//state", "NG");
        }

        parsertmp.GetXmlString(content);
        VR_LOGD("content: %s", content.c_str());
        std::unique_ptr<VrNotifyCommonProperty> noitfyProperty(VrNotifyCommonProperty::default_instance().New());
        noitfyProperty->set_sender("vr");
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyCommonAction);
        noitfyProperty->mutable_vuicommonaction()->set_action(content);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());
        return;
    }

    res = parser.GetNodeText("//notification/type[text()='notifyNavigationStatusChanged']", content);
    if (res) {
        VR_String strIsMapOn;
        parser.GetNodeText(VR_NAVI_MAP_STATUS, strIsMapOn);
#ifdef VR_DM_TARGET_MUIFA
        m_spNaviStatusManager->OnMessageNaviStatusChanged(strIsMapOn);
#endif

#ifdef VR_DM_TARGET_WUKONG
        m_spNaviMsgHandler->SetMapStatus(strIsMapOn);
#endif
        return;
    }

    parser.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
    VR_String strMsg = parser.TransferXMLtoString();
    SendReqToNATP(strMsg);

    return;
}

void VR_DialogManger::OnNotifyWeatherScreenChangedToNATP(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const RequestVrCommonProperty& commonProperty = (const RequestVrCommonProperty&)msg;
    if (commonProperty.has_reqweatherinfo()) {
        const VUIReqWeatherInfo& VUIWeatherInfo = commonProperty.reqweatherinfo();
        if (VUIWeatherInfo.has_info()) {
            const NIVUIWeatherInfo& weatherInfo = VUIWeatherInfo.info();
            VR_String strCity = "";
            VR_String strDate = "";
            VR_String strDisplayMode = "";
            VR_String strPromptMode = "";
            VR_String strDisplayType = "";
            if (weatherInfo.has_city()) {
                strCity = weatherInfo.city();
            }
            if (weatherInfo.has_date()) {
                strDate = weatherInfo.date();
            }
            if (weatherInfo.has_diplaymode()) {
                strDisplayMode = weatherInfo.diplaymode();
            }
            if (weatherInfo.has_promptmode()) {
                strPromptMode = weatherInfo.promptmode();
            }
            if (weatherInfo.has_displaytype()) {
                strDisplayType = weatherInfo.displaytype();
            }

            VR_String strXml = VR_WEATHER_NOTIFY_SCREEN_CHANGED_XML;
            VR_XmlStrParse xmlStrParse;
            if (!xmlStrParse.LoadString(strXml)) {
                VR_LOGD("LoadString [%s] error", strXml.c_str());
                return;
            }
            xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
            xmlStrParse.setTextTo(VR_WEATHER_NOTIFY_CITY, strCity);
            xmlStrParse.setTextTo(VR_WEATHER_NOTIFY_DATE, strDate);
            xmlStrParse.setTextTo(VR_WEATHER_NOTIFY_DISP_MODE, strDisplayMode);
            xmlStrParse.setTextTo(VR_WEATHER_NOTIFY_PROMPT_MODE, strPromptMode);
            xmlStrParse.setTextTo(VR_WEATHER_NOTIFY_DISP_TYPE, strDisplayType);
            VR_String strMsg = xmlStrParse.TransferXMLtoString();
            SendReqToNATP(strMsg);
        }
    }
}

void VR_DialogManger::NotifyUpdateWeatherData(const VR_String &strXml)
{
    VR_LOGD_FUNC();

    VR_XmlStrParse xmlStrParse;
    if (!xmlStrParse.LoadString(strXml)) {
        VR_LOGE("load_string error");
        return;
    }
    VR_String strScenario = xmlStrParse.GetXMLValue(VR_WEATHER_PARAMS_SCENARIO);
    VR_String strCity = xmlStrParse.GetXMLValue(VR_WEATHER_PARAMS_HMI_CITY);
    VR_String strDate = xmlStrParse.GetXMLValue(VR_WEATHER_PARAMS_HMI_DATE);
    VR_String strDispMode = xmlStrParse.GetXMLValue(VR_WEATHER_PARAMS_HMI_DISPLAY_MODE);
    VR_String strData = xmlStrParse.GetXMLValue(VR_WEATHER_PARAMS_HMI_DATA);
    std::unique_ptr<VrNotifyCommonProperty> noitfyProperty(VrNotifyCommonProperty::default_instance().New());
    noitfyProperty->set_sender("NATP");
    noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyUpdateWeatherData);
    NIVUIUpdateWeatherData* weatherdata = noitfyProperty->mutable_updateweatherdata()->mutable_data();
    weatherdata->set_city(strCity);
    weatherdata->set_date(strDate);
    weatherdata->set_diplaymode(strDispMode);
    weatherdata->set_jsondata(strData);
    weatherdata->set_displaytype(strScenario);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());
}

void VR_DialogManger::SendReqToNATP(VR_String &strXml)
{
    VR_LOGD("strXml=[%s]", strXml.c_str());
    std::unique_ptr<VrNotifyiAutoProxy> protoMsg(VrNotifyiAutoProxy::default_instance().New());
    protoMsg->set_sender("vr_s");
    protoMsg->set_funccode(VrNotifyiAutoProxy_VrFunc_NotifyRecogMessage);
    protoMsg->mutable_recogmsg()->set_xml_out(strXml);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFIAUTOPROXY, protoMsg.release());
}

void VR_DialogManger::ReplyDomainTypeHints(const EV_EventContext &context, const VR_ProtoMessageBase &msg, bool bRet)
{
    VR_LOGD("bRet: %s", bRet ? "true" : "false");
    const RequestVrCommonProperty& commonProperty = (const RequestVrCommonProperty&)msg;

    std::unique_ptr<RespVrCommonPropertyResult> replyMsg(RespVrCommonPropertyResult::default_instance().New());
    replyMsg->set_sender(commonProperty.sender());
    replyMsg->set_taskid(commonProperty.taskid());
    VUIReqDomainTypeHintsResult *pReq = replyMsg->mutable_reqdomaintypehintsresult();
    if (NULL != pReq) {
        pReq->set_result(bRet);
        m_spEventSenderIF->Reply(context, replyMsg.release());
    }
    else {
        VR_LOGE("mutable_reqdomaintypehintsresult error");
    }
}

void VR_DialogManger::ReplyUpdateHintsActionResultToVR(const EV_EventContext &context, const VR_ProtoMessageBase &msg, bool bRet)
{
    VR_LOGD("bRet: %s", bRet ? "true" : "false");
    const RequestVrCommonProperty& commonProperty = (const RequestVrCommonProperty&)msg;

    std::unique_ptr<RespVrCommonPropertyResult> replyMsg(RespVrCommonPropertyResult::default_instance().New());
    replyMsg->set_sender(commonProperty.sender());
    replyMsg->set_taskid(commonProperty.taskid());
    VUIReqUpdateHintsActionResultToVRResult *pReq = replyMsg->mutable_requpdatehintsactionresulttovrresult();
    if (NULL != pReq) {
        pReq->set_result(bRet);
        m_spEventSenderIF->Reply(context, replyMsg.release());
    }
    else {
        VR_LOGE("mutable_requpdatehintsactionresulttovrresult error");
    }
}

void VR_DialogManger::ReplyStartVUI(const EV_EventContext &context, const VR_ProtoMessageBase &msg, bool bRet)
{
    VR_LOGD("bRet: %s", bRet ? "true" : "false");
    const RequestVrCommonProperty& commonProperty = (const RequestVrCommonProperty&)msg;

    std::unique_ptr<RespVrCommonPropertyResult> replyMsg(RespVrCommonPropertyResult::default_instance().New());
    replyMsg->set_sender(commonProperty.sender());
    replyMsg->set_taskid(commonProperty.taskid());
    VUIReqStartVUIResult *pReq = replyMsg->mutable_reqstartvuiresult();
    if (NULL != pReq) {
        pReq->set_result(bRet);
        m_spEventSenderIF->Reply(context, replyMsg.release());
    }
    else {
        VR_LOGE("mutable_reqstartvuiresult error");
    }
}

void VR_DialogManger::ReplyStartVRDiagActionResultToVR(const EV_EventContext &context, const VR_ProtoMessageBase &msg, bool bRet)
{
    VR_LOGD("bRet: %s", bRet ? "true" : "false");
    const RequestVrCommonProperty& commonProperty = (const RequestVrCommonProperty&)msg;

    std::unique_ptr<RespVrCommonPropertyResult> replyMsg(RespVrCommonPropertyResult::default_instance().New());
    replyMsg->set_sender(commonProperty.sender());
    replyMsg->set_taskid(commonProperty.taskid());
    VUIReqStartVRDiagActionResultToVRResult *pReq = replyMsg->mutable_reqstartvrdiagactionresulttovrresult();
    if (NULL != pReq) {
        pReq->set_result(bRet);
        m_spEventSenderIF->Reply(context, replyMsg.release());
    }
    else {
        VR_LOGE("mutable_reqstartvrdiagactionresulttovrresult error");
    }
}

void VR_DialogManger::ReplyStopVRDiag(const EV_EventContext &context, const VR_ProtoMessageBase &msg, bool bRet)
{
    VR_LOGD("bRet: %s", bRet ? "true" : "false");
    const RequestVrCommonProperty& commonProperty = (const RequestVrCommonProperty&)msg;

    std::unique_ptr<RespVrCommonPropertyResult> replyMsg(RespVrCommonPropertyResult::default_instance().New());
    replyMsg->set_sender(commonProperty.sender());
    replyMsg->set_taskid(commonProperty.taskid());
    VUIReqStopVRDiagResult *pReq = replyMsg->mutable_reqstopvrdiagresult();
    if (NULL != pReq) {
        pReq->set_result(bRet);
        m_spEventSenderIF->Reply(context, replyMsg.release());
    }
    else {
        VR_LOGE("mutable_reqstopvrdiagresult error");
    }
}

void VR_DialogManger::ReplyStopVRDiagActionResultToVR(const EV_EventContext &context, const VR_ProtoMessageBase &msg, bool bRet)
{
    VR_LOGD("bRet: %s", bRet ? "true" : "false");
    const RequestVrCommonProperty& commonProperty = (const RequestVrCommonProperty&)msg;

    std::unique_ptr<RespVrCommonPropertyResult> replyMsg(RespVrCommonPropertyResult::default_instance().New());
    replyMsg->set_sender(commonProperty.sender());
    replyMsg->set_taskid(commonProperty.taskid());
    VUIReqStopVRDiagActionResultToVRResult *pReq = replyMsg->mutable_reqstopvrdiagactionresulttovrresult();
    if (NULL != pReq) {
        pReq->set_result(bRet);
        m_spEventSenderIF->Reply(context, replyMsg.release());
    }
    else {
        VR_LOGE("mutable_reqstopvrdiagactionresulttovrresult error");
    }
}

void VR_DialogManger::ReplyVrServiceStatus(const EV_EventContext &context, const VR_ProtoMessageBase &msg, bool bRet)
{
    VR_LOGD_FUNC();
    const RequestVrCommonProperty& commonProperty = (const RequestVrCommonProperty&)msg;

    std::unique_ptr<RespVrCommonPropertyResult> replyMsg(RespVrCommonPropertyResult::default_instance().New());
    replyMsg->set_sender(commonProperty.sender());
    replyMsg->set_taskid(commonProperty.taskid());

    VUIReqVrServiceStatusResult *pReq = replyMsg->mutable_reqvrservicestatusresult();
    if (NULL != pReq) {
        pReq->set_result(bRet);
        m_spEventSenderIF->Reply(context, replyMsg.release());
    }
    else {
        VR_LOGE("mutable_reqvrservicestatusresult error");
    }
}

VR_String VR_DialogManger::GetVUIWorkRootPath()
{
    // 1.if not exist vui dir and create it
    BL_Dir vuiDir(BL_FILE_PREFIX_RW);
    bool result = (CL_TRUE == vuiDir.MakeDir(VR_VUI_WORK_FILE_RWDISK_FOLDERNAME,
                                             CL_FALSE));
    if (!result) {
        VR_LOGD("create %s directory failed", VR_VUI_WORK_FILE_RWDISK_FOLDERNAME);
        return("");
    }

    // 2.load work vui path
    BL_String vuiFilePath;
    BL_File avcLanFile(BL_FILE_PREFIX_RW);
    result = (CL_TRUE == avcLanFile.GetLogicalPath(VR_VUI_WORK_FILE_RWDISK_FOLDERNAME,
                                                   vuiFilePath));
    if (!result) {
        VR_LOGD("get %s absolute path failed", VR_VUI_WORK_FILE_RWDISK_FOLDERNAME);
        return("");
    }

    VR_LOGD("get vui root path:%s", vuiFilePath.GetString());
    return(vuiFilePath.GetString());
}

inline VR_UINT32 VR_DialogManger::GetActionId()
{
    VR_IdGenerator nId;
    return nId.GetID();
}

bool VR_DialogManger::IsSourceSupportResume()
{
    VR_LOGD_FUNC();
    struct MediaInfo_CurSrc curSrc;
    memset(&curSrc, 0, sizeof(curSrc));

    bool res = NMMediaInfoHelper::load(MediaInfoKey_CurSrc, &curSrc, sizeof(curSrc));
    if (!res) {
        VR_LOGD("get current source error");
        return false;
    }

    if (MediaInfo_SrcID_BTA == curSrc.curSrc
        || MediaInfo_SrcID_USB1 == curSrc.curSrc
        || MediaInfo_SrcID_USB2 == curSrc.curSrc
        || MediaInfo_SrcID_PlayZone == curSrc.curSrc
        || MediaInfo_SrcID_MediaPlayer == curSrc.curSrc
        || MediaInfo_SrcID_MediaShare == curSrc.curSrc) {
        return true;
    }
    return false;
}

void VR_DialogManger::ReplyMusicPlayError(VR_String &strId)
{
    VR_LOGD_FUNC();
    VR_XmlStrParse parser;
    parser.LoadString(VR_MEDIA_PLAY_LOCAL_MUSIC_PESP_XML);

    parser.SetNodeText("//action_id", strId);
    parser.SetNodeText("//action_result/id", VR_IdGenerator().GetID());
    parser.SetNodeText("//error_no", VR_MEDIA_RESULT_EMPTY);

    VR_String xml;
    parser.GetXmlString(xml);
    SendReqToNATP(xml);
}

void VR_DialogManger::ReplyAddUserError(VR_String &strId)
{
    VR_LOGD_FUNC();

    VR_XmlStrParse parser;
    parser.LoadString(VR_ADD_USER_ACION_RESULT);

    parser.SetNodeText("//action_id", strId);
    parser.SetNodeText("//action_result/id", VR_IdGenerator().GetID());
    parser.SetNodeText("//error_no", "VB_RESULT_ERROR");

    VR_String xml;
    parser.GetXmlString(xml);
    SendReqToNATP(xml);
}

bool VR_DialogManger::CheckDiagMode()
{
#ifdef HAVE_NUTSHELL_OS
    nutshell::NCEnvironment  env;
    nutshell::NCString   ncStrValue;
    nutshell::NCString   ncStrKey("sys/diagmode");
    env.get(ncStrKey, ncStrValue);
    std::string strMode = ncStrValue.getString();
    VR_LOGD("strMode = [%s]", strMode.c_str());

    if ("unknown" == strMode || "off" == strMode) {
        return false;
    }
    else if ("factorymode" == strMode || "servicemode" == strMode || "developmode" == strMode) {
        return true;
    }
    return false;
#else
    return true;
#endif
}

void  VR_DialogManger::DiagCopyFinish()
{
    VR_LOGD_FUNC();

    std::unique_ptr<VrLoopBack>  loopMessage(VrLoopBack::default_instance().New());
    loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
    VrEngineControlMessage *controlMessage = loopMessage->mutable_m_controlmessage();
    controlMessage->set_type(DMTaskMessageType_DiagEventResult);
    controlMessage->mutable_diageventresult()->set_eventtype(event_copy_finish);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
}

void  VR_DialogManger::DiagDeleteFinish()
{
    VR_LOGD_FUNC();

    std::unique_ptr<VrLoopBack>  loopMessage(VrLoopBack::default_instance().New());
    loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
    VrEngineControlMessage *controlMessage = loopMessage->mutable_m_controlmessage();
    controlMessage->set_type(DMTaskMessageType_DiagEventResult);
    controlMessage->mutable_diageventresult()->set_eventtype(event_delete_finish);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
}

void VR_DialogManger::RemoveVBTFlagFile()
{
#ifdef HAVE_NUTSHELL_OS
    VR_ConfigureIF::VR_EngineType bEngType = VR_ConfigureIF::Instance()->getEngineType();
    if (VR_ConfigureIF::VR_EngineType::VBT != bEngType) {
        return;
    }
    NCDir dir;
    NCFile file;

    // create dir
    NCString cVbtLogDir = "/data/vr/vbtflg/";
    if (!dir.exists(cVbtLogDir)) {
        dir.makePath(cVbtLogDir);
        return;
    }

    NCString startMode("");
    NCPowerStatus powerStatus;
    powerStatus.getStartMode(startMode);
    if ("B-On" != startMode) {
        VR_LOGD("ISNOT B-OFF POWER DOWN!!!!!");
        return;
    }

    // remove on.flg
    NCString cVbtLogOnFlg = "/data/vr/vbtflg/on.flg";
    if (file.exists(cVbtLogOnFlg)) {
        file.remove(cVbtLogOnFlg);
    }

    // remove off.flg
    NCString cVbtLogOffFlg = "/data/vr/vbtflg/off.flg";
    if (file.exists(cVbtLogOffFlg)) {
        file.remove(cVbtLogOffFlg);
    }
#endif
}

void VR_DialogManger::ConfigureVRDESetting(VR_Settings &rstSetting, std::string& rstrVdataVersion)
{
    VR_LOGD_FUNC();

    BL_DataValue    dataValue;
    BL_DataStorage  dataStorage;
    std::string     strVDataVersion;
    BOOL  bResult  =  dataStorage.GetValue(BL_DATA_KEY_VR_VRVERSION, dataValue);
    if (bResult) {
        char  szData[VR_DATA_VERSION_SIZE + 1] = { 0 };
        memcpy(szData, dataValue.GetData(), dataValue.GetSize());
        strVDataVersion = szData;
    }
    VR_LOGD("get value result: [%d], strVDataVersion: [%s]", bResult, strVDataVersion.c_str());

    std::string strVersion;
    BL_File cVDataPathFile(BL_FILE_PREFIX_VDATA);
    char* pFileBuffer(NULL);

    do {
        CL_BOOL bFileExist =  cVDataPathFile.FileExists("../VDATA_ERROR.FLG");
        VR_LOGD("vdata error flag file is: [%d]", bFileExist);
        if (bFileExist) {
            rstSetting.enVDStatus = enVDataStatus_Error;
     //       m_bVDataUpdateError = true;
            return;
        }

        CL_BOOL bOpenResult = cVDataPathFile.Open("version.xml", CL_AbstractFile::OpenMode::kReadOnly);
        if (!bOpenResult) {
            VR_LOGD("open version xml file failed.");
            break;
        }

        CL_AbstractFile::FileSize uFileSize = cVDataPathFile.GetFileSize();
        VR_LOGD("get version file size = [%d].", uFileSize);
        if (0 == uFileSize) {
            break;
        }

        pFileBuffer = VR_new char[uFileSize + 1];
        if (NULL == pFileBuffer) {
            VR_LOGD("malloc vresion xml failed.");
            break;
        }
        memset(pFileBuffer, 0x00, sizeof(char) * (uFileSize + 1));

        CL_BOOL cReadResult = cVDataPathFile.Read(pFileBuffer, uFileSize);
        if (!cReadResult) {
            VR_LOGD("read vrsion file failed.");
            break;
        }

        pugi::xml_document doc;
        pugi::xml_parse_result xmlResult  = doc.load(pFileBuffer);
        if (!xmlResult) {
            VR_LOGD("load version xmlbuffer failed.");
            break;
        }

        if (doc.empty()) {
            VR_LOGD("doc xml is empty.");
            break;
        }

        strVersion = doc.select_node("//version").node().child_value();
        VR_LOGD("strVersion = [%s]", strVersion.c_str());
    } while (0);

    if (cVDataPathFile.IsOpen()) {
        cVDataPathFile.Close();
    }

    if (NULL != pFileBuffer) {
        delete[] pFileBuffer;
        pFileBuffer = NULL;
    }

    if (strVDataVersion != strVersion) {
        rstSetting.enVDStatus = enVDataStatus_VersionChange;
    }
    else {
        rstSetting.enVDStatus = enVDataStatus_VersionNOChange;
    }
    rstrVdataVersion = strVersion;
}

void VR_DialogManger::UpdateErrorONS()
{
    VR_LOGD_FUNC();
}


void   VR_DialogManger::QueryNaviPathPointList()
{
    VR_LOGD_FUNC();
}


void VR_DialogManger::OnReqestVrSettingState(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    // dm send to de for change language.
    VR_LOGD_FUNC();
    const VrRequestSettingProperty& settingProperty = (const VrRequestSettingProperty&)msg;
    if (!settingProperty.has_type()) {
        VR_LOGD("OnRequestVRSettingState: has_type = [%d]", settingProperty.has_type());
        return;
    }

    static VR_String xmlResult = "<event>"
                                 "</event>";
    static VR_String xmlStart = "";
    VrDMVRSettingType type = settingProperty.type();

    switch (type) {
    case VrDMVRSettingType_Language:
        {
            const VrRequestChangeLanguage& ChangeLanguage = settingProperty.changelanguage();
            if (!ChangeLanguage.has_language()) {
                VR_LOGD("has_language = [%d]", ChangeLanguage.has_language());
                return;
            }
            const std::string& strLanguage = ChangeLanguage.language();
            xmlStart = "<event name=\"changeLanguage\">"
                            "<language>%1%</language>"
                       "</event>";

            xmlResult = (boost::format(xmlStart) % strLanguage).str();

            break;
        }
    case VrDMVRSettingType_Contry:
        {
            const VrRequestContry& SettingCountry = settingProperty.contry();
            if (!SettingCountry.has_contry()) {
                VR_LOGD("has_language = [%d]", SettingCountry.has_contry());
                return;
            }

            const std::string& strContry = SettingCountry.contry();
            xmlStart = "<event name=\"changeSettings\">"
                            "<param name=\"%1%\" value=\"%2%\"/>"
                       "</event>";

            xmlResult = (boost::format(xmlStart) % "country" % strContry).str();
            break;
        }
    case VrDMVRSettingType_Product:
        {
            const VrRequestProduct& SettingProduct = settingProperty.product();
            if (!SettingProduct.has_product()) {
                VR_LOGD("has_product = [%d]", SettingProduct.has_product());
                return;
            }

            const std::string& strProduct = SettingProduct.product();
            xmlStart = "<event name=\"changeSettings\">"
                              "<param name=\"%1%\" value=\"%2%\"/>"
                       "</event>";

            xmlResult = (boost::format(xmlStart) % "product" % strProduct).str();
            break;
        }
    case VrDMVRSettingType_WebSearchEngine:
        {
            if (!settingProperty.has_websearcheng()) {
                return;
            }
            return;
        }
    case VrDMVRSettingType_PromptLevel:
        {
            const VrRequestPromptLevel& PromptLevel = settingProperty.promptlevel();
            if (!PromptLevel.has_promptlevel()) {
                VR_LOGD("has_promptlevel = [%d]", PromptLevel.has_promptlevel());
                return;
            }

            int nPromptLevel = PromptLevel.promptlevel();
            std::string strValue("FULL");
            switch (nPromptLevel) {
            case VR_PROMPTLEVEL_OFF:
                {
                   strValue = "OFF";
                    break;
                }
            case VR_PROMPTLEVEL_ON:
                {
                    strValue = "FULL";
                    break;
                }
            case VR_PROMPTLEVEL_LOW:
                {
                    strValue = "SIMPLE";
                    break;
                }
            case VR_PROMPTLEVEL_HIGH:
                {
                    strValue = "FULL";
                    break;
                }
            default:
                break;
            }
            xmlStart = "<event name=\"changeSettings\">"
                           "<param name=\"%1%\" value=\"%2%\"/>"
                       "</event>";

            xmlResult = (boost::format(xmlStart) % "promptLevel" % strValue).str();
            break;
        }
    case VrDMVRSettingType_SpeakOverPrompt:
        {
            const VrRequestSpeakOverPrompt& SpeakOverPrompt = settingProperty.speakoverprompt();
            if (!SpeakOverPrompt.has_speakoverprompt()) {
                VR_LOGD("has_speakoverprompt() = [%d]", SpeakOverPrompt.has_speakoverprompt());
                return;
            }

            bool bOverPrompt = SpeakOverPrompt.speakoverprompt();
            std::string strValue("OFF");
            if (bOverPrompt) {
                strValue = "ON";
            }
            xmlStart = "<event name=\"changeSettings\">"
                              "<param name=\"%1%\" value=\"%2%\"/>"
                      "</event>";

            xmlResult = (boost::format(xmlStart) % "bargein" % strValue).str();
            break;
        }
    case VrDMVRSettingType_SpeakOverBeep:
        {
            break;
        }
    case VrDMVRSettingType_DisplayComfirm:
        {
            const VrRequestDisplayComfirm& DisplayComfirm = settingProperty.displaycomfirm();
            if (!DisplayComfirm.has_displaycomfirm()) {
                VR_LOGD("has_displaycomfirm() = [%d]", DisplayComfirm.has_displaycomfirm());
                return;
            }

            bool bDisplayComfirm = DisplayComfirm.displaycomfirm();
            std::string strValue("OFF");
            if (bDisplayComfirm) {
                strValue = "ON";
            }
            xmlStart = "<event name=\"changeSettings\">"
                           "<param name=\"%1%\" value=\"%2%\"/>"
                       "</event>";

            xmlResult = (boost::format(xmlStart) % "displayConfirm" % strValue).str();
            break;
        }
    case VrDMVRSettingType_NotifyAll:
        {
            const VrRequestNotifyAll&  notifyAll = settingProperty.notifyall();
            if (!notifyAll.has_notifyall()) {
                VR_LOGD("has_notifyAll() = [%d]", notifyAll.has_notifyall());
                return;
            }

            xmlStart = "<event name=\"changeSettings\">"
                            "<param name=\"%1%\" value=\"%2%\"/>"
                      "</event>";

            xmlResult = (boost::format(xmlStart) % "VRSetting" % "OK").str();
            break;
        }
    case VrDMVRSettingType_FullUpdate:
        {
            const VrDataFullUpdata& rsDataFullUpdate = settingProperty.full_update_status();
            if (!rsDataFullUpdate.has_fullupdatestatus()) {
                VR_LOGD("has_full_update_status = [%d]", rsDataFullUpdate.has_fullupdatestatus());
                return;
            }

            const std::string& strFullUpdateStatus = rsDataFullUpdate.fullupdatestatus(); // off / navifulldata / finished
            xmlStart = "<event name=\"fullupdateNotify\">"
                            "<status>%1%</status>"
                       "</event>";

            xmlResult = (boost::format(xmlStart) % strFullUpdateStatus).str();
            break;
        }
    case VrDMVRSettingType_StartedNotify:
        {
            m_spVoiceRecogStatus->OnRecevieMessage(msg);
            return;
        }
    case VrDMVRSettingType_VrLanguage:
        {
            NotifyLanguageStatusToVR();
            return;
        }
    case VrDMVRSettingType_VrAwakenWord:
        {
            NotifyAwakenWord();
            return;
        }
    case VrDMVRSettingType_VrRouteOption:
        {
            m_spNaviStatusManager->NotifyRouteOptionChanged();
            return;
        }
    case VrDMVRSettingType_VrWuwSwitch:
        {
            NotifyWuwSwitch();
            return;
        }
    case VrDMVRSettingType_VehicleIG:
    {
        std::unique_ptr<VrNotifyCommonProperty> noitfyProperty(VrNotifyCommonProperty::default_instance().New());
        noitfyProperty->set_sender("alexa");
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyAlexaVehicleLowFuel);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());
        break;
    }
    default:
        break;
    }
    VR_LOGI("SendMessage: [%s]", xmlResult.c_str());
    m_spEngineIF->SendMessage(xmlResult);
}




/* EOF */
