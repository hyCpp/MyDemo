/**
 * Copyright @ 2015 - 2016 Suntec Software(Shanghai) Co., Ltd.
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
#include <iostream>
#include "VR_CommuMediation.h"
#include "VR_NCConnectAdaptor_mock.h"
#include "VR_NCNetworkAccessAdaptor_mock.h"
#include "VR_Macro.h"
#include "VR_Def.h"

using ::testing::Return;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::AnyNumber;
using ::testing::SetArgReferee;
using ::testing::InSequence;

using nutshell::NC_TRUE;
using nutshell::NC_FALSE;

#define RESOURCE_ID  "wireless#voice#centervr"
#define NET_TYPE     "wifiSta"
#define WIFI_NET_TYPE_FAKE "wifista"
#define REPLY_PERMIT "permit"
#define REPLY_REJECT "reject"
#define REPLY_WAIT   "wait"
#define BOOL2ERRORS(b) ((b) ? 0 : -1)

/**
* VR_CommuMediation_test.cpp
*
* Unit test class for VR_CommuMediation.
*/
class VR_CommuMediation_test : public testing::Test
{
public:
    VR_CommuMediation_test();
    void SetUp();
    void TearDown();
    void GotoNetworkReady();

protected:
    VR_NCConnectAdaptor_mock*                       m_iConnect;
    VR_NCNetworkAccessAdaptor_mock*                 m_iNetworkAccess;
    VR_CommuMediation::VR_CommuMediationReplier*    m_replier;
    VR_CommuMediation::VR_CommuMediationNetReplier* m_netReplier;
    boost::shared_ptr<VR_CommuMediation>            m_commuMediation;
    bool                                            m_bActiveRelease;

private:
    VR_DISALLOW_EVIL_CONSTRUCTORS(VR_CommuMediation_test);
};

static int OnStatusChanged(int status)
{
    return 0;
}

VR_CommuMediation_test::VR_CommuMediation_test()
: m_iConnect(VR_new NiceMock<VR_NCConnectAdaptor_mock>())
, m_iNetworkAccess(VR_new NiceMock<VR_NCNetworkAccessAdaptor_mock>())
, m_replier(VR_new VR_CommuMediation::VR_CommuMediationReplier())
, m_netReplier(VR_new VR_CommuMediation::VR_CommuMediationNetReplier())
, m_commuMediation(nullptr)
, m_bActiveRelease(true)
{
    m_commuMediation.reset(VR_new VR_CommuMediation(
          m_iConnect
        , m_iNetworkAccess
        , m_replier
        , m_netReplier));
}

void VR_CommuMediation_test::SetUp()
{
    ASSERT_NE(nullptr, m_iConnect);
    ASSERT_NE(nullptr, m_iNetworkAccess);
    ASSERT_NE(nullptr, m_replier);
    ASSERT_NE(nullptr, m_netReplier);
    ASSERT_NE(nullptr, m_commuMediation.get());

    ON_CALL(*m_iConnect, bindService()).WillByDefault(Return(NC_TRUE));
    ON_CALL(*m_iNetworkAccess, bindService()).WillByDefault(Return(NC_TRUE));

    // cannot invoke replyRequest ON_CALL(request), 
    // or replyRequest would be invoked before request returns.
    ON_CALL(*m_iConnect, request(_, _, _, _)).WillByDefault(SetArgReferee<3>(RESOURCE_ID)); 
    ON_CALL(*m_iConnect, request(_, _, _, _, _)).WillByDefault(SetArgReferee<4>(RESOURCE_ID));
    // EXPECT_CALL(*m_iConnect, bindService()).Times(AnyNumber()); // thanks to NiceMock
}

void VR_CommuMediation_test::TearDown()
{
    if (m_bActiveRelease) {
        m_commuMediation->Release();
        switch (m_commuMediation->GetState()) {
        case VR_CommuMediation::StateNetworkReady:
            m_netReplier->replyRelease(RESOURCE_ID, 0);
            EXPECT_EQ(VR_CommuMediation::StateConnectPermit, m_commuMediation->GetState());
            // release NCNetworkAccess first, then go on to release NCConnect
        case VR_CommuMediation::StateConnectPermit:
            m_replier->replyRelease(RESOURCE_ID, "succeed");
            EXPECT_EQ(VR_CommuMediation::StateConnectReject, m_commuMediation->GetState());
            break;
        case VR_CommuMediation::StateConnectWait:
            m_replier->replyStopWait(RESOURCE_ID);
            EXPECT_EQ(VR_CommuMediation::StateConnectWait, m_commuMediation->GetState());
            break;
        default:
            break;
        }
    }
}

void VR_CommuMediation_test::GotoNetworkReady()
{
    
    EXPECT_TRUE(m_commuMediation->Init());
    EXPECT_EQ(VR_CommuMediation::StateConnectReject, m_commuMediation->GetState());

    EXPECT_TRUE(m_commuMediation->Request());
    m_replier->replyRequest(RESOURCE_ID, NET_TYPE, REPLY_PERMIT);
    EXPECT_EQ(VR_CommuMediation::StateConnectPermit, m_commuMediation->GetState());

    m_netReplier->replyRequest(RESOURCE_ID, 0);
    EXPECT_EQ(VR_CommuMediation::StateNetworkReady, m_commuMediation->GetState());
}

TEST_F(VR_CommuMediation_test, Init_false1)
{
    EXPECT_CALL(*m_iConnect, bindService()).WillOnce(Return(NC_FALSE));
    EXPECT_FALSE(m_commuMediation->Init());
}

TEST_F(VR_CommuMediation_test, Init_false2)
{
    {
        InSequence seq;
        EXPECT_CALL(*m_iConnect, bindService());
        EXPECT_CALL(*m_iNetworkAccess, bindService()).WillOnce(Return(NC_FALSE));
        EXPECT_CALL(*m_iConnect, unbindService());
    }

    EXPECT_FALSE(m_commuMediation->Init());
}

TEST_F(VR_CommuMediation_test, Init_true)
{
    {
        InSequence seq;
        EXPECT_CALL(*m_iConnect, bindService());
        EXPECT_CALL(*m_iNetworkAccess, bindService());
    }

    EXPECT_EQ(VR_CommuMediation::StateUninitialized, m_commuMediation->GetState());
    EXPECT_TRUE(m_commuMediation->Init());
    EXPECT_EQ(VR_CommuMediation::StateConnectReject, m_commuMediation->GetState());
}

TEST_F(VR_CommuMediation_test, Deinit)
{
    {
        InSequence seq;
        EXPECT_CALL(*m_iConnect, bindService());
        EXPECT_CALL(*m_iNetworkAccess, bindService());
        EXPECT_CALL(*m_iNetworkAccess, unbindService());
        EXPECT_CALL(*m_iConnect, unbindService());
    }

    EXPECT_EQ(VR_CommuMediation::StateUninitialized, m_commuMediation->GetState());
    EXPECT_TRUE(m_commuMediation->Init());
    EXPECT_EQ(VR_CommuMediation::StateConnectReject, m_commuMediation->GetState());
    m_commuMediation->Deinit();
    EXPECT_EQ(VR_CommuMediation::StateUninitialized, m_commuMediation->GetState());
}

TEST_F(VR_CommuMediation_test, Request_permit_0)
{
    {
        InSequence seq;
        // EXPECT_CALL(*m_iConnect, bindService());
        // EXPECT_CALL(*m_iConnect, request(_, _, _, _));
        // EXPECT_CALL(*m_iNetworkAccess, bindService());
        // EXPECT_CALL(*m_iNetworkAccess, request(_, _, _));
    }
    // GotoNetworkReady();

    EXPECT_TRUE(m_commuMediation->Request());
    EXPECT_EQ(VR_CommuMediation::StateConnectReject, m_commuMediation->GetState());
}

TEST_F(VR_CommuMediation_test, Request_permit_0_OnStatusChanged)
{
    {
        InSequence seq;
        EXPECT_CALL(*m_iConnect, request(_, _, _, _));
        EXPECT_CALL(*m_iNetworkAccess, request(_, _, _));
    }
    boost::function<int(int)> func1 = OnStatusChanged;

    // m_commuMediation->SetOnStatusChanged((void *)OnStatusChanged);
    m_commuMediation->SetOnStatusChanged(func1);
    GotoNetworkReady();
}

TEST_F(VR_CommuMediation_test, Request_permit_n0)
{
    EXPECT_TRUE(m_commuMediation->Init());
    EXPECT_EQ(VR_CommuMediation::StateConnectReject, m_commuMediation->GetState());

    EXPECT_TRUE(m_commuMediation->Request());
    m_replier->replyRequest(RESOURCE_ID, NET_TYPE, REPLY_PERMIT);
    EXPECT_EQ(VR_CommuMediation::StateConnectPermit, m_commuMediation->GetState());

    m_netReplier->replyRequest(RESOURCE_ID, 1234);
    EXPECT_EQ(VR_CommuMediation::StateConnectPermit, m_commuMediation->GetState());
}

TEST_F(VR_CommuMediation_test, disconnectFlag)
{
    {
        InSequence seq;
        EXPECT_CALL(*m_iConnect, request(_, _, _, _));
        EXPECT_CALL(*m_iNetworkAccess, request(_, _, _));
        EXPECT_CALL(*m_iConnect, release(_));
    }

    EXPECT_TRUE(m_commuMediation->Init());
    EXPECT_EQ(VR_CommuMediation::StateConnectReject, m_commuMediation->GetState());
    EXPECT_TRUE(m_commuMediation->Request());
    m_replier->replyRequest(RESOURCE_ID, NET_TYPE, REPLY_PERMIT);
    EXPECT_EQ(VR_CommuMediation::StateConnectPermit, m_commuMediation->GetState());

    m_commuMediation->Release();
    m_netReplier->replyRequest(RESOURCE_ID, 1234);
    EXPECT_EQ(VR_CommuMediation::StateConnectPermit, m_commuMediation->GetState());

    m_replier->replyRelease(RESOURCE_ID, "succeed");
    EXPECT_EQ(VR_CommuMediation::StateConnectReject, m_commuMediation->GetState());

    m_replier->replyRequest(RESOURCE_ID, NET_TYPE, REPLY_REJECT);
    EXPECT_EQ(VR_CommuMediation::StateConnectReject, m_commuMediation->GetState());
}

TEST_F(VR_CommuMediation_test, disconnectFlag_replyWithoutRequest)
{
    GotoNetworkReady();
    m_commuMediation->Release();
    m_netReplier->replyRelease(RESOURCE_ID, 4321);
    EXPECT_EQ(VR_CommuMediation::StateConnectPermit, m_commuMediation->GetState());

    m_netReplier->replyRelease(RESOURCE_ID, 0);
    EXPECT_EQ(VR_CommuMediation::StateConnectPermit, m_commuMediation->GetState());

    m_replier->replyRelease(RESOURCE_ID, "failed");
    EXPECT_EQ(VR_CommuMediation::StateConnectPermit, m_commuMediation->GetState());

    m_replier->replyRelease(RESOURCE_ID, "succeed");
    EXPECT_EQ(VR_CommuMediation::StateConnectReject, m_commuMediation->GetState());
}

TEST_F(VR_CommuMediation_test, Request_reject)
{
    EXPECT_TRUE(m_commuMediation->Init());
    EXPECT_EQ(VR_CommuMediation::StateConnectReject, m_commuMediation->GetState());

    EXPECT_TRUE(m_commuMediation->Request());
    m_replier->replyRequest(RESOURCE_ID, WIFI_NET_TYPE_FAKE, REPLY_REJECT);
    EXPECT_EQ(VR_CommuMediation::StateConnectReject, m_commuMediation->GetState());
}

TEST_F(VR_CommuMediation_test, Request_waitPermit)
{
    EXPECT_TRUE(m_commuMediation->Init());
    EXPECT_EQ(VR_CommuMediation::StateConnectReject, m_commuMediation->GetState());

    EXPECT_TRUE(m_commuMediation->Request());
    m_replier->replyRequest(RESOURCE_ID, NET_TYPE, REPLY_WAIT);
    EXPECT_EQ(VR_CommuMediation::StateConnectWait, m_commuMediation->GetState());

    m_replier->replyWaitPermit(RESOURCE_ID);
    EXPECT_EQ(VR_CommuMediation::StateConnectReject, m_commuMediation->GetState());
}

TEST_F(VR_CommuMediation_test, Request_waitPermitAfterStopWait)
{
    EXPECT_TRUE(m_commuMediation->Init());
    EXPECT_EQ(VR_CommuMediation::StateConnectReject, m_commuMediation->GetState());

    EXPECT_TRUE(m_commuMediation->Request());
    m_replier->replyRequest(RESOURCE_ID, NET_TYPE, REPLY_WAIT);
    EXPECT_EQ(VR_CommuMediation::StateConnectWait, m_commuMediation->GetState());

     // trigger stopwait

    m_replier->replyStopWait(RESOURCE_ID);
    EXPECT_EQ(VR_CommuMediation::StateConnectWait, m_commuMediation->GetState());

    m_commuMediation->Request();
    m_replier->replyWaitPermit(RESOURCE_ID);
    EXPECT_EQ(VR_CommuMediation::StateConnectReject, m_commuMediation->GetState());

}

TEST_F(VR_CommuMediation_test, Request_stopwait)
{
    {
        InSequence seq;
        EXPECT_CALL(*m_iConnect, request(_, _, _, _));
        EXPECT_CALL(*m_iConnect, stopWait(_));
    }

    EXPECT_TRUE(m_commuMediation->Init());
    EXPECT_EQ(VR_CommuMediation::StateConnectReject, m_commuMediation->GetState());

    EXPECT_TRUE(m_commuMediation->Request());
    m_replier->replyRequest(RESOURCE_ID, NET_TYPE, REPLY_WAIT);
    EXPECT_EQ(VR_CommuMediation::StateConnectWait, m_commuMediation->GetState());

    m_replier->replyStopWait(RESOURCE_ID);
    EXPECT_EQ(VR_CommuMediation::StateConnectWait, m_commuMediation->GetState());
}

TEST_F(VR_CommuMediation_test, Release)
{
    {
        InSequence seq;
        EXPECT_CALL(*m_iConnect, request(_, _, _, _));
        EXPECT_CALL(*m_iNetworkAccess, request(_, _, _));
        EXPECT_CALL(*m_iNetworkAccess, release(_));
        EXPECT_CALL(*m_iConnect, release(_));
    }

    EXPECT_EQ(VR_CommuMediation::StateUninitialized, m_commuMediation->GetState());
    m_commuMediation->Release();
    EXPECT_EQ(VR_CommuMediation::StateUninitialized, m_commuMediation->GetState());

    GotoNetworkReady();
    m_commuMediation->Release();
    m_netReplier->replyRelease(RESOURCE_ID, 4321);
    EXPECT_EQ(VR_CommuMediation::StateNetworkReady, m_commuMediation->GetState());

    m_netReplier->replyRelease(RESOURCE_ID, 0);
    EXPECT_EQ(VR_CommuMediation::StateConnectPermit, m_commuMediation->GetState());

    m_replier->replyRelease(RESOURCE_ID, "failed");
    EXPECT_EQ(VR_CommuMediation::StateConnectPermit, m_commuMediation->GetState());

    m_replier->replyRelease(RESOURCE_ID, "succeed");
    EXPECT_EQ(VR_CommuMediation::StateConnectReject, m_commuMediation->GetState());
}

TEST_F(VR_CommuMediation_test, interrupt)
{
    {
        InSequence seq;
        EXPECT_CALL(*m_iConnect, request(_, _, _, _));
        EXPECT_CALL(*m_iConnect, replyInterrupt(_, _));
    }

    EXPECT_EQ(VR_CommuMediation::StateUninitialized, m_commuMediation->GetState());
    m_replier->requestInterrupt(RESOURCE_ID, "pre-empted"); // bypassed for empty m_resourceId

    EXPECT_TRUE(m_commuMediation->Init());
    EXPECT_EQ(VR_CommuMediation::StateConnectReject, m_commuMediation->GetState());
    EXPECT_TRUE(m_commuMediation->Request()); // issue request, to get m_resourceId
    m_replier->requestInterrupt(RESOURCE_ID, "pre-empted");
}

TEST_F(VR_CommuMediation_test, interrupt_Flag)
{
    {
        InSequence seq;
        EXPECT_CALL(*m_iConnect, request(_, _, _, _));
        EXPECT_CALL(*m_iNetworkAccess, request(_, _, _));
        EXPECT_CALL(*m_iNetworkAccess, release(_));
        EXPECT_CALL(*m_iConnect, release(_));
        EXPECT_CALL(*m_iConnect, replyInterrupt(_, _));
    }

    GotoNetworkReady();
    m_replier->requestInterrupt(RESOURCE_ID, "pre-empted");
    m_netReplier->replyRelease(RESOURCE_ID, 0);
    EXPECT_EQ(VR_CommuMediation::StateConnectPermit, m_commuMediation->GetState());
    m_replier->replyRelease(RESOURCE_ID, "succeed");
    EXPECT_EQ(VR_CommuMediation::StateConnectReject, m_commuMediation->GetState());
}

TEST_F(VR_CommuMediation_test, Connect)
{
    EXPECT_EQ(VR_CommuMediation::StateUninitialized, m_commuMediation->GetState());
    m_commuMediation->Connect();
    EXPECT_EQ(VR_CommuMediation::StateConnectReject, m_commuMediation->GetState());

    m_commuMediation->Connect();
    m_replier->replyRequest(RESOURCE_ID, NET_TYPE, REPLY_PERMIT);
    EXPECT_EQ(VR_CommuMediation::StateConnectPermit, m_commuMediation->GetState());

    m_commuMediation->Connect();
    m_netReplier->replyRequest(RESOURCE_ID, 0);
    EXPECT_EQ(VR_CommuMediation::StateNetworkReady, m_commuMediation->GetState());

    // EXPECT_EQ(BOOL2ERRORS(false), m_commuMediation->Connect()) << " while state is StateNetworkReady";
    EXPECT_TRUE(m_commuMediation->Connect());
    m_commuMediation->Disconnect();
    m_bActiveRelease = false; // try to release by DTOR_WAIT_TIMEOUT mechanism
}

/* EOF */
