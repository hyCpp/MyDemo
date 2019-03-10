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

#include "VR_DataAccessorManager.h"

#include "VR_DataAccessorContact.h"
#include "VR_DataAccessorNavi.h"
#include "VR_DataAccessorNaviCN.h"
#include "VR_DataAccessorVoiceTag.h"
#include "VR_DataAccessorMedia.h"

#include "Vr_Asr_Engine.h"

#include "VR_ConfigureIF.h"
#include "VR_DECommon.h"
#include "VR_Log.h"
#include "VR_Def.h"

#include "MEM_list.h"
#include "MEM_map.h"

#include <algorithm>
#include <boost/shared_array.hpp>
#include <boost/bind.hpp>
#include <cstdio>
#include <sqlite3.h>
#include <sstream>
#include <sys/types.h>

#include <sys/statvfs.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

const int VR_MAX_MUSIC_GRAMMAR_COUNT = 15000;

#define VR_ITEM_ID                  "id"
#define VR_ITEM_NAME                "name"
#define VR_ITEM_SHORTCUT            "shortcut"
#define VR_ITEM_PRON                "pron"
#define VR_ITEM_PRON_ID             "pronid"
#define VR_PHONE_TYPE               "type"
#define VR_PHONE_NUMBER             "number"
#define VR_GRAMMAR_ID               "grammarid"
#define VR_GRAMMAR_AGENT            "agent"
#define VR_GRAMMAR_PATH             "path"
#define VR_GRAMMAR_SONG_COUNT       "songitemcount"
#define VR_GRAMMAR_GENERAL_COUNT    "generalitemcount"
#define VR_GRAMMAR_GENRE_COUNT      "genreitemcount"
#define VR_DELETE_NODE_NAME         "delete"
#define VR_ADD_NODE_NAME            "add"
#define VR_CATEGORY_NAME            "name"

#define VR_DEVICE_ADDRESS           "deviceaddress"

#define VR_AGENT_MEDIA  "media"
#define VR_AGENT_PHONE  "phone"

#define VR_GRAMMAR_TYPE_PHONE  "name"
#define VR_GRAMMAR_TYPE_ARTIST "artist"
#define VR_GRAMMAR_TYPE_ALBUM "album"
#define VR_GRAMMAR_TYPE_TITLE "title"
#define VR_GRAMMAR_TYPE_NORMAL "normal"
#define VR_GRAMMAR_TYPE_DEFLAUT  ""

#define VR_GRAMMAR_MSG_INIT         "grammar_init"
#define VR_GRAMMAR_MSG_DIFF         "grammar_diff"
#define VR_GRAMMAR_MSG_NEW          "grammar_new"
#define VR_GRAMMAR_MSG_DYNAMIC      "grammar_dynamic"
#define VR_GRAMMAR_MSG_PREUPDATE    "grammar_preupdate"
#define VR_GRAMMAR_MSG_DISPREUPDATE "grammar_dispreupdate"
#define VR_GRAMMAR_MSG_ACTIVE       "grammar_active"
#define VR_GRAMMAR_MSG_DISACTIVE    "grammar_disactive"

#define VR_GRAMMAR_CATEGORY_CONTACT             "contact"
#define VR_GRAMMAR_CATEGORY_PHONETYPE           "phonetype"
#define VR_GRAMMAR_CATEGORY_QUICKREPLYMESSAGE   "quickreplymessage"
#define VR_GRAMMAR_CATEGORY_MESSAGETYPE         "messagetype"
#define VR_GRAMMAR_CATEGORY_DATA                "data"
#define VR_GRAMMAR_CATEGORY_SEARCHAPP           "searchapp"
#define VR_GRAMMAR_CATEGORY_FMGENRE             "fmgenre"
#define VR_GRAMMAR_CATEGORY_SATCHANNELNAME      "satchannelname"
#define VR_GRAMMAR_CATEGORY_SATCHANNELNUMBER    "satchannelnumber"
#define VR_GRAMMAR_CATEGORY_SATGENRE            "satgenre"
#define VR_GRAMMAR_CATEGORY_HDSUBCHANNEL        "hdsubchannel"
#define VR_GRAMMAR_CATEGORY_APPS                "apps"
#define VR_GRAMMAR_CATEGORY_APPSOFFBOARD        "appsoffboard"
#define VR_GRAMMAR_CATEGORY_VOICETAG            "voicetag"

#define VR_DB_TABLENAME_PHONERECORDS            "phonerecords"
#define VR_DB_TABLENAME_CONTACT                 VR_GRAMMAR_CATEGORY_CONTACT
#define VR_DB_TABLENAME_PHONETYPE               VR_GRAMMAR_CATEGORY_PHONETYPE
#define VR_DB_TABLENAME_QUICKREPLYMESSAGE       VR_GRAMMAR_CATEGORY_QUICKREPLYMESSAGE
#define VR_DB_TABLENAME_MESSAGETYPE             VR_GRAMMAR_CATEGORY_MESSAGETYPE
#define VR_DB_TABLENAME_DATA                    VR_GRAMMAR_CATEGORY_DATA
#define VR_DB_TABLENAME_SEARCHAPP               VR_GRAMMAR_CATEGORY_SEARCHAPP
#define VR_DB_TABLENAME_FMGENRE                 VR_GRAMMAR_CATEGORY_FMGENRE
#define VR_DB_TABLENAME_SATCHANNELNAME          VR_GRAMMAR_CATEGORY_SATCHANNELNAME
#define VR_DB_TABLENAME_SATCHANNELNUMBER        VR_GRAMMAR_CATEGORY_SATCHANNELNUMBER
#define VR_DB_TABLENAME_SATGENRE                VR_GRAMMAR_CATEGORY_SATGENRE
#define VR_DB_TABLENAME_HDSUBCHANNEL            VR_GRAMMAR_CATEGORY_HDSUBCHANNEL
#define VR_DB_TABLENAME_APPS                    VR_GRAMMAR_CATEGORY_APPS
#define VR_DB_TABLENAME_APPSOFFBOARD            VR_GRAMMAR_CATEGORY_APPSOFFBOARD

#define VR_DICTIONARY_STATE_SYNCING     "DICTIONARY_SYNCING"
#define VR_DICTIONARY_STATE_OK          "DICTIONARY_OK"
#define VR_DICTIONARY_STATE_NONE        "DICTIONARY_NONE"

#define VR_GLOBALSTATE_NUMBER_OF_TOTAL              "NUMBER_OF_TOTAL"
#define VR_GLOBALSTATE_NUMBER_OF_MUSIC              "NUMBER_OF_MUSIC"
#define VR_GLOBALSTATE_NUMBER_OF_ARTIST             "NUMBER_OF_ARTIST"
#define VR_GLOBALSTATE_NUMBER_OF_ALBUM              "NUMBER_OF_ALBUM"
#define VR_GLOBALSTATE_NUMBER_OF_PLAYLIST           "NUMBER_OF_PLAYLIST"

#define VR_GLOBALSTATE_MEDIA_MUSIC_SOURCE_SUPPORT   "MEDIA_MUSIC_SOURCE_SUPPORT"
#define VR_GLOBALSTATE_UPDATING_SOURCE_ID           "UPDATING_SOURCE_ID"
#define VR_GLOBALSTATE_MUSIC_DEVICE_CONNECTED       "MUSIC_DEVICE_CONNECTED"

#define VR_GLOBALSTATE_GRAMMAR_UPDATE_STATUS                "GRAMMAR_UPDATE_STATUS"
#define VR_GLOBALSTATE_GRAMMAR_UPDATE_STATUS_NEED_UPDATE    "needUpdate"
#define VR_GLOBALSTATE_GRAMMAR_UPDATE_STATUS_NO_CHANGE      "noChange"

#define VR_GLOBALSTATE_MUSIC_DICTIONARY_STATE       "MUSIC_DICTIONARY_STATE"
#define VR_GLOBALSTATE_ARTIST_DICTIONARY_STATE      "ARTIST_DICTIONARY_STATE"
#define VR_GLOBALSTATE_ALBUM_DICTIONARY_STATE       "ALBUM_DICTIONARY_STATE"
#define VR_GLOBALSTATE_PLAYLIST_DICTIONARY_STATE    "PLAYLIST_DICTIONARY_STATE"
#define VR_GLOBALSTATE_SONG_DICTIONARY_STATE        "SONG_DICTIONARY_STATE"
#define VR_GLOBALSTATE_GENRE_DICTIONARY_STATE       "GENRE_DICTIONARY_STATE"
#define VR_GLOBALSTATE_PHONEBOOK_DICTIONARY_STATE   "PHONEBOOK_DICTIONARY_STATE"
#define VR_GLOBALSTATE_DICTIONARY_STATE_SUFFIX      "_DICTIONARY_STATE"

#define VR_GLOBALSTATE_PHONE_DEVICE_ID              "PHONE_DEVICE_ID"
#define VR_GLOBALSTATE_PHONE_STATE_CONNECTED        "PHONE_STATE_CONNECTED"
#define VR_GLOBALSTATE_PHONE_STATE_REGISTERED       "PHONE_STATE_REGISTERED"
#define VR_GLOBALSTATE_PHONEBOOK_LIST_EXIST         "PHONEBOOK_LIST_EXIST"

#define VR_GLOBALSTATE_DECKLESS_MODE                "MEDIA_DECKLESS_MODEL"
#define VR_GLOBALSTATE_DECKLESS_MODE_DECKLESS       "DECKLESS"
#define VR_GLOBALSTATE_DECKLESS_MODE_NOT_DECKLESS   "NOT_DECKLESS"

#define VR_GLOBALSTATE_VALUE_TRUE   "true"
#define VR_GLOBALSTATE_VALUE_FALSE  "false"
#define VR_GLOBALSTATE_VALUE_ZERO   "0"

const std::string VR_GLOBALSTATE_ACTIVED_SOURCE_ID = "ACTIVED_SOURCE_ID";

const std::string VR_GLOBALSTATE_VALUE_EMPTY = "";
const unsigned int GRAMMAR_DIR_MAX_SIZE = 146800640; // (140M: 146800640, 6M: 6291456)

// remove paired parentheses and square brackets
pcrecpp::RE VR_DataAccessorManager::m_bracketsRule("(?:\\((?:(?>[^\\[\\]\\(\\)]+)|(?0))*\\)|\\[(?:(?>[^\\[\\]\\(\\)]+)|(?0))*\\])");
// get alias before "ft keyword" and after it
pcrecpp::RE VR_DataAccessorManager::m_featuringRule("(.*?)(Ft\\.|Feat\\.|Featuring|ft\\.|feat\\.|featuring)");
// replace consecutive space with one space in media item alias
pcrecpp::RE VR_DataAccessorManager::m_consecutiveSpaceRule("  +");
// replace special character with space
pcrecpp::RE m_specialCharRule("[\\x00\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\x08\\x09\\x0A\\x0B\\x0C\\x0D\\x0E\\x0F"
"\\x10\\x11\\x12\\x13\\x14\\x15\\x16\\x17\\x18\\x19\\x1A\\x1B\\x1C\\x1D\\x1E\\x1F"
"\\x7F"
"\\x80\\x81\\x82\\x83\\x84\\x85\\x86\\x87\\x88\\x89\\x8A\\x8B\\x8C\\x8D\\x8E\\x8F"
"\\x90\\x91\\x92\\x93\\x94\\x95\\x96\\x97\\x98\\x99\\x9A\\x9B\\x9C\\x9D\\x9E\\x9F"
"\\(\\)\\/\\[\\]\\{\\};:~!@#\\$%\\^&\\*_=\\+\\|\\\\\\<\\>-]", pcrecpp::UTF8());

VR_DataAccessorManager::VR_DataAccessorManager(VR_AsrRequestor* asrRequestor, VR_DECommonIF* common, VR_ConfigureIF* config)
    : VR_VoiceTagManager(config)
    , m_pDataSynchronizer(VR_new VR_DataSynchronizer(asrRequestor))
    , m_accessorContact(nullptr)
    , m_accessorNavi(nullptr)
    , m_accessorVoiceTag(nullptr)
    , m_accessorMedia(nullptr)
    , m_configure(config)
    , m_deCommonIF(common)
    , m_isMusicGrammarDroped(false)
    , m_isPhoneGrammarActive(false)
    , m_requestUpdateMediaGrammarFinish(false)
    , m_bIsRecognizing(false)
    , m_generateGrammarEnable(false)
{
    initializeState();

    VR_DECommonIF::DE_Country region = m_deCommonIF->getCountryType();

    // Init Accessors
    if (VR_DECommonIF::DE_Country::cn == region) {
        m_accessorNavi.reset(VR_new VR_DataAccessorNaviCN);
    }
    else if (VR_DECommonIF::DE_Country::gen == region
        && "Southeast-Asia" != m_deCommonIF->getProductCountry()
        && "SouthKorea" != m_deCommonIF->getProductCountry()) {
        m_accessorNavi.reset(VR_new VR_DataAccessorNavi(m_configure));
    }

    m_accessorVoiceTag.reset(VR_new VR_DataAccessorVoiceTag(this));
    m_accessorMedia.reset(VR_new VR_DataAccessorMedia);
    m_accessorMedia->setGrammarUpdateStatus(VR_GLOBALSTATE_GRAMMAR_UPDATE_STATUS_NO_CHANGE);
    VR_LOGD("Grammar Update Status: noChange");
    m_accessorContact.reset(VR_new VR_DataAccessorContact);

    m_musicGrammarCallback = genMusicCTermListCallback;

    // load contextID according region
    switch (region) {
    case VR_DECommonIF::DE_Country::vt:
    {
        m_categoryContextIDMap["voicetag"] = "ctx_voice_tag_contact_name_list_";
        m_voiceTagContextID = m_categoryContextIDMap["voicetag"];
        break;
    }
    case VR_DECommonIF::DE_Country::eu:
    {
        m_categoryContextIDMap["genre"] = "ctx_media_play_genre_list_";
        m_musicCategoryList.push_back("genre");
    }
    // EU region has more category than default
    default:
    {
        m_categoryContextIDMap["contact"] = "ctx_phone_contact_name_list_1";
        m_categoryContextIDMap["album"] = "ctx_media_play_album_list_";
        m_categoryContextIDMap["artist"] = "ctx_media_play_artist_list_";
        m_categoryContextIDMap["playlist"] = "ctx_media_play_playlist_list_";
        m_categoryContextIDMap["song"] = "ctx_media_play_music_list_";
        m_categoryContextIDMap["dynamic"] = "ctx_dynamic";

        m_musicCategoryList.push_back("album");
        m_musicCategoryList.push_back("artist");
        m_musicCategoryList.push_back("playlist");
        m_musicCategoryList.push_back("song");
        break;
    }
    }
}

VR_DataAccessorManager::~VR_DataAccessorManager()
{
    m_accessorMedia->closeDB();
    m_accessorContact->closeDB();
}

void VR_DataAccessorManager::updateState(const std::string &stateMsg)
{
    pugi::xml_document msgDoc;
    msgDoc.load_string(stateMsg.c_str());
    pugi::xml_node eventNode = msgDoc.select_node("//event").node();

    bool isStateChanged = false;
    std::string prePhoneDeviceID = getState(VR_GLOBALSTATE_PHONE_DEVICE_ID);

    for (pugi::xml_node_iterator it = eventNode.begin(); it != eventNode.end(); ++it) {
        std::string key(it->attribute("key").as_string());
        std::string value(it->attribute("value").as_string());
        if (VR_GLOBALSTATE_PHONE_STATE_CONNECTED == key) {
            if ("connected" == value) {
                value = VR_GLOBALSTATE_VALUE_TRUE;
            }
            else {
                value = VR_GLOBALSTATE_VALUE_FALSE;
                handlePhoneActive(false, "", false);
            }
        }
        else if (VR_GLOBALSTATE_PHONE_DEVICE_ID == key) {
            int id = std::strtol(value.c_str(), NULL, 10);
            std::ostringstream oss;
            oss << id;
            value = oss.str();
        }
        else if (VR_GLOBALSTATE_PHONE_STATE_REGISTERED == key) {
            int id = std::strtol(value.c_str(), NULL, 10);
            if (id > 0) {
                value = VR_GLOBALSTATE_VALUE_TRUE;
            }
            else {
                value = VR_GLOBALSTATE_VALUE_FALSE;
            }
        }
        else {
            // do not handle
        }

        std::string keyState(getState(key));
        if (value == keyState) {
            continue;
        }
        else {
            setState(key, value);
            isStateChanged = true;
        }
        VR_LOGI("GlobalState [%s] updated [%s]", key.c_str(), value.c_str());
    }

    if (!isStateChanged) {
        VR_LOGI("state not changed");
        return;
    }

    // std::string USB_1_State(getState("USB_1_CONNECTED"));
    // std::string USB_2_State(getState("USB_2_CONNECTED"));
    // std::string IPOD_1_State(getState("IPOD_1_CONNECTED"));
    // std::string IPOD_2_State(getState("IPOD_2_CONNECTED"));

    // if (VR_GLOBALSTATE_VALUE_TRUE == USB_1_State
    //     || VR_GLOBALSTATE_VALUE_TRUE == USB_2_State
    //     || VR_GLOBALSTATE_VALUE_TRUE == IPOD_1_State
    //     || VR_GLOBALSTATE_VALUE_TRUE == IPOD_2_State) {
    //     std::string globalMusicConnected(getState(VR_GLOBALSTATE_MUSIC_DEVICE_CONNECTED));
    //     if (VR_GLOBALSTATE_VALUE_TRUE != globalMusicConnected) {
    //         setState(VR_GLOBALSTATE_MUSIC_DEVICE_CONNECTED, VR_GLOBALSTATE_VALUE_TRUE);
    //         VR_LOGI("GlobalState [" VR_GLOBALSTATE_MUSIC_DEVICE_CONNECTED "] updated [" VR_GLOBALSTATE_VALUE_TRUE "]");
    //     }
    // }
    // else {
    //     std::string globalMusicConnected(getState(VR_GLOBALSTATE_MUSIC_DEVICE_CONNECTED));
    //     if (VR_GLOBALSTATE_VALUE_FALSE != globalMusicConnected) {
    //         setState(VR_GLOBALSTATE_MUSIC_DEVICE_CONNECTED, VR_GLOBALSTATE_VALUE_FALSE);
    //         VR_LOGI("GlobalState [" VR_GLOBALSTATE_MUSIC_DEVICE_CONNECTED "] updated [" VR_GLOBALSTATE_VALUE_FALSE "]");
    //     }
    // }

    // process voicetag grammar active/inactive
    if (!m_voiceTagContextID.empty()) {
        pugi::xml_node phoneConnectNode = eventNode.find_child_by_attribute(
            "key",
            VR_GLOBALSTATE_PHONE_STATE_CONNECTED);
        pugi::xml_node phoneDeviceNode = eventNode.find_child_by_attribute(
            "key",
            VR_GLOBALSTATE_PHONE_DEVICE_ID);
        std::string deviceAddress = getState(VR_GLOBALSTATE_PHONE_DEVICE_ID);
        if (!phoneConnectNode.empty() || !phoneDeviceNode.empty()) {
            std::string globalPhoneConnected(getState(VR_GLOBALSTATE_PHONE_STATE_CONNECTED));
            if ((VR_GLOBALSTATE_VALUE_TRUE == globalPhoneConnected) && (VR_GLOBALSTATE_VALUE_ZERO != deviceAddress)) {
                setCurrentDevice(deviceAddress);
                grammarActive("ctx_voice_tag_recognize",
                    std::string("grm_cmd_voice_tag_recognize#rul_slt_voice_tag_contact_name_list_") + deviceAddress);
            }
        }
        std::string globalPhoneID(getState(VR_GLOBALSTATE_PHONE_DEVICE_ID));
        if (!phoneDeviceNode.empty()
            && (prePhoneDeviceID != globalPhoneID)
            && (VR_GLOBALSTATE_VALUE_ZERO != prePhoneDeviceID)) {
            grammarDisactive("ctx_voice_tag_recognize",
                std::string("grm_cmd_voice_tag_recognize#rul_slt_voice_tag_contact_name_list_") + prePhoneDeviceID);
            clearCurrentDevice();
        }
    }

    notifyStateUpdated();
}

void VR_DataAccessorManager::getUpdateState(std::string& stateMsg)
{
    pugi::xml_document msgDoc;
    msgDoc.load_string("");
    pugi::xml_node dataNode = msgDoc.append_child("data");
    dataNode.append_attribute("id").set_value("globalState");
    pugi::xml_node gNode = dataNode.append_child("g");
    gNode.append_attribute("xmlns").set_value("");

    for (VoiceMap<std::string, std::string>::iterator it = m_stateMap.begin();
        it != m_stateMap.end();
        ++it) {
        pugi::xml_node itemNode = gNode.append_child("item");
        itemNode.append_attribute("key").set_value(it->first.c_str());
        itemNode.append_attribute("value").set_value(it->second.c_str());
    }

    std::ostringstream oss;
    dataNode.print(oss);
    stateMsg = oss.str();
}

void VR_DataAccessorManager::setUpdateStateCallback(
    boost::function<void(const std::string &)> &callback)
{
    m_updateStateCallback = callback;
}

void VR_DataAccessorManager::setNotifyCallback(
    boost::function<void(const std::string &)> &callback)
{
    m_notifyCallback = callback;
}

void VR_DataAccessorManager::updateGrammar(const std::string &grammarMsg)
{
    VR_LOGD_FUNC();
    VR_LOGI("updateGrammar: received GrammarMsg [%s] ", grammarMsg.c_str());

    if (VR_DECommonIF::DE_Country::vt == m_deCommonIF->getCountryType()) {
        VR_LOGI("VoiceTag Region do not handle grammar message.");
        return;
    }

    pugi::sp_xml_document spGrammarMsgDoc(VR_new pugi::xml_document());
    if (!spGrammarMsgDoc->load_string(grammarMsg.c_str())) {
        VR_LOGE("load xml from grammarMsg failed");
        return;
    }

    // process grammar msg preupdate and dispreupdate
    std::string nodeName(spGrammarMsgDoc->first_child().name());
    if (VR_GRAMMAR_MSG_PREUPDATE == nodeName) {
        handleGrammarPreactiveMsg(spGrammarMsgDoc);
        notifyStateUpdated();
        return;
    }
    else if (VR_GRAMMAR_MSG_DISPREUPDATE == nodeName) {
        handleGrammarDispreactiveMsg(spGrammarMsgDoc);
        notifyStateUpdated();
        return;
    }

    if (m_bIsRecognizing) {
        // cache grammar msg
        m_grammarMsgQuque.push(spGrammarMsgDoc);

        // check whether grammar need change
        const std::string & globalActivedSourceID = getState(VR_GLOBALSTATE_ACTIVED_SOURCE_ID);
        VR_LOGI("updateGrammar: active source [%s]", globalActivedSourceID.c_str());

        std::string msgName = spGrammarMsgDoc->first_child().name();
        bool grammarNeedchange = false;
        std::string grammarSourceID = spGrammarMsgDoc->first_child().attribute(VR_GRAMMAR_ID).as_string();
        std::string agent = spGrammarMsgDoc->first_child().attribute(VR_GRAMMAR_AGENT).as_string();
        if (globalActivedSourceID == grammarSourceID && VR_AGENT_MEDIA == agent) {
            if (VR_GRAMMAR_MSG_DISACTIVE == msgName) {
                std::string reply = spGrammarMsgDoc->first_child().attribute("reply").as_string();
                VR_LOGI("updateGrammar: grammarid [%s] reply[%s]", grammarSourceID.c_str(), reply.c_str());
                if ("true" == reply) {
                    grammarNeedchange = true;
                }
            }
            else if (VR_GRAMMAR_MSG_INIT == msgName || VR_GRAMMAR_MSG_DIFF == msgName) {
                grammarNeedchange = true;
            }
        }

        // set change status
        if (grammarNeedchange) {
            setState(VR_GLOBALSTATE_GRAMMAR_UPDATE_STATUS, VR_GLOBALSTATE_GRAMMAR_UPDATE_STATUS_NEED_UPDATE);
            m_accessorMedia->setGrammarUpdateStatus(VR_GLOBALSTATE_GRAMMAR_UPDATE_STATUS_NEED_UPDATE);
            VR_LOGD("Grammar Update Status: needUpdate");
        }
    }
    else {
        handleGrammarMsg(spGrammarMsgDoc);
        }
}

void VR_DataAccessorManager::updateGrammar(pugi_vr::sp_xml_document spGrammarMsgDoc)
{
    VR_LOGD_FUNC();
    if (VR_DECommonIF::DE_Country::vt == m_deCommonIF->getCountryType()) {
        VR_LOGI("VoiceTag Region do not handle grammar message.");
        return;
    }

    // process grammar msg preupdate and dispreupdate
    std::string nodeName(spGrammarMsgDoc->first_child().name());
    if (VR_GRAMMAR_MSG_PREUPDATE == nodeName) {
        handleGrammarPreactiveMsg(spGrammarMsgDoc);
        return;
    }
    else if (VR_GRAMMAR_MSG_DISPREUPDATE == nodeName) {
        handleGrammarDispreactiveMsg(spGrammarMsgDoc);
        return;
    }

    if (m_bIsRecognizing) {
        // cache grammar msg
        m_grammarMsgQuque.push(spGrammarMsgDoc);

        // check whether grammar need change
        const std::string & globalActivedSourceID = getState(VR_GLOBALSTATE_ACTIVED_SOURCE_ID);
        VR_LOGI("updateGrammar: active source [%s]", globalActivedSourceID.c_str());

        std::string msgName = spGrammarMsgDoc->first_child().name();
        bool grammarNeedchange = false;
        std::string grammarSourceID = spGrammarMsgDoc->first_child().attribute(VR_GRAMMAR_ID).as_string();
        std::string agent = spGrammarMsgDoc->first_child().attribute(VR_GRAMMAR_AGENT).as_string();
        if (globalActivedSourceID == grammarSourceID && VR_AGENT_MEDIA == agent) {
            if (VR_GRAMMAR_MSG_DISACTIVE == msgName) {
                std::string reply = spGrammarMsgDoc->first_child().attribute("reply").as_string();
                VR_LOGI("updateGrammar: grammarid [%s] reply[%s]", grammarSourceID.c_str(), reply.c_str());
                if ("true" == reply) {
                    grammarNeedchange = true;
                }
            }
            else if (VR_GRAMMAR_MSG_INIT == msgName || VR_GRAMMAR_MSG_DIFF == msgName) {
                grammarNeedchange = true;
            }
        }

        // set change status
        if (grammarNeedchange) {
            setState(VR_GLOBALSTATE_GRAMMAR_UPDATE_STATUS, VR_GLOBALSTATE_GRAMMAR_UPDATE_STATUS_NEED_UPDATE);
            m_accessorMedia->setGrammarUpdateStatus(VR_GLOBALSTATE_GRAMMAR_UPDATE_STATUS_NEED_UPDATE);
            VR_LOGD("Grammar Update Status: needUpdate");
        }
    }
    else {
        handleGrammarMsg(spGrammarMsgDoc);
    }

}

void VR_DataAccessorManager::clearAllGrammar()
{
    m_pDataSynchronizer->stopGrammar();
    VR_DECommonIF::DE_Country region = m_deCommonIF->getCountryType();
    if (VR_DECommonIF::DE_Country::vt == region) {
        // clear voiceTag grammar
        clearVoiceTagGrammar();
    }
    else {
        // clear music grammar
        for (int i = 1; i < 5; ++i) {
            std::ostringstream oss;
            oss << i;
            std::string index = oss.str();
            for (VoiceList<std::string>::iterator it = m_musicCategoryList.begin();
                it != m_musicCategoryList.end();
                ++it) {
                std::string contextID = m_categoryContextIDMap[*it] + index;
                // VR_GRAMMAR_TYPE_MEDIA FOR CLC
                m_pDataSynchronizer->deleteGrammar(contextID, VR_GRAMMAR_TYPE_DEFLAUT);
            }
        }
    }

    // clear phone grammar
    m_pDataSynchronizer->deleteGrammar(
        "ctx_phone_contact_name_list_1",
        VR_GRAMMAR_TYPE_DEFLAUT,
        boost::bind(&VR_DataAccessorManager::clearGrammarCallback,
            this,
            _1));
}

bool VR_DataAccessorManager::saveVoiceTagGrammar(
    const std::string &voiceTagID,
    const std::string &name,
    const std::string &phoneme,
    const std::string &deviceAddress)
{
    if (m_voiceTagContextID.empty()) {
        return false;
    }
    VoiceList<spC_Term>::type addList;
    addList.push_back(VR_DataSynchronizer::getCTerm(atoi(voiceTagID.c_str()), name, phoneme));
    return m_pDataSynchronizer->updateGrammar(
        m_voiceTagContextID + deviceAddress,
        VR_GRAMMAR_TYPE_DEFLAUT,
        addList,
        boost::bind(&VR_DataAccessorManager::voiceTagUpdateGrammarCallback,
            this,
            _1,
            deviceAddress));
}

bool VR_DataAccessorManager::deleteVoiceTagGrammar(
    const VoiceList<std::pair<std::string, std::string>>::type &deleteList,
    const std::string &deviceAddress)
{
    if (m_voiceTagContextID.empty()) {
        return false;
    }
    VoiceList<spC_Term>::type delList;
    for (VoiceList<std::pair<std::string, std::string>>::const_iterator it = deleteList.cbegin();
        it != deleteList.cend();
        ++it) {
        delList.push_back(
            VR_DataSynchronizer::getCTerm(
                atoi((it->first).c_str()),
                it->second));
    }
    return m_pDataSynchronizer->deleteGrammar(
        m_voiceTagContextID + deviceAddress,
        VR_GRAMMAR_TYPE_DEFLAUT,
        delList,
        boost::bind(&VR_DataAccessorManager::voiceTagDeleteGrammarCallback,
            this,
            _1,
            deviceAddress));
}

bool VR_DataAccessorManager::deleteAllVoiceTagGrammar(const std::string &deviceAddress)
{
    if (m_voiceTagContextID.empty()) {
        return false;
    }
    return m_pDataSynchronizer->deleteGrammar(
        m_voiceTagContextID + deviceAddress,
        VR_GRAMMAR_TYPE_DEFLAUT,
        boost::bind(&VR_DataAccessorManager::voiceTagDeleteGrammarCallback,
            this,
            _1,
            deviceAddress));
}

bool VR_DataAccessorManager::clearVoiceTagGrammar()
{
    if (m_voiceTagContextID.empty()) {
        return false;
    }
    bool result = m_pDataSynchronizer->deleteGrammar(m_voiceTagContextID + "1", VR_GRAMMAR_TYPE_DEFLAUT);
    result = result && m_pDataSynchronizer->deleteGrammar(m_voiceTagContextID + "2", VR_GRAMMAR_TYPE_DEFLAUT);
    result = result && m_pDataSynchronizer->deleteGrammar(m_voiceTagContextID + "3", VR_GRAMMAR_TYPE_DEFLAUT);
    result = result && m_pDataSynchronizer->deleteGrammar(m_voiceTagContextID + "4", VR_GRAMMAR_TYPE_DEFLAUT);
    result = result && m_pDataSynchronizer->deleteGrammar(m_voiceTagContextID + "5", VR_GRAMMAR_TYPE_DEFLAUT);
    return result;
}

// invoke in ASR thread
void VR_DataAccessorManager::voiceTagUpdateGrammarCallback(
    const N_Vr::N_Asr::C_Event_Phase &phaseEvent,
    const std::string &deviceAddress)
{
    N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type msgType = phaseEvent.Get_Event_Phase_Type();
    if (msgType == N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Proc_End) {
        m_pDataSynchronizer->notifyUpdateGrammarCategoryFinish("voicetag");
    }
}

// invoke in ASR thread
void VR_DataAccessorManager::voiceTagDeleteGrammarCallback(
    const N_Vr::N_Asr::C_Event_Phase &phaseEvent,
    const std::string &deviceAddress)
{
    N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type msgType = phaseEvent.Get_Event_Phase_Type();
    if (msgType == N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Proc_End) {
        m_pDataSynchronizer->notifyUpdateGrammarCategoryFinish("voicetag");
    }
}

// invoke in ASR thread
void VR_DataAccessorManager::updateGrammarCallback(
    const N_Vr::N_Asr::C_Event_Phase &phaseEvent,
    const std::string &category)
{
    N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type msgType = phaseEvent.Get_Event_Phase_Type();
    switch (msgType) {
    case N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Proc_Begin:
        {
            break;
        }
    case N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Proc_End:
        {
            m_pDataSynchronizer->notifyUpdateGrammarCategoryFinish(category);
            break;
        }
    default:
        break;
    }
}

// invoke in ASR thread
void VR_DataAccessorManager::clearGrammarCallback(const N_Vr::N_Asr::C_Event_Phase &phaseEvent)
{
    N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type msgType = phaseEvent.Get_Event_Phase_Type();
    switch (msgType) {
    case N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Proc_Begin:
        {
            break;
        }
    case N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Proc_End:
        {
            if (m_notifyCallback) {
                m_notifyCallback("<event-result name=\"initialpersondata\"></event>");
            }
            break;
        }
    default:
        break;
    }
}

// invoke in ASR thread
void VR_DataAccessorManager::notifyGrammarCallback(const N_Vr::N_Asr::C_Event_Notify &notifyEvent, const std::string &category, const std::string &contextID, unsigned int totalCount)
{
    N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type msgType = notifyEvent.Get_Event_Notify_Type();
    if (N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type_Context_List_File_Size != msgType) {
        return;
    }

    static unsigned int size = 0;

    if (VR_GRAMMAR_CATEGORY_CONTACT == category) { // phonebook grammar
        size = notifyEvent.Get_Context_File_Size();
    }
    else {
        static unsigned int count = 0;
        ++count;
        size += notifyEvent.Get_Context_File_Size();
        if (count < totalCount) {
            return;
        }
        count = 0;
    }
    VR_LOGD("get file size finished");

    SaveGrammarTaskInfo info;
    info.isActive = false;
    info.size = size;
    size = 0;

    // get current SourceID from contextID
    std::string currentSourceID = "0";
    if (category != VR_GRAMMAR_CATEGORY_CONTACT) {
        currentSourceID = contextID.substr(contextID.find_last_of('_') + 1);
    }
    info.sourceID = currentSourceID;

    if (getGrammarSpaceForSave(info)) {
        // send save request to ASR, finished when receive it's callback
        saveGrammarToFile(info);
    }
    else {
        std::string infoMsg = getXMLMsgFromTaskInfo(info);
        m_pDataSynchronizer->notifyGrammarSpaceNotEnough(infoMsg);
    }
}

void VR_DataAccessorManager::requestUpdateGrammar(N_Vr::N_Asr::C_Request_Context_List_Update &updateMsg)
{
    m_pDataSynchronizer->updateGrammarWithoutSave(updateMsg);
}

// invoke in ASR thread
unsigned int VR_DataAccessorManager::getAvailableGrammarSize()
{
    std::string grammarFilePath = m_configure->getUsrPath() + "../ASR";
    unsigned int dirSize = static_cast<unsigned int>(getGrammarDirSize(grammarFilePath));
    if (GRAMMAR_DIR_MAX_SIZE > dirSize) {
        return (GRAMMAR_DIR_MAX_SIZE - dirSize);
    }
    else {
        return 0;
    }
}

// invoke in ASR thread
off_t VR_DataAccessorManager::getGrammarDirSize(const std::string &path)
{
    DIR *dir = nullptr;
    struct dirent *dirEntry = nullptr;
    off_t dirSize = 0;

    dir = opendir(path.c_str());
    if (!dir) {
        VR_LOGD("failed to open dir: %s", path.c_str());
        return dirSize;
    }
    dirEntry = readdir(dir);
    while (nullptr != dirEntry) {
        if (strcmp(dirEntry->d_name, ".") == 0 || strcmp(dirEntry->d_name, "..") == 0) { // current dir OR parrent dir
            dirEntry = readdir(dir);
            continue;
        }
        else if (dirEntry->d_type == 8) { // file
            struct stat fileStat;
            std::string fileName = path + "/" + dirEntry->d_name;
            int result = stat(fileName.c_str(), &fileStat);
            if (0 != result) {
                VR_LOGD("failed to get file stat of file:%s", fileName.c_str());
                dirEntry = readdir(dir);
                continue;
            }
            dirSize += fileStat.st_size;
        }
        else if (dirEntry->d_type == 4) { // dir
            std::string subDirPath = path + "/" + dirEntry->d_name;
            dirSize += getGrammarDirSize(subDirPath);
        }
        dirEntry = readdir(dir);
    }

    closedir(dir);
    return dirSize;
}

// invoke in ASR thread
bool VR_DataAccessorManager::getGrammarSpaceForSave(SaveGrammarTaskInfo &info)
{
    VR_LOGD_FUNC();
    unsigned int availableSize = getAvailableGrammarSize();
    VR_LOGD("grammar size: %d, availableSize: %d", info.size, availableSize);
    if (info.size <= availableSize) {
        return true;
    }
    return false;
}

// invoke in DE thread
void VR_DataAccessorManager::handleGrammarSpaceNotEnough(const std::string &infoMsg)
{
    SaveGrammarTaskInfo info = getTaskInfoFromXMLMsg(infoMsg);
    handleGrammarSpaceNotEnough(info);
}

// invoke in DE thread
void VR_DataAccessorManager::handleGrammarSpaceNotEnough(SaveGrammarTaskInfo &info)
{
    std::string globalActivedSourceID(getState(VR_GLOBALSTATE_ACTIVED_SOURCE_ID));
    info.isActive = (globalActivedSourceID == info.sourceID);

    // get sourceIDs that can be deleted
    VoiceList<std::string>::type sourceIDList;
    sourceIDList.push_back("1");
    sourceIDList.push_back("2");
    sourceIDList.push_back("3");
    sourceIDList.push_back("4");
    sourceIDList.remove(globalActivedSourceID);

    info.sourceIDList.swap(sourceIDList);
    if (!requestDeleteMediaGrammar(info)) {
        bool isPhoneBook = (VR_GLOBALSTATE_VALUE_ZERO == info.sourceID);
        if (!isPhoneBook && !info.isActive) {  // music grammar
            std::string size = std::to_string(info.size);
            m_pDataSynchronizer->notifyUpdateGrammarCategoryFailed(info.sourceID, size);
        }
        else {
            // send save request to ASR, finished when receive it's callback
            saveGrammarToFile(info);
        }
    }
}

// invoke in ASR thread
bool VR_DataAccessorManager::requestDeleteMediaGrammar(SaveGrammarTaskInfo &info)
{
    VR_LOGD_FUNC();
    bool isPhoneBook = (VR_GLOBALSTATE_VALUE_ZERO == info.sourceID);
    if (!isPhoneBook && !info.isActive) {
        VR_LOGD("Inactive music grammar out of range, do not delete other grammar.");
        return false;
    }

    if (info.sourceIDList.empty()) {
        VR_LOGD("No grammar can be delete.");
        return false;
    }

    std::string deviceID = info.sourceIDList.front();
    info.sourceIDList.pop_front();
    std::string infoMsg = getXMLMsgFromTaskInfo(info);
    m_pDataSynchronizer->notifyDeleteMediaGrammar(deviceID, infoMsg);
    return true;
}

// invoke in ASR thread
void VR_DataAccessorManager::saveGrammarToFile(SaveGrammarTaskInfo &info)
{
    VR_LOGD_FUNC();
    m_pDataSynchronizer->notifySaveGrammarFile(info.sourceID);
}

// invoke in ASR thread
void VR_DataAccessorManager::deleteMediaGrammarCallback(const N_Vr::N_Asr::C_Event_Phase &phaseEvent, const std::string &category, SaveGrammarTaskInfo &info)
{
    static VoiceList<std::string>::type categoryList = m_musicCategoryList;

    N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type msgType = phaseEvent.Get_Event_Phase_Type();
    if (N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Proc_End == msgType) {
        categoryList.remove(category);
    }

    if (categoryList.empty()) {
        categoryList = m_musicCategoryList;
        unsigned int availableSize = getAvailableGrammarSize();
        if (info.size <= availableSize) {
            // send save request to ASR, finished when receive it's callback
            saveGrammarToFile(info);
        }
        else {
            if (!requestDeleteMediaGrammar(info)) {
                bool isPhoneBook = (VR_GLOBALSTATE_VALUE_ZERO == info.sourceID);
                if (!isPhoneBook && !info.isActive) {  // music grammar
                    std::string size = std::to_string(info.size);
                    m_pDataSynchronizer->notifyUpdateGrammarCategoryFailed(info.sourceID, size);
                }
                else {
                    // send save request to ASR, finished when receive it's callback
                    saveGrammarToFile(info);
                }
            }
        }
    }
}

// invoke in ASR thread
void VR_DataAccessorManager::saveGrammarFileCallback(const N_Vr::N_Asr::C_Event_Phase &phaseEvent, const std::string &category)
{
    N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type msgType = phaseEvent.Get_Event_Phase_Type();
    if (N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Proc_End != msgType) {
        return;
    }
    m_pDataSynchronizer->notifyUpdateGrammarCategoryFinish(category);
}

std::string VR_DataAccessorManager::getXMLMsgFromTaskInfo(const SaveGrammarTaskInfo &info)
{
    pugi::xml_document doc;
    pugi::xml_node infoNode = doc.append_child("SaveGrammarTaskInfo");
    infoNode.append_child("isActive").text().set(info.isActive);
    infoNode.append_child("size").text().set(info.size);
    infoNode.append_child("sourceID").text().set(info.sourceID.c_str());
    pugi::xml_node sourceIDListNode = infoNode.append_child("sourceIDList");
    for (auto &sourceID : info.sourceIDList) {
        sourceIDListNode.append_child("ID").text().set(sourceID.c_str());
    }
    std::ostringstream oss;
    doc.print(oss);
    return oss.str();
}

SaveGrammarTaskInfo VR_DataAccessorManager::getTaskInfoFromXMLMsg(const std::string &infoMsg)
{
    SaveGrammarTaskInfo info;
    pugi::xml_document doc;
    doc.load_string(infoMsg.c_str());
    pugi::xml_node infoNode = doc.select_node("//SaveGrammarTaskInfo").node();
    info.isActive = infoNode.child("isActive").text().as_bool();
    info.size = infoNode.child("size").text().as_int();
    info.sourceID = infoNode.child("sourceID").text().as_string();
    pugi::xml_node sourceIDListNode = infoNode.child("sourceIDList");
    pugi::xml_node sourceIDNode = sourceIDListNode.first_child();
    while (sourceIDNode) {
        std::string sourceID = sourceIDNode.text().as_string();
        info.sourceIDList.push_back(sourceID);
        sourceIDNode = sourceIDNode.next_sibling();
    }
    return info;
}

// invoke in DE thread
void VR_DataAccessorManager::onDeleteMediaGrammar(const std::string &sourceID, const std::string &infoMsg)
{
    SaveGrammarTaskInfo info = getTaskInfoFromXMLMsg(infoMsg);
    handleDeleteMediaGrammar(sourceID, info);
}

// invoke in DE thread
void VR_DataAccessorManager::handleDeleteMediaGrammar(const std::string &sourceID, SaveGrammarTaskInfo &info)
{
    VR_LOGD_FUNC();
    VR_LOGD("delete grammar, ID: %s", sourceID.c_str());
    std::string contextID;
    VoiceList<spC_Term>::type addList;
    std::string currentSourceID = info.sourceID;
    for (auto categoryName : m_musicCategoryList) {
        contextID = m_categoryContextIDMap[categoryName] + sourceID;
        m_pDataSynchronizer->deleteGrammarFile(
            contextID,
            boost::bind(&VR_DataAccessorManager::deleteMediaGrammarCallback,
                this,
                _1,
                categoryName,
                info));
    }

    if (currentSourceID != sourceID) {
        m_emptyGrammarIDSet.insert(sourceID);
        requestMediaGrammar(sourceID);
    }
}

void VR_DataAccessorManager::requestMediaGrammar(const std::string &sourceID)
{
    VR_LOGD_FUNC();
    VR_LOGD("current grammarID:%s", sourceID.c_str());
    pugi::xml_document msgDoc;
    pugi::xml_node actionNode = msgDoc.append_child("action");
    actionNode.append_attribute("agent").set_value("media");
    actionNode.append_attribute("op").set_value("requestGrammar");
    actionNode.append_attribute("grammarID").set_value(sourceID.c_str());
    std::ostringstream oss;
    actionNode.print(oss);
    if (m_notifyCallback) {
        m_notifyCallback(oss.str());
    }
}

// invoke in DE thread
void VR_DataAccessorManager::onSaveGrammarToFile(const std::string &sourceID)
{
    if (VR_GLOBALSTATE_VALUE_ZERO == sourceID) {
        std::string contextID = m_categoryContextIDMap[VR_GRAMMAR_CATEGORY_CONTACT];
        m_pDataSynchronizer->saveGrammarFile(
            contextID,
            boost::bind(&VR_DataAccessorManager::saveGrammarFileCallback,
                    this,
                    _1,
                    VR_GRAMMAR_CATEGORY_CONTACT));
    }
    else {
        for (auto &category : m_musicCategoryList) {
            std::string contextID = m_categoryContextIDMap[category] + sourceID;
            m_pDataSynchronizer->saveGrammarFile(
                contextID,
                boost::bind(&VR_DataAccessorManager::saveGrammarFileCallback,
                        this,
                        _1,
                        category));
        }
    }
}

void VR_DataAccessorManager::checkDirSizeForIFlytek()
{
}

void VR_DataAccessorManager::getInfo(
    const std::string &operation,
    const std::string &reqMsg,
    std::string &response)
{
    VR_LOGD_FUNC();
    VR_DataAccessor * accessor = getAccessor(operation);
    if (accessor) {
        accessor->getInfo(operation, reqMsg, response);
    }
}

void VR_DataAccessorManager::setCountryID(const std::string &countryID)
{
    if (!m_accessorNavi) {
        VR_LOGI("current region do not support Navi voice recognition");
        return;
    }
    m_accessorNavi->setCountryID(countryID);
}

bool VR_DataAccessorManager::isRecognizing()
{
    VR_LOGD_FUNC();
    return m_bIsRecognizing;
}

void VR_DataAccessorManager::handleMusicResult(pugi::xml_node &result, const std::string &category)
{
    // eliminate repeated item id
    std::string categoryID = category + "_id";
    pugi::xml_node itemsNode = result.select_node("//items").node();

    if (!itemsNode) {
        VR_LOGE("no items node in asr result.");
        return;
    }

    pugi::xml_node itemNode = itemsNode.first_child();
    while (itemNode) {
        std::string currentID = itemNode.child(categoryID.c_str()).text().as_string();
        pugi::xpath_node_set repeatedSet = itemsNode.select_nodes((std::string(".//") + categoryID + "[text()='" + currentID + "']").c_str());
        if (repeatedSet.size() <= 1) {
            itemNode = itemNode.next_sibling();
            continue;
        }
        pugi::xpath_node_set::iterator it = repeatedSet.begin();
        ++it; // set iterator to second record
        while (it != repeatedSet.end()) {
            itemsNode.remove_child(it->node().parent());
            ++it;
        }
        itemNode = itemNode.next_sibling();
    }

    // replace category name with formal name from database
    if (!m_accessorMedia) {
        VR_LOGE("point of accessor media is null.");
        return;
    }
    m_accessorMedia->updateMusicItemName(itemsNode, category);
}

void VR_DataAccessorManager::onRecognizeBegin()
{
    VR_LOGD_FUNC();
    m_bIsRecognizing = true;

    // check grammar need update
    checkGrammarNeedUpdate();
}

void VR_DataAccessorManager::onRecognizeEnd()
{
    VR_LOGD_FUNC();
    m_bIsRecognizing = false;
    while (!m_grammarMsgQuque.empty()) {
        handleGrammarMsg(m_grammarMsgQuque.front());
        m_grammarMsgQuque.pop();
    }

    if (m_pDataSynchronizer->isAsrIdle()) {
        triggerGenerateGrammar();
    }
}

void VR_DataAccessorManager::onUpdateGrammarCategoryFailed(const std::string &sourceID, const std::string &size)
{
    std::string globalActivedSourceID(getState(VR_GLOBALSTATE_ACTIVED_SOURCE_ID));
    if (globalActivedSourceID == sourceID) { // current sourceID is actived, check size again.
        SaveGrammarTaskInfo info;
        info.isActive = false;
        info.size = stoi(size);
        info.sourceID = sourceID;

        if (getGrammarSpaceForSave(info)) {
            onSaveGrammarToFile(sourceID);
        }
        else {
            handleGrammarSpaceNotEnough(info);
        }
        return;
    }

    m_isMusicGrammarDroped = true;
    m_emptyGrammarIDSet.insert(sourceID);
    for (auto &categoryName : m_musicCategoryList) {
        onUpdateGrammarCategoryFinish(categoryName);
    }
}

void VR_DataAccessorManager::onUpdateGrammarCategoryFinish(const std::string &category)
{
    std::string key = category;
    if (VR_GRAMMAR_CATEGORY_CONTACT == key) {
        key = "PHONEBOOK";
        VR_LOGP("case:212-16-799 212-17-799 212-18-799 DataAccessor generate phonebook grammar end");
        if (m_notifyCallback) {
            m_notifyCallback("<grammar_result op=\"grammar\" agent=\"" VR_AGENT_PHONE "\" errcode=\"0\" />");
        }
    }

    bool needUpdateGlobalState = true;

    VoiceList<std::string>::iterator it = std::find(m_musicCategoryList.begin(), m_musicCategoryList.end(), category);
    if (it != m_musicCategoryList.end()) {
        m_updatingDictionaryStateMap[category] = VR_DICTIONARY_STATE_OK;
        std::string globalUpdatingSourceID = getState(VR_GLOBALSTATE_UPDATING_SOURCE_ID);
        std::string globalActivedSourceID = getState(VR_GLOBALSTATE_ACTIVED_SOURCE_ID);
        if (globalUpdatingSourceID != globalActivedSourceID) {
            needUpdateGlobalState = false;
        }
        checkMusicGrammarState();
        if (needUpdateGlobalState && VR_DICTIONARY_STATE_OK == m_updatingDictionaryStateMap["music"]) {
            setState(VR_GLOBALSTATE_MUSIC_DICTIONARY_STATE, VR_DICTIONARY_STATE_OK);
            VR_LOGI("GlobalState [" VR_GLOBALSTATE_MUSIC_DICTIONARY_STATE "] updated [" VR_DICTIONARY_STATE_OK "]");
        }
    }

    if (needUpdateGlobalState) {
        std::transform(key.begin(), key.end(), key.begin(), toupper);
        key += VR_GLOBALSTATE_DICTIONARY_STATE_SUFFIX;
        setState(key, VR_DICTIONARY_STATE_OK);
        VR_LOGI("GlobalState [%s] updated [" VR_DICTIONARY_STATE_OK "]", key.c_str());
    }
    notifyStateUpdated();

    m_pDataSynchronizer->updateGrammarCategoryFinish(category);
    if (m_pDataSynchronizer->isAsrIdle()) {
        onUpdateGrammarFinish();
    }
}

void VR_DataAccessorManager::onUpdateGrammarFinish()
{
    m_generatingGrammarMsg.reset();
    triggerGenerateGrammar();
}

void VR_DataAccessorManager::triggerGenerateGrammar()
{
    VR_LOGD_FUNC();
    // check grammar state need update
    checkGrammarNeedUpdate();

    // process next grammar msg
    if (m_bIsRecognizing) {
        return;
    }

    if (m_generateGrammarList.empty()) {
        return;
    }

    while (m_pDataSynchronizer->isAsrIdle() && !m_generateGrammarList.empty() && m_generateGrammarEnable) {
        m_generatingGrammarMsg = m_generateGrammarList.front();
        generateGrammar(m_generatingGrammarMsg);
        m_generateGrammarList.pop_front();
    }
}

void VR_DataAccessorManager::handleGrammarMsg(pugi::sp_xml_document &spGrammarMsgDoc)
{
    // execute actvie msg and push grammar msg to grammarstack
    VR_LOGD_FUNC();
    if (!m_voiceTagContextID.empty()) {
        VR_LOGI("VoiceTag Region not process normal grammar message");
        return;
    }
    std::string nodeName(spGrammarMsgDoc->first_child().name());

    if (VR_GRAMMAR_MSG_ACTIVE == nodeName || VR_GRAMMAR_MSG_DISACTIVE == nodeName) {
        handleGrammarActiveMsg(spGrammarMsgDoc);
    }
    else if (VR_GRAMMAR_MSG_INIT == nodeName
        || VR_GRAMMAR_MSG_DIFF == nodeName
        || VR_GRAMMAR_MSG_NEW == nodeName
        || VR_GRAMMAR_MSG_DYNAMIC == nodeName) {
        handleGrammarGenerateMsg(spGrammarMsgDoc);
    }
    else {
        // don't handle
    }
}

void VR_DataAccessorManager::handleGrammarActiveMsg(pugi::sp_xml_document &spGrammarMsgDoc)
{
    VR_LOGD_FUNC();
    std::string nodeName(spGrammarMsgDoc->first_child().name());
    std::string grammarID(spGrammarMsgDoc->first_child().attribute(VR_GRAMMAR_ID).as_string());
    std::string agent(spGrammarMsgDoc->first_child().attribute(VR_GRAMMAR_AGENT).as_string());
    std::string path(spGrammarMsgDoc->first_child().attribute(VR_GRAMMAR_PATH).as_string());
    int songCount(atoi(spGrammarMsgDoc->first_child().attribute(VR_GRAMMAR_SONG_COUNT).as_string()));
    int otherCount(atoi(spGrammarMsgDoc->first_child().attribute(VR_GRAMMAR_GENERAL_COUNT).as_string()));

    if (VR_GRAMMAR_MSG_ACTIVE == nodeName) {
        if (VR_AGENT_MEDIA == agent) {
            handleMusicActive(true, grammarID, songCount, otherCount, path);
        }
        else if (VR_AGENT_PHONE == agent) {
            handlePhoneActive(true, m_configure->getUsrPath() + path);
        }
    }
    else if (VR_GRAMMAR_MSG_DISACTIVE == nodeName) {
        bool needReply = true;
        if (std::string("false") == spGrammarMsgDoc->first_child().attribute("reply").as_string()) {
            needReply = false;
        }
        if (VR_AGENT_MEDIA == agent) {
            handleMusicActive(false, grammarID, 0, 0, "", needReply);
        }
        else if (VR_AGENT_PHONE == agent) {
            handlePhoneActive(false, "");
        }
    }
}

void VR_DataAccessorManager::handleGrammarGenerateMsg(pugi::sp_xml_document &spGrammarMsgDoc)
{
    VR_LOGD_FUNC();
    // remove old conflict grammar msg
    pugi::xml_node msgNode = spGrammarMsgDoc->first_child();
    std::string currentGrammarID;
    if (std::string(VR_GRAMMAR_MSG_INIT) == msgNode.name()) {
        std::string currentAgent = msgNode.attribute(VR_GRAMMAR_AGENT).as_string();
        currentGrammarID = msgNode.attribute(VR_GRAMMAR_ID).as_string();

        VoiceList<pugi::sp_xml_document>::iterator it = m_generateGrammarList.begin();
        while (m_generateGrammarList.end() != it) {
            std::string agent = (*it)->first_child().attribute(VR_GRAMMAR_AGENT).as_string();
            std::string grammarID = (*it)->first_child().attribute(VR_GRAMMAR_ID).as_string();
            if (currentAgent == agent && currentGrammarID == grammarID) {
                it = m_generateGrammarList.erase(it);
                continue;
            }
            ++it;
        }
    }

    if (!currentGrammarID.empty() && (m_priorGrammarID == currentGrammarID)) {
        m_priorGrammarID.clear();
        m_generateGrammarList.push_front(spGrammarMsgDoc);
    }
    else {
        m_generateGrammarList.push_back(spGrammarMsgDoc);
    }

    // process next grammar msg
    while (m_pDataSynchronizer->isAsrIdle() && !m_generateGrammarList.empty() && m_generateGrammarEnable) {
        m_generatingGrammarMsg = m_generateGrammarList.front();
        generateGrammar(m_generatingGrammarMsg);
        m_generateGrammarList.pop_front();
    }
}

void VR_DataAccessorManager::handleGrammarPreactiveMsg(pugi::sp_xml_document &spGrammarMsgDoc)
{
    VR_LOGD_FUNC();
    setState(VR_GLOBALSTATE_MUSIC_DEVICE_CONNECTED, VR_GLOBALSTATE_VALUE_TRUE);
}

void VR_DataAccessorManager::handleGrammarDispreactiveMsg(pugi::sp_xml_document &spGrammarMsgDoc)
{
    VR_LOGD_FUNC();
    setState(VR_GLOBALSTATE_MUSIC_DEVICE_CONNECTED, VR_GLOBALSTATE_VALUE_FALSE);
}

void VR_DataAccessorManager::generateGrammar(pugi::sp_xml_document &spGrammarMsgDoc)
{
    VR_LOGD_FUNC();
    pugi::xml_node grammarNode = spGrammarMsgDoc->first_child();

    std::string isEmpty = grammarNode.attribute("isempty").as_string();

    std::string nodeName(grammarNode.name());
    std::string grammarID(grammarNode.attribute(VR_GRAMMAR_ID).as_string());
    std::string agent(grammarNode.attribute(VR_GRAMMAR_AGENT).as_string());
    std::string path(grammarNode.attribute(VR_GRAMMAR_PATH).as_string());
    int songCount(grammarNode.attribute(VR_GRAMMAR_SONG_COUNT).as_int());
    int otherCount(grammarNode.attribute(VR_GRAMMAR_GENERAL_COUNT).as_int());

    VR_DECommonIF::DE_Country eRegion = m_deCommonIF->getCountryType();
    if (VR_DECommonIF::DE_Country::eu == eRegion) {
        otherCount += atoi(spGrammarMsgDoc->first_child().attribute(VR_GRAMMAR_GENRE_COUNT).as_string());
    }

    if (VR_GRAMMAR_MSG_INIT == nodeName) { // notify ASR update Grammar by read DB
        if (VR_GLOBALSTATE_VALUE_TRUE == isEmpty) {
            VR_LOGD("Null Device connected, do not generate grammar.");
            std::string msgToDM(std::string("<grammar_result op=\"grammar\" agent=\"" VR_AGENT_MEDIA "\" grammarid=\"")
            + grammarID
            + "\" errcode=\"0\" />");
            if (m_notifyCallback) {
                m_notifyCallback(msgToDM);
            }
            return;
        }
        if (VR_AGENT_MEDIA == agent) {
            handleMediaGrammarInit(path, grammarID, songCount, otherCount);
        }
        else if (VR_AGENT_PHONE == agent) {
            handlePhoneGrammarInit(m_configure->getUsrPath() + path);
        }
    }
    else if (VR_GRAMMAR_MSG_DIFF == nodeName) { // notify ASR update Grammar by diff msg
        if (VR_DECommonIF::DE_Country::cn == eRegion) {
            handleMediaGrammarInit(path, grammarID, songCount, otherCount);
        }
        else {
            pugi::xml_node category;
            category = spGrammarMsgDoc->first_child().first_child();
            handleMediaGrammarDiff(category, grammarID, songCount, otherCount);
        }
    }
    else if (VR_GRAMMAR_MSG_NEW == nodeName) { // notify ASR update Dynamic Criteria
        pugi::xml_node category;
        category = spGrammarMsgDoc->first_child().first_child();
        handleGrammarNew(category);
    }
    else if (VR_GRAMMAR_MSG_DYNAMIC == nodeName) { // notify ASR update Dynamic Criteria
        pugi::xml_node category;
        category = spGrammarMsgDoc->first_child().first_child();
        handleGrammarDynamic(category);
    }
}

std::string VR_DataAccessorManager::selectGrammarType(std::string categoryName)
{
    if (categoryName == "song") {
        return VR_GRAMMAR_TYPE_TITLE;  // for song
    }
    else if (categoryName == "playlist") {
        return VR_GRAMMAR_TYPE_NORMAL; // for playlist
    }
    else if (categoryName == "artist") {
        return VR_GRAMMAR_TYPE_ARTIST; // for artist
    }
    else if (categoryName == "album") {
        return VR_GRAMMAR_TYPE_ALBUM; // for album
    }
    else if (categoryName == "genre") {
        return VR_GRAMMAR_TYPE_NORMAL; // for genre
    }
    else {
        return VR_GRAMMAR_TYPE_NORMAL;
    }
}

void VR_DataAccessorManager::handleMediaGrammarInit(
    const std::string &path,
    const std::string &grammarID,
    int songCount,
    int otherCount)
{
    VR_LOGD_FUNC();
    VR_LOGP("case:212-9-760 212-10-760 212-11-760 DataAccessor get music full grammar message");
    sqlite3 * dbHandler;
    int result = sqlite3_open_v2(path.c_str(), &dbHandler, SQLITE_OPEN_READONLY | SQLITE_OPEN_NOMUTEX, NULL);
    if (SQLITE_OK != result) {
        {
            static bool errorLogMark = false;
            if (!errorLogMark) {
                errorLogMark = true;
                VR_ERROR(VOICE_VR_ERR_OPEN_DB, VR_DE_OPTIONDATA_NONE, "Open DB file %s failed, resultID: [%d]", path.c_str(), result);
            }
        }
        sqlite3_close(dbHandler);
        return;
    }

    VoiceList<std::string>::type grammarList;
    getAvailableCategory(grammarList, songCount, otherCount);

    if (grammarList.empty()) {
        std::string msgToDM(std::string("<grammar_result op=\"grammar\" agent=\"" VR_AGENT_MEDIA "\" grammarid=\"")
        + grammarID
        + "\" errcode=\"1\" />");
        if (m_notifyCallback) {
            m_notifyCallback(msgToDM);
        }
    }
    else {
        if (grammarList.size() < m_musicCategoryList.size()) {
            m_isMusicGrammarDroped = true;
        }
    }

    m_requestUpdateMediaGrammarFinish = false;
    char * errmsg = nullptr;
    std::string sqlRequest;
    std::string sqlCommand("SELECT nExternalId,cName,nPronunciationId,cPronunciation FROM ");
    std::string categoryName;
    std::string contextID;
    VoiceList<spC_Term>::type addList;
    for (VoiceList<std::string>::iterator it = m_musicCategoryList.begin();
        it != m_musicCategoryList.end();
        ++it) {
        addList.clear();
        categoryName.assign(*it);

        if (!isCategoryAvailable(categoryName)) {
            continue;
        }

        contextID = m_categoryContextIDMap[categoryName] + grammarID;

        VR_LOGD("ContextID: [%s]", contextID.c_str());

        size_t size = grammarList.size();
        grammarList.remove(categoryName);
        if (grammarList.size() != size) {
            sqlRequest.assign(sqlCommand + VR_DataAccessorMedia::getMusicDBTableName(categoryName) + ";");

            result = sqlite3_exec(dbHandler,
                sqlRequest.c_str(),
                m_musicGrammarCallback, &addList,
                &errmsg);
            if (SQLITE_OK != result) {
                {
                    static bool errorLogMark = false;
                    if (!errorLogMark) {
                        errorLogMark = true;
                        VR_ERROR(VOICE_VR_ERR_SQL, VR_DE_OPTIONDATA_NONE, "Run SQL request VR_DataAccessorManager handleMediaGrammarInit failed, error code: [%d], error msg: [%s]",
                            result,
                            errmsg);
                    }
                }
                continue;
            }
        }

        VR_LOGD("Records Number: [%d]", addList.size());

        VR_LOGD("Start updateGrammar");
        VR_LOGP("case:212-9-770 212-10-770 212-11-770 DataAccessor generate music full grammar begin");
        updateMusicGrammarState(categoryName, grammarID);
        std::string clcType = selectGrammarType(categoryName);

        N_Vr::N_Asr::C_Request_Context_List_Update updateMsg;
        updateMsg.m_string_Id_Context.assign(contextID);
        updateMsg.m_list_spo_Term_For_Add.swap(addList);
        updateMsg.m_string_Id_Category = clcType;
        updateMsg.m_b_Clear = true;
        updateMsg.m_function_On_Event_Notify = boost::bind(&VR_DataAccessorManager::notifyGrammarCallback,
                                                            this,
                                                            _1,
                                                            categoryName,
                                                            contextID,
                                                            m_musicCategoryList.size());

        requestUpdateGrammar(updateMsg);
    }
    sqlite3_close(dbHandler);
    m_requestUpdateMediaGrammarFinish = true;
    checkDirSizeForIFlytek();
}

void VR_DataAccessorManager::handleMediaGrammarDiff(
    pugi::xml_node &category,
    const std::string &grammarID,
    int songCount,
    int otherCount)
{
    VR_LOGD_FUNC();

    VoiceList<std::string>::type grammarList;
    getAvailableCategory(grammarList, songCount, otherCount);

    if (grammarList.empty()) {
        std::string msgToDM(std::string("<grammar_result op=\"grammar\" agent=\"" VR_AGENT_MEDIA "\" grammarid=\"")
        + grammarID
        + "\" errcode=\"1\" />");
        if (m_notifyCallback) {
            m_notifyCallback(msgToDM);
        }
    }
    else {
        if (grammarList.size() < m_musicCategoryList.size()) {
            m_isMusicGrammarDroped = true;
        }
    }

    m_requestUpdateMediaGrammarFinish = false;
    VR_LOGP("case:212-12-760 212-13-760 212-14-760 212-15-760 DataAccessor get music diff grammar message");
    VoiceList<spC_Term>::type addList;
    VoiceList<spC_Term>::type delList;
    std::string categoryName;
    std::string contextID;
    pugi::xml_node tempNode;

    VoiceList<pugi::xml_node>::type categoryNodeList;
    while (!category.empty()) {
        std::string name = category.attribute(VR_CATEGORY_NAME).as_string();

        if (grammarList.end() == std::find(grammarList.begin(), grammarList.end(), name)) {
            category = category.next_sibling();
            continue; // category is not in grammarList, skip
        }

        if (!isCategoryAvailable(name)) {
            category = category.next_sibling();
            continue;
        }
        categoryNodeList.push_back(category);
        category = category.next_sibling();
    }

    for (auto &categoryNode : categoryNodeList) {
        addList.clear();
        delList.clear();
        categoryName.assign(categoryNode.attribute(VR_CATEGORY_NAME).as_string());

        contextID = m_categoryContextIDMap[categoryName] + grammarID;

        VR_LOGD("ContextID: [%s]", contextID.c_str());

        // add "delete item"
        tempNode = categoryNode.child(VR_DELETE_NODE_NAME);
        if (!tempNode.empty()) {
            tempNode = tempNode.first_child();
            while (!tempNode.empty()) {
                int id(tempNode.attribute(VR_ITEM_ID).as_int());
                std::string name(tempNode.attribute(VR_ITEM_NAME).as_string());
                getMusicCTermList(delList, id, name);
                tempNode = tempNode.next_sibling();
            }
        }

        // add "add item"
        tempNode = categoryNode.child(VR_ADD_NODE_NAME);
        if (!tempNode.empty()) {
            tempNode = tempNode.first_child();
            while (!tempNode.empty()) {
                int id(tempNode.attribute(VR_ITEM_ID).as_int());
                std::string name(tempNode.attribute(VR_ITEM_NAME).as_string());
                std::string pronIDStr(tempNode.attribute(VR_ITEM_PRON_ID).as_string());
                std::string pronNameStr(tempNode.attribute(VR_ITEM_PRON).as_string());
                getMusicCTermList(addList, id, name, pronIDStr, pronNameStr);
                tempNode = tempNode.next_sibling();
            }
        }

        if (!addList.empty() || !delList.empty()) {
            VR_LOGD("Start updateGrammar");
            VR_LOGP("case:212-12-770 212-13-770 212-14-770 212-15-770 DataAccessor generate music diff grammar begin");
            updateMusicGrammarState(categoryName, grammarID);
            std::string clcType = selectGrammarType(categoryName);

            N_Vr::N_Asr::C_Request_Context_List_Update updateMsg;
            updateMsg.m_string_Id_Context.assign(contextID);
            updateMsg.m_list_spo_Term_For_Add.swap(addList);
            updateMsg.m_string_Id_Category = clcType;
            updateMsg.m_function_On_Event_Notify = boost::bind(&VR_DataAccessorManager::notifyGrammarCallback,
                                                                this,
                                                                _1,
                                                                categoryName,
                                                                contextID,
                                                                categoryNodeList.size());

            requestUpdateGrammar(updateMsg);
        }
    }
    m_requestUpdateMediaGrammarFinish = true;
}

void VR_DataAccessorManager::handlePhoneGrammarInit(const std::string &path)
{
    VR_LOGP("case:212-9-760 212-10-760 212-11-760 DataAccessor get phone init grammar message");
    VoiceList<spC_Term>::type addList;
    processContactGrammarNew(addList, path);

    updateOtherGrammarState(VR_GRAMMAR_CATEGORY_CONTACT);
    std::string contextID = m_categoryContextIDMap[VR_GRAMMAR_CATEGORY_CONTACT];
    VR_LOGD("ContextID: [%s]", contextID.c_str());
    VR_LOGP("case:212-16-770 212-17-770 212-18-770 DataAccessor generate phonebook grammar begin");
    std::string clcType = VR_GRAMMAR_TYPE_PHONE; // for phone

    N_Vr::N_Asr::C_Request_Context_List_Update updateMsg;
    updateMsg.m_string_Id_Context.assign(contextID);
    updateMsg.m_list_spo_Term_For_Add.swap(addList);
    updateMsg.m_string_Id_Category = clcType;
    updateMsg.m_b_Clear = true;
    updateMsg.m_function_On_Event_Notify = boost::bind(&VR_DataAccessorManager::notifyGrammarCallback,
                                                        this,
                                                        _1,
                                                        VR_GRAMMAR_CATEGORY_CONTACT,
                                                        contextID,
                                                        0);

    requestUpdateGrammar(updateMsg);
    checkDirSizeForIFlytek();
}

void VR_DataAccessorManager::handleGrammarNew(pugi::xml_node &category)
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

        // if (VR_GRAMMAR_CATEGORY_AUDIOSOURCE == categoryName) {
        //     pugi::xml_node idNode = category.first_child();
        //     processAudioSourceGrammarNew(idNode, addList);
        // }
        // else if (VR_GRAMMAR_CATEGORY_QUICKREPLYMESSAGE == categoryName) {
        //     pugi::xml_node messageNode = category.first_child();
        //     processQuickReplyMsgGrammarNew(messageNode, addList);
        // }
        // else if (VR_GRAMMAR_CATEGORY_SATCHANNELNUMBER == categoryName) { // is number, not create grammar
        //     pugi::xml_node idNode = category.first_child();
        //     processNoGrammarAndSaveToDB(categoryName, idNode);
        // }
        // else if (VR_GRAMMAR_CATEGORY_PHONETYPE == categoryName) { // phonetype not create grammar
        //     pugi::xml_node idNode = category.first_child();
        //     processNoGrammarAndSaveToDB(categoryName, idNode);
        // }
        // else {
        //     pugi::xml_node idNode = category.first_child();
        //     processOtherGrammarNew(idNode, categoryName, addList);
        // }
        // else {
            VR_LOGD("unhandle category, skip");
            category = category.next_sibling();
            continue;
        // }

        updateOtherGrammarState(categoryName);
        std::string clcType = VR_GRAMMAR_TYPE_NORMAL; // for radio, but not use
        m_pDataSynchronizer->updateGrammar(
            contextID,
            clcType,
            addList,
            boost::bind(&VR_DataAccessorManager::updateGrammarCallback,
                this,
                _1,
                categoryName),
            true);
        category = category.next_sibling();
    }
}

void VR_DataAccessorManager::handleGrammarDynamic(pugi::xml_node &category)
{
    VR_LOGD_FUNC();
    VoiceList<spC_Term>::type addList;
    std::string categoryName;
    std::string contextID;

    while (!category.empty()) {
        addList.clear();
        categoryName.assign(category.attribute(VR_CATEGORY_NAME).as_string());
        if ("default" == categoryName) {
            categoryName = "dynamic";
        }

        if (!isCategoryAvailable(categoryName)) {
            category = category.next_sibling();
            continue;
        }
        contextID = m_categoryContextIDMap[categoryName];
        VR_LOGD("ContextID: [%s]", contextID.c_str());

        // updateOtherGrammarState(categoryName);
        std::string clcType = VR_GRAMMAR_TYPE_NORMAL; // for radio, but not use
        m_pDataSynchronizer->updateGrammar(
            contextID,
            clcType,
            addList,
            boost::bind(&VR_DataAccessorManager::updateGrammarCallback,
                this,
                _1,
                categoryName),
            true);
        category = category.next_sibling();
    }
}

void VR_DataAccessorManager::handleMusicActive(
    bool isActive,
    const std::string &grammarID,
    int songCount,
    int otherCount,
    const std::string &path,
    bool needResponse)
{
    VR_LOGD_FUNC();
    std::string operation;
    std::string globalActivedSourceID(getState(VR_GLOBALSTATE_ACTIVED_SOURCE_ID));
    if (isActive) {
        if (VR_GLOBALSTATE_VALUE_ZERO != globalActivedSourceID) {
            setMusicGrammarActive(globalActivedSourceID, false, 0, 0);
            m_accessorMedia->closeDB();
        }

        m_accessorMedia->openDB(path);

        // update state
        setState(VR_GLOBALSTATE_NUMBER_OF_MUSIC, m_accessorMedia->getSongNumber());
        setState(VR_GLOBALSTATE_NUMBER_OF_ARTIST, m_accessorMedia->getArtistNumber());
        setState(VR_GLOBALSTATE_NUMBER_OF_ALBUM, m_accessorMedia->getAlbumNumber());
        setState(VR_GLOBALSTATE_NUMBER_OF_PLAYLIST, m_accessorMedia->getPlaylistNumber());

        setMusicGrammarActive(grammarID, true, songCount, otherCount);

        int total = songCount + otherCount;
        total = total > VR_MAX_MUSIC_GRAMMAR_COUNT ? otherCount : total;
        std::ostringstream oss;
        oss << total;
        setState(VR_GLOBALSTATE_NUMBER_OF_TOTAL, oss.str());
        setState(VR_GLOBALSTATE_ACTIVED_SOURCE_ID, grammarID);
        setState(VR_GLOBALSTATE_MEDIA_MUSIC_SOURCE_SUPPORT, VR_GLOBALSTATE_VALUE_TRUE);

        std::string globalUpdatingSourceID(getState(VR_GLOBALSTATE_UPDATING_SOURCE_ID));
        if (grammarID == globalUpdatingSourceID) {
            setState(VR_GLOBALSTATE_MUSIC_DICTIONARY_STATE, m_updatingDictionaryStateMap["music"]);
            setState(VR_GLOBALSTATE_ARTIST_DICTIONARY_STATE, m_updatingDictionaryStateMap["artist"]);
            setState(VR_GLOBALSTATE_ALBUM_DICTIONARY_STATE, m_updatingDictionaryStateMap["album"]);
            setState(VR_GLOBALSTATE_PLAYLIST_DICTIONARY_STATE, m_updatingDictionaryStateMap["playlist"]);
            setState(VR_GLOBALSTATE_SONG_DICTIONARY_STATE, m_updatingDictionaryStateMap["song"]);
            setState(VR_GLOBALSTATE_GENRE_DICTIONARY_STATE, m_updatingDictionaryStateMap["genre"]);
        }
        else {
            setState(VR_GLOBALSTATE_MUSIC_DICTIONARY_STATE, VR_DICTIONARY_STATE_OK);
            setState(VR_GLOBALSTATE_ARTIST_DICTIONARY_STATE, VR_DICTIONARY_STATE_OK);
            setState(VR_GLOBALSTATE_ALBUM_DICTIONARY_STATE, VR_DICTIONARY_STATE_OK);
            setState(VR_GLOBALSTATE_PLAYLIST_DICTIONARY_STATE, VR_DICTIONARY_STATE_OK);
            setState(VR_GLOBALSTATE_SONG_DICTIONARY_STATE, VR_DICTIONARY_STATE_OK);
            setState(VR_GLOBALSTATE_GENRE_DICTIONARY_STATE, VR_DICTIONARY_STATE_OK);
        }

        if (m_emptyGrammarIDSet.end() != m_emptyGrammarIDSet.find(grammarID)) {
            m_emptyGrammarIDSet.erase(grammarID);
            requestMediaGrammar(grammarID);
            setState(VR_GLOBALSTATE_MUSIC_DICTIONARY_STATE, VR_DICTIONARY_STATE_SYNCING);
            setState(VR_GLOBALSTATE_ARTIST_DICTIONARY_STATE, VR_DICTIONARY_STATE_SYNCING);
            setState(VR_GLOBALSTATE_ALBUM_DICTIONARY_STATE, VR_DICTIONARY_STATE_SYNCING);
            setState(VR_GLOBALSTATE_PLAYLIST_DICTIONARY_STATE, VR_DICTIONARY_STATE_SYNCING);
            setState(VR_GLOBALSTATE_SONG_DICTIONARY_STATE, VR_DICTIONARY_STATE_SYNCING);
            setState(VR_GLOBALSTATE_GENRE_DICTIONARY_STATE, VR_DICTIONARY_STATE_SYNCING);
            m_priorGrammarID = grammarID;
        }

        operation = "active";
    }
    else {
        setMusicGrammarActive(grammarID, false, 0, 0);
        if (grammarID != globalActivedSourceID) {
            return;
        }

        m_accessorMedia->closeDB();
        setState(VR_GLOBALSTATE_NUMBER_OF_MUSIC, VR_GLOBALSTATE_VALUE_ZERO);
        setState(VR_GLOBALSTATE_NUMBER_OF_ARTIST, VR_GLOBALSTATE_VALUE_ZERO);
        setState(VR_GLOBALSTATE_NUMBER_OF_ALBUM, VR_GLOBALSTATE_VALUE_ZERO);
        setState(VR_GLOBALSTATE_NUMBER_OF_PLAYLIST, VR_GLOBALSTATE_VALUE_ZERO);
        setState(VR_GLOBALSTATE_NUMBER_OF_TOTAL, VR_GLOBALSTATE_VALUE_ZERO);
        setState(VR_GLOBALSTATE_ACTIVED_SOURCE_ID, VR_GLOBALSTATE_VALUE_ZERO);
        setState(VR_GLOBALSTATE_MEDIA_MUSIC_SOURCE_SUPPORT, VR_GLOBALSTATE_VALUE_FALSE);

        setState(VR_GLOBALSTATE_MUSIC_DICTIONARY_STATE, VR_DICTIONARY_STATE_NONE);
        setState(VR_GLOBALSTATE_ARTIST_DICTIONARY_STATE, VR_DICTIONARY_STATE_NONE);
        setState(VR_GLOBALSTATE_ALBUM_DICTIONARY_STATE, VR_DICTIONARY_STATE_NONE);
        setState(VR_GLOBALSTATE_PLAYLIST_DICTIONARY_STATE, VR_DICTIONARY_STATE_NONE);
        setState(VR_GLOBALSTATE_SONG_DICTIONARY_STATE, VR_DICTIONARY_STATE_NONE);
        setState(VR_GLOBALSTATE_GENRE_DICTIONARY_STATE, VR_DICTIONARY_STATE_NONE);
        operation = "disactive";

        // handleMediaNullDevice();
    }

    VR_LOGI("GlobalState update:\n"
        "[" VR_GLOBALSTATE_NUMBER_OF_TOTAL "]:[%s]\n"
        "[" VR_GLOBALSTATE_NUMBER_OF_MUSIC "]:[%s]\n"
        "[" VR_GLOBALSTATE_NUMBER_OF_ARTIST "]:[%s]\n"
        "[" VR_GLOBALSTATE_NUMBER_OF_ALBUM "]:[%s]\n"
        "[" VR_GLOBALSTATE_NUMBER_OF_PLAYLIST "]:[%s]\n"
        "[%s]:[%s]\n"
        "[" VR_GLOBALSTATE_MEDIA_MUSIC_SOURCE_SUPPORT "]:[%s]\n"
        , getState(VR_GLOBALSTATE_NUMBER_OF_MUSIC).c_str()
        , getState(VR_GLOBALSTATE_NUMBER_OF_TOTAL).c_str()
        , getState(VR_GLOBALSTATE_NUMBER_OF_ARTIST).c_str()
        , getState(VR_GLOBALSTATE_NUMBER_OF_ALBUM).c_str()
        , getState(VR_GLOBALSTATE_NUMBER_OF_PLAYLIST).c_str()
        , VR_GLOBALSTATE_ACTIVED_SOURCE_ID.c_str()
        , getState(VR_GLOBALSTATE_ACTIVED_SOURCE_ID).c_str()
        , getState(VR_GLOBALSTATE_MEDIA_MUSIC_SOURCE_SUPPORT).c_str());

    VR_LOGI("GlobalState update:\n"
        "[" VR_GLOBALSTATE_MUSIC_DICTIONARY_STATE "]:[%s]\n"
        "[" VR_GLOBALSTATE_ARTIST_DICTIONARY_STATE "]:[%s]\n"
        "[" VR_GLOBALSTATE_ALBUM_DICTIONARY_STATE "]:[%s]\n"
        "[" VR_GLOBALSTATE_PLAYLIST_DICTIONARY_STATE "]:[%s]\n"
        "[" VR_GLOBALSTATE_SONG_DICTIONARY_STATE "]:[%s]\n"
        "[" VR_GLOBALSTATE_GENRE_DICTIONARY_STATE "]:[%s]\n"
        , getState(VR_GLOBALSTATE_MUSIC_DICTIONARY_STATE).c_str()
        , getState(VR_GLOBALSTATE_ARTIST_DICTIONARY_STATE).c_str()
        , getState(VR_GLOBALSTATE_ALBUM_DICTIONARY_STATE).c_str()
        , getState(VR_GLOBALSTATE_PLAYLIST_DICTIONARY_STATE).c_str()
        , getState(VR_GLOBALSTATE_SONG_DICTIONARY_STATE).c_str()
        , getState(VR_GLOBALSTATE_GENRE_DICTIONARY_STATE).c_str());

    if (needResponse && m_notifyCallback) {
        m_notifyCallback(std::string("<grammar_result op=\"")
            + operation
            + "\" agent=\"" VR_AGENT_MEDIA "\" grammarid=\""
            + grammarID
            + "\" errcode=\"0\" />");
    }
    notifyStateUpdated();
}

void VR_DataAccessorManager::handlePhoneActive(bool isActive, const std::string &path, bool needResponse)
{
    VR_LOGD_FUNC();
    std::string operation;
    bool isSwitched = false;
    if (isActive) {
        operation = "active";
        if (!m_isPhoneGrammarActive) {
            m_isPhoneGrammarActive = true;
            isSwitched = true;

            m_accessorContact->openDB(path);
            setPhoneContactGrammarActive(true);
            int contactCount = m_accessorContact->getContactCount();
            std::string phoneDictState;
            if (0 != contactCount) {
                setState(VR_GLOBALSTATE_PHONEBOOK_LIST_EXIST, VR_GLOBALSTATE_VALUE_TRUE);
                phoneDictState = VR_DICTIONARY_STATE_OK;
            }
            else {
                setState(VR_GLOBALSTATE_PHONEBOOK_LIST_EXIST, VR_GLOBALSTATE_VALUE_FALSE);
                phoneDictState = VR_DICTIONARY_STATE_NONE;
            }

            bool isPhoneDictTBD = false;
            VoiceList<pugi::sp_xml_document>::iterator it = m_generateGrammarList.begin();
            while (m_generateGrammarList.end() != it) {
                // VR_GRAMMAR_MSG_INIT and VR_AGENT_PHONE means phonebook contact grammar msg
                if (std::string(VR_AGENT_PHONE) == (*it)->first_child().attribute(VR_GRAMMAR_AGENT).as_string()
                    && std::string(VR_GRAMMAR_MSG_INIT) == (*it)->first_child().name()) {
                    isPhoneDictTBD = true;
                    break;
                }
                ++it;
            }

            if (isPhoneDictTBD || getState(VR_GLOBALSTATE_PHONEBOOK_DICTIONARY_STATE) == VR_DICTIONARY_STATE_SYNCING) {
                phoneDictState = VR_DICTIONARY_STATE_SYNCING;
            }
            setState(VR_GLOBALSTATE_PHONEBOOK_DICTIONARY_STATE, phoneDictState);
            VR_LOGI("GlobalState [" VR_GLOBALSTATE_PHONEBOOK_LIST_EXIST "] updated [%s]\n"
                "GlobalState [" VR_GLOBALSTATE_PHONEBOOK_DICTIONARY_STATE "] updated [%s]"
                , getState(VR_GLOBALSTATE_PHONEBOOK_LIST_EXIST).c_str()
                , phoneDictState.c_str());
        }
    }
    else {
        operation = "disactive";
        if (m_isPhoneGrammarActive) {
            m_isPhoneGrammarActive = false;
            isSwitched = true;

            setPhoneContactGrammarActive(false);
            m_accessorContact->closeDB();
            setState(VR_GLOBALSTATE_PHONEBOOK_LIST_EXIST, VR_GLOBALSTATE_VALUE_FALSE);
            setState(VR_GLOBALSTATE_PHONEBOOK_DICTIONARY_STATE, VR_DICTIONARY_STATE_NONE);
            VR_LOGI("GlobalState [" VR_GLOBALSTATE_PHONEBOOK_LIST_EXIST "] updated [%s]", getState(VR_GLOBALSTATE_PHONEBOOK_LIST_EXIST).c_str());
            VR_LOGI("GlobalState [" VR_GLOBALSTATE_PHONEBOOK_DICTIONARY_STATE "] updated [" VR_DICTIONARY_STATE_NONE "]");
        }
    }
    if (needResponse && m_notifyCallback) {
        m_notifyCallback(std::string("<grammar_result op=\"") + operation + "\" agent=\"" VR_AGENT_PHONE "\" errcode=\"0\" />");
    }
    if (isSwitched) {
        notifyStateUpdated();
    }
}

void VR_DataAccessorManager::processAudioSourceList(pugi::xml_node &audioSourceNode)
{
    VR_LOGD_FUNC();
    if (!audioSourceNode) {
        VR_LOGD("audioSourceNode is empty, ingore this message.");
        return;
    }

    std::string currentDecklessMode = VR_GLOBALSTATE_DECKLESS_MODE_DECKLESS;
    m_sourceNameMap.clear();

    // loop audio source list
    pugi::xml_node formalNode = audioSourceNode.first_child();
    while (!formalNode.empty()) {
        std::string audioSourceName = formalNode.attribute(VR_ITEM_NAME).as_string();
        std::string mediaSourceName = formalNode.attribute("medianame").as_string();
        if ("USB" == audioSourceName
            || "USB2" == audioSourceName
            || "iPod" == audioSourceName
            || "iPod2" == audioSourceName) {
            formatSourceName(audioSourceName);
            formatSourceName(mediaSourceName);
            m_sourceNameMap[audioSourceName] = mediaSourceName;
        }
        else if ("CD" == audioSourceName) {
            currentDecklessMode = VR_GLOBALSTATE_DECKLESS_MODE_NOT_DECKLESS;
        }
        else {
            // do not handle
        }
        formalNode = formalNode.next_sibling();
    }

    // check deckless
    std::string oldDecklessMode = getState(VR_GLOBALSTATE_DECKLESS_MODE);
    if (currentDecklessMode != oldDecklessMode) {
        setState(VR_GLOBALSTATE_DECKLESS_MODE, currentDecklessMode);
        notifyStateUpdated();
    }
}

void VR_DataAccessorManager::formatSourceName(std::string &sourceName)
{
    if ("USB2" == sourceName) {
        sourceName = "USB 2";
    }
    else if ("iPod2" == sourceName) {
        sourceName = "iPod 2";
    }
    else {
        // do not handle
    }
}

std::string VR_DataAccessorManager::getMediaSourceName(std::string uiSourceName)
{
    VR_LOGD_FUNC();
    std::string mediaSourceName;
    if (m_sourceNameMap.end() != m_sourceNameMap.find(uiSourceName)) {
        mediaSourceName = m_sourceNameMap.at(uiSourceName);
    }
    else {
        mediaSourceName = uiSourceName;
    }
    return mediaSourceName;
}

void VR_DataAccessorManager::processContactGrammarNew(
    VoiceList<spC_Term>::type &addList,
    const std::string &path)
{
    sqlite3 * dbHandler;
    int result = sqlite3_open_v2(path.c_str(), &dbHandler, SQLITE_OPEN_READONLY | SQLITE_OPEN_NOMUTEX, NULL);
    if (SQLITE_OK != result) {
        {
            static bool errorLogMark = false;
            if (!errorLogMark) {
                errorLogMark = true;
                VR_ERROR(VOICE_VR_ERR_OPEN_DB, VR_DE_OPTIONDATA_NONE, "Open DB file %s failed, resultID: [%d]", path.c_str(), result);
            }
        }
        sqlite3_close(dbHandler);
        return;
    }

    std::string sqlCommand("SELECT id,full FROM contact");
    char * errmsg = NULL;
    result = sqlite3_exec(dbHandler, sqlCommand.c_str(), genContactCTermListCallback, &addList, &errmsg);
    if (SQLITE_OK != result) {
        {
            static bool errorLogMark = false;
            if (!errorLogMark) {
                errorLogMark = true;
                VR_ERROR(VOICE_VR_ERR_SQL, VR_DE_OPTIONDATA_NONE, "Run SQL request VR_DataAccessorManager processContactGrammarNew failed, error code: [%d], error msg: [%s]", result, errmsg);
            }
        }
        sqlite3_close(dbHandler);
        return;
    }
    sqlite3_close(dbHandler);
}

// void VR_DataAccessorManager::processQuickReplyMsgGrammarNew(
//     pugi::xml_node &messageNode,
//     VoiceList<spC_Term>::type &addList)
// {
//     while (!messageNode.empty()) {
//         unsigned int id = messageNode.attribute(VR_ITEM_ID).as_int();
//         std::string idStr(messageNode.attribute(VR_ITEM_ID).as_string());
//         std::string name(messageNode.attribute(VR_ITEM_NAME).as_string());
//         addList.push_back(VR_DataSynchronizer::getCTerm(id, name));
//         insertRecord(VR_GRAMMAR_CATEGORY_QUICKREPLYMESSAGE, idStr, name);
//         messageNode = messageNode.next_sibling();
//     }
// }

// void VR_DataAccessorManager::processAudioSourceGrammarNew(
//     pugi::xml_node &idNode,
//     VoiceList<spC_Term>::type &addList)
// {
//     pugi::xml_node nameNode;
//     while (!idNode.empty()) {
//         unsigned int id = idNode.attribute(VR_ITEM_ID).as_int();
//         std::string idStr = idNode.attribute(VR_ITEM_ID).as_string();
//         std::string nameStr = idNode.attribute(VR_ITEM_NAME).as_string();
//         std::string grammarId = idNode.attribute(VR_GRAMMAR_ID).as_string();
//         insertRecord(VR_GRAMMAR_CATEGORY_AUDIOSOURCE, idStr, nameStr, grammarId);

//         nameNode = idNode.first_child();
//         while (!nameNode.empty()) {
//             addList.push_back(
//                 VR_DataSynchronizer::getCTerm(
//                     id,
//                     std::string(nameNode.attribute(VR_ITEM_NAME).as_string()),
//                     std::string(nameNode.attribute(VR_ITEM_PRON).as_string())));
//             nameNode = nameNode.next_sibling();
//         }
//         idNode = idNode.next_sibling();
//     }
// }

// void VR_DataAccessorManager::processNoGrammarAndSaveToDB(
//     const std::string &tableName,
//     pugi::xml_node &idNode)
// {
//     VR_LOGD_FUNC();
//     while (!idNode.empty()) {
//         std::string idStr(idNode.attribute(VR_ITEM_ID).as_string());
//         std::string name(idNode.attribute(VR_ITEM_NAME).as_string());
//         insertRecord(tableName, idStr, name);
//         idNode = idNode.next_sibling();
//     }
// }

// void VR_DataAccessorManager::processOtherGrammarNew(
//     pugi::xml_node &idNode,
//     std::string &categoryName,
//     VoiceList<spC_Term>::type &addList)
// {
//     pugi::xml_node nameNode;
//     while (!idNode.empty()) {
//         unsigned int id = idNode.attribute(VR_ITEM_ID).as_int();
//         std::string idStr = idNode.attribute(VR_ITEM_ID).as_string();
//         std::string nameStr = idNode.attribute(VR_ITEM_NAME).as_string();
//         std::string shortcut = idNode.attribute(VR_ITEM_SHORTCUT).as_string();
//         insertRecord(categoryName, idStr, nameStr, shortcut);

//         if (!shortcut.empty()) {
//             addList.push_back(VR_DataSynchronizer::getCTerm(id, shortcut.c_str()));
//         }
//         nameNode = idNode.first_child();
//         while (!nameNode.empty()) {
//             addList.push_back(
//                 VR_DataSynchronizer::getCTerm(
//                     id,
//                     std::string(nameNode.attribute(VR_ITEM_NAME).as_string()),
//                     std::string(nameNode.attribute(VR_ITEM_PRON).as_string())));
//             nameNode = nameNode.next_sibling();
//         }
//         idNode = idNode.next_sibling();
//     }
// }

void VR_DataAccessorManager::setMusicGrammarActive(
    const std::string &grammarID,
    bool enable,
    int songCount,
    int otherCount)
{
    VoiceList<std::string>::type mediaMainList;
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
            categoryName = "music";
        }
        else {
            categoryName = *it;
        }

        mediaMainList.push_back(std::string("grm_cmd_media_main#rul_slt_media_play_") + categoryName + "_list_" + grammarID);
        std::string ruleID = std::string("grm_cmd_media_play_")
            + categoryName + "#rul_slt_media_play_"
            + categoryName + "_list_" + grammarID;
        if (enable) {
            grammarActive(std::string("ctx_media_play_") + categoryName, ruleID);
        }
        else {
            grammarDisactive(std::string("ctx_media_play_") + categoryName, ruleID);
        }
    }
    if (enable) {
        grammarActive("ctx_media_main", mediaMainList);
        if ((songCount + otherCount) > VR_MAX_MUSIC_GRAMMAR_COUNT
            && songCount <= VR_MAX_MUSIC_GRAMMAR_COUNT) {
            mediaMainList.remove(std::string("grm_cmd_media_main#rul_slt_media_play_music_list_") + grammarID);
        }
        grammarActive("ctx_topmenu_main", mediaMainList);
    }
    else {
        grammarDisactive("ctx_media_main", mediaMainList);
        grammarDisactive("ctx_topmenu_main", mediaMainList);
    }
}

void VR_DataAccessorManager::setPhoneContactGrammarActive(bool enable)
{
    if (enable) {
        grammarActive("ctx_phone_contact", "grm_cmd_phone_contact#rul_slt_phone_contact_name_list_1");
        grammarActive("ctx_phone_main", "grm_cmd_phone_main#rul_slt_phone_contact_name_list_1");
        grammarActive("ctx_topmenu_main", "grm_cmd_phone_main#rul_slt_phone_contact_name_list_1");
    }
    else {
        grammarDisactive("ctx_phone_contact", "grm_cmd_phone_contact#rul_slt_phone_contact_name_list_1");
        grammarDisactive("ctx_phone_main", "grm_cmd_phone_main#rul_slt_phone_contact_name_list_1");
        grammarDisactive("ctx_topmenu_main", "grm_cmd_phone_main#rul_slt_phone_contact_name_list_1");
    }
}

void VR_DataAccessorManager::grammarActive(const std::string &contextID, const VoiceList<std::string>::type &ruleIDList)
{
    m_pDataSynchronizer->setGrammarActive(contextID, true, ruleIDList);
}

void VR_DataAccessorManager::grammarActive(const std::string &contextID, const std::string &ruleID)
{
    VoiceList<std::string>::type ruleIDList(1, ruleID);
    m_pDataSynchronizer->setGrammarActive(contextID, true, ruleIDList);
}

void VR_DataAccessorManager::grammarDisactive(const std::string &contextID, const VoiceList<std::string>::type &ruleIDList)
{
    m_pDataSynchronizer->setGrammarActive(contextID, false, ruleIDList);
}

void VR_DataAccessorManager::grammarDisactive(const std::string &contextID, const std::string &ruleID)
{
    VoiceList<std::string>::type ruleIDList(1, ruleID);
    m_pDataSynchronizer->setGrammarActive(contextID, false, ruleIDList);
}

bool VR_DataAccessorManager::isCategoryAvailable(const std::string &category)
{
    return (m_categoryContextIDMap.end() != m_categoryContextIDMap.find(category));
}

void VR_DataAccessorManager::getAvailableCategory(VoiceList<std::string>::type &categoryList, int songCount, int otherCount)
{
    int totalCount = otherCount + songCount;
    categoryList = m_musicCategoryList;

    if (totalCount > VR_MAX_MUSIC_GRAMMAR_COUNT) {
        if (otherCount > VR_MAX_MUSIC_GRAMMAR_COUNT) {
            categoryList.clear();
            categoryList.push_back("song");
        }
        if (songCount > VR_MAX_MUSIC_GRAMMAR_COUNT) {
            categoryList.remove("song");
        }
    }
}

VR_DataAccessor * VR_DataAccessorManager::getAccessor(const std::string &operation)
{
    if (m_accessorContact->isOperationHandled(operation)) {
        return m_accessorContact.get();
    }
    else if (m_accessorNavi && m_accessorNavi->isOperationHandled(operation)) {
        return m_accessorNavi.get();
    }
    else if (m_accessorVoiceTag->isOperationHandled(operation)) {
        return m_accessorVoiceTag.get();
    }
    else if (m_accessorMedia->isOperationHandled(operation)) {
        return m_accessorMedia.get();
    }
    else {
        return nullptr;
    }
}

// void VR_DataAccessorManager::createTable()
// {
//     char * errmsg = nullptr;
//     int result;
//     VoiceList<std::string>::type tableNameList;
//     std::string sqlRequest("SELECT name FROM sqlite_master WHERE type='table';");
//     result = sqlite3_exec(m_dbHandler,
//         sqlRequest.c_str(),
//         getSingleColumnList,
//         &tableNameList,
//         &errmsg);
//     if (SQLITE_OK != result) {
//         VR_LOGE("Run SQL request [%s] failed, error code: [%d], error msg: [%s]",
//             sqlRequest.c_str(),
//             result,
//             errmsg);
//         return;
//     }

//     VoiceMap<std::string, std::string>::type tableFormatMap;
//     tableFormatMap.insert(
//         std::pair<std::string, std::string>(
//             VR_DB_TABLENAME_CONTACT,
//             "id integer,first varchar(255),last varchar(255),full varchar(255)"));
//     tableFormatMap.insert(
//         std::pair<std::string, std::string>(
//             VR_DB_TABLENAME_PHONERECORDS,
//             "id integer,typeid integer,number varchar(255)"));
//     tableFormatMap.insert(
//         std::pair<std::string, std::string>(
//             VR_DB_TABLENAME_AUDIOSOURCE,
//             "id integer,name varchar(255),grammarid varchar(255)"));
//     // tableFormatMap.insert(std::pair<std::string, std::string>(VR_DB_TABLENAME_PHONETYPE, "typeid integer,typename varchar(255)"));
//     // tableFormatMap.insert(std::pair<std::string, std::string>(VR_DB_TABLENAME_QUICKREPLYMESSAGE, "id integer,name varchar(255)"));
//     // tableFormatMap.insert(std::pair<std::string, std::string>(VR_DB_TABLENAME_MESSAGETYPE, "typeid integer,typename varchar(255)"));
//     // tableFormatMap.insert(std::pair<std::string, std::string>(VR_DB_TABLENAME_DATA, "id integer,name varchar(255),shortcut varchar(255)")); // navi agent
//     // tableFormatMap.insert(std::pair<std::string, std::string>(VR_DB_TABLENAME_SEARCHAPP, "id integer,name varchar(255)"));
//     // tableFormatMap.insert(std::pair<std::string, std::string>(VR_DB_TABLENAME_FMGENRE, "id integer,name varchar(255)"));
//     // tableFormatMap.insert(std::pair<std::string, std::string>(VR_DB_TABLENAME_SATCHANNELNAME, "id integer,name varchar(255)"));
//     // tableFormatMap.insert(std::pair<std::string, std::string>(VR_DB_TABLENAME_SATCHANNELNUMBER, "id integer,name integer"));
//     // tableFormatMap.insert(std::pair<std::string, std::string>(VR_DB_TABLENAME_SATGENRE, "id integer,name varchar(255)"));
//     // tableFormatMap.insert(std::pair<std::string, std::string>(VR_DB_TABLENAME_HDSUBCHANNEL, "id integer,name varchar(255),intval integer"));
//     // tableFormatMap.insert(std::pair<std::string, std::string>(VR_DB_TABLENAME_APPS, "id integer,name varchar(255),shortcut varchar(255)"));
//     // tableFormatMap.insert(std::pair<std::string, std::string>(VR_DB_TABLENAME_APPSOFFBOARD, "id integer,name varchar(255),shortcut varchar(255)"));

//     std::size_t size = tableNameList.size();
//     for (VoiceMap<std::string, std::string>::iterator it = tableFormatMap.begin();
//         it != tableFormatMap.end();
//         ++it) {
//         tableNameList.remove(it->first);
//         if (size != tableNameList.size()) {
//             size = tableNameList.size();
//         }
//         else {
//             sqlRequest.assign("CREATE TABLE " + it->first + "(" + it->second + ")");
//             result = sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, &errmsg);
//             if (SQLITE_OK != result) {
//                 VR_LOGE("Run SQL request [%s] failed, error code: [%d], error msg: [%s]",
//                     sqlRequest.c_str(), result, errmsg);
//                 return;
//             }
//         }
//     }
// }

int VR_DataAccessorManager::getSingleColumnList(
    void *stringList,
    int columnNum,
    char **columnValue,
    char **columnName)
{
    // check column number count available, otherwise continue to operate next result
    if (SQLITE_CALLBACK_COLUMN_NUMBER_ZERO == columnNum) {
        return 0;
    }
    if (columnValue[0]) {
        ((VoiceList<std::string>::type*)stringList)->push_back(columnValue[0]);
    }
    return 0;
}

int VR_DataAccessorManager::getSingleColumnValue(
    void *stringValue,
    int columnNum,
    char **columnValue,
    char **columnName)
{
    // check column number count available, otherwise continue to operate next result
    if (SQLITE_CALLBACK_COLUMN_NUMBER_ZERO == columnNum) {
        return 0;
    }
    if (columnValue[0]) {
        *((std::string *)stringValue) = columnValue[0];
    }
    return 0;
}

int VR_DataAccessorManager::genContactCTermListCallback(
    void *addList,
    int columnNum,
    char **columnValue,
    char **columnName)
{
    // check column number count available, otherwise continue to operate next result
    if (columnNum < SQLITE_CALLBACK_COLUMN_NUMBER_GEN_CTERM) {
        return 0;
    }
    if (nullptr == columnValue[0] || nullptr == columnValue[1]) {
        return 0;
    }

    std::string contactName = columnValue[1];

    VoiceList<std::string>::type itemList;

    if (getContactNameList(contactName, itemList)) {

       ((VoiceList<spC_Term>::type*)addList)->push_back(
           VR_DataSynchronizer::getCTermByNameList(
               atoi(columnValue[0]),
               itemList));
    }
    return 0;
}

int VR_DataAccessorManager::genMusicCTermListCallback(
    void *addList,
    int columnNum,
    char **columnValue,
    char **columnName)
{
    // check column number count available, otherwise continue to operate next result
    if (columnNum < SQLITE_CALLBACK_COLUMN_NUMBER_GEN_MUSIC_CTERM) {
        return 0;
    }
    if (nullptr == columnValue[0] || nullptr == columnValue[1]) {
        return 0;
    }

    int id = atoi(columnValue[0]);
    std::string itemName(columnValue[1]);

    generateMusicCTermList(*static_cast<VoiceList<spC_Term>::type*>(addList), id, itemName);
    return 0;
}

// list include alias and formal name of music item
bool VR_DataAccessorManager::getMusicItemList(const std::string &itemName, VoiceList<std::string>::type &list)
{
    if (itemName.empty()) {
        return false;
    }

    // remove paired parentheses and square brackets
    string result = itemName;
    int number = m_bracketsRule.GlobalReplace("", &result);
    m_specialCharRule.GlobalReplace(" ", &result);
    m_consecutiveSpaceRule.GlobalReplace(" ", &result);
    result = trim(result);
    if (!result.empty() && 0 != number) {
        list.push_back(result);
        VR_LOGD("Media Alias: %s", result.c_str());
    }

    // process Ft. ft. Feat. feat. and Featuring featuring
    result = itemName;
    m_specialCharRule.GlobalReplace(" ", &result);
    m_consecutiveSpaceRule.GlobalReplace(" ", &result);
    pcrecpp::StringPiece input(result);
    std::string alias;
    while (m_featuringRule.Consume(&input, &alias)) {
        list.push_back(trim(alias));
        VR_LOGD("Media Alias: %s", alias.c_str());
    }
    if (input.size() > 0 && (unsigned)input.size() != result.size()) {
        list.push_back(trim(std::string(input.data())));
        VR_LOGD("Media Alias: %s", input.data());
    }

    list.push_front(trim(result));
    VR_LOGD("Formal Name: %s", result.c_str());
    return true;
}

void VR_DataAccessorManager::generateMusicCTermList(VoiceList<spC_Term>::type &cTermList, const unsigned int &id, const std::string &name)
{
    // get music alias name
    VoiceList<std::string>::type itemList;
    if (getMusicItemList(name, itemList)) {
        cTermList.push_back(
            VR_DataSynchronizer::getCTermByNameList(
                id,
                itemList)
        );
        }
}

bool VR_DataAccessorManager::getContactNameList(const std::string &itemName, VoiceList<std::string>::type &list)
{
    if (itemName.empty()) {
        return false;
    }
    list.push_back(itemName);
    std::string tmpName = itemName;

    // replace special char to space
    m_specialCharRule.GlobalReplace(" ", &tmpName);
    m_consecutiveSpaceRule.GlobalReplace(" ", &tmpName);
    tmpName = trim(tmpName);

    if (itemName != tmpName) {
        list.push_back(tmpName);
    }
    return true;
}

void VR_DataAccessorManager::getMusicCTermList(VoiceList<spC_Term>::type &cTermList, const unsigned int &id, const std::string &name, const std::string &pronIDStr, const std::string &pronNameStr)
{
    generateMusicCTermList(cTermList, id, name);
}

// void VR_DataAccessorManager::insertRecord(
//     const std::string &tableName,
//     const std::string &value1,
//     const std::string &value2,
//     const std::string &extendValue)
// {
//     std::string sqlRequest(std::string("INSERT INTO ") + tableName
//         + " VALUES (\"" + value1 + "\", \"" + value2 + "\""
//         + (extendValue.empty() ? "" : (", \"" + extendValue + "\""))
//         + ")");
//     sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);
// }

void VR_DataAccessorManager::notifyStateUpdated()
{
    if (m_updateStateCallback) {
        m_updateStateCallback("");
    }
}

void VR_DataAccessorManager::checkMusicGrammarState()
{
    if (!m_requestUpdateMediaGrammarFinish) {
        return;
    }

    if (VR_DICTIONARY_STATE_SYNCING == m_updatingDictionaryStateMap["music"]
        && VR_DICTIONARY_STATE_SYNCING != m_updatingDictionaryStateMap["genre"]
        && VR_DICTIONARY_STATE_SYNCING != m_updatingDictionaryStateMap["artist"]
        && VR_DICTIONARY_STATE_SYNCING != m_updatingDictionaryStateMap["album"]
        && VR_DICTIONARY_STATE_SYNCING != m_updatingDictionaryStateMap["playlist"]
        && VR_DICTIONARY_STATE_SYNCING != m_updatingDictionaryStateMap["song"]) {
        m_updatingDictionaryStateMap["music"] = VR_DICTIONARY_STATE_OK;

        std::string errcode;
        if (m_isMusicGrammarDroped) {
            errcode = "1";
            m_isMusicGrammarDroped = false;
        }
        else {
            errcode = "0";
        }
        std::string msgToDM(std::string("<grammar_result op=\"grammar\" agent=\"" VR_AGENT_MEDIA "\" grammarid=\"")
            + getState(VR_GLOBALSTATE_UPDATING_SOURCE_ID)
            + "\" errcode=\""
            + errcode
            + "\" />");
        if (m_notifyCallback) {
            m_notifyCallback(msgToDM);
        }
        setState(VR_GLOBALSTATE_UPDATING_SOURCE_ID, VR_GLOBALSTATE_VALUE_ZERO);
        VR_LOGI("GlobalState [" VR_GLOBALSTATE_UPDATING_SOURCE_ID "] updated [%s]", getState(VR_GLOBALSTATE_UPDATING_SOURCE_ID).c_str());
        VR_LOGP("case:212-9-780 212-10-780 212-11-780 212-12-780 212-13-780 212-14-780 212-15-780 DataAccessor generate music grammar end");
    }
}

void VR_DataAccessorManager::initializeState()
{
    m_stateMap.clear();
    pugi::xml_document msgDoc;
    std::string globalStatePath;
    bool isGetData = m_deCommonIF->isGetFromData();
    if (isGetData) {
        globalStatePath = m_configure->getDataPath() + "settings/globalState.xml";
    }
    else {
        globalStatePath = "/system/etc/voicerecog/settings/globalState.xml";
    }
    msgDoc.load_file(globalStatePath.c_str());
    pugi::xpath_node_set nodes = msgDoc.select_nodes("//item");

    for (pugi::xpath_node_set::iterator it = nodes.begin(); it != nodes.end(); ++it) {
        setState(it->node().attribute("key").as_string(), it->node().attribute("value").as_string());
    }

    setState(VR_GLOBALSTATE_UPDATING_SOURCE_ID, VR_GLOBALSTATE_VALUE_ZERO);
    setState(VR_GLOBALSTATE_ACTIVED_SOURCE_ID, VR_GLOBALSTATE_VALUE_ZERO);
    // should add in glabalState.xml
    setState(VR_GLOBALSTATE_PHONE_DEVICE_ID, VR_GLOBALSTATE_VALUE_ZERO);

    m_updatingDictionaryStateMap["music"] = VR_DICTIONARY_STATE_NONE;
    m_updatingDictionaryStateMap["album"] = VR_DICTIONARY_STATE_NONE;
    m_updatingDictionaryStateMap["artist"] = VR_DICTIONARY_STATE_NONE;
    m_updatingDictionaryStateMap["playlist"] = VR_DICTIONARY_STATE_NONE;
    m_updatingDictionaryStateMap["song"] = VR_DICTIONARY_STATE_NONE;
    m_updatingDictionaryStateMap["genre"] = VR_DICTIONARY_STATE_NONE;
}

void VR_DataAccessorManager::updateMusicGrammarState(const std::string &categoryName, const std::string &grammarID)
{
    std::string globalUpdatingSourceID(getState(VR_GLOBALSTATE_UPDATING_SOURCE_ID));
    if (grammarID != globalUpdatingSourceID) {
        globalUpdatingSourceID = grammarID;
        setState(VR_GLOBALSTATE_UPDATING_SOURCE_ID, globalUpdatingSourceID);
        VR_LOGI("GlobalState [" VR_GLOBALSTATE_UPDATING_SOURCE_ID "] updated [%s]", globalUpdatingSourceID.c_str());
    }

    if (VR_DICTIONARY_STATE_SYNCING != m_updatingDictionaryStateMap["music"]) {
        m_updatingDictionaryStateMap["music"] = VR_DICTIONARY_STATE_SYNCING;
    }
    m_updatingDictionaryStateMap[categoryName] = VR_DICTIONARY_STATE_SYNCING;

    std::string globalActivedSourceID(getState(VR_GLOBALSTATE_ACTIVED_SOURCE_ID));
    if (globalActivedSourceID != globalUpdatingSourceID) {
        VR_LOGD("updating sourceID not match actived sourceID");
        return;
    }

    std::string key = categoryName;
    std::string globalMuiscDicState(getState(VR_GLOBALSTATE_MUSIC_DICTIONARY_STATE));
    if (VR_DICTIONARY_STATE_SYNCING != globalMuiscDicState) {
        setState(VR_GLOBALSTATE_MUSIC_DICTIONARY_STATE, VR_DICTIONARY_STATE_SYNCING);
        VR_LOGI("GlobalState [" VR_GLOBALSTATE_MUSIC_DICTIONARY_STATE "] updated [" VR_DICTIONARY_STATE_SYNCING "]");
    }
    std::transform(key.begin(), key.end(), key.begin(), toupper);
    key += VR_GLOBALSTATE_DICTIONARY_STATE_SUFFIX;
    setState(key, VR_DICTIONARY_STATE_SYNCING);
    VR_LOGI("GlobalState [%s] updated [" VR_DICTIONARY_STATE_SYNCING "]", key.c_str());

    notifyStateUpdated();
}

void VR_DataAccessorManager::updateOtherGrammarState(const std::string &categoryName)
{
    std::string key = categoryName;
    if (VR_GRAMMAR_CATEGORY_CONTACT == key) {
        key = "PHONEBOOK";
    }
    key += VR_GLOBALSTATE_DICTIONARY_STATE_SUFFIX;
    setState(key, VR_DICTIONARY_STATE_SYNCING);
    VR_LOGI("GlobalState [%s] updated [" VR_DICTIONARY_STATE_SYNCING "]", key.c_str());

    notifyStateUpdated();
}

void VR_DataAccessorManager::getFullName(std::string &fullName, const std::string firstName, const std::string lastName, bool isNormal)
{
    if (isNormal) {
        fullName.assign(firstName
            + ((firstName.empty() || lastName.empty()) ? "" : " ")
            + lastName);
    }
    else {
        fullName.assign(lastName + firstName);
    }
}

std::string VR_DataAccessorManager::trim(std::string str)
{
    if (str.empty()) {
        return str;
    }
    str.erase(0, str.find_first_not_of(" "));
    size_t foundPos = str.find_last_not_of(" ");
    if (foundPos != std::string::npos) {
        str.erase(str.find_last_not_of(" ") + 1);
    }
    else {
        str.clear();
    }

    return str;
}

const std::string & VR_DataAccessorManager::getState(const std::string &stateKey)
{
    if (m_stateMap.end() != m_stateMap.find(stateKey)) {
        return m_stateMap.at(stateKey);
    }
    else {
        return VR_GLOBALSTATE_VALUE_EMPTY;
    }
}

void VR_DataAccessorManager::setState(const std::string &stateKey, const std::string &stateValue)
{
    m_stateMap[stateKey] = stateValue;
}

void VR_DataAccessorManager::checkGrammarNeedUpdate()
{
    VR_LOGD_FUNC();
    std::string globalActivedSourceID(getState(VR_GLOBALSTATE_ACTIVED_SOURCE_ID));

    bool grammarNeedUpdate = false;
    VoiceList<pugi::sp_xml_document>::iterator it = m_generateGrammarList.begin();
    while (m_generateGrammarList.end() != it) {
        if (globalActivedSourceID == (*it)->first_child().attribute(VR_GRAMMAR_ID).as_string()
            && (std::string(VR_AGENT_MEDIA) == (*it)->first_child().attribute(VR_GRAMMAR_AGENT).as_string())) {
            grammarNeedUpdate = true;
            break;
        }
        ++it;
    }

    if (grammarNeedUpdate) {
        setState(VR_GLOBALSTATE_GRAMMAR_UPDATE_STATUS, VR_GLOBALSTATE_GRAMMAR_UPDATE_STATUS_NEED_UPDATE);
        m_accessorMedia->setGrammarUpdateStatus(VR_GLOBALSTATE_GRAMMAR_UPDATE_STATUS_NEED_UPDATE);
        VR_LOGD("Grammar Update Status: needUpdate");
    }
    else {
        setState(VR_GLOBALSTATE_GRAMMAR_UPDATE_STATUS, VR_GLOBALSTATE_GRAMMAR_UPDATE_STATUS_NO_CHANGE);
        m_accessorMedia->setGrammarUpdateStatus(VR_GLOBALSTATE_GRAMMAR_UPDATE_STATUS_NO_CHANGE);
        VR_LOGD("Grammar Update Status: noChange");
    }
}

void VR_DataAccessorManager::resetGrammarGeneration()
{
    m_pDataSynchronizer->resetRequest();
    m_generatingGrammarMsg.reset();
    m_generateGrammarList.clear();

    VoiceQueue<pugi::sp_xml_document>::type tempQueue;
    std::swap(m_grammarMsgQuque, tempQueue);

    if (VR_DECommonIF::DE_Country::vt == m_deCommonIF->getCountryType()) {
        VR_LOGI("voicetag region do not need to set grammar state");
        return;
    }

    // reset grammar state
    for (VoiceMap<std::string, std::string>::iterator it = m_categoryContextIDMap.begin();
        it != m_categoryContextIDMap.end();
        ++it) {
        std::string key = it->first;
        if (VR_GRAMMAR_CATEGORY_CONTACT == key) {
            key = "PHONEBOOK";
        }
        std::transform(key.begin(), key.end(), key.begin(), toupper);
        key += VR_GLOBALSTATE_DICTIONARY_STATE_SUFFIX;
        setState(key, VR_DICTIONARY_STATE_NONE);
        VR_LOGI("GlobalState [%s] updated [" VR_DICTIONARY_STATE_NONE "]", key.c_str());
    }

    setState(VR_GLOBALSTATE_MUSIC_DICTIONARY_STATE, VR_DICTIONARY_STATE_NONE);
    setState(VR_GLOBALSTATE_UPDATING_SOURCE_ID, VR_GLOBALSTATE_VALUE_ZERO);

    m_updatingDictionaryStateMap["music"] = VR_DICTIONARY_STATE_NONE;
    m_updatingDictionaryStateMap["album"] = VR_DICTIONARY_STATE_NONE;
    m_updatingDictionaryStateMap["artist"] = VR_DICTIONARY_STATE_NONE;
    m_updatingDictionaryStateMap["playlist"] = VR_DICTIONARY_STATE_NONE;
    m_updatingDictionaryStateMap["song"] = VR_DICTIONARY_STATE_NONE;
    m_updatingDictionaryStateMap["genre"] = VR_DICTIONARY_STATE_NONE;
}

void VR_DataAccessorManager::restartGenerateGrammar()
{
    m_pDataSynchronizer->resetRequest();
    if (!m_generatingGrammarMsg) {
        VR_LOGD("no grammar is generating");
        return;
    }
    m_generateGrammarList.push_front(m_generatingGrammarMsg);
    m_generatingGrammarMsg.reset();
}

bool VR_DataAccessorManager::checkContextIDAvailable(const std::string &contextID)
{
    VoiceMap<std::string, std::string>::iterator it = m_categoryContextIDMap.begin();
    while (it != m_categoryContextIDMap.end()) {
        if (0 == contextID.compare(0, it->second.size(), it->second)) {
            return true;
        }
        ++it;
    }
    return false;
}

std::string VR_DataAccessorManager::getContextIDAgent(const std::string &contextID)
{
    size_t firstPos = contextID.find_first_of('_');
    size_t secondPos = contextID.find_first_of('_', firstPos + 1);
    std::string agent = contextID.substr(firstPos + 1, secondPos - firstPos - 1);
    if ("voice" == agent) {
        agent = "voicetag";
    }
    return agent;
}

std::string VR_DataAccessorManager::getContextIDIndex(const std::string &contextID)
{
    size_t pos = contextID.find_last_of('_');
    std::string index = contextID.substr(pos + 1);
    return index;
}

void VR_DataAccessorManager::setGenerateGrammarEnable(bool isEnable)
{
    m_generateGrammarEnable = isEnable;
}

bool VR_DataAccessorManager::generateVoiceTagGrammar(std::string &deviceAddress)
{
    if (m_voiceTagContextID.empty()) {
        return false;
    }

    VoiceList<spC_Term>::type addList;
    VoiceList<VR_VoiceTagInfo>::type voiceTagInfoList;
    getVoiceTagInfo(deviceAddress, voiceTagInfoList);

    VoiceList<VR_VoiceTagInfo>::iterator it = voiceTagInfoList.begin();
    while (it != voiceTagInfoList.end()) {
        addList.push_back(VR_DataSynchronizer::getCTerm(atoi(it->voiceTagID.c_str()), it->name, it->phoneme));
        ++it;
    }

    return m_pDataSynchronizer->updateGrammar(
        m_voiceTagContextID + deviceAddress,
        VR_GRAMMAR_TYPE_DEFLAUT,
        addList,
        boost::bind(&VR_DataAccessorManager::voiceTagUpdateGrammarCallback,
            this,
            _1,
            deviceAddress));
}

void VR_DataAccessorManager::removeGrammarDiffMsg(const std::string &agent, const std::string &grammarID)
{
    VoiceList<pugi::sp_xml_document>::iterator it = m_generateGrammarList.begin();
    while (m_generateGrammarList.end() != it) {
        if (std::string(VR_GRAMMAR_MSG_DIFF) != (*it)->first_child().name()) {
            ++it;
            continue;
        }

        std::string currentAgent = (*it)->first_child().attribute(VR_GRAMMAR_AGENT).as_string();
        std::string currentGrammarID = (*it)->first_child().attribute(VR_GRAMMAR_ID).as_string();
        if (currentAgent == agent && currentGrammarID == grammarID) {
            it = m_generateGrammarList.erase(it);
            continue;
        }
        ++it;
    }
}

void VR_DataAccessorManager::handleMediaNullDevice()
{
    VR_LOGD_FUNC();
    std::string globalActivedSourceID = getState(VR_GLOBALSTATE_ACTIVED_SOURCE_ID);
    if (globalActivedSourceID != VR_GLOBALSTATE_VALUE_ZERO) {
        return;
    }

    std::string globalMusicConnected(getState(VR_GLOBALSTATE_MUSIC_DEVICE_CONNECTED));

    if (VR_GLOBALSTATE_VALUE_TRUE == globalMusicConnected) {
        setState(VR_GLOBALSTATE_MUSIC_DICTIONARY_STATE, VR_DICTIONARY_STATE_OK);
        setState(VR_GLOBALSTATE_ARTIST_DICTIONARY_STATE, VR_DICTIONARY_STATE_OK);
        setState(VR_GLOBALSTATE_ALBUM_DICTIONARY_STATE, VR_DICTIONARY_STATE_OK);
        setState(VR_GLOBALSTATE_PLAYLIST_DICTIONARY_STATE, VR_DICTIONARY_STATE_OK);
        setState(VR_GLOBALSTATE_SONG_DICTIONARY_STATE, VR_DICTIONARY_STATE_OK);
        setState(VR_GLOBALSTATE_GENRE_DICTIONARY_STATE, VR_DICTIONARY_STATE_OK);
    }
    else {
        setState(VR_GLOBALSTATE_MUSIC_DICTIONARY_STATE, VR_DICTIONARY_STATE_NONE);
        setState(VR_GLOBALSTATE_ARTIST_DICTIONARY_STATE, VR_DICTIONARY_STATE_NONE);
        setState(VR_GLOBALSTATE_ALBUM_DICTIONARY_STATE, VR_DICTIONARY_STATE_NONE);
        setState(VR_GLOBALSTATE_PLAYLIST_DICTIONARY_STATE, VR_DICTIONARY_STATE_NONE);
        setState(VR_GLOBALSTATE_SONG_DICTIONARY_STATE, VR_DICTIONARY_STATE_NONE);
        setState(VR_GLOBALSTATE_GENRE_DICTIONARY_STATE, VR_DICTIONARY_STATE_NONE);
    }
}

void VR_DataAccessorManager::generateDisplayGrammar(const VoiceList<std::string>::type &itemList)
{
    VR_LOGD_FUNC();
    unsigned int id = 1;
    VoiceList<spC_Term>::type addList;
    VoiceList<std::string>::const_iterator it = itemList.begin();
    while (it != itemList.end()) {
        addList.push_back(VR_DataSynchronizer::getCTerm(id, *it));
        ++id;
        ++it;
    }

    N_Vr::N_Asr::C_Request_Context_List_Update updateMsg;
    updateMsg.m_string_Id_Context.assign("ctx_common_list_dynamic_list_1");
    updateMsg.m_list_spo_Term_For_Add.swap(addList);
    updateMsg.m_string_Id_Category = VR_GRAMMAR_TYPE_NORMAL;
    updateMsg.m_b_Clear = true;
    updateMsg.m_i_Priority_Id = 11;
    updateMsg.m_function_On_Event_Phase = boost::bind(&VR_DataAccessorManager::generateDisplayGrammarCallback, this, _1);
    requestUpdateGrammar(updateMsg);
}

void VR_DataAccessorManager::generateDisplayGrammarCallback(const N_Vr::N_Asr::C_Event_Phase &phaseEvent)
{
    VR_LOGD_FUNC();
    N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type msgType = phaseEvent.Get_Event_Phase_Type();
    if (N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Proc_End == msgType) {
        m_pDataSynchronizer->updateGrammarCategoryFinish("displayGrammar");
    }
}

/* EOF */
