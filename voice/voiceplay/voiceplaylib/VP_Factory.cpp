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
#include "VP_Factory.h"

#include "VC_CommonIF.h"
#include "VC_CommonDef.h"
#include "VP_DataManager.h"
#include "VP_SoundPlayer.h"
using namespace std;

shared_ptr<VP_Factory> VP_Factory::s_pInstance = nullptr;
VP_Factory::InstanceHolder VP_Factory::instanceHolder;

shared_ptr<VP_Factory> VP_Factory::Instance()
{
    return s_pInstance;
}

VP_Factory::~VP_Factory()
{
    VC_CommonIF::Destroy();
}

shared_ptr<VP_DataManager> VP_Factory::AcquireDataMgr()
{
    if (nullptr == m_pDataMgr) {
        shared_ptr<VP_DataManager> pDataMgr = std::make_shared<VP_DataManager>();
        CL_ERROR err = (nullptr == pDataMgr ? APL_ERR_VP_NEW_ERROR : pDataMgr->Initialize());
        if (APL_ERR_NO_ERROR != err) {
            VP_DEBUG_ERROR("in VP_Factory: initialize data manager failed!\n");
            return nullptr;
        }
        m_pDataMgr = pDataMgr;
    }

    return m_pDataMgr;
}

shared_ptr<VP_VoicePlayControl> VP_Factory::AcquirePlayControl()
{
    if (nullptr == m_pControl) {
        shared_ptr<VP_VoicePlayControl> pControl = std::make_shared<VP_VoicePlayControl>(
                AcquireDataMgr(), AcquireSoundPlayer());
        if (nullptr == pControl) {
            VP_DEBUG_ERROR("in VP_Factory: allocating play control failed!\n");
            return nullptr;
        }
        m_pControl = pControl;
    }

    return m_pControl;
}

shared_ptr<VP_SoundPlayer> VP_Factory::AcquireSoundPlayer()
{
    // VP_SoundPlayer derives from BL_Thread, it will stop thread when calling destructor of base class
    if (nullptr == m_pSoundPlayer) {
        shared_ptr<VP_SoundPlayer> pSoundPlayer = std::make_shared<VP_SoundPlayer>(
                VC_CommonIF::Instance()->CreatePlayer(VC_AUDIO_SOUND));
        CL_ERROR err = (nullptr == pSoundPlayer ? APL_ERR_VP_NEW_ERROR : pSoundPlayer->Initialize());
        if (APL_ERR_NO_ERROR != err) {
            VP_DEBUG_ERROR("in VP_Factory: initialize sound player failed!\n");
            return nullptr;
        }
        m_pSoundPlayer = pSoundPlayer;
    }
    return m_pSoundPlayer;
}

shared_ptr<VP_DurationManager> VP_Factory::AcquireDurationManager()
{
    if (nullptr == m_pDurationManager) {
        m_pDurationManager = make_shared<VP_DurationManager>();
    }

    return m_pDurationManager;
}

/* EOF */
