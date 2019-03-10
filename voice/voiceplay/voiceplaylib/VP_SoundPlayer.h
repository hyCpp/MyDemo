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
 * @file VP_SoundPlayer.h                                            
 * @brief Summary for VP_SoundPlayer.
 *
 * This file includes the declaration of class VP_SoundPlayer and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VP_SOUNDPLAYER_H
#define VP_SOUNDPLAYER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#pragma once

#include <deque>
#include <mutex>

#ifndef CXX_CL_AUTOSYNC_H
#	include "CL_AutoSync.h"
#endif
#ifndef CXX_CL_WAITOBJ_H
#	include "CL_WaitObj.h"
#endif

#ifndef CXX_BL_THREAD_H
#	include "BL_Thread.h"
#endif


#include "VP_Factory.h"
#include "VP_Channel.h"
#include "VP_Define.h"
#include "VP_DataAccessorList.h"

#include "VC_CommonDef.h"
#include "VC_Player.h"
#include "VC_AudioSession.h"

// Forward declaration
class VP_DataAccessorList; 
class VP_DurationManager;
class VP_Channel;
class VP_VoicePlayControl;

/**
 * Summary for class VP_SoundPlayer
 * 
 * This is a detail description for class VP_SoundPlayer
 *
 */
class VP_SoundPlayer : protected BL_Thread, public VC_Player::Listener
{
public:
    explicit VP_SoundPlayer(VC_Player* pPlayer);
    virtual ~VP_SoundPlayer();

    CL_ERROR Initialize();

    VOID OnPlayComplete(CL_BOOL bSuccess);
    VOID OnPlayComplete(CL_BOOL bSuccess, int reqId);
    virtual VOID Init();

    virtual void StopPlayerThread();

    virtual void Play(std::shared_ptr<VP_DataAccessorList> pList);

    virtual VP_PLAYER_STATUS getPlayerStatus() const;

    virtual void setPlayerStatus(VP_PLAYER_STATUS status);

    virtual std::shared_ptr<VP_DataAccessorList> getPlayList() const;

    virtual void Stop();

    void onVpPlayEnd(std::shared_ptr<VP_DataAccessorList>& pList, VP_PLAY_RESULT playResult);

    void postResult(std::shared_ptr<VP_DataAccessorList> pList, VP_PLAY_RESULT playResult);

    void onTtsServiceDown();

protected:
    virtual void Run();
    void Play();

private:
    VP_SoundPlayer(const VP_SoundPlayer&);
    VP_SoundPlayer& operator=(const VP_SoundPlayer&);
    bool CreateStopMsg();
    int ReqIdCreater();
    void notifyStoppedState(std::shared_ptr<VP_DataAccessorList> pList);

private:
    volatile VP_PLAYER_STATUS               m_state;
    CL_BOOL                                 m_bIsTtsPlay;
    CL_BOOL                                 m_bResultState;
    VP_PLAY_RESULT                          m_playRes;
    int 					                m_currentReqId;
    static unsigned int		                m_reqId;

    CL_WaitObj 				                m_syncPlayEnd;

    VC_Player*                              m_pPlayer;
    std::shared_ptr<VP_DataAccessorList>    m_playList;
    std::mutex                              m_mutex;
};

#endif /* VP_SOUNDPLAYER_H */
/* EOF */
