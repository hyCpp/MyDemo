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

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "stdafx.h"
#include "TTS_Controler.h"
#include "TTS_PlaylistInfo.h"
#include "VC_CommonIF.h"
#include "TTS_EngineManager.h"
#include "TTS_Event.h"
#include "TTS_PlayThread.h"
#include "VC_AudioSession.h"
#include "VC_CommonIF.h"
#include "VC_CommonDef.h"
#include "FTLog.h"
#include "functionchecking/TestCaseID.h"
#include "TTS_PlayThread_test.h"
#include "TTS_PlayElementText.h"

using ::testing::_;
using ::testing::Return;

/**
* TTS_PlayThread_Test
*
* The class is just for TTS_PlayThread test.                                
*/

class TTS_PlayThread_Test:public testing::Test
{
    public:
        TTS_PlayThread m_pTTS_PlayThread;
};


TEST_F(TTS_PlayThread_Test, StopPlay_IDLE)
{
    m_pTTS_PlayThread.StopPlay(TTS_PLAYER_WAIT);
}

TEST_F(TTS_PlayThread_Test, StopPlay_PLAYING)
{
    m_pTTS_PlayThread.StopPlay(TTS_PLAYER_ABORT);
}

TEST_F(TTS_PlayThread_Test, StopPlay_has_stop)
{
    m_pTTS_PlayThread.m_stop = true;
    m_pTTS_PlayThread.StopPlay(TTS_PLAYER_ABORT);
}

TEST_F(TTS_PlayThread_Test, PlayReq)
{
    TTS_PlaylistInfo playlist("test", NULL);
    m_pTTS_PlayThread.PlayReq(&playlist);
}

TEST_F(TTS_PlayThread_Test, Play)
{
    TTS_PlaylistInfo playlist("test", NULL);
    m_pTTS_PlayThread.PlayReq(&playlist);

    m_pTTS_PlayThread.Play();
}

TEST_F(TTS_PlayThread_Test, Pause)
{
    m_pTTS_PlayThread.Pause(NULL);
}

TEST_F(TTS_PlayThread_Test, Resume)
{
    m_pTTS_PlayThread.Resume(NULL);
}

/* EOF */
