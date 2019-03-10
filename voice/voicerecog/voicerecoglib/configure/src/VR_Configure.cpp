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

#include <fstream>
#include "stdafx.h"
#include "VR_Configure.h"
#include "VR_Log.h"
#include "BL_String.h"
#include "BL_RWFile.h"
#include "BL_File.h"
#include "BL_FullFile.h"
#include "BL_Dir.h"
#include "VR_Def.h"

#include "VR_ContentProvider.h"
#include "ncore/NCString.h"
#include "system/NCEnvironment.h"
#include "BL_SystemLanguage_DEF.h"
#include <boost/assign/list_of.hpp>
#include <cutils/properties.h>
#include <stdlib.h>

#include "navi/Voice/VoiceRecog/Internal/VrAgentActivation.pb.h"

#define VR_SUNTEC_PATH_NAME "/SUNTEC/"
#define VR_VBT_PATH_NAME "/VBT/"
#define VR_IFLYTEK_PATH_NAME "/IFLYTEK/SRRes/"
#define VR_ASR_PATH_NAME "/ASR/"
#define VR_TUTORIAL_PATH_NAME "/TUTORIAL/"

static nutshell::NCSyncObj g_cSyncObj;

static const VoiceMap<std::string, int>::type vrLanguages = boost::assign::map_list_of
        (VR_LANGUAGE_ZH_CN, BL_LANGUAGE_TYPE_SIMPLIFIED_CHINESE)
        (VR_LANGUAGE_ZH_HK, BL_LANGUAGE_TYPE_TRADITIONAL_CHINESE)
        (VR_LANGUAGE_EN_US, BL_LANGUAGE_TYPE_ENGLISH)
        (VR_LANGUAGE_EN_GB, BL_LANGUAGE_TYPE_UKENGLISH)
        (VR_LANGUAGE_TH, BL_LANGUAGE_TYPE_THAI);

#define TTS_LANG_MAX_NUM 37
static const VR_String  vrTTSLanguageMap[TTS_LANG_MAX_NUM][3] =
      { { "en-us", "ENU", "" },
        { "fr-ca", "FRC", "" },
        { "es-mx", "SPM", "" },
        { "en-au", "ENA", "" },
        { "zh-cn", "MNC", "" },
        { "zh-tw", "MNT", "" },
        { "zh-hk", "CAH", "" },
        { "ko-kr", "KOK", "" },
        { "pl",    "PLP", "" },
        { "fr",    "FRF", "" },
        { "nl",    "DUN", "" },
        { "nl-be", "DUB", "" },
        { "en-gb", "ENG", "" },
        { "en-id", "ENI", "" },
        { "id",    "IDI", "" },
        { "el",    "GRG", "" },
        { "fi",    "FIF", "" },
        { "hi",    "HII", "" },
        { "sv",    "SWS", "" },
        { "he",    "HEI", "" },
        { "da",    "DAD", "" },
        { "no",    "NON", "" },
        { "sk",    "SKS", "" },
        { "ar-ml", "ARW", "male" },
        { "ar-fm", "ARW", "female" },
        { "it",    "ITI", "" },
        { "hu",    "HUH", "" },
        { "pt-br", "PTB", "" },
        { "tr",    "TRT", "" },
        { "th",    "THT", "" },
        { "de",    "GED", "" },
        { "ru",    "RUR", "" },
        { "cs",    "CZC", "" },
        { "ro",    "ROR", "" },
        { "es",    "SPE", "" },
        { "pt",    "PTP", "" },
        { "en-me", "ENG", "" } };


// VR_UpdateStatePacket

VR_UpdateStatePacket::VR_UpdateStatePacket()
{
    // phone state
    m_mapUpdateState.insert(std::make_pair(VR_PHONE_STATE_CONNECTED, "disconnected"));
    m_mapUpdateState.insert(std::make_pair(VR_PHONE_DEVICE_ID, "0"));
    m_mapUpdateState.insert(std::make_pair(VR_PHONE_MAPPROTOCOL, ""));
    m_mapUpdateState.insert(std::make_pair(VR_PHONE_GETMESSAGESTATE, ""));
    m_mapUpdateState.insert(std::make_pair(VR_PHONE_STATE_REGISTERED, "0"));
    m_mapUpdateState.insert(std::make_pair(VR_STEERING_PHONEKEY_TYPE, "TEL"));
    m_mapUpdateState.insert(std::make_pair(VR_BTAUDIO_CONNECTED, "false"));

    // music state
    m_mapUpdateState.insert(std::make_pair(VR_USB_1_CONNECTED, "false"));
    m_mapUpdateState.insert(std::make_pair(VR_USB_2_CONNECTED, "false"));
    m_mapUpdateState.insert(std::make_pair(VR_IPOD_1_CONNECTED, "false"));
    m_mapUpdateState.insert(std::make_pair(VR_IPOD_2_CONNECTED, "false"));
    m_mapUpdateState.insert(std::make_pair(VR_MUSIC_SONG_INFORMATION_AVAILABLE, "false"));
    m_mapUpdateState.insert(std::make_pair(VR_MEDIAINFOKEY_MAGICSTRING, ""));

    // radio state
    m_mapUpdateState.insert(std::make_pair(VR_RADIO_STATE_FM_ACTIVE, "false"));
    m_mapUpdateState.insert(std::make_pair(VR_RADIO_STATE_AM_ACTIVE, "false"));
    m_mapUpdateState.insert(std::make_pair(VR_RADIO_XM_CONNECTED, "false"));
    m_mapUpdateState.insert(std::make_pair(VR_RADIO_STATE_FM_HD_ACTIVE, "false"));
    m_mapUpdateState.insert(std::make_pair(VR_RADIO_STATE_XM_ACTIVE, "false"));

    // climate state
    m_mapUpdateState.insert(std::make_pair(VR_CLIMATE_FANSPEED_MAX, "5"));
    m_mapUpdateState.insert(std::make_pair(VR_CLIMATE_TEMPERATURE_MIN, "15"));
    m_mapUpdateState.insert(std::make_pair(VR_CLIMATE_TEMPERATURE_MAX, "32"));
    m_mapUpdateState.insert(std::make_pair(VR_CLIMATE_BASIC_ACTIVE, "false"));
    m_mapUpdateState.insert(std::make_pair(VR_CLIMATE_FRONT_SCREEN_ACTIVE, "false"));
    m_mapUpdateState.insert(std::make_pair(VR_CLIMATE_REAR_SCREEN_ACTIVE, "false"));
    m_mapUpdateState.insert(std::make_pair(VR_CLIMATE_STEERING_SCREEN_ACTIVE, "false"));
    m_mapUpdateState.insert(std::make_pair(VR_CLIMATE_LEXUS_CONCIERGE_SCREEN_ACTIVE, "false"));
    m_mapUpdateState.insert(std::make_pair(VR_CLIMATE_CONCIERGE_ACTIVE, "false"));
    m_mapUpdateState.insert(std::make_pair(VR_CLIMATE_FRONT_SEAT_SCREEN_ACTIVE, "false"));
    m_mapUpdateState.insert(std::make_pair(VR_CLIMATE_REAR_SEAT_SCREEN_ACTIVE, "false"));
    m_mapUpdateState.insert(std::make_pair(VR_CLIMATE_SEAT_OPERATION_SCREEN_ACTIVE, "false"));

    // apps state
    m_mapUpdateState.insert(std::make_pair(VR_TSL_STATE_AVAILABLE, "false"));
    m_mapUpdateState.insert(std::make_pair(VR_TSL_APPS_AVAILABLE, "false"));
    m_mapUpdateState.insert(std::make_pair(VR_TSL_APPLIST, ""));

    // voicetraining state
    m_mapUpdateState.insert(std::make_pair(VR_OPERESTRICT, "off"));
}

VR_UpdateStatePacket::~VR_UpdateStatePacket()
{
    m_mapUpdateState.clear();
}

void   VR_UpdateStatePacket::SetUpdateState(const std::string& strStateKey, const std::string& strStateValue)
{
    VR_LOGD("------------strStateKey:%s---strStateValue <%s>------------", strStateKey.c_str(), strStateValue.c_str());
    auto iter = m_mapUpdateState.find(strStateKey);
    if (m_mapUpdateState.end() != iter) {
        iter->second = strStateValue;
    }
    else {
        VR_LOGD("Invalid VR_UpdateStatePacket strStateKey: [%s]", strStateKey.c_str());
    }
}

void   VR_UpdateStatePacket::GetStateValueByStateKey(const std::string& strStateKey, std::string& strStateValue) const
{
    strStateValue.clear();
    auto iter = m_mapUpdateState.find(strStateKey);
    if (m_mapUpdateState.end() != iter) {
        strStateValue = iter->second;
    }
    else {
        VR_LOGD("Invalid VR_UpdateStatePacket strStateKey: [%s]", strStateKey.c_str());
    }
}

VR_UpdateGrammarPacket::VR_UpdateGrammarPacket()
{
    m_mapUpdateGrammar.insert(std::make_pair(VR_PHONE_GRAMMAR_DISACTIVE, ""));
    m_mapUpdateGrammar.insert(std::make_pair(VR_PHONE_GRAMMAR_INIT, ""));
    m_mapUpdateGrammar.insert(std::make_pair(VR_PHONE_GRAMMAR_ACTIVE, ""));
    m_mapUpdateGrammar.insert(std::make_pair(VR_PHONE_GRAMMAR_NEW_QUICKREPLAYMESSAGE, ""));
    m_mapUpdateGrammar.insert(std::make_pair(VR_PHONE_GRAMMAR_NEW_PHONETYPE, ""));
    m_mapUpdateGrammar.insert(std::make_pair(VR_PHONE_GRAMMAR_NEW_MESSAGETYPE, ""));
    m_mapUpdateGrammar.insert(std::make_pair(VR_MUSIC_GRAMMAR_NEW_AUDIOSOURCE, ""));
    m_mapUpdateGrammar.insert(std::make_pair(VR_MUSIC_GRAMMAR_DISACTIVE, ""));
    m_mapUpdateGrammar.insert(std::make_pair(VR_MUSIC_GRAMMAR_INIT_1, ""));
    m_mapUpdateGrammar.insert(std::make_pair(VR_MUSIC_GRAMMAR_INIT_2, ""));
    m_mapUpdateGrammar.insert(std::make_pair(VR_MUSIC_GRAMMAR_DIFF_1, ""));
    m_mapUpdateGrammar.insert(std::make_pair(VR_MUSIC_GRAMMAR_DIFF_2, ""));
    m_mapUpdateGrammar.insert(std::make_pair(VR_MUSIC_GRAMMAR_ACTIVE, ""));
    m_mapUpdateGrammar.insert(std::make_pair(VR_RADIO_GRAMMAR_NEW_FMGENRE, ""));
    m_mapUpdateGrammar.insert(std::make_pair(VR_RADIO_GRAMMAR_NEW_SATCHANNEL, ""));
    m_mapUpdateGrammar.insert(std::make_pair(VR_RADIO_GRAMMAR_NEW_SATGENRE, ""));
}

VR_UpdateGrammarPacket::~VR_UpdateGrammarPacket()
{
    m_mapUpdateGrammar.clear();
}

void   VR_UpdateGrammarPacket::SetUpdateGrammar(const std::string& strGrammarKey, const std::string& strGrammarValue)
{
    VR_LOGD("------------strGrammarKey:%s---strGrammarValue <%s>------------", strGrammarKey.c_str(), strGrammarValue.c_str());
    auto iter = m_mapUpdateGrammar.find(strGrammarKey);
    if (m_mapUpdateGrammar.end() != iter) {
        iter->second = strGrammarValue;
    }
    else {
        VR_LOGD("Invalid VR_UpdateGrammarPacket strGrammarKey: [%s]", strGrammarKey.c_str());
    }

}

void   VR_UpdateGrammarPacket::GetGrammarValueByGrammarKey(const std::string& strGrammarKey, std::string& strGrammarVule) const
{
    strGrammarVule.clear();
    auto iter = m_mapUpdateGrammar.find(strGrammarKey);
    if (m_mapUpdateGrammar.end() != iter) {
        strGrammarVule = iter->second;
    }
    else {
        VR_LOGD("Invalid VR_UpdateGrammarPacket strGrammarKey: [%s]", strGrammarKey.c_str());
    }

}

// VR_AgentStatus

VR_AgentStatus::VR_AgentStatus()
    : m_bfm_radio_broadcasting(false)
    , m_bam_radio_broadcasting(false)
    , m_bfm_hd_radio_broadcasting(false)
    , m_boptional_hd_radio_broadcasting(false)
    , m_bxm_hd_connecting(false)
    , m_bxm_radio_broadcasting(false)
    , m_btelephone_connection_state(false)
    , m_bspp_connection_state(false)
    , m_busb_slip_connection_state(false)
    , m_bmap_sd_insert_state(false)
    , m_bdataservice_availability(false)
    , m_bentune_availability(false)
    , m_bmap_connection_state(false)
    , m_bregister_pairing_state(false)
{

}

VR_AgentStatus::~VR_AgentStatus()
{

}

bool  VR_AgentStatus::GetAgentStatus(VR_AGENT_STATUS aType)  const
{
    bool   agentStatus = false;
    switch (aType) {
    case fm_radio_broadcasting :
        {
            agentStatus = m_bfm_radio_broadcasting;
            break;
        }
    case am_radio_broadcasting :
        {
            agentStatus = m_bam_radio_broadcasting;
            break;
        }
    case fm_hd_radio_broadcasting :
        {
            agentStatus = m_bfm_hd_radio_broadcasting;
            break;
        }
    case optional_hd_radio_broadcasting :
        {
            agentStatus = m_boptional_hd_radio_broadcasting;
            break;
        }
    case xm_hd_connecting :
        {
            agentStatus = m_bxm_hd_connecting;
            break;
        }
    case xm_radio_broadcasting :
        {
            agentStatus = m_bxm_radio_broadcasting;
            break;
        }
    case telephone_connection_state :
        {
            agentStatus = m_btelephone_connection_state;
            break;
        }
    case spp_connection_state :
        {
            agentStatus = m_bspp_connection_state;
            break;
        }
    case usb_slip_connection_state :
        {
            agentStatus = m_busb_slip_connection_state;
            break;
        }
    case map_sd_insert_state :
        {
            agentStatus = m_bmap_sd_insert_state;
            break;
        }
    case dataservice_availability :
        {
            agentStatus = m_bdataservice_availability;
            break;
        }
    case entune_availability :
        {
            agentStatus = m_bentune_availability;
            break;
        }
    case map_connection_state :
        {
            agentStatus = m_bmap_connection_state;
            break;
        }
    case register_pairing_state :
        {
            agentStatus = m_bregister_pairing_state;
            break;
        }
    default:
        break;
    }

    return agentStatus;
}

void  VR_AgentStatus::SetAgentStatus(VR_AGENT_STATUS aType, bool bStauts)
{
    switch (aType) {
    case fm_radio_broadcasting :
        {
            m_bfm_radio_broadcasting = bStauts;
            break;
        }
    case am_radio_broadcasting :
        {
            m_bam_radio_broadcasting = bStauts;
            break;
        }
    case fm_hd_radio_broadcasting :
        {
            m_bfm_hd_radio_broadcasting = bStauts;
            break;
        }
    case optional_hd_radio_broadcasting :
        {
            m_boptional_hd_radio_broadcasting = bStauts;
            break;
        }
    case xm_hd_connecting :
        {
            m_bxm_hd_connecting = bStauts;
            break;
        }
    case xm_radio_broadcasting :
        {
            m_bxm_radio_broadcasting = bStauts;
            break;
        }
    case telephone_connection_state :
        {
            m_btelephone_connection_state = bStauts;
            break;
        }
    case spp_connection_state :
        {
            m_bspp_connection_state = bStauts;
            break;
        }
    case usb_slip_connection_state :
        {
            m_busb_slip_connection_state = bStauts;
            break;
        }
    case map_sd_insert_state :
        {
            m_bmap_sd_insert_state = bStauts;
            break;
        }
    case dataservice_availability :
        {
            m_bdataservice_availability = bStauts;
            break;
        }
    case entune_availability :
        {
            m_bentune_availability = bStauts;
            break;
        }
    case map_connection_state :
        {
            m_bmap_connection_state = bStauts;
            break;
        }
    case register_pairing_state :
        {
            m_bregister_pairing_state = bStauts;
            break;
        }
    default:
        break;
    }
}

void VR_AgentStatus::SetAgentStatusToProto(VR_ProtoMessageBase &rMsg)
{
    VR_String strProtoTypeName = rMsg.GetTypeName();
    if ("navi.VoiceRecog.VrAgentActivationResp" != strProtoTypeName) {
        return;
    }

    navi::VoiceRecog::VrAgentActivationResp &rVrAgentAvctivationStatus = (navi::VoiceRecog::VrAgentActivationResp &)rMsg;
    rVrAgentAvctivationStatus.set_fm_radio_broadcasting(m_bfm_radio_broadcasting);
    rVrAgentAvctivationStatus.set_am_radio_broadcasting(m_bam_radio_broadcasting);
    rVrAgentAvctivationStatus.set_fm_hd_radio_broadcasting(m_bfm_hd_radio_broadcasting);
    rVrAgentAvctivationStatus.set_optional_hd_radio_broadcasting(m_boptional_hd_radio_broadcasting);
    rVrAgentAvctivationStatus.set_xm_hd_connecting(m_bxm_hd_connecting);
    rVrAgentAvctivationStatus.set_xm_radio_broadcasting(m_bxm_radio_broadcasting);
    rVrAgentAvctivationStatus.set_telephone_connection_state(m_btelephone_connection_state);
    rVrAgentAvctivationStatus.set_spp_connection_state(m_bspp_connection_state);
    rVrAgentAvctivationStatus.set_usb_slip_connection_state(m_busb_slip_connection_state);
    rVrAgentAvctivationStatus.set_map_sd_insert_state(m_bmap_sd_insert_state);
    rVrAgentAvctivationStatus.set_dataservice_availability(m_bdataservice_availability);
    rVrAgentAvctivationStatus.set_entune_availability(m_bentune_availability);
    rVrAgentAvctivationStatus.set_map_connection_state(m_bmap_connection_state);
    rVrAgentAvctivationStatus.set_register_pairing_state(m_bregister_pairing_state);
    rVrAgentAvctivationStatus.set_recog_state(navi::VoiceRecog::Unspecfied);    // Outside of North America destination is 00H : Unspecified
}

VR_ConfigureIF* VR_ConfigureIF::Instance()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    static VR_Configure m_config;
    return &m_config;
}

VR_Configure::VR_Configure()
    : m_svr_setting()
    , m_bInitFlag(false)
    , m_agentStatus()
    , m_updateState()
    , m_updateGrammar()
    , m_engineStatusControl(VR_Engine_Idle)
{
    initMap();
    Initialize();
}

int VR_Configure::convert2SysLang(const std::string &strVRLanguage)
{
    int sysLang = BL_LANGUAGE_TYPE_DEFAULT;
    VoiceMap<std::string, int>::const_iterator it = vrLanguages.find(strVRLanguage);
    if (it != vrLanguages.end()) {
        sysLang = it->second;
    }
    VR_LOGD("VRLang = %s => SysLang = %d", strVRLanguage.c_str(), sysLang);
    return sysLang;
}

std::string VR_Configure::getTTSLanguage(std::string cLanguage)
{
    std::string vrTTSLanguage = "";
    for (int iIndex = 0; iIndex < TTS_LANG_MAX_NUM; iIndex++) {
        if (cLanguage == vrTTSLanguageMap[iIndex][0]) {
            vrTTSLanguage = vrTTSLanguageMap[iIndex][1];
            break;
        }
    }
    RETURN(vrTTSLanguage);
}

void VR_Configure::getTTSLanguage(std::string cLanguage, std::string &rsTTSLang, std::string &rcSex)
{
    rsTTSLang = "";
    rcSex = "";
    for (int iIndex = 0; iIndex < TTS_LANG_MAX_NUM; iIndex++) {
        if (cLanguage == vrTTSLanguageMap[iIndex][0]) {
            rsTTSLang = vrTTSLanguageMap[iIndex][1];
            rcSex = vrTTSLanguageMap[iIndex][2];
            break;
        }
    }
}

void VR_Configure::setCountryIDForVBT(nutshell::INT32 i32CountryID)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_iCountryIDVBT = i32CountryID;
}

nutshell::INT32 VR_Configure::getCountryIDForVBT()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_iCountryIDVBT;
}

void  VR_Configure::Initialize()
{
    VR_LOGD_FUNC();

    if (m_bInitFlag) {
        return;
    }
    m_bInitFlag = true;

    // get region
    nutshell::NCEnvironment environment;
    nutshell::NCString ncContry;
    environment.getProductCountry(ncContry);
    m_region = ncContry.getString();
    VR_LOGD("region = %s", m_region.c_str());

    // get language settings.
    VR_ContentProvider vr_cp;
    m_svr_setting.language = vr_cp.GetContentProviderValue(VR_CP_VR_LANGUAGE);
    // get system language
    std::string strSystemLanguage = vr_cp.GetContentProviderValue(VR_CP_LANGUAGE);
    VR_LOGI("strSystemLanguage = [%s]", strSystemLanguage.c_str());

    std::string aecStatus = vr_cp.GetContentProviderValue(VR_ENGINE_AEC_STATUS);
    VR_LOGI("iAuto3 aecStatus = [%s]", aecStatus.c_str());
    if (aecStatus.compare("ON") == 0) {
        m_svr_setting.AECSwitch = true;
    }
    else {
        m_svr_setting.AECSwitch = false;
    }

    // get the input output wavfile button.
    m_svr_setting.inputWavFlag = false;
    m_svr_setting.outputWavFlag = false;
    // get country
    BL_APLINFO_REGION blRegion =  BL_AplInfo::GetRegion();
    m_svr_setting.country = BLRegionToVRRegion(blRegion);
    VR_LOGI("blregion: [%d] , country:[%d]", blRegion, m_svr_setting.country);

    if (m_svr_setting.country == VR_REGION_OC) {
        m_svr_setting.language = VR_LANGUAGE_EN_AU;
    }
    VR_LOGI("m_svr_setting.language = [%s]", m_svr_setting.language.c_str());

    // switch language , quickly acc off / on, according to the region
    // make vr_language and langauge is the same,
    // the logic is same with the vr_setting listener
    // When Malay, Indonesian, or Vietnamese is selected as the system language, English VR is activated.
    // SEA Language: Malay, Indonesian Vietnamese  Thai  UK English
    if (m_svr_setting.country != VR_REGION_EUROPEAN) {
        if (m_svr_setting.country == VR_REGION_SEA || m_svr_setting.country == VR_REGION_THAILAND) {
            if (VR_LANGUAGE_TH != strSystemLanguage) {
                strSystemLanguage = VR_LANGUAGE_EN_GB;
            }
        }

        if (strSystemLanguage != m_svr_setting.language) {
            m_svr_setting.language = strSystemLanguage;
        }
    }

    // get product.
    BL_APLINFO_PRODUCT_TYPE blProduct =  BL_AplInfo::GetProductType();
    m_svr_setting.product = BLProductToVRProduct(blProduct);
    VR_LOGI("blprodcut: [%d], prodcut:[%d]", blProduct, m_svr_setting.product);

    std::string strPromptLevel = vr_cp.GetContentProviderValue(VR_CP_PROMPT_LEVEL);
    VR_LOGI("promptlevel = [%s]", strPromptLevel.c_str());

    if (strPromptLevel == "On" || strPromptLevel == "on") {
        if (m_svr_setting.country == VR_REGION_US) {
            m_svr_setting.promptlevel = VR_PROMPTLEVEL_HIGH;
        }
        else {
            m_svr_setting.promptlevel = VR_PROMPTLEVEL_ON;
        }
    }
    else if (strPromptLevel == "Off" || strPromptLevel == "off") {
        m_svr_setting.promptlevel = VR_PROMPTLEVEL_OFF;
    }
    else if (strPromptLevel == "High" || strPromptLevel == "high") {
        m_svr_setting.promptlevel = VR_PROMPTLEVEL_HIGH;
    }
    else {
        m_svr_setting.promptlevel = VR_PROMPTLEVEL_LOW;
    }
    std::string strBagin = vr_cp.GetContentProviderValue(VR_CP_SPEAK_OVER_PROMPT);
    VR_LOGI("strBagin = [%s]", strBagin.c_str());

    if (strBagin == "On" || strBagin == "on") {
        m_svr_setting.speak_over_prompt = true;
    }
    else {
        m_svr_setting.speak_over_prompt = false;
    }

    std::string strWebSearchEngine = vr_cp.GetContentProviderValue(VR_CP_WEB_SEARCH_ENGINE);
    VR_LOGI("strSearchEngine = [%s]", strWebSearchEngine.c_str());
    m_svr_setting.web_search_engine = strWebSearchEngine; // for  na t2

    std::string strComfirm = vr_cp.GetContentProviderValue(VR_CP_DISPLAY_COMFIRM);
    VR_LOGI("strComfirm = [%s]", strComfirm.c_str());

    if (strComfirm == "On" || strComfirm == "on") {
        m_svr_setting.display_confirm = true;
    }
    else {
        m_svr_setting.display_confirm = false;
    }
    m_svr_setting.speak_over_beep = false;

    // set current engine
    // Load appropriate engine according to the region.
    // the default engine is suntec dialog engine.
    switch (m_svr_setting.country) {
    case VR_REGION_US:
    case VR_REGION_OC:
        {
            m_currentEngineType = VR_EngineType::VBT;
        }
        break;
    case VR_REGION_CHINA:
        {
            m_currentEngineType = VR_EngineType::IFLYTEK;
        }
        break;
    default:
        {
            m_currentEngineType = VR_EngineType::SUNTEC;
        }
        break;
    }
    m_currentEngineDir = m_engineDirMap[m_currentEngineType];
    configurePath();

    // avc lan connect status init
    m_bAvcLanConnected = false;
    // avc lan connect status begin
    std::string  strStatus = vr_cp.GetContentProviderValue(VR_UNIT_STATUS);
    std::string  strType   = "dcu";
    VR_LOGD("strStatus=[%s]", strStatus.c_str());
    if ("connected" == strStatus) {
        strType = vr_cp.GetContentProviderValue(VR_UNIT_TYPE);
        VR_LOGD("strType=[%s]", strType.c_str());
        if (("meu" == strType) || ("dcu" == strType)) {
            VR_LOGD("meu == strType || dcu == strType");
            m_bAvcLanConnected = true;
        }
    }
    m_bAvcLanConnected = false;
    // avc lan connect status end
    m_bStartStatus = false;

    setAsrMapDataPath(true); //  add for ASR when update MapData

    m_iRoadKind = 0xFFFFFFFF;
    m_iLon = 0xFFFFFFFF;
    m_iLat = 0xFFFFFFFF;
    m_bDemoOn = false;
    m_bCursor = false;

    // set head unit type
    if (("connected" == strStatus) && ("dcu" == strType)) {
        setHeadUnitType(VR_HEADUNIT_MEU);
    }
    else {
        setHeadUnitType(VR_HEADUNIT_DCU);
    }

    // for VBT for AU
    m_iCountryIDVBT = 1; // 1: AU 2:NZ

    m_bVehicleHybridVRFlag = false;

    m_uiNaviStatus = 3; // EVIP_SERVICE_STATUS_READY

    m_bVRStartAgentStatus = false;
    m_bPersonData = true;
    m_bSmartAgent = true;
    m_bHigherInterrupt = true;
    m_bCancelStatus = false;
    m_bOpenSessionStatus = false;
    m_bPrepareStatus = false;
    m_bUsbConnect = false;
    m_bIpodConnect = false;
    m_bBtaConnect = false;

    // set strHybridVRUrl String to "" when init.
    m_strHybridVRUrl = "";

    m_bRadioStatus = false;
}

VR_Configure::~VR_Configure()
{

}

void VR_Configure::initMap()
{
    m_engineDirMap[VR_EngineType::SUNTEC] = "/SUNTEC/";
    m_engineDirMap[VR_EngineType::VBT] = "/VBT/";
    m_engineDirMap[VR_EngineType::IFLYTEK] = VR_SUNTEC_PATH_NAME;

    m_engineLibraryNameMap[VR_EngineType::SUNTEC] = "libvr_dialogengine_suntec-navi.so";
    m_engineLibraryNameMap[VR_EngineType::VBT] = "libvr_dialogengine_vbt-navi.so";
    m_engineLibraryNameMap[VR_EngineType::IFLYTEK] = "libvr_dialogengine_suntec-navi.so"; // "libvr_dialogengine_iflytec-navi.so";
}

void VR_Configure::configurePath()
{
    BL_File pathFile(BL_FILE_PREFIX_MDATA);
    BL_String mapDataFullPath;
    pathFile.GetLogicalPath("VR", mapDataFullPath);
    m_mapDataFullPath.assign(mapDataFullPath.GetString());
    VR_LOGI("MDATA Path: %s", getMapDataPath().c_str());

    pathFile.ChangePrefix(BL_FILE_PREFIX_VDATA);
    BL_String dataFullPath;
    pathFile.GetLogicalPath("VR", dataFullPath);
    m_dataFullPath.assign(dataFullPath.GetString());
    VR_LOGI("VDATA Path: %s", getDataPath().c_str());

    pathFile.ChangePrefix(BL_FILE_PREFIX_RW);
    BL_String usrFullPath;
    pathFile.GetLogicalPath("VR", usrFullPath);
    m_usrFullPath.assign(usrFullPath.GetString());
    VR_LOGI("USER Path: %s", getUsrPath().c_str());

    BL_Dir dir(BL_FILE_PREFIX_RW);
    dir.MakeDir("VR");
    if (!m_currentEngineDir.empty()) {
        dir.MakeDir((std::string("VR") + m_currentEngineDir).c_str());
        m_relativeUsrPath = "VR" + m_currentEngineDir;
    }
    dir.MakeDir((std::string("VR") + VR_TUTORIAL_PATH_NAME).c_str());
    dir.MakeDir((std::string("VR") + VR_ASR_PATH_NAME).c_str());
}

const std::string VR_Configure::getDataPath()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_dataFullPath + m_currentEngineDir;
}

const std::string VR_Configure::getMapDataPath()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_mapDataFullPath;
}

const std::string VR_Configure::getUsrPath()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_usrFullPath + m_currentEngineDir;
}

const std::string VR_Configure::getAsrDataPath()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);

    std::string strAsrDataPath = "";
    switch (m_svr_setting.country) {
    case VR_REGION_CHINA:
        {
            strAsrDataPath = m_dataFullPath + VR_IFLYTEK_PATH_NAME;
        }
        break;
    default:
        {
            strAsrDataPath = m_dataFullPath + VR_ASR_PATH_NAME;
        }
        break;
    }

    return strAsrDataPath;
}

void VR_Configure::setAsrMapDataPath(bool mock)
{
    VR_LOGD_FUNC();
    nutshell::NCAutoSync  lock(g_cSyncObj);
    if (mock) {
        m_asrMapDataPath = m_mapDataFullPath + VR_ASR_PATH_NAME;
    }
    else {
        m_asrMapDataPath = "";
    }
}

const std::string VR_Configure::getAsrMapDataPath()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_asrMapDataPath; // m_mapDataFullPath + VR_ASR_PATH_NAME;
}

const std::string VR_Configure::getAsrUsrPath()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_usrFullPath + VR_ASR_PATH_NAME;
}

const std::string VR_Configure::getTutorialPath()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_dataFullPath + VR_TUTORIAL_PATH_NAME;
}

bool VR_Configure::makeDir(const std::string &dirName)
{
    BL_Dir dir(BL_FILE_PREFIX_RW);
    return dir.MakeDir((m_relativeUsrPath + dirName).c_str());
}

bool VR_Configure::makeDirTree(const std::string &dirName)
{
    BL_Dir dir(BL_FILE_PREFIX_RW);
    return dir.MakeDirTree((m_relativeUsrPath + dirName).c_str());
}

bool VR_Configure::removeDir(const std::string &dirName)
{
    BL_Dir dir(BL_FILE_PREFIX_RW);
    return dir.RemoveDir((m_relativeUsrPath + dirName).c_str());
}

bool VR_Configure::moveFile(const std::string &srcFileName, const std::string &dstFileName)
{
    BL_Dir dir(BL_FILE_PREFIX_RW);
    return dir.MoveFile((m_relativeUsrPath + srcFileName).c_str(), (m_relativeUsrPath + dstFileName).c_str());
}

bool VR_Configure::removeFile(const std::string &fileName)
{
    BL_File file(BL_FILE_PREFIX_RW);
    return file.RemoveFile((m_relativeUsrPath + fileName).c_str());
}

bool VR_Configure::isFileExist(const std::string &fileName)
{
    BL_File file(BL_FILE_PREFIX_RW);
    return file.FileExists((m_relativeUsrPath + fileName).c_str());
}

bool VR_Configure::isSuntectFileExist(const std::string &fileName)
{
    BL_File file(BL_FILE_PREFIX_VDATA);
    std::string strPath = "VR/SUNTEC/";
    strPath += fileName;

    if (file.FileExists(strPath.c_str())) {
        VR_LOGI("[%s] BL_FILE_SEARCH_FOUND_FILE", __FUNCTION__);
        RETURN(true);
    }

    VR_LOGI("[%s] BL_FILE_SEARCH_NOT_FOUND_FILE ", __FUNCTION__);
    RETURN(false);
}

VR_ConfigureIF::VR_EngineType VR_Configure::getEngineType()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_currentEngineType;
}

std::string VR_Configure::getEngineLibraryName()
{
    return m_engineLibraryNameMap[m_currentEngineType];
}

VR_REGION_TYPE  VR_Configure::BLRegionToVRRegion(BL_APLINFO_REGION blRegion)
{
    VR_REGION_TYPE vrRegion(VR_REGION_INVALID);
    switch (blRegion) {
    case BL_APLINFO_REGION_INVALID:
        {
            vrRegion = VR_REGION_INVALID;
            break;
        }
    case BL_APLINFO_REGION_CHINA:   // China
        {
            vrRegion = VR_REGION_CHINA;
            break;
        }
    case BL_APLINFO_REGION_US:  // America Canada Mexico
        {
            vrRegion = VR_REGION_US;
            break;
        }
    case BL_APLINFO_REGION_IN: // India
        {
            vrRegion = VR_REGION_IN;
            break;
        }
    case BL_APLINFO_REGION_BR:  // Brazil
    case BL_APLINFO_REGION_AR:  // South-America
        {
            vrRegion = VR_REGION_BR;
            break;
        }
    case BL_APLINFO_REGION_SEA: // eath-south aria
    case BL_APLINFO_REGION_THAILAND: // Thailand
        {
            vrRegion = VR_REGION_SEA;
            break;
        }
    case BL_APLINFO_REGION_SA:  // South-Africa
        {
           vrRegion = VR_REGION_SA;
           break;
        }
    case BL_APLINFO_REGION_QG: // MiddleーEast
        {
            vrRegion = VR_REGION_QG;
            break;
        }
    case BL_APLINFO_REGION_TW: // Taiwan
        {
            vrRegion = VR_REGION_TW;
            break;
        }
    case BL_APLINFO_REGION_OC: // Oceania
        {
            vrRegion = VR_REGION_OC;
            break;
        }
    case BL_APLINFO_REGION_HK: // HongKong　Macau
        {
            vrRegion = VR_REGION_HK;
            break;
        }
    case BL_APLINFO_REGION_KR: // SouthKorea
        {
            vrRegion = VR_REGION_KR;
            break;
        }
    case BL_APLINFO_REGION_EUROPEAN: // SouthKorea
        {
            vrRegion = VR_REGION_EUROPEAN;
            break;
        }
    default:
        {
            vrRegion = VR_REGION_INVALID;
            VR_LOGD("!!!!!!INVAILD REGION!!!!!!! %s", BL_AplInfo::GetRegionStr());
            break;
        }
    }
    return vrRegion;
}

VR_PRODUCT_TYPE VR_Configure::BLProductToVRProduct(BL_APLINFO_PRODUCT_TYPE blProduct)
{
    VR_PRODUCT_TYPE vrProduct(VR_PRODUCT_TYPE_UNKNOWN);
    switch (blProduct) {
    case BL_APLINFO_PRODUCT_TYPE_UNKNOWN:
        {
            vrProduct = VR_PRODUCT_TYPE_UNKNOWN;
            break;
        }
    case BL_APLINFO_PRODUCT_TYPE_T0:
        {
            vrProduct = VR_PRODUCT_TYPE_T0;
            break;
        }
    case BL_APLINFO_PRODUCT_TYPE_T1:
        {
            vrProduct = VR_PRODUCT_TYPE_T1;
            break;
        }
    case BL_APLINFO_PRODUCT_TYPE_T2:
        {
            vrProduct = VR_PRODUCT_TYPE_T2;
            break;
        }
    case BL_APLINFO_PRODUCT_TYPE_T_EMV:
        {
            vrProduct = VR_PRODUCT_TYPE_T_EMV;
            break;
        }
    case BL_APLINFO_PRODUCT_TYPE_L1:
        {
            vrProduct = VR_PRODUCT_TYPE_L1;
            break;
        }
    case BL_APLINFO_PRODUCT_TYPE_L1_5:
        {
            vrProduct = VR_PRODUCT_TYPE_L1_5;
            break;
        }
    case BL_APLINFO_PRODUCT_TYPE_L2:
        {
            vrProduct = VR_PRODUCT_TYPE_L2;
            break;
        }
    default:
        break;
    }
    return vrProduct;
}

const std::string VR_Configure::getVRLanguage()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_svr_setting.language;
}

void VR_Configure::setVRLanguage(const std::string &strVRLanguage)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_svr_setting.language = strVRLanguage;
}

VR_REGION_TYPE VR_Configure::getVRContry()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_svr_setting.country;
}

void VR_Configure::setVRContry(VR_REGION_TYPE country)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_svr_setting.country = country;
}

std::string VR_Configure::getVRRegion()
{
    return m_region;
}


VR_PRODUCT_TYPE VR_Configure::getVRProduct()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_svr_setting.product;
}

void VR_Configure::setVRProduct(VR_PRODUCT_TYPE product)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_svr_setting.product = product;
}

const std::string VR_Configure::getVRWebSearchEng()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_svr_setting.web_search_engine;
}

void VR_Configure::setVRWebSearchEng(const std::string &strVRWebSearchEng)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_svr_setting.web_search_engine = strVRWebSearchEng;
}

int  VR_Configure::getVRPromptLevel()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_svr_setting.promptlevel;
}

void  VR_Configure::setVRPromptLevel(int nPromptLevel)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_svr_setting.promptlevel = nPromptLevel;
}

bool VR_Configure::getVROverPrompt()
{
    VR_PRODUCT_TYPE emProductType = getVRProduct();
    VR_REGION_TYPE emVRCountry = getVRContry();
    if ((VR_PRODUCT_TYPE_T0 == emProductType) || (VR_PRODUCT_TYPE_T1 == emProductType) || (VR_PRODUCT_TYPE_T2 == emProductType) ||  (VR_PRODUCT_TYPE_L1 == emProductType)) {
        if (VR_REGION_US == emVRCountry) {
            VR_LOGD("SUPPORTED ! Prodcut = %d Count %d speak_over_prompt = %d", emProductType, emVRCountry, m_svr_setting.speak_over_prompt);
            nutshell::NCAutoSync  lock(g_cSyncObj);
            return m_svr_setting.speak_over_prompt;
        }
    }

    VR_LOGD("NOT SUPPORTED ! Prodcut = %d Count %d", emProductType, emVRCountry);
    return false;
}

void  VR_Configure::setVROverPrompt(bool bOverPrompt)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_svr_setting.speak_over_prompt = bOverPrompt;
}

bool VR_Configure::getVROverBeep()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_svr_setting.speak_over_beep;
}

bool VR_Configure::getOutputWavOption()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_svr_setting.outputWavFlag;
}

bool VR_Configure::getInputWavOption()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_svr_setting.inputWavFlag;
}

void VR_Configure::setInputWavOption(bool on)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_svr_setting.inputWavFlag = on;
}

void VR_Configure::setOutputWavOption(bool on)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_svr_setting.outputWavFlag = on;
}

void VR_Configure::setVROverBeep(bool bOverBeep)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_svr_setting.speak_over_beep = bOverBeep;
}

bool VR_Configure::getVRDisplayComfirm()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_svr_setting.display_confirm;
}

void VR_Configure::setVRDisplayComfirm(bool bDisplayComfirm)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_svr_setting.display_confirm = bDisplayComfirm;
}

bool VR_Configure::getAECEngineStatus()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_svr_setting.AECSwitch;
}

void VR_Configure::setAECEngineStatus(bool status)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_svr_setting.AECSwitch = status;
}


// get avclan connect status
bool VR_Configure::getAvcLanConnectStatus()
{
    VR_ContentProvider        vr_cp;
    std::string  strStatus = vr_cp.GetContentProviderValue(VR_UNIT_STATUS);
    std::string  strType   = "dcu";

    VR_LOGI("strStatus=[%s]", strStatus.c_str());
    if ("connected" == strStatus) {
        strType = vr_cp.GetContentProviderValue(VR_UNIT_TYPE);
        if (("meu" == strType) || ("dcu" == strType)) {
            VR_LOGI("return true");
            return true;
        }
    }
    VR_LOGI("return false");
    return false;
}

bool VR_Configure::getInnerAvcLanConnectStatus()
{

    return false;
    VR_REGION_TYPE country = getVRContry();
    VR_PRODUCT_TYPE productType = getVRProduct();
    VR_LOGD("VR_Configure::getInnerAvcLanConnectStatus country = [%d], productType = [%d]", country, productType);

    if (VR_PRODUCT_TYPE_T0 == productType) {
        return false;
    }
    else if (VR_PRODUCT_TYPE_T1 == productType) {
        return false;
    }
    else if (VR_PRODUCT_TYPE_T2 == productType) {
        if ((VR_REGION_US == country) || (VR_REGION_OC == country) || (VR_REGION_SA == country) || (VR_REGION_CHINA == country) || (VR_REGION_SEA == country)) {
            return true;
        }
        else {
            return false;
        }
    }
    else if (VR_PRODUCT_TYPE_T_EMV == productType) {
        if ((VR_REGION_OC == country) || (VR_REGION_SA == country) || (VR_REGION_SEA == country)) {
            return true;
        }
        else {
            return false;
        }
    }
    else if (VR_PRODUCT_TYPE_L1 == productType) {
        return false;
    }
    else if (VR_PRODUCT_TYPE_L1_5 == productType) {
        if (VR_REGION_CHINA == country) {
            return true;
        }
        else {
            return false;
        }
    }
    else if (VR_PRODUCT_TYPE_L2 == productType) {
        if ((VR_REGION_HK == country) || (VR_REGION_OC == country) || (VR_REGION_SA == country) || (VR_REGION_CHINA == country) || (VR_REGION_SEA == country)) {
            return true;
        }
        else {
            return false;
        }
    }
    return false;
}

bool VR_Configure::isVoiceTagRegion()
{
    VR_REGION_TYPE country = getVRContry();
    VR_PRODUCT_TYPE productType = getVRProduct();
    VR_LOGD("VR_Configure::getInnerAvcLanConnectStatus country = [%d], productType = [%d]", country, productType);

    if (VR_PRODUCT_TYPE_T2 == productType) {
        /* Middle and Near East: VR_REGION_QG  Taiwan: VR_REGION_TW  Brazil+South America: VR_REGION_BR   India: VR_REGION_IN */
        if ((VR_REGION_QG == country) || (VR_REGION_TW == country) || (VR_REGION_BR == country) || (VR_REGION_IN == country)) {
            return true;
        }
    }
    else if (VR_PRODUCT_TYPE_T_EMV == productType) {
        /* Middle and Near East: VR_REGION_QG South America: VR_REGION_BR   India: VR_REGION_IN */
        if ((VR_REGION_QG == country) || (VR_REGION_BR == country) || (VR_REGION_IN == country)) {
            return true;
        }
    }
    else if (VR_PRODUCT_TYPE_L2 == productType) {
        /* Middle and Near East: VR_REGION_QG  taiwan: VR_REGION_TW  Brazil: VR_REGION_BR */
        if ((VR_REGION_QG == country) || (VR_REGION_TW == country) || (VR_REGION_BR == country)) {
            return true;
        }
    }

    return false;
}


bool   VR_Configure::getAgentStatus(VR_AGENT_STATUS aType) const
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return  m_agentStatus.GetAgentStatus(aType);
}

void  VR_Configure::setAgentStatus(VR_AGENT_STATUS aType, bool bStatus)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_agentStatus.SetAgentStatus(aType, bStatus);
}

void VR_Configure::setAgentStatusToProto(VR_ProtoMessageBase& rMsg)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_agentStatus.SetAgentStatusToProto(rMsg);
}

void VR_Configure::setLocateInfo(nutshell::INT32 wroadkind, nutshell::INT32 iLon, nutshell::INT32 iLat, bool bDemoOn)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_iRoadKind = wroadkind;
    m_iLon = iLon;
    m_iLat = iLat;
    m_bDemoOn = bDemoOn;
}

void VR_Configure::setDemoOnFlg(bool bDemoOn)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_bDemoOn = bDemoOn;
}

void VR_Configure::getLocateInfo(nutshell::INT32 &wroadkind, nutshell::INT32 &iLon, nutshell::INT32 &iLat, bool &bDemoOn)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    wroadkind = m_iRoadKind;
    iLon = m_iLon;
    iLat = m_iLat;
    bDemoOn = m_bDemoOn;
}

bool VR_Configure::getDemoOnFlg()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_bDemoOn;
}

void VR_Configure::setCursorFlg(bool bCursor)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_bCursor = bCursor;
}

bool VR_Configure::getCursorFlg()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_bCursor;
}

bool  VR_Configure::getStartStatus()   const
{
    nutshell::NCAutoSync   lock(g_cSyncObj);
    return   m_bStartStatus;
}

void  VR_Configure::setStartStatus(bool bStatus)
{
    nutshell::NCAutoSync   lock(g_cSyncObj);
    m_bStartStatus  =  bStatus;
}

const std::string& VR_Configure::getHeadUnitType() const
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_headUnitType;
}

void VR_Configure::setHeadUnitType(const std::string &strHeadUnitType)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_headUnitType = strHeadUnitType;
}

const std::string VR_Configure::getUnitMeasurement()
{
    VR_ContentProvider vr_cp;
    std::string Unit6 = vr_cp.GetContentProviderValue(VR_CP_UNIT_OF_MEASUREMENT);
    VR_LOGD("VR Unit6 = [%s]", Unit6.c_str());
    return Unit6;
}

void VR_Configure::resetMapDataPath()
{
#if 0
    BL_File pathFile(BL_FILE_PREFIX_MDATA);
    BL_String mapDataFullPath;
    pathFile.GetAbsolutePath("VR", mapDataFullPath);
    {
      nutshell::NCAutoSync  lock(g_cSyncObj);
      m_mapDataFullPath.assign(mapDataFullPath.GetString());
    }
    VR_LOGI("resetMapDataPath MDATA Path: %s", getMapDataPath().c_str());
#else
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_mapDataFullPath = "/pdata/.MAPDATA/MDATA/VR";
#endif
    VR_LOGI("resetMapDataPath MDATA Path: %s", getMapDataPath().c_str());
}

void VR_Configure::setHybridVRFlag(bool bVehiclTypeFlag)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    VR_LOGD("VR_Configure::setHybridVRFlag bVehiclTypeFlag = [%d]", bVehiclTypeFlag);
    m_bVehicleHybridVRFlag = bVehiclTypeFlag;
}

bool VR_Configure::getHybridVRFlag()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_bVehicleHybridVRFlag;
}

void VR_Configure::setNaviStatus(unsigned int iStatus)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_uiNaviStatus = iStatus;
}

unsigned int VR_Configure::getNaviStatus()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_uiNaviStatus;
}

bool VR_Configure::getVRStartAgentStatus()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_bVRStartAgentStatus;
}

void VR_Configure::setVRStartAgentStatus(bool bStatus)
{
     nutshell::NCAutoSync  lock(g_cSyncObj);
     m_bVRStartAgentStatus = bStatus;
}

bool VR_Configure::getVRCancelStatus()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_bCancelStatus;
}

void VR_Configure::setVRCancelStatus(bool bStatus)
{
     nutshell::NCAutoSync  lock(g_cSyncObj);
     m_bCancelStatus = bStatus;
}

bool VR_Configure::getOpenSessionStatus()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_bOpenSessionStatus;
}

void VR_Configure::setOpenSessionStatus(bool bStatus)
{
     nutshell::NCAutoSync  lock(g_cSyncObj);
     m_bOpenSessionStatus = bStatus;
}

bool VR_Configure::getPrepareStatus()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_bPrepareStatus;
}

void VR_Configure::setPrepareStatus(bool bStatus)
{
     nutshell::NCAutoSync  lock(g_cSyncObj);
     m_bPrepareStatus = bStatus;
}

bool VR_Configure::getPersonDataStatus()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_bPersonData;
}

void VR_Configure::setPersonDataStatus(bool bStatus)
{
     nutshell::NCAutoSync  lock(g_cSyncObj);
     m_bPersonData = bStatus;
}

bool VR_Configure::getSmartAgentStatus()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_bSmartAgent;
}

void VR_Configure::setSmartAgentStatus(bool bStatus)
{
     nutshell::NCAutoSync  lock(g_cSyncObj);
     m_bSmartAgent = bStatus;
}

bool VR_Configure::getHigherInterruptStatus()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_bHigherInterrupt;
}

void VR_Configure::setHigherInterruptStatus(bool bStatus)
{
     nutshell::NCAutoSync  lock(g_cSyncObj);
     m_bHigherInterrupt = bStatus;
}

void VR_Configure::setHybridVRUrl(const std::string &rstrUrl)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_strHybridVRUrl = rstrUrl;
}

std::string VR_Configure::getHybridVRUrl()
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_strHybridVRUrl;
}

void VR_Configure::setUpdateState(const std::string& strStateKey, const std::string& strStateValue)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_updateState.SetUpdateState(strStateKey, strStateValue);
}

void VR_Configure::getStateValueByStateKey(const std::string& strStateKey, std::string& strStateValue)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_updateState.GetStateValueByStateKey(strStateKey, strStateValue);
}

void VR_Configure::setUpdateGrammar(const std::string& strGrammarKey, const std::string& strGrammarValue)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_updateGrammar.SetUpdateGrammar(strGrammarKey, strGrammarValue);
}

void VR_Configure::getGrammarValueByGrammarKey(const std::string& strGrammarKey, std::string& strGrammarVule)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_updateGrammar.GetGrammarValueByGrammarKey(strGrammarKey, strGrammarVule);
}

void VR_Configure::setMediaConnectStatus(VR_MEDIA_CONNECT_STATUS emType, bool bStatus)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    if (usb_connect == emType) {
        m_bUsbConnect = bStatus;
    }
    else if (ipod_connect == emType) {
        m_bIpodConnect = bStatus;
    }
    else if (btaudio_connect == emType) {
        m_bBtaConnect = bStatus;
    }
}

bool VR_Configure::getMediaConnectStatus(VR_MEDIA_CONNECT_STATUS emType)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    bool bStatus = false;
    if (usb_connect == emType) {
        bStatus = m_bUsbConnect;
    }
    else if (ipod_connect == emType) {
        bStatus = m_bIpodConnect;
    }
    else if (btaudio_connect == emType) {
        bStatus = m_bBtaConnect;
    }
    return bStatus;
}

std::string VR_Configure::getStrRoadName() const
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_strRoadName;
}

void VR_Configure::setStrRoadName(const std::string &strRoadName)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_strRoadName = strRoadName;
}

bool VR_Configure::getBRadioStatus() const
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    return m_bRadioStatus;
}

void VR_Configure::setBRadioStatus(bool bRadioStatus)
{
    nutshell::NCAutoSync  lock(g_cSyncObj);
    m_bRadioStatus = bRadioStatus;
}

VR_ENGINE_STATUS_CONTROL VR_Configure::getEngineCurrentStatus()
{
    nutshell::NCAutoSync lock(g_cSyncObj);
    return m_engineStatusControl;
}

bool VR_Configure::setEngineCurrentStatus(VR_ENGINE_STATUS_CONTROL status)
{
    nutshell::NCAutoSync lock(g_cSyncObj);

    if (status == VR_Engine_Idle) {
        m_engineStatusControl = status;
        return true;
    }

    bool engineIsActivtity;

    switch (m_engineStatusControl) {
    case VR_Engine_Idle:
        engineIsActivtity = true;
        break;
    case VR_Engine_Alexa_Activtity:
        if (status == VR_Engine_Alexa_Activtity) {
            engineIsActivtity = true;
        }
        else {
            engineIsActivtity = false;
        }
        break;
    case VR_Engine_Baidu_Activtity:

    case VR_Engine_Google_Activtity:

    case VR_Engine_Dictation_Activtity:

    case VR_Engine_Nuance_Activtity:
        engineIsActivtity = false;
        break;
    default:
        break;
    }

    if (engineIsActivtity) {
        m_engineStatusControl = status;
    }

    return engineIsActivtity;
}



/* EOF */
