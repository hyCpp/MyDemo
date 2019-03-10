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

/**
 * @file VR_ConfigureDEF.h
 * @brief Declaration file of VR_ConfigureDEF.
 *
 * This file includes the declaration of VR_ConfigureDEF.
 * provide interface of VR_ConfigureDEF
 *
 * @attention used for C++ only.
 */

#ifndef VR_CONFIGUREDEF_H
#define VR_CONFIGUREDEF_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <string>
// #include <BL_AplInfo.h>
// language
// refer to http: //iwiki/Project/17Model/iAutoPlatform/17CY/Project/17Model/iAutoPlatform/17CY/projectcommon/language-abbr
// en-us:default
// http://review.iauto.net/developguide/iauto2-settings-uri
const std::string VR_LANGUAGE_EN_US = "en-us";
const std::string VR_LANGUAGE_FR_CA = "fr-ca";
const std::string VR_LANGUAGE_ES_MX = "es-mx";
const std::string VR_LANGUAGE_ES    = "es";
const std::string VR_LANGUAGE_PT    = "pt";
const std::string VR_LANGUAGE_CS    = "cs";
const std::string VR_LANGUAGE_DA    = "da";
const std::string VR_LANGUAGE_NL    = "nl";
const std::string VR_LANGUAGE_FI    = "fi";
const std::string VR_LANGUAGE_NL_BE = "nl-be";
const std::string VR_LANGUAGE_FR    = "fr";
const std::string VR_LANGUAGE_DE    = "de";
const std::string VR_LANGUAGE_EL    = "el";
const std::string VR_LANGUAGE_IT    = "it";
const std::string VR_LANGUAGE_NO    = "no";
const std::string VR_LANGUAGE_PL    = "pl";
const std::string VR_LANGUAGE_RU    = "ru";
const std::string VR_LANGUAGE_SV    = "sv";
const std::string VR_LANGUAGE_TR    = "tr";
const std::string VR_LANGUAGE_HE    = "he";
const std::string VR_LANGUAGE_HU    = "hu";
const std::string VR_LANGUAGE_RO    = "ro";
const std::string VR_LANGUAGE_SK    = "sk";
const std::string VR_LANGUAGE_UK    = "uk";
const std::string VR_LANGUAGE_KO_KR = "ko-kr";
const std::string VR_LANGUAGE_EN_GB = "en-gb";
const std::string VR_LANGUAGE_ZH_HK = "zh-hk";
const std::string VR_LANGUAGE_EN_AU = "en-au";
const std::string VR_LANGUAGE_ZH_CN = "zh-cn";
const std::string VR_LANGUAGE_EN_ME = "en-me";
const std::string VR_LANGUAGE_AR    = "ar";
const std::string VR_LANGUAGE_ZH_TW = "zh-tw";
const std::string VR_LANGUAGE_TH    = "th";
const std::string VR_LANGUAGE_ID    = "id";
const std::string VR_LANGUAGE_VI    = "vi";
const std::string VR_LANGUAGE_MS    = "ms";
const std::string VR_LANGUAGE_PR_BR = "pt-br";
const std::string VR_LANGUAGE_EN_ID = "en-id";
const std::string VR_LANGUAGE_HI    = "hi";
const std::string VR_LANGUAGE_PA    = "pa";
const std::string VR_LANGUAGE_ML    = "ml";
const std::string VR_LANGUAGE_TA    = "ta";
const std::string VR_LANGUAGE_TE    = "te";
const std::string VR_LANGUAGE_KN    = "kn";
const std::string VR_LANGUAGE_BN    = "bn";
const std::string VR_LANGUAGE_GU    = "gu";
const std::string VR_LANGUAGE_MR    = "mr";


// product's sales country
// "Japan", "America", "Canada", "Mexico", "European", "Russia",
// "China", "Oceania", "SouthKorea", "Taiwan", "North-Africa",
// "Middle-East", "Africa", "South-America", "Middle-America", "Brazil",
// "South-Africa", "Southeast-Asia", "Thailand", "South-Asia", "HongkongMacau",
// "India"
const std::string VR_SALEREGION_AMERICA = "America";
const std::string VR_SALEREGION_CANADA = "Canada";
const std::string VR_SALEREGION_EUROPEAN = "European";
const std::string VR_SALEREGION_SOUTHEASTASIA = "Southeast-Asia";
const std::string VR_SALEREGION_THAILAND = "Thailand";

//  product's name
// "T0", "T1", "T2", "T-EMV",
// "L1", "L1.5", "L2"
const std::string VR_PRODUCT_T0 = "T0";
const std::string VR_PRODUCT_T1 = "T1";
const std::string VR_PRODUCT_T2 = "T2";
const std::string VR_PRODUCT_TEMV = "TEMV";
const std::string VR_PRODUCT_L1 = "L1";
const std::string VR_PRODUCT_L1_5 = "L1.5";
const std::string VR_PRODUCT_L2 = "L2";

// unit name
const std::string VR_HEADUNIT_MEU = "meu";
const std::string VR_HEADUNIT_DCU = "dcu";

// vr prompt level.
const int VR_PROMPTLEVEL_OFF = 0;
const int VR_PROMPTLEVEL_ON = 1;
const int VR_PROMPTLEVEL_LOW = 2;
const int VR_PROMPTLEVEL_HIGH = 3;

// VBT state
const std::string VR_PHONE_STATE_CONNECTED = "PHONE_STATE_CONNECTED";
const std::string VR_PHONE_DEVICE_ID = "PHONE_DEVICE_ID";
const std::string VR_PHONE_STATE_REGISTERED = "PHONE_STATE_REGISTERED";
const std::string VR_STEERING_PHONEKEY_TYPE = "STEERING_PHONEKEY_TYPE";
const std::string VR_PHONE_MAPPROTOCOL = "PHONE_MAPPROTOCOL";
const std::string VR_PHONE_GETMESSAGESTATE = "PHONE_GETMESSAGESTATE";
const std::string VR_OPERESTRICT = "OPERESTRICT";
const std::string VR_BTAUDIO_CONNECTED = "BTAUDIO_CONNECTED";
const std::string VR_USB_1_CONNECTED = "USB_1_CONNECTED";
const std::string VR_USB_2_CONNECTED = "USB_2_CONNECTED";
const std::string VR_IPOD_1_CONNECTED = "IPOD_1_CONNECTED";
const std::string VR_IPOD_2_CONNECTED = "IPOD_2_CONNECTED";
const std::string VR_MEDIAINFOKEY_MAGICSTRING = "MEDIAINFOKEY_MAGICSTRING";
const std::string VR_MUSIC_SONG_INFORMATION_AVAILABLE = "MUSIC_SONG_INFORMATION_AVAILABLE";
const std::string VR_TSL_STATE_AVAILABLE = "TSL_STATE_AVAILABLE";
const std::string VR_TSL_APPS_AVAILABLE = "TSL_APPS_AVAILABLE";
const std::string VR_TSL_APPLIST = "TSL_APPSLIST";
const std::string VR_RADIO_STATE_FM_ACTIVE = "RADIO_STATE_FM_ACTIVE";
const std::string VR_RADIO_STATE_AM_ACTIVE = "RADIO_STATE_AM_ACTIVE";
const std::string VR_RADIO_XM_CONNECTED = "RADIO_XM_CONNECTED";
const std::string VR_RADIO_STATE_FM_HD_ACTIVE = "RADIO_STATE_FM_HD_ACTIVE";
const std::string VR_RADIO_STATE_XM_ACTIVE = "RADIO_STATE_XM_ACTIVE";
const std::string VR_CLIMATE_FANSPEED_MAX = "CLIMATE_FANSPEED_MAX";
const std::string VR_CLIMATE_TEMPERATURE_MIN = "CLIMATE_TEMPERATURE_MIN";
const std::string VR_CLIMATE_TEMPERATURE_MAX = "CLIMATE_TEMPERATURE_MAX";
const std::string VR_CLIMATE_BASIC_ACTIVE = "CLIMATE_BASIC_ACTIVE";
const std::string VR_CLIMATE_FRONT_SCREEN_ACTIVE = "CLIMATE_FRONT_SCREEN_ACTIVE";
const std::string VR_CLIMATE_REAR_SCREEN_ACTIVE = "CLIMATE_REAR_SCREEN_ACTIVE";
const std::string VR_CLIMATE_STEERING_SCREEN_ACTIVE = "CLIMATE_STEERING_SCREEN_ACTIVE";
const std::string VR_CLIMATE_LEXUS_CONCIERGE_SCREEN_ACTIVE = "CLIMATE_LEXUS_CONCIERGE_SCREEN_ACTIVE";
const std::string VR_CLIMATE_CONCIERGE_ACTIVE = "CLIMATE_CONCIERGE_ACTIVE";
const std::string VR_CLIMATE_FRONT_SEAT_SCREEN_ACTIVE = "CLIMATE_FRONT_SEAT_SCREEN_ACTIVE";
const std::string VR_CLIMATE_REAR_SEAT_SCREEN_ACTIVE = "CLIMATE_REAR_SEAT_SCREEN_ACTIVE";
const std::string VR_CLIMATE_SEAT_OPERATION_SCREEN_ACTIVE = "CLIMATE_SEAT_OPERATION_SCREEN_ACTIVE";

// VBT gramamr
const std::string VR_PHONE_GRAMMAR_DISACTIVE = "phone_grammar_disactive";
const std::string VR_PHONE_GRAMMAR_INIT = "phone_grammar_init";
const std::string VR_PHONE_GRAMMAR_ACTIVE = "phone_grammar_active";
const std::string VR_PHONE_GRAMMAR_NEW_QUICKREPLAYMESSAGE = "phone_grammar_new_quickreplymessage";
const std::string VR_PHONE_GRAMMAR_NEW_PHONETYPE = "phone_grammar_new_phonetype";
const std::string VR_PHONE_GRAMMAR_NEW_MESSAGETYPE = "phone_grammar_new_messagetype";
const std::string VR_MUSIC_GRAMMAR_NEW_AUDIOSOURCE = "music_grammar_new_audiosource";
const std::string VR_MUSIC_GRAMMAR_INIT_1 = "music_grammar_init_1";
const std::string VR_MUSIC_GRAMMAR_INIT_2 = "music_grammar_init_2";
const std::string VR_MUSIC_GRAMMAR_DIFF_1 = "music_grammar_diff_1";
const std::string VR_MUSIC_GRAMMAR_DIFF_2 = "music_grammar_diff_2";
const std::string VR_MUSIC_GRAMMAR_ACTIVE = "music_grammar_active";
const std::string VR_MUSIC_GRAMMAR_DISACTIVE = "music_grammar_disactive";
const std::string VR_RADIO_GRAMMAR_NEW_FMGENRE = "radio_grammar_new_fmgenre";
const std::string VR_RADIO_GRAMMAR_NEW_SATCHANNEL = "radio_grammar_new_satchannel";
const std::string VR_RADIO_GRAMMAR_NEW_SATGENRE = "radio_grammar_new_satgenre";

enum VR_REGION_TYPE
{
    VR_REGION_INVALID = -1,
    VR_REGION_CHINA = 0,        // China
    VR_REGION_JAPAN,            // Japan
    VR_REGION_US,               // America
    VR_REGION_MALAYSIA,         // Malaysia
    VR_REGION_GERMANY,          // Germany
    VR_REGION_THAILAND,         // Thailand
    VR_REGION_SG_ID,            // Singapore & Indonesia
    VR_REGION_SG,               // Singapore
    VR_REGION_ID,               // Indonesia
    VR_REGION_MY_BN,            // Malaysia & Brunei
    VR_REGION_VN,               // Vietnam
    VR_REGION_IN,               // India
    VR_REGION_BR,               // Brazil
    VR_REGION_PH,               // Philippines
    VR_REGION_ES,
    VR_REGION_SA,               // South-Africa
    VR_REGION_GS,
    VR_REGION_AR,               // South-America(Argentina)
    VR_REGION_QG,               // Middle-East
    VR_REGION_CS,               // center south america
    VR_REGION_RC,
    VR_REGION_TW,               // Taiwan
    VR_REGION_OC,               // Oceania
    VR_REGION_HK,               // Hongkong&&Macau
    VR_REGION_SEA,              // Southeast-Asia
    VR_REGION_KR,               // SouthKorea
    VR_REGION_MEXICO,           // Mexico
    VR_REGION_EUROPEAN,         // European
    VR_REGION_RUSSIA,           // Russia
    VR_REGION_NORTH_AFRICA,     // North-Africa
    VR_REGION_AFRICA,           // Africa
    VR_REGION_MIDDLE_AMERICA,   // Middle-America
    VR_REGION_SOUTH_ASIA,       // South-Asia
    VR_REGION_CANADA,           // Canada
};

enum VR_PRODUCT_TYPE
{
    VR_PRODUCT_TYPE_UNKNOWN = 0,
    /* for 17cy project type begin */
    VR_PRODUCT_TYPE_T0,      // T0
    VR_PRODUCT_TYPE_T1,      // T1
    VR_PRODUCT_TYPE_T2,      // T2
    VR_PRODUCT_TYPE_T_EMV,   // T-EMV
    VR_PRODUCT_TYPE_L1,      // L1
    VR_PRODUCT_TYPE_L1_5,    // L1.5
    VR_PRODUCT_TYPE_L2,      // L2
    /* for 17cy project type end */
};

// define struct VR_Settings.
typedef struct {
    std::string language;
    VR_REGION_TYPE country;
    VR_PRODUCT_TYPE product;
    std::string web_search_engine;
    int promptlevel;
    bool speak_over_prompt;
    bool speak_over_beep;
    bool display_confirm;
    bool outputWavFlag;
    bool inputWavFlag;
    bool AECSwitch;
}sVR_Settings;

enum VR_AGENT_STATUS
{
    fm_radio_broadcasting = 0,
    am_radio_broadcasting = 1,
    fm_hd_radio_broadcasting = 2,
    optional_hd_radio_broadcasting = 3,
    xm_hd_connecting = 4,
    xm_radio_broadcasting = 5,
    telephone_connection_state = 6,
    spp_connection_state = 7,
    usb_slip_connection_state = 8,
    map_sd_insert_state = 9,
    dataservice_availability = 10,
    entune_availability = 11,
    map_connection_state = 12,
    register_pairing_state = 13
};

enum VR_MEDIA_CONNECT_STATUS
{
    usb_connect = 0,
    ipod_connect = 1,
    btaudio_connect = 2
};

enum VR_ENGINE_STATUS_CONTROL
{
    VR_Engine_Idle = 0,
    VR_Engine_Alexa_Activtity,
    VR_Engine_Baidu_Activtity,
    VR_Engine_Google_Activtity,
    VR_Engine_Dictation_Activtity,
    VR_Engine_Nuance_Activtity,
};

#endif /* VR_CONFIGUREDEF_H */
/* EOF */
