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
 * @file  TTS_PlayElementText
 * @brief Declaration file of class TTS_PlayElementText.
 *
 * This file includes the declaration of test class TTS_PlayElementText and
 * the definitions of the macros, struct, enum and so on.
 *
 * @attention used for C++ only.
 */

#pragma once

#ifndef CXX_TTS_PLAYELEMENTTEXT_H
#define CXX_TTS_PLAYELEMENTTEXT_H

#include "TTS_PlayElementBase.h"
#include "BL_String.h"
#include "TTS_IF.h"
#include "CL_SyncObj.h"
#include "CL_WaitObj.h"
#include "BL_SystemLanguage_DEF.h"
#include <stdio.h>

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

class TTS_PlayData;
class TTS_EngineManager;
class TTS_EngineBuffer;

/**
* TTS_PlayElementText
*
* The class is just for TTS_PlayElementText.
*/
class TTS_PlayElementText : public TTS_PlayElementBase 
{
public:
    TTS_PlayElementText(TTS_EngineManager* engine, const BL_String &text, const BL_String& sender, const BL_String& language, const BL_String& speakersex);
    virtual ~TTS_PlayElementText();

    const BL_String & GetPlayText() const
    {
        return m_text;
    }

    virtual CL_BOOL DoPlay(TTS_EngineBuffers* buffer);

    virtual int GetSampleRate() const;

private:
    TTS_PlayElementText(const TTS_PlayElementText&);
    TTS_PlayElementText & operator = (const TTS_PlayElementText&);

private:
    TTS_EngineManager   *m_engine;
    BL_String			m_text;
    BL_String           m_sender;
    BL_String           m_language;
    BL_String           m_speakersex;
};

#endif
/* EOF */
