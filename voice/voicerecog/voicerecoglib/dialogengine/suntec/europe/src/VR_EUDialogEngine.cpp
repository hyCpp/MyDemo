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
#include "VR_InitPersonDate.h"
#include "VR_DEDataManager.h"
#include "VR_EUDialogEngine.h"
#include <boost/bind.hpp>
#include <boost/assign/list_of.hpp>
#include "uscxml/messages/SendRequest.h"
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <math.h>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "Vr_Asr_Audio_In.h"
#include "VC_WavFile.h"

#include <ncore/NCStartPerformance.h>
#include "VR_Configure.h"
#include "VR_DataAccessor.h"
#include "VR_DECommon.h"
#include "VR_MsgDefine.h"
#include "VR_AudioInWriter.h"
#include "VR_AudioInLocalWriter.h"

using namespace nutshell;
using namespace std;
using namespace N_Vr;
using namespace N_Asr;

#define CHECK_NULL(handle, msg) do { \
    if (handle == NULL) { \
        VR_LOGE(msg); \
        return; \
    } \
} while(0)

#define CCHECK_NULL_IM() CHECK_NULL(_interManager, "interpreter manager is null")

VoiceMap<std::string, std::string>::type cmdEUTagMap = boost::assign::map_list_of
        ("phone_dial_by_number_shortcut_PHRASE", "phone_number")
        ("phone_number_loop_PHRASE", "phone_number");


// VR_EUDialogEngine
VR_EUDialogEngine::VR_EUDialogEngine(VR_ConfigureIF* configureInstance)
    : _dataProcessor(configureInstance->getDataPath())
    , _continueAsr(false)
    , _asrResultId(0)
    , _asrIsRunning(false)
    , _changeLanguage(false)
    , _fullUpdateNavi(false)
    , _languageId("")
    , _needBargein(false)
    , _interrputRecv(false)
    , _resourceState(ResourceState::GRAMMAR_NOT_READY)
    , _countrySearchId("001")
    , _bHasQuitVRApp(true)
    , _bHasCloseSession(true)
    , _bNaviFullData(false)
    , _bMusicFilter(false)
    , m_isAsrReturnPlayBeep(false)
    , m_sessionState(false)
    , _canceltype(CANCEL_NONE)
    , m_asrInitStateFirstTime(INIT_NONE)
    , m_agentLoadStateFirstTime(LOAD_NONE)
    , m_receiveAsrResult(false)
    , m_DMStarted(false)
    , m_bVDataError(false)
    , m_isCanceledByDE(false)
    , m_isAsrSpeechEndRecv(false)
    , m_isIpod(false)
    , m_routeExistStatus("false")
    , m_screenContent("")
{
    VR_LOGD_FUNC();
    _interManager = NULL; // new in start
    _thread = NULL; // new in start
    _actionListener = NULL; // receive in start
    _isRunning = false;
    _engine = NULL;
    _addressForAsr.s_bAddress = false;
    m_pConfigureIF = configureInstance;
    m_pDECommonIF = VR_new VR_DECommon();
    _dataAccessorManager = NULL;
    _deDataManager.reset(VR_new VR_DEDataManager(m_pConfigureIF->getDataPath()));
    m_pDEMessageBuilder = VR_new VR_DEMessageBuilder(m_pDECommonIF);
    m_pIntentionParser = VR_new VR_IntentionParser(m_pDECommonIF);

    _audioInSource = boost::shared_ptr<VR_AudioInSource>(VR_new VR_AudioInSource());

    VR_AudioInWriter* audioInWriter;
    if (m_pConfigureIF->getInnerAvcLanConnectStatus()) {
        audioInWriter = NULL;
    }
    else {
        audioInWriter = VR_new VR_AudioInLocalWriter();
    }
    _audioInSource->setAudioInWriter(audioInWriter);
}


bool
VR_EUDialogEngine::Initialize(VR_DialogEngineListener* listerner, const VR_Settings &settings)
{
    VR_LOGD_FUNC();
    dumpTestcaseSwitch(true);
    tthread::lock_guard<tthread::recursive_mutex> lock(_mutex);
    if (NULL == _interManager) {
        _interManager = VR_new VR_InterpreterManager(this, this, m_pDECommonIF);
    }

    m_pDECommonIF->init(m_pConfigureIF);

    if (NULL == _dataAccessorManager) {
        _dataAccessorManager = VR_new VR_DataAccessorManagerEU(this, m_pDECommonIF, m_pConfigureIF);
    }

    {
        _actionListener = listerner;
        m_lastPlayed = PlayedType::NONE;

        m_startBeepSeqId = VR_ACTION_SEQ_ID_INVALID;
        m_startBeepLatency = 160;

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
        boost::function<void(const std::string &)> callback = boost::bind(&VR_EUDialogEngine::onStateUpdate, this, _1);
        _dataAccessorManager->setUpdateStateCallback(callback);
        boost::function<void(const std::string &)> notifyCallback = boost::bind(&VR_EUDialogEngine::requestAction, this, _1);
        _dataAccessorManager->setNotifyCallback(notifyCallback);

        _dataAccessorManager->setCountryID("1");
        _deDataManager->initData(m_pDECommonIF->getVRLanguage());
        initPhoneTypeName();
        _dataProcessor.initData(m_pDECommonIF->getVRLanguage());
        loadHandleEventFuncMap();
        initGrammarDeactiveMap();
    }

    std::ifstream vrThresholdFile("/pdata/vrThreshold");
    if (vrThresholdFile) {
        vrThresholdFile >> VR_RECOGNIZE_CONFIDENCE_THRESHOLD;
        VR_LOGD("VR Confidence Threshold: %d", VR_RECOGNIZE_CONFIDENCE_THRESHOLD);
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

void VR_EUDialogEngine::UnInitialize()
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
    if (_engine != NULL) {
        delete _engine;
        _engine = NULL;
    }
    if (_dataAccessorManager != NULL) {
        delete _dataAccessorManager;
        _dataAccessorManager = NULL;
    }
    m_bVDataError = false;
    dumpTestcaseSwitch(false);
}

void VR_EUDialogEngine::clearTempData()
{
    VR_LOGD_FUNC();
    tthread::lock_guard<tthread::recursive_mutex> lock(_mutex);
    {
        _stateSatck.empty();
        clearTmpState();
        _currentStateBackChangeAgent.stateName = "";
        _currentStateBackChangeAgent.stateAttr = "";
        _dataProcessor.clearListDataFromDM();
    }
}

std::string VR_EUDialogEngine::getHints(const std::string& hintsParams)
{
    VR_LOGD_FUNC();
    VR_LOGP("DE receive getHints event 215-2-300");

    pugi::xml_document doc;
    if (!doc.load_string(VR_MSG_EU_HINTS)) {
        VR_LOGI("load VR_MSG_EU_HINTS as xml failed.");
        return "";
    }
    pugi::xml_node displayNode = doc.select_node("//display").node();
    processContentBeforeDisplay(displayNode);

    std::ostringstream oss;
    displayNode.print(oss);
    std::string resultStr = oss.str();
    VR_LOGP("DE reply hints result 215-2-301");
    return resultStr;
}

bool VR_EUDialogEngine::Start()
{
    VR_LOGD_FUNC();
    if (_isRunning) {
        VR_LOGI("dialogEngine has started! we will restart it!");
        Stop();
    }
    VR_LOGD("dialogengine start thread=====");
    _isRunning = true;
    int nice = m_pDECommonIF->getTargetNice("vr_ctrl");
    VR_LOGD("set vr_ctrl priority %d", nice);
    _thread = VR_new tthread::thread(VR_EUDialogEngine::run, this, "vr_ctrl", nice); // start thread
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

void VR_EUDialogEngine::doStop()
{
    if (!_isRunning) {
        VR_LOGI("dialogEngine has stop");
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
            delete _thread;
            _thread = NULL;
        }
        else {
            {
                static bool errorLogMark = false;
                if (!errorLogMark) {
                    errorLogMark = true;
                    VR_ERROR(VOICE_VR_ERR_INVALID_OP, VR_DE_OPTIONDATA_NONE, "can't stop from itself thread, stop failed");
                }
            }
            return;
        }
    }

    if (_actionListener != NULL) {
        resourceStateChange("init", ResourceState::GRAMMAR_NOT_READY);
        _actionListener->OnStopped();
    }
    return;
}

void VR_EUDialogEngine::Stop()
{
    VR_LOGD_FUNC();
    doStop();

}

VR_EUDialogEngine::~VR_EUDialogEngine()
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
    if (_interManager != NULL) {
        delete _interManager;
        _interManager = NULL;
    }
    if (_engine != NULL) {
        delete _engine;
        _engine = NULL;
    }
    if (_dataAccessorManager != NULL) {
        delete _dataAccessorManager;
        _dataAccessorManager = NULL;
    }
}


bool VR_EUDialogEngine::updateGrammar(N_Vr::N_Asr::C_Request_Context_List_Update& updateMsg)
{
    VR_LOGD_FUNC();
    if (_engine) {
       VR_LOGD("Category ID is %s", updateMsg.m_string_Id_Category.c_str());
        _engine->Context_List_Update(updateMsg);
        RETURN(true);
    }
    else {
        VR_LOGE("ASR Engine point is Null");
        RETURN(false);
    }
}

void VR_EUDialogEngine::setGrammarActive(const std::string &contextID, bool isActive, const VoiceList<std::string>::type &ruleIDList)
{
    spC_Request_Activate spActive = _grammarDeactiveMap[contextID];
    if (nullptr == spActive) {
        VR_LOGE("unhandle contextID [%s]", contextID.c_str());
        return;
    }
    for (VoiceList<std::string>::const_iterator it = ruleIDList.cbegin();
        it != ruleIDList.cend();
        ++it) {
        if (isActive) {
            spActive->m_list_string_Id_Rule.remove(*it);
        }
        else {
            spActive->m_list_string_Id_Rule.push_back(*it);
        }
    }
    spActive->m_list_string_Id_Rule.sort();
    spActive->m_list_string_Id_Rule.unique();
}

void VR_EUDialogEngine::updateGrammarCategoryFinish(const std::string &category)
{
    VR_LOGD_FUNC();
    uscxml::Event event;
    event.setName("updateGrammarCategoryFinish");
    event.setContent(category);
    postEvent(event);
}

void VR_EUDialogEngine::updateGrammarCategoryFailed(const std::string &sourceID, const std::string &size)
{
    VR_LOGD_FUNC();
    uscxml::Event event;
    event.setName("updateGrammarCategoryFailed");
    event.setSendId(sourceID);
    event.setContent(size);
    postEvent(event);
}

void VR_EUDialogEngine::updateGrammarFinish()
{
    VR_LOGD_FUNC();
    uscxml::Event event;
    event.setName("updateGrammarFinish");
    postEvent(event);
}

void VR_EUDialogEngine::loadGracenoteFinish(const std::string &loadResult)
{
    VR_LOGD_FUNC();
    uscxml::Event event;
    event.setName("loadGracenoteFinish");
    event.setContent(loadResult);
    postEvent(event);
}

void VR_EUDialogEngine::grammarSpaceNotEnough(const std::string &infoMsg)
{
    VR_LOGD_FUNC();
    uscxml::Event event;
    event.setName("grammarSpaceNotEnough");
    event.setContent(infoMsg);
    postEvent(event);
}

void VR_EUDialogEngine::deleteMediaGrammar(const std::string &deviceID, const std::string &infoMsg)
{
    uscxml::Event event;
    event.setName("deleteMediaGrammar");
    event.setSendId(deviceID);
    event.setContent(infoMsg);
    postEvent(event);
}

void VR_EUDialogEngine::saveGrammarFile(const std::string &sourceID)
{
    uscxml::Event event;
    event.setName("saveGrammarFile");
    event.setSendId(sourceID);
    postEvent(event);
}

void VR_EUDialogEngine::removeSpaceInAsrResult(pugi::xml_node asrNode)
{
    VR_LOGD_FUNC();
    std::string cmdAllValue = asrNode.select_node("//node[@name='intent']").node().attribute("value").as_string();
    VoiceMap<std::string, std::string>::iterator it = cmdEUTagMap.find(cmdAllValue.c_str());

    if (it != cmdEUTagMap.end()) {
        /* remove space in sentence */
//        pugi::xml_node sentenceNode = asrNode.select_node("//node[@name='intent']").node();
//        std::string sentenceStr = sentenceNode.attribute("sentenceValue").as_string();
//        std::string sentenceText;
//        const char* ptr1 = sentenceStr.c_str();
//        while (*ptr1 != '\0') {
//            if (*ptr1 != ' ') {
//                sentenceText.push_back(*ptr1);
//            }
//            ++ptr1;
//        }
//        sentenceNode.attribute("sentenceValue").set_value(sentenceText.c_str());

        /* remove space in */
        pugi::xpath_node_set nodeSets = asrNode.select_nodes(std::string("//" + it->second).c_str());
        pugi::xpath_node_set::iterator itNode = nodeSets.begin();
        while (itNode != nodeSets.end()) {
            std::string txt = itNode->node().text().as_string();
            std::string resultTxt;
            const char* ptr = txt.c_str();
            while (*ptr != '\0') {
                if (*ptr != ' ') {
                    resultTxt.push_back(*ptr);
                }
                ++ptr;
            }
            itNode->node().text().set(resultTxt.c_str());
            ++itNode;
        }
    }
}


std::string VR_EUDialogEngine::parseAsrToIntention(pugi::xml_node doc)
{
    VR_LOGD_FUNC();
    // set the maxConf and maxCmdName
    int maxConfOut = 0;
    pugi::xml_node maxNodeOut;

    pugi::xpath_node_set docNodesOut = doc.select_nodes("//node[@name='hypothesis']");
    if (docNodesOut.begin() == docNodesOut.end()) {
        VR_LOGE("can't find node with name=hypothesis!");
        RETURN("");
    }
    // get the best node
    pugi::xpath_node_set::iterator itNodeOut = docNodesOut.begin();
    while (itNodeOut != docNodesOut.end()) {
        int currentConfOut = itNodeOut->node().attribute("confidence").as_int();
        if (maxConfOut < currentConfOut) {
            maxConfOut = currentConfOut;
            maxNodeOut = itNodeOut->node();
        }
        else if (maxConfOut == currentConfOut) {
            int currentConfIn = itNodeOut->node().first_child().attribute("confidence").as_int();
            int maxConfIn = maxNodeOut.first_child().attribute("confidence").as_int();
            if (maxConfIn < currentConfIn) {
                maxNodeOut = itNodeOut->node();
            }
            else {
            }
        }
        else {
        }
        ++itNodeOut;
    }
    // add to limit confidence
    if (maxConfOut < VR_RECOGNIZE_CONFIDENCE_THRESHOLD) {
        VR_LOGI("the maxConfOut is %d, hasn't meet %d", maxConfOut, VR_RECOGNIZE_CONFIDENCE_THRESHOLD);
        RETURN("");
    }
    //
    string maxCmdName = maxNodeOut.first_child().attribute("value").as_string();
    // get nodes to containItemNodes.children
    int itemCount = 0;
    pugi::xpath_node_set docNodesIn = doc.select_nodes("//node[@name='intent']");
    if (docNodesIn.begin() == docNodesIn.end()) {
        VR_LOGE("can't find node with name=intent!");
        RETURN("");
    }

    pugi::xml_document containItemNodes;
    pugi::xpath_node_set::iterator itNodeIn = docNodesIn.begin();
    while (itNodeIn != docNodesIn.end()) {
        std::string itCmdName = itNodeIn->node().attribute("value").as_string();
        int confidence = itNodeIn->node().attribute("confidence").as_int();
        if (itCmdName == maxCmdName
            && confidence >= VR_RECOGNIZE_CONFIDENCE_THRESHOLD) {
            containItemNodes.append_copy(itNodeIn->node());
            ++itemCount;
        }
        else {
        }
        ++itNodeIn;
    }
    // build node
    pugi::xml_document resultDoc;
    pugi::xml_node result = resultDoc.append_child("node");
    result.set_name("node");
    result.append_attribute("name").set_value("intent");
    result.append_attribute("value").set_value(maxCmdName.c_str());
    std::string sentenceStr = maxNodeOut.attribute("value").as_string();
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
    pugi::xml_node conItemNode = containItemNodes.first_child();
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


    VoiceList<std::string>::type itemList;
    // filter music result
    if ("media_enter_artist_with_slot_PHRASE" == maxCmdName
        || "media_artist_name_PHRASE" == maxCmdName) {
        _dataAccessorManager->handleMusicResult(result, "artist");
        getDisplayItemList(result, "artist", itemList);
    }
    if ("media_enter_album_with_slot_PHRASE" == maxCmdName
        || "media_album_name_PHRASE" == maxCmdName) {
        _dataAccessorManager->handleMusicResult(result, "album");
        getDisplayItemList(result, "album", itemList);
    }
    if ("media_playlist_with_slot_PHRASE" == maxCmdName
        || "media_playlist_name_PHRASE" == maxCmdName) {
        _dataAccessorManager->handleMusicResult(result, "playlist");
        getDisplayItemList(result, "playlist", itemList);
    }
    if ("media_enter_title_with_slot_PHRASE" == maxCmdName
        || "media_title_name_PHRASE" == maxCmdName) {
        _dataAccessorManager->handleMusicResult(result, "song");
        getDisplayItemList(result, "song", itemList);
    }
    if ("media_enter_genre_with_slot_PHRASE" == maxCmdName
        || "media_genre_name_PHRASE" == maxCmdName) {
        _dataAccessorManager->handleMusicResult(result, "genre");
        getDisplayItemList(result, "genre", itemList);
    }
    else {
        // do not hanlde
    }

    _dataAccessorManager->generateDisplayGrammar(itemList);

    if ("hmi_picklist_dynamic_PHRASE" == maxCmdName) {
        maxCmdName = "hmi_picklist_line_PHRASE";
        result.attribute("value").set_value(maxCmdName.c_str());
        pugi::xml_node itemNode = result.select_node("//item").node();
        itemNode.child("line_id").set_name("number");
        itemNode.child("line_content").set_name("content");
        std::string lineID = itemNode.child("number").text().as_string();
        result.attribute("sentenceValue").set_value(lineID.c_str());
    }

    if ("phone_dial_name_shortcut_PHRASE" == maxCmdName
        || "phone_entry_PHRASE" == maxCmdName) {
        // remove same contact name, get full contact id by search DB and set the max confidence phonetypeid
        preprocessPhoneTypeResult(result);
        m_pIntentionParser->mergePhoneContactTypeEU(result);
    }
    else if ("tuner_station_PHRASE" == maxCmdName
        || "tuner_station_name_input_PHRASE" == maxCmdName) {
        pugi::xpath_node_set stationIDs = result.select_nodes("//station_id");
        pugi::xpath_node_set::iterator it = stationIDs.begin();
        while (it != stationIDs.end()) {
            int stationGrammarID = it->node().text().as_int();
            std::string band = _dataAccessorManager->getStationBand(stationGrammarID);
            it->node().text().set(_dataAccessorManager->getStationID(stationGrammarID));
            it->node().parent().append_child("station_band").text().set(band.c_str());
            ++it;
        }
        preprocessTunnerSameResult(result);
    }
    else if ("media_playlist_with_slot_PHRASE" == maxCmdName
        || "media_playlist_name_PHRASE" == maxCmdName) {
        pugi::xpath_node_set playlistIDNodes = result.select_nodes("//playlist_id");
        pugi::xpath_node_set::iterator it = playlistIDNodes.begin();
        while (it != playlistIDNodes.end()) {
            std::string playlistID = it->node().text().as_string();
            if (!checkPlaylistAvailable(playlistID)) {
                itemsNode.remove_child(it->node().parent());
            }
            ++it;
        }
        pugi::xpath_node_set itemNodeSet = itemsNode.select_nodes(".//item");
        if (itemNodeSet.empty() && "media_playlist_name_PHRASE" == maxCmdName) {
            result.append_attribute("no_asr_result").set_value(1);
        }
    }
    else if ("media_artist_name_PHRASE" == maxCmdName
        || "media_album_name_PHRASE" == maxCmdName) {
        _dataAccessorManager->handleAsrMusicStepByStepResult(result);
    }
    else if ("media_oneshot_name_PHRASE" == maxCmdName
        || "media_oneshot_with_slot_PHRASE" == maxCmdName) {
        _dataAccessorManager->handleMusicOneshotResult(result);
    }
    else if (0 == maxCmdName.compare(0, 15, "phone_category_")) {
        size_t pos = maxCmdName.find_last_of('_');
        std::string phoneType = maxCmdName.substr(15, pos - 15);
        int phoneTypeID = -1;
        if ("mobile" == phoneType) {
            phoneTypeID = 0;
        }
        else if ("home" == phoneType) {
            phoneTypeID = 1;
        }
        else if ("work" == phoneType) {
            phoneTypeID = 2;
        }
        else if ("other" == phoneType) {
            phoneTypeID = 3;
        }
        else {
            // do not handle
        }

        if (!_dataAccessorManager->checkPhoneTypeAvailable(phoneTypeID)) {
            RETURN("");
        }
    }
    else {
        // do not handle
    }

    removeSpaceInAsrResult(result);
    m_pIntentionParser->changePOINameByPOIId(result, _dataAccessorManager);
    bool ret = m_pIntentionParser->filterIntention(result, _bNaviFullData, _bMusicFilter);
    if (!ret) {
        VR_LOGD("remove some node by filterRadioFreqGen");
        RETURN("");
    }

    // recount the count
    size_t count = result.select_nodes("//item").size();
    headerNode.child("count").text().set(count);

    // to string
    std::ostringstream oss;
    result.print(oss);
    std::string intentionStr = oss.str();
    VR_LOGI("DE parser the intention from ASR:[%s]\n", intentionStr.c_str());
    RETURN(intentionStr);
}

int VR_EUDialogEngine::generateSeqID()
{
    ++_seqId;
    if (VR_ACTION_SEQ_ID_INVALID == _seqId) {
        return ++_seqId;
    }
    else {
        return _seqId;
    }
}

int VR_EUDialogEngine::requestAction(const std::string& action)
{
    if (_actionListener == NULL) {
        {
            static bool errorLogMark = false;
            if (!errorLogMark) {
                errorLogMark = true;
                VR_ERROR(VOICE_VR_ERR_NULL_PARAM, VR_DE_OPTIONDATA_NONE, "_actionListener is NULL");
            }
        }
        return VR_ACTION_SEQ_ID_INVALID;
    }

    VR_LOG_TO_FILE("requestAction", action);
    tthread::lock_guard<tthread::recursive_mutex> lck(_mutexRequest);
    int seqID = generateSeqID();
    _actionListener->OnRequestAction(action, seqID);
    RETURN(seqID);
}

void VR_EUDialogEngine::requestVR(const std::string& grammar)
{
    VR_LOGP("request VR, dialogID[%s]", grammar.c_str());
    m_isAsrReturnPlayBeep = false;

    if ((_grammarStr == "navi_speak_state_{country}")
        || (_grammarStr == "navi_speak_city_{country}")
        || (_grammarStr == "navi_speak_street_{country}")) {
        _addressForAsr.s_bAddress = true;
    }

    std::string newGrammar = changeGrammarIDForAsr(grammar);
    VR_LOGD("requestVR grammarId = [%s], new grammarId= [%s]", grammar.c_str(), newGrammar.c_str());

    if (NULL == _engine) {
        {
            static bool errorLogMark = false;
            if (!errorLogMark) {
                errorLogMark = true;
                VR_ERROR(VOICE_VR_ERR_INVALID_PARAM, VR_DE_OPTIONDATA_NONE, "Asr engine is null");
            }
        }
        responseAsrError();
        return;
    }

    if (_asrIsRunning) {
        // normal case can't enter
        {
            static bool errorLogMark = false;
            if (!errorLogMark) {
                errorLogMark = true;
                VR_ERROR(VOICE_VR_ERR_INVALID_OP, VR_DE_OPTIONDATA_NONE, "ASR Engine is running!!!");
            }
        }
        return;
    }

    // Mark bargein or not
    bool isBargeInOverBeepSupported = m_pDECommonIF->getVROverBeep(); // || _needBargein;
    _audioInSource->setAudioMode(isBargeInOverBeepSupported ? VC_AUDIO_MODE_BARGEIN : VC_AUDIO_MODE_NORMAL);

    _audioInSource->Prepare();
    VR_LOGP("audioIn source prepare()");
    m_audioInPrepareTime = ::GetTickCount();

    VR_LOGP("play 'start' beep");
    std::string beepPath = m_pConfigureIF->getDataPath() + START_BEEPPATH;
    m_startBeepSeqId = reqDmPlayBeep(beepPath);

    C_Request_Recognize req;
    if (isBargeInOverBeepSupported) {

        const int Asr_Absolute_Threshold_L1_Value = -3800;
        const int Asr_Sensitivity_L1_Value = 25;
        const int Asr_Minspeech_L1_Value = 120;

        C_Request_Param_Set param;
        param.m_e_Param_Type = E_Param_Type_Fx_Sensitivity;
        param.m_i_Value = Asr_Sensitivity_L1_Value;
        _engine->Param_Set(param);

        param.m_e_Param_Type = E_Param_Type_Fx_Absolute_Threshold;
        param.m_i_Value = Asr_Absolute_Threshold_L1_Value;
        _engine->Param_Set(param);

        param.m_e_Param_Type = E_Param_Type_Fx_Minspeech;
        param.m_i_Value = Asr_Minspeech_L1_Value;
        _engine->Param_Set(param);

        req.m_b_Barge_In = 1;
    }

    if (_addressForAsr.s_bAddress) {
        spC_Request_Activate sp_reqActive(VR_new C_Request_Activate());
        VR_LOGD("requestVR for address, id = %d, state|city|street = %d", _addressForAsr.s_id, _addressForAsr.s_enAddressType);
        switch (_addressForAsr.s_enAddressType) {
        case AddressType::address_state:
            sp_reqActive->m_string_Field_Start = "state";
            sp_reqActive->m_string_Field_Stop = "state";
            sp_reqActive->m_string_Field_Activate_From = "state";
            break;

        case AddressType::address_city:
            sp_reqActive->m_string_Field_Start = "city";
            sp_reqActive->m_string_Field_Stop = "city";
            sp_reqActive->m_string_Field_Activate_From = "state";
            sp_reqActive->m_vector_i_User_Data_Lo.push_back(_addressForAsr.s_id);
            break;

        case AddressType::address_street:
            sp_reqActive->m_string_Field_Start = "street";
            sp_reqActive->m_string_Field_Stop = "street";
            sp_reqActive->m_string_Field_Activate_From = "city";
            sp_reqActive->m_vector_i_User_Data_Lo.push_back(_addressForAsr.s_id);
            break;

        default:
            break;
        }
        _addressForAsr.s_bAddress = false;

        std::stringstream inter;
        inter << std::setw(3) << std::setfill('0') << _countrySearchId;
        std::string sretStr = inter.str();
        sp_reqActive->m_string_Id_Context = "ctx_navi_address_oneshot_" + sretStr;
        VR_LOGD("sp_reqActive->m_string_Id_Context = %s", sp_reqActive->m_string_Id_Context.c_str());

        req.m_list_spo_Request_Activate.push_back(sp_reqActive);
    }

    // set deactive grammar
    for (VoiceMap<std::string, spC_Request_Activate>::iterator it = _grammarDeactiveMap.begin();
        it != _grammarDeactiveMap.end();
        ++it) {
        if (nullptr != it->second) {
            req.m_list_spo_Request_Activate.push_back(it->second);
        }
    }

    req.m_string_Id_Party = "Origin";
    req.m_string_Id_Dialog = newGrammar;
    req.m_spo_Audio_In = _audioInSource;
    req.m_function_On_Event_Phase = boost::bind(&VR_EUDialogEngine::onAsrPhase, this, _1);
    req.m_function_On_Event_Notify = boost::bind(&VR_EUDialogEngine::onAsrNotify, this, _1);
    req.m_function_On_Event_Result = boost::bind(&VR_EUDialogEngine::onAsrResult, this, _1);
    req.m_b_Measure_Time = false;

    VR_LOGP("ASR recognize start", "335");
    m_isAsrSpeechEndRecv = false;
    E_Result ret = _engine->Recognize_Start(req);
    if (ret == E_Result::E_Result_Succeeded) {
        _asrIsRunning = true;
        VR_LOGI("Recognize_Start success");
    }
    else {
        _audioInSource->UnPrepare();
        responseAsrError();
        {
            static bool errorLogMark = false;
            if (!errorLogMark) {
                errorLogMark = true;
                VR_ERROR(VOICE_VR_ERR_INVALID_OP, VR_DE_OPTIONDATA_NONE,     "Recognize_Start failed");
            }
        }
    }
}

std::string VR_EUDialogEngine::namelistToStr(uscxml::Event& reqCopy)
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

std::string VR_EUDialogEngine::namelistToStr(uscxml::Event& reqCopy, const std::string& dataName)
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

void VR_EUDialogEngine::parserVRState(pugi::xml_node& doc)
{
    VR_LOGD_FUNC();
    m_interpreterState = doc.select_node("//state").node().child_value();


    // -------------parser tag------------------
    std::string tag = doc.select_node("//tag").node().child_value();
    std::string vr_StateSecond = _deDataManager->getState2Text(tag);
    pugi::xml_node tagNode = doc.select_node("//tag").node();
    tagNode.first_child().set_value(vr_StateSecond.c_str());


    if (m_interpreterState == "idle"
        || m_interpreterState == "prompt"
        || m_interpreterState == "promptWithBargein"
        || m_interpreterState == "promptPlaying"
        || m_interpreterState == "listening"
        || m_interpreterState == "paused") {
        // -------------parser prompt------------------
        std::string vr_StateSecond2 = _deDataManager->getState2Text(tag);
        pugi::xml_node promptNode = doc.select_node("//prompt").node();
        promptNode.first_child().set_value(vr_StateSecond2.c_str());

        pugi::xml_node resultNode = doc.select_node("//result").node();
        resultNode.text().set("");
    }
    else if (m_interpreterState == "processing"
        || "speechResult" == m_interpreterState) {
        pugi::xml_node promptNode = doc.select_node("//prompt").node();
        promptNode.text().set("");
    }
}

VoiceList<std::string>::type VR_EUDialogEngine::parserPrompt(pugi::xml_node& doc)
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

    VoiceMap<std::string, std::string>::type dicMap;
    pugi::xpath_node_set xpathNodes = doc.select_nodes("//param");
    pugi::xpath_node_set::iterator xpathNode = xpathNodes.begin();
    while (xpathNode != xpathNodes.end()) {
        std::string kk1 = xpathNode->node().attribute("key").as_string();
        std::string vv1 = xpathNode->node().attribute("value").as_string();
        // add for phone
        if (doc.select_node("//tts").node() != NULL) {
            if ("number_sequence" == kk1) {
                vv1 = "\033\\tn=phone\\" + vv1 + "\033\\tn=normal\\";
            }
        }
        // alter gracenote phoneme
        if (_dataAccessorManager->getGracenoteSupportedEnable()) {
            if (doc.select_node("//tts").node() != NULL) {
                vv1 = getMediaGracenotePhoneme(kk1, vv1);
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


void VR_EUDialogEngine::parserPromptReplaceKeyWithValue(std::string& promptStr, VoiceMap<std::string, std::string>::type& dicMap)
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

void VR_EUDialogEngine::loadHandleEventFuncMap()
{
    _asyncEventHandles["asr"] = &VR_EUDialogEngine::handleAsr;
    _asyncEventHandles["tts"] = &VR_EUDialogEngine::handleTTSOnly;
    _asyncEventHandles["ttsWithAsr"] = &VR_EUDialogEngine::handleTTSWithAsr;
    _asyncEventHandles["display"] = &VR_EUDialogEngine::handleDisplay;
    _asyncEventHandles["fetchItem"] = &VR_EUDialogEngine::handleFetchItem;
    _asyncEventHandles["closeSession"] = &VR_EUDialogEngine::handleCloseSession;
    _asyncEventHandles["resendAsrEvent"] = &VR_EUDialogEngine::handleResendAsrEvent;
    _asyncEventHandles["changeAgent"] = &VR_EUDialogEngine::handleChangeAgent;
    _asyncEventHandles["backAgent"] = &VR_EUDialogEngine::handleBackAgent;
    _asyncEventHandles["interrupt"] = &VR_EUDialogEngine::handleInterrupt;
    _asyncEventHandles["hardKey"] = &VR_EUDialogEngine::handleReturnKeyOrButton;
    _asyncEventHandles["buttonPressed"] = &VR_EUDialogEngine::handleReturnKeyOrButton;
    _asyncEventHandles["action"] = &VR_EUDialogEngine::handleAction;
    _asyncEventHandles["asrStartover"] = &VR_EUDialogEngine::handleAsrStartover;
    _asyncEventHandles["asrImmediately"] = &VR_EUDialogEngine::handleAsrImmediately;
    _asyncEventHandles["popCurrentAgentStacks"] = &VR_EUDialogEngine::handlePopCurrentAgentStacks;
    _asyncEventHandles["isAddrSupported"] = &VR_EUDialogEngine::handleIsAddrSupported;
    _asyncEventHandles["startOver"] = &VR_EUDialogEngine::handleStartOverEvent;
    _asyncEventHandles["initAsrFactory_Event"] = &VR_EUDialogEngine::handleInitAsr;
    _asyncEventHandles["initInterpretes_Event"] = &VR_EUDialogEngine::handleInitInterpreter;
    _asyncEventHandles["sendMessage_Event"] = &VR_EUDialogEngine::handleSendMessage;
    _asyncEventHandles["quitVRApp"] = &VR_EUDialogEngine::handleQuitVRApp;
    _asyncEventHandles["updateGrammarCategoryFinish"] = &VR_EUDialogEngine::handleUpdateGrammarCategoryFinish;
    _asyncEventHandles["updateGrammarCategoryFailed"] = &VR_EUDialogEngine::handleUpdateGrammarCategoryFailed;
    _asyncEventHandles["updateGrammarFinish"] = &VR_EUDialogEngine::handleUpdateGrammarFinish;
    _asyncEventHandles["grammarSpaceNotEnough"] = &VR_EUDialogEngine::handleGrammarSpaceNotEnough;
    _asyncEventHandles["deleteMediaGrammar"] = &VR_EUDialogEngine::handleDeleteMediaGrammar;
    _asyncEventHandles["saveGrammarFile"] = &VR_EUDialogEngine::handleSaveGrammarFile;
    _asyncEventHandles["agentLoadSuccessFirstTime"] = &VR_EUDialogEngine::handleAgentLoadedSuccessFirstTime;
    _asyncEventHandles["agentLoadFailFirstTime"] = &VR_EUDialogEngine::handleAgentLoadedFailFirstTime;
    _asyncEventHandles["initInterpreter"] = &VR_EUDialogEngine::handlePreInitInterpreter;
    _asyncEventHandles["changeSourceByName"] = &VR_EUDialogEngine::handleChangeSourceByName;
    _asyncEventHandles["loadGracenoteFinish"] = &VR_EUDialogEngine::handleLoadGracenoteFinish;
    // ASR events
    _asyncEventHandles[VR_ASR_EVENT_NAME] = &VR_EUDialogEngine::handleAsrEvent;

    _eventHandles["preInit"] = &VR_EUDialogEngine::handlePreUpdateData;
    _eventHandles["back"] = &VR_EUDialogEngine::handleBack;
    _eventHandles["doBack"] = &VR_EUDialogEngine::handleDoBack;
    _eventHandles["doUpdateDataModel"] = &VR_EUDialogEngine::handleDoUpdateDataModel;
    _eventHandles["receiveOpResult"] = &VR_EUDialogEngine::handleReceiveOpResult;
}

// Handle action result, such as Query action result
void VR_EUDialogEngine::handleReceiveOpResult(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    CCHECK_NULL_IM();
    _interManager->receiveOpResult();
}

void VR_EUDialogEngine::handleIsAddrSupported(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    std::string support = "true";
    std::string language = m_pDECommonIF->getVRLanguage();
    std::string region = m_pDECommonIF->getProductCountry();
    std::string countryId = _countrySearchId;
    if ((language == "en-gb") && (region == "HongkongMacau")
        && (countryId == "2")) {
        support = "false_macau_eng";
    }
    std::string xml = "<support>" + support + "</support>";
    _interManager->notifyOpResult("isAddrSupported", xml);
    // _interManager->assignData("isAddrSupported_result", xml);
    // _interManager->receiveEvent("done.isAddrSupported", xml);
}

void VR_EUDialogEngine::handlePopCurrentAgentStacks(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    clearSameAgentState();
}

void VR_EUDialogEngine::initGrammarDeactiveMap()
{
    _grammarDeactiveMap.clear();
    pugi::xml_document doc;
    if (!doc.load_string(VR_MSG_GRAMMAR_RULE_ID_EU)) {
        VR_LOGE("load Grammar RuleID failed");
        return;
    }
    pugi::xml_node grammarDeactiveNode = doc.select_node("//grammarDeactive").node();
    pugi::xml_node contextIDNode = grammarDeactiveNode.first_child();
    while (!contextIDNode.empty()) {
        std::string contextID(contextIDNode.name());
        if (!contextID.empty()) {
            spC_Request_Activate spActive(VR_new C_Request_Activate());
            spActive->m_string_Id_Context.assign(contextID);

            pugi::xml_node ruleIDNode = contextIDNode.first_child();
            while (!ruleIDNode.empty()) {
                std::string ruleID(ruleIDNode.text().as_string());
                if (!ruleID.empty()) {
                    spActive->m_list_string_Id_Rule.push_back(ruleID);
                }
                ruleIDNode = ruleIDNode.next_sibling();
            }
            _grammarDeactiveMap[contextID] = spActive;
        }
        contextIDNode = contextIDNode.next_sibling();
    }
}

void VR_EUDialogEngine::handleDoUpdateDataModel(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    if (_interManager != NULL) {
        _interManager->updateGlobalStates(); // update global state
    }
    else {
        VR_LOGE("interpreter manager is null");
    }
}

void VR_EUDialogEngine::handleTTSOnly(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    std::string xmlStr = namelistToStr(reqCopy, MODEL_TTS);
    VR_LOGD("####handleTTS Paramers: %s", xmlStr.c_str());
    if (xmlStr.empty()) {
        VR_LOGE("$modelTTS is empty");
    }
    else {
        pugi::xml_document doc;
        doc.load_string(xmlStr.c_str());
        VoiceList<std::string>::type txtList = parserPrompt(doc);

        int promptLevel = m_pConfigureIF->getVRPromptLevel();
        if (!promptLevel) {
            bool helpType = doc.select_node("//helpType").node().text().as_bool();
            if (!helpType) {
                // don't paly tts
                _interManager->receiveEvent("done.playTts", "");
                return;
            }
        }
        // std::string prompt = doc.select_node("//prompt").node().child_value();
        reqDmPlayTTS(txtList);
    }
}

void VR_EUDialogEngine::handleTTS(uscxml::Event& reqCopy)
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
        if (!promptLevel) {
            bool helpType = doc.select_node("//helpType").node().text().as_bool();
            if (!helpType) {
                // don't paly tts
                if (_continueAsr) {
                    VR_LOGI("start VR without prompt, grammar[%s]", _grammarStr.c_str());
                    _continueAsr = false;
                    requestVR(_grammarStr);
                }
                return;
            }
        }
        // std::string prompt = doc.select_node("//prompt").node().child_value();
        reqDmPlayTTS(txtList);
    }
}

void VR_EUDialogEngine::handleAsrStartover(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    if (!_engine) {
        VR_LOGE("asr engine is null");
        return;
    }
    m_isCanceledByDE = true;
    _engine->Recognize_Cancel();
    requestVR(_grammarStr);
    // cancel asr,receive reply, start asr
}

void VR_EUDialogEngine::handleAsrImmediately(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    // stop tts, receive reply, start asr
    _continueAsr = false;
    handleTTSStop(reqCopy);
    requestVR(_grammarStr);
}

void VR_EUDialogEngine::handleTTSStop(uscxml::Event& reqCopy)
{
    reqDmStopTTS();
}

void VR_EUDialogEngine::interruptActionResult()
{
    VR_LOGD_FUNC();

    bool isAsrStopped = !_asrIsRunning;
    bool isTTsStopped = _listStopTTsSeq.empty();
    bool isBeepPlayed = _listPlayBeepSeq.empty();

    if (isTTsStopped && isAsrStopped && isBeepPlayed && _interrputRecv) {
        _interManager->receiveEvent("done.interrupt", "");
        VR_LOGD("done.interrupt send");
        _interrputRecv = false;
    }
    else {
        VR_LOGD("done.interrupt can't send, isTTsStopped = %d, isBeepPlayed = %d, isAsrStopped = %d",
                isTTsStopped, isBeepPlayed, isAsrStopped);
    }
}

void VR_EUDialogEngine::handleInterrupt(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    _continueAsr = false;
    _needBargein = false;
    _interrputRecv = true;
    if (_engine) {
        m_isCanceledByDE = true;
        _engine->Recognize_Cancel();
    }
    handleTTSStop(reqCopy);

    interruptActionResult();
}

void VR_EUDialogEngine::handleAsr(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    _needBargein = false;
    _continueAsr = false;
    doHandleAsr(reqCopy);
}

void VR_EUDialogEngine::doHandleAsr(uscxml::Event& reqCopy)
{
    _grammarStr = parseAsrGrammar(reqCopy);
    requestVR(_grammarStr);
}

std::string VR_EUDialogEngine::parseAsrGrammar(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    std::string xmlStr = namelistToStr(reqCopy, MODEL_ASR);
    if (xmlStr.empty()) {
        VR_LOGE("$modelAsr is empty");
        return "";
    }
    else {
        pugi::xml_document doc;
        doc.load_string(xmlStr.c_str());
        pugi::xpath_node languageNode = doc.select_node("//language");
        std::string languageStr = languageNode.node().child_value();
        VR_LOGI("languageStr [%s] has not used", languageStr.c_str());
        pugi::xpath_node grammarNode = doc.select_node("//grammar");
        std::string grammarStr = grammarNode.node().child_value();
        return grammarStr;
    }
}

void VR_EUDialogEngine::handleDisplay(uscxml::Event& reqCopy)
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
            string number = m_isIpod ? "1" : "0";
            iPodDisplay.text().set(number.c_str());
        }

        pugi::xml_node contentDisplay = docScxml.select_node("//content").node();
        if (contentDisplay) {
            m_screenContent = contentDisplay.text().as_string();
            VR_LOGD("m_screenContent is [%s]", m_screenContent.c_str());
        }

        // display need parserPrompt and addItemsToDisplayList
        parserPrompt(docScxml);
        // add items

        pugi::xml_node hintsNodeScxml = docScxml.select_node("//hints/list[@id]").node();
        if (NULL != hintsNodeScxml) {
            _dataProcessor.addItemsToHintsDispaly(hintsNodeScxml, true, m_pDECommonIF->getHybridVRFlag());
            // _dataProcessor.addItemsToDispaly(hintsNodeScxml);
        }
        else {
            pugi::xml_node selectListNode = docScxml.select_node("//selectList/list[@id]").node();
            if (NULL != selectListNode) {
                _dataProcessor.addItemsToDispaly(selectListNode);
                processContentBeforeDisplay(displayNodeScxml);
                processListBeforeDisplay(selectListNode);
            }
            else {
                VR_LOGI("screenDispaly has no hints or selectList");
            }
        }
    }
    else if (contentTypeStrScxml == "ShowPopupMessage") {
        VR_LOGD("this display content is ShowPopupMessage and need parserPrompt");
        // parser display
        parserPrompt(docScxml);
    }
    else if (contentTypeStrScxml == "VRState") {
        VR_LOGD("this display content is VRState and need parser tag and prompt");
        // tag , prompt
        parserVRState(docScxml);
    }
    else {
        VR_LOGD("this display is not ScreenDisplay, need't parserPrompt and addItemsToDisplayList");
    }

    std::stringstream oss;
    displayNodeScxml.print(oss);

    requestAction(oss.str());
    VR_LOGP("DE request to display, content:%s", contentTypeStrScxml.c_str());
}

void VR_EUDialogEngine::handleFetchItem(uscxml::Event& reqCopy)
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


void VR_EUDialogEngine::handleQuitVRApp(uscxml::Event& evt)
{
    VR_LOGD_FUNC();
    if (!_bHasQuitVRApp) {
        _canceltype = CANCEL_PENDING;
        handleCloseSession(evt);
        std::string msg;
        m_pDEMessageBuilder->buildQuitVRAppDisplay(msg);
        requestAction(msg);
        _bHasQuitVRApp = true;
    }
}

void VR_EUDialogEngine::handleUpdateGrammarCategoryFinish(uscxml::Event& evt)
{
    std::string category = evt.getContent();
    _dataAccessorManager->onUpdateGrammarCategoryFinish(category);
}

void VR_EUDialogEngine::handleUpdateGrammarCategoryFailed(uscxml::Event& evt)
{
    std::string sourceID = evt.getSendId();
    std::string size = evt.getContent();
    _dataAccessorManager->onUpdateGrammarCategoryFailed(sourceID, size);
}

void VR_EUDialogEngine::handleUpdateGrammarFinish(uscxml::Event& evt)
{
    _dataAccessorManager->onUpdateGrammarFinish();
}

void VR_EUDialogEngine::handleGrammarSpaceNotEnough(uscxml::Event& evt)
{
    std::string infoMsg = evt.getContent();
    _dataAccessorManager->handleGrammarSpaceNotEnough(infoMsg);
}

void VR_EUDialogEngine::handleDeleteMediaGrammar(uscxml::Event& evt)
{
    std::string deviceID = evt.getSendId();
    std::string infoMsg = evt.getContent();
    _dataAccessorManager->onDeleteMediaGrammar(deviceID, infoMsg);
}

void VR_EUDialogEngine::handleSaveGrammarFile(uscxml::Event& evt)
{
    std::string sourceID = evt.getSendId();
    _dataAccessorManager->onSaveGrammarToFile(sourceID);
}

void VR_EUDialogEngine::handleAgentLoadedSuccessFirstTime(uscxml::Event& evt)
{
    if (LOAD_NONE == m_agentLoadStateFirstTime) {
        m_agentLoadStateFirstTime = LOAD_SUCCESS;
        notifyStartFinishAfterCheck();
    }
}

void VR_EUDialogEngine::handleAgentLoadedFailFirstTime(uscxml::Event& evt)
{
    if (LOAD_NONE == m_agentLoadStateFirstTime) {
        m_agentLoadStateFirstTime = LOAD_FAIL;
        notifyStartFinishAfterCheck();
    }
}

void VR_EUDialogEngine::resourceStateChange(std::string type, ResourceState value)
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

void VR_EUDialogEngine::handleCloseSession(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    if (!_bHasCloseSession) {
        VR_LOGD("set _resourceState = not ready");
        // _resourceState = ResourceState::GRAMMAR_NOT_READY;
        resourceStateChange("engine", ResourceState::GRAMMAR_NOT_READY);
        std::string beepPath = m_pConfigureIF->getDataPath() + END_BEEPPATH;


        clearTempData(); // clear data
        // REVIEW: maybe can't receive quitVRApp from scxml
        _interManager->stopAgent(); // stop interpreter
        handleInterrupt(reqCopy);
        std::string msg;
        m_pDEMessageBuilder->buildVRStateQuitDisplay(msg);
        requestAction(msg);
        // _resourceState = ResourceState::READY;
        _bHasCloseSession = true;
        reqDmPlayBeep(beepPath);
        VR_LOGP("DE closeSession, then send quitVRApp");
    }

}

void VR_EUDialogEngine::handleFetchData(uscxml::Event& reqCopy)
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
    else {
        VR_LOG_TO_FILE("getInfo.eventName", eventName);
        VR_LOG_TO_FILE("getInfo.sendMsg", xmlStr);
        _dataAccessorManager->getInfo(eventName, xmlStr, listStr);
        VR_LOG_TO_FILE("getInfo.recMsg", listStr);

        if ("_getMusicByType" == eventName) {
            pugi::xml_document requestDoc;
            requestDoc.load_string(xmlStr.c_str());
            std::string type = requestDoc.select_node("//type").node().text().as_string();
            std::transform(type.begin(), type.end(), type.begin(), [](unsigned char c) { return std::tolower(c); });
            pugi::xml_document resultDoc;
            resultDoc.load_string(listStr.c_str());
            VoiceList<std::string>::type itemList;
            getDisplayItemList(resultDoc, type, itemList);
            _dataAccessorManager->generateDisplayGrammar(itemList);
        }

        pugi::xml_document retDoc;
        retDoc.load_string(listStr.c_str());
        if (eventName == "_sendVoiceID") {
            _addressForAsr.s_bAddress = true;
            std::string strType = retDoc.select_node("//type").node().text().as_string();
            if (strType == "STATE") {
                _addressForAsr.s_enAddressType = AddressType::address_state;
            }
            else if (strType == "CITY") {
                _addressForAsr.s_enAddressType = AddressType::address_city;
            }
            else if (strType == "STREET") {
                _addressForAsr.s_enAddressType = AddressType::address_street;
            }
            else {
                _addressForAsr.s_enAddressType = AddressType::address_state;
            }

            std::string id = retDoc.select_node("//voiceId").node().text().as_string();
            _addressForAsr.s_id = atoi(id.c_str());
        }
        else {
            _dataProcessor.updateListByDataAccessor(retDoc);
        }
    }
    // std::string dataName = eventName + "_result";
    _interManager->notifyOpResult(eventName, listStr);
    // _interManager->assignData(dataName, listStr); // assign data
    // _interManager->receiveEvent("done." + eventName, listStr);
    return;
}

void VR_EUDialogEngine::handleResendAsrEvent(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    VR_LOGI("resendAsrEvent and the eventName:%s\n", _currentIntention.first.c_str());
    VR_LOGI("resendAsrEvent and the eventIntention:%s\n", _currentIntention.second.c_str());
    _interManager->receiveEvent(_currentIntention.first, _currentIntention.second);
}

void VR_EUDialogEngine::handleAction(uscxml::Event& reqCopy)
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
    std::string xmlStr = namelistToStr(reqCopy); // get namelist data
    pugi::xml_document docNamelist;
    docNamelist.load_string(xmlStr.c_str());

    // add for navi languageId
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

void VR_EUDialogEngine::handleChangeSourceByName(uscxml::Event& reqCopy)
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

void VR_EUDialogEngine::handleLoadGracenoteFinish(uscxml::Event& evt)
{
    VR_LOGD_FUNC();
    // add the gracenoteFinish call back process
    std::string loadResult = evt.getContent();

    std::string magicString;
    bool loadStatus = false;

    pugi::xml_document doc;
    if (!doc.load_string(loadResult.c_str())) {
         VR_LOGE("pugi load string Error");
         _dataAccessorManager->notifyLoadGracenoteFinish(magicString, loadStatus);
    }
    pugi::xml_node node = doc.first_child();

    magicString = node.attribute(VR_DE_ASR_LOAD_RESULT_MAGICSTRING).as_string();
    std::string result = node.attribute(VR_DE_ASR_LOAD_RESULT_NAME).as_string();

    loadStatus = (result == VR_DE_ASR_LOAD_RESULT_TRUE);

    _dataAccessorManager->notifyLoadGracenoteFinish(magicString, loadStatus);
    return;
}

void VR_EUDialogEngine::changeSourceByNameFailed()
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
    processActionResultFromDM(actionFailedResult, -1);
}

void VR_EUDialogEngine::handleReturnKeyOrButton(uscxml::Event& reqCopy)
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

void VR_EUDialogEngine::handleTTSWithAsr(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    _needBargein = false;
    _continueAsr = true;
    _grammarStr = parseAsrGrammar(reqCopy);
    handleTTS(reqCopy); // wait for done.playTts,then handleAs
    return;
}

void VR_EUDialogEngine::requestService(const uscxml::Event& interalEvent)
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
        return;
    }
    // async
    else {
        postEvent(evt);
    }
}

void VR_EUDialogEngine::postEvent(uscxml::Event& reqCopy)
{
    // internal service and not in the same thread, it has lock insider
    tthread::lock_guard<tthread::recursive_mutex> lock(_mutex);
    uscxml::Event* reqCopyPoint = VR_new uscxml::Event(reqCopy);
    _internalServiceQueue.push(reqCopyPoint);
    _condVar.notify_all();
}

std::string VR_EUDialogEngine::buildInitParm()
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
    VR_LOGD("handlePreInitInterpreter : sessionStateNode set");
    pugi::xml_node sessionStateNode = preData.append_child("state");
    bool sessionState = sessionStateGet();
    VR_LOGD("handlePreInitInterpreter : sessionStateNode val = %d", sessionState);
    if (sessionState) {
        VR_LOGD("handlePreInitInterpreter : sessionStateNode set on");
        sessionStateNode.text().set("on");
    }
    else {
        VR_LOGD("handlePreInitInterpreter : sessionStateNode set off");
        sessionStateNode.text().set("off");
    }
    std::ostringstream oss;
    doc.print(oss);
    std::string preInitStr = oss.str();
    return preInitStr;
}

void VR_EUDialogEngine::handlePreUpdateData(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    this->doStateUpdate("", false);
    _interManager->updateGlobalStates(); // update global state
    uscxml::Event evt;
    evt.setName("initInterpreter");
    postEvent(evt);
}

void VR_EUDialogEngine::handlePreInitInterpreter(uscxml::Event& reqCopy)
{
    VR_LOGD_FUNC();
    const std::string preInitStr = buildInitParm();
    _interManager->receiveInitEvent("done.preInit", preInitStr);
}

// should invoke in uscxml thread
void VR_EUDialogEngine::handleDoBack(uscxml::Event& reqCopy)
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
        {
            static bool errorLogMark = false;
            if (!errorLogMark) {
                errorLogMark = true;
                VR_ERROR(VOICE_VR_ERR_INVALID_PARAM, VR_DE_OPTIONDATA_NONE, "the _backStateName is null!");
            }
        }
    }
}


void VR_EUDialogEngine::run(void* instance)
{
    VR_LOGD_FUNC();
    VR_EUDialogEngine* dialogEngine = static_cast<VR_EUDialogEngine*>(instance);
    dialogEngine->doRun();
}

void VR_EUDialogEngine::doRun()
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

void VR_EUDialogEngine::step(uscxml::Event* pEvent)
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
            handleFetchData(*pEvent);
        }
        else {
            if (_asyncEventHandles.find(evtName) == _asyncEventHandles.end()) {
                evtName = "action";
            }
            (this->*_asyncEventHandles[evtName])(*pEvent);
        }

    }
}

void VR_EUDialogEngine::handleChangeAgent(uscxml::Event& evt)
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

void VR_EUDialogEngine::handleSendMessage(uscxml::Event& evt)
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
        VR_LOGD("DM send to DE has error:%s!\n", eventStr.c_str());
    }
}

void VR_EUDialogEngine::handleAsrEvent(uscxml::Event& evt)
{
    VR_LOGD_FUNC();
    std::string evtName = evt.getRaw(); // raw store sub event name
    if (VR_ASR_EVT_LISTEN == evtName) {
      std::string name = "done." + evtName;
      _interManager->receiveEvent(name, "");
    }
    else if (VR_ASR_EVT_BOS == evtName) {
        if (_needBargein) {
            handleTTSStop(_currentEvent);
        }
        std::string name = "done." + evtName;
        _interManager->receiveEvent(name, "");
    }
    else if (VR_ASR_EVT_EOS == evtName) {
        std::string name = "done." + evtName;
        _interManager->receiveEvent(name, "");
        std::string beepPath = m_pConfigureIF->getDataPath() + RETURN_BEEPPATH;
        reqDmPlayBeep(beepPath);
        m_isAsrReturnPlayBeep = true;
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
            handleQuitVRApp(_currentEvent);
            _audioInSource->UnPrepare();
            VR_LOGE("asr result not recv");
            return;
        }
        m_isCanceledByDE = false;

        _asrIsRunning = false;
        VR_LOGD("asrIsRunning is false");

        if (_changeLanguage) {
            changeASREngineLanguage(_languageId);
            _languageId = "";
            _changeLanguage = false;
            resourceStateChange("engine", ResourceState::READY);
        }

        if (_fullUpdateNavi) {
            if (_engine) {
                VR_LOGD("before delete engine");
                delete _engine;
                _engine = NULL;
                VR_LOGD("delete engine");
            }

            uscxml::Event evt;
            evt.setName("navi_reset_asr");
            handleInitAsr(evt);

            _fullUpdateNavi = false;
        }

        sendEventToDETask(evt);

        interruptActionResult();
        replyCancelIfDEStopped();
    }
    else {
        VR_LOGE("Unknow ASR event type:%s", evtName.c_str());
    }
}

void VR_EUDialogEngine::handleStartOverEvent(uscxml::Event& evt)
{
    processStartOverEvent();
}

void VR_EUDialogEngine::handleInitInterpreter(uscxml::Event& evt)
{
    VR_LOGD_FUNC();
    if (_interManager != NULL) {
        // _interManager->registerScxml();
        _interManager->start();
    }
}

void VR_EUDialogEngine::handleInitAsr(uscxml::Event& evt)
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

    if (NULL == _engine) {
        if ("navi_reset_asr" == evt.getName()) {
            _dataAccessorManager->restartGenerateGrammar();
        }
        else {
            _dataAccessorManager->resetGrammarGeneration();
        }
        _dataAccessorManager->setGenerateGrammarEnable(false);

        C_Request_Factory o_Request_Factory;
        o_Request_Factory.m_e_Type_Engine = E_Type_Engine_Vocon;
        std::string language = m_pDECommonIF->getVRLanguage();
        std::string outLanguage;
        bool ret = m_pDECommonIF->getAsrSupportLanguage(language, outLanguage);
        if (ret) {
            _dataAccessorManager->resetAsrVMPStatus(outLanguage);
            o_Request_Factory.m_string_Id_Language = outLanguage;
        }
        else {
            {
                static bool errorLogMark = false;
                if (!errorLogMark) {
                    errorLogMark = true;
                    VR_ERROR(VOICE_VR_ERR_FILE_NOT_EXIST, VR_DE_OPTIONDATA_NONE, "asr data is error, can't find asr data");
                }
            }
            if (INIT_NONE == m_asrInitStateFirstTime) {
                m_asrInitStateFirstTime = INIT_FAIL;
                notifyStartFinishAfterCheck();
            }
            return;
        }

        VR_LOGD("VR_Language : %s, Mapped Language : %s", language.c_str(), outLanguage.c_str());

        o_Request_Factory.m_function_On_Event_Init_Notify = boost::bind(&VR_EUDialogEngine::onAsrInit, this, _1);
        _engine = C_Engine::Factory(o_Request_Factory);

        requestGrammarAfterCheck();

        NCSTARTPERF_START;
        NCSTARTPERF_OUT("VR", "ASR is ready");
        NCSTARTPERF_END;

        if (_engine != NULL) {
            resourceStateChange("engine", ResourceState::READY);
        }
        if (INIT_NONE == m_asrInitStateFirstTime) {
            if (_engine != NULL) {
                m_asrInitStateFirstTime = INIT_SUCCESS;
            }
            else {
                m_asrInitStateFirstTime = INIT_FAIL;
            }
            notifyStartFinishAfterCheck();
        }

        if ("navi_reset_asr" == evt.getName()) {
            _dataAccessorManager->triggerGenerateGrammar();
        }
    }
}

void VR_EUDialogEngine::clearSameAgentState()
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

void VR_EUDialogEngine::clearTmpState()
{
    VR_LOGD_FUNC();
    _tmpStateBack.agentName = "";
    _tmpStateBack.stateName = "";
    _tmpStateBack.stateAttr = "";
}

void VR_EUDialogEngine::handleBack(uscxml::Event& evt)
{
    VR_LOGD_FUNC();
    // _stateSatck
    _forward_back = FORBACK;
    if (_stateSatck.empty()) {
        VR_LOGI("stateStack is empty, process back, stop interpreter");
        uscxml::Event evt;
        evt.setName("quitVRApp");
        postEvent(evt);
        // handleCloseSession(_currentEvent);
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

void VR_EUDialogEngine::processStartOverEvent()
{
    VR_LOGD_FUNC();
    handleInterrupt(_currentEvent);
    if (NULL == _interManager) {
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

void VR_EUDialogEngine::sendCancelMsg()
{
    std::string msg;

    while (!optionList.empty()) {
        std::string option = optionList.front();
        VR_LOGD("sendCancelMsg : get option = %s", option.c_str());
        if (option.empty()) {
            msg = "<event-result name=\"cancel\"/>";
        }
        else {
            msg = "<event-result name=\"cancel\" option=\"" + option +"\"/>";
        }

        requestAction(msg);
        VR_LOGD("sendCancelMsg : msg = %s", msg.c_str());
        optionList.pop_front();
    }
    _canceltype = CANCEL_NONE;
    m_isCanceledByDE = false;
    clearTtsAndBeepList();
    m_lastPlayed = PlayedType::NONE;
    resourceStateChange("engine", ResourceState::READY);

    // notify grammar manager recognize end
    _dataAccessorManager->onRecognizeEnd();
}

void VR_EUDialogEngine::replyCancelIfDEStopped()
{
    VR_LOGD_FUNC();
    VR_LOGP("DE wait tts and asr stop 215-3-302");

    bool isAsrStopped = !_asrIsRunning;
    bool isTTsStopped = _listPlayTTsSeq.empty();
    bool isBeepPlayed = _listPlayBeepSeq.empty();

    if (isTTsStopped && isBeepPlayed && isAsrStopped && CANCEL_PENDING == _canceltype) {
        sendCancelMsg();
        VR_LOGP("DE asr and tts all stop ,quest DM close media 215-3-305");
    }
    else {
        VR_LOGD("wait beep %d, tts %d, asr %d", isBeepPlayed, isTTsStopped, isAsrStopped);
    }
}

void VR_EUDialogEngine::clearTtsAndBeepList()
{
    // clear tts/beep stop/play list
    VR_LOGD_FUNC();
    _listPlayTTsSeq.clear();
    _listPlayBeepSeq.clear();
    _listStopTTsSeq.clear();
    _listStopBeepSeq.clear();
}

bool VR_EUDialogEngine::isTtsCrash()
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

void VR_EUDialogEngine::processCancelEvent(std::string& option)
{
    VR_LOGD_FUNC();
    optionList.push_back(option);
    VR_LOGD("processCancelEvent : push option = %s", option.c_str());
    if ("ttscrash" == option) {
        clearTtsAndBeepList();
        VR_LOGD("clear Tts And Beep List");
    }

    if (_canceltype == CANCEL_NONE) {
        if (!_bHasQuitVRApp) {
            handleQuitVRApp(_currentEvent);
            replyCancelIfDEStopped();
        }
        else {
            sendCancelMsg();
            VR_LOGD("give up the cancel because startAgent has not send");
        }
    }
    else {
        // cancel type is PENDING, means DE is in quiting,
        // push the cancel to list, wait quit done
        if (!_asrIsRunning) {
            replyCancelIfDEStopped();
        }
        else {
            VR_LOGD("DE is quiting, push the cancel to list");
        }
    }
}

void VR_EUDialogEngine::processGetHintsEvent(const pugi::xml_node& eventNode)
{
    VR_LOGD_FUNC();
    VR_LOGP("DE receive getHints event 215-2");
    std::string agentName = eventNode.select_node("//agentName").node().child_value();
    std::string pageSizeStr = eventNode.select_node("//pageSize").node().child_value();
    int pageSize = atoi(pageSizeStr.c_str());
    std::string resultStr = _dataProcessor.getHintsData(agentName, pageSize, true, m_pConfigureIF->getHybridVRFlag(), true);
    VR_LOGP("DE reply hints result 215-2");
    requestAction(resultStr);
}

bool VR_EUDialogEngine::processPrepareAgentEvent(const pugi::xml_node& eventNode)
{
    VR_LOGD_FUNC();
    if (ResourceState::READY != _resourceState) {
        VR_LOGE("Can't process start agent in this state, Not Ready!!!");
        return false;
    }

    std::string agentName = eventNode.child_value("agent");
    VR_LOGP("DE receive PrepareAgent name:", agentName.c_str());
    _lastAgent = _currentAgent;
    _currentAgent = agentName;
    // the two parms used for preInit
    _forward_back = FORWARD;
    _resendEvent = false;
    // add for get countryId
    std::string action = "<action agent = \"navi\" op = \"requestDefaultInfo\">"
                         "</action>";
    requestAction(action);
    // set the VRApp is start
    _bHasCloseSession = false;
    _bHasQuitVRApp = false;
    bool isPrepare = _interManager->prepareAgent(agentName);
    if (!isPrepare) {
        // start failed, quit VRApp
        VR_LOGD("prepareAgent ERROR");
        handleQuitVRApp(_currentEvent);
        return false;
    }
    return true;
}

bool VR_EUDialogEngine::processStartAgentEvent(const pugi::xml_node& eventNode)
{
  VR_LOGD_FUNC();

  if (ResourceState::READY != _resourceState) {
      VR_LOGE("Can't process start agent in this state, Not Ready!!!");
      return false;
  }

  // notify grammar manager recognize begin
  _dataAccessorManager->onRecognizeBegin();

  std::string agentName = eventNode.child_value("agent");
  VR_LOGP("DE receive startAgent name:", agentName.c_str());
  _lastAgent = _currentAgent;
  _currentAgent = agentName;
  // the two parms used for preInit
  _forward_back = FORWARD;
  _resendEvent = false;
  // add for get countryId
  std::string action = "<action agent = \"navi\" op = \"requestDefaultInfo\">"
                       "</action>";
  requestAction(action);
  // set the VRApp is start
  _bHasCloseSession = false;
  _bHasQuitVRApp = false;

  const std::string preInitStr = buildInitParm();
  bool isStart = _interManager->startAgent(agentName, preInitStr);
  if (!isStart) {
      // start failed, quit VRApp
      VR_LOGD("startAgent ERROR");
      handleQuitVRApp(_currentEvent);
      return false;
  }
  return true;
}

void VR_EUDialogEngine::processStartDictationEvent()
{
    VR_LOGD_FUNC();
}

void VR_EUDialogEngine::processUpdateStateEvent(const std::string& eventStr)
{
    VR_LOGD_FUNC();
    _dataAccessorManager->updateState(eventStr);
}

void VR_EUDialogEngine::processSettingEvent(const pugi::xml_node& eventNode)
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

// for sessionStateChanged
void VR_EUDialogEngine::sessionStateSet(bool val)
{
    VR_LOGD_FUNC();
    VR_LOGP("sessionStateSet : sessionStateSet = %d", val);
    m_sessionState = val;
}

bool VR_EUDialogEngine::sessionStateGet()
{
    return m_sessionState;
}

void VR_EUDialogEngine::processEventFromDM(const std::string& eventStr)
{
    VR_LOGD_FUNC();
    pugi::xml_document doc;
    doc.load_string(eventStr.c_str());
    pugi::xml_node eventNode = doc.select_node("//event").node();
    std::string eventName = eventNode.attribute("name").as_string();
    VR_LOGP("Process event from DM, name=%s", eventName.c_str());

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
        VR_LOGD("prepare event recv");
        bool isPrepareState = processPrepareAgentEvent(eventNode);
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
        VR_LOGD("processEventFromDM : cancel option %s", option.c_str());
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
    else if (0 == strcmp(eventName.c_str(), "initialpersondata")) {
        processInitializePersonData();
        return;
    }
    else if (0 == strcmp(eventName.c_str(), "changeLanguage")) {
        processChangeLanguage(eventNode);
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
    else if (0 == strcmp(eventName.c_str(), "fullupdateNotify")) {
        std::string status = eventNode.select_node("//status").node().text().as_string();
        // off、navifulldata、finished
        if (status == "navifulldata") {
            _bNaviFullData = true;
            resetAsr(false);
        }
        else if (status == "finished") {
            _bNaviFullData = false;
            resetAsr(true);
        }
    }
    else {
        if (0 == strcmp(eventName.c_str(), "buttonPressed")) {
            if (_bNaviFullData) {
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

            // filter the button event in special VRState
            bool isAvailiable = checkButtonPressEventAvailiable(valueStr, m_interpreterState);
            if (!isAvailiable) {
                return;
            }

            eventNode.select_node("//keycode").node().append_attribute("content").set_value(m_screenContent.c_str());

            std::ostringstream oss;
            doc.print(oss);
            std::string buttonEventStr = oss.str();
            VR_LOGD("keycode add content, send to interManager = [%s]", buttonEventStr.c_str());
            _interManager->receiveEvent(eventName, buttonEventStr);
            return;

        }
        VR_LOGD("event recv = %s", eventName.c_str());
        _interManager->receiveEvent(eventName, eventStr);
        return;
    }
    responseEventResult(eventNode);
    return;
}

void VR_EUDialogEngine::responseEventResult(pugi::xml_node &eventNode)
{
    VR_LOGD_FUNC();
    std::stringstream ss;
    eventNode.set_name("event-result");
    eventNode.print(ss);
    requestAction(ss.str());
    VR_LOGP("Response event result, content:[%s]", ss.str().c_str());
    return;
}

/**
  * Reset ASR engine when map data is changed
  * @param mock: whether map data is valid
  * @comment it will be called by DE controller thread
  */
void VR_EUDialogEngine::resetAsr(bool enableNavi)
{
    VR_LOGD_FUNC();
    resourceStateChange("engine", ResourceState::GRAMMAR_NOT_READY);
    handleQuitVRApp(_currentEvent);

    VR_LOGI("reset asr engine, ptr=%p", _engine);
    m_pConfigureIF->setAsrMapDataPath(enableNavi);

    if (_asrIsRunning) {
        _fullUpdateNavi = true;
    }
    else {
        if (_engine) {
            VR_LOGD("before delete engine");
            delete _engine;
            _engine = NULL;
            VR_LOGD("delete engine");
        }

        uscxml::Event evt;
        evt.setName("navi_reset_asr");
        handleInitAsr(evt);
    }
}

void VR_EUDialogEngine::processInitializePersonData()
{
    VR_LOGD_FUNC();
    spVR_DE_Task task(VR_new VR_InitPersonDate());
    bool isCompleted = task->execute(this);
    if (!isCompleted) {
        _listDETaskSeq.push_back(task);
    }

    return;
}

void VR_EUDialogEngine::processChangeLanguage(const pugi::xml_node& languageNode)
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
    _dataAccessorManager->processChangeVrLanguage(language);
    std::string outLanguage;
    bool ret = m_pDECommonIF->getAsrSupportLanguage(language, outLanguage);
    _languageId = outLanguage;
    if (ret) {
        if (_engine) {
            resourceStateChange("engine", ResourceState::GRAMMAR_NOT_READY);
            if (!_asrIsRunning) {
                changeASREngineLanguage(_languageId);
                resourceStateChange("engine", ResourceState::READY);
            }
            else {
                _changeLanguage = true;
                m_isCanceledByDE = true;
                _engine->Recognize_Cancel();
            }
        }
        else {
            VR_LOGD("_engine is null");
            handleInitAsr(_currentEvent);
        }
    }
    else {
        {
            static bool errorLogMark = false;
            if (!errorLogMark) {
                errorLogMark = true;
                VR_ERROR(VOICE_VR_ERR_UNSUPPORTED, VR_DE_OPTIONDATA_NONE, "VRLanguage = %s, outLanguage = %s, asr is not support the language", language.c_str(), outLanguage.c_str());
            }
        }
    }
}

void VR_EUDialogEngine::processActionResultFromDM(const std::string& eventStr, int seqId)
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

    if (op == "requestDefaultInfo") {
        _countrySearchId = doc.select_node("//countryId").node().text().as_string();
        VR_LOGD("receive _countrySearchId = %s", _countrySearchId.c_str());
        _dataAccessorManager->setCountryID(_countrySearchId);
    }

    if ("done.queryRouteExist" == eventName) {
        VR_LOGD("done.queryRouteExist recv.");
        pugi::xml_node listNode = actionResultNode.node().select_node("//exist").node();
        m_routeExistStatus = listNode.text().as_string();
        return;
    }

    // scxml don't care playBeep
    if ("done.playBeep" != eventName
        && "done.stopTts" != eventName
        && "done.stopBeep" != eventName) {
        _interManager->notifyOpResult(op, resultStr, false);
    }

    processActionResult(eventName, seqId);
}

void VR_EUDialogEngine::processActionResult(const std::string& eventName, int seqId)
{
    VR_LOGP("Process done event, name=%s, seqId=%d", eventName.c_str(), seqId);
    if (eventName == "done.playTts") {
        {
            VR_LOGD("receive action-result of %s, seqId = %d", eventName.c_str(), seqId);
            if (!_listPlayTTsSeq.empty()) {
                VoiceList<int>::iterator it = _listPlayTTsSeq.end();
                --it;
                if (seqId == *it) {
                    _listPlayTTsSeq.clear();
                }
            }

            _listPlayTTsSeq.remove(seqId);
            _listStopTTsSeq.remove(seqId);

            VR_LOGD("remove it, seqId = %d, list size = %d", seqId, _listPlayTTsSeq.size());
            if (_listPlayTTsSeq.empty() && (PlayedType::TTS == m_lastPlayed)) {
                m_lastPlayed = PlayedType::NONE;
            }
        }

        interruptActionResult();
        replyCancelIfDEStopped();
    }

    if (eventName == "done.playBeep") {
        {
            VR_LOGD("receive action-result of %s, seqId = %d", eventName.c_str(), seqId);
            if (!_listPlayBeepSeq.empty()) {
                VoiceList<int>::iterator it = _listPlayBeepSeq.end();
                --it;
                if (seqId == *it) {
                    _listPlayBeepSeq.clear();
                }
            }

            _listPlayBeepSeq.remove(seqId);
            _listStopBeepSeq.remove(seqId);
            VR_LOGD("remove it, seqId = %d, list size = %d", seqId, _listPlayBeepSeq.size());
            if (_listPlayBeepSeq.empty() && (PlayedType::BEEP == m_lastPlayed)) {
                m_lastPlayed = PlayedType::NONE;
            }
        }

        interruptActionResult();
        replyCancelIfDEStopped();
    }

    if ((eventName == "done.stopTts") || (eventName == "done.stopBeep")) {
        if (eventName == "done.stopTts") {
            {
                VR_LOGD("receive action-result of %s, seqId = %d", eventName.c_str(), seqId);
                VR_LOGD("_listPlayTTsSeq.size = %d", _listPlayTTsSeq.size());
                if (!_listStopTTsSeq.empty()) {
                    _listPlayTTsSeq.remove(_listStopTTsSeq.front());
                    _listStopTTsSeq.pop_front();
                    VR_LOGD("after remove stoped seqId, _listPlayTTsSeq.size = %d", _listPlayTTsSeq.size());
                }
                else {
                    VR_LOGD("_listStopTTsSeq is empty");
                }
            }
        }

        if (eventName == "done.stopBeep") {
            int lastBeepSeqId = VR_ACTION_SEQ_ID_INVALID;
            {
                VR_LOGD("receive action-result of %s, seqId = %d", eventName.c_str(), seqId);
                VR_LOGD("_listPlayBeepSeq.size = %d", _listPlayBeepSeq.size());
                if (!_listStopBeepSeq.empty()) {
                    lastBeepSeqId = _listStopBeepSeq.front();
                    _listPlayBeepSeq.remove(lastBeepSeqId);
                    _listStopBeepSeq.pop_front();
                    VR_LOGD("after remove stoped seqId, _listPlayBeepSeq.size = %d", _listPlayBeepSeq.size());
                }
                else {
                    VR_LOGD("_listStopBeepSeq is empty");
                }
            }

            processStartBeepEnd(lastBeepSeqId);
        }

        interruptActionResult();
        replyCancelIfDEStopped();
    }

    if (eventName == "done.playTts") {
        if (_continueAsr) {
            VR_LOGI("done.playTts, start VR, grammar[%s]", _grammarStr.c_str());
            _continueAsr = false;
            requestVR(_grammarStr);
        }
    }
    else if (eventName == "done.playBeep") {
        VR_LOGP("receive action-result of done.playBeep");
        processStartBeepEnd(seqId);
    }
}

void VR_EUDialogEngine::processStartBeepEnd(int seqId)
{
    bool isBargeInOverBeepSupported = m_pDECommonIF->getVROverBeep();
    if (!isBargeInOverBeepSupported) {
        VR_LOGI("process start beep end");
        tthread::lock_guard<tthread::recursive_mutex> lock(_mutexStartBeep);
        if (m_startBeepSeqId == seqId) {
            int ignoreTime = ::GetTickCount() - m_audioInPrepareTime;
            ignoreTime += m_startBeepLatency;
            VR_LOGD("ignore time %dms", ignoreTime);
            _audioInSource->IgnoreAudioInByTime(ignoreTime);
            _condVarStartBeep.notify_all();
            m_startBeepSeqId = VR_ACTION_SEQ_ID_INVALID;
        }
    }
}

void VR_EUDialogEngine::processGrammarFromDM(const std::string& eventStr)
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

bool VR_EUDialogEngine::SendMessage(const std::string& eventStr, int actionSeqId)
{
    VR_LOGD_FUNC();
    if (NULL == _interManager) {
        {
            static bool errorLogMark = false;
            if (!errorLogMark) {
                errorLogMark = true;
                VR_ERROR(VOICE_VR_ERR_NULL_PARAM, VR_DE_OPTIONDATA_NONE, "has not start!");
            }
        }
        RETURN(false);
    }

    VR_LOGP("Received external event=%s, seqId=%d", eventStr.c_str(), actionSeqId);
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

void VR_EUDialogEngine::handleDoChangeAgent(uscxml::Event& evt)
{
    VR_LOGD_FUNC();
    std::string agentName = evt.getRaw();
    VR_LOGP("DE request changeagent 215-6-300 215-7-300");
    if (FORWARD == _forward_back) {
        this->saveTmpState(_currentStateBackChangeAgent.stateName, _currentStateBackChangeAgent.stateAttr);
    }

    _lastAgent = _currentAgent;
    _currentAgent = agentName;
    VR_LOGD("----------------change Anget----------------");
    bool isChange = _interManager->changeAgent(agentName);
    if (!isChange) {
        // change failed, quit VR
        handleQuitVRApp(_currentEvent);
    }
    VR_LOGP("DE changeagent over 215-6-301 215-7-301");
}

void VR_EUDialogEngine::handleBackAgent(uscxml::Event &evt)
{
    VR_LOGD_FUNC();
    std::string agentName = evt.getRaw();
    _lastAgent = _currentAgent;
    _currentAgent = agentName;
    VR_LOGD("----------------back Anget----------------");
    bool ok = _interManager->backAgent(agentName);

    if (!ok) {
        // change failed, quit VR
        VR_LOGD("Back Agent fail");
        handleQuitVRApp(_currentEvent);
    }
}

void VR_EUDialogEngine::pushTmpToStack()
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

void VR_EUDialogEngine::saveTmpState(const std::string& stateName, const std::string& stateAttr)
{
    VR_LOGD_FUNC();
    tthread::lock_guard<tthread::recursive_mutex> lock(_mutex);
    _tmpStateBack.agentName = _currentAgent;
    _tmpStateBack.stateName = stateName;
    _tmpStateBack.stateAttr = stateAttr;
    VR_LOGI("Save tmp agent for back, state:%s, attr:%s", stateName.c_str(), stateAttr.c_str());
}

std::string VR_EUDialogEngine::getTmpAttr()
{
    RETURN(_tmpStateBack.stateAttr);
}

void VR_EUDialogEngine::saveCurrentState(const std::string& stateName, const std::string& stateAttr)
{
    VR_LOGD_FUNC();
    tthread::lock_guard<tthread::recursive_mutex> lock(_mutex);
    _currentStateBackChangeAgent.stateName = stateName;
    _currentStateBackChangeAgent.stateAttr = stateAttr;
    VR_LOGP("Save current agent for back, state:%s, attr:%s", stateName.c_str(), stateAttr.c_str());
}

void VR_EUDialogEngine::onAsrInit(C_Event_Init_Notify const & notify)
{
    C_Event_Init_Notify::E_Event_Init_Type type = notify.Get_Event_Init_Notify_Type();
    if (C_Event_Init_Notify::E_Event_Init_Notify_Type_Context_Not_Exist == type
        || C_Event_Init_Notify::E_Event_Init_Notify_Type_Context_Invalid == type) {
        m_regenerationContextIDList.push_back(notify.Get_Event_Init_Notify_Param_String());
        VR_LOGD("ASR notify invalid contextID: %s", notify.Get_Event_Init_Notify_Param_String().c_str());
    }
    else {
        // do not handle
    }
}

void VR_EUDialogEngine::onAsrPhase(C_Event_Phase const& phase)
{
    C_Event_Phase::E_Event_Phase_Type msgType = phase.Get_Event_Phase_Type();
    switch (msgType) {
    case C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Proc_End:
    {
        VR_LOGP("ASR event: end, event is %d", msgType);
        uscxml::Event evt(VR_ASR_EVENT_NAME);
        evt.setRaw(VR_ASR_EVT_END);
        postEvent(evt);

        m_isAsrSpeechEndRecv = false;

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

    case C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Listen_Begin:
    {
        VR_LOGI("ASR event: listen begin, event is %d", msgType);
        bool isBargeInOverBeepSupported = m_pDECommonIF->getVROverBeep();
        if (!isBargeInOverBeepSupported) {
            tthread::lock_guard<tthread::recursive_mutex> lock(_mutexStartBeep);
            if (VR_ACTION_SEQ_ID_INVALID != m_startBeepSeqId) {
                _condVarStartBeep.wait(_mutexStartBeep);
            }
        }

        uscxml::Event evt(VR_ASR_EVENT_NAME);
        evt.setRaw(VR_ASR_EVT_LISTEN);
        postEvent(evt);

        break;
    }

    case C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Listen_End:
    {
        VR_LOGP("ASR event:End of listen, play 'end' beep, event is %d", msgType);
        break;
    }

    case C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_None:
    case C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Proc_Begin:
    case C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Proc_Fail:
    case C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Speech_End_Rec:
    {
        VR_LOGI("ASR event: %d, don't care", msgType);
        break;
    }

    default:
        VR_LOGD("ASR event unknown : %d", msgType);
        break;
    }
}

void VR_EUDialogEngine::onAsrNotify(C_Event_Notify const& notify)
{
    std::string retMsg;
    m_pDEMessageBuilder->buildVolumeDisplay(notify, retMsg);
    requestAction(retMsg);
}

void VR_EUDialogEngine::responseAsrError()
{
    std::string eventName = "failed.asr";
    _currentIntention = std::pair<std::string, std::string>(eventName, "");
    if (NULL != _interManager) {
        _interManager->receiveEvent(eventName, "");
    }
}

void VR_EUDialogEngine::onAsrResult(C_Event_Result const& result)
{
    VR_LOGD_FUNC();
    uscxml::Event evt(VR_ASR_EVENT_NAME);
    evt.setRaw(VR_ASR_EVT_RESULT);
    evt.setContent(result.Get_XML()->c_str());
    postEvent(evt);
}

int VR_EUDialogEngine::reqDmPlayBeep(const std::string& beepPath)
{
    VR_LOGD_FUNC();
    if (isTtsCrash()) {
        VR_LOGD("ttscrash don't need play or stop");
        return VR_ACTION_SEQ_ID_INVALID;
    }
    if (PlayedType::TTS == m_lastPlayed) {
            reqDmStopTTS();
    }
    std::string playBeepMsg;
    m_pDEMessageBuilder->buildPlayBeepAction(beepPath, playBeepMsg);

    VR_LOGP("Request play beep:[%s]", playBeepMsg.c_str());
    int seqId = requestAction(playBeepMsg);
    {
        _listPlayBeepSeq.push_back(seqId);
    }
    VR_LOGD("Request play beep , store seqId = %d into list", seqId);

    m_lastPlayed = PlayedType::BEEP;
    RETURN(seqId);
}

int VR_EUDialogEngine::reqDmPlayTTS(const VoiceList<std::string>::type& txtList)
{
    VR_LOGD_FUNC();
    if (isTtsCrash()) {
        VR_LOGD("ttscrash don't need play or stop");
        return VR_ACTION_SEQ_ID_INVALID;
    }
    if (PlayedType::TTS == m_lastPlayed) {
            reqDmStopTTS();
    }
    std::string ttsMsg;
    m_pDEMessageBuilder->buildPlayTTSAction(txtList, ttsMsg);
    int seqId = requestAction(ttsMsg);
    VR_LOGP("playTTS: %s", ttsMsg.c_str());
    {
        _listPlayTTsSeq.push_back(seqId);
    }
    m_lastPlayed = PlayedType::TTS;
    VR_LOGD("play tts , push seqId = %d into list", seqId);
    RETURN(seqId);
}


int VR_EUDialogEngine::reqDmPlayTTS(const std::string& ttsTxt)
{
    VR_LOGD_FUNC();
    if (isTtsCrash()) {
        VR_LOGD("ttscrash don't need play or stop");
        return VR_ACTION_SEQ_ID_INVALID;
    }
    if (PlayedType::TTS == m_lastPlayed) {
            reqDmStopTTS();
    }
    std::string ttsMsg;
    m_pDEMessageBuilder->buildPlayTTSAction(ttsTxt, ttsMsg);
    int seqId = requestAction(ttsMsg);
    VR_LOGP("playTTS: %s", ttsMsg.c_str());
    {
        _listPlayTTsSeq.push_back(seqId);
    }
    m_lastPlayed = PlayedType::TTS;
    VR_LOGD("play tts , push seqId = %d into list", seqId);
    RETURN(seqId);
}

int VR_EUDialogEngine::reqDmStopTTS()
{
    if (isTtsCrash()) {
        VR_LOGD("ttscrash don't need play or stop");
        return VR_ACTION_SEQ_ID_INVALID;
    }

    {
        if (_listPlayTTsSeq.empty()) {
            RETURN(VR_ACTION_SEQ_ID_INVALID);
        }
        int lastTTS = _listPlayTTsSeq.back();
        if (!_listStopTTsSeq.empty() && _listStopTTsSeq.back() == lastTTS) {
            RETURN(VR_ACTION_SEQ_ID_INVALID);
        }
        _listStopTTsSeq.push_back(lastTTS);
    }
    std::string stopTTSMsg;
    m_pDEMessageBuilder->buildStopTTSAction(stopTTSMsg);
    int seqId = requestAction(stopTTSMsg);
    if (PlayedType::TTS == m_lastPlayed) {
        m_lastPlayed = PlayedType::NONE;
    }
    RETURN(seqId);
}

std::string VR_EUDialogEngine::changeGrammarIDForAsr(const std::string &grammar)
{
    std::string newGrammar(grammar);
    std::string countryFlag("{country}");
    // std::string country("Australia");
    std::size_t pos = newGrammar.find(countryFlag);
    if (std::string::npos != pos) {
        // newGrammar.replace(pos, countryFlag.size(), country);
        std::stringstream inter;
        inter << std::setw(3) << std::setfill('0') << _countrySearchId;
        std::string sretStr = inter.str();
        newGrammar.replace(pos, countryFlag.size(), sretStr);
    }

    RETURN(newGrammar);
}

void VR_EUDialogEngine::setItemValue(pugi::xml_node doc, const std::string skey, const std::string svalue)
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

void VR_EUDialogEngine::onStateUpdate(const std::string &msgToDM)
{
    VR_LOGD_FUNC();
    doStateUpdate(msgToDM, true);
}

void VR_EUDialogEngine::doStateUpdate(const std::string &msgToDM, bool notifly)
{
    VR_LOGD_FUNC();
    // send msg to DM
//    if (!msgToDM.empty()) {
//        requestAction(msgToDM);
//    }

    // get stateMsg
    std::string stateMsg;
    _dataAccessorManager->getUpdateState(stateMsg);

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
    isNaviModelExist = isNaviModelExist && (!_bNaviFullData);

    // check country is Southeast-Asia and language = th
    std::string language = m_pDECommonIF->getVRLanguage();
    bool isAsia = (country == "Southeast-Asia") || (country == "Thailand");
    if (isAsia && (language != "th")) {
        isNaviModelExist = false;
    }

    m_pDECommonIF->setNaviModel(isNaviModelExist);
    std::ostringstream ss;
    ss << std::boolalpha << isNaviModelExist;
    std::string isStr = ss.str();
    setItemValue(doc, "NAVI_MODEL_EXIST", isStr);

    std::stringstream oss;
    doc.print(oss);

    VR_LOGD("doStateUpdate : notifyUpdateGlobalStates");
    _interManager->notifyUpdateGlobalStates(oss.str(), notifly);

    std::string activiedSourceId =  doc.select_node("//item[key='ACTIVED_SOURCE_ID']").node().attribute("value").as_string();
    VR_LOGD("ACTIVED_SOURCE_ID = %s", activiedSourceId.c_str());
    if ("0" == activiedSourceId) {
        _bMusicFilter = true;
    }
    else {
        _bMusicFilter = false;
    }
}

void VR_EUDialogEngine::initPhoneTypeName()
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

void VR_EUDialogEngine::preprocessPhoneTypeResult(pugi::xml_node &result)
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
    // count max == 12
    int count = 0;
    pugi::xml_node newItemsNode = listNode.append_child("items");
    itemNode = itemsNode.first_child();
    while (!itemNode.empty() && count < 12) {
        std::string contactName = itemNode.child("contact_name").text().as_string();
        std::string reqMsg = "<data><name>" + contactName + "</name></data>";
        std::string response;
        _dataAccessorManager->getInfo("getContactIds", reqMsg, response);
        if (!response.empty()) {
            pugi::xml_document msgDoc;
            msgDoc.load_string(response.c_str());
            pugi::xpath_node_set nodeSets = msgDoc.select_nodes("//id");
            pugi::xpath_node_set::iterator nodeIt = nodeSets.begin();
            while (nodeIt != nodeSets.end() && count < 12) {
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

// remove same tunner station, if  fm is same with dab remove the fm asr item
void VR_EUDialogEngine::preprocessTunnerSameResult(pugi::xml_node &result)
{
    VR_LOGD_FUNC();

    pugi::xml_node asrTunerItems = result.select_node("//items").node();
    VoiceVector<std::string>::type dabVec;

    pugi::xpath_node_set dabReults = asrTunerItems.select_nodes("//item[station_band='DAB']/station_name");
    pugi::xpath_node_set fmResults = asrTunerItems.select_nodes("//item[station_band='FM']");

    // if no dab or fm in result return
    if (0 ==dabReults.size() || 0 == fmResults.size()) {
        return;
    }

    // save the dab name in a vector
    for (auto it = dabReults.begin(); it != dabReults.end(); ++it) {
       std::string dabName = it->node().text().as_string();
       dabVec.push_back(dabName);
    }

    // use the dab result to remove fm same result
    for (auto it = fmResults.begin(); it != fmResults.end(); ++it) {
        pugi::xml_node item = it->node();
        std::string fmName  = item.select_node("station_name").node().text().as_string();
        auto found = find(dabVec.begin(), dabVec.end(), fmName);
        if (found != dabVec.end()) {
            asrTunerItems.remove_child(item);
        }
    }
}

void VR_EUDialogEngine::notifyStartFinishAfterCheck()
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

void VR_EUDialogEngine::processContentBeforeDisplay(pugi::xml_node &displayNode)
{
    pugi::xml_node screenTitleNode = displayNode.select_node("//screenTitle").node();
    if (screenTitleNode) {
        screenTitleNode.text().set(_deDataManager->getPrompts(screenTitleNode.text().as_string()).c_str());
    }

    pugi::xpath_node_set commandSet = displayNode.select_nodes("//selectList//commandItem/command");
    pugi::xpath_node_set::const_iterator it = commandSet.begin();
    while (it != commandSet.end()) {
        it->node().text().set(_deDataManager->getPrompts(it->node().text().as_string()).c_str());
        ++it;
    }
}

void VR_EUDialogEngine::processListBeforeDisplay(pugi::xml_node &listNode)
{
    pugi::xml_node startIndexNode = listNode.select_node("//startIndex").node();
    if (startIndexNode.empty()) {
        VR_LOGI("No startIndexNode!");
        return;
    }

    pugi::xml_node pageSizeNode = listNode.select_node("//pageSize").node();
    if (pageSizeNode.empty()) {
        VR_LOGI("No pageSizeNode!");
        return;
    }

    std::string startIndexString = startIndexNode.text().as_string();
    int startIndex = atoi(startIndexString.c_str());
    if (startIndex < 0) {
        startIndex = 0;
        VR_LOGI("the startIndex=%d,so we modify it to 0", startIndex);
    }
    size_t unsignedStartIndex = static_cast<size_t>(startIndex);

    std::string pageSizeString = pageSizeNode.text().as_string();
    int pageSize = atoi(pageSizeString.c_str());

    // rebuild items according to the start index and page
    pugi::xpath_node_set itemNodes = listNode.select_nodes("//item");
    if (unsignedStartIndex >= itemNodes.size()) {
        VR_LOGI("List is processed, return!");
        return;
    }

    listNode.remove_child("items");
    pugi::xml_node itemsNode = listNode.append_child("items");

    for (int i = 0; i < pageSize; ++i) {
        size_t index = unsignedStartIndex + i;
        if (index < itemNodes.size()) {
            pugi::xml_node nodeIt = itemNodes[index].node();
            itemsNode.append_copy(nodeIt);
        }
        else {
            VR_LOGI("pagesize=%d", pageSize);
            VR_LOGI("startIndex=%d", unsignedStartIndex);
            VR_LOGI("when add items to dispaly list, item has reached end!");
            break;
        }
    }
}

void VR_EUDialogEngine::handleAsrResult(const std::string &asrResult)
{
    std::string name = "done." VR_ASR_EVT_RESULT;

    if (!m_isAsrReturnPlayBeep) {
        std::string beepPath = m_pConfigureIF->getDataPath() + RETURN_BEEPPATH;
        reqDmPlayBeep(beepPath);
    }
    m_isAsrReturnPlayBeep = true;

    std::string xml = asrResult;
    VR_LOGP("ASR event:recognition result, content:[%s]", xml.c_str());
    VR_LOG_TO_FILE("ASR result", xml);
    if (xml.empty()) {
        VR_LOGD("asr result.Get_XML() is empty!");
        responseAsrError();
        return;
    }
    pugi::xml_document doc;
    doc.load_string(xml.c_str());
    m_pIntentionParser->preProcAsrResult(doc);
    std::string intentionStr = parseAsrToIntention(doc);
    if (intentionStr.empty()) {
        VR_LOGD("after parser, asr intentionStr is empty");
        responseAsrError();
        return;
    }

    pugi::xml_document intentionDoc;
    intentionDoc.load_string(intentionStr.c_str());
    if (1 == intentionDoc.document_element().attribute("no_asr_result").as_int()) {
        name = "failed." VR_ASR_EVT_RESULT;
    }

    _dataProcessor.updateListByAsr(intentionStr);
    _currentIntention = std::pair<std::string, std::string>(name, intentionStr);
    _interManager->receiveEvent(name, intentionStr);
}

void VR_EUDialogEngine::sendEventToDETask(uscxml::Event evt)
{
    VR_LOGD_FUNC();
    VoiceList<spVR_DE_Task>::iterator it = _listDETaskSeq.begin();
    while (it != _listDETaskSeq.end()) {
        spVR_DE_Task task = *it;
        bool isCountiue = task->onEvent(this, evt);
        bool isFinished = task->isFinished();

        if (!isFinished) {
            VR_LOGD("task not finish, push back to de task list");
            ++it;
        }
        else {
            VR_LOGD("task finish");
            it = _listDETaskSeq.erase(it);
        }

        if (!isCountiue) {
            VR_LOGD("don't need send event to other task!");
            break;
        }
        else {
            VR_LOGD("send event to other task!");
        }
    }
}

bool VR_EUDialogEngine::isEventNeedHandle(uscxml::Event* pEvent)
{
    // Event Check
    // If Event From Interpreter, And CloseSession Has Finished. Event Don't Need Handle, Return FALSE.
    if (!pEvent->getOrigin().compare(0, strlen(VR_INTERPRETER_EVENT_TYPE), VR_INTERPRETER_EVENT_TYPE)) {
        if (_bHasCloseSession) {
            VR_LOGD("DE has Closession, %s Event Don't Need Send", pEvent->getOrigin().c_str());
            return false;
        }
    }
    return true;
}

bool VR_EUDialogEngine::stopCurrentDialog()
{
    VR_LOGD_FUNC();

    resourceStateChange("engine", ResourceState::GRAMMAR_NOT_READY);
    handleQuitVRApp(_currentEvent);

    if (!_engine) {
        VR_LOGI("asr engine is null");
        return false;
    }
    if (!_asrIsRunning) {
        return true;
    }
    VR_LOGD("Recognize_Cancel");
    m_isCanceledByDE = true;
    _engine->Recognize_Cancel();
    return false;
}

void VR_EUDialogEngine::initializeAsrPersonalData()
{
    VR_LOGD_FUNC();
    if (!_engine) {
        VR_LOGI("asr engine is null");
        return;
    }
    resourceStateChange("init", ResourceState::GRAMMAR_NOT_READY);

    VR_LOGD("before delete engine");
    delete _engine;
    _engine = nullptr;
    VR_LOGD("delete engine");

    C_Engine::Clear_User_Data();

    uscxml::Event unused;
    handleInitAsr(unused);
    resourceStateChange("init", ResourceState::READY);

    std::string returnMsg = "<event-result name=\"initialpersondata\">"
                            "</event-result>";
    requestAction(returnMsg);
}

bool VR_EUDialogEngine::checkPlaylistAvailable(std::string &playlistID)
{
    std::string response;
    std::string requestMsg;
    requestMsg = "<id>" + playlistID + "</id>";
    _dataAccessorManager->getInfo("_getPlaylistSongsNumber", requestMsg, response);
    pugi::xml_document doc;
    if (!doc.load_string(response.c_str())) {
        VR_LOGE("load get playlist song number result failed");
        return false;
    }
    int songNumber = doc.select_node("//number").node().text().as_int();

    return (songNumber > 0);
}

void VR_EUDialogEngine::requestGrammarAfterCheck()
{
    VR_LOGD_FUNC();
    if (m_DMStarted && nullptr != _engine) {
        requestGrammar();
    }
}

void VR_EUDialogEngine::processStartedNotify()
{
    VR_LOGD_FUNC();
    m_DMStarted = true;
    notifyStartFinishAfterCheck();
    requestGrammarAfterCheck();
}

void VR_EUDialogEngine::requestGrammar()
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

void VR_EUDialogEngine::onRequestGrammarFinish()
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

void VR_EUDialogEngine::changeASREngineLanguage(const std::string languageID)
{
    _dataAccessorManager->setGenerateGrammarEnable(false);
    if (_engine) {

        _dataAccessorManager->resetAsrVMPStatus(languageID);
        C_Request_Language_Change o_Request_Language_Change;
        o_Request_Language_Change.m_string_Id_Language = languageID;
        o_Request_Language_Change.m_function_On_Event_Init_Notify = boost::bind(&VR_EUDialogEngine::onAsrInit, this, _1);
        _engine->Language_Change(o_Request_Language_Change);
    }
    _dataAccessorManager->resetGrammarGeneration();
    requestGrammarAfterCheck();
}

std::string VR_EUDialogEngine::getMediaGracenotePhoneme(std::string& ttsPromptId, std::string& ttsContent)
{
    VR_LOGD_FUNC();
    std::string musicCategory;

    std::string result = ttsContent;

    if ("media_genre_name" == ttsPromptId) {
        musicCategory = VR_MEDIA_ASR_GRACENOTE_PHONEME_GENRE;
    }
    else if ("media_artist_name" == ttsPromptId) {
        musicCategory = VR_MEDIA_ASR_GRACENOTE_PHONEME_ARTIST;
    }
    else if ("media_album_name" == ttsPromptId) {
        musicCategory = VR_MEDIA_ASR_GRACENOTE_PHONEME_ALBUM;
    }
    else {
        // this media name don't need to get phoneme
        return result;
    }

    // get ASR CTerm object
    spC_Term cTerm = _dataAccessorManager->getGracenoteAsrCterm(musicCategory, ttsContent);

    if (cTerm) {
        _engine->Get_Transcriptions(*cTerm);

        if (!cTerm->m_vector_buffer_Transcriptions.empty()) {

            C_Buffer pronBuffer = cTerm->m_vector_buffer_Transcriptions.front();


            // get the first phoneme in asr vector
            std::string phoneme = std::string(reinterpret_cast<char *>(pronBuffer.m_sai_Buffer.get()), pronBuffer.m_i_Size);

             // strip # in phoneme
            if (stripHashInPhoneme(phoneme)) {
                result = VR_TTS_PHONEME_TAG_HEAD + phoneme + VR_TTS_PHONEME_TAG_TAIL;
            }
        }
    }

    return result;
}

bool VR_EUDialogEngine::stripHashInPhoneme(std::string& phoneme)
{
    // strip the # in phoneme
    if (phoneme.empty()) {
        VR_LOGE("Asr Phoneme Error [%s]", phoneme.c_str());
        return false;
    }

    // strip /0 first
    size_t pos =  phoneme.find_last_not_of("\0");
    if (pos == std::string::npos) {
        return false;
    }
    phoneme = phoneme.substr(0, pos);

    // strip #
    phoneme.erase(0, phoneme.find_first_not_of("#"));
    size_t foundPos = phoneme.find_last_not_of("#");
    if (foundPos == std::string::npos) {
        return false;
    }
    phoneme.erase(phoneme.find_last_not_of("#") + 1);
    return true;
}

// judge VRState type to send button press event to interpreter
bool VR_EUDialogEngine::checkButtonPressEventAvailiable(const std::string& buttonKeycode, const std::string& VRState)
{

    if (VR_STATE_RECOGNIZING == VRState) {

        if (0 == strcmp(buttonKeycode.c_str(), VR_BUTTON_KEY_CODE_CANCEL)
            || 0 == strcmp(buttonKeycode.c_str(), VR_BUTTON_KEY_CODE_PTT_LONG)
            || 0 == strcmp(buttonKeycode.c_str(), VR_BUTTON_KEY_CODE_HARD_NORMAL)
            || 0 == strcmp(buttonKeycode.c_str(), VR_BUTTON_KEY_CODE_METER_BACK_NORMAL)) {
            return true;
        }
        else {
            return false;
        }
    }
    else if (VR_STATE_PROCESSING == VRState) {

        if (0 == strcmp(buttonKeycode.c_str(), VR_BUTTON_KEY_CODE_CANCEL)
            || 0 == strcmp(buttonKeycode.c_str(), VR_BUTTON_KEY_CODE_PTT_LONG)
            || 0 == strcmp(buttonKeycode.c_str(), VR_BUTTON_KEY_CODE_METER_BACK_NORMAL)) {
            return true;
        }
        else {
            return false;
        }
    }
    else if (VR_STATE_READING == VRState) {

        if (0 == strcmp(buttonKeycode.c_str(), VR_BUTTON_KEY_CODE_CANCEL)
            || 0 == strcmp(buttonKeycode.c_str(), VR_BUTTON_KEY_CODE_PTT_SHORT)
            || 0 == strcmp(buttonKeycode.c_str(), VR_BUTTON_KEY_CODE_PTT_LONG)
            || 0 == strcmp(buttonKeycode.c_str(), VR_BUTTON_KEY_CODE_METER_BACK_NORMAL)) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return true;
    }
}

void VR_EUDialogEngine::getDisplayItemList(pugi::xml_node &result, const std::string &category, VoiceList<std::string>::type &itemlist)
{
    VR_LOGD_FUNC();
    std::string categoryName = category + "_name";
    pugi::xpath_node_set nameNodes = result.select_nodes((std::string("//") + categoryName).c_str());
    pugi::xpath_node_set::iterator nameNodeIt = nameNodes.begin();
    while (nameNodeIt != nameNodes.end()) {
        itemlist.push_back(nameNodeIt->node().text().as_string());
        ++nameNodeIt;
    }
}

/* EOF */
