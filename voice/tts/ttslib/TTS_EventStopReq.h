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
 * @file TTS_EventStopReq.h
 * @brief Declaration file of class TTS_EventStopReq.                  
 *
 * This file includes the declaration of class TTS_EventStopReq and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */
#pragma once

#ifndef CXX_TTS_EVENTSTOPREQ_H
#define CXX_TTS_EVENTSTOPREQ_H

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

#include "TTS_Event.h"

class TTS_PlayThread;
class TTS_PlaylistInfoManager;

/**
* TTS_EventStopReq
*
* The class is just for TTS_EventStopReq.
*/
class TTS_EventStopReq : public TTS_Event
{
public:
    TTS_EventStopReq(TTS_PlayThread *thread, TTS_PlaylistInfoManager* manager, 
            TTS_TextToSpeechImpl::PlaybackListener* listener, int reqID, const BL_String &sender, CL_BOOL stopAll);
    virtual ~TTS_EventStopReq();

    virtual void OnEvent();

protected:
    void Stop();
    void StopAll();    


private:
    TTS_EventStopReq(const TTS_EventStopReq&);
    TTS_EventStopReq& operator = (const TTS_EventStopReq&);

    TTS_PlayThread		*m_playThread;
    TTS_PlaylistInfoManager *m_manager;
    TTS_TextToSpeechImpl::PlaybackListener* m_listener;
    int						m_reqID;
    const BL_String			m_sender;
    CL_BOOL					m_stopAll;
};

#endif
/* EOF */
