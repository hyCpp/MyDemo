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
#include "VR_NCNetworkAccessAdaptor.h"
#include "VR_Macro.h"
#include "VR_Def.h"

#define RESOURCE_ID   "resouce#use#role"
#define WIFI_NET_TYPE "wifiSta"

TEST(VR_NCNetworkAccessAdaptor_test, OverAll)
{
    VR_NCNetworkAccessAdaptor networkAdapter(VR_new NCNetworkAccess());
    networkAdapter.setReplyReceiver(VR_new VR_CommuMediation::VR_CommuMediationNetReplier());
    networkAdapter.bindService();
    AccessMode mode;
    networkAdapter.request(RESOURCE_ID, WIFI_NET_TYPE, mode);
    networkAdapter.release(RESOURCE_ID);
    networkAdapter.unbindService();
}

/* EOF */
