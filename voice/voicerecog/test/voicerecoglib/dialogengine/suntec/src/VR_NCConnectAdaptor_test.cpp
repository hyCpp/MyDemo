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
#include "VR_CommuMediation.h"
#include "VR_NCConnectAdaptor.h"
#include "VR_Macro.h"
#include "VR_Def.h"

using namespace nutshell;

#define RESOURCE_NAME "wireless"
#define USE_NAME      "voice"
#define ROLE_NAME     "centervr"
#define WIFI_NET_TYPE "wifiSta"

/**
* VR_NCConnectAdaptor_test.cpp
*
* Unit test class for VR_NCConnectAdaptor.
*/
class VR_NCConnectAdaptor_test : public testing::Test
{
public:
    VR_NCConnectAdaptor_test();
    virtual ~VR_NCConnectAdaptor_test();
    void SetUp();
    void TearDown();

protected:
    VR_NCConnectAdaptor connectAdapter;

private:
    VR_DISALLOW_EVIL_CONSTRUCTORS(VR_NCConnectAdaptor_test);
};

VR_NCConnectAdaptor_test::VR_NCConnectAdaptor_test()
    : connectAdapter(VR_new NCConnect())
{
}

VR_NCConnectAdaptor_test::~VR_NCConnectAdaptor_test()
{
}

void VR_NCConnectAdaptor_test::SetUp()
{
    connectAdapter.setReplyReceiver(VR_new VR_CommuMediation::VR_CommuMediationReplier());
    connectAdapter.bindService();
}

void VR_NCConnectAdaptor_test::TearDown()
{
    connectAdapter.unbindService();
}

TEST_F(VR_NCConnectAdaptor_test, Request_s)
{
    NCString resourceID;
    connectAdapter.request(RESOURCE_NAME, USE_NAME, ROLE_NAME, resourceID);
    connectAdapter.release(resourceID);
}

TEST_F(VR_NCConnectAdaptor_test, Request_l)
{
    NCString resourceID;
    connectAdapter.request(RESOURCE_NAME, USE_NAME, ROLE_NAME, WIFI_NET_TYPE, resourceID);
    connectAdapter.stopWait(resourceID);
}

TEST_F(VR_NCConnectAdaptor_test, Interrupt)
{
    connectAdapter.replyInterrupt("resouce#use#role", "succeed");
}

/* EOF */
