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

#include "VP_DurationManager.h"
#include "BL_SystemLanguage.h"

VP_DurationManager::VP_DurationManager() 
{
}

VP_DurationManager::~VP_DurationManager()
{
}

INT VP_DurationManager::GetDuration(const BL_String &text)
{
    int lang = GetSystemLanguage();
    return GetDurationByLanguage(text, lang);
}

INT VP_DurationManager::GetDurationByLanguage(const BL_String &text, int lang)
{
    const XCHAR* p = text.GetString();
    if (NULL == p) {
        VP_DEBUG_ERROR("Tts content is null");
        return 0;
    }

    int iDuration = 0;
    switch (lang) {
    case BL_LANGUAGE_TYPE_SIMPLIFIED_CHINESE:
            iDuration = CalcDuration_MNC(p);
            break;
    case BL_LANGUAGE_TYPE_ENGLISH:
    case BL_LANGUAGE_TYPE_UKENGLISH:
            iDuration = CalcDuration_ENG(p);
            break;
    case BL_LANGUAGE_TYPE_THAI:
            iDuration = CalcDuration_THT(text);
            break;
    case BL_LANGUAGE_TYPE_INDONESIAN:
            iDuration = CalcDuration_IDI(text);
            break;
    case BL_LANGUAGE_TYPE_PORTUGUESE:
            iDuration = CalcDuration_PTP(text);
            break;
    case BL_LANGUAGE_TYPE_SPANISH:
            iDuration = CalcDuration_SPE(text);
            break;
    case BL_LANGUAGE_TYPE_JAPANESE:
    case BL_LANGUAGE_TYPE_MALAY:
    default:
            iDuration = CalcDuration_MNC(p);
            break;
    }
   
    return iDuration;
}

INT VP_DurationManager::GetSystemLanguage()
{
    int lang = BL_SystemLanguage::GetVoiceLanguage();
    VP_LOGD("get system language is %d", lang);
    return lang;
}

INT VP_DurationManager::CalcDuration_MNC(const XCHAR* p)
{
    int iDuration = 0;
    int flag = 0;
    int commas = 0;
    int periods = 0;
    int chars = 0;
    while (*p) {
        if ((*p & 0xc0) != 0x80) {
            if (',' == *p) {
                ++commas;
            } 
            else if ('.' == *p) {
                ++periods;
            } 
            else {
                if ('[' == *p) {
                    flag = 1;
                }
                else if (']' == *p) {
                    flag = 0;
                }
                else if (!flag) {
                    chars++;
                }
                else {
                }
            }
        }
        ++p;
    }

    iDuration = chars*204 + commas*820 + periods*630;
    return iDuration;
}

INT VP_DurationManager::CalcDuration_ENG(const XCHAR* p)
{
    VP_LOGD("CalcDuration_ENG() enter...........");
    int iDuration = 0;
    int phraseLen = strlen(p);
    iDuration = phraseLen * 1000 / 12.5;
    return iDuration;
}

INT VP_DurationManager::CalcDuration_THT(const BL_String &text)
{
    VP_LOGD("CalcDuration_THT() enter...........");
    int iDuration = 0;
    int phraseLen = text.GetLength();
    iDuration = phraseLen * 1000 / 28.4;
    return iDuration;
}

INT VP_DurationManager::CalcDuration_IDI(const BL_String &text)
{
    int phraseLen = text.GetLength();
    int duration = phraseLen * 1000 / 13.5;
    return duration;
}

INT VP_DurationManager::CalcDuration_PTP(const BL_String &text)
{
    int phraseLen = text.GetLength();
    int duration = phraseLen * 1000 / 10.9;
    return duration;
}

INT VP_DurationManager::CalcDuration_SPE(const BL_String &text)
{
    int phraseLen = text.GetLength();
    int duration = phraseLen * 1000 / 13.8;
    return duration;

}
/* EOF */
