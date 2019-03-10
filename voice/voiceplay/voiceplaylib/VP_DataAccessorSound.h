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
 * @file VP_DataAccessorSound.h                                            
 * @brief Summary for VP_DataAccessorID.
 *
 * This file includes the declaration of class VP_DataAccessorID and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VP_DATAACCESSORSOUND_H
#define VP_DATAACCESSORSOUND_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#pragma once

#ifndef CXX_BL_STRING_H
#	include "BL_String.h"
#endif

#include "VC_Player.h"

#include "CL_SyncObj.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "navi/Voice/TTS/TtsPlayText.pb.h"
#include "navi/Voice/TTS/TtsBasePlay.pb.h"

class EV_EventReceiver;
class VP_VoicePlayControl;

/**
 * Data Accessor means a place to store sound data 
 *
 * VP_DataAccessor is a pure abstract class and it has three sub-classes: 
 * VP_DataAccessorID, VP_DataAccessorBuf, VP_DataAccessorTts 
 */
class VP_DataAccessor
{
public:
    virtual ~VP_DataAccessor() {}

    virtual CL_ERROR Play(VC_Player* pPlayer, VC_Player::Listener *pListener, 
            CL_BOOL& isTtsPlay, int reqId, int tokenid) = 0;

    void setLastPhrase(bool lastPhrase)
    {
        m_bLastPhrase = lastPhrase;
    }

    bool isLastPhrase() const
    {
        return m_bLastPhrase;
    }

    void setPlayControl(std::shared_ptr<VP_VoicePlayControl> playControl)
    {
        m_pControl = playControl;
    }

    std::shared_ptr<VP_VoicePlayControl> getPlayControl() const
    {
        return m_pControl;
    }

    virtual void playFinished()
    {
    }

protected:
    CL_SyncObj  m_lock;
    bool        m_bLastPhrase;
    std::shared_ptr<VP_VoicePlayControl> m_pControl;
};

/**
 * this class is used to operate the wave file 
 *
 * DataAccessorID is used for wave file because of a relationship between ID and wave file 
 */
class VP_DataAccessorID : public VP_DataAccessor
{
public:
    VP_DataAccessorID(const BL_String& strFileName, DWORD dwOffset, DWORD dwSize)
        :m_strFileName(strFileName), m_dwOffset(dwOffset), m_dwSize(dwSize) 
    {
    }
    
    virtual ~VP_DataAccessorID() {}

    virtual CL_ERROR Play(VC_Player* pPlayer, VC_Player::Listener *pListener, 
            CL_BOOL& isTtsPlay, int reqId, int tokenid);

protected:
    BL_String	m_strFileName;
    DWORD		m_dwOffset;
    DWORD		m_dwSize;
};

/**
 * this class is used to operate binary sound data in memory 
 * 
 * DataAccessorBuf is used to supply a way to access sound data in memory 
 *
 */
class VP_DataAccessorBuf : public VP_DataAccessor
{
public:
    VP_DataAccessorBuf(const BYTE* pBuf, DWORD dwSize)
        :m_dwSize(dwSize), m_pBuf(pBuf) 
    {
    }

    virtual ~VP_DataAccessorBuf()
    {
        // delete m_pBuf;
        // m_pBuf = nullptr;
    }
    
    virtual CL_ERROR Play(VC_Player* pPlayer, VC_Player::Listener *pListener, 
            CL_BOOL& isTtsPlay, int reqId, int tokenid);

    virtual void playFinished();

protected:
    DWORD		m_dwSize;
    const BYTE* m_pBuf;

private:
    VP_DataAccessorBuf(const VP_DataAccessorBuf&);
    VP_DataAccessorBuf& operator=(const VP_DataAccessorBuf&);
};

/**
 * this class is used to operate TTS data 
 * 
 * DataAccessorTts is used to supply a way to operate TTS data 
 *
 */
class VP_DataAccessorTts : public VP_DataAccessor
{
public:
    explicit VP_DataAccessorTts(const BL_String& strTtsText, const std::string& lang)
        : m_strTtsText(strTtsText) 
        , m_ttsLang(lang)
    {
    }

    virtual ~VP_DataAccessorTts() {}
    
    virtual CL_ERROR Play(VC_Player* pPlayer, VC_Player::Listener *pListener, 
            CL_BOOL& isTtsPlay, int reqId, int tokenid);

    virtual void SendMessage(EV_EVENT_ID msgId, EV_MSG_PTR msg);
    virtual ::navi::TTS::TtsPlayText* CreateMessage(int reqId, int tokenid);

protected:
    BL_String	m_strTtsText;
    std::string m_ttsLang;
};

/**
 * this class is used to operate specified wave file 
 * 
 * VP_DataAccessorWaveFile is used to supply a way to operate specified wave file
 *
 */
class VP_DataAccessorWaveFile : public VP_DataAccessor
{
public:
    explicit VP_DataAccessorWaveFile(const BL_String& strFileName)
        : m_strFileName(strFileName) 
    {
    }

    virtual ~VP_DataAccessorWaveFile() {}
    
    virtual CL_ERROR Play(VC_Player* pPlayer, VC_Player::Listener *pListener, 
            CL_BOOL& isTtsPlay, int reqId, int tokenid);

protected:
    BL_String   m_strFileName;
};


#endif /* VP_DATAACCESSORSOUND_H */
/* EOF */
