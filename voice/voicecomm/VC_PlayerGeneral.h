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
 * @file VC_PlayerGeneral.h                                            
 * @brief Summary for VC_PlayerGeneral.
 *
 * This file includes the declaration of class VC_PlayerGeneral and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VC_PLAYERGENERAL_H
#define VC_PLAYERGENERAL_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#pragma once

#include "BL_String.h"
#ifndef CXX_CL_SYNCOBJ_H
#   include "CL_SyncObj.h"
#endif
#ifndef CXX_CL_WAITOBJ_H
#	include "CL_WaitObj.h"
#endif
#ifndef CXX_BL_THREAD_H
#	include "BL_Thread.h"
#endif

#include "CL_List.h"
#include "VC_CommDef.h"
#include "VC_Player.h"
#include "VC_DecEvent.h"
#include "VC_Decoder.h"
#include "VC_AudioOut.h"

class VC_AudioBuffer;

/**
 * Summary for class VC_PlayerGeneral
 * 
 * This is a detail description for class VC_PlayerGeneral
 *
 */
class  VC_PlayerGeneral: public VC_Player, VC_Decoder::Listener, VC_AudioOut::Listener, protected BL_Thread
{
public:
    explicit VC_PlayerGeneral(VC_AUDIO_TYPE audioType);
    virtual ~VC_PlayerGeneral();
    virtual CL_ERROR Init();

    virtual CL_BOOL Play(const BL_String& fileName, LONG lOffset, LONG lSize, 
            VC_Player::Listener* pListener, int tokenid = 0, bool last_phrase = false);

    virtual CL_BOOL Play(const VOID* pBuf, LONG lLen, VC_Player::Listener* pListener, 
            int tokenid = 0, bool last_phrase = false);

    virtual CL_BOOL Play(const BL_String& ttsText, DWORD dwLanguage, 
            VC_Player::Listener* pListener, int tokenid = 0, bool last_phrase = false);

    virtual CL_BOOL Stop();
    virtual void StopPlayer();

    virtual INT GetVolume() const;
    virtual CL_ERROR SetVolume(INT vol);

    // Inherited from VC_Decoder::Listener
    virtual VC_AudioBuffer* OnDecBuffer();
    virtual VOID OnDecReady(VC_AudioBuffer *pBuf);

    // Inherited from VC_AudioOut::Listener
    virtual VOID OnAudioOutDone(VC_AudioBuffer* pBuffer);
    virtual VOID OnAudioOutComplete(CL_BOOL success);
    virtual DWORD GetDuration(const BL_String& text);
protected:
    virtual void Run();

protected:
    static const INT VC_AUDIO_BUF_COUNT = 16; // audio out buffer count
    enum STATE { ST_NONE = 0, ST_PENDING, ST_PLAYING, ST_WAITING }; 

    VC_AUDIO_TYPE m_audioType;
    volatile STATE m_state;

    CL_WaitObj m_waitStop;
    // AudioOut
    VC_AudioOut* m_pAudioOut;
    VC_AudioBuffer *m_buffers[VC_AUDIO_BUF_COUNT];
    int m_dwBufCount;
    // Decoders
    VC_Decoder* m_pDecoder;
    VC_Decoder* m_pDecoders[VC_DECODER_NUM];

    VC_Player::Listener* m_pListener;
    VC_DecEvent* m_pEvent;
    int m_eventType;

    int m_dwIndexR;
    int m_dwIndexW;
    volatile CL_BOOL m_bStop;
    volatile CL_BOOL m_bStopNormal;
    int m_iFragCount;
    int m_tokenId;

    // for test
    int m_dwDecodeLen;
    int m_dwPlayLen;

    bool m_bLastPhrase;     // mark whether we need to wait for a moment after draining

private:

    VC_PlayerGeneral(const VC_PlayerGeneral&);
    VC_PlayerGeneral& operator = (const VC_PlayerGeneral&);

    CL_BOOL Flush();
    VOID DoPlay();
    VOID InitPlayer();
    VOID CheckStop();

    // for beep
    CL_BOOL IsPCM(const VOID *buf);
    CL_ERROR DoDecodePcm(const BYTE *dataBuf, LONG leftSize, int tokenid);
};


#endif /* VC_PLAYERGENERAL_H */
/* EOF */
