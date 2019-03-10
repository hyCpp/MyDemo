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
 * @file VP_Event.h                                            
 * @brief Summary for VP_Event.
 *
 * This file includes the declaration of class VP_Event and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VP_LANGUAGE_SETTING_H
#define VP_LANGUAGE_SETTING_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#pragma once
#include <string>

#include "MEM_map.h"
#include "BL_AplInfo.h"
#include "BL_SystemLanguage.h"
#include "BL_Setting.h"
#include "BL_NDataFile.h"

/**
 * class used to setting current language according to different region and country 
 *
 * This class is a class used to set language code using by TTS and VP module 
 */
class VP_LanguageSetting 
{
public:
    VP_LanguageSetting();
    const std::string& getLanguage() const;
    const std::string& getGender() const;
    const BL_String& getGuideSoundFileName() const;
    void Initialize();

private:
    void loadLanguage();
    void loadAllRegions();
    void loadGender();
    void setGender(int genderValue);
    void setGuideFileName();
    inline void loadChina();
    inline void loadIndia();
    inline void loadSouthEastAsia();
    inline void loadSouthAfrica();
    inline void loadTaiwan();
    inline void loadMiddleEast();
    inline void loadBrazil();
    inline void loadArgentina();
    inline void loadHK_Macau();
    inline void loadOceanian();

private:
    VoiceMap<int, VoiceMap<int, std::string>::type>::type m_languageTable;
    std::string m_currentLanguage;
    BL_String m_guideSoundFileName;
    std::string m_mark;
    std::string m_gender;
    const static XCHAR* VP_DATA_PATH;
};

#endif /* VP_LANGUAGE_SETTING_H */
/* EOF */
