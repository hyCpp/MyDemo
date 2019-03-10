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
#ifndef GTEST_INCLUDE_GTEST_GTEST_H_
#    include "gtest/gtest.h"
#endif

#ifndef GMOCK_INCLUDE_GMOCK_GMOCK_H_
#    include "gmock/gmock.h"
#endif

#include "VP_ModuleIF.h"
#include "VP_ChannelManager_mock.h"
#include "NC_SubAudioManager_mock.h"
#include "VP_ReceiverCb.h"

using ::testing::Return;
using ::testing::AtLeast;
using ::testing::_;
using namespace std;

/**
 * Summary for class VP_ModuleIF_Test
 * 
 * This is a detail description for class VP_ModuleIF_Test
 *
 */
class VP_ModuleIF_Test : public testing::Test
{
public:
    virtual void SetUp()
    {
       m_pVP_ModuleIF = new(MEM_Voice) VP_ModuleIF();
    }

    virtual void TearDown() {}

public:
    VP_ModuleIF* m_pVP_ModuleIF;
};

TEST_F(VP_ModuleIF_Test, Start)
{
    m_pVP_ModuleIF->Start();
}

TEST_F(VP_ModuleIF_Test, Initialize)
{
//    shared_ptr<MockVP_ChannelManager> channelMgr = make_shared<MockVP_ChannelManager>();
//    m_pVP_ModuleIF->m_channelMgr = channelMgr;
//    EXPECT_CALL(*channelMgr, registerChannel(_)).Times(2).WillRepeatedly(Return(true));
//    m_pVP_ModuleIF->Initialize();
}

TEST_F(VP_ModuleIF_Test, Destory)
{
//  EXPECT_CALL(*channelMgr, registerChannel(_)).WillOnce(Return(true));
//  m_pVP_ModuleIF->Initialize();
    m_pVP_ModuleIF->Destory();
}

TEST_F(VP_ModuleIF_Test, Stop)
{
    // m_pVP_ModuleIF->m_pPlayControl = nullptr;
    m_pVP_ModuleIF->Stop();
}

/* EOF */
