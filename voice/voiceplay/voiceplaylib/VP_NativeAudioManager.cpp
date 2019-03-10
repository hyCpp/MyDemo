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
#include "VP_NativeAudioManager.h"
#include "VP_ChannelManager.h"

using namespace std;

VP_NativeAudioManager::VP_NativeAudioManager()
    :m_pListener(nullptr)
{
}

bool VP_NativeAudioManager::registerAvm(const string &audioName, VP_AudioManagerListener* listener)
{
    VP_LOGD("Request to register audio source %s", audioName.c_str());
    m_pListener = listener;

    return true;
}

bool VP_NativeAudioManager::unregister(const string &audioName)
{
    VP_LOGD("Request to unregister source %s", audioName.c_str());
    return true;
}

bool VP_NativeAudioManager::reqAudioSource(const string &audioName, bool on)
{
    VP_LOGD("Request to %s audio source %s", on ? "open" : "close", audioName.c_str());

    if (on) {
        m_pListener->notifyStatusChanged(audioName, "on");
    }
    else {
        m_pListener->notifyStatusChanged(audioName, "off");
    }

    return true;
}

bool VP_NativeAudioManager::cfmAudioStatus(const string &audioName, const std::string &status)
{
    VP_LOGD("Confirm audio source %s, status: %s", audioName.c_str(), status.c_str());
    return true;
}
