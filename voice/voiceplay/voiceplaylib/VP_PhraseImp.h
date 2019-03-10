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
 * @file VP_PhraseImp.h                                            
 * @brief Summary for VP_PhraseImp.
 *
 * This file includes the declaration of class VP_PhraseImp and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VP_PHRASEIMP_H
#define VP_PHRASEIMP_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#pragma once

#include <memory>

#ifndef CXX_BL_STRING_H
#include "BL_String.h"
#endif

#include "VP_SoundFile.h"

struct VP_Phrase;
class VP_DataManager;
class VP_BufferManager;
class VP_DataAccessor;

/**
 * Summary for class VP_PhraseImp
 *
 * There should be a detail description for
 * class VP_PhraseImp
 */
class VP_PhraseImp
{
public:
    VP_PhraseImp();
    virtual ~VP_PhraseImp() {}

    virtual DWORD GetDuration();
    virtual CL_ERROR SetPhraseInfo(VP_DataManager &dataManager) 
    { 
        return APL_ERR_NO_ERROR; 
    }

    virtual std::shared_ptr<VP_DataAccessor> CreateDataAccessor();

    virtual CL_BOOL CanBeCancelled() const 
    { 
        return CL_FALSE; 
    }

    CL_BOOL IsCancelled() const 
    { 
        return m_isCancelled; 
    }

    void SetCancelled() 
    { 
        m_isCancelled = CL_TRUE; 
    }

protected:
    VP_Phrase m_phraseInfo;
    BL_String m_filename;
    CL_BOOL m_isCancelled;
};

/**
 * Summary for class VP_WaveIdPhrase
 * 
 * This is a detail description for class VP_WaveIdPhrase
 *
 */
class VP_WaveIdPhrase : public VP_PhraseImp
{
public:
    explicit VP_WaveIdPhrase(DWORD id);
    virtual ~VP_WaveIdPhrase();

    virtual CL_ERROR SetPhraseInfo(VP_DataManager &dataManager);
    virtual std::shared_ptr<VP_DataAccessor> CreateDataAccessor();
    virtual CL_BOOL CanBeCancelled() const;

private:
    const DWORD m_phraseId;
};

/**
 * Summary for class VP_TtsPhrase
 * 
 * This is a detail description for class VP_TtsPhrase
 *
 */
class VP_TtsPhrase : public VP_PhraseImp
{
public:
    explicit VP_TtsPhrase(const BL_String& ttsContent);
    virtual ~VP_TtsPhrase();

    virtual DWORD GetDuration();
    virtual std::shared_ptr<VP_DataAccessor> CreateDataAccessor();
    virtual void setTTSLang(const std::string& lang);
    virtual const std::string& getTTSLang() const;

private:
    const BL_String m_ttsContent;
    std::string m_lang;
};

/**
 * Summary for class VP_TtsPhrase
 * 
 * This is a detail description for class VP_TtsPhrase
 *
 */
class VP_WaveFilePhrase : public VP_PhraseImp
{
public:
    explicit VP_WaveFilePhrase(const std::string& fileName);
    virtual ~VP_WaveFilePhrase();

    virtual std::shared_ptr<VP_DataAccessor> CreateDataAccessor();
    virtual CL_ERROR SetPhraseInfo(VP_DataManager &dataManager);

private:
    VP_WaveFilePhrase(const VP_WaveFilePhrase&);
    VP_WaveFilePhrase& operator=(const VP_WaveFilePhrase&);

    BYTE* m_pBuff;
    DWORD m_length;
};


#endif /* VP_PHRASEIMP_H */
/* EOF */
