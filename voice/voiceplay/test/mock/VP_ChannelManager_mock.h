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
 * @file VP_ChannelManager_mock.h                                            
 * @brief Summary for MockVP_ChannelManager.
 *
 * This file includes the declaration of class MockVP_ChannelManager and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VP_CHANNELMANAGER_MOCK_H
#define VP_CHANNELMANAGER_MOCK_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <string>
#include <list>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "VP_ChannelManager.h"
#include "VP_Channel.h"

/**
 * This is a mock class for VP_PlayerStoppedEvent 
 *
 * This class is used to mock a object for class 
 * VP_PlayerStoppedEvent
 */
class MockVP_ChannelManager : public VP_ChannelManager
{
public:
    MOCK_METHOD1(registerChannel, bool(const std::string& channelName));
    MOCK_METHOD1(unregisterChannel, bool(const std::string& channelName));
    MOCK_CONST_METHOD1(getChannel, std::shared_ptr<VP_Channel>(const std::string& channelName));
    MOCK_METHOD3(confirmAudioStatus, void(const std::string& sourceName, const std::string& currentSource, bool reqAudio));  
    MOCK_METHOD1(queryAudioStatus, void(const std::string& currentSource));
};

#endif
/* EOF */
