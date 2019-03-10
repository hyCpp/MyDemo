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

#include "VR_DataAccessorManagerEU.h"
#include "VR_DataAccessorMediaEU.h"
#include "VR_DataAccessorContactEU.h"
#include "Vr_Asr_Engine.h"
#include "VR_Log.h"
#include "VR_MsgDefine.h"

#include <sqlite3.h>
#include <boost/make_shared.hpp>

const int VR_FM_STATION_BASE_GRAMMAR_ID = 1000;

#define VR_GRAMMAR_CATEGORY_DAB_STATION         "dabstation"
#define VR_GRAMMAR_CATEGORY_FM_STATION          "fmstation"
#define VR_GRAMMAR_CATEGORY_CONTACT             "contact"
#define VR_GRAMMAR_CATEGORY_AUDIOSOURCE         "audiosource"

#define VR_DICTIONARY_STATE_SYNCING     "DICTIONARY_SYNCING"
#define VR_DICTIONARY_STATE_OK          "DICTIONARY_OK"

#define VR_GLOBALSTATE_DICTIONARY_STATE_SUFFIX      "_DICTIONARY_STATE"

#define VR_GRAMMAR_ID               "grammarid"
#define VR_GRAMMAR_MSG_INIT         "grammar_init"
#define VR_GRAMMAR_MSG_DIFF         "grammar_diff"
#define VR_GRAMMAR_MSG_NEW          "grammar_new"
#define VR_GRAMMAR_MSG_PREUPDATE    "grammar_preupdate"
#define VR_GRAMMAR_MSG_DISPREUPDATE "grammar_dispreupdate"
#define VR_GRAMMAR_MSG_ACTIVE       "grammar_active"
#define VR_GRAMMAR_MSG_DISACTIVE    "grammar_disactive"

#define VR_GRAMMAR_AGENT            "agent"
#define VR_GRAMMAR_PATH             "path"

#define VR_AGENT_MEDIA              "media"
#define VR_AGENT_PHONE              "phone"
#define VR_ITEM_ID                  "id"
#define VR_ITEM_NAME                "name"
#define VR_CATEGORY_NAME            "name"
#define VR_GRAMMAR_MSG_NEW          "grammar_new"

#define VR_GRAMMAR_TYPE_EU_DEFLAUT  ""

using namespace N_Vr;
using namespace N_Asr;

bool VR_DataAccessorManagerEU::m_gracenoteSupported = false;

VR_DataAccessorManagerEU::VR_DataAccessorManagerEU(VR_AsrRequestor* asrRequestor, VR_DECommonIF* common, VR_ConfigureIF* config)
    : VR_DataAccessorManager(asrRequestor, common, config)
{
    m_accessorMediaEU = boost::make_shared<VR_DataAccessorMediaEU>();
    m_accessorMedia = m_accessorMediaEU;
    std::string language = m_deCommonIF->getVRLanguage();
    processChangeVrLanguage(language);

    m_accessorContactEU = boost::make_shared<VR_DataAccessorContactEU>();
    m_accessorContact = m_accessorContactEU;
    m_categoryContextIDMap["dabstation"] = "ctx_tuner_station_name_list";
    m_categoryContextIDMap["fmstation"] = "ctx_tuner_station_name_list";

    m_musicGrammarCallback = genMusicCTermListCallbackEU;
}



VR_DataAccessorManagerEU::~VR_DataAccessorManagerEU()
{

}

std::string VR_DataAccessorManagerEU::getStationBand(int stationGrammarID)
{
    if (stationGrammarID > VR_FM_STATION_BASE_GRAMMAR_ID) {
        return "FM";
    }
    else {
        return "DAB";
    }
}

int VR_DataAccessorManagerEU::getStationID(int stationGrammarID)
{
    int stationID = stationGrammarID;
    if (stationGrammarID > VR_FM_STATION_BASE_GRAMMAR_ID) {
        stationID -= VR_FM_STATION_BASE_GRAMMAR_ID;
    }
    return stationID;
}

int VR_DataAccessorManagerEU::getStationGrammarID(int stationID, VR_StationType type)
{
    int stationGrammarID = stationID;
    if (VR_StationType::FM == type) {
        stationGrammarID += VR_FM_STATION_BASE_GRAMMAR_ID;
    }
    return stationGrammarID;
}

void VR_DataAccessorManagerEU::handleMusicOneshotResult(pugi::xml_node &result)
{
    if (!m_accessorMedia) {
        VR_LOGE("point of accessor media is null.");
        return;
    }
    m_accessorMediaEU->handleMusicOneshotResult(result);
}

void VR_DataAccessorManagerEU::processRadioGrammarNew(VoiceList<spC_Term>::type &addList)
{
    if (!m_accessorMedia) {
        VR_LOGE("point of accessor media is null.");
        return;
    }

    VoiceMap<int, std::string>::type & FMMap = m_accessorMediaEU->getRadioStationMap(VR_StationType::FM);
    VoiceMap<int, std::string>::type & DABMap = m_accessorMediaEU->getRadioStationMap(VR_StationType::DAB);

    VoiceMap<int, std::string>::iterator it = FMMap.begin();
    while (it != FMMap.end()) {
        addList.push_back(VR_DataSynchronizer::getCTerm(getStationGrammarID(it->first, VR_StationType::FM), it->second));
        ++it;
    }

    it = DABMap.begin();
    while (it != DABMap.end()) {
        addList.push_back(VR_DataSynchronizer::getCTerm(getStationGrammarID(it->first, VR_StationType::DAB), it->second));
        ++it;
    }
}

void VR_DataAccessorManagerEU::handleGrammarNew(pugi::xml_node &category)
{
    VR_LOGD_FUNC();
    VR_LOGP("case:212-16-760 212-17-760 212-18-760 DataAccessor get grammar_new message");
    VoiceList<spC_Term>::type addList;
    std::string categoryName;
    std::string contextID;

    while (!category.empty()) {
        addList.clear();
        categoryName.assign(category.attribute(VR_CATEGORY_NAME).as_string());

        if (!isCategoryAvailable(categoryName)) {
            category = category.next_sibling();
            continue;
        }
        contextID = m_categoryContextIDMap[categoryName];
        VR_LOGD("ContextID: [%s]", contextID.c_str());

        if (VR_GRAMMAR_CATEGORY_DAB_STATION == categoryName
            || VR_GRAMMAR_CATEGORY_FM_STATION == categoryName) {
            processRadioGrammarNew(addList);
        }
        else {
            VR_LOGD("unhandle category, skip");
            category = category.next_sibling();
            continue;
        }

        updateOtherGrammarState(categoryName);
        std::string clcType = VR_GRAMMAR_TYPE_EU_DEFLAUT;  // for eu radio

        N_Vr::N_Asr::C_Request_Context_List_Update updateMsg;
        updateMsg.m_string_Id_Context.assign(contextID);
        updateMsg.m_list_spo_Term_For_Add.swap(addList);
        updateMsg.m_string_Id_Category = clcType;
        updateMsg.m_b_Clear = true;
        updateMsg.m_function_On_Event_Phase = boost::bind(&VR_DataAccessorManager::updateGrammarCallback,
                                                            this,
                                                            _1,
                                                            categoryName);
        requestUpdateGrammar(updateMsg);
        category = category.next_sibling();
    }
}

void VR_DataAccessorManagerEU::updateOtherGrammarState(const std::string &categoryName)
{
    std::string key = categoryName;
    if (VR_GRAMMAR_CATEGORY_CONTACT == key) {
        key = "PHONEBOOK";
    }
    std::transform(key.begin(), key.end(), key.begin(), toupper);
    key += VR_GLOBALSTATE_DICTIONARY_STATE_SUFFIX;
    setState(key, VR_DICTIONARY_STATE_SYNCING);
    VR_LOGI("GlobalState [%s] updated [" VR_DICTIONARY_STATE_SYNCING "]", key.c_str());

    notifyStateUpdated();
}

void VR_DataAccessorManagerEU::setMusicGrammarActive(
    const std::string &grammarID,
    bool enable,
    int songCount,
    int otherCount)
{
    VoiceList<std::string>::type mediaMainList;
    VoiceList<std::string>::type mediaOneshotList;
    VoiceList<std::string>::type categoryList;
    if (enable) {
        getAvailableCategory(categoryList, songCount, otherCount);
    }
    else {
        categoryList = m_musicCategoryList;
    }
    std::string categoryName;

    for (VoiceList<std::string>::iterator it = categoryList.begin();
        it != categoryList.end();
        ++it) {
        if ("song" == *it) {
            categoryName = "title";
        }
        else {
            categoryName = *it;
        }

        mediaMainList.push_back(std::string("grm_cmd_topmenu_main#rul_slt_media_") + categoryName + "_list_" + grammarID);
        mediaOneshotList.push_back(std::string("grm_cmd_media_oneshot_name#rul_slt_media_") + categoryName + "_list_" + grammarID);
        std::string ruleID = std::string("grm_cmd_media_")
            + categoryName + "_name#rul_slt_media_"
            + categoryName + "_list_" + grammarID;
        if (enable) {
            grammarActive(std::string("ctx_media_") + categoryName + "_name_speak", ruleID);
        }
        else {
            grammarDisactive(std::string("ctx_media_") + categoryName + "_name_speak", ruleID);
        }
    }
    if (enable) {
        grammarActive("ctx_media_oneshot_name_speak", mediaOneshotList);
        if ((songCount + otherCount) > VR_MAX_MUSIC_GRAMMAR_COUNT
            && songCount <= VR_MAX_MUSIC_GRAMMAR_COUNT) {
            mediaMainList.remove(std::string("grm_cmd_topmenu_main#rul_slt_media_title_list_") + grammarID);
        }
        grammarActive("ctx_topmenu_idle", mediaMainList);
    }
    else {
        grammarDisactive("ctx_media_oneshot_name_speak", mediaOneshotList);
        grammarDisactive("ctx_topmenu_idle", mediaMainList);
    }
}

void VR_DataAccessorManagerEU::setPhoneContactGrammarActive(bool enable)
{
    if (enable) {
        grammarActive("ctx_phone_phone_anyspeech", "grm_cmd_phone_entry#rul_slt_phone_contact_name_list_1");
        grammarActive("ctx_phone_phone_entry_speak", "grm_cmd_phone_entry#rul_slt_phone_contact_name_list_1");
        grammarActive("ctx_topmenu_idle", "grm_cmd_topmenu_main#rul_slt_phone_contact_name_list_1");
    }
    else {
        grammarDisactive("ctx_phone_phone_anyspeech", "grm_cmd_phone_entry#rul_slt_phone_contact_name_list_1");
        grammarDisactive("ctx_phone_phone_entry_speak", "grm_cmd_phone_entry#rul_slt_phone_contact_name_list_1");
        grammarDisactive("ctx_topmenu_idle", "grm_cmd_topmenu_main#rul_slt_phone_contact_name_list_1");
    }
}

bool VR_DataAccessorManagerEU::checkPhoneTypeAvailable(int phoneTypeID)
{
    if (!m_accessorContact) {
        VR_LOGE("point of accessor contact is null.");
        return false;
    }

    bool result = m_accessorContactEU->phoneTypeAvailable(phoneTypeID);
    return result;
}

void VR_DataAccessorManagerEU::processChangeVrLanguage(std::string language)
{
    m_accessorMediaEU->setVoiceLanguage(language);
}

void VR_DataAccessorManagerEU::handleAsrMusicStepByStepResult(pugi_vr::xml_node &result)
{
    if (!m_accessorMediaEU->filterAsrMusicStepByStepResult(result)) {
        return;
    }
}

void VR_DataAccessorManagerEU::updateGrammar(const std::string &grammarMsg)
{
    VR_LOGD_FUNC();
    preprocessRadioMsg(grammarMsg);

    pugi::sp_xml_document spGrammarMsgDoc(VR_new pugi::xml_document());
    if (!spGrammarMsgDoc->load_string(grammarMsg.c_str())) {
        VR_LOGE("load xml from grammarMsg failed");
        return;
    }
    std::string nodeName(spGrammarMsgDoc->first_child().name());
    std::string agent(spGrammarMsgDoc->first_child().attribute(VR_GRAMMAR_AGENT).as_string());

    // filter wait load vmp msg
    if (VR_AGENT_MEDIA == agent
        && VR_GRAMMAR_MSG_NEW != nodeName
        && VR_GRAMMAR_MSG_DISPREUPDATE != nodeName
        && VR_GRAMMAR_MSG_PREUPDATE != nodeName) {
        bool isNeed = isNeedWaitLoadVmp(spGrammarMsgDoc);
        if (isNeed) {
            std::string loadingString = m_asrDataManager.getCurrentLoadingMagicString();
            // push grammar msg to gracenote list
            pushBackMsgToGracenoteMsgList(spGrammarMsgDoc, loadingString);
            return;
        }
    }

    VR_DataAccessorManager::updateGrammar(spGrammarMsgDoc);
}

void VR_DataAccessorManagerEU::resetGrammarGeneration()
{
    VR_LOGD_FUNC();
    // clear gracenoteMsgList
    m_gracenoteMsgList.clear();

    VR_DataAccessorManager::resetGrammarGeneration();
}

void VR_DataAccessorManagerEU::saveRadioData(pugi::xml_node &itemNode, VoiceMap<int, std::string>::type & radioMap)
{
    radioMap.clear();
    while (!itemNode.empty()) {
        int id = itemNode.attribute(VR_ITEM_ID).as_int();
        std::string name = itemNode.attribute(VR_ITEM_NAME).as_string();
        if (name.empty()) {
            itemNode = itemNode.next_sibling();
            continue;
        }
        radioMap.insert(std::pair<int, std::string>(id, name));
        itemNode = itemNode.next_sibling();
    }
}

void VR_DataAccessorManagerEU::preprocessRadioMsg(const std::string &grammarMsg)
{
    if (!m_accessorMediaEU) {
        return;
    }
    pugi::xml_document doc;
    doc.load_string(grammarMsg.c_str());
    pugi::xml_node msgNode = doc.select_node((std::string(".//") + VR_GRAMMAR_MSG_NEW).c_str()).node();
    if (!msgNode) {
        return;
    }
    pugi::xpath_node_set categoryNodeSet = msgNode.select_nodes(".//category");
    for (auto it : categoryNodeSet) {
        pugi::xml_node category = it.node();
        std::string categoryName = category.attribute(VR_CATEGORY_NAME).as_string();

        if (VR_GRAMMAR_CATEGORY_DAB_STATION == categoryName) {
            pugi::xml_node itemNode = category.first_child();
            VoiceMap<int, std::string>::type & DABMap = m_accessorMediaEU->getRadioStationMap(VR_StationType::DAB);
            saveRadioData(itemNode, DABMap);
        }
        else if (VR_GRAMMAR_CATEGORY_FM_STATION == categoryName) {
            pugi::xml_node itemNode = category.first_child();
            VoiceMap<int, std::string>::type & FMMap = m_accessorMediaEU->getRadioStationMap(VR_StationType::FM);
            saveRadioData(itemNode, FMMap);
        }
        else {
            // ignore other category
        }
    }
}


bool VR_DataAccessorManagerEU::isNeedWaitLoadVmp(pugi_vr::sp_xml_document spGrammarMsgDoc)
{
    // is vmp data exists
    if (!m_asrDataManager.isVmpDataExists()) {
        return false;
    }

    // process grammar msg media init or active
    std::string nodeName(spGrammarMsgDoc->first_child().name());

    VR_DialogEngineAsrDataManager::VMPLoadState loadStatus = m_asrDataManager.getVMPLoadState();
    if (VR_GRAMMAR_MSG_INIT == nodeName
        || (VR_GRAMMAR_MSG_ACTIVE == nodeName
        && VR_DialogEngineAsrDataManager::VMPLoadState::NONE == loadStatus)) {

        std::string magicString;
        // get magic string from db
        std::string path = spGrammarMsgDoc->first_child().attribute(VR_GRAMMAR_PATH).as_string();
        getGracenoteMagicString(path, magicString);

        // check is need to load vmp
        VR_DialogEngineAsrDataManager::MagicStringValueState value = m_asrDataManager.checkMagicStringAvalible(magicString);
        if (VR_DialogEngineAsrDataManager::MagicStringValueState::VALID_VALUE == value) {
            // notify asr load vmp
            m_pDataSynchronizer->loadAsrGracenoteClc(
                 magicString,
                 boost::bind(&VR_DataAccessorManagerEU::Asr_Load_GracenoteClcCallback,
                     this,
                     _1,
                     magicString));

            VR_LOGD("Set VMP State Loading [%s]", magicString.c_str());
            // setLoadState loading and magic string
            m_asrDataManager.setVMPLoadState(VR_DialogEngineAsrDataManager::VMPLoadState::LOADING, magicString);
            return true;
        }
        else if (VR_DialogEngineAsrDataManager::MagicStringValueState::NULL_VALUE == value) {
            VR_LOGD("magic String null Set VMP State Failed", magicString.c_str());
            m_asrDataManager.setVMPLoadState(VR_DialogEngineAsrDataManager::VMPLoadState::LOADFAIL, magicString);
            m_gracenoteSupported = false;

            // process gracenote list first
            while (!m_gracenoteMsgList.empty()) {
                std::pair<pugi_vr::sp_xml_document, std::string> msg = m_gracenoteMsgList.front();
                pugi_vr::sp_xml_document grammarMsg = msg.first;
                VR_DataAccessorManager::updateGrammar(grammarMsg);
                m_gracenoteMsgList.pop_front();
            }
            return false;
       }
    }
    else {
        if (loadStatus == VR_DialogEngineAsrDataManager::VMPLoadState::LOADING) {
            return true;
        }
    }

    return false;

}

void VR_DataAccessorManagerEU::getMusicCTermList(
    VoiceList<spC_Term>::type &cTermList,
    const unsigned int &id,
    const std::string &name,
    const std::string &pronIDStr,
    const std::string &pronNameStr)
{
    generateMusicCTermListEU(cTermList, id, name, pronIDStr, pronNameStr);
}

int VR_DataAccessorManagerEU::genMusicCTermListCallbackEU(
    void *addList,
    int columnNum,
    char **columnValue,
    char **columnName)
{
    // check column number count available, otherwise continue to operate next result
    if (columnNum < SQLITE_CALLBACK_COLUMN_NUMBER_GEN_MUSIC_CTERM_EU) {
        return 0;
    }

    if (nullptr == columnValue[0] || nullptr == columnValue[1]) {
        return 0;
    }

    int id = atoi(columnValue[0]);
    std::string itemName(columnValue[1]);
    std::string pronIDStr;
    std::string pronNameStr;
    if (columnValue[2]) {
        pronIDStr = columnValue[2];
    }

    if (columnValue[3]) {
        pronNameStr = columnValue[3];
    }

    generateMusicCTermListEU(*static_cast<VoiceList<spC_Term>::type*>(addList), id, itemName, pronIDStr, pronNameStr);
    return 0;
}

void VR_DataAccessorManagerEU::generateMusicCTermListEU(
    VoiceList<spC_Term>::type &cTermList,
    const unsigned int &id,
    const std::string &name,
    const std::string &pronIDStr,
    const std::string &pronNameStr)
{
    int pronID = -1;
    if (m_gracenoteSupported && !pronIDStr.empty()) {
        std::stringstream ss;
        ss << pronIDStr;
        ss >> pronID;
        if (ss.fail()) {
            pronID = -1;
        }
    }

    VoiceList<std::string>::type formalList;
    if (!getMusicItemList(name, formalList)) {
        return;
    }
    if (formalList.empty()) {
        return;
    }

    // get gracenote name and append to formal list
    if (m_gracenoteSupported && !pronNameStr.empty()) {
        std::string genreName = pronNameStr;
        VoiceList<std::string>::type genreList;
        if (getMusicItemList(genreName, genreList)) {
            for (auto iter = genreList.begin(); iter != genreList.end(); iter++) {
                formalList.push_back(*iter);
            }
        }
    }

    cTermList.push_back(
        VR_DataSynchronizer::getCTermWithPronIdNameList(
            id,
            formalList,
            pronID));
}

bool VR_DataAccessorManagerEU::getGracenoteSupportedEnable()
{
    return m_gracenoteSupported;
}


void VR_DataAccessorManagerEU::resetAsrVMPStatus(const std::string &language)
{
    VR_LOGD_FUNC();
    // set asr lanuage
    m_asrDataManager.resetAsrVMPStatus(language);
    m_gracenoteSupported = false;
}


void VR_DataAccessorManagerEU::getGracenoteMagicString(const std::string &path, std::string &magicString)
{

    sqlite3 * dbHandler;
    int dbResult = sqlite3_open_v2(path.c_str(), &dbHandler, SQLITE_OPEN_READONLY | SQLITE_OPEN_NOMUTEX, NULL);
    if (SQLITE_OK != dbResult) {
        {
            static bool errorLogMark = false;
            if (!errorLogMark) {
                errorLogMark = true;
                VR_ERROR(VOICE_VR_ERR_OPEN_DB, VR_DE_OPTIONDATA_NONE, "Open DB file %s failed, dbResultID: [%d]", path.c_str(), dbResult);
            }
        }
        sqlite3_close(dbHandler);
        return;
    }

    if (!dbHandler) {
        return;
    }

    std::string sql = "SELECT magicstring from magicstring;";

    char* errmsg = NULL;

    // exec sql
    int result = sqlite3_exec(dbHandler, sql.c_str(), queryMagicStringCallback, &magicString, &errmsg);
    if (SQLITE_OK != result) {
        VR_LOGE("Run SQL request [%s] failed, error code: [%d], error msg: [%s]", sql.c_str(), result, errmsg);
        return;
    }
}


spC_Term VR_DataAccessorManagerEU::getGracenoteAsrCterm(std::string& musicCategory, std::string& mediaName)
{
    VR_LOGD_FUNC();

    std::pair <std::string, std::string> graceInfo;

    // query gracenote info from db
    if (!m_accessorMediaEU->queryGracenoteInfoFromDB(musicCategory, mediaName, graceInfo)) {
        return NULL;
    }

    std::string name =  mediaName;
    std::string proId = graceInfo.first;
    std::string proName = graceInfo.second;
    std::string clcDomain = selectMusicClcDomainType(musicCategory);

    // build ASR CTerm object
    spC_Term c_term;
    // genre use the gracenote name
    if (VR_MEDIA_ASR_GRACENOTE_PHONEME_GENRE == musicCategory) {
        if (proName.empty()) {
            return NULL;
        }
        name = proName;
        // get media name alias
        VoiceList<std::string>::type itemList;
        if (!getMusicItemList(name, itemList)) {
            return NULL;
        }
        c_term = m_pDataSynchronizer->getGracenoteCtermByName(itemList, clcDomain);
    }
    // artist or album use the gracenote id
    else {
        if (proId.empty()) {
            return NULL;
         }

        int gracenoteId = VR_MEIDA_GRACENOTE_ID_INVALID;
        std::stringstream ss;
        ss << proId;
        ss >> gracenoteId;
        if (ss.fail()) {
            gracenoteId = VR_MEIDA_GRACENOTE_ID_INVALID;
        }
        // get media name alias
        VoiceList<std::string>::type itemList;
        if (!getMusicItemList(name, itemList)) {
            return NULL;
        }
        c_term = m_pDataSynchronizer->getGracenoteCtermById(itemList, gracenoteId, clcDomain);
    }

    return c_term;

}


std::string VR_DataAccessorManagerEU::selectMusicClcDomainType(const std::string &categoryName)
{
    if (categoryName == "song") {
        return VR_MEDIA_MUSIC_DOMAIN_TITLE;  // for song
    }
    else if (categoryName == "playlist") {
        return VR_MEDIA_MUSIC_DOMAIN_NORMAL; // for playlist
    }
    else if (categoryName == "artist") {
        return VR_MEDIA_MUSIC_DOMAIN_ARTIST; // for artist
    }
    else if (categoryName == "album") {
        return VR_MEDIA_MUSIC_DOMAIN_ALBUM; // for album
    }
    else if (categoryName == "genre") {
        return VR_MEDIA_MUSIC_DOMAIN_NORMAL; // for genre
    }
    else {
        return VR_MEDIA_MUSIC_DOMAIN_NORMAL;
    }

}

void VR_DataAccessorManagerEU::Asr_Load_GracenoteClcCallback(
    const N_Vr::N_Asr::C_Event_Notify &notify,
    std::string magicString)
{
    VR_LOGD_FUNC();

    std::string result;
    N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type msgType = notify.Get_Event_Notify_Type();
    switch (msgType) {
    case N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_Transcription_Id_Not_Support:
        {
            result = VR_DE_ASR_LOAD_RESULT_FALSE;
            break;
        }
    case N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_Transcription_Id_Support:
        {
            result = VR_DE_ASR_LOAD_RESULT_TRUE;
            break;
        }
    default:
        {
            result = VR_DE_ASR_LOAD_RESULT_FALSE;
            break;
        }
    }

    // build result node
    pugi::xml_document doc;
    if (!doc.load_string(VR_DE_ASR_LOAD_RESULT_XML)) {
        VR_LOGE("pugi load string Error!");
        m_pDataSynchronizer->notifyAsrLoadGracenoteFinish(VR_DE_ASR_LOAD_RESULT_XML);
        return;
    }
    pugi::xml_node resultNode = doc.first_child();
    resultNode.attribute(VR_DE_ASR_LOAD_RESULT_MAGICSTRING).set_value(magicString.c_str());
    resultNode.attribute(VR_DE_ASR_LOAD_RESULT_NAME).set_value(result.c_str());

    // to string
    std::ostringstream oss;
    doc.print(oss);
    std::string loadResult = oss.str();
    VR_LOGD("load vmp callback result [%s]", loadResult.c_str());
    m_pDataSynchronizer->notifyAsrLoadGracenoteFinish(loadResult);

}


int VR_DataAccessorManagerEU::queryMagicStringCallback(void *magicString, int columnNum, char **columnValue, char **columnName)
{
    if (SQLITE_CALLBACK_COLUMN_MAGICSTRING != columnNum) {
        return 0;
    }
    std::string* tmp = (std::string *)magicString;
    if (columnValue[0]) {
        *tmp = columnValue[0];
    }
    return 0;
}

void VR_DataAccessorManagerEU::notifyLoadGracenoteFinish(const std::string &magicString, bool loadStatus)
{
    VR_LOGD_FUNC();

    // release the asr quest queue
    m_pDataSynchronizer->updateGrammarCategoryFinish("vmpClc");

    // update load vmp clc flag
    m_asrDataManager.onCallBackSetVMPState(magicString, loadStatus);

    // set gracenote flag
    m_gracenoteSupported = m_asrDataManager.getAsrVMPStatus();

    // begin process queue msg
    processGracenoteMsgQueue(magicString);

}

void VR_DataAccessorManagerEU::processGracenoteMsgQueue(const std::string callBackMagicString)
{
    VR_LOGD_FUNC();
    while (!m_gracenoteMsgList.empty()) {
        std::pair<pugi_vr::sp_xml_document, std::string> msg = m_gracenoteMsgList.front();

        pugi_vr::sp_xml_document spGrammarMsgDoc = msg.first;
        std::string loadMagicString = msg.second;
        // process grammar msg media init or active
        if (callBackMagicString != loadMagicString) {
            break;
        }
        VR_DataAccessorManager::updateGrammar(spGrammarMsgDoc);
        m_gracenoteMsgList.pop_front();
   }
}

void VR_DataAccessorManagerEU::pushBackMsgToGracenoteMsgList(pugi_vr::sp_xml_document spGrammarMsgDoc, const std::string &magicString)
{
    VR_LOGD_FUNC();
    pugi::xml_node msgNode = spGrammarMsgDoc->first_child();
    // remove repeat grammar_init in the queue
    if (std::string(VR_GRAMMAR_MSG_INIT) == msgNode.name()) {
        std::string currentAgent = msgNode.attribute(VR_GRAMMAR_AGENT).as_string();
        std::string currentGrammarID = msgNode.attribute(VR_GRAMMAR_ID).as_string();

        VoiceList<std::pair<pugi_vr::sp_xml_document, std::string> >::iterator it = m_gracenoteMsgList.begin();
        while (m_gracenoteMsgList.end() != it) {
            pugi_vr::sp_xml_document msg = it->first;
            std::string loadMagicString = it->second;

            pugi::xml_node tmpNode = msg->first_child();
            std::string agent = tmpNode.attribute(VR_GRAMMAR_AGENT).as_string();
            std::string grammarID = tmpNode.first_child().attribute(VR_GRAMMAR_ID).as_string();

            if (currentAgent == agent && currentGrammarID == grammarID && loadMagicString == magicString) {
                VR_LOGD("remove the same grammar msg in list");
                it = m_gracenoteMsgList.erase(it);
                continue;
            }
            ++it;
        }
    }
    m_gracenoteMsgList.push_back(std::pair<pugi_vr::sp_xml_document, std::string> (spGrammarMsgDoc, magicString));
}

/* EOF */
