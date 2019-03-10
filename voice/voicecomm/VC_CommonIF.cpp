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
#include "VC_CommonIF.h"
#include "VC_Factory.h"

#include "VC_AudioInGeneral.h"
#include "VC_AudioOutGeneral.h"
#include "VC_PlayerGeneral.h"
#include "VC_AudioSessionAlsa.h"

VC_CommonIF* VC_CommonIF::s_pInstance = NULL;

VC_CommonIF* VC_CommonIF::Instance()
{
    VC_LOGD_FUNC();
    if (NULL == s_pInstance) {
        s_pInstance = new(MEM_Voice) VC_CommonIF;
    }
    return s_pInstance;
}

void VC_CommonIF::Destroy()
{
    VC_LOGI("Destroy enter");
    if (NULL != s_pInstance) {
        delete s_pInstance;
    }
}

VC_CommonIF::VC_CommonIF() : m_pAudioSession(NULL), m_pAudioOut(NULL)
{
    VC_LOGD_FUNC();
}

VC_CommonIF::~VC_CommonIF()
{
    if (m_pAudioSession) {
        VC_LOGI("~VC_CommonIF()");
        delete m_pAudioSession;
        m_pAudioSession = NULL;
    }

    if (m_pAudioOut) {
        delete m_pAudioOut;
        m_pAudioOut = NULL;
    }
}

VC_AudioSession* VC_CommonIF::GetAudioSession(VC_MODULE_TYPE moduleType) 
{
    if (NULL == m_pAudioSession) {
        VC_Factory *factory = VC_Factory::Instance();
        if (factory) {
            m_pAudioSession = factory->CreateAudioSession(moduleType);
        }
    }
    return m_pAudioSession;
}

VC_AudioIn* VC_CommonIF::CreateAudioIn(bool mock)
{
    VC_LOGD_FUNC();
    return VC_Factory::Instance()->CreateAudioIn(mock);
}

VC_AudioOut* VC_CommonIF::CreateAudioOut(VC_AUDIO_TYPE audioType)
{
    VC_LOGD_FUNC();
    return VC_Factory::Instance()->CreateAudioOut(audioType);
}

VC_Player* VC_CommonIF::CreatePlayer(VC_AUDIO_TYPE audioType) 
{
    VC_LOGD_FUNC();
    return VC_Factory::Instance()->CreatePlayer(audioType);
}
    
VC_AudioOut* VC_CommonIF::GetAudioOut(VC_AUDIO_TYPE audioType)
{
    if (m_pAudioOut) {
        return m_pAudioOut;
    }
    m_pAudioOut = VC_Factory::Instance()->CreateAudioOut(audioType);
    return m_pAudioOut;

}
/* EOF */
