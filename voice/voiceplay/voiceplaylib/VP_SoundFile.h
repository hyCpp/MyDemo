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
 * @file VP_SoundFile.h                                            
 * @brief Summary for VP_SoundFile.
 *
 * This file includes the declaration of class VP_SoundFile and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VP_SOUNDFILE_H
#define VP_SOUNDFILE_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#pragma once

#ifndef CXX_BL_STRING_H
#include "BL_String.h"
#endif

#include <memory>
#include "VP_Define.h"
#include "BL_NDataFile.h"

const DWORD VP_SNDF_ROADNAME_LEN = 128;

struct VP_Phrase
{
    DWORD id;   // id for this audio file
    DWORD sameNameFlag; 
    DWORD Offset;   // where the binary data located 
    DWORD Size;     // length for the binary data
    DWORD Time;     // time consumed by playing this phrase
};

// NameIdPair used to describe the relationship between id and name
struct VP_NameIdPair
{
    char	name[VP_SNDF_ROADNAME_LEN];
    DWORD	id;
};

struct VP_Header
{
    BYTE formatFlag[4];
    BYTE version[4];		// current version : 0101
    BYTE createDataTime[16];
    DWORD phraseOffset;
    DWORD phraseCount;      
    DWORD nameIdOffset;     // phraseCount * sizeof(VP_Phrase) + phraseOffset
    DWORD nameIdCount;
    DWORD soundOffset;
    DWORD soundCount;
};

struct VP_PhraseList
{
    VP_PhraseList()
    : count(0) 
    {
        for (DWORD i = 0; i < VP_MAX_PHRASE; ++i) {
            phrase[i] = NULL;
        }
    }

    VP_Phrase *&operator[] (int idx)
    {
        return phrase[idx];
    }

    DWORD GetTotalPlayTime() const
    {
        DWORD period = 0;
        for (DWORD i = 0; i < count; ++i) {
            period += phrase[i]->Time;
        }
        return period;
    }

    DWORD count;
    VP_Phrase *phrase[VP_MAX_PHRASE];   // VP_MAX_PHRASE = 20
};

/**
 * Summary for class VP_SoundFile
 *
 * There should be a detail description for
 * class VP_SoundFile
 */
class VP_SoundFile
{
public:
    VP_SoundFile();

    virtual ~VP_SoundFile();

    virtual CL_ERROR Initialize(const BL_String& soundFileName);

    virtual const VP_Phrase* GetPhraseInfo(DWORD phraseId) const;

    CL_ERROR UnloadSoundFile();

private:
    VP_SoundFile(const VP_SoundFile&);
    VP_SoundFile& operator=(const VP_SoundFile&);
    const VP_Phrase* GetPhraseObject(DWORD phraseId) const;

private:
    std::shared_ptr<BL_AbstractFile> soundFile;
    DWORD	m_phraseCount;
    VP_Phrase	*m_pPhrases;
    BYTE *m_rawPhrases;
};


#endif /* VP_SOUNDFILE_H */
/* EOF */
