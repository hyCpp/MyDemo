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
 * @file TTS_PlaylistInfo.h
 * @brief Declaration file of class TTS_PlaylistInfo.
 *
 * This file includes the declaration of class TTS_PlaylistInfo and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */
#pragma once

#ifndef CXX_TTS_TTSPLAYLISTIMPL_H
#define CXX_TTS_TTSPLAYLISTIMPL_H

#include "TTS_IF.h"
#include "CL_List.h"
#include "BL_String.h"
#include "TTS_PlayElementBase.h"
#include "TTS_IF.h"
#include "TTS_TextToSpeechImpl.h"
// #include "TTS_TuningManager.h"

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

class TTS_ResumeStrategyBase;
class TTS_Controler;

/**
* TTS_PlaylistInfo
*
* The class is just for TtsPlaylist.
*/
class TTS_PlaylistInfo 
{
public:
    TTS_PlaylistInfo(const BL_String &sender, TTS_TextToSpeechImpl* owner);
    TTS_PlaylistInfo(const BL_String &sender, const TTS_POLICY &policy, TTS_TextToSpeechImpl* owner);
    virtual ~TTS_PlaylistInfo();

    virtual CL_ERROR AddText(const BL_String& text, const BL_String& lang, const BL_String& speakersex);
    // virtual CL_ERROR AddText(const BL_String& text, const BL_String& language, const BL_String &filePath);
    virtual CL_ERROR AddSound(const BL_String& soundFileName);
    virtual CL_ERROR AddSound(int soundID);
    virtual CL_ERROR AddSilence(int millisecond);

    void SendPlayEnd(TTS_PLAYER_STATE state);
    void SendPlayState(TTS_PLAYER_STATE state, int playNum = -1);

    virtual TTS_PlayElementBase* GetPlayElement()
    {
        return m_playElementList.Pop();
    }

    virtual CL_List<TTS_PlayElementBase> & GetPlayElementList()
    {
        return m_playElementList;
    }

    virtual void SetRequestID(const int reqID)
    {
        m_reqID = reqID;
    }

    virtual int GetRequestId() const
    {
        return m_reqID;
    }

    virtual const BL_String& GetSender() const
    {
        return m_sender;
    }

    virtual void SetPolicy(const TTS_POLICY &policy)
    {
        m_policy = policy;
    }

    virtual const TTS_POLICY& GetPolicy()
    {
        return m_policy;
    }

    virtual void SetTokenID(const int tokenID)
    {
        m_tokenID = tokenID;
    }

    virtual int GetTokenID() const 
    {
        return m_tokenID;
    }

    virtual void SetCurrentPlay(int current)
    {
        m_currentElem = current;
    }

    virtual int GetCurrentPlay()
    {
        return m_currentElem;
    }


    virtual TTS_TextToSpeechImpl *GetOwner()
    {
        return m_owner;
    }

    virtual void SetStream(TTS_STREAM_DEVICE_TYPE stream)
    {
        m_stream = stream;
    }

    virtual TTS_STREAM_DEVICE_TYPE GetStream()
    {
        return m_stream;
    }

    virtual void SetDrainWait(bool drainWait) 
    {
        m_drainWait = drainWait;
    }

    virtual bool IsDrainWait() const
    {
        return m_drainWait;
    }

#if 0
    virtual void SetTuningManager(TTS_TuningManager *tuningManager)
    {
        m_tuningManager = tuningManager;
    }

    virtual TTS_TuningManager *GetTuningManager()
    {
        return m_tuningManager;
    }

#endif    


    void DoPlay();

private:
    TTS_PlaylistInfo(const TTS_PlaylistInfo&);
    TTS_PlaylistInfo& operator = (const TTS_PlaylistInfo&);

private:
    CL_List<TTS_PlayElementBase>	m_playElementList;
    int								m_reqID;
    BL_String						m_sender;
    int								m_currentElem;
    TTS_TextToSpeechImpl			*m_owner;
    TTS_POLICY						m_policy;
    TTS_STREAM_DEVICE_TYPE			m_stream;
    // TTS_TuningManager               *m_tuningManager;
    int                             m_tokenID;
    bool                            m_drainWait; // if wait when call audio stream's drain function.
};

#endif
/* EOF */
