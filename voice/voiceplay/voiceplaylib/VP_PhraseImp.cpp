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

#include "VP_SoundPlayer.h"
#include "VP_Factory.h"
#include "VP_PhraseImp.h"
#include "VP_DataManager.h"
#include "VP_DataAccessorSound.h"
using namespace std;

// --------------------------------------------------
// VP_PhraseImp
// --------------------------------------------------
VP_PhraseImp::VP_PhraseImp()
    :m_isCancelled(CL_FALSE)
{
    memset(&m_phraseInfo, 0, sizeof(m_phraseInfo));
}

DWORD
VP_PhraseImp::GetDuration()
{
    if (IsCancelled()) {
        return 0;
    }
    return (m_phraseInfo.Time);
}

shared_ptr<VP_DataAccessor> VP_PhraseImp::CreateDataAccessor()
{
    VP_LOGI("Enter into VP_PhraseImp CreateDataAccessor\n");
    return nullptr;
}

// --------------------------------------------------
// VP_WaveIdPhrase
// --------------------------------------------------
VP_WaveIdPhrase::VP_WaveIdPhrase(DWORD id)
    :m_phraseId(id)
{
}

VP_WaveIdPhrase::~VP_WaveIdPhrase() 
{
}

CL_ERROR
VP_WaveIdPhrase::SetPhraseInfo(VP_DataManager &dataManager)
{
    CL_ERROR err = dataManager.GetGuidePhrase(m_phraseId, m_phraseInfo, m_filename);

    return err;
}

shared_ptr<VP_DataAccessor> VP_WaveIdPhrase::CreateDataAccessor()
{
    shared_ptr<VP_DataAccessorID> pAccessor = make_shared<VP_DataAccessorID>(
            m_filename, m_phraseInfo.Offset, m_phraseInfo.Size);
    if (nullptr == pAccessor) {
        VP_DEBUG_ERROR("DataAccessor can't be nullptr!");
        return nullptr;
    }

    return pAccessor;
}

CL_BOOL
VP_WaveIdPhrase::CanBeCancelled() const
{
    if (VP_WAVE_ID_CANCEL_1 == m_phraseId || VP_WAVE_ID_CANCEL_2 == m_phraseId || VP_WAVE_ID_CANCEL_3 == m_phraseId) {
        return CL_TRUE;
    } 
    else {
        VP_LOGI("VP_WaveIdPhrase::CanBeCancelled() id:%lu\n", m_phraseId);
        return CL_FALSE;
    }
}

// --------------------------------------------------
// VP_TtsPhrase
// --------------------------------------------------
VP_TtsPhrase::VP_TtsPhrase(const BL_String& ttsContent)
    : m_ttsContent(ttsContent) 
{
}

VP_TtsPhrase::~VP_TtsPhrase() 
{
}

DWORD VP_TtsPhrase::GetDuration()
{
    return VP_Factory::Instance()->AcquireDurationManager()->GetDuration(m_ttsContent.GetString());
}

void VP_TtsPhrase::setTTSLang(const std::string& lang)
{
    m_lang = lang;
}

const std::string& VP_TtsPhrase::getTTSLang() const
{
    return m_lang;
}

shared_ptr<VP_DataAccessor> VP_TtsPhrase::CreateDataAccessor()
{
    shared_ptr<VP_DataAccessorTts> pAccessor = make_shared<VP_DataAccessorTts>(m_ttsContent, m_lang);
    if (nullptr == pAccessor) {
        VP_DEBUG_ERROR("DataAccessor can't be nullptr");
        BL_ErrorLog(APL_ERR_VP_NEW_ERROR, BL_ERROR_NO_OPTION);
        return nullptr;
    }
    return pAccessor;
}


// --------------------------------------------------
// VP_WaveFilePhrase
// --------------------------------------------------
VP_WaveFilePhrase::VP_WaveFilePhrase(const std::string& fileName)
    : VP_PhraseImp(), m_pBuff(nullptr), m_length(0)
{
    m_filename = BL_String(fileName.c_str());
}

VP_WaveFilePhrase::~VP_WaveFilePhrase()
{
}

shared_ptr<VP_DataAccessor> VP_WaveFilePhrase::CreateDataAccessor()
{
    VP_LOGD_FUNC();
    shared_ptr<VP_DataAccessorBuf> pAccessor = make_shared<VP_DataAccessorBuf>(m_pBuff, m_length);

    if (nullptr == pAccessor) {
        VP_DEBUG_ERROR("DataAccessor can't be nullptr");
        BL_ErrorLog(APL_ERR_VP_NEW_ERROR, BL_ERROR_NO_OPTION);
        RETURN(nullptr);
    }

    RETURN(pAccessor);
}

CL_ERROR VP_WaveFilePhrase::SetPhraseInfo(VP_DataManager &dataManager)
{
    VP_LOGD_FUNC();
    FILE* file = fopen(m_filename.GetString(), "rb");
    if (!file) {
        VP_DEBUG_ERROR("fopen: %s fail, err code is %d", m_filename.GetString(), errno);
        RETURN(APL_ERR_VP_PHRASE_NOTEXIST);
    }

    fseek(file, 0, SEEK_END);
    m_length = ftell(file);
    fseek(file, 0, SEEK_SET);

    VP_LOGD("filename[%s], length[%d]", m_filename.GetString(), m_length);
    m_pBuff = new BYTE[m_length];
    if (nullptr == m_pBuff) {
        VP_DEBUG_ERROR("new byte[%d] failed!", m_length);
        RETURN(APL_ERR_VP_PHRASE_NOTEXIST);
    }

    int iRead = fread(m_pBuff, m_length, 1, file);
    if (1 != iRead) {
        VP_DEBUG_ERROR("read buffer failed! filename[%s]", m_filename.GetString());
        RETURN(APL_ERR_VP_PHRASE_NOTEXIST);
    }

    fclose(file);

    RETURN(APL_ERR_NO_ERROR);
}

/* EOF */
