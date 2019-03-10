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
 * @file VP_DataManager.h                                            
 * @brief Summary for VP_DataManager.
 *
 * This file includes the declaration of class VP_DataManager and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VP_DATAMANAGER_H
#define VP_DATAMANAGER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#pragma once

#ifndef CXX_CL_AUTOSYNC_H
#	include "CL_AutoSync.h"
#endif

#include <map>
#include <string>
#include <mutex>
#include <memory>
#include "VP_SoundFile.h"
#include "VP_LanguageSetting.h"

class BL_String;
class VP_SoundLangMap;

/**
 * Summary for class VP_DataManager
 *
 * There should be a detail description for
 * class VP_DataManager
 */
class VP_DataManager
{
public:
    VP_DataManager();
    VP_DataManager(std::shared_ptr<VP_LanguageSetting> languageSetting, std::shared_ptr<VP_SoundFile> soundFile);
    virtual ~VP_DataManager();

    CL_ERROR Initialize();

    virtual CL_ERROR GetGuidePhrase(DWORD phraseId, VP_Phrase& phrase, BL_String& filename);

    const std::string& GetLanguage() const;

    const std::string& GetGender() const;
    
    CL_ERROR LoadLanguage();

    CL_ERROR UnloadLanguage();

protected:
    std::mutex  m_mutex;
    std::shared_ptr<VP_SoundFile>	m_guideSound;
    std::shared_ptr<VP_LanguageSetting> m_languageSetting;
    BL_String m_guideSoundFileName;
    std::string m_language;
    std::string m_gender;

private:
    VP_DataManager(const VP_DataManager&);
    VP_DataManager& operator=(const VP_DataManager&);
};

#endif /* VP_DATAMANAGER_H */
/* EOF */
