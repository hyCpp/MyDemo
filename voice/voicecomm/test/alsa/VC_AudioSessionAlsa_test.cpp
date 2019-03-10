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
 * @file VC_IAudioOutMedia_Test                                         
 * @brief Declaration file of class VC_IAudioOutMedia_Test.                  
 *
 * This file includes the declaration of test googletest and googlemock.
 *
 * @attention used for C++ only.
 */

#include "VC_AudioSessionAlsa.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::Return;
using ::testing::_;

/**
* VC_AudioSessionAlsa_test
*
* The class is just for VC_AudioSessionAlsa test.
*/

/**
 * Summary for class VC_AudioSessionAlsa_test
 * 
 * This is a detail description for class VC_AudioSessionAlsa_test
 *
 */
class VC_AudioSessionAlsa_test : public testing::Test
{
    public:
        virtual void SetUp()
        {
            m_alsa = new VC_AudioSessionAlsa(11);
        }

        virtual void TearDown()
        {
            delete m_alsa;
            m_alsa = NULL;
        }

    public:
        VC_AudioSessionAlsa*   m_alsa;
};

TEST_F(VC_AudioSessionAlsa_test, ReqAudioOutOn)
{
    m_alsa->ReqAudioOutOn(1);
}

TEST_F(VC_AudioSessionAlsa_test, ReqAudioOutOff)
{
    m_alsa->ReqAudioOutOff(2);
}

TEST_F(VC_AudioSessionAlsa_test, SetChannelCloseByOSListener)
{
    m_alsa->SetChannelCloseByOSListener(NULL);
}

TEST_F(VC_AudioSessionAlsa_test, TurnAudioIn)
{
    EXPECT_TRUE(m_alsa->TurnAudioIn(true));
}

TEST_F(VC_AudioSessionAlsa_test, SendMsgForSourceActivity)
{
    m_alsa->SendMsgForSourceActivity();
}

TEST_F(VC_AudioSessionAlsa_test, SetSessionActive)
{
    m_alsa->SetSessionActive(true);
}

/* EOF */