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
 * @file TTS_ReceiverCb.h
 * @brief Declaration file of class TTS_ReceiverCb.                  
 *
 * This file includes the declaration of class TTS_ReceiverCb and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#ifndef _CXX_TTS_RECEVER_CB_H_
#define _CXX_TTS_RECEVER_CB_H_

#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventReceiver.h"
#include "TTS_IF.h"
#include "MEM_map.h"
#include <string>
#include "navi/Voice/TTS/TtsPlayList.pb.h"
#include "TTS_TextToSpeechImpl.h"

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

class TTS_PlayListener;

/**
* TTS_ReceiverCb
*
* The class is just for TTS_ReceiverCb.
*/
class TTS_ReceiverCb
{
public:
    TTS_ReceiverCb();
    virtual ~TTS_ReceiverCb();

    virtual void PreInitialize();
    virtual void Initialize();
    virtual void Start();
    virtual void Stop();
    virtual void Destory();

    void LoadEngine(); 


    virtual void Register(EV_EventReceiver* er);                                                                               
    void OnRecvPlayText(void* msg);
    void OnRecvPlayFile(void* msg);
    void OnRecvPlayList(void* msg);
    void OnRecvPlayId(void* msg);
    void OnRecvStop(void* msg);
    void OnRecvSetParam(void* msg);
    void OnRecvGetParam(void* msg);
    void OnRecvListRole(void* msg);
    void OnRecvGetDefaultRole(void* msg) 
    {
    }

    void OnRecvListParamRange(void* msg)
    {
    }

    void OnRecvRegisterSound(void* msg);
    void OnRecvUnRegisterSound(void* msg);
    // void OnRecvGetMaxInputLength(void *msg);
    void OnRecvSynthesizeToFile(void *msg);
    void OnRecvServiceStatus(void *msg);
    void OnRecvPause(void *msg);
    void OnRecvResume(void *msg);

    void OnRecvRegisterTuningData(void *msg) 
    {
    }

    static void SendMessage(EV_EVENT_ID msgId, google::protobuf::MessageLite* msg);

private:
    void SetPlaylistElement(TTS_PlaylistInfo *playlist, const BL_String& language, const BL_String& speakersex, const ::navi::TTS::TtsPlayList_Phrase& phrase);

private:

    TTS_ReceiverCb(const TTS_ReceiverCb&);
    TTS_ReceiverCb& operator = (const TTS_ReceiverCb&);

    TTS_PlayListener    *m_listener;
    TTS_TextToSpeechImpl *m_tts;
};

/**
* TTS_PlayListener
*
* The class is just for TTS_ReceiverCb.
*/
class TTS_PlayListener : public TTS_TextToSpeechImpl::PlaybackListener
{
public:
    TTS_PlayListener();
    virtual ~TTS_PlayListener();
    virtual void OnTtsSpeechCompleted(const BL_String& sender, int reqId, TTS_PLAY_RESULT result);
    // virtual void OnRegisterTuningDataOver(const BL_String& sender, TTS_REGISTER_RESULT result);
    virtual void OnPlayStateOver(const BL_String& sender, int reqId, TTS_PLAYER_STATE state, int playNum);

private:

    TTS_PlayListener(const TTS_PlayListener&);
    TTS_PlayListener& operator = (const TTS_PlayListener&);

    TTS_ReceiverCb *m_receiver;
};

#endif /* _CXX_TTS_RECEVER_CB_H_ */
/* EOF */
