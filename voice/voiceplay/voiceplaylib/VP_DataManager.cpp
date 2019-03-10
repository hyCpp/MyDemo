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

#ifndef CXX_BL_ERROR_H
#	include "BL_Error.h"
#endif

#ifndef CXX_BL_SYSTEMLANGUAGE_H
#   include "BL_SystemLanguage.h"
#endif

#include "VP_Define.h"
#include "VP_DataManager.h"
#include "VP_Factory.h"
using namespace std;

#define ALIGN4(x) (((x)+3) & (~3))

VP_DataManager::VP_DataManager() 
    : m_guideSound(make_shared<VP_SoundFile>()),
      m_languageSetting(make_shared<VP_LanguageSetting>())
{
}

VP_DataManager::VP_DataManager(shared_ptr<VP_LanguageSetting> languageSetting, 
        shared_ptr<VP_SoundFile> soundFile)
    : m_guideSound(soundFile),
      m_languageSetting(languageSetting)
{
}

VP_DataManager::~VP_DataManager() 
{
}

CL_ERROR VP_DataManager::Initialize()
{
    VP_LOGD_FUNC();
    if (nullptr == m_languageSetting || nullptr == m_guideSound) {
        return APL_ERR_VP_INVALID_VALUE;
    }

    CL_ERROR err = LoadLanguage();

    VP_LOGI("LoadSoundData ok\n");
    return err;
}

CL_ERROR VP_DataManager::LoadLanguage() 
{
    m_languageSetting->Initialize();

    m_language = m_languageSetting->getLanguage();
    m_gender = m_languageSetting->getGender();
    m_guideSoundFileName = m_languageSetting->getGuideSoundFileName();
    VP_LOGD("VP_DataManager initialize, lang = [%s], sound file = [%s]\n", 
            m_language.c_str(), m_guideSoundFileName.GetString());

    CL_ERROR err = m_guideSound->Initialize(m_guideSoundFileName);
    if (APL_ERR_NO_ERROR != err) {
        BL_APLINFO_REGION region = BL_AplInfo::GetRegion();
        int languageCode = BL_SystemLanguage::GetVoiceLanguage();
        int optData = (static_cast<int>(region) << 16) | languageCode;

        VP_ERROR(VOICE_VR_ERR_FILE_NOT_EXIST, optData, "LoadLanguage: load guide sound file:[%s], err=%ld\n", m_guideSoundFileName.GetString(), err);
    }

    return err;
}

CL_ERROR VP_DataManager::GetGuidePhrase(DWORD phraseId, VP_Phrase& phrase, BL_String& filename)
{
    const VP_Phrase* pPhrase = m_guideSound->GetPhraseInfo(phraseId);
    if (nullptr == pPhrase) {
        VP_DEBUG_ERROR("VP_DataManager::GetGuidePhrase failed, id=0x%lx\n", phraseId);
        return APL_ERR_VP_PHRASE_NOTEXIST;
    }

    // FIXED: Return pointer which address is not aligned by 4
    VP_Phrase* temp = const_cast<VP_Phrase*>(pPhrase);
    if (0 == (reinterpret_cast<intptr_t>(pPhrase) & 3)) {
        memcpy(&phrase, reinterpret_cast<void*>(temp), sizeof(phrase));
    } 
    else {
        VP_LOGI("SetGuidePhrase, phrase addr not align 4 byte, ptr=%p\n", pPhrase);
        phrase.id = pPhrase->id;
        phrase.sameNameFlag = pPhrase->sameNameFlag;
        phrase.Offset = pPhrase->Offset;
        phrase.Size = pPhrase->Size;
        phrase.Time = pPhrase->Time;
    }

    filename = m_guideSoundFileName; 

    return APL_ERR_NO_ERROR;
}

CL_ERROR VP_DataManager::UnloadLanguage()
{
    VP_LOGD_FUNC();
    m_guideSound->UnloadSoundFile();

    RETURN(APL_ERR_NO_ERROR);
}

const string& VP_DataManager::GetLanguage() const
{
    return m_language;
}

const string& VP_DataManager::GetGender() const
{
    return m_gender;
}

/* EOF */
