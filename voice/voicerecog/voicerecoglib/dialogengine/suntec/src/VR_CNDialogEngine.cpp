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

#include "VR_DEDataManager.h"
#include "VR_CNDialogEngine.h"
#include <boost/bind.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include "uscxml/messages/SendRequest.h"
#include <chrono>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <math.h>
#include <iomanip>
#include <sstream>
#include "Vr_Asr_Audio_In.h"
#include "VC_WavFile.h"
#include <ncore/NCStartPerformance.h>
#include "VR_Configure.h"
#include "VR_DEMessageBuilder.h"
#include "VR_DataAccessor.h"
#include "VR_DECommon.h"
#include "VR_DataAccessorManagerCN.h"
// #include "VR_AudioInWriter.h"
// #include "VR_AudioInLocalWriter.h"


using namespace nutshell;
using namespace std;
using namespace N_Vr;
using namespace N_Asr;

#define DEG_TO_NAVI    (0x10000 / 360.0)

VoiceMap<std::string, std::string>::type asrLanguage_cn = boost::assign::map_list_of
        (VR_LANGUAGE_EN_US, "english")
         (VR_LANGUAGE_ZH_HK, "cantonese")
        (VR_LANGUAGE_ZH_CN, "mandarin");

VoiceMap<std::string, std::string>::type stateClimateMap = boost::assign::map_list_of
        ("CLIMATE_FRONT_SCREEN_ACTIVE", "false")
        ("CLIMATE_FRONT_SEAT_SCREEN_ACTIVE", "false")
        ("CLIMATE_REAR_SCREEN_ACTIVE", "false")
        ("CLIMATE_REAR_SEAT_SCREEN_ACTIVE", "false")
        ("CLIMATE_SEAT_OPERATION_SCREEN_ACTIVE", "false")
        ("CLIMATE_STEERING_SCREEN_ACTIVE", "false")
        ("CLIMATE_FRONT_SEAT_VEN_SCREEN_ACTIVE", "false")
        ("CLIMATE_REAR_SEAT_VEN_SCREEN_ACTIVE", "false")
        ("CLIMATE_LEXUS_CONCIERGE_SCREEN_ACTIVE", "false")
        ("CLIMATE_FANSPEED_MAX", "5")
        ("CLIMATE_BASIC_ACTIVE", "false")
        ("CLIMATE_CONCIERGE_ACTIVE", "false")
        ("CLIMATE_WIPERDEICE_ACTIVE", "false")
        ("CLIMATE_REARAIRCON_ACTIVE", "false")
        ("CLIMATE_DUALMODE_ACTIVE", "false");

VoiceMap<std::string, std::string>::type cmnIdToKeyMap = boost::assign::map_list_of
        ("CMN7018", "CLIMATE_FRONT_SCREEN_ACTIVE")
        ("CMN7020", "CLIMATE_FRONT_SEAT_SCREEN_ACTIVE")
        ("CMN7019", "CLIMATE_REAR_SCREEN_ACTIVE")
        ("CMN7021", "CLIMATE_REAR_SEAT_SCREEN_ACTIVE")
        ("CMN7023", "CLIMATE_SEAT_OPERATION_SCREEN_ACTIVE")
        ("CMN7035", "CLIMATE_STEERING_SCREEN_ACTIVE")
        ("CMN7034", "CLIMATE_FRONT_SEAT_VEN_SCREEN_ACTIVE")
        ("CMN7036", "CLIMATE_REAR_SEAT_VEN_SCREEN_ACTIVE")
        ("CMN7022", "CLIMATE_LEXUS_CONCIERGE_SCREEN_ACTIVE")
        ("CMN7", "CLIMATE_BASIC_ACTIVE")
        ("CMN0005", "CLIMATE_BASIC_ACTIVE")
        ("CMN7033", "CLIMATE_CONCIERGE_ACTIVE")
        ("CMN7007", "CLIMATE_WIPERDEICE_ACTIVE")
        ("CMN7008", "CLIMATE_WIPERDEICE_ACTIVE")
        ("CMN7009", "CLIMATE_REARAIRCON_ACTIVE")
        ("CMN7010", "CLIMATE_REARAIRCON_ACTIVE")
        ("CMN7005", "CLIMATE_DUALMODE_ACTIVE")
        ("CMN7006", "CLIMATE_DUALMODE_ACTIVE");

VoiceVector<std::string>::type limitCMDIDOnLocalRecogVector = boost::assign::list_of
        ("CMN2102")
        ("CMN2105")
        ("CMN2107")
        ("CMN2108")
        ("CMN2109")
        ("CMN0101")
        ("CMN8101")
        ("CMN8102")
        ("CMN8103")
        ("CMN8104")
        ("CMN8105");

VoiceVector<std::string>::type centerOnlyCMDRecogVector = boost::assign::list_of
        ("CMN2105")
        ("CMN2108")
        ("CMN0101")
        ("CMN8101")
        ("CMN8102")
        ("CMN8103")
        ("CMN8104")
        ("CMN8105");

VoiceVector<std::string>::type addressCMDIDVector = boost::assign::list_of
        ("CMN2003")
        ("CMN2031")
        ("CMN2032")
        ("CMN2050")
        ("CMN2033")
        ("CMN2034");

VoiceVector<std::string>::type poiCMDIDVector = boost::assign::list_of
        ("CMN2006")
        ("CMN2008")
        ("CMN2101")
        ("CMN2102")
        ("CMN2103")
        ("CMN2104")
        ("CMN2105")
        ("CMN2106")
        ("CMN2107")
        ("CMN2108")
        ("CMN2109")
        ("CMN2025")
        ("CMN2026");

VoiceVector<std::string>::type informationCMDIDVector = boost::assign::list_of
        ("CMN8101")
        ("CMN8102")
        ("CMN8103")
        ("CMN8104")
        ("CMN8105");

VoiceVector<std::string>::type climateMaxMinCMDIDVector = boost::assign::list_of
        ("CMN7024")
        ("CMN7025")
        ("CMN7026")
        ("CMN7027");

VoiceVector<std::string>::type communicationErrorVector = boost::assign::list_of
        ("CommunicationIsLocked")
        ("ServiceIsNotInContract")
        ("ServiceIsCurrentlyStopped")
        ("CommunicationIsNotPossible")
        ("PreparingForCommunication")
        ("BluetoothMustBeSetUp")
        ("APhoneMustBeSelected")
        ("MobilePhoneCommunicationNoSetUp")
        ("NoCommunicationsConnection")
        ("NeedToSetInitializeCommunication")
        ("WifiIsNotConnected")
        ("SystemError");

VR_CNDialogEngine::VR_CNDialogEngine(VR_ConfigureIF* pConfig)
    : _dataProcessor(pConfig->getDataPath())
    , _continueAsr(false)
    , _interrputRecv(false)
    , _asrResultId(0)
    , _asrIsRunning(false)
    , _changeLanguage(false)
    , _initPersonData(false)
    , _fullUpdateNavi(false)
    , _languageId("")
    , _needBargein(false)
    , _resourceState(ResourceState::GRAMMAR_NOT_READY)
    , _sessionState(SESSION_IDLE)
    , _lastEndBeepSeqId(0)
    , _lastStartBeepSeqId(0)
    , _lastTtsSeqId(0)
    , _lastStopBeepSeqId(0)
    , _lastStopTtsSeqId(0)
    , _bMapDataPreparing(false)
    , m_iVehiclePosLong(0xFFFFFFFF)
    , m_iVehiclePosLat(0xFFFFFFFF)
    , m_strAsrMode("")
    , m_strCommunicationStatus("")
    , m_bCommunicationStatusFlag(false)
    , _asrReturnBeepPlayed(false)
    , _audioSessionState(false)
    , m_hasGotCenterVRUrl(false)
 //   , m_spCommuMediation(VR_new VR_CommuMediation())
    , m_asrInitStateFirstTime(INIT_NONE)
    , m_agentLoadStateFirstTime(LOAD_NONE)
    , m_receiveAsrResult(false)
    , m_DMStarted(false)
    , m_bVDataError(false)
    , m_isCanceledByDE(false)
    , m_isAsrSpeechEndRecv(false)
    , m_isIpod(false)
    , m_lastReturnBeep(0)
    , _continueAsrByReturnBeep(false)
    , m_routeExistStatus("false")
{
    VR_LOGD_FUNC();
    _interManager = NULL; // new in start
    _thread = NULL; // new in start
    _actionListener = NULL; // receive in start
    _isRunning = false;
    _asrEngine = NULL;
    _addressForAsr.s_bAddress = false;
    m_pConfigureIF = pConfig;
    m_pDECommonIF = VR_new VR_DECommon();
    _dataAccessorManager = NULL;
    _deDataManager.reset(VR_new VR_DEDataManager(m_pConfigureIF->getDataPath()));
    m_pDEMessageBuilder = VR_new VR_DEMessageBuilder(m_pDECommonIF);
    m_pIntentionParser = VR_new VR_IntentionParser(m_pDECommonIF);

 //   VR_AudioInWriter* audioInWriter;
    if (m_pConfigureIF->getInnerAvcLanConnectStatus()) {
 //       audioInWriter = NULL;
    }
    else {
 //       audioInWriter = VR_new VR_AudioInLocalWriter();
    }

    _audioInSource = boost::shared_ptr<VR_AudioInSource>(VR_new VR_AudioInSource());
 //   _audioInSource->setAudioInWriter(audioInWriter);

}

bool
VR_CNDialogEngine::Initialize(VR_DialogEngineListener* listerner, const VR_Settings &settings)
{
    VR_LOGD_FUNC();
    dumpTestcaseSwitch(true);
    tthread::lock_guard<tthread::recursive_mutex> lock(_mutex);
    if (NULL == _interManager) {
        _interManager = VR_new VR_InterpreterManager(this, this, m_pDECommonIF);
    }

    m_pDECommonIF->init(m_pConfigureIF);

    if (NULL == _dataAccessorManager) {
        _dataAccessorManager = VR_new VR_DataAccessorManagerCN(this, m_pDECommonIF, m_pConfigureIF);
    }

    {
        _actionListener = listerner;
        m_voiceTagNotifyType = C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_None;
        _internalServiceQueue.clear();
        _stateSatck = VoiceStack<BackStateInfo>::type(); // empty
        _currentAgent = "";
        _lastAgent = "";
        _currentIntention = std::pair<std::string, std::string>();
        _backStateName = ""; // save the state name in backing, it is also the mark for transition when changeAgent
        _resendEvent = false;
        _forward_back = FORWARD;
        _seqId = 0;
        _grammarStr = "";
        _currentEvent = uscxml::Event();
        _continueAsr = false;
        boost::function<void(const std::string &)> callback = boost::bind(&VR_CNDialogEngine::onStateUpdate, this, _1);
        _dataAccessorManager->setUpdateStateCallback(callback);
        boost::function<void(const std::string &)> notifyCallback = boost::bind(&VR_CNDialogEngine::requestAction, this, _1);
        _dataAccessorManager->setNotifyCallback(notifyCallback);

        _deDataManager->initData(m_pDECommonIF->getVRLanguage());
        initPhoneTypeName();
        _dataProcessor.initData(m_pDECommonIF->getVRLanguage());
        loadHandleEventFuncMap();

        _songDisableSceneFlag = false;
        _bDisableTopmenuSong = false;
        std::string strTagnamesConfPath = m_pConfigureIF->getAsrDataPath();
        readTagDataConf(strTagnamesConfPath + "tagname_config.json");
    }

    // check VDATA status
    switch (settings.enVDStatus) {
    case enVDataStatus_VersionChange:
        {
            C_Engine::Clear_User_Data();
        }
        break;
    case enVDataStatus_Error:
        {
            C_Engine::Clear_User_Data();
            m_bVDataError = true;
        }
        break;
    default:
        break;
    }

    RETURN(true);
}

void VR_CNDialogEngine::UnInitialize()
{
    VR_LOGD_FUNC();
    Stop(); // delete thread and interpreter
    if (m_pIntentionParser != NULL) {
        delete m_pIntentionParser;
        m_pIntentionParser = NULL;
    }
    if (m_pDECommonIF != NULL) {
        delete m_pDECommonIF;
        m_pDECommonIF = NULL;
    }
    if (m_pDEMessageBuilder != NULL) {
        delete m_pDEMessageBuilder;
        m_pDEMessageBuilder = NULL;
    }
    if (_interManager != NULL) {
        delete _interManager;
        _interManager = NULL;
    }
    if (_asrEngine != NULL) {
        // fix bug for iflytek's grave when acc off
        N_Vr::N_Asr::C_Request_Param_Set accOffParam;
        accOffParam.m_e_Param_Type = E_Param_Type_Acc_Off;
        accOffParam.m_i_Value = 1;
        _asrEngine->Param_Set(accOffParam);
        VR_LOGD("acc off param setted to iflytek");
        delete _asrEngine;
        _asrEngine = NULL;
    }
    if (_dataAccessorManager != NULL) {
        delete _dataAccessorManager;
        _dataAccessorManager = NULL;
    }
    m_bVDataError = false;
    dumpTestcaseSwitch(false);
}

// Clear agent stack info
void VR_CNDialogEngine::clearTempData()
{
    VR_LOGD_FUNC();
    tthread::lock_guard<tthread::recursive_mutex> lock(_mutex);
    {
        // _deDataManager->releaseData();
        _stateSatck.empty();
        clearTmpState();
        _currentStateBackChangeAgent.stateName = "";
        _currentStateBackChangeAgent.stateAttr = "";
        _dataProcessor.clearListDataFromDM();
    }
}

bool VR_CNDialogEngine::Start()
{
    VR_LOGD_FUNC();
    if (_isRunning) {
        VR_LOGD("dialogEngine has started! we will restart it!");
        Stop();
    }
    VR_LOGD("dialogengine start thread=====");
    _isRunning = true;
    int nice = m_pDECommonIF->getTargetNice("vr_ctrl");
    VR_LOGD("set vr_ctrl priority %d", nice);
    _thread = VR_new tthread::thread(VR_CNDialogEngine::run, this, "vr_ctrl", nice); // start thread
    // some long time op push _internalServiceQueue
    uscxml::Event initInterpretesEvent;
    initInterpretesEvent.setName("initInterpretes_Event");
    postEvent(initInterpretesEvent);

    uscxml::Event initEvent;
    initEvent.setName("initAsrFactory_Event");
    postEvent(initEvent);

    if (_actionListener != NULL) {
        _actionListener->OnStarted();
    }
    RETURN(true);
}

void VR_CNDialogEngine::doStop()
{
    VR_LOGD_FUNC();

    if (!_isRunning) {
        VR_LOGD("dialogEngine has stop");
        return;
    }

    if (_interManager != NULL) {
        _interManager->stop();
    }

    if (_thread != NULL) {
        if (_thread->get_id() != tthread::this_thread::get_id()) {
            _isRunning = false;
            _condVar.notify_all();
            _thread->join();
            delete(_thread);
            _thread = NULL;
        }
        else {
            VR_LOGE("can't stop from itself thread, stop failed");
            return;
        }
    }

    if (_actionListener != NULL) {
        resourceStateChange("init", ResourceState::GRAMMAR_NOT_READY);
        _actionListener->OnStopped();
    }
    return;
}

void VR_CNDialogEngine::Stop()
{
    VR_LOGD_FUNC();
    doStop();

}

VR_CNDialogEngine::~VR_CNDialogEngine()
{
    VR_LOGD_FUNC();
    doStop(); // delete thread and interpreter
    if (m_pIntentionParser != NULL) {
        delete m_pIntentionParser;
        m_pIntentionParser = NULL;
    }
    if (m_pDECommonIF != NULL) {
        delete m_pDECommonIF;
        m_pDECommonIF = NULL;
    }
    if (m_pDEMessageBuilder != NULL) {
        delete m_pDEMessageBuilder;
        m_pDEMessageBuilder = NULL;
    }
    if (NULL != _interManager) {
        delete _interManager;
        _interManager = NULL;
    }
    if (_asrEngine != NULL) {
        delete _asrEngine;
        _asrEngine = NULL;
    }
    if (_dataAccessorManager != NULL) {
        delete _dataAccessorManager;
        _dataAccessorManager = NULL;
    }
}

bool VR_CNDialogEngine::updateGrammar(N_Vr::N_Asr::C_Request_Context_List_Update& updateMsg)
{
    VR_LOGD_FUNC();
    if (_asrEngine) {
        _asrEngine->Context_List_Update(updateMsg);
        RETURN(true);
    }
    else {
        VR_LOGE("ASR Engine point is Null");
        RETURN(false);
    }
}

bool VR_CNDialogEngine::genVoiceTagPhoneme()
{
    RETURN(false);
}


void VR_CNDialogEngine::setGrammarActive(const std::string &contextID, bool isActive, const VoiceList<std::string>::type &ruleIDList)
{
    VR_LOGD_FUNC();

    if (!_asrEngine) {
        VR_LOGE("ASR Engine point is Null");
        return;
    }

    if (std::string::npos != contextID.find("ctx_media_")) {
        if (_bDisableTopmenuSong) {
            _bDisableTopmenuSong = false;
            N_Vr::N_Asr::C_Request_Param_Set songLoadParam;
            songLoadParam.m_e_Param_Type = E_Param_Type_Resource_Song;
            songLoadParam.m_i_Value = 1;
            _asrEngine->Param_Set(songLoadParam);
            VR_LOGI("Load_Song_Resource_Song_For_Media_Source");
        }
        if (!ruleIDList.empty()) {
            _bDisableTopmenuSong = true;
        }
        N_Vr::N_Asr::C_Request_Param_Set param;
        param.m_e_Param_Type = E_Param_Type_Audio_Src;
        if (isActive) {
            param.m_string_Value = contextID;
        }
        else {
            param.m_string_Value = "";
        }
        _asrEngine->Param_Set(param);
        VR_LOGI("contextID:%s is Active[%d]", contextID.c_str(), isActive);

        if (_bDisableTopmenuSong && isActive) {
            N_Vr::N_Asr::C_Request_Param_Set songUnLoadParam;
            songUnLoadParam.m_e_Param_Type = E_Param_Type_Resource_Song;
            songUnLoadParam.m_i_Value = 0;
            _asrEngine->Param_Set(songUnLoadParam);
            VR_LOGI("Default_UnLoad_Song_Resource_Song");
        }
    }
    else if (std::string::npos != contextID.find("ctx_phone_")) {
        N_Vr::N_Asr::C_Request_Param_Set param;
        param.m_e_Param_Type = E_Param_Type_Contact_Src;
        if (isActive) {
            param.m_string_Value = contextID;
        }
        else {
            param.m_string_Value = "";
        }
        _asrEngine->Param_Set(param);
        VR_LOGI("%s %s", contextID.c_str(), isActive?"active":"disactive");
    }
    else {
        VR_LOGI("%s", (contextID + " is not correct").c_str());
    }
}

void VR_CNDialogEngine::updateGrammarCategoryFinish(const std::string &category)
{
    VR_LOGD_FUNC();
    uscxml::Event event;
    event.setName("updateGrammarCategoryFinish");
    event.setContent(category);
    postEvent(event);
}

void VR_CNDialogEngine::updateGrammarCategoryFailed(const std::string &sourceID, const std::string &size)
{
    VR_LOGD_FUNC();
    uscxml::Event event;
    event.setName("updateGrammarCategoryFailed");
    event.setSendId(sourceID);
    event.setContent(size);
    postEvent(event);
}

void VR_CNDialogEngine::updateGrammarFinish()
{
    VR_LOGD_FUNC();
    uscxml::Event event;
    event.setName("updateGrammarFinish");
    postEvent(event);
}

void VR_CNDialogEngine::grammarSpaceNotEnough(const std::string &infoMsg)
{
    VR_LOGD_FUNC();
    uscxml::Event event;
    event.setName("grammarSpaceNotEnough");
    event.setContent(infoMsg);
    postEvent(event);
}

void VR_CNDialogEngine::deleteMediaGrammar(const std::string &deviceID, const std::string &infoMsg)
{
    uscxml::Event event;
    event.setName("deleteMediaGrammar");
    event.setSendId(deviceID);
    event.setContent(infoMsg);
    postEvent(event);
}

void VR_CNDialogEngine::saveGrammarFile(const std::string &sourceID)
{
    uscxml::Event event;
    event.setName("saveGrammarFile");
    event.setSendId(sourceID);
    postEvent(event);
}

bool VR_CNDialogEngine::GetDataTagName(VoiceMap<std::string, std::string>::type & mapSlotValue, std::string strCMD, int iID, std::string strName)
{
    VR_LOGD_FUNC();

    if (m_cmnIdTagnamesMap.empty()) {
        VR_LOGE("subObjTagname map is Null");
        RETURN(false);
    }

    if (m_cmnIdTagnamesMap.count(strCMD.c_str()) == 0) {
        VR_LOGE("subObjTagname map vector is Null");
        RETURN(false);
    }

    boost::format strIdFmt("%1%");
    boost::format strFloatFmt("%.1f");
    if ("CMN3025" == strCMD || "CMN3027" == strCMD) { // FM freq format
        float fFreq = iID / 1000.0f;
        std::string strFltFreq = (strFloatFmt % fFreq).str();
        strIdFmt % strFltFreq.c_str();
    }
    else {
        if ("CMN7017" == strCMD) {
            float fDegrees = iID / 10.0f;
            std::string strFltDegrees = (strFloatFmt % fDegrees).str();
            strIdFmt % strFltDegrees.c_str();
        }
        else {
            if ("CMN7028" == strCMD) {
                std::string strMaxFanSpeed = stateClimateMap["CLIMATE_FANSPEED_MAX"];
                int nMaxSpeed = atoi(strMaxFanSpeed.c_str());
                if (iID > nMaxSpeed) {
                    VR_LOGD("current fan speed is [%d], max fan speed is [%d]", iID, nMaxSpeed);
                    RETURN(false);
                }
                else {
                    strIdFmt % iID;
                }
            }
            else {
                strIdFmt % iID;
            }
        }
    }

    VoiceVector<std::string>::type &vectorTagNames = m_cmnIdTagnamesMap[strCMD];
    int nlen = vectorTagNames.size();

    if (1 == nlen) {
        mapSlotValue.insert(std::pair<std::string, std::string>(vectorTagNames[0].c_str(), strIdFmt.str()));
    }
    else {
        if (2 == nlen) {
            mapSlotValue.insert(std::pair<std::string, std::string>(vectorTagNames[0].c_str(), strIdFmt.str()));
            mapSlotValue.insert(std::pair<std::string, std::string>(vectorTagNames[1].c_str(), strName));
        }
        else {
            VR_LOGE("subObjTagname vector size is error");
            RETURN(false);
        }
    }
    RETURN(true);
}

void VR_CNDialogEngine::readTagDataConf(const std::string strConfFilePath)
{
    VR_LOGD_FUNC();
    if (strConfFilePath.empty()) {
        return;
    }

    VR_LOGI("strConfFilePath is %s", strConfFilePath.c_str());

    std::ifstream fs(strConfFilePath.c_str());
    if (!fs.is_open()) {
        VR_LOGE("initial tagname map error:[%d]\n", __LINE__);
        return;
    }

    std::stringstream ss;
    ss << fs.rdbuf();
    std::string str = ss.str();
    fs.close();

    rapidjson::Document docJson;
    docJson.Parse<0>(str.c_str());

    if (docJson.HasParseError()) {
        VR_LOGE("initial tagname map error:[%d]\n", __LINE__);
        return;
    }

    const rapidjson::Value &arry = docJson["cmnidarry"];

    for (rapidjson::SizeType idx = 0; idx<arry.Size(); ++idx) {
        const rapidjson::Value &arryObj = arry[idx];
        std::string cmnid = arryObj["cmnid"].GetString();
        VoiceVector<std::string>::type conflist;
        m_cmnIdTagnamesMap.insert(std::make_pair(cmnid, conflist));
        const rapidjson::Value &keynamesArry = arryObj["keynames"];
        for (rapidjson::SizeType index = 0; index < keynamesArry.Size(); ++index) {
            m_cmnIdTagnamesMap[cmnid].push_back(keynamesArry[index].GetString());
        }
    }
}

// add for CN Climate state
void VR_CNDialogEngine::updateStateClimateMap(const std::string &stateMsg)
{
    pugi::xml_document doc;
    pugi::xml_parse_result ret = doc.load_string(stateMsg.c_str());

    if (!ret) {
        VR_LOGE("error to load stateMsg :[%d]\n", __LINE__);
        return;
    }

    pugi::xml_node itemNode = doc.select_node("/data/g").node();
    for (pugi::xml_node_iterator it = itemNode.begin(); it != itemNode.end(); ++it) {
        std::string key(it->attribute("key").as_string());
        std::string value(it->attribute("value").as_string());

        VoiceMap<std::string, std::string>::iterator itKeyValue = stateClimateMap.find(key);

        if (itKeyValue == stateClimateMap.end()
            || value == stateClimateMap[key]) {
            continue;
        }
        else {
            stateClimateMap[key] = value;
        }
        VR_LOGI("ClimateState [%s] updated [%s]", key.c_str(), value.c_str());
    }

}

bool VR_CNDialogEngine::checkClimateIntentIsValid(const std::string strCmnId)
{
    const std::string strClimateCmdKey = "CMN7";
    const std::string strClimateMenuCmdKey = "CMN0005";

    std::string strTmpCmp = strCmnId;
    boost::to_upper(strTmpCmp);

    std::size_t nFind = strTmpCmp.find(strClimateCmdKey);
    // not climate command
    if (strTmpCmp != strClimateMenuCmdKey
        && std::string::npos == nFind) {
        RETURN(true);
    }

    if (std::string::npos != nFind) {
        // CLIMATE_BASIC_ACTIVE CMN7
        VoiceMap<std::string, std::string>::iterator itKeyValue = cmnIdToKeyMap.find(strTmpCmp);
        if (itKeyValue == cmnIdToKeyMap.end()) {
            if ("false" == stateClimateMap[cmnIdToKeyMap[strClimateCmdKey]]) {
                RETURN(false);
            }
        }
        else {
            // not CMN7 CMN0005
            if ("false" == stateClimateMap[cmnIdToKeyMap[strTmpCmp]]) {
                RETURN(false);
            }
        }
    }
    else {
        if ("false" == stateClimateMap[cmnIdToKeyMap[strClimateMenuCmdKey]]) {
            RETURN(false);
        }
    }

    RETURN(true);
}

// false: reject, true: recognize
bool VR_CNDialogEngine::checkNaviRejectMapPreparing(const std::string strCmnId)
{
    const std::string strNaviCmdKey = "CMN2";
    const std::string strNaviMenuCmdId = "CMN0001";

    std::string strTmpCmp = strCmnId;
    boost::to_upper(strTmpCmp);

    std::size_t nFind = strTmpCmp.find(strNaviCmdKey);

    if (strNaviMenuCmdId == strTmpCmp
        || nFind != std::string::npos) {
        if (_bMapDataPreparing) {
            RETURN(false);
        }
    }

    RETURN(true);
}

std::string VR_CNDialogEngine::parseAsrAddressStrNBst(const std::string& xml)
{
    VR_LOGD_FUNC();
    std::string strAsrIntentResult = "";
    std::string sentenceStr = "";
    std::string strMaxCmdName = "";

    std::string strRetHead =  "<node name=\"intent\" value=\"%1%\" sentenceValue=\"%2%\" confidence=\"0\">"
                                        "<list id=\"%3%\">"
                                            "<header>"
                                                "<pageSize>0</pageSize>"
                                                "<startIndex>0</startIndex>"
                                                "<count>%4%</count>"
                                            "</header>"
                                            "<items>";

    std::string strRetAddressBodyItem = "<item>"
                                        "<addressId>"
                                            "<zone>%1%</zone>"
                                            "<zoneId>%2%</zoneId>"
                                            "<city>%3%</city>"
                                            "<cityId>%4%</cityId>"
                                            "<district>%5%</district>"
                                            "<districtId>%6%</districtId>"
                                            "<street>%7%</street>"
                                            "<streetId>%8%</streetId>"
                                            "<streetBody>%9%</streetBody>"
                                            "<houseNumber>%10%</houseNumber>"
                                        "</addressId>"
                                    "</item>";

    std::string strRetTail =        "</items>"
                                    "</list>"
                                "</node>";

    rapidjson::Document jsonDoc;
    jsonDoc.Parse<0>(xml.c_str());

    if (jsonDoc.HasParseError()) {
        VR_LOGE("DE parser the intention from ASR:[%d]\n", __LINE__);
        RETURN("");
    }

    if (jsonDoc.HasMember("rc")) {
        int iRc = jsonDoc["rc"].GetInt();
        if (0 != iRc) {
            VR_LOGD("DE parser the intention from ASR:[%d]\n", __LINE__);
            RETURN("");
        }
    }

    if (jsonDoc.HasMember("text")) {
        const rapidjson::Value& objRecogText = jsonDoc["text"];
        if (objRecogText.IsString()) {
            sentenceStr = objRecogText.GetString();
        }
    }

    if (!jsonDoc.HasMember("semantic")) {
        VR_LOGD("DE parser the intention from ASR:[%d]\n", __LINE__);
        RETURN("");
    }

    const rapidjson::Value &objSemantic = jsonDoc["semantic"];

    if (!objSemantic.HasMember("slots")) {
        VR_LOGD("DE parser the intention from ASR:[%d]\n", __LINE__);
        RETURN("");
    }

    const rapidjson::Value &objSlots = objSemantic["slots"];

    if (objSlots.HasMember("cmnId")) {
        const rapidjson::Value &objCmd = objSlots["cmnId"];
        if (objCmd.IsString()) {
            strMaxCmdName = objCmd.GetString();
            VR_LOGI("The Last Result cmd:%s\n", objCmd.GetString());
        }
    }

    // add sementic item
    std::string strAsrIntentItem = "";
    int nItemCount = 0;

    std::string strSlotsZone = "";
    std::string strSlotsZoneId = "";
    std::string strSlotsCity = "";
    std::string strSlotsCityId = "";
    std::string strSlotsDistrict = "";
    std::string strSlotsDistrictId = "";
    std::string strSlotsStreet = "";
    std::string strSlotsStreetId = "";
    std::string strSlotsStreetBody = "";
    std::string strSlotsHouseNumber = "";

    if (objSlots.HasMember("province")) {
        const rapidjson::Value& objProvince = objSlots["province"];
        strSlotsZone = objProvince.GetString();
    }

    if (("navi_speak_entire_address_{country}" == _grammarStr)
        && ("" == strSlotsZone)) {
        RETURN("");
    }

    if (objSlots.HasMember("city")) {
        const rapidjson::Value& objCity = objSlots["city"];
        strSlotsCity = objCity.GetString();
    }

    if (objSlots.HasMember("area")) {
        const rapidjson::Value& objArea = objSlots["area"];
        strSlotsDistrict = objArea.GetString();
    }

    if (objSlots.HasMember("street")) {
        const rapidjson::Value& objStreet = objSlots["street"];
        strSlotsStreet = objStreet.GetString();
    }

    if (objSlots.HasMember("streetBody")) {
        const rapidjson::Value& objStreetBody = objSlots["streetBody"];
        strSlotsStreetBody = objStreetBody.GetString();
    }

    if (objSlots.HasMember("houseNumber")) {
        const rapidjson::Value& objHouseNumber = objSlots["houseNumber"];
        strSlotsHouseNumber = objHouseNumber.GetString();
    }


    if ("" != strSlotsZone || "" != strSlotsCity
        || "" != strSlotsDistrict || "" != strSlotsStreet
        || "" != strSlotsStreetBody || "" != strSlotsHouseNumber) {
        strAsrIntentItem = (boost::format(strRetAddressBodyItem) % strSlotsZone % strSlotsZoneId % strSlotsCity % strSlotsCityId % strSlotsDistrict % strSlotsDistrictId % strSlotsStreet % strSlotsStreetId % strSlotsStreetBody % strSlotsHouseNumber).str();
        ++nItemCount;
    }

    if (jsonDoc.HasMember("moreResultsBySr")) {

        const rapidjson::Value &objMoreRstBySr = jsonDoc["moreResultsBySr"];

        if (!objMoreRstBySr.IsArray()) {
            VR_LOGD("DE parser the intention from ASR:[%d]\n", __LINE__);
            RETURN("");
        }

        for (rapidjson::SizeType idx = 0; idx < objMoreRstBySr.Size(); ++idx) {
            const rapidjson::Value &ObjArray = objMoreRstBySr[idx];
            if (ObjArray.IsObject()) {
                std::string strZone = "";
                std::string strZoneId = "";
                std::string strCity = "";
                std::string strCityId = "";
                std::string strDistrict = "";
                std::string strDistrictId = "";
                std::string strStreet = "";
                std::string strStreetId = "";
                std::string strStreetBody = "";
                std::string strHouseNumber = "";
                // std::string strPOIName = "";

                if (!ObjArray.HasMember("semantic")) {
                    VR_LOGD("DE parser the intention from ASR:[%d]\n", __LINE__);
                    RETURN("");
                }

                const rapidjson::Value &objAddrSemantic = ObjArray["semantic"];

                if (!objAddrSemantic.HasMember("slots")) {
                    VR_LOGD("DE parser the intention from ASR:[%d]\n", __LINE__);
                    RETURN("");
                }

                const rapidjson::Value &objAddrSlots = objAddrSemantic["slots"];

                if (objAddrSlots.HasMember("cmnId")) {
                    const rapidjson::Value &objMoreCmd = objAddrSlots["cmnId"];
                    if (objMoreCmd.IsString()) {
                        std::string strMorCmdName = objMoreCmd.GetString();
                        VR_LOGI("The More Result cmd:%s\n", objMoreCmd.GetString());
                        if (strMorCmdName != strMaxCmdName) {
                           continue;
                        }
                    }
                }

                if (objAddrSlots.HasMember("province")) {
                    const rapidjson::Value& objProvince = objAddrSlots["province"];
                    strZone = objProvince.GetString();
                }

                if (("navi_speak_entire_address_{country}" == _grammarStr)
                    && ("" == strZone)) {
                    RETURN("");
                }

                if (objAddrSlots.HasMember("city")) {
                    const rapidjson::Value& objCity = objAddrSlots["city"];
                    strCity = objCity.GetString();
                }

                if (objAddrSlots.HasMember("area")) {
                    const rapidjson::Value& objArea = objAddrSlots["area"];
                    strDistrict = objArea.GetString();
                }

                if (objAddrSlots.HasMember("street")) {
                    const rapidjson::Value& objStreet = objAddrSlots["street"];
                    strStreet = objStreet.GetString();
                }

                if (objAddrSlots.HasMember("streetBody")) {
                    const rapidjson::Value& objStreetBody = objAddrSlots["streetBody"];
                    strStreetBody = objStreetBody.GetString();
                }

                if (objAddrSlots.HasMember("houseNumber")) {
                    const rapidjson::Value& objHouseNumber = objAddrSlots["houseNumber"];
                    strHouseNumber = objHouseNumber.GetString();
                }

                std::string strItems = (boost::format(strRetAddressBodyItem) % strZone % strZoneId % strCity % strCityId % strDistrict % strDistrictId % strStreet % strStreetId % strStreetBody % strHouseNumber).str();
                strAsrIntentItem = strAsrIntentItem + strItems;
                ++nItemCount;
            }
        }
    }


    std::stringstream ss;
    ss << _asrResultId;
    _asrResultId++;
    strAsrIntentResult = (boost::format(strRetHead) % strMaxCmdName % sentenceStr % ss.str() % std::to_string(nItemCount)).str();
    strAsrIntentResult = strAsrIntentResult + strAsrIntentItem + strRetTail;

    // format the result
    pugi::xml_document xmlResult;
    pugi::xml_parse_result ret = xmlResult.load_string(strAsrIntentResult.c_str());

    if (!ret) {
        VR_LOGD("error to load strAsrIntentResult");
        RETURN("");
    }

    std::ostringstream oss;
    xmlResult.print(oss);
    std::string strXmlResult = oss.str();

    VR_LOGI("The Last strXmlResult :%s\n", strXmlResult.c_str());

    RETURN(strXmlResult);

}

std::string VR_CNDialogEngine::parseAsrPOIStrNBst(const std::string& xml)
{
    VR_LOGD_FUNC();
    std::string strAsrIntentResult = "";
    std::string sentenceStr = "";
    std::string strMaxCmdName = "";
    bool webSearchFlag = false;

    std::string strRetHead =  "<node name=\"intent\" value=\"%1%\" sentenceValue=\"%2%\" confidence=\"0\">"
                                        "<list id=\"%3%\">"
                                            "<header>"
                                                "<pageSize>0</pageSize>"
                                                "<startIndex>0</startIndex>"
                                                "<count>%4%</count>"
                                                "<webSearch>%5%</webSearch>"
                                            "</header>"
                                            "<items>";

    std::string strRetPOIBodyItem = "<item>"
                                        "<POI_name>%1%</POI_name>"
                                        "<city>%2%</city>"
                                "</item>";

    std::string strRetTail =        "</items>"
                                    "</list>"
                                "</node>";
    std::string strRetPOICateBodyItem = "<item>"
                                        "<POI_category_id>%1%</POI_category_id>"
                                        "<POI_category_name>%2%</POI_category_name>"
                                        "<city>%3%</city>"
                                "</item>";


    rapidjson::Document jsonDoc;
    jsonDoc.Parse<0>(xml.c_str());

    if (jsonDoc.HasParseError()) {
        VR_LOGE("DE parser the intention from ASR:[%d]\n", __LINE__);
        RETURN("");
    }

    if (jsonDoc.HasMember("rc")) {
        int iRc = jsonDoc["rc"].GetInt();
        if (0 != iRc) {
            VR_LOGD("DE parser the intention from ASR:[%d]\n", __LINE__);
            RETURN("");
        }
    }

    if (jsonDoc.HasMember("text")) {
        const rapidjson::Value& objRecogText = jsonDoc["text"];
        if (objRecogText.IsString()) {
            sentenceStr = objRecogText.GetString();
        }
    }

    if (!jsonDoc.HasMember("semantic")) {
        VR_LOGD("DE parser the intention from ASR:[%d]\n", __LINE__);
        RETURN("");
    }

    const rapidjson::Value &objSemantic = jsonDoc["semantic"];

    if (!objSemantic.HasMember("slots")) {
        VR_LOGD("DE parser the intention from ASR:[%d]\n", __LINE__);
        RETURN("");
    }

    const rapidjson::Value &objSlots = objSemantic["slots"];

    if (objSlots.HasMember("cmnId")) {
        const rapidjson::Value &objCmd = objSlots["cmnId"];
        if (objCmd.IsString()) {
            strMaxCmdName = objCmd.GetString();
            VR_LOGI("The Last Result cmd:%s\n", objCmd.GetString());
        }
    }

    // add sementic item
    std::string strAsrIntentItem = "";
    int nItemCount = 0;
    std::string strSlotsPOIName = "";
    std::string strSlotsPOINameCity = "";
    std::string strSlotsPOIGenreName = "";
    std::string strSlotsPOIGenreID = "";
    std::string strSlotsPOIGenreCity = "";

    if (objSlots.HasMember("poiName")) {
        const rapidjson::Value& objpoiName = objSlots["poiName"];
        strSlotsPOIName = objpoiName.GetString();

        if (objSlots.HasMember("city")) {
            const rapidjson::Value& objpoiNameCity = objSlots["city"];
            strSlotsPOINameCity = objpoiNameCity.GetString();
        }
        else if (objSlots.HasMember("province")) {
            const rapidjson::Value& objpoiNameCity = objSlots["province"];
            strSlotsPOINameCity = objpoiNameCity.GetString();
        }
    }

    if ("" != strSlotsPOIName) {
        strAsrIntentItem = (boost::format(strRetPOIBodyItem) % strSlotsPOIName % strSlotsPOINameCity).str();
        ++nItemCount;
    }

    if (jsonDoc.HasMember("moreResultsBySr")) {

        const rapidjson::Value &objMoreRstBySr = jsonDoc["moreResultsBySr"];

        if (!objMoreRstBySr.IsArray()) {
            VR_LOGD("DE parser the intention from ASR:[%d]\n", __LINE__);
            RETURN("");
        }

        for (rapidjson::SizeType idx = 0; idx < objMoreRstBySr.Size(); ++idx) {
            const rapidjson::Value &ObjArray = objMoreRstBySr[idx];
            if (ObjArray.IsObject()) {
                std::string strPOIName = "";
                std::string strPOINameCity = "";

                if (!ObjArray.HasMember("semantic")) {
                    VR_LOGD("DE parser the intention from ASR:[%d]\n", __LINE__);
                    RETURN("");
                }

                const rapidjson::Value &objAddrSemantic = ObjArray["semantic"];

                if (!objAddrSemantic.HasMember("slots")) {
                    VR_LOGD("DE parser the intention from ASR:[%d]\n", __LINE__);
                    RETURN("");
                }

                const rapidjson::Value &objAddrSlots = objAddrSemantic["slots"];

                if (objAddrSlots.HasMember("cmnId")) {
                    const rapidjson::Value &objMoreCmd = objAddrSlots["cmnId"];
                    if (objMoreCmd.IsString()) {
                        std::string strMorCmdName = objMoreCmd.GetString();
                        VR_LOGI("The More Result cmd:%s\n", objMoreCmd.GetString());
                        if (strMorCmdName != strMaxCmdName) {
                           continue;
                        }
                    }
                }

                if (objAddrSlots.HasMember("poiName")) {
                    const rapidjson::Value& objpoiName = objAddrSlots["poiName"];
                    strPOIName = objpoiName.GetString();

                    if (objAddrSlots.HasMember("city")) {
                        const rapidjson::Value& objpoiNameCity = objAddrSlots["city"];
                        strPOINameCity = objpoiNameCity.GetString();
                    }
                    else if (objAddrSlots.HasMember("province")) {
                        const rapidjson::Value& objpoiNameCity = objAddrSlots["province"];
                        strPOINameCity = objpoiNameCity.GetString();
                    }
                }

                if ("" != strPOIName) {
                    std::string strItems = (boost::format(strRetPOIBodyItem) % strPOIName % strPOINameCity).str();
                    strAsrIntentItem = strAsrIntentItem + strItems;
                    ++nItemCount;
                }
            }
        }
    }

    if (objSlots.HasMember("poiCategoryId")) {
        const rapidjson::Value &objPoiCategoryId = objSlots["poiCategoryId"];
        if (objPoiCategoryId.IsString()) {
            strSlotsPOIGenreID = objPoiCategoryId.GetString();
        }

        if (objSlots.HasMember("poiCategory")) {
            const rapidjson::Value &objPoiCategoryName = objSlots["poiCategory"];
            if (objPoiCategoryName.IsString()) {
                strSlotsPOIGenreName = objPoiCategoryName.GetString();
            }
        }

        if (objSlots.HasMember("city")) {
            const rapidjson::Value& objpoiCategoryCity = objSlots["city"];
            strSlotsPOIGenreCity = objpoiCategoryCity.GetString();
        }
        else if (objSlots.HasMember("province")) {
            const rapidjson::Value& objpoiCategoryCity = objSlots["province"];
            strSlotsPOIGenreCity = objpoiCategoryCity.GetString();
        }

        nItemCount = 0;
        ++nItemCount;
        strAsrIntentItem = (boost::format(strRetPOICateBodyItem) % strSlotsPOIGenreID % strSlotsPOIGenreName % strSlotsPOIGenreCity).str();
    }

    if (jsonDoc.HasMember("data")) {
        std::string xmlStr = "";
        if (getPOIFromWebData(xml, xmlStr)) {
            _dataProcessor.updateListByWebSearch(xmlStr);
            webSearchFlag = true;
        }
       // webSearchFlag = true;
    }

    std::stringstream ss;
    ss << _asrResultId;
    _asrResultId++;
    strAsrIntentResult = (boost::format(strRetHead) % strMaxCmdName % sentenceStr % ss.str() % std::to_string(nItemCount)  % (webSearchFlag ? "true" : "false")).str();
    strAsrIntentResult = strAsrIntentResult + strAsrIntentItem + strRetTail;

    // format the result
    pugi::xml_document xmlResult;
    pugi::xml_parse_result ret = xmlResult.load_string(strAsrIntentResult.c_str());

    if (!ret) {
        VR_LOGD("error to load strAsrIntentResult");
        RETURN("");
    }

    std::ostringstream oss;
    xmlResult.print(oss);
    std::string strXmlResult = oss.str();

    VR_LOGI("The Last strXmlResult :%s\n", strXmlResult.c_str());

    RETURN(strXmlResult);

}


std::string VR_CNDialogEngine::parseAsrInfoStr(const std::string& xml)
{
    VR_LOGD_FUNC();
    std::string strAsrIntentResult = "";
    std::string sentenceStr = "";
    std::string strMaxCmdName = "";
    std::string xmlStr = "";

    std::string strRetHead =  "<node name=\"intent\" value=\"%1%\" sentenceValue=\"%2%\" confidence=\"0\">"
                                    "%3%"
                                "</node>";
    std::string nullList  =     "<list id=\"%1%\">"
                                            "<header>"
                                                "<pageSize>0</pageSize>"
                                                "<startIndex>0</startIndex>"
                                                "<count>0</count>"
                                            "</header>"
                                "</list>";


    rapidjson::Document jsonDoc;
    jsonDoc.Parse<0>(xml.c_str());

    if (jsonDoc.HasParseError()) {
        VR_LOGE("DE parser the intention from ASR:[%d]\n", __LINE__);
        RETURN("");
    }

    if (jsonDoc.HasMember("rc")) {
        int iRc = jsonDoc["rc"].GetInt();
        if (0 != iRc) {
            VR_LOGD("DE parser the intention from ASR:[%d]\n", __LINE__);
            RETURN("");
        }
    }

    if (jsonDoc.HasMember("text")) {
        const rapidjson::Value& objRecogText = jsonDoc["text"];
        if (objRecogText.IsString()) {
            sentenceStr = objRecogText.GetString();
        }
    }

    if (!jsonDoc.HasMember("semantic")) {
        VR_LOGD("DE parser the intention from ASR:[%d]\n", __LINE__);
        RETURN("");
    }

    const rapidjson::Value &objSemantic = jsonDoc["semantic"];

    if (!objSemantic.HasMember("slots")) {
        VR_LOGD("DE parser the intention from ASR:[%d]\n", __LINE__);
        RETURN("");
    }

    const rapidjson::Value &objSlots = objSemantic["slots"];

    if (objSlots.HasMember("cmnId")) {
        const rapidjson::Value &objCmd = objSlots["cmnId"];
        if (objCmd.IsString()) {
            strMaxCmdName = objCmd.GetString();
            VR_LOGI("The Information Result cmd:%s\n", objCmd.GetString());
        }
    }

    // add sementic item
    // std::string strAsrIntentItem = "";

    if (jsonDoc.HasMember("data")) {
        const rapidjson::Value &objData = jsonDoc["data"];
        VR_LOGI("The Information Result Data:%s\n", objData.GetString());

        if (!getInformationFromWebData(xml, xmlStr)) {
            VR_LOGD("error to load strAsrIntentResult");
            // RETURN("");
        }

        if ("" == xmlStr) {
            std::stringstream ss;
            ss << _asrResultId;
            _asrResultId++;
            xmlStr = (boost::format(nullList) % ss.str()).str();
        }
    }
    else {
        std::stringstream ss;
        ss << _asrResultId;
        _asrResultId++;
        xmlStr = (boost::format(nullList) % ss.str()).str();
    }

    strAsrIntentResult = (boost::format(strRetHead) % strMaxCmdName % sentenceStr % xmlStr).str();

    // format the result
    pugi::xml_document xmlResult;
    pugi::xml_parse_result ret = xmlResult.load_string(strAsrIntentResult.c_str());

    if (!ret) {
        VR_LOGD("error to load strAsrIntentResult");
        RETURN("");
    }

    std::ostringstream oss;
    xmlResult.print(oss);
    std::string strXmlResult = oss.str();

    VR_LOGI("The Last strXmlResult :%s\n", strXmlResult.c_str());

    RETURN(strXmlResult);

}

std::string VR_CNDialogEngine::parseAsrToIntention(const std::string& xml)
{
    VR_LOGD_FUNC();
    int maxConfOut = 0;
    std::string maxCmdName = "";
    std::string sentenceStr = "";
    // std::string serviceStr = "";
    int itemCount = 0;
    // int mainId = 0;
    bool bRet = true;

    pugi::xml_node maxNodeOut;
    pugi::xml_document doc;
    pugi::xml_document containItemNodes;

    rapidjson::Document jsonDoc;
    jsonDoc.Parse<0>(xml.c_str());

    do {
        bRet = false;
        if (jsonDoc.HasParseError()) {
            VR_LOGE("DE parser the intention from ASR: json format was error");
            break;
        }

        if (jsonDoc.HasMember("text")) {
            const rapidjson::Value& objRecogText = jsonDoc["text"];
            if (objRecogText.IsString()) {
                sentenceStr = objRecogText.GetString();
            }
        }

        if (jsonDoc.HasMember("rc")) {
            int iRc = jsonDoc["rc"].GetInt();
            if (0 != iRc) {
                VR_LOGI("DE parser the intention from ASR: rc 4");
                break;
            }
        }

        // if (jsonDoc.HasMember("service")) {
        //     const rapidjson::Value& objService = jsonDoc["service"];
        //     if (objService.IsString()) {
        //         serviceStr = objService.GetString();
        //     }
        // }

        if (jsonDoc.HasMember("nlocalconfidencescore")) {
            const rapidjson::Value& objScore = jsonDoc["nlocalconfidencescore"];
            if (objScore.IsString()) {
                maxConfOut = atoi(objScore.GetString());
                maxConfOut *= 5;
            }
        }

        if (!jsonDoc.HasMember("semantic")) {
            VR_LOGD("DE parser the intention from ASR: no semantic");
            break;
        }

        const rapidjson::Value &objSemantic = jsonDoc["semantic"];

        if (!objSemantic.HasMember("slots")) {
            VR_LOGD("DE parser the intention from ASR: no slots");
            break;
        }

        const rapidjson::Value &objSlots = objSemantic["slots"];

        if (objSlots.HasMember("cmnId")) {
            const rapidjson::Value &objCmd = objSlots["cmnId"];
            if (objCmd.IsString()) {
                maxCmdName = objCmd.GetString();
                VR_LOGI("The Last Result cmd:%s\n", objCmd.GetString());
                // add for climate state
                if (!checkClimateIntentIsValid(maxCmdName)) {
                    VR_LOGD("The State of Climate is not true");
                    RETURN("");
                }
                // add for rejecting navi commands when map is preparing
                if (!checkNaviRejectMapPreparing(maxCmdName)) {
                    VR_LOGD("Reject the navi command because of mapPreparing");
                    RETURN("");
                }
            }
        }

        // for (VoiceVector<std::string>::iterator it = centerOnlyCMDRecogVector.begin(); it != centerOnlyCMDRecogVector.end(); ++it) {
        //     if (maxCmdName == *it) {
        //         if (!jsonDoc.HasMember("data")) {
        //             for (VoiceVector<std::string>::iterator it1 = communicationErrorVector.begin(); it1 != communicationErrorVector.end(); ++it1) {
        //                 if (m_strCommunicationStatus == *it1) {
        //                     RETURN("done.queryGBookStatus");
        //                 }
        //             }
        //         }
        //     }
        // }

        if ("mix" != m_strAsrMode) {
            for (VoiceVector<std::string>::iterator it = limitCMDIDOnLocalRecogVector.begin(); it != limitCMDIDOnLocalRecogVector.end(); ++it) {
                if (maxCmdName == *it) {
                    RETURN("");
                }
            }
        }

        // // for only local vr model
        // if (!m_pConfigureIF->getHybridVRFlag()) {
        //     for (VoiceVector<std::string>::iterator it = limitCMDIDOnLocalRecogVector.begin(); it != limitCMDIDOnLocalRecogVector.end(); ++it) {
        //         if (maxCmdName == *it) {
        //             RETURN("");
        //         }
        //     }
        // }

        // Adreress
        for (VoiceVector<std::string>::iterator it = addressCMDIDVector.begin(); it != addressCMDIDVector.end(); ++it) {
            if (maxCmdName == *it) {
                std::string strIntentResult = parseAsrAddressStrNBst(xml);
                RETURN(strIntentResult);
            }
        }

        // POI
        for (VoiceVector<std::string>::iterator it = poiCMDIDVector.begin(); it != poiCMDIDVector.end(); ++it) {
            if (maxCmdName == *it) {
                std::string strIntentResult = parseAsrPOIStrNBst(xml);
                RETURN(strIntentResult);
            }
        }

        // information
        for (VoiceVector<std::string>::iterator it = informationCMDIDVector.begin(); it != informationCMDIDVector.end(); ++it) {
            if (maxCmdName == *it) {
                std::string strIntentResult = parseAsrInfoStr(xml);
                RETURN(strIntentResult);
            }
        }



        if (objSlots.HasMember("code")) {
            const rapidjson::Value &objPhoneNo = objSlots["code"];
            if (objPhoneNo.IsString()) {
                pugi::xml_node tmpNode = doc.append_child("node");
                tmpNode.set_name("node");
                pugi::xml_node tmpNode1 = tmpNode.append_child("node");
                tmpNode1.set_name("node");
                tmpNode1.append_attribute("name").set_value("phone_number");
                tmpNode1.append_attribute("value").set_value(objPhoneNo.GetString());
                tmpNode1.append_attribute("confidence").set_value(maxConfOut);
            }
        }

        if (!objSlots.HasMember("subObjs")) {
            VR_LOGI("DE parser the intention from ASR: no subObjs");
            bRet = true;
            break;
        }

        // id is no need for max or min climate command
        VoiceVector<std::string>::iterator itClimateCmd = climateMaxMinCMDIDVector.begin();
        for (; itClimateCmd != climateMaxMinCMDIDVector.end(); ++itClimateCmd) {
            if (maxCmdName == *itClimateCmd) {
                break;
            }
        }

        if (itClimateCmd != climateMaxMinCMDIDVector.end()) {
            VR_LOGD("Climate subObjs id is no need");
            bRet = true;
            break;
        }

        const rapidjson::Value &objSubObjs = objSlots["subObjs"];

        if (!objSubObjs.IsArray()) {
            VR_LOGD("json format error: subObjs was not array");
            break;
        }

        for (rapidjson::SizeType idx = 0; idx < objSubObjs.Size(); ++idx) {
            const rapidjson::Value &ObjArray = objSubObjs[idx];
            if (ObjArray.IsObject()) {
                const rapidjson::Value &ObjArrayID = ObjArray["id"];

                if (!ObjArrayID.IsInt()) {
                    VR_LOGD("json format error: the type of id was not int");
                    RETURN("");
                }

                const rapidjson::Value &ObjArrayName = ObjArray["name"];

                if (!ObjArrayName.IsString()) {
                    VR_LOGD("json format error: the type of name was not string");
                    RETURN("");
                }

                int iSubobjID = ObjArrayID.GetInt();
                std::string strSubobjName = ObjArrayName.GetString();

                pugi::xml_node tmpNode = doc.append_child("node");
                tmpNode.set_name("node");
                itemCount++;

                VoiceMap<std::string, std::string>::type mapSlotValue;

                bool bRet = GetDataTagName(mapSlotValue, maxCmdName, iSubobjID, strSubobjName);
                if (!bRet) {
                    VR_LOGD("GetDataTagName is error");
                    RETURN("");
                }

                for (VoiceMap<std::string, std::string>::iterator it = mapSlotValue.begin(); it != mapSlotValue.end(); ++it) {
                    pugi::xml_node tmpNode1 = tmpNode.append_child("node");
                    tmpNode1.set_name("node");
                    tmpNode1.append_attribute("name").set_value(it->first.c_str());
                    tmpNode1.append_attribute("value").set_value(it->second.c_str());
                    tmpNode1.append_attribute("confidence").set_value(maxConfOut);
                }

                if (objSlots.HasMember("phonetype")) {
                    const rapidjson::Value &objPhoneType = objSlots["phonetype"];
                    if (objPhoneType.IsObject()) {
                        // get phonetype fix me
                        const rapidjson::Value &ObjPhoneTypeID = objPhoneType["id"];

                        if (!ObjPhoneTypeID.IsInt()) {
                            VR_LOGD("json format error: phone type not int");
                            RETURN("");
                        }

                        int nPhoneTypeId = ObjPhoneTypeID.GetInt();
                        nPhoneTypeId = transformPhoneTypeID(nPhoneTypeId);

                        pugi::xml_node tmpNode1 = tmpNode.append_child("node");
                        tmpNode1.set_name("node");
                        tmpNode1.append_attribute("name").set_value("contact_phone_type");
                        tmpNode1.append_attribute("value").set_value(std::to_string(nPhoneTypeId).c_str());
                        tmpNode1.append_attribute("confidence").set_value(maxConfOut);
                    }
                }
            }
        }
        bRet = true;
    } while (false);

    if (!bRet) {
        RETURN("");
    }
    // build node
    pugi::xml_document resultDoc;
    pugi::xml_node result = resultDoc.append_child("node");
    result.set_name("node");
    result.append_attribute("name").set_value("intent");
    result.append_attribute("value").set_value(maxCmdName.c_str());
    result.append_attribute("sentenceValue").set_value(sentenceStr.c_str());
    result.append_attribute("confidence").set_value(maxConfOut);
    // list node
    pugi::xml_node listNode = result.append_child("list");
    std::stringstream ss;
    ss << _asrResultId;
    _asrResultId++;
    std::string asrResultId = "asr" + ss.str();
    listNode.append_attribute("id").set_value(asrResultId.c_str());
    // header node
    pugi::xml_node headerNode = listNode.append_child("header");
    headerNode.append_child("pageSize").append_child(pugi::node_pcdata).set_value("0");

    headerNode.append_child("startIndex").append_child(pugi::node_pcdata).set_value("0");
    char buf[64] = {};
    // memset(buf, 0x00, sizeof(buf));
    snprintf(buf, 64, "%d", itemCount);
    headerNode.append_child("count").append_child(pugi::node_pcdata).set_value(buf);
    // items node
    pugi::xml_node itemsNode = listNode.append_child("items");
    pugi::xml_node conItemNode = doc.first_child();
    while (NULL != conItemNode) {
        // item node
        pugi::xml_node itemNode = itemsNode.append_child("item");
        pugi::xml_document tmpNode;
        tmpNode.append_copy(conItemNode);
        tmpNode.first_child().set_name("nodeTmp");
        pugi::xpath_node_set xpathNodes = tmpNode.select_nodes("//node");
        pugi::xpath_node_set::iterator nodeIt = xpathNodes.begin();
        while (nodeIt != xpathNodes.end()) {
            std::string nodeName = nodeIt->node().attribute("name").as_string();
            pugi::xml_node subNode = itemNode.append_child(nodeName.c_str());
            std::string nodeValue = nodeIt->node().attribute("value").as_string();
            subNode.append_child(pugi::node_pcdata).set_value(nodeValue.c_str());
            int nodeConfidence = nodeIt->node().attribute("confidence").as_int();
            subNode.append_attribute("confidence").set_value(nodeConfidence);
            ++nodeIt;
        }
        conItemNode = conItemNode.next_sibling();
    }

    if (0 == maxCmdName.compare(0, 7, "CMN4011")
        || 0 == maxCmdName.compare(0, 7, "CMN4012")) {
        // remove same contact name, get full contact id by search DB and set the max confidence phonetypeid
        preprocessPhoneTypeResult(result);
    }
    else if (0 == maxCmdName.compare(0, 7, "CMN3002")
        || 0 == maxCmdName.compare(0, 7, "CMN3003")) {
        _dataAccessorManager->handleMusicResult(result, "artist");
    }
    else if (0 == maxCmdName.compare(0, 7, "CMN3005")
        || 0 == maxCmdName.compare(0, 7, "CMN3006")) {
        _dataAccessorManager->handleMusicResult(result, "album");
    }
    else if (0 == maxCmdName.compare(0, 7, "CMN3007")
        || 0 == maxCmdName.compare(0, 7, "CMN3009")) {
        _dataAccessorManager->handleMusicResult(result, "playlist");
    }
    else if (0 == maxCmdName.compare(0, 7, "CMN3010")
        || 0 == maxCmdName.compare(0, 7, "CMN3012")) {
        _dataAccessorManager->handleMusicResult(result, "song");
    }
    else {
        // do not handle
    }


    // recount the count
    size_t count = result.select_nodes("//item").size();
    headerNode.child("count").text().set((unsigned int)count);

    // m_pIntentionParser->intelligentConventRadioCN(result);
    // to string
    std::ostringstream oss;
    result.print(oss);
    std::string intentionStr = oss.str();
    VR_LOGI("DE parser the intention from ASR:[%s]\n", intentionStr.c_str());
    RETURN(intentionStr);
}

int VR_CNDialogEngine::generateSeqID()
{
    ++_seqId;
    if (0 == _seqId) {
        return ++_seqId;
    }
    else {
        return _seqId;
    }
}

int VR_CNDialogEngine::requestAction(const std::string& action)
{
    VR_LOGD_FUNC();
    assert(NULL != _actionListener);
//    std::string str = action;
//    std::size_t pos = str.find(">''<");
//    while (pos != std::string::npos) {
//        str.replace(pos+1, 2, "");
//        pos = str.find(">''<");
//    }
    VR_LOG_TO_FILE("requestAction", action);
    tthread::lock_guard<tthread::recursive_mutex> lck(_mutexRequest);
    int seqID = generateSeqID();
    _actionListener->OnRequestAction(action, seqID);
    RETURN(seqID);
}

void VR_CNDialogEngine::requestVR(const std::string& grammar)
{
    VR_LOGD_FUNC();

    if (isDEQuiting()) {
       VR_LOGD("can't requestVR");
       return;
    }

    VR_LOGP("request ASR dialogID[%s]", grammar.c_str());
    std::string newGrammar = changeGrammarIDForAsr(grammar);
    VR_LOGI("request ASR grammarId = [%s], new grammarId= [%s]", grammar.c_str(), newGrammar.c_str());

    _asrReturnBeepPlayed = false;
    if (NULL == _asrEngine) {
        VR_LOGE("Asr engine is null");
        responseAsrError();
        return;
    }

    // REVIEW: topmenu not recog song when toal > 15000, only for CN
    if (_bDisableTopmenuSong) {
        if (_songDisableSceneFlag) {
            _songDisableSceneFlag = false;
            N_Vr::N_Asr::C_Request_Param_Set songUnLoadParam;
            songUnLoadParam.m_e_Param_Type = E_Param_Type_Resource_Song;
            songUnLoadParam.m_i_Value = 0;
           _asrEngine->Param_Set(songUnLoadParam);
           VR_LOGI("Secne_UnLoad_Song_Resource_Song");
        }
        if (newGrammar == "media_music_speak_song_name") {
            _songDisableSceneFlag = true;
            N_Vr::N_Asr::C_Request_Param_Set songLoadParam;
            songLoadParam.m_e_Param_Type = E_Param_Type_Resource_Song;
            songLoadParam.m_i_Value = 1;
            _asrEngine->Param_Set(songLoadParam);
            VR_LOGI("Secne_Load_Song_Resource_Song");
        }
    }

    if (_asrIsRunning) {
        // normal case can't enter
        VR_LOGE("ASR Engine is running!!!");
        return;
    }

    // need wait returnbeep done
    if (0 == m_lastReturnBeep) {
        VR_LOGD("return beep done");
        _continueAsrByReturnBeep = false;
    }
    else {
        VR_LOGD("need wait return beep done");
        _continueAsrByReturnBeep = true;
        return;
    }

    _audioInSource->setAudioMode(VC_AUDIO_MODE_NORMAL);
    _audioInSource->Prepare();
    _lastStartBeepSeqId = reqPlayBeep(START_BEEPPATH);

    C_Request_Recognize req;
    req.m_string_Id_Dialog = newGrammar;
    // req.m_string_Id_Mode   = g_string_Iss_Sr_Mode_Mix; // Local | Mix | Cloud
    req.m_string_Id_Mode   = g_string_Iss_Sr_Mode_Local;
    m_strAsrMode = "local";

    if (m_pConfigureIF->getHybridVRFlag() && ("NoError" == m_strCommunicationStatus)) {
        req.m_string_Id_Mode   = g_string_Iss_Sr_Mode_Mix;
        m_strAsrMode = "mix";
        if (!m_hasGotCenterVRUrl) {
            std::string reqVrUrl = "<action agent=\"communication\" op=\"queryCenterVRURL\"></action>";
            requestAction(reqVrUrl);
        }
        // get vehicel long lat
        int iRoadKind;
        bool bDemoOn;
        m_pConfigureIF->getLocateInfo(iRoadKind, m_iVehiclePosLong, m_iVehiclePosLat, bDemoOn);
        double lon = (double)m_iVehiclePosLong / (3600 * 256);
        double lat = (double)m_iVehiclePosLat  / (3600 * 256);

        // set vechile postion's long
        N_Vr::N_Asr::C_Request_Param_Set paramLon;
        paramLon.m_e_Param_Type = E_Param_Type_Longitude;
        std::stringstream ssLon;
        ssLon << lon;
        ssLon >> paramLon.m_string_Value;
        _asrEngine->Param_Set(paramLon);

        // set vechile postion's lat
        N_Vr::N_Asr::C_Request_Param_Set paramLat;
        paramLat.m_e_Param_Type = E_Param_Type_Latitude;
        std::stringstream ssLat;
        ssLat << lat;
        ssLat >> paramLat.m_string_Value;
        _asrEngine->Param_Set(paramLat);

        VR_LOGI("request ASR vehicel pos is [%d,%d], [%s,%s]", m_iVehiclePosLong, m_iVehiclePosLat, paramLon.m_string_Value.c_str(), paramLat.m_string_Value.c_str());
    }

    if (_addressForAsr.s_bAddress) {
        switch (_addressForAsr.s_enAddressType) {
        case AddressType::address_state:
            VR_LOGI("request VR province or city is null");
            break;
        case AddressType::address_city:
            VR_LOGI("request VR for address is %d, json string is %s", _addressForAsr.s_enAddressType, _addressForAsr.s_strJsonProv.c_str());
            req.m_string_Custom = _addressForAsr.s_strJsonProv;
            break;
        case AddressType::address_district:
            VR_LOGI("request VR for address is %d, json string is %s", _addressForAsr.s_enAddressType, _addressForAsr.s_strJsonProvCity.c_str());
            req.m_string_Custom = _addressForAsr.s_strJsonProvCity;
            break;
        default:
            break;
        }
    }
    _addressForAsr.s_bAddress = false;

    // REVIEW:
    // for address bug
    if ("" == req.m_string_Custom) {
        if ("navi_speak_city_{country}" == _grammarStr) {
            req.m_string_Custom = _addressForAsr.s_strJsonProv;
        }
        else if ("navi_speak_district_{country}" == _grammarStr) {
            req.m_string_Custom = _addressForAsr.s_strJsonProvCity;
        }
    }

    req.m_string_Id_Party = "Origin";
    req.m_spo_Audio_In = _audioInSource;
    req.m_function_On_Event_Phase = boost::bind(&VR_CNDialogEngine::onAsrPhase, this, _1);
    req.m_function_On_Event_Notify = boost::bind(&VR_CNDialogEngine::onAsrNotify, this, _1);
    req.m_function_On_Event_Result = boost::bind(&VR_CNDialogEngine::onAsrResult, this, _1);
    req.m_b_Measure_Time = false;

    VR_LOGP("ASR recognize start");
    m_isAsrSpeechEndRecv = false;
    E_Result ret = _asrEngine->Recognize_Start(req);
    if (ret == E_Result::E_Result_Succeeded) {
        _asrIsRunning = true;
        VR_LOGI("Recognize start success");
    }
    else {
        _audioInSource->UnPrepare();
        responseAsrError();
        VR_LOGE("Recognize start failed");
    }
}

std::string VR_CNDialogEngine::namelistToStr(uscxml::Event& reqCopy)
{
    auto namelist = reqCopy.getNameList();
    VR_LOGD("namelist data.size=[%d]", namelist.size());
    if (namelist.empty()) {
        RETURN("");
    }

    const uscxml::Data& data = namelist.begin()->second;
    if (data.compound.empty()) {
        RETURN("");
    }

    const uscxml::Data& innerdata = data.compound.begin()->second;
    RETURN(innerdata.getAtom());
}

std::string VR_CNDialogEngine::namelistToStr(uscxml::Event& reqCopy, const std::string& dataName)
{
    auto nameList = reqCopy.getNameList();
    VR_LOGD("namelist data.size=[%d]", nameList.size());
    auto finded = nameList.find(dataName);
    if (finded == nameList.end()) {
        RETURN("");
    }

    const uscxml::Data& data = finded->second;
    if (data.empty()) {
        RETURN("");
    }

    const uscxml::Data& innerdata = data.compound.begin()->second;
    RETURN(innerdata.getAtom());
}

void VR_CNDialogEngine::parserVRState(pugi::xml_node& doc)
{
    VR_LOGD_FUNC();
    std::string state = doc.select_node("//state").node().child_value();

    // -------------parser tag------------------
    std::string tag = doc.select_node("//tag").node().child_value();
    std::string vr_StateSecond = _deDataManager->getState2Text(tag);
    pugi::xml_node tagNode = doc.select_node("//tag").node();
    tagNode.first_child().set_value(vr_StateSecond.c_str());


    if (state == "idle"
        || state == "prompt"
        || state == "promptWithBargein"
        || state == "promptPlaying"
        || state == "listening"
        || state == "paused") {
        // -------------parser prompt------------------
        std::string vr_StateSecond2 = _deDataManager->getState2Text(tag);
        pugi::xml_node promptNode = doc.select_node("//prompt").node();
        promptNode.first_child().set_value(vr_StateSecond2.c_str());

        pugi::xml_node resultNode = doc.select_node("//result").node();
        resultNode.text().set("");
    }
    else if (state == "processing"
        || "speechResult" == state) {
        pugi::xml_node promptNode = doc.select_node("//prompt").node();
        promptNode.text().set("");
    }

    // add for engineType 20160108 begin
    pugi::xml_node engineTypeNode = doc.select_node("//engineType").node();
    if ("mix" == m_strAsrMode) {
        engineTypeNode.first_child().set_value("server");
    }
    else {
        engineTypeNode.first_child().set_value("local");
    }
    // add for engineType 20160108 end
}


VoiceList<std::string>::type VR_CNDialogEngine::parserPrompt(pugi::xml_node& doc)
{
    VR_LOGD_FUNC();
    // construct promptStr
    std::string promptStr;
    std::string promptId = doc.select_node("//promptId").node().child_value();
    char* source = const_cast<char*>(promptId.c_str());
    char* p = strtok(source, ",");
    VoiceList<std::string>::type txtList;
    while (p != NULL) {
        std::string tmpStr = _deDataManager->getPrompts(p);
        // _dicData.getCompound()[p];
        if (tmpStr.empty()) {
            tmpStr = p;
            VR_LOGE("cant not find promptId,promptStr=%s", tmpStr.c_str());
        }
        txtList.push_back(tmpStr);
        promptStr.append(tmpStr);
        p = strtok(NULL, ",");
    }

    static const std::string s_naviTags[] = { "input full address", "POI", "City", "POI name", "POI Category",
                                              "street", "city", "state" };
    static const std::string s_ttsNumberSwitchOn  = "[n1]";
    static const std::string s_ttsNumberSwitchOff = "[n0]";
    static const std::string s_ttsNaviSwitchOn    = "[k1]";
    static const std::string s_ttsNaviSwitchOff   = "[k0]";
    VoiceMap<std::string, std::string>::type dicMap;
    pugi::xpath_node_set xpathNodes = doc.select_nodes("//param");
    pugi::xpath_node_set::iterator xpathNode = xpathNodes.begin();
    while (xpathNode != xpathNodes.end()) {
        std::string kk1 = xpathNode->node().attribute("key").as_string();
        std::string vv1 = xpathNode->node().attribute("value").as_string();
        // add for phone TTS tag 20160612 15-311-1-04536 begin
        if (doc.select_node("//tts").node() != NULL) {
            if ("number" == kk1) {
                vv1 = s_ttsNumberSwitchOn + vv1 + s_ttsNumberSwitchOff;
            }
            else if (std::find(std::begin(s_naviTags), std::end(s_naviTags), kk1) != std::end(s_naviTags)) {
                vv1 = s_ttsNaviSwitchOn   + vv1 + s_ttsNaviSwitchOff;
            }
            else {
                VR_LOGD("No code is required for this branch.");
            }
        }
        // add end
        dicMap[kk1] = vv1;
        ++xpathNode;
    }

    // traver txtList and replace key with value
    VoiceList<std::string>::iterator it = txtList.begin();
    VoiceList<std::string>::type retTxtList;
    while (it != txtList.end()) {
        std::string promptSingle = *it;
        parserPromptReplaceKeyWithValue(promptSingle, dicMap);
        VR_LOGD("promptSingle = %s", promptSingle.c_str());
        retTxtList.push_back(promptSingle);
        ++it;
    }

    parserPromptReplaceKeyWithValue(promptStr, dicMap);
    pugi::xml_node promptNode = doc.select_node("//promptArgs").node().parent();
    while (NULL != doc.select_node("//promptArgs")) {
        doc.select_node("//promptArgs").parent().remove_child("promptArgs");
    }
    promptNode.remove_child("prompt");
    promptNode.append_child("prompt");
    promptNode.select_node("//prompt").node().append_child(pugi::node_pcdata).set_value(promptStr.c_str());
    return retTxtList;
}


void VR_CNDialogEngine::parserPromptReplaceKeyWithValue(std::string& promptStr, VoiceMap<std::string, std::string>::type& dicMap)
{
    // modify promptStr
    int st = promptStr.find_first_of('{');
    while (st >= 0) {
        std::string remainStr = promptStr.substr(st + 1);
        int ed = remainStr.find_first_of('}');
        if (ed > 0) {
            std::string k1 = promptStr.substr(st + 1, ed);
            promptStr.replace(st, ed + 2, dicMap[k1]);
        }
        else {
        }
        st = promptStr.find_first_of('{');
    }
}

void VR_CNDialogEngine::loadHandleEventFuncMap()
{
    // events sent by scxml will be handled in interpreter thread
    _eventHandles["back"] = &VR_CNDialogEngine::handleBack;
    _eventHandles["doBack"] = &VR_CNDialogEngine::handleDoBack;
    _eventHandles["doUpdateDataModel"] = &VR_CNDialogEngine::handleDoUpdateDataModel;
    _eventHandles["receiveOpResult"] = &VR_CNDialogEngine::handleReceiveOpResult;
    _eventHandles["preInit"] = &VR_CNDialogEngine::handlePreUpdateData;
    // events sent by scxml will be handled in controller thread
    _asyncEventHandles["initInterpreter"] = &VR_CNDialogEngine::handlePreInitInterpreter;
    _asyncEventHandles["asr"] = &VR_CNDialogEngine::handleAsr;
    _asyncEventHandles["tts"] = &VR_CNDialogEngine::handleTTS;
    _asyncEventHandles["ttsWithAsr"] = &VR_CNDialogEngine::handleTTSWithAsr;
    _asyncEventHandles["display"] = &VR_CNDialogEngine::handleDisplay;
    _asyncEventHandles["fetchItem"] = &VR_CNDialogEngine::handleFetchItem;
    _asyncEventHandles["closeSession"] = &VR_CNDialogEngine::handleCloseSession;
    _asyncEventHandles["resendAsrEvent"] = &VR_CNDialogEngine::handleResendAsrEvent;
    _asyncEventHandles["changeAgent"] = &VR_CNDialogEngine::handleChangeAgent;
    _asyncEventHandles["backAgent"] = &VR_CNDialogEngine::handleBackAgent;
    _asyncEventHandles["interrupt"] = &VR_CNDialogEngine::handleInterrupt;
    _asyncEventHandles["hardKey"] = &VR_CNDialogEngine::handleReturnKeyOrButton;
    _asyncEventHandles["buttonPressed"] = &VR_CNDialogEngine::handleReturnKeyOrButton;
    _asyncEventHandles["action"] = &VR_CNDialogEngine::handleAction; // REVIEW: other event
    _asyncEventHandles["asrStartover"] = &VR_CNDialogEngine::handleAsrStartover;
    _asyncEventHandles["asrImmediately"] = &VR_CNDialogEngine::handleAsrImmediately;
    _asyncEventHandles["popCurrentAgentStacks"] = &VR_CNDialogEngine::handlePopCurrentAgentStacks;
    _asyncEventHandles["startOver"] = &VR_CNDialogEngine::handleStartOverEvent;
    _asyncEventHandles["initAsrFactory_Event"] = &VR_CNDialogEngine::handleInitAsr;
    _asyncEventHandles["initInterpretes_Event"] = &VR_CNDialogEngine::handleInitInterpreter;
    _asyncEventHandles["getWebSearchResult"] = &VR_CNDialogEngine::handleGetWebSearchResult;
    _asyncEventHandles["getCommunicationStatus"] = &VR_CNDialogEngine::handleGetCommunicationStatus;
    _asyncEventHandles["quitVRApp"] = &VR_CNDialogEngine::handleQuitVRApp;
    _asyncEventHandles["updateGrammarCategoryFinish"] = &VR_CNDialogEngine::handleUpdateGrammarCategoryFinish;
    _asyncEventHandles["updateGrammarCategoryFailed"] = &VR_CNDialogEngine::handleUpdateGrammarCategoryFailed;
    _asyncEventHandles["updateGrammarFinish"] = &VR_CNDialogEngine::handleUpdateGrammarFinish;
    _asyncEventHandles["grammarSpaceNotEnough"] = &VR_CNDialogEngine::handleGrammarSpaceNotEnough;
    _asyncEventHandles["deleteMediaGrammar"] = &VR_CNDialogEngine::handleDeleteMediaGrammar;
    _asyncEventHandles["saveGrammarFile"] = &VR_CNDialogEngine::handleSaveGrammarFile;
    _asyncEventHandles["agentLoadSuccessFirstTime"] = &VR_CNDialogEngine::handleAgentLoadedSuccessFirstTime;
    _asyncEventHandles["agentLoadFailFirstTime"] = &VR_CNDialogEngine::handleAgentLoadedFailFirstTime;
    _asyncEventHandles["changeSourceByName"] = &VR_CNDialogEngine::handleChangeSourceByName;

    // internal events
    _asyncEventHandles["sendMessage_Event"] = &VR_CNDialogEngine::handleSendMessage;

    // ASR events
    _asyncEventHandles[VR_ASR_EVENT_NAME] = &VR_CNDialogEngine::handleAsrEvent;
}

void VR_CNDialogEngine::handleReceiveOpResult(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    _interManager->receiveOpResult();
}

void VR_CNDialogEngine::handlePopCurrentAgentStacks(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    clearSameAgentState();
}

void VR_CNDialogEngine::handleDoUpdateDataModel(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    _interManager->updateGlobalStates(); // update global state
}

void VR_CNDialogEngine::handleTTS(uscxml::Event& reqCopy)
{
    playTTS(reqCopy, false);
    return;
}

// _continueAsr should be setted to true if second parameter "continueAsr" setted to true
void VR_CNDialogEngine::playTTS(uscxml::Event& reqCopy, bool continueAsr)
{
    VR_LOGD_FUNC();
    std::string xmlStr = namelistToStr(reqCopy, MODEL_TTS);
    if (xmlStr.empty()) {
        VR_LOGE("$modelTTS is empty");
    }
    else {
        pugi::xml_document doc;
        doc.load_string(xmlStr.c_str());
        VoiceList<std::string>::type txtList = parserPrompt(doc);

        int promptLevel = m_pConfigureIF->getVRPromptLevel();
        if (0 == promptLevel) {
            bool helpType = doc.select_node("//helpType").node().text().as_bool();
            if (!helpType) {
                // don't paly tts, simulate done.playTts event to interpeter
                if (continueAsr) {
                    VR_LOGI("start VR without prompt, grammar[%s]", _grammarStr.c_str());
                    _continueAsr = false;
                    requestVR(_grammarStr);
                }
                else {
                    _interManager->receiveEvent("done.playTts", "");
                }
                return;
            }
        }
        // std::string prompt = doc.select_node("//prompt").node().child_value();
        reqPlayTTS(txtList);
    }
}

// REVIEW: asr result has return, this startOver drop
void VR_CNDialogEngine::handleAsrStartover(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    if (!_asrEngine) {
        VR_LOGE("asr engine is null");
        return;
    }
    m_isCanceledByDE = true;
    _asrEngine->Recognize_Cancel();
    requestVR(_grammarStr);
    // cancel asr,receive reply, start asr
}

void VR_CNDialogEngine::handleAsrImmediately(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    // stop tts, receive reply, start asr

    bool waitStopTts = stopTTS(reqCopy);
    if (!waitStopTts) {
        VR_LOGD("don't need wait stop tts");
        _continueAsr = false;
        requestVR(_grammarStr);
    }
    else {
        VR_LOGD("need wait stop tts");
        _continueAsr = true;
    }
}

bool VR_CNDialogEngine::stopTTS(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    if (isTtsCrash()) {
        VR_LOGD("ttscrash don't need play or stop");
        return false;
    }
    if (0 != _lastTtsSeqId) {
        std::string stopTTSMsg;
        m_pDEMessageBuilder->buildStopTTSAction(stopTTSMsg);
        _lastStopTtsSeqId = requestAction(stopTTSMsg);
        VR_LOGD("stopTTSMsg send , _lastStopTtsSeqId = %d", _lastStopTtsSeqId);
        return true;
    }
    return false;
}

void VR_CNDialogEngine::interruptActionResult()
{
    VR_LOGD_FUNC();
    if (!_lastStopTtsSeqId && !_asrIsRunning && !_lastStartBeepSeqId && !m_lastReturnBeep && _interrputRecv) {
        _interManager->receiveEvent("done.interrupt", "");
        VR_LOGD("done.interrupt send");
        _interrputRecv = false;
    }
    else {
        VR_LOGD("done.interrupt can't send, lastStopTtsSeqId = %d, asrIsRunning = %d, lastStartBeepSeqId = %d, m_lastReturnBeep = %d",
                _lastStopTtsSeqId, _asrIsRunning, _lastStartBeepSeqId, m_lastReturnBeep);
    }
}

void VR_CNDialogEngine::handleInterrupt(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    _continueAsr = false;
    _needBargein = false; // REVIEW: remove
    _interrputRecv = true;
    _continueAsrByReturnBeep = false;
    if (_asrEngine && _asrIsRunning) {
        m_isCanceledByDE = true;
        _asrEngine->Recognize_Cancel();
    }
    else {
        VR_LOGD("asrEngine is null or asrIsRunning is false");
    }
    stopTTS(reqCopy);

    interruptActionResult();
}

void VR_CNDialogEngine::handleAsr(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    _needBargein = false;
    _continueAsr = false;
    doHandleAsr(reqCopy);
}

void VR_CNDialogEngine::doHandleAsr(uscxml::Event& reqCopy)
{
    parseAsrGrammar(reqCopy);
    requestVR(_grammarStr);
}

void VR_CNDialogEngine::parseAsrGrammar(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    std::string xmlStr = namelistToStr(reqCopy, MODEL_ASR);
    if (xmlStr.empty()) {
        VR_LOGE("$modelAsr is empty");
    }
    else {
        pugi::xml_document doc;
        doc.load_string(xmlStr.c_str());
        pugi::xpath_node languageNode = doc.select_node("//language");
        std::string languageStr = languageNode.node().child_value();
        VR_LOGI("languageStr [%s] has not used", languageStr.c_str());
        pugi::xpath_node grammarNode = doc.select_node("//grammar");
        std::string grammarStr = grammarNode.node().child_value();
        _grammarStr = grammarStr;
    }
}

// add for T1 CN disk hints
bool VR_CNDialogEngine::isShowDiskHints(const std::string &stateMsg)
{
    pugi::xml_document doc;
    doc.load_string(stateMsg.c_str());

    pugi::xml_node itemNode = doc.select_node("/data/g").node();
    for (pugi::xml_node_iterator it = itemNode.begin(); it != itemNode.end(); ++it) {
        std::string key(it->attribute("key").as_string());
        std::string value(it->attribute("value").as_string());

        if ("MEDIA_DECKLESS_MODEL" == key) {
            if ("DECKLESS" == value) {
                VR_LOGD("deckless");
                return false;
            }
            else if ("NOT_DECKLESS" == value) {
                VR_LOGD("not deckless");
                return true;
            }
        }
    }
    return true;
}

void VR_CNDialogEngine::handleDisplay(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    // get data <diaplsy> from data model and send out as action
    std::string displayStrScxml = namelistToStr(reqCopy);
    pugi::xml_document docScxml;
    docScxml.load_string(displayStrScxml.c_str());
    pugi::xml_node displayNodeScxml = docScxml.select_node("//display").node();
    std::string contentTypeStrScxml = displayNodeScxml.attribute("content").as_string();
    if (contentTypeStrScxml == "ScreenDisplay") {
        VR_LOGP("ScreenDisplay:[%s]", displayStrScxml.c_str());

        pugi::xml_node iPodDisplay = docScxml.select_node("//IpodDisplay").node();
        if (iPodDisplay) {
            std::string number = m_isIpod ? "1" : "0";
            iPodDisplay.text().set(number.c_str());
        }

        // display need parserPrompt and addItemsToDisplayList
        parserPrompt(docScxml);
        // add items
        pugi::xml_node hintsNodeScxml = docScxml.select_node("//hints/list[@id]").node();
        if (NULL != hintsNodeScxml) {
            // add for 16-209-1-00521
            bool isShowClimateHints = true;
            if ("false" == stateClimateMap["CLIMATE_BASIC_ACTIVE"]) {
                isShowClimateHints = false;
            }
            // add for deckless
            std::string stateMsg;
            _dataAccessorManager->getUpdateState(stateMsg);
            bool isShowDisk = isShowDiskHints(stateMsg);
            _dataProcessor.addItemsToHintsDispaly(hintsNodeScxml, true, m_pDECommonIF->getHybridVRFlag(), isShowClimateHints, isShowDisk);
        }
        else {
            pugi::xml_node selectListNode = docScxml.select_node("//selectList/list[@id]").node();
            if (NULL != selectListNode) {
                _dataProcessor.addItemsToDispaly(selectListNode);
            }
            else {
                VR_LOGD("this display for screenDispaly has no hints or selectList");
            }
        }
    }
    else if (contentTypeStrScxml == "ShowPopupMessage") {
        VR_LOGI("this display content is ShowPopupMessage and need parserPrompt");
        // parser display
        parserPrompt(docScxml);
    }
    else if (contentTypeStrScxml == "VRState") {
        VR_LOGI("this display content is VRState and need parser tag and prompt");
        // tag , prompt
        parserVRState(docScxml);
    }
    else {
        VR_LOGI("this display is not ScreenDisplay, need't parserPrompt and addItemsToDisplayList");
    }
    std::stringstream oss;
    displayNodeScxml.print(oss);
    std::string resultStrScxml;
    resultStrScxml = oss.str();
    requestAction(resultStrScxml);
}

void VR_CNDialogEngine::handleFetchItem(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    std::string nameListStr = namelistToStr(reqCopy);
    pugi::xml_document doc;
    doc.load_string(nameListStr.c_str());
    std::string listId = doc.select_node("//listId").node().child_value();
    std::string indexStr = doc.select_node("//index").node().child_value();
    VR_LOGD("listId = %s, index = %s", listId.c_str(), indexStr.c_str());
    int index = atoi(indexStr.c_str());
    pugi::xml_node listNode = _dataProcessor.getNode(listId);
    pugi::xml_document docRes;
    docRes.append_copy(listNode);
    pugi::xpath_node_set itemNodeSet = docRes.select_nodes("/list/items/item");
    // pugi::xpath_node_set itemNodeSet = listNode.select_nodes("//item");
    std::string itemStr;
    int count = itemNodeSet.size();
    VR_LOGD("fetchItem size = %d", count);
    if (index < count) {
        pugi::xml_node itemNode = itemNodeSet[index].node();
        std::stringstream oss;
        itemNode.print(oss);
        itemStr = oss.str();
        _interManager->receiveEvent("done.fetchItem", itemStr);
    }
    else {
        _interManager->receiveEvent("failed.fetchItem", itemStr);
        VR_LOGE("the index in FetchItem has over the items.size=%d", itemNodeSet.size());
    }
}

// It only be called by interpreter
void VR_CNDialogEngine::handleQuitVRApp(uscxml::Event& evt)
{
    VR_LOGD_FUNC();
    std::string msg;
    m_pDEMessageBuilder->buildQuitVRAppDisplay(msg);
    requestAction(msg);
}

void VR_CNDialogEngine::handleUpdateGrammarCategoryFinish(uscxml::Event& evt)
{
    std::string category = evt.getContent();
    _dataAccessorManager->onUpdateGrammarCategoryFinish(category);
}

void VR_CNDialogEngine::handleUpdateGrammarCategoryFailed(uscxml::Event& evt)
{
    std::string sourceID = evt.getSendId();
    std::string size = evt.getContent();
    _dataAccessorManager->onUpdateGrammarCategoryFailed(sourceID, size);
}

void VR_CNDialogEngine::handleUpdateGrammarFinish(uscxml::Event& evt)
{
    _dataAccessorManager->onUpdateGrammarFinish();
}

void VR_CNDialogEngine::handleGrammarSpaceNotEnough(uscxml::Event& evt)
{
    std::string infoMsg = evt.getContent();
    _dataAccessorManager->handleGrammarSpaceNotEnough(infoMsg);
}

void VR_CNDialogEngine::handleDeleteMediaGrammar(uscxml::Event& evt)
{
    std::string deviceID = evt.getSendId();
    std::string infoMsg = evt.getContent();
    _dataAccessorManager->onDeleteMediaGrammar(deviceID, infoMsg);
}

void VR_CNDialogEngine::handleSaveGrammarFile(uscxml::Event& evt)
{
    std::string sourceID = evt.getSendId();
    _dataAccessorManager->onSaveGrammarToFile(sourceID);
}

void VR_CNDialogEngine::handleAgentLoadedSuccessFirstTime(uscxml::Event& evt)
{
    if (LOAD_NONE == m_agentLoadStateFirstTime) {
        m_agentLoadStateFirstTime = LOAD_SUCCESS;
        notifyStartFinishAfterCheck();
    }
}

void VR_CNDialogEngine::handleAgentLoadedFailFirstTime(uscxml::Event& evt)
{
    if (LOAD_NONE == m_agentLoadStateFirstTime) {
        m_agentLoadStateFirstTime = LOAD_FAIL;
        notifyStartFinishAfterCheck();
    }
}

void VR_CNDialogEngine::quitByError(uscxml::Event& evt)
{
    VR_LOGD_FUNC();

    handleQuitVRApp(evt);

    closeSession(evt);
}

void VR_CNDialogEngine::resourceStateChange(std::string type, ResourceState value)
{
    VR_LOGD_FUNC();
    _resourceState = value;
    VR_LOGD("type = %s, _resourceState = %d", type.c_str(), _resourceState);
    std::stringstream oss;
    oss << static_cast<int>(_resourceState);
    std::string resStr = oss.str();
    std::string returnMsg = "<action agent=\"destatus\"  op=\"notifyResourceState\">"
                            "<resourceStateType>" + type + "</resourceStateType>"
                            "<resourceState>" + resStr + "</resourceState>"
                            "</action>";

    requestAction(returnMsg);
    VR_LOGD("_resourceStateChange Send requestAction : %s", returnMsg.c_str());
}

// Interpreter call when dialog is complete
void VR_CNDialogEngine::handleCloseSession(uscxml::Event& reqCopy)
{
    // Need async wait 'end' beep playback complete
    // _lastEndBeepSeqId = reqPlayBeep(END_BEEPPATH);

    closeSession(reqCopy);
}

bool VR_CNDialogEngine::isDEQuiting()
{
    if (SESSION_QUITING == _sessionState || SESSION_QUITING_WAITING == _sessionState) {
        return true;
    }
    else {
        return false;
    }
}

void VR_CNDialogEngine::closeSession(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    if (!isDEQuiting()) {
        _sessionState = SESSION_QUITING;
        VR_LOGD("_sessionState is SESSION_QUITING");

        resourceStateChange("engine", ResourceState::GRAMMAR_NOT_READY);

        // clear agent statck data
        clearTempData();

        // stop current interpreter
        _interManager->stopAgent();

        // Cancel asr, stop TTS prompt
        handleInterrupt(reqCopy);

        // Send 'quit' state to UI
        std::string msg;
        bool HybridVRFlag = false;
        if ("mix" == m_strAsrMode) {
            HybridVRFlag = true;
        }
        m_pDEMessageBuilder->buildVRStateQuitDisplay(msg, HybridVRFlag);
        requestAction(msg);
    }
    return;
}

std::string VR_CNDialogEngine::processPoiListSelect(std::string xmlPOIStr)
{
    pugi::xml_document doc;
    VR_LOGD("select result enter [%s]", xmlPOIStr.c_str());
    if (!doc.load_string(xmlPOIStr.c_str())) {
        VR_LOGE("load_string error!");
        std::string tmp = "<list id=\"_checkHidePoiResult\">"
                               "<header>"
                                    "<pageSize>0</pageSize>"
                                    "<startIndex>0</startIndex>"
                                    "<count>0</count>"
                                    "<webSearch>false</webSearch>"
                               "</header>"
                               "<item>"
                               "</item>"
                          "</list>";
        return tmp;
    }

    std::string categoryId = "";

    pugi::xml_node countNode = doc.select_node("//count").node();
    int count = countNode.text().as_int();

    pugi::xml_node asrListNode = doc.select_node("//asrResult/list").node();
    pugi::xml_node dmListNode = doc.select_node("//poiResult/list").node();

    pugi::xpath_node_set listItems = asrListNode.select_nodes(".//item");
    pugi::xpath_node_set::iterator listItem = listItems.begin();

    pugi::xpath_node_set dmListItemsNodes = dmListNode.select_nodes(".//poi_category_id");
    pugi::xpath_node_set::iterator dmListItemNode = dmListItemsNodes.begin();

    VoiceVector<std::string>::type dmvector;

    while (dmListItemNode != dmListItemsNodes.end()) {
        dmvector.push_back(dmListItemNode->node().text().as_string());
        ++dmListItemNode;
    }

    while (listItem != listItems.end()) {
        categoryId = listItem->node().select_node(".//POI_category_id").node().text().as_string();
        if (std::find(dmvector.begin(), dmvector.end(), categoryId) == dmvector.end()) {
            VR_LOGD("not find %s, need remove", categoryId.c_str());
            listItem->node().parent().remove_child(listItem->node());
            count--;
        }
        ++listItem;
    }

    countNode.text().set(count);
    asrListNode.attribute("id").set_value("_checkHidePoiResult");
    dmListNode.parent().remove_child(dmListNode);

    _dataProcessor.updateListByActionResult(asrListNode);

    std::stringstream oss;
    asrListNode.print(oss);
    std::string tep = oss.str();
    VR_LOGD("select result out = %s", tep.c_str());
    return tep;
}

void VR_CNDialogEngine::handleFetchData(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    std::string eventName = reqCopy.getName();
    std::string xmlStr = namelistToStr(reqCopy);
    std::string listStr;
    if (eventName == "_getHints" || eventName == "_getMoreHints") {
        pugi::xml_document doc;
        doc.load_string(xmlStr.c_str());
        pugi::xml_node agentNameNode =  doc.select_node("//agentName").node();
        std::string agentName = agentNameNode.child_value();
        listStr = _dataProcessor.getHintsCap(eventName, agentName);
    }
    else if (eventName == "_checkHidePoiResult") {
        listStr = processPoiListSelect(xmlStr.c_str());
        VR_LOGD("list select result = %s", listStr.c_str());
    }
    else {
        _dataAccessorManager->getInfo(eventName, xmlStr, listStr);
        VR_LOG_TO_FILE("getInfo.eventName", eventName);
        VR_LOG_TO_FILE("getInfo.sendMsg", xmlStr);
        VR_LOG_TO_FILE("getInfo.recMsg", listStr);

        pugi::xml_document retDoc;
        retDoc.load_string(listStr.c_str());
        if (eventName == "_sendVoiceID") { // REVIEW:
            pugi::xml_document retDoc;
            retDoc.load_string(xmlStr.c_str());
            _addressForAsr.s_bAddress = true;
            std::string strType = retDoc.select_node("//type").node().text().as_string();
            if ("CITY" == strType) {
                _addressForAsr.s_enAddressType = AddressType::address_city;
                std::string strProvince = retDoc.select_node("//info/addressInfo/zone").node().text().as_string();
                if ("''" == strProvince || strProvince.empty()) {
                    _addressForAsr.s_strJsonProv = "";
                }
                else {
                    _addressForAsr.s_strJsonProv = (boost::format("{\"province\": \"%1%\"}") % strProvince).str();
                }
            }
            else if ("DISTRICT" == strType) {
                _addressForAsr.s_enAddressType = AddressType::address_district;
                std::string strProvince = retDoc.select_node("//info/addressInfo/zone").node().text().as_string();
                std::string strCity = retDoc.select_node("//info/addressInfo/city").node().text().as_string();
                if ("''" == strProvince || strProvince.empty() || "''" == strCity || strCity.empty()) {
                    _addressForAsr.s_strJsonProvCity = "";
                }
                else {
                    _addressForAsr.s_strJsonProvCity = (boost::format("{\"province\": \"%1%\", \"city\": \"%2%\"}") % strProvince % strCity).str();
                }
            }
            else {
                _addressForAsr.s_enAddressType = AddressType::address_state;
            }
        }
        else {
            _dataProcessor.updateListByDataAccessor(retDoc);
        }
    }
    _interManager->notifyOpResult(eventName, listStr);
    // std::string dataName = eventName + "_result";
    // _interManager->assignData(dataName, listStr); // assign data
    // _interManager->receiveEvent("done." + eventName, listStr);
    return;
}

void VR_CNDialogEngine::handleResendAsrEvent(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    VR_LOGI("resendAsrEvent and the eventName:%s\n", _currentIntention.first.c_str());
    VR_LOGI("resendAsrEvent and the eventIntention:%s\n", _currentIntention.second.c_str());
    _interManager->receiveEvent(_currentIntention.first, _currentIntention.second);
}

void VR_CNDialogEngine::handleAction(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child();
    node.set_name("action");
    // check if the eventName = AgentName.eventName, get the AgentName
    std::string evtName = reqCopy.getName().c_str();
    std::string agentName;
    size_t pos = evtName.find('.');
    if (std::string::npos != pos) {
        agentName = evtName.substr(0, pos);
        node.append_attribute("agent").set_value(agentName.c_str());
        evtName = evtName.substr(pos + 1);
    }
    else {
        node.append_attribute("agent").set_value(this->_currentAgent.c_str());
    }
    node.append_attribute("op").set_value(evtName.c_str());
    node.append_attribute("detype").set_value("cnde");
    std::string xmlStr = namelistToStr(reqCopy); // get namelist data
    pugi::xml_document docNamelist;
    docNamelist.load_string(xmlStr.c_str());

    // add for CN navi languageId
    pugi::xpath_node_set nodeSetItems = docNamelist.select_nodes("//languageId");
    const pugi::xpath_node *iter = nodeSetItems.begin();
    for (; iter != nodeSetItems.end(); ++iter) {
        int nSysLang = VR_Configure::convert2SysLang(m_pDECommonIF->getVRLanguage());
        (*iter).node().text().set(std::to_string(nSysLang).c_str());
    }

    pugi::xml_node dataNode = docNamelist;
    while (dataNode.first_child().name() == std::string("data")) {
        dataNode = dataNode.first_child();
    }

    pugi::xml_object_range<pugi::xml_node_iterator> rang = dataNode.children();
    pugi::xml_node_iterator ita = rang.begin();
    while (ita != rang.end()) {
        node.append_copy(*ita);
        ++ita;
    }
    //
//    pugi::xpath_node_set namelistNodes = dataNode.select_nodes("/data/*");
//    pugi::xpath_node_set::iterator itNode = namelistNodes.begin();
//    while (itNode != namelistNodes.end()) {
//        node.append_copy(itNode->node());
//        ++itNode;
//    }

    if ("queryRouteExist" == evtName) {
        VR_LOGD("queryRouteExist recv.");
        std::string resultStr = "<action-result agent=\"" + agentName + "\" op=\"queryRouteExist\" errcode=\"0\"><exist>" + m_routeExistStatus + "</exist></action-result>";
        _interManager->notifyOpResult("queryRouteExist", resultStr, false);
        VR_LOGD("queryRouteExist recv:%s", resultStr.c_str());
        return;
    }

    std::ostringstream oss;
    node.print(oss);
    std::string actionStr = oss.str();
    requestAction(actionStr);
}

void VR_CNDialogEngine::handleChangeSourceByName(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    std::string xmlStr = namelistToStr(reqCopy); // get namelist data
    pugi::xml_document reqDoc;
    if (!reqDoc.load_string(xmlStr.c_str())) {
        VR_LOGE("load changeSourceByName xml failed.");
        changeSourceByNameFailed();
        return;
    }

    std::string uiSourceName = reqDoc.select_node("//sourceName").node().text().as_string();
    std::string mediaSourceName = _dataAccessorManager->getMediaSourceName(uiSourceName);

    if (mediaSourceName.empty()) {
        VR_LOGE("Can not get mediaSourceName by uiSourceName:%s", uiSourceName.c_str());
        changeSourceByNameFailed();
        return;
    }

    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("action");
    node.append_attribute("agent").set_value("media");
    node.append_attribute("op").set_value("changeSourceByName");
    node.append_child("sourceName").text().set(mediaSourceName.c_str());

    std::ostringstream oss;
    node.print(oss);
    std::string actionStr = oss.str();
    requestAction(actionStr);
}

void VR_CNDialogEngine::changeSourceByNameFailed()
{
    VR_LOGD_FUNC();
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("action-result");
    node.append_attribute("agent").set_value("media");
    node.append_attribute("op").set_value("changeSourceByName");
    node.append_attribute("errcode").set_value("false");

    std::ostringstream oss;
    node.print(oss);
    std::string actionFailedResult = oss.str();
    if (_interManager) {
        _interManager->notifyOpResult("changeSourceByName", actionFailedResult, false);
    }
    else {
        VR_LOGD("point of interManager is nullptr.");
    }
}

void VR_CNDialogEngine::handleReturnKeyOrButton(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    std::string evtName = reqCopy.getName();
    std::string xmlStr = namelistToStr(reqCopy); // get namelist data
    pugi::xml_document docNamelist;
    docNamelist.load_string(xmlStr.c_str());
    pugi::xpath_node_set namelistNodes = docNamelist.select_nodes("//keycode");
    pugi::xml_node keyNode = namelistNodes.begin()->node();
    std::string keyCodeStr = keyNode.child_value();

    std::string str = "<event-result><keycode/></event-result>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    pugi::xml_node evtResultNode = doc.first_child();
    evtResultNode.append_attribute("name").set_value(evtName.c_str());
    pugi::xml_node keyCodeNode = evtResultNode.first_child();
    keyCodeNode.append_attribute("value").set_value(keyCodeStr.c_str());
    std::stringstream oss;
    doc.print(oss);
    requestAction(oss.str());
}

void VR_CNDialogEngine::handleTTSWithAsr(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    _needBargein = false; // REVIEW: remove
    _continueAsr = true;
    parseAsrGrammar(reqCopy);
    playTTS(reqCopy, true); // wait for done.playTts,then handleAs
    return;
}

void VR_CNDialogEngine::requestService(const uscxml::Event& interalEvent)
{
    VR_LOGD_FUNC();
    _currentEvent = uscxml::Event(interalEvent);
    uscxml::Event evt = uscxml::Event(interalEvent);
    std::string eventName = _currentEvent.getName();
    VR_DUMP_TESTCASE("request:", eventName);
    VR_LOGP("Handle interpreter service request, event=[%s]", eventName.c_str());
    VR_LOG_TO_FILE("invokeName:", eventName);
    VR_LOG_TO_FILE("--------------------", "");

    _dataProcessor.checkListDataCountAndRepair(evt);

    // sync , invoke in uscxml thread
    if (_eventHandles.find(eventName) != _eventHandles.end()) {
        (this->*_eventHandles[eventName])(evt);
    }
    else {  // async
        postEvent(evt);
    }
    return;
}

void VR_CNDialogEngine::postEvent(uscxml::Event& reqCopy)
{
    // internal service and not in the same thread, it has lock insider
    tthread::lock_guard<tthread::recursive_mutex> lock(_mutex);
    uscxml::Event* reqCopyPoint = VR_new uscxml::Event(reqCopy);
    _internalServiceQueue.push(reqCopyPoint);
    _condVar.notify_all();
}

std::string VR_CNDialogEngine::buildInitParm()
{
    VR_LOGD_FUNC();
    pugi::xml_document doc;
    pugi::xml_node preData = doc.append_child("pre");
    pugi::xml_node resendNode = preData.append_child("resendEvent");
    std::string vOut = _resendEvent ? "true" : "false";
    _resendEvent = false;
    resendNode.text().set(vOut.c_str());
    pugi::xml_node transTypeNode = preData.append_child("transType");
    transTypeNode.text().set(_forward_back.c_str());
    pugi::xml_node agentNode = preData.append_child("agent");
    if (_forward_back == FORWARD) {
        agentNode.text().set(_currentAgent.c_str());
    }
    else if (_forward_back == FORBACK) {
        // agentNode.text().set(_lastAgent.c_str());
        int pos = _backStateName.find_first_of('_');
        std::string agentName = _backStateName.substr(0, pos);
        agentNode.text().set(agentName.c_str());
    }
    // for sessionState
    pugi::xml_node sessionStateNode = preData.append_child("state");
    bool sessionState = sessionStateGet();
    VR_LOGD("sessionStateNode val = %d", sessionState);
    if (sessionState) {
        VR_LOGD("sessionStateNode set on");
        sessionStateNode.text().set("on");
    }
    else {
        VR_LOGD("sessionStateNode set off");
        sessionStateNode.text().set("off");
    }

    std::ostringstream oss;
    doc.print(oss);
    std::string preInitStr = oss.str();
    return preInitStr;
}

void VR_CNDialogEngine::handlePreUpdateData(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    this->doStateUpdate("", false);
    _interManager->updateGlobalStates(); // update global state
    uscxml::Event evt;
    evt.setName("initInterpreter");
    postEvent(evt);
}

void VR_CNDialogEngine::handlePreInitInterpreter(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    const std::string preInitStr = buildInitParm();
    _interManager->receiveInitEvent("done.preInit", preInitStr);
}

// should invoke in uscxml thread
void VR_CNDialogEngine::handleDoBack(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    // after change agent, process the really back
    if (!_backStateName.empty()) {
        VR_LOGI("backName = [%s]", _backStateName.c_str());
        _interManager->setTargetName(_backStateName);
        _interManager->receiveEvent("back", "");
        _backStateName = "";
    }
    else {
        VR_LOGE("the _backStateName is null!");
    }
}


void VR_CNDialogEngine::run(void* instance)
{
    VR_LOGD_FUNC();
    VR_CNDialogEngine* dialogEngine = static_cast<VR_CNDialogEngine*>(instance);
    dialogEngine->doRun();
}

void VR_CNDialogEngine::doRun()
{
    while (_isRunning) {
        uscxml::Event* pEvent = NULL;
        {   // limit lock scope
            tthread::lock_guard<tthread::recursive_mutex> lock(_mutex);
            while (_internalServiceQueue.isEmpty()) {
                _condVar.wait(_mutex);
                if (!_isRunning) {
                    return;
                }
            }
            pEvent = _internalServiceQueue.pop(); // pop event
        }

        if (pEvent == NULL) {
             VR_LOGD("Event Ptr Is Null !!!");
            continue;
        }

        step(pEvent);
        delete pEvent;
    }
}

void VR_CNDialogEngine::step(uscxml::Event* pEvent)
{
    VR_LOGD_FUNC();
    if (!isEventNeedHandle(pEvent)) {
        return;
    }
    std::string evtName = pEvent->getName();
    VR_LOGI("async handle event:%s", evtName.c_str());
    if (evtName.empty()) {
        VR_LOGE("receive empty event name");
    }
    else {
        if (evtName[0] == '_') {
            handleFetchData(*pEvent); // fetch data from db
        }
        else {
            if (_asyncEventHandles.find(evtName) == _asyncEventHandles.end()) {
                evtName = "action"; // REVIEW: this evtName is not action, modify to error log
            }
            (this->*_asyncEventHandles[evtName])(*pEvent);
        }
    }
}

void VR_CNDialogEngine::handleChangeAgent(uscxml::Event& evt)
{
    VR_LOGD_FUNC();
    // get agent name and save resendEvent value
    std::string changeAgentData = namelistToStr(evt);
    pugi::xml_document docNamelist;
    docNamelist.load_string(changeAgentData.c_str());
    pugi::xml_node agentNode = docNamelist.select_node("//agent").node();
    std::string agentName = agentNode.text().as_string();
    pugi::xml_node  resendNode = docNamelist.select_node("//resendEvent").node();
    _resendEvent = resendNode.text().as_bool();
    _forward_back = FORWARD;
    VR_LOGI("scxml initiatly change agent to agentName[%s]\n", agentName.c_str());
    VR_LOGI("receive _resendEvent from last agent is [%d]\n", _resendEvent);
    uscxml::Event evtAgentName;
    evtAgentName.setRaw(agentName);
    handleDoChangeAgent(evtAgentName);
}

void VR_CNDialogEngine::handleSendMessage(uscxml::Event& evt)
{
    pugi::xml_document doc;
    std::string eventStr = evt.getXML();
    doc.load_string(eventStr.c_str());
    std::string nodeName = doc.first_child().name();
    int pos = nodeName.find("grammar");
    if (pos != std::string::npos) {
        processGrammarFromDM(eventStr);
    }
    else if (nodeName == "event") {
        processEventFromDM(eventStr);
    }
    else if (nodeName == "action-result") {
        int seqId = std::strtoll(evt.getSendId().c_str(), NULL, 10);
        processActionResultFromDM(eventStr, seqId);
    }
    else {
        VR_LOGE("DM send to DE has error:%s!\n", eventStr.c_str());
    }
}

void VR_CNDialogEngine::handleAsrEvent(uscxml::Event& evt)
{
    VR_LOGD_FUNC();
    std::string evtName = evt.getRaw(); // raw store sub event name
    if (VR_ASR_EVT_LISTEN == evtName) {
      std::string name = "done." + evtName;
      _interManager->receiveEvent(name, "");
    }
    else if (VR_ASR_EVT_BOS == evtName) {
        if (_needBargein) { // REVIEW: remove
            stopTTS(_currentEvent);
        }
        std::string name = "done." + evtName;
        _interManager->receiveEvent(name, "");
    }
    else if (VR_ASR_EVT_EOS == evtName) {
        std::string name = "done." + evtName;
        _interManager->receiveEvent(name, "");
        m_lastReturnBeep = reqPlayBeep(RETURN_BEEPPATH);
        _asrReturnBeepPlayed = true;
    }
    else if (VR_ASR_EVT_RESULT == evtName) {
        m_asrResult = evt.getContent();
        m_receiveAsrResult = true;
    }
    else if (VR_ASR_EVT_END == evtName) {
        // process asr result
        if (m_receiveAsrResult) {       
                m_receiveAsrResult = false;
                handleAsrResult(m_asrResult);
                m_asrResult.clear();
        }
        else if (!m_isCanceledByDE) {
            _asrIsRunning = false;
            quitByError(_currentEvent);
            _audioInSource->UnPrepare();
            VR_LOGE("asr result not recv");
            return;
        }
        m_isCanceledByDE = false;

        if (isTtsCrash()) {
            replyCancelAndReady();
        }

        if (_changeLanguage) {
            languageParamSet();
        }

        if (_fullUpdateNavi) {
            if (_asrEngine) {
                N_Vr::N_Asr::C_Request_Param_Set param;
                param.m_e_Param_Type = E_Param_Type_Resource_Map;
                param.m_i_Value = !_bMapDataPreparing ? 1 : 0;
                _asrEngine->Param_Set(param);
                VR_LOGD("map resource %s", !_bMapDataPreparing ? "loaded +++" : " unloaded ---");
            }
            else {
                VR_LOGD("_asrEngine is null");
            }
            _fullUpdateNavi = false;
        }

        if (_initPersonData) {
            if (_asrEngine) {
                VR_LOGD("before delete engine");
                delete _asrEngine;
                _asrEngine = NULL;
                VR_LOGD("delete engine");
            }
            // clear user data
            C_Engine::Clear_User_Data();
            // init asr
            uscxml::Event evt;
            handleInitAsr(evt);
            resourceStateChange("init", ResourceState::READY);

            _initPersonData = false;
        }
        interruptActionResult();
    }
    else if (VR_ASR_EVT_CONNECT == evtName) {
        VR_LOGD("Mediation: request connect");
    //    m_spCommuMediation->Connect();
    }
    else if (VR_ASR_EVT_DISCONNECT == evtName) {
        VR_LOGD("Mediation: request disconnect");
   //     m_spCommuMediation->Disconnect();
    }
    else {
        VR_LOGE("Unknow ASR event type:%s", evtName.c_str());
    }
}

void VR_CNDialogEngine::handleStartOverEvent(uscxml::Event& evt)
{
    processStartOverEvent();
}

void VR_CNDialogEngine::handleInitInterpreter(uscxml::Event& evt)
{
    VR_LOGD_FUNC();
    if (_interManager != NULL) {
        _interManager->start();
    }
}

void VR_CNDialogEngine::handleInitAsr(uscxml::Event& evt)
{
    VR_LOGD_FUNC();
    if (m_bVDataError) {
        _dataAccessorManager->resetGrammarGeneration();
        if (INIT_NONE == m_asrInitStateFirstTime) {
            m_asrInitStateFirstTime = INIT_SUCCESS;
            notifyStartFinishAfterCheck();
        }
        resourceStateChange("engine", ResourceState::GRAMMAR_NOT_READY);
        return;
    }

    if (NULL == _asrEngine) {
        _dataAccessorManager->resetGrammarGeneration();
        _dataAccessorManager->setGenerateGrammarEnable(false);

        std::string strLanguage = m_pDECommonIF->getVRLanguage();
        C_Request_Factory cAsrFactory;
        cAsrFactory.m_e_Type_Engine = E_Type_Engine_iFlytek;
        if (VR_LANGUAGE_ZH_CN == strLanguage) {
            cAsrFactory.m_string_Id_Language = "mandarin";
        }
        else if (VR_LANGUAGE_EN_US == strLanguage) {
            cAsrFactory.m_string_Id_Language = "english";
        }
        else if (VR_LANGUAGE_ZH_HK == strLanguage) {
            cAsrFactory.m_string_Id_Language = "cantonese";
        }
        else {
            cAsrFactory.m_string_Id_Language = "mandarin";
        }

 //       m_spCommuMediation->SetOnStatusChanged(boost::bind(&VR_CNDialogEngine::networkStatusChanged, this, _1));
        cAsrFactory.m_string_Center_Vr_Url     = m_pConfigureIF->getHybridVRUrl();
        m_hasGotCenterVRUrl = cAsrFactory.m_string_Center_Vr_Url.length() > 0;

        cAsrFactory.m_function_On_Event_Init_Notify = boost::bind(&VR_CNDialogEngine::onAsrInit, this, _1);
        _asrEngine = C_Engine::Factory(cAsrFactory);
        requestGrammarAfterCheck();
        VR_LOGD("asr factory end. CenterVrUrl = [%s]", cAsrFactory.m_string_Center_Vr_Url.c_str());
        if (_asrEngine != NULL) {
            resourceStateChange("engine", ResourceState::READY);
        }
        if (INIT_NONE == m_asrInitStateFirstTime) {
            if (_asrEngine != NULL) {
                m_asrInitStateFirstTime = INIT_SUCCESS;
            }
            else {
                m_asrInitStateFirstTime = INIT_FAIL;
            }
            notifyStartFinishAfterCheck();
        }
    }
}

std::string VR_CNDialogEngine::getHints(const std::string& hintsParams)
{
    VR_LOGD_FUNC();
    pugi::xml_document doc;
    doc.load_string(hintsParams.c_str());
    VR_LOGP("DE receive getHints event 215-2-300");
    std::string agentName = doc.select_node("//agentName").node().child_value();
    std::string pageSizeStr = doc.select_node("//pageSize").node().child_value();
    int pageSize = atoi(pageSizeStr.c_str());
    std::string resultStr = _dataProcessor.getHintsData(agentName, pageSize, true, m_pConfigureIF->getHybridVRFlag());
    VR_LOGP("DE reply hints result 215-2-301");
    return resultStr;
}


void VR_CNDialogEngine::clearSameAgentState()
{
    VR_LOGD_FUNC();
    while (0 != _stateSatck.size()) {
        BackStateInfo backName = _stateSatck.top();
        if (backName.agentName == _currentAgent) {
            VR_LOGI("clear the state[%s] in stack!\n", backName.stateName.c_str());
            _stateSatck.pop();
        }
        else {
            return;
        }
    }
}

void VR_CNDialogEngine::clearTmpState()
{
    VR_LOGD_FUNC();
    _tmpStateBack.agentName = "";
    _tmpStateBack.stateName = "";
    _tmpStateBack.stateAttr = "";
}

void VR_CNDialogEngine::handleBack(uscxml::Event& evt)
{
    VR_LOGD_FUNC();
    // _stateSatck
    _forward_back = FORBACK;
    if (_stateSatck.empty()) {
        VR_LOGI("stateStack is empty, process back, stop interpreter");
        uscxml::Event evt;
        evt.setName("quitVRApp");
        postEvent(evt);
        return;
    }
    else {
        // if the sback of state is agentIdle ,pop all agentName = _currentAgentName
        BackStateInfo backName = _stateSatck.top();
        _stateSatck.pop();
        _backStateName = backName.stateName;
        std::string agentName = backName.agentName;
        VR_LOGI("back tmp stack [%s][%s]\n", _backStateName.c_str(), agentName.c_str());
        if (strcmp(agentName.c_str(), _currentAgent.c_str())) {
            // the agentName diff
            VR_LOGI("in the back process, it need changeAgent to agengName=%s state=%s, then the new agent send doback to DE", agentName.c_str(), _backStateName.c_str());
            uscxml::Event evtChangeAgent;
            evtChangeAgent.setName("backAgent");
            evtChangeAgent.setRaw(agentName);
            postEvent(evtChangeAgent);
            // changeAgent(agentName);
            // then request service wait for preInit
        }
        else {
            // doback
            VR_LOGI("do back normal in agent=%s, to state=%s, no changeagent!", agentName.c_str(), _backStateName.c_str());
            handleDoBack(evt);
        }
    }
}

void VR_CNDialogEngine::processStartOverEvent()
{
    VR_LOGD_FUNC();
    handleInterrupt(_currentEvent);
    if (NULL == _interManager) {
        VR_LOGE("interManager is empty, start over failed.");
        return;
    }
    _interManager->stopAgent();
    _dataProcessor.clearListDataFromDM();

    BackStateInfo backName;
    while (0 != _stateSatck.size()) {
        backName = _stateSatck.top();
        _stateSatck.pop();
    }

    // the two parms used for preInit
    _currentAgent = "topmenu";
    _lastAgent = "topmenu";
    _forward_back = FORWARD;
    _resendEvent = false;
    _interManager->startAgent(_currentAgent);
}

void VR_CNDialogEngine::replyCancelAndReady()
{
    VR_LOGD_FUNC();
    _lastEndBeepSeqId = 0;
    _lastTtsSeqId = 0;
    replyCancel();

    resourceStateChange("engine", ResourceState::READY);
    _sessionState = SESSION_IDLE;
    clearTtsAndBeepList();
    VR_LOGD("_sessionState is SESSION_IDLE");

    // notify grammar manager recognize end
    _dataAccessorManager->onRecognizeEnd();
}

void VR_CNDialogEngine::replyCancel()
{
    VR_LOGD_FUNC();
    std::string msg;

    while (!optionList.empty()) {
        std::string option = optionList.front();
        VR_LOGD("get option = %s", option.c_str());
        if (option.empty()) {
            msg = "<event-result name=\"cancel\"/>";
        }
        else {
            msg = "<event-result name=\"cancel\" option=\"" + option +"\"/>";
        }

        requestAction(msg);
        VR_LOGD("msg = %s", msg.c_str());
        optionList.pop_front();
    }
    m_isCanceledByDE = false;
}

void VR_CNDialogEngine::clearTtsAndBeepList()
{
    // clear tts/beep stop/play list
    VR_LOGD_FUNC();
    _lastEndBeepSeqId = 0;
    _lastStartBeepSeqId = 0;
    _lastTtsSeqId = 0;
    _lastStopBeepSeqId = 0;
    _lastStopTtsSeqId = 0;
    m_lastReturnBeep = 0;
}

bool VR_CNDialogEngine::isTtsCrash()
{
    VR_LOGD_FUNC();
    VoiceList<std::string>::iterator option = optionList.begin();
    while (option != optionList.end()) {
        if ("ttscrash" == *option) {
            VR_LOGD("ttscrash don't need play or stop");
            return true;
        }
        ++option;
    }
    return false;
}

void VR_CNDialogEngine::processCancelEvent(const std::string& option)
{
    VR_LOGD_FUNC();
    optionList.push_back(option);
    VR_LOGD("push option = %s", option.c_str());

    uscxml::Event unused;

    if (SESSION_IDLE == _sessionState) {
        VR_LOGD("_sessionState is SESSION_IDLE");
        replyCancel();
    }
    else if (SESSION_QUITING == _sessionState) { // in quiting state, waiting 'end' beep complete
        // set a flag to reply 'cancel' action result when sessin close is complete
        handleQuitVRApp(unused);
        _lastEndBeepSeqId = reqPlayBeep(END_BEEPPATH);
        _sessionState = SESSION_QUITING_WAITING;
        VR_LOGD("_sessionState is SESSION_QUITING_WAITING");
    }
    else if (SESSION_RUNNING == _sessionState || SESSION_PREPARE == _sessionState) {
        VR_LOGD("_sessionState is %d", _sessionState);
        closeSession(unused);
        handleQuitVRApp(unused);
        _lastEndBeepSeqId = reqPlayBeep(END_BEEPPATH);
        _sessionState = SESSION_QUITING_WAITING;
        VR_LOGD("_sessionState is SESSION_QUITING_WAITING");
    }
    else {
        // cancel type is PENDING, means DE is in quiting,
        // push the cancel to list, wait quit done
        VR_LOGD("_sessionState is %d", _sessionState);
        // _sessionState is SESSION_QUITING_WAITING, wait asr end and tts over

    }

    if ("ttscrash" == option) {
        clearTtsAndBeepList();
        VR_LOGD("clear Tts And Beep List");
        if (!_asrIsRunning && !m_isCanceledByDE) {
            replyCancelAndReady();
            VR_LOGD("ttscrash send");
        }
    }
}

void VR_CNDialogEngine::processGetHintsEvent(const pugi::xml_node& eventNode)
{
    VR_LOGD_FUNC();
    VR_LOGP("receive getHints event");
    std::string agentName = eventNode.select_node("//agentName").node().child_value();
    std::string pageSizeStr = eventNode.select_node("//pageSize").node().child_value();
    int pageSize = atoi(pageSizeStr.c_str());
    std::string resultStr = _dataProcessor.getHintsData(agentName, pageSize, true, m_pConfigureIF->getHybridVRFlag(), true);
    VR_LOGP("reply hints result");
    requestAction(resultStr);
}

bool VR_CNDialogEngine::processPrepareAgentEvent(const pugi::xml_node& eventNode)
{
    VR_LOGD_FUNC();

    if (SESSION_IDLE != _sessionState) {
        if (isDEQuiting()) {
           handleQuitVRApp(_currentEvent);
           VR_LOGE("Send Quit App");
        }
        VR_LOGE("Can't process in this state: %d", _sessionState);
        return false;
    }

    std::string agentName = eventNode.child_value("agent");
    _lastAgent = _currentAgent;
    _currentAgent = agentName;
    // the two parms used for preInit
    _forward_back = FORWARD;
    _resendEvent = false;

    _sessionState = SESSION_PREPARE;
    VR_LOGD("_sessionState is SESSION_PREPARE");
    // the same with startAgent, only Interpreter state change to PREPARE
    bool isPrepare = _interManager->prepareAgent(agentName);
    if (!isPrepare) {
        // start failed, quit VRApp
        quitByError(_currentEvent);
        VR_LOGE("prepareAgent ERROR");
        return false;
    }
    return true;
}

bool VR_CNDialogEngine::processStartAgentEvent(const pugi::xml_node& eventNode)
{
    VR_LOGD_FUNC();
    if (SESSION_IDLE != _sessionState && SESSION_PREPARE != _sessionState) {
        if (isDEQuiting()) {
           handleQuitVRApp(_currentEvent);
           VR_LOGE("Send Quit App");
        }
        VR_LOGE("Can't process in this state: %d", _sessionState);
        return false;
    }
    // notify grammar manager recognize begin
    _dataAccessorManager->onRecognizeBegin();

    std::string agentName = eventNode.child_value("agent");
    _lastAgent = _currentAgent;
    _currentAgent = agentName;
    // the two parms used for preInit
    _forward_back = FORWARD;
    _resendEvent = false;

    const std::string preInitStr = buildInitParm();
    _sessionState = SESSION_RUNNING;
    VR_LOGD("_sessionState is SESSION_RUNNING");
    // Interpreter state is ready or prepare normal case
    bool isStart = _interManager->startAgent(agentName, preInitStr);
    if (!isStart) {
        VR_LOGE("startAgent ERROR");
        quitByError(_currentEvent);
        return false;
    }
    return true;
}

void VR_CNDialogEngine::processStartDictationEvent()
{
    VR_LOGD_FUNC();
}

void VR_CNDialogEngine::processUpdateStateEvent(const std::string& eventStr)
{
    VR_LOGD_FUNC();
    _dataAccessorManager->updateState(eventStr);
}

void VR_CNDialogEngine::processSettingEvent(const pugi::xml_node& eventNode)
{
    VR_LOGD_FUNC();
    pugi::xpath_node_set paramNode = eventNode.select_nodes("//param");
    pugi::xpath_node_set::iterator itnode = paramNode.begin();
    while (itnode != paramNode.end()) {
        std::string name = itnode->node().attribute("name").as_string();
        std::string val = itnode->node().attribute("value").as_string();
        _setting[name] = val;
        ++itnode;
        VR_LOGI("set key[%s] = value[%s]", name.c_str(), val.c_str());
    }
}

void VR_CNDialogEngine::processUpdateCommStatusEvent(const pugi::xml_node& eventNode)
{
    VR_LOGD_FUNC();
    std::string value = eventNode.select_node("//code").node().text().as_string();
    VR_LOGI("CommStatus [code] = value[%s]", value.c_str());
    m_strCommunicationStatus = value;
}

// for sessionStateChanged
void VR_CNDialogEngine::sessionStateSet(bool val)
{
    VR_LOGP("sessionStateSet, audio session state[CN]=%d", val);
    _audioSessionState = val;
}

bool VR_CNDialogEngine::sessionStateGet()
{
    return _audioSessionState;
}

void VR_CNDialogEngine::processEventFromDM(const std::string& eventStr)
{
    VR_LOGD_FUNC();
    pugi::xml_document doc;
    doc.load_string(eventStr.c_str());
    pugi::xml_node eventNode = doc.select_node("//event").node();
    std::string eventName = eventNode.attribute("name").as_string();

    if (0 == strcmp(eventName.c_str(), "startedNotify")) {
        processStartedNotify();
        responseEventResult(eventNode);
        return;
    }

    if (m_bVDataError) {
        eventNode.remove_attribute("errcode");
        eventNode.append_attribute("errcode").set_value("1");
        responseEventResult(eventNode);
        VR_LOGD("VData Error, ingore the msg.");
        return;
    }

    if (0 == strcmp(eventName.c_str(), "prepare")) {
        // for dm performance, do original startagent
        // return 0:OK, 1:ERROR
        VR_LOGP("prepare event recv");
        bool isPrepareState = processPrepareAgentEvent(eventNode);
        VR_LOGP("prepare event recv %d", isPrepareState);
        std::string returnMsg = "";
        if (isPrepareState) {
            VR_LOGD("prepare event OK");
            returnMsg = "<event-result name=\"prepare\" errcode=\"0\">"
                                    "</event-result>";
        }
        else {
            VR_LOGE("prepare event Failed");
            returnMsg = "<event-result name=\"prepare\" errcode=\"1\">"
                                    "</event-result>";
        }
        requestAction(returnMsg);
        return;
    }
    else if (0 == strcmp(eventName.c_str(), "startAgent")) {
        VR_LOGP("receive startAgent event");
        // need PrepareAgent OK
        bool isStartState = processStartAgentEvent(eventNode);
        std::string returnMsg = "";
        if (isStartState) {
            VR_LOGD("startAgent event OK");
            returnMsg = "<event-result name=\"startAgent\" errcode=\"0\">"
                                    "</event-result>";
        }
        else {
            VR_LOGE("startAgent event Failed");
            returnMsg = "<event-result name=\"startAgent\" errcode=\"1\">"
                                    "</event-result>";
        }
        requestAction(returnMsg);
        return;
    }
    else if (0 == strcmp(eventName.c_str(), "cancel")) {
        std::string option = eventNode.attribute("option").as_string();
        VR_LOGD("processEventFromDM : cn cancel option %s", option.c_str());
        processCancelEvent(option);
        return;
    }
    else if (0 == strcmp(eventName.c_str(), "updateState")) {
        processUpdateStateEvent(eventStr);
    }
    else if (0 == strcmp(eventName.c_str(), "getHints")) {
        processGetHintsEvent(eventNode);
    }
    else if (0 == strcmp(eventName.c_str(), "startDictation")) {
        processStartDictationEvent();
    }
    else if (0 == strcmp(eventName.c_str(), "changeSettings")) {
        processSettingEvent(eventNode);
    }
    else if (0 == strcmp(eventName.c_str(), "changeLanguage")) {
        processChangeLanguage(eventNode);
    }
    else if (0 == strcmp(eventName.c_str(), "initialpersondata")) {
        processInitializePersonData();
    }
    else if (0 == strcmp(eventName.c_str(), "media")) {
        pugi::xml_node audioSourceNode = eventNode.select_node(".//category[@name='audiosource']").node();
        _dataAccessorManager->processAudioSourceList(audioSourceNode);
    }
    else if (0 == strcmp(eventName.c_str(), "getResourceState")) {
        std::stringstream oss;
        oss << static_cast<int>(_resourceState);
        std::string resStr = oss.str();
        std::string returnMsg = "<event-result name=\"getResourceState\">"
                                "<state>" + resStr + "</state>"
                                "</event-result>";
        requestAction(returnMsg);
        return;
    }
    else if (0 == strcmp(eventName.c_str(), "fullupdateNotify")) {
        std::string status = eventNode.select_node("//status").node().text().as_string();
        // off、navifulldata、finished
        if (status == "navifulldata") {
            _bMapDataPreparing = true;
            resetAsr(false);
        }
        else if (status == "finished") {
            _bMapDataPreparing = false;
            resetAsr(true);
        }
    }
    else if (0 == strcmp(eventName.c_str(), "cdf_finish")) {
        VR_LOGD("handle cdf_finish");
        if (m_pConfigureIF->getHybridVRFlag()) {
            N_Vr::N_Asr::C_Request_Param_Set param;
            param.m_e_Param_Type = E_Param_Type_Msg_Login;
            if (_asrEngine) {
                _asrEngine->Param_Set(param);
            }
            else {
                VR_LOGE("_asrEngine is null");
            }
        }
        return;
    }
    else if (0 == strcmp(eventName.c_str(), "sessionStateChanged")) {
        std::string status = eventNode.select_node("//state").node().text().as_string();
        VR_LOGP("state is %s", status.c_str());
        if (status == "on") {
            VR_LOGD("sessionStateChanged on");
            sessionStateSet(true);
        }
        else if (status == "off") {
            VR_LOGD("sessionStateChanged off");
            sessionStateSet(false);
        }
        _interManager->receiveEvent(eventName, eventStr);
    }
    else if (0 == strcmp(eventName.c_str(), "getGBookStatus")) {
        VR_LOGD("getGBookStatus");
        processUpdateCommStatusEvent(eventNode);
    }
    else if (0 == strcmp(eventName.c_str(), "preConnect")) {
        if (m_pConfigureIF->getHybridVRFlag()) {
            std::string netType = eventNode.child_value();
            VR_LOGD("Mediation: preConnect. got netType [%s].", netType.c_str());
   //         m_spCommuMediation->SetNetType(netType.c_str());
   //         m_spCommuMediation->Connect(true);
        }
        else {
            VR_LOGD("Mediation: preConnect is suppressed due to inactive HybridVRFlag");
        }
        return;
    }
    else {
        if (0 == strcmp(eventName.c_str(), "buttonPressed")) {
            if (_bMapDataPreparing) {
                std::string value = eventNode.select_node("//keycode").node().attribute("value").as_string();
                if (value == "navi") {
                    return;
                }
            }
            //
            std::string valueStr = eventNode.select_node("//keycode").node().attribute("value").as_string();
            if (0 == strcmp(valueStr.c_str(), "start_over")) {
                processStartOverEvent();
                responseEventResult(eventNode);
                return;
            }
            //
        }
        VR_LOGD("event recv = %s", eventName.c_str());
        _interManager->receiveEvent(eventName, eventStr);
        return;
    }
    responseEventResult(eventNode);
    return;
}

void VR_CNDialogEngine::responseEventResult(pugi::xml_node &eventNode)
{
    VR_LOGD_FUNC();
    std::stringstream oss;
    eventNode.set_name("event-result");
    eventNode.print(oss);
    requestAction(oss.str());
    VR_LOGP("Response event result, content:[%s]", oss.str().c_str());
    return;
}

void VR_CNDialogEngine::resetAsr(bool enableNavi)
{
    VR_LOGD_FUNC();
    tthread::lock_guard<tthread::recursive_mutex> lock(_mutex);
    if (NULL == _asrEngine) {
        VR_LOGD("_asrEngine is null");
        return;
    }

    if (SESSION_IDLE != _sessionState) {
        quitByError(_currentEvent);
        VR_LOGD("Dialogengine_Updata_Cancel");
    }

    if (_asrIsRunning) {
        _fullUpdateNavi = true;
    }
    else {
        N_Vr::N_Asr::C_Request_Param_Set param;
        param.m_e_Param_Type = E_Param_Type_Resource_Map;
        param.m_i_Value = enableNavi ? 1 : 0;
        _asrEngine->Param_Set(param);
        VR_LOGD("map resource %s", enableNavi ? "loaded +++" : " unloaded ---");    
    }
}

void VR_CNDialogEngine::processInitializePersonData()
{
    VR_LOGD_FUNC();
    resourceStateChange("init", ResourceState::GRAMMAR_NOT_READY);
    resourceStateChange("engine", ResourceState::GRAMMAR_NOT_READY);
    handleQuitVRApp(_currentEvent);
    // stop Asr
    if (NULL != _asrEngine && _asrIsRunning) {
        VR_LOGD("Recognize_Cancel");
        m_isCanceledByDE = true;
        _asrEngine->Recognize_Cancel();
    }
    // wait stop Asr end
    if (_asrIsRunning) {
        _initPersonData = true;
         VR_LOGD("asr engine not cancel");
    }
    else {
        // delete Asr
        if (_asrEngine) {
            VR_LOGD("before delete engine");
            delete _asrEngine;
            _asrEngine = NULL;
            VR_LOGD("delete engine");
        }
        // clear user data
      //  C_Engine::Clear_User_Data();
        // init asr
        uscxml::Event evt;
        handleInitAsr(evt);
        resourceStateChange("init", ResourceState::READY);
    }
    return;
}

void VR_CNDialogEngine::languageParamSet()
{
    VR_LOGD_FUNC();
    _dataAccessorManager->setGenerateGrammarEnable(false);
    resourceStateChange("engine", ResourceState::GRAMMAR_NOT_READY);
    N_Vr::N_Asr::C_Request_Language_Change param;
    param.m_string_Id_Language  = _languageId;
    param.m_function_On_Event_Init_Notify = boost::bind(&VR_CNDialogEngine::onAsrInit, this, _1);
    _asrEngine->Language_Change(param);
    _changeLanguage = false;
    _languageId = "";
    resourceStateChange("engine", ResourceState::READY);
    _dataAccessorManager->resetGrammarGeneration();
    requestGrammarAfterCheck();
}

// REVIEW: ch-ch, asr.int,  if currentlan not support VR, need not changeLanguage
void VR_CNDialogEngine::processChangeLanguage(const pugi::xml_node& languageNode)
{
    VR_LOGD_FUNC();
    m_pDECommonIF->notifyVRLanguageChange();
    if (!m_pDECommonIF->isSupportVR()) {
        VR_LOGD("this country|language|product is not support VR");
        return;
    }
    std::string language = m_pDECommonIF->getVRLanguage();
    VR_LOGD("processchange language = %s", language.c_str());
    _deDataManager->initData(language);
    initPhoneTypeName();
    _dataProcessor.initData(language);
    VoiceMap<std::string, std::string>::iterator ite = asrLanguage_cn.find(language);
    if (ite == asrLanguage_cn.end()) {
        VR_LOGE("asr is not support the language");
    }
    else {
        if (_asrEngine) {
            _languageId  = ite->second;
            if (!_asrIsRunning) {
                VR_LOGI("changelanguage");
                languageParamSet();
            }
            else {
                VR_LOGI("changelanguage, _asrIsRunning = true");
                _changeLanguage = true;
                m_isCanceledByDE = true;
                _asrEngine->Recognize_Cancel();
            }
        }
        else {
            VR_LOGI("_asrEngine is null");
            handleInitAsr(_currentEvent);
        }
    }
    // return
}

int  VR_CNDialogEngine::networkStatusChanged(int status)
{
    VR_LOGD_FUNC();
    N_Vr::N_Asr::C_Request_Param_Set param;
    param.m_e_Param_Type = E_Param_Type_Network_Change;
    param.m_i_Value = status;
    _asrEngine->Param_Set(param); // _asrEngine won't be null
    return 0;
}

void VR_CNDialogEngine::processUpdateCenterVrUrl(const pugi::xml_node& eventNode)
{
    VR_LOGD_FUNC();
    // update CenterVrUrl to asr engine
    N_Vr::N_Asr::C_Request_Param_Set urlParam;
    urlParam.m_e_Param_Type = E_Param_Type_Center_Vr_Url;
    urlParam.m_string_Value = m_pConfigureIF->getHybridVRUrl();
    m_hasGotCenterVRUrl     = urlParam.m_string_Value.length() > 0;
    if (_asrEngine) {
        _asrEngine->Param_Set(urlParam);
        VR_LOGD("Update CenterVrUrl to [%s].", urlParam.m_string_Value.c_str());
    }
    else {
        VR_LOGE("_asrEngine is null");
    }
}

void VR_CNDialogEngine::processActionResultFromDM(const std::string& eventStr, int seqId)
{
    VR_LOGD_FUNC();

    if (m_bVDataError) {
        VR_LOGD("VData Error, ingore the msg.");
        return;
    }

    pugi::xml_document doc;
    doc.load_string(eventStr.c_str());
    pugi::xpath_node actionResultNode = doc.select_node("//action-result");
    pugi::xml_node listNode = actionResultNode.node().select_node("//list").node();
    std::string resultStr = eventStr;
    if (NULL != listNode) {
        // update list data in DE
        pugi::xml_node tmpNode = actionResultNode.node();

        pugi::xpath_node_set itemXpathNodes = tmpNode.select_nodes("//startIndex");
        int count = itemXpathNodes.size();
        if (itemXpathNodes.size() < 1) {
        }
        else {
            for (int i = 0; i < count; ++i) {
                itemXpathNodes[i].node().text().set("0");
            }
        }

        _dataProcessor.updateListByActionResult(tmpNode);
        // delete the items and send it to scxml
        // actionResultNode.node().select_node("//items").parent().remove_child("items");

        std::ostringstream oss;
        actionResultNode.node().print(oss);
        resultStr = oss.str();
    }
    std::string op = actionResultNode.node().attribute("op").as_string();
    // std::string dataName = op + "_result";
    std::string eventName = "done." + op;

    if ("requestGrammar" == op) {
        return;
    }

    if (("done.queryGBookStatus" == eventName)) {
        VR_LOGI("done.queryGBookStatus From DM, status:%d", m_bCommunicationStatusFlag);
        if (m_bCommunicationStatusFlag) {
            processUpdateCommStatusEvent(actionResultNode.node());
            m_bCommunicationStatusFlag = false;
            // dataName = "getCommunicationStatus_result";
            eventName = "done.getCommunicationStatus";
            op = "getCommunicationStatus";
        }
    }
    else if ("done.queryCenterVRURL" == eventName) {
        processUpdateCenterVrUrl(actionResultNode.node());
        return;
    }
    else if ("done.queryRouteExist" == eventName) {
        VR_LOGD("done.queryRouteExist recv.");
        pugi::xml_node listNode = actionResultNode.node().select_node("//exist").node();
        m_routeExistStatus = listNode.text().as_string();
        return;
    }

    if ("done.playBeep" != eventName
        && "done.stopTts" != eventName
        && "done.stopBeep" != eventName
        && "done.playTts" != eventName) {
        // beep or TTS 'done' event don't need interpreter fetch sync
        _interManager->notifyOpResult(op, resultStr, false);
    }
    else if ("done.playTts" == eventName) { // interpreter not care done.playBeep, done.stopBeep, done.stopTts
        _interManager->receiveEvent("done.playTts", "");
    }

    processDoneEvent(eventName, seqId);
}

void VR_CNDialogEngine::processDoneEvent(const std::string& eventName, int seqId)
{
    VR_LOGD_FUNC();
    VR_LOGP("Process done event, name=%s, seqId=%d", eventName.c_str(), seqId);
    if (eventName == "done.playTts") {
        if (seqId == _lastTtsSeqId) {
            _lastTtsSeqId = 0;
            _lastStopTtsSeqId = 0;
            interruptActionResult();
        }

        if (_continueAsr) {
          _continueAsr = false;
          VR_LOGI("done.playTts, start VR, grammar[%s]", _grammarStr.c_str());
          requestVR(_grammarStr);
        }
    }
    else if (eventName == "done.stopTts") {
        if (seqId == _lastStopTtsSeqId) {
            _lastStopTtsSeqId = 0;
            _lastTtsSeqId = 0;
            interruptActionResult();
            if (_continueAsr) {
              _continueAsr = false;
              VR_LOGI("done.stopTts, start VR, grammar[%s]", _grammarStr.c_str());
              requestVR(_grammarStr);
            }
        }
    }
    else if (eventName == "done.playBeep" || eventName == "done.stopBeep") {
        VR_LOGP("receive action-result of done.playBeep, id:%d", seqId);
        if (seqId == _lastStartBeepSeqId) {
            _lastStartBeepSeqId = 0;
            interruptActionResult();
        }

        if (seqId == m_lastReturnBeep) {
            m_lastReturnBeep = 0;
            interruptActionResult();
            if (_continueAsrByReturnBeep) {
                _continueAsrByReturnBeep = false;
                VR_LOGI("done.returnBeep, start VR, grammar[%s]", _grammarStr.c_str());
                requestVR(_grammarStr);
            }
        }

        if (seqId == _lastEndBeepSeqId) {
            if (SESSION_QUITING_WAITING == _sessionState) {
                VR_LOGD("_sessionState is SESSION_QUITING_WAITING");
                int count = 0;
                while (_asrIsRunning) {
                    usleep(10000);
                    count++;
                    if (count == 500) {
                        VR_LOGE("Cancel Msg Recv, Asr Stop Time Out");
                        replyCancel();
                    }
                }

                replyCancelAndReady();
                return;
            }
        }
    }
}

void VR_CNDialogEngine::processGrammarFromDM(const std::string& eventStr)
{
    pugi::xml_document doc;
    if (!doc.load_string(eventStr.c_str())) {
        return;
    }
    pugi::xml_node eventNode = doc.first_child();
    std::string eventName = eventNode.name();

    // set the ipod type
    if ("grammar_active" == eventName) {
        std::string sourceType = eventNode.attribute("sourcetype").as_string();
        m_isIpod = ("ipod" == sourceType);
    }

    if (m_bVDataError) {
        VR_LOGD("grammar msg name=%s", eventNode.name());
        eventNode.set_name("grammar_result");
        eventNode.remove_attribute("errcode");
        eventNode.append_attribute("errcode").set_value("1");

        if ("grammar_init" == eventName || "grammar_diff" == eventName) {
            eventNode.append_attribute("op").set_value("grammar");
        }
        else if ("grammar_active" == eventName) {
            eventNode.append_attribute("op").set_value("active");
        }
        else if ("grammar_disactive" == eventName) {
            eventNode.append_attribute("op").set_value("disactive");
        }
        std::ostringstream oss;
        eventNode.print(oss);
        VR_LOGD("grammar msg response=%s", oss.str().c_str());
        requestAction(oss.str());
        VR_LOGD("VData Error, ingore the msg.");
        return;
    }
    _dataAccessorManager->updateGrammar(eventStr);
}

bool
VR_CNDialogEngine::SendMessage(const std::string& eventStr, int actionSeqId)
{
    VR_LOGD_FUNC();
    if (NULL == _interManager) {
        VR_LOGE("has not start!");
        RETURN(false);
    }
    // seqID default value is 0 in China region
    if (VR_ACTION_SEQ_ID_INVALID == actionSeqId) {
        actionSeqId = 0;
    }
    VR_LOGP("received external event=%s, seqId=%d", eventStr.c_str(), actionSeqId);

    VR_LOG_TO_FILE("sendMsg", eventStr);
    VR_DUMP_TESTCASE("msg:", eventStr);

    uscxml::Event event;
    event.setName("sendMessage_Event");
    std::ostringstream oss;
    oss << actionSeqId;
    event.setSendId(oss.str());
    event.setXML(eventStr.c_str());
    postEvent(event);
    RETURN(true);
}

void VR_CNDialogEngine::handleDoChangeAgent(uscxml::Event& evt)
{
    VR_LOGD_FUNC();
    std::string agentName = evt.getRaw();
    if (FORWARD == _forward_back) {
        this->saveTmpState(_currentStateBackChangeAgent.stateName, _currentStateBackChangeAgent.stateAttr);
    }

    _lastAgent = _currentAgent;
    _currentAgent = agentName;
    VR_LOGD("----------------change Anget----------------");
    bool ok = _interManager->changeAgent(agentName);

    if (!ok) {
        // change failed, quit VR
        VR_LOGE("changeAgent fail");
        quitByError(_currentEvent);
    }
}

void VR_CNDialogEngine::handleBackAgent(uscxml::Event &evt)
{
    VR_LOGD_FUNC();
    std::string agentName = evt.getRaw();
    _lastAgent = _currentAgent;
    _currentAgent = agentName;
    VR_LOGD("----------------back Anget----------------");
    bool ok = _interManager->backAgent(agentName);

    if (!ok) {
        // change failed, quit VR
        VR_LOGE("Back Agent fail");
        quitByError(_currentEvent);
    }
}

void VR_CNDialogEngine::pushTmpToStack()
{
    VR_LOGD_FUNC();
    tthread::lock_guard<tthread::recursive_mutex> lock(_mutex);
    if (_tmpStateBack.agentName.empty()) {
        VR_LOGI("tmpStateBack'agent is empty");
        return;
    }
    else if (_tmpStateBack.stateName.empty()) {
        VR_LOGI("tmpStateBack'state is empty");
        return;
    }
    else {
        _stateSatck.push(_tmpStateBack);
        VR_LOGI("push tmp stack[%s],[%s]\n", _tmpStateBack.stateName.c_str(), _tmpStateBack.stateAttr.c_str());
    }
}

void VR_CNDialogEngine::saveTmpState(const std::string& stateName, const std::string& stateAttr)
{
    VR_LOGD_FUNC();
    tthread::lock_guard<tthread::recursive_mutex> lock(_mutex);
    _tmpStateBack.agentName = _currentAgent;
    _tmpStateBack.stateName = stateName;
    _tmpStateBack.stateAttr = stateAttr;
    VR_LOGI("Save tmp agent for back, state:%s, attr:%s", stateName.c_str(), stateAttr.c_str());
}

std::string VR_CNDialogEngine::getTmpAttr()
{
    RETURN(_tmpStateBack.stateAttr);
}

void VR_CNDialogEngine::saveCurrentState(const std::string& stateName, const std::string& stateAttr)
{
    VR_LOGD_FUNC();
    tthread::lock_guard<tthread::recursive_mutex> lock(_mutex);
    _currentStateBackChangeAgent.stateName = stateName;
    _currentStateBackChangeAgent.stateAttr = stateAttr;
    VR_LOGP("Save current agent for back, state:%s, attr:%s", stateName.c_str(), stateAttr.c_str());
}

// invoke only once after asr engine start/restart
void VR_CNDialogEngine::onAsrInit(C_Event_Init_Notify const & notify)
{
    C_Event_Init_Notify::E_Event_Init_Type type = notify.Get_Event_Init_Notify_Type();
    if (C_Event_Init_Notify::E_Event_Init_Notify_Type_Context_Not_Exist == type
        || C_Event_Init_Notify::E_Event_Init_Notify_Type_Context_Invalid == type) {
        m_regenerationContextIDList.push_back("ctx_media_play_music_list_1");
        m_regenerationContextIDList.push_back("ctx_media_play_music_list_2");
        m_regenerationContextIDList.push_back("ctx_media_play_music_list_3");
        m_regenerationContextIDList.push_back("ctx_media_play_music_list_4");
        m_regenerationContextIDList.push_back("ctx_phone_contact_name_list_1");
        VR_LOGD("grammar not invalid, regenerate all.");
    }
}

// ASR engine callback events
void VR_CNDialogEngine::onAsrPhase(C_Event_Phase const& phase)
{
    VR_LOGD_FUNC();
    C_Event_Phase::E_Event_Phase_Type msgType = phase.Get_Event_Phase_Type();
    switch (msgType) {
    case C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Listen_Begin:
    {
        VR_LOGI("ASR event: listen begin, event is %d", msgType);
        uscxml::Event evt(VR_ASR_EVENT_NAME);
        evt.setRaw(VR_ASR_EVT_LISTEN);
        postEvent(evt);
        break;
    }

    case C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Speech_Begin_Fx:
    {
        VR_LOGP("ASR event: begin of speech, event is %d", msgType);
        uscxml::Event evt(VR_ASR_EVENT_NAME);
        evt.setRaw(VR_ASR_EVT_BOS);
        postEvent(evt);
        break;
    }

    case C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Response_Timeout:
    case C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Speech_Timeout:
    case C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Speech_End_Fx:
    {
        VR_LOGP("ASR event: end of speech, event is %d", msgType);
        if (!m_isAsrSpeechEndRecv) {
            m_isAsrSpeechEndRecv = true;
            uscxml::Event evt(VR_ASR_EVENT_NAME);
            evt.setRaw(VR_ASR_EVT_EOS);
            postEvent(evt);
            break;
        }
        else {
            VR_LOGD("ASR event: end of speech alreadly recv, don't need handle");
        }
    }

    case C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Proc_End:
    {
        VR_LOGP("ASR event: end, event is %d", msgType);
        {
            tthread::lock_guard<tthread::recursive_mutex> lock(_mutex);
            _asrIsRunning = false;
            VR_LOGD("asrIsRunning is false");
            _condVarAsr.notify_all();
        }
        uscxml::Event evt(VR_ASR_EVENT_NAME);
        evt.setRaw(VR_ASR_EVT_END);
        postEvent(evt);

        m_isAsrSpeechEndRecv = false;

        break;
    }

    default:
        VR_LOGD("ASR event unknown : %d", msgType);
        break;
    }
}

void VR_CNDialogEngine::onAsrNotify(C_Event_Notify const& notify)
{
    if (C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_Network_Connect == notify.Get_Event_Notify_Type()) {
        VR_LOGD("Mediation: request connect");
        uscxml::Event evt(VR_ASR_EVENT_NAME);
        evt.setRaw(VR_ASR_EVT_CONNECT);
        postEvent(evt);
    }
    else if (C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_Network_Disconnect == notify.Get_Event_Notify_Type()) {
        VR_LOGD("Mediation: request disconnect");
        uscxml::Event evt(VR_ASR_EVENT_NAME);
        evt.setRaw(VR_ASR_EVT_DISCONNECT);
        postEvent(evt);
    }
    else {
        // Don't push volume change event to event queue of controller because it don't affect engine state
        std::string retMsg;
        m_pDEMessageBuilder->buildVolumeDisplay(notify, retMsg);
        requestAction(retMsg);
    }
}

void VR_CNDialogEngine::responseAsrError()
{
    std::string eventName = "failed.asr";
    _currentIntention = std::pair<std::string, std::string>(eventName, "");
    if (NULL != _interManager) {
        _interManager->receiveEvent(eventName, "");
    }
}

void VR_CNDialogEngine::onAsrResult(C_Event_Result const& result)
{
    VR_LOGD_FUNC();
    uscxml::Event evt(VR_ASR_EVENT_NAME);
    evt.setRaw(VR_ASR_EVT_RESULT);
    evt.setContent(result.Get_Json()->c_str());
    postEvent(evt);
}

int VR_CNDialogEngine::reqPlayBeep(const std::string& beepSubPath)
{
    VR_LOGD_FUNC();
    if (isTtsCrash()) {
        VR_LOGD("ttscrash don't need play or stop");
        return 0;
    }
    VR_LOGP("Request play beep:[%s]", beepSubPath.c_str());
    std::string playBeepMsg;
    std::string beepPath = m_pConfigureIF->getDataPath() + beepSubPath;
    m_pDEMessageBuilder->buildPlayBeepAction(beepPath, playBeepMsg);
    return requestAction(playBeepMsg);
}

int VR_CNDialogEngine::reqPlayTTS(const std::string& ttsTxt)
{
    VR_LOGD_FUNC();
    if (isTtsCrash()) {
        VR_LOGD("ttscrash don't need play or stop");
        return 0;
    }
    std::string ttsMsg;
    m_pDEMessageBuilder->buildPlayTTSAction(ttsTxt, ttsMsg);
    VR_LOGP("Request play TTS: %s", ttsMsg.c_str());
    _lastTtsSeqId = requestAction(ttsMsg);
    return _lastTtsSeqId;
}

int VR_CNDialogEngine::reqPlayTTS(const VoiceList<std::string>::type& txtList)
{
    VR_LOGD_FUNC();
    if (isTtsCrash()) {
        VR_LOGD("ttscrash don't need play or stop");
        return 0;
    }
    std::string ttsMsg;
    m_pDEMessageBuilder->buildPlayTTSAction(txtList, ttsMsg);
    VR_LOGP("Request play TTS: %s", ttsMsg.c_str());
    _lastTtsSeqId = requestAction(ttsMsg);
    return _lastTtsSeqId;
}

std::string VR_CNDialogEngine::changeGrammarIDForAsr(const std::string &grammar)
{
    std::string newGrammar(grammar);
    std::string countryFlag("_{country}");
    std::string country("");
    std::size_t pos = newGrammar.find(countryFlag);
    if (std::string::npos != pos) {
        newGrammar.replace(pos, countryFlag.size(), country);
    }

    RETURN(newGrammar);
}

void VR_CNDialogEngine::setItemValue(pugi::xml_node doc, const std::string skey, const std::string svalue)
{
    VR_LOGI("set key=%s, to value=%s", skey.c_str(), svalue.c_str());
    std::string xpathStr = "//item[@key='" + skey + "']";
    pugi::xml_node countryNode = doc.select_node(xpathStr.c_str()).node();
    if (countryNode) {
        countryNode.attribute("value").set_value(svalue.c_str());
    }
    else {
        pugi::xml_node gnode = doc.select_node("//g").node();
        pugi::xml_node itemNode = gnode.append_child("item");
        itemNode.append_attribute("key").set_value("COUNTRY");
        itemNode.append_attribute("value").set_value(svalue.c_str());
    }
}

void VR_CNDialogEngine::onStateUpdate(const std::string &msgToDM)
{
    VR_LOGD_FUNC();
    doStateUpdate(msgToDM, true);
}

void VR_CNDialogEngine::doStateUpdate(const std::string &msgToDM, bool notifly)
{
    VR_LOGD_FUNC();
    // get stateMsg
    std::string stateMsg;
    _dataAccessorManager->getUpdateState(stateMsg);

    // for climate state change
    updateStateClimateMap(stateMsg);

    // add country into global state
    pugi::xml_document doc;
    doc.load_string(stateMsg.c_str());
    // set country
    // int country = m_pConfigureIF->getVRContry();
    std::string country = m_pDECommonIF->getProductCountry();
    VR_LOGD("country = %s", country.c_str());
    setItemValue(doc, "COUNTRY", country);
    // set navi model exist
    bool isNaviModelExist = false;
    std::fstream file;
    std::string mapDataPath = m_pConfigureIF->getMapDataPath();
    file.open(mapDataPath.c_str(), std::ios::in);
    if (file) {
        isNaviModelExist = true;
    }
    isNaviModelExist = isNaviModelExist && (!_bMapDataPreparing);
    m_pDECommonIF->setNaviModel(isNaviModelExist);
    std::ostringstream ss;
    ss << std::boolalpha << isNaviModelExist;
    std::string isStr = ss.str();
    setItemValue(doc, "NAVI_MODEL_EXIST", isStr);

    std::stringstream oss;
    doc.print(oss);

    VR_LOGD("doStateUpdate : notifyUpdateGlobalStates");
    _interManager->notifyUpdateGlobalStates(oss.str(), notifly);
}

void VR_CNDialogEngine::addGrammarData()
{
    std::string grammarInit = std::string("<grammar_init agent=\"media\" grammarid=\"1\" path=\"") + m_pConfigureIF->getDataPath() + "MusicCatalog.db\" />";
    SendMessage(grammarInit);

    pugi::xml_document phoneMsgDoc;
    phoneMsgDoc.load_file((m_pConfigureIF->getDataPath() + "phoneGrammarMsg.xml").c_str());

    ostringstream oss;
    phoneMsgDoc.print(oss);
    std::string grammarNew = oss.str();

    SendMessage(grammarNew);

    // std::string grammarNew = "<grammar_new agent=\"phone\">"
    //                         "<category name=\"contact\">"
    //                             "<person id=\"5\" first=\"David\" last=\"Smith\">"
    //                                 "<phone_item type=\"2\" number=\"18845678912\"/>"
    //                                 "<phone_item type=\"3\" number=\"13645678912\"/>"
    //                                 "<phone_item type=\"1\" number=\"021-45678901\"/>"
    //                             "</person>"
    //                             "<person id=\"4\" first=\"Jim\" last=\"\">"
    //                                 "<phone_item type=\"2\" number=\"13698468345\"/>"
    //                             "</person>"
    //                             "<person id=\"3\" first=\"\" last=\"Alex\">"
    //                                 "<phone_item type=\"1\" number=\"021-69895841\"/>"
    //                             "</person>"
    //                             "<person id=\"1\" first=\"James\" last=\"Williams\">"
    //                                 "<phone_item type=\"3\" number=\"15464513854\"/>"
    //                             "</person>"
    //                             "<person id=\"2\" first=\"Emily\" last=\"Brown\">"
    //                                 "<phone_item type=\"2\" number=\"13876684973\"/>"
    //                                 "<phone_item type=\"3\" number=\"18735546864\"/>"
    //                                 "<phone_item type=\"1\" number=\"021-81646954\"/>"
    //                             "</person>"
    //                         "</category>"
    //                         "<category name=\"quickreplymessage\">"
    //                             "<message id=\"2\" name=\"Be x minutes late\"/>"
    //                         "</category>"
    //                         "<category name=\"phonetype\">"
    //                             "<fromal id=\"1\" name=\"home\">"
    //                                 "<alias name=\"home\"/>"
    //                             "</fromal>"
    //                             "<formal id=\"2\" name=\"mobile\">"
    //                                 "<alias name=\"cell\"/>"
    //                                 "<alias name=\"mobile phone\"/>"
    //                                 "<alias name=\"mobile 1\"/>"
    //                             "</formal>"
    //                             "<formal id=\"3\" name=\"mobile 2\">"
    //                                 "<alias name=\"cell 2\"/>"
    //                                 "<alias name=\"mobile 2\"/>"
    //                             "</formal>"
    //                         "</category>"
    //                         "<category name=\"messagetype\">"
    //                             "<formal id=\"1\" name=\"Email\">"
    //                                 "<alias name=\"Email\"/>"
    //                                 "<alias name=\"mail\"/>"
    //                             "</formal>"
    //                             "<formal id=\"2\" name=\"SMS\">"
    //                                 "<alias name=\"SMS\"/>"
    //                                 "<alias name=\"MMS\"/>"
    //                             "</formal>"
    //                         "</category>"
    //                     "</grammar_new>";

    // SendMessage(grammarNew);
}

void VR_CNDialogEngine::initPhoneTypeName()
{
    VR_DataAccessor::setPhoneTypeName(0, _deDataManager->getPrompts("VOMOBILE"));
    VR_DataAccessor::setPhoneTypeName(1, _deDataManager->getPrompts("VOHOME"));
    VR_DataAccessor::setPhoneTypeName(2, _deDataManager->getPrompts("VOWORK"));
    VR_DataAccessor::setPhoneTypeName(3, _deDataManager->getPrompts("VOOTHER"));

    VR_DataAccessor::setPhoneTypeName(4, _deDataManager->getPrompts("VOMOBILE"));
    VR_DataAccessor::setPhoneTypeName(5, _deDataManager->getPrompts("VOHOME"));
    VR_DataAccessor::setPhoneTypeName(6, _deDataManager->getPrompts("VOWORK"));
    VR_DataAccessor::setPhoneTypeName(7, _deDataManager->getPrompts("VOOTHER"));

    std::ostringstream oss;
    for (int index = 2; index < 5; ++index) {
        oss << index;
        VR_DataAccessor::setPhoneTypeName(index * 4 + 0, _deDataManager->getPrompts(std::string("VOMOBILE").append(oss.str())));
        VR_DataAccessor::setPhoneTypeName(index * 4 + 1, _deDataManager->getPrompts(std::string("VOHOME").append(oss.str())));
        VR_DataAccessor::setPhoneTypeName(index * 4 + 2, _deDataManager->getPrompts(std::string("VOWORK").append(oss.str())));
        VR_DataAccessor::setPhoneTypeName(index * 4 + 3, _deDataManager->getPrompts(std::string("VOOTHER").append(oss.str())));
        oss.str("");
    }
}

bool VR_CNDialogEngine::getPOIFromWebData(const std::string& strData, std::string& xmlStr)
{
    VR_LOGD_FUNC();
    std::string POIListXml =
        "<list id=\"webSearch\">"
           "<header>"
               "<pageSize>0</pageSize>"
               "<startIndex>0</startIndex>"
               "<count>%1%</count>"
           "</header> "
            "<items>"
                "%2%"
            "</items>"
        "</list>";

    std::string POIItemXml =
        "<item>"
            "<poi>"
                "<poiInfo>"
                        "<id>%1%</id>"
                        "<alias>%2%</alias>"
                        "<address>%3%</address>"
                        "<direction>%4%</direction>"
                        "<distance>%5%</distance>"
                        "<unit>%6%</unit>"
                "</poiInfo>"
                "<guidePoint>"
                        "<name>%7%</name>"
                        "<routeAddress>%12%</routeAddress>"
                        "<routeTelNumber>%13%</routeTelNumber>"
                        "<displayGuideType></displayGuideType>"
                        "<linkId>0</linkId>"
                        "<longitude>%8%</longitude>"
                        "<latitude>%9%</latitude>"
                        "<poiId>0</poiId>"
                        "<areaId>0</areaId>"
                        "<list>"
                            "<header>"
                                "<pageSize>0</pageSize>"
                                "<startIndex>0</startIndex>"
                                "<count>%10%</count>"
                            "</header>"
                            "<items>"
                                    "%11%"
                            "</items>"
                        "</list>"
                "</guidePoint>"
            "</poi>"
        "</item>";

    std::string POIGuidePointItem =
        "<item>"
            "<guideType></guideType>"
                "<guideLongitude>%1%</guideLongitude>"
                "<guideLatitude>%2%</guideLatitude>"
        "</item>";
    std::string retStr = " ";
    // std::string::size_type sz;
    // std::string nullStr = "";
    rapidjson::Document jsonDoc;
    jsonDoc.Parse<0>(strData.c_str());
    if (jsonDoc.HasParseError()) {
        VR_LOGE("POIFromWebData parse error [%d]\n", __LINE__);
        RETURN(false);
    }

    if (!jsonDoc.HasMember("data")) {
        VR_LOGE("POIFromWebData parse error [%d]\n", __LINE__);
        RETURN(false);
    }

    const rapidjson::Value& objData = jsonDoc["data"];
    if (objData.HasMember("code")) {
        const rapidjson::Value& valState = objData["code"];
        int iState = valState.GetInt();
        if (0 != iState) {
            VR_LOGE("POI From webSerch Error :[%d]\n", iState);
            RETURN(false);
        }
    }

    if (objData.HasMember("result")) {
        const rapidjson::Value& valResult = objData["result"];
        if (!valResult.IsArray()) {
            VR_LOGE("DE parser the intention from ASR:[%d]\n", __LINE__);
            xmlStr = retStr;
            RETURN(false);
        }

        std::string Unit6 = "m";	
        std::string poiItemStr = "";
        int poiCount = 0;
        for (rapidjson::SizeType idx = 0; idx < valResult.Size(); ++idx) {
            const rapidjson::Value& objArray = valResult[idx];
            if (objArray.IsObject()) {

                int poiId = 0;
                int lon = 0;
                int lat = 0;

                double direction = 0.0;

                const rapidjson::Value& valName = objArray["name"];
                const rapidjson::Value& valAddress = objArray["address"];
                const rapidjson::Value& valLon = objArray["lng"];
                const rapidjson::Value& valLat = objArray["lat"];

                const rapidjson::Value& valDistance = objArray["distance"];
                const rapidjson::Value& valDirection = objArray["direction"];

                std::string name = valName.GetString();
                std::string address = valAddress.GetString();
                std::string distance = valDistance.GetString();

                // add for poi tel 20160226
                std::string strTelephone = "";

                if (valLon.IsString()) {
                    float flon = atof(valLon.GetString());
                    lon = (int)(flon * 3600 * 256);
                }

                if (valLat.IsString()) {
                    float flat = atof(valLat.GetString());
                    lat = (int)(flat * 3600 * 256);
                }

                if (valDirection.IsString()) {
                    direction = atof(valDirection.GetString());
                    direction = direction * DEG_TO_NAVI;
                }

                if (objArray.HasMember("tel")) {
                    const rapidjson::Value& valTelephone = objArray["tel"];
                    if (valTelephone.IsString()) {
                        strTelephone = valTelephone.GetString();
                    }
                }

#if 0
                if (nullStr != valLon.GetString()) {
                    lon = std::stof(valLon.GetString(), &sz);
                    lon = lon * 3600 * 256;
                }
                else {
                    VR_LOGE("POI longitude is error\n");
                    break;
                }

                if (nullStr != valLat.GetString()) {
                    lat = std::stof(valLat.GetString(), &sz);
                    lat = lat * 3600 * 256;
                }
                else {
                    VR_LOGE("POI latitude is error\n");
                    break;
                }

                if (nullStr != valEntryLon.GetString()) {
                    entryLon = std::stof(valEntryLon.GetString(), &sz);
                    entryLon = entryLon * 3600 * 256;
                }
                else {
                    VR_LOGD("POI Entry longitude is error\n");
                }

                if (nullStr != valEntryLat.GetString()) {
                    entryLat = std::stof(valEntryLat.GetString(), &sz);
                    entryLat = entryLat * 3600 * 256;
                }
                else {
                    VR_LOGD("POI Entry latitude is error\n");
                }

                if ((nullStr != valEntryLon.GetString()) && (nullStr != valEntryLat.GetString())) {
                    ++guidePointCnt;
                }

                if (nullStr != valDistance.GetString()) {
                    distance = std::stod(valDistance.GetString(), &sz);
                    if (Unit6.compare("mile") == 0)  {
                        distance = distance / 1609.344; // mile to meter
                    }
                    else if (Unit6.compare("km") == 0) {
                        distance = distance / 1000;
                    }
                    else {
                        Unit6 = "m";
                    }
                }
                else {
                    VR_LOGE("POI distance is error\n");
                    break;
                }

                if (nullStr != valDirection.GetString()) {
                    direction = std::stod(valDirection.GetString(), &sz);
                }
                else {
                    VR_LOGE("POI direction is error\n");
                    break;
                }

                std::string poiGuidePointStr = (boost::format(POIGuidePointItem) % std::to_string(entryLon) % std::to_string(entryLat)).str();
                std::string poiItem = (boost::format(POIItemXml)
                    % std::to_string(poiId)
                    % name
                    % address
                    % std::to_string(direction)
                    % std::to_string(distance)
                    % Unit6
                    % name
                    % std::to_string(lon)
                    % std::to_string(lat)
                    % std::to_string(guidePointCnt)
                    % poiGuidePointStr).str();
#endif
                std::string poiGuidePointStr = (boost::format(POIGuidePointItem) % std::to_string(lon) % std::to_string(lat)).str();

                std::string poiItem = (boost::format(POIItemXml)
                    % std::to_string(poiId)
                    % name
                    % address
                    % std::to_string(direction)
                    % distance
                    % Unit6
                    % name
                    % std::to_string(lon)
                    % std::to_string(lat)
                    % std::to_string(1)
                    % poiGuidePointStr
                    % address
                    % strTelephone).str();

                poiItemStr += poiItem;
                ++poiCount;
                VR_LOGI("getPOIInfoFromWebData:[%d] [%s]\n", __LINE__, poiItem.c_str());
            }
        }
        if (0 < poiCount) {
            retStr = (boost::format(POIListXml) % std::to_string(poiCount) % poiItemStr).str();
            xmlStr =  retStr;
            RETURN(true);
        }
    }
    xmlStr =  retStr;
    RETURN(false);
}

bool VR_CNDialogEngine::getInformationFromWebData(const std::string& strData, std::string& xmlStr)
{
    VR_LOGD_FUNC();
    std::string strRetHead = "<list id=\"%1%\">"
                                "<header>"
                                            "<pageSize>0</pageSize>"
                                            "<startIndex>0</startIndex>"
                                            "<count>%2%</count>"
                                        "</header>"
                                        "<items>"
                                            "%3%"
                                        "</items>"
                                "</list>";
    std::string strInfoItem = "<item>"
                                  "<content>%1%</content>"
                                  "<tts>%2%</tts>"
                              "</item>";
    std::string retStr = "";
    VR_LOGI("getInformationFromWebData:%s[%d]\n", strData.c_str(), __LINE__);

    rapidjson::Document jsonDoc;
    jsonDoc.Parse<0>(strData.c_str());
    if (jsonDoc.HasParseError()) {
        VR_LOGE("information parse error [%d]\n", __LINE__);
        RETURN(false);
    }

    if (!jsonDoc.HasMember("data")) {
        VR_LOGE("information parse error [%d]\n", __LINE__);
        RETURN(false);
    }

    const rapidjson::Value& objData = jsonDoc["data"];
    if (objData.HasMember("code")) {
        const rapidjson::Value& valState = objData["code"];
        int iState = valState.GetInt();
        if (0 != iState) {
            VR_LOGI("Info From webSearch Error :[%d]\n", iState);
            RETURN(false);
        }
    }

    if (objData.HasMember("result")) {
        const rapidjson::Value& valResult = objData["result"];
        if (!valResult.IsArray()) {
            VR_LOGE("DE parser the intention from ASR:[%d]\n", __LINE__);
            xmlStr = retStr;
            RETURN(false);
        }
        std::string itemList = "";
        int itemCount = 0;
        for (rapidjson::SizeType idx = 0; idx < valResult.Size(); ++idx) {
            const rapidjson::Value& objArray = valResult[idx];
            if (objArray.IsObject()) {
                const rapidjson::Value& valContent = objArray["content"];
                const rapidjson::Value& valTts = objArray["tts"];
                std::string strContent = valContent.GetString();
                std::string strTts = valTts.GetString();
                std::string infoItem = (boost::format(strInfoItem) % strContent % strTts).str();
                itemList += infoItem;
                itemCount++;
            }
        }
        if (0 >= itemCount) {
            VR_LOGE("The web result itemCount error\n");
            xmlStr =  retStr;
            RETURN(false);
        }
        std::stringstream ss;
        ss << _asrResultId;
        _asrResultId++;
        retStr = (boost::format(strRetHead) % ss.str() % std::to_string(itemCount) % itemList).str();
        xmlStr =  retStr;
        RETURN(true);
    }
    xmlStr =  retStr;
    RETURN(false);
}

void VR_CNDialogEngine::handleGetWebSearchResult(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    std::string resultStr = _dataProcessor.getWebSearchResult();
    _interManager->notifyOpResult("getWebSearchResult", resultStr);
    // _interManager->assignData("getWebSearchResult_result", resultStr);
    // _interManager->receiveEvent("done.getWebSearchResult", resultStr);
}

void VR_CNDialogEngine::handleGetCommunicationStatus(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
   if (m_pConfigureIF->getHybridVRFlag()) {
        std::string requestCommuStatus = "<action agent=\"communication\" op=\"queryGBookStatus\"></action>";
        m_strCommunicationStatus = "";
        m_bCommunicationStatusFlag = true;
        requestAction(requestCommuStatus);
        return;
    }
    std::string resultStr = "<action-result agent=\"communication\" op=\"getCommunicationStatus\"><code/></action-result>";
    _interManager->receiveEvent("done.getCommunicationStatus", resultStr);
}

void VR_CNDialogEngine::preprocessPhoneTypeResult(pugi::xml_node &result)
{
    VR_LOGD_FUNC();
    // get first phonetype and remove same contact name
    pugi::xml_node listNode = result.select_node("//list").node();
    pugi::xml_node itemsNode = listNode.child("items");
    pugi::xml_node countNode = result.select_node("//count").node();

    pugi::xml_node itemNode = itemsNode.first_child();
    if (itemNode.empty()) {
        return;
    }
    std::string firstPhoneType = itemNode.child("contact_phone_type").text().as_string();
    int firstPhoneTypeConfidence = itemNode.child("contact_phone_type").attribute("confidence").as_int();

    // remove same contact id
    VoiceSet<std::string>::type contactIDSet;
    while (!itemNode.empty()) {
        std::string contactID = itemNode.child("contact_id").text().as_string();
        // get formal contact name
        std::string reqMsg = "<data><id>" + contactID + "</id></data>";
        std::string contactName;
        _dataAccessorManager->getInfo("getContactName", reqMsg, contactName);
        itemNode.child("contact_name").text().set(contactName.c_str());
        // check removable
        pugi::xml_node removeNode;
        if (contactIDSet.end() != contactIDSet.find(contactID)) {
            removeNode = itemNode;
        }
        else {
            contactIDSet.insert(contactID);
        }
        itemNode = itemNode.next_sibling();
        if (!removeNode.empty()) {
            itemsNode.remove_child(removeNode);
        }
    }

    // remove same contact name
    itemNode = itemsNode.first_child();
    VoiceSet<std::string>::type contactNameSet;
    while (!itemNode.empty()) {
        std::string contactName = itemNode.child("contact_name").text().as_string();
        pugi::xml_node removeNode;
        if (contactNameSet.end() != contactNameSet.find(contactName)) {
            removeNode = itemNode;
        }
        else {
            contactNameSet.insert(contactName);
        }
        itemNode = itemNode.next_sibling();
        if (!removeNode.empty()) {
            itemsNode.remove_child(removeNode);
        }
    }

    // get full contact id by search DB and set the max confidence phonetypeid
    // count max == 5
    int count = 0;
    pugi::xml_node newItemsNode = listNode.append_child("items");
    itemNode = itemsNode.first_child();
    while (!itemNode.empty() && count < 5) {
        std::string contactName = itemNode.child("contact_name").text().as_string();
        std::string reqMsg = "<data><name>" + contactName + "</name></data>";
        std::string response;
        _dataAccessorManager->getInfo("getContactIds", reqMsg, response);
        if (!response.empty()) {
            pugi::xml_document msgDoc;
            msgDoc.load_string(response.c_str());
            pugi::xpath_node_set nodeSets = msgDoc.select_nodes("//id");
            pugi::xpath_node_set::iterator nodeIt = nodeSets.begin();
            while (nodeIt != nodeSets.end() && count < 5) {
                pugi::xml_node itemTmpNode = newItemsNode.append_copy(itemNode);
                pugi::xml_node contactIdNode = itemTmpNode.child("contact_id");
                contactIdNode.text().set(nodeIt->node().text().as_string());
                contactIdNode.attribute("confidence").set_value("9999"); // set confidence to max
                pugi::xml_node typeNode = itemTmpNode.child("contact_phone_type");
                typeNode.text().set(firstPhoneType.c_str());
                typeNode.attribute("confidence").set_value(firstPhoneTypeConfidence);
                ++count;
                ++nodeIt;
            }
        }
        itemNode = itemNode.next_sibling();
    }
    listNode.remove_child(itemsNode);
    countNode.text().set(count);
}


int VR_CNDialogEngine::transformPhoneTypeID(int iflytekTypeId)
{
    /*
    iflytek phone type id:
    00,01,02,03  means mobile, home, work, other
    10,11,12,13  means mobile2, home2, work2, other2
    20, ...
    ...     ,33
    DE phone type id:
    0, 1, 2, 3  means mobile, home, work, other
    4, 5, 6, 7  means mobile1, home1, work1, other1
    8, ...
    ...    , 19
    */

    if (0 <= iflytekTypeId && iflytekTypeId <= 3) {
        return iflytekTypeId;
    }

    int typeId = 3; // default

    int typeCode = iflytekTypeId % 10;
    int typeIndex = iflytekTypeId / 10 + 1;

    bool isInRange = (2 <= typeIndex && typeIndex <= 4) && (0 <= typeCode && typeCode <= 3) && (iflytekTypeId <= 33);
    if (isInRange) {
        typeId = typeIndex * 4 + typeCode;
    }
    else {
        VR_LOGE("iflytek phonetype is out of range, return default type \"other\"");
    }

    return typeId;
}

void VR_CNDialogEngine::notifyStartFinishAfterCheck()
{
    VR_LOGD_FUNC();
    if (m_DMStarted && INIT_NONE != m_asrInitStateFirstTime && LOAD_NONE != m_agentLoadStateFirstTime) {
        VR_LOGD("NotifyStartFinishAfterCheck ASR and Agent OK");
        requestAction("<action agent=\"destatus\"  op=\"notifyStartFinish\"></action>");
        if (INIT_SUCCESS == m_asrInitStateFirstTime && LOAD_SUCCESS == m_agentLoadStateFirstTime) {
            resourceStateChange("init", ResourceState::READY);
        }
        else {
            resourceStateChange("init", ResourceState::GRAMMAR_NOT_READY);
        }
    }
}

void VR_CNDialogEngine::handleAsrResult(const std::string &asrResult)
{
    VR_LOGD_FUNC();
    std::string name = "done." VR_ASR_EVT_RESULT;

    if (!_asrReturnBeepPlayed) {
        _asrReturnBeepPlayed = true;
        reqPlayBeep(RETURN_BEEPPATH);
    }

    std::string xml = asrResult;
    VR_LOGP("recognition result, content:[%s]", xml.c_str());
    if (xml.empty()) {
        VR_LOGI("asr result is empty");
        // add check the gbook status when speech time out 16-207-1-00609 start
        if (m_pConfigureIF->getHybridVRFlag() && ("NoError" != m_strCommunicationStatus)) {
            std::string event = "done.queryGBookStatus";
            std::string strGbookStatusCode = "<code>" + m_strCommunicationStatus + "</code>";
            _currentIntention = std::pair<std::string, std::string>(event, strGbookStatusCode);
            _interManager->receiveEvent(event, strGbookStatusCode);
            return;
        }
        // add end
        responseAsrError();
        return;
    }

    std::string intention = parseAsrToIntention(xml);
    if (intention.empty()) {
        VR_LOGD("Hybrid mode[%d] communication error is [%s]", m_pConfigureIF->getHybridVRFlag(), m_strCommunicationStatus.c_str());
        if (m_pConfigureIF->getHybridVRFlag() && ("NoError" != m_strCommunicationStatus)) {
            std::string event = "done.queryGBookStatus";
            intention = "<code>" + m_strCommunicationStatus + "</code>";
            _currentIntention = std::pair<std::string, std::string>(event, intention);
            _interManager->receiveEvent(event, intention);
            return;
        }
        responseAsrError();
        return;
    }

    _dataProcessor.updateListByAsr(intention);
    _currentIntention = std::pair<std::string, std::string>(name, intention);
    _interManager->receiveEvent(name, intention);
}

bool VR_CNDialogEngine::isEventNeedHandle(uscxml::Event *pEvent)
{
    // Event Check
    // If Event From Interpreter, And CloseSession Has Finished. Event Don't Need Handle, Return FALSE.
    if (!pEvent->getOrigin().compare(0, strlen(VR_INTERPRETER_EVENT_TYPE), VR_INTERPRETER_EVENT_TYPE)) {
        if (SESSION_QUITING == _sessionState || SESSION_QUITING_WAITING == _sessionState) {
            VR_LOGD("DE has Closession, %s Event Don't Need Send", pEvent->getOrigin().c_str());
            return false;
        }
    }
    return true;
}

void VR_CNDialogEngine::requestGrammarAfterCheck()
{
    VR_LOGD_FUNC();
    if (m_DMStarted && nullptr != _asrEngine) {
        requestGrammar();
    }
}

void VR_CNDialogEngine::processStartedNotify()
{
    VR_LOGD_FUNC();
    m_DMStarted = true;
    notifyStartFinishAfterCheck();
    requestGrammarAfterCheck();
}

void VR_CNDialogEngine::requestGrammar()
{
    VR_LOGD_FUNC();
    VoiceSet<std::string>::type mediaGrammarIDSet;
    for (VoiceList<std::string>::iterator it = m_regenerationContextIDList.begin();
        it != m_regenerationContextIDList.end();
        ++it) {
        if (!_dataAccessorManager->checkContextIDAvailable(*it)) {
            continue;
        }

        VR_GrammarRequest request;
        request.agent = _dataAccessorManager->getContextIDAgent(*it);

        pugi::xml_document msgDoc;
        pugi::xml_node actionNode = msgDoc.append_child("action");
        actionNode.append_attribute("agent").set_value(request.agent.c_str());
        actionNode.append_attribute("op").set_value("requestGrammar");
        if ("media" == request.agent) {
            std::string index = _dataAccessorManager->getContextIDIndex(*it);
            if (mediaGrammarIDSet.find(index) != mediaGrammarIDSet.end()) {
                continue;
            }
            mediaGrammarIDSet.insert(index);
            actionNode.append_attribute("grammarID").set_value(index.c_str());
            request.grammarID = index;
        }

        m_grammarRequestList.push_back(request);
        std::ostringstream oss;
        actionNode.print(oss);
        requestAction(oss.str());
    }

    onRequestGrammarFinish();
}

void VR_CNDialogEngine::onRequestGrammarFinish()
{
    VR_LOGD_FUNC();

    VoiceList<VR_GrammarRequest>::iterator it = m_grammarRequestList.begin();
    while (it != m_grammarRequestList.end()) {
        if ("media" == it->agent) {
            _dataAccessorManager->removeGrammarDiffMsg(it->agent, it->grammarID);
        }
        ++it;
    }

    _dataAccessorManager->setGenerateGrammarEnable(true);
    _dataAccessorManager->triggerGenerateGrammar();

    m_regenerationContextIDList.clear();
    m_grammarRequestList.clear();
}

/* EOF */
