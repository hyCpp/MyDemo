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
 * @file TTS_EventPauseReq.h 
 * @brief Declaration file of class TTS_Pause.
 *
 * This file includes the declaration of class TTS_EventPauseReq and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#pragma once

#ifndef CXX_TTS_EVENTPAUSEREQ_H
#define CXX_TTS_EVENTPAUSEREQ_H

#include "TTS_Event.h"

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

class TTS_PlayThread;
class TTS_PlaylsitInfoManager;
/**
* TTS_EventPauseReq
*
* The class is just for TTS_Pause.
*/
class TTS_EventPauseReq : public TTS_Event
{
public:
    TTS_EventPauseReq(TTS_PlayThread *thread, TTS_TextToSpeechImpl::PlaybackListener* listener, 
            TTS_PlaylistInfoManager* manager, int reqID, const BL_String &sender);
    virtual ~TTS_EventPauseReq();

    virtual void OnEvent();

protected:
    void Pause();

private:
    TTS_EventPauseReq(const TTS_EventPauseReq &);
    TTS_EventPauseReq& operator = (const TTS_EventPauseReq &);
    TTS_PlayThread		   *m_playThread;
    TTS_TextToSpeechImpl::PlaybackListener* m_listener;
    TTS_PlaylistInfoManager *m_manager;
    int						m_reqID;
    const BL_String			m_sender;
};

#endif /* CXX_TTS_EVENTPAUSEREQ_H */
/* EOF */
