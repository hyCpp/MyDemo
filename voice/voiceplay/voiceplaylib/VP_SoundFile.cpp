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

#include "BL_FilePublicDef.h"
#include "VP_SoundFile.h"

using namespace std;

const DWORD VP_SND_FLAG_INVALID = 0xFFFFFFFF;
const DWORD VP_SND_FLAG_CONFLICTID = 0xFFFFFFFE;
#define ALIGN4(x) (((x)+3) & (~3))

VP_SoundFile::VP_SoundFile()
    : soundFile(nullptr)
    , m_phraseCount(0)
    , m_pPhrases(nullptr)
    , m_rawPhrases(nullptr)
{
}

VP_SoundFile::~VP_SoundFile()
{
    if (nullptr != m_rawPhrases) {
        delete[] m_rawPhrases;
        m_rawPhrases = nullptr;
        VP_LOGD("Release all memory allocated by soundfile");
    }
}

CL_ERROR VP_SoundFile::Initialize(const BL_String& soundFileName)
{
    VP_LOGD_FUNC();
    if (nullptr == soundFile) {
        soundFile = make_shared<BL_NDataFile>();
    }
    VP_LOGI("fileName=%s", soundFileName.GetString());
    if (!soundFile->Open(soundFileName, BL_FILE_OPEN_MODE_R)) {
        BL_ErrorLog(APL_ERR_VP_FILE_OPEN, BL_ERROR_NO_OPTION);
        return APL_ERR_VP_FILE_OPEN;
    }

    // initialize data header
    DWORD dwReadSize = 0;
    VP_Header header;
    if (!soundFile->Read(&header, sizeof(VP_Header), &dwReadSize)) {
        BL_ErrorLog(APL_ERR_VP_FILE_READ, dwReadSize);
        VP_LOGD("sound file read head failed, dwReadSize=%d", dwReadSize);
        soundFile->Close();
        return APL_ERR_VP_FILE_READ;
    }

    DWORD phrase_size = sizeof(VP_Phrase) * header.phraseCount;
    m_rawPhrases = new(MEM_Voice) BYTE[phrase_size];
    BYTE* pPhraseBuf = m_rawPhrases;

    if (!soundFile->Seek(header.phraseOffset, BL_FILE_SEEK_SET)) {   // seek to the head of phrase
        soundFile->Close();
        VP_LOGD("sound file seek  failed");
        return APL_ERR_VP_FILE_READ;
    }

    if (!soundFile->Read(pPhraseBuf, sizeof(VP_Phrase) * header.phraseCount, &dwReadSize)) { // read all phrases
        soundFile->Close();
        VP_LOGD("sound file read phrase failed");
        return APL_ERR_VP_FILE_READ;
    }

    soundFile->Close();

    m_phraseCount = header.phraseCount;
    m_pPhrases = reinterpret_cast<VP_Phrase*>(pPhraseBuf);

    VP_LOGI("VP_SoundFile::Initialize ok\n");
    return APL_ERR_NO_ERROR;
}

CL_ERROR VP_SoundFile::UnloadSoundFile()
{
    delete[] m_rawPhrases;
    m_rawPhrases = nullptr;
    VP_LOGD("Unload the memory allocated by sound file!\n");

    return APL_ERR_NO_ERROR;
}

const VP_Phrase* VP_SoundFile::GetPhraseInfo(DWORD phraseId) const
{
    if (NULL == m_pPhrases) {
        VP_DEBUG_ERROR("VP_SoundFile::GetPhraseInfo, phrases is null\n");
        return NULL;
    }

    const VP_Phrase *pPhrase = this->GetPhraseObject(phraseId);
    if (NULL == pPhrase) {
        VP_DEBUG_ERROR("VP_SoundFile::GetPhraseInfo, invalid id=0x%lx\n", phraseId);
        return NULL;
    }

    if (VP_SND_FLAG_INVALID != pPhrase->sameNameFlag) {
        VP_DEBUG_ERROR("VP_SoundFile::GetPhraseInfo, invalid flag id=0x%lx\n", phraseId);
        return NULL;
    }

    return pPhrase;
}

const VP_Phrase*
VP_SoundFile::GetPhraseObject(DWORD phraseId) const
{
    const VP_Phrase* pPhrase = m_pPhrases;
    for (; pPhrase < m_pPhrases + m_phraseCount; ++pPhrase) {
        if (phraseId == pPhrase->id) {
            return pPhrase;
        }
    }
    VP_DEBUG_ERROR("VP_SoundFile::GetPhraseObject, count=%ld, id=0x%lx\n", m_phraseCount, phraseId);
    return NULL;
}

/* EOF */
