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
#include "VP_LanguageSetting.h"
using namespace std;

const XCHAR* VP_LanguageSetting::VP_DATA_PATH = XTEXT("VP/");
VP_LanguageSetting::VP_LanguageSetting()
{
}

void VP_LanguageSetting::Initialize()
{
    loadAllRegions();
    loadLanguage();
    loadGender();
}

void VP_LanguageSetting::loadLanguage() 
{
    BL_APLINFO_REGION region = BL_AplInfo::GetRegion();

    BL_String language;
    int languageCode = BL_SystemLanguage::GetVoiceLanguage();

    m_currentLanguage = m_languageTable[region][languageCode];
    if (m_currentLanguage.empty()) {
        int optData = (static_cast<int>(region) << 16) | languageCode;
        VP_ERROR(VOICE_VR_ERR_INVALID_PARAM, optData, "Region or language information is wrong, region code: [%d], language code: [%d]!\n", region, languageCode);
    }
}

void VP_LanguageSetting::loadGender()
{
    int gender = -1;

    BL_SettingData data;
    BL_Setting setting;
    if (setting.GetValue(BL_SETTING_KEY_NAVI_VOICETYPETTS, data)) {
        if (data.GetSize() == sizeof(INT)) {
            memcpy(&gender, data.GetData(), data.GetSize());
        }
    }
    VP_LOGD("Gender is: [%d]\n", gender);
    setGender(gender);
}

void VP_LanguageSetting::setGender(int genderValue)
{
    if (genderValue == 0) {
        m_gender = "female";
        m_mark = "F";
    }
    else if (genderValue == 1) {
        m_gender = "male";
        m_mark = "M";
    }
    else {
        m_mark = m_gender = "";
    }

    setGuideFileName();
}

void VP_LanguageSetting::setGuideFileName()
{ 
    BL_String fileName;
    fileName.Format("%sIG%s%s.SND", VP_DATA_PATH, m_mark.c_str(), m_currentLanguage.c_str());

    BL_NDataFile soundFile;
    if (!soundFile.FileExists(fileName)) {
        m_mark = m_gender = "";
        fileName.Format("%sIG%s.SND", VP_DATA_PATH, m_currentLanguage.c_str());
    }

    m_guideSoundFileName = fileName;
}

const string& VP_LanguageSetting::getLanguage() const
{
    return m_currentLanguage;
}

const string& VP_LanguageSetting::getGender() const
{
    return m_gender;
}

const BL_String& VP_LanguageSetting::getGuideSoundFileName() const
{
    return m_guideSoundFileName;
}

void VP_LanguageSetting::loadAllRegions()
{
    loadChina();
    loadIndia();
    loadSouthEastAsia();
    loadSouthAfrica();
    loadTaiwan();
    loadMiddleEast();
    loadBrazil();
    loadArgentina();
    loadHK_Macau();
    loadOceanian();
}

void VP_LanguageSetting::loadChina()
{
    VoiceMap<int, string>::type china;

    china.insert(make_pair(BL_LANGUAGE_TYPE_TRADITIONAL_CHINESE, "CAH"));
    china.insert(make_pair(BL_LANGUAGE_TYPE_SIMPLIFIED_CHINESE, "MNC"));
    china.insert(make_pair(BL_LANGUAGE_TYPE_ENGLISH, "ENU"));

    m_languageTable[BL_APLINFO_REGION_CHINA] = china;
}

void VP_LanguageSetting::loadIndia()
{
    VoiceMap<int, string>::type india;

    india.insert(make_pair(BL_LANGUAGE_TYPE_ENGLISH, "ENI"));
    india.insert(make_pair(BL_LANGUAGE_TYPE_HINDI, "HII"));
    india.insert(make_pair(BL_LANGUAGE_TYPE_PUNJABI, "PJB"));
    india.insert(make_pair(BL_LANGUAGE_TYPE_TAMIL, "TML"));
    india.insert(make_pair(BL_LANGUAGE_TYPE_MALAYALAM, "MYM"));
    india.insert(make_pair(BL_LANGUAGE_TYPE_TELUGU, "TLG"));
    india.insert(make_pair(BL_LANGUAGE_TYPE_KANNADA, "KND"));
    india.insert(make_pair(BL_LANGUAGE_TYPE_BENGALI, "BGL"));
    india.insert(make_pair(BL_LANGUAGE_TYPE_GUJARATI, "GRT"));
    india.insert(make_pair(BL_LANGUAGE_TYPE_MARATE, "MRT"));

    m_languageTable[BL_APLINFO_REGION_IN] = india;
}

void VP_LanguageSetting::loadSouthEastAsia()
{
    VoiceMap<int, string>::type southeast_asia;

    southeast_asia.insert(make_pair(BL_LANGUAGE_TYPE_THAI, "THT"));
    southeast_asia.insert(make_pair(BL_LANGUAGE_TYPE_INDONESIAN, "IDI"));
    southeast_asia.insert(make_pair(BL_LANGUAGE_TYPE_ENGLISH, "ENG"));
    southeast_asia.insert(make_pair(BL_LANGUAGE_TYPE_VIETNAMESE, "VTN"));
    southeast_asia.insert(make_pair(BL_LANGUAGE_TYPE_MALAY, "MLY"));
    m_languageTable[BL_APLINFO_REGION_SEA] = southeast_asia;
}

void VP_LanguageSetting::loadSouthAfrica()
{
    VoiceMap<int, string>::type south_africa;

    south_africa.insert(make_pair(BL_LANGUAGE_TYPE_ENGLISH, "ENG"));
    m_languageTable[BL_APLINFO_REGION_SA] = south_africa;
}

void VP_LanguageSetting::loadTaiwan()
{
    VoiceMap<int, string>::type taiwan;

    taiwan.insert(make_pair(BL_LANGUAGE_TYPE_TRADITIONAL_CHINESE, "MNT"));
    m_languageTable[BL_APLINFO_REGION_TW] = taiwan;
}

void VP_LanguageSetting::loadMiddleEast()
{
    VoiceMap<int, string>::type middle_east;
    
    middle_east.insert(make_pair(BL_LANGUAGE_TYPE_ARABIC, "ARW"));
    middle_east.insert(make_pair(BL_LANGUAGE_TYPE_ENGLISH, "ENG"));
    m_languageTable[BL_APLINFO_REGION_QG] = middle_east;
}

void VP_LanguageSetting::loadBrazil()
{
    VoiceMap<int, string>::type brazil;

    brazil.insert(make_pair(BL_LANGUAGE_TYPE_ENGLISH, "ENG"));
    brazil.insert(make_pair(BL_LANGUAGE_TYPE_PORTUGUESE, "PTB"));
    brazil.insert(make_pair(BL_LANGUAGE_TYPE_SPANISH, "SPE"));
    m_languageTable[BL_APLINFO_REGION_BR] = brazil;
}

void VP_LanguageSetting::loadArgentina()
{
    VoiceMap<int, string>::type argentina;

    argentina.insert(make_pair(BL_LANGUAGE_TYPE_ENGLISH, "ENG"));
    argentina.insert(make_pair(BL_LANGUAGE_TYPE_PORTUGUESE, "PTB"));
    argentina.insert(make_pair(BL_LANGUAGE_TYPE_SPANISH, "SPE"));
    m_languageTable[BL_APLINFO_REGION_AR] = argentina;
}

void VP_LanguageSetting::loadHK_Macau()
{
    VoiceMap<int, string>::type hk_macau;

    hk_macau.insert(make_pair(BL_LANGUAGE_TYPE_ENGLISH, "ENG"));
    hk_macau.insert(make_pair(BL_LANGUAGE_TYPE_TRADITIONAL_CHINESE, "CAH"));
    hk_macau.insert(make_pair(BL_LANGUAGE_TYPE_SIMPLIFIED_CHINESE, "MNC"));
    m_languageTable[BL_APLINFO_REGION_HK] = hk_macau;
}

void VP_LanguageSetting::loadOceanian()
{
    VoiceMap<int, string>::type oceanian;

    oceanian.insert(make_pair(BL_LANGUAGE_TYPE_ENGLISH, "ENA"));
    m_languageTable[BL_APLINFO_REGION_OC] = oceanian;
}

/* EOF */
