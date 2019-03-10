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

#include    "stdafx.h"
#include    "gtest/gtest.h"
#include    "VR_DialogEngine_mock.h"
#include    "VR_PhoneBookXMLFormat.h"
#include    "VR_ContentProvider.h"
#include    "VR_PhoneBookStateListener.h"

using namespace  navi::dataprovider;

/**
 *  @brief  the VR_PhoneBookStateListener_Test class
 *
 *
 *  this class is just for VR_PhoneBookStateListener test
 */
class  VR_PhoneBookStateListener_Test  :  public  testing::Test
{
public:
    virtual  void  SetUp();
    virtual  void  TearDown();

public:
    VR_PhoneBookStateListener * m_pStateListener;

};

void   VR_PhoneBookStateListener_Test::SetUp()
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock());
    m_pStateListener = VR_new VR_PhoneBookStateListener(spEngine);
}

void  VR_PhoneBookStateListener_Test::TearDown()
{
    if (NULL != m_pStateListener) {
        delete m_pStateListener;
        m_pStateListener = NULL;
    }
}

TEST_F(VR_PhoneBookStateListener_Test, test_Assign)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    m_pStateListener->Assign(spEngine);
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_GetInstance)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    m_pStateListener->Assign(spEngine);
    m_pStateListener->GetInstance();
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_Dispose)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    m_pStateListener->Assign(spEngine);
    m_pStateListener->Dispose();
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_initialize_avp_dis)
{
    VR_ContentProvider  cp;
    cp.UpdateContentProviderValue("content://system/info", "bt/avp", "disconnected");
    m_pStateListener->Initialize();
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_initialize_avp)
{
    VR_ContentProvider  cp;
    cp.UpdateContentProviderValue("content://system/info", "bt/avp", "connected");
    m_pStateListener->Initialize();
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_initialize_curdev)
{
    VR_ContentProvider  cp;
    cp.UpdateContentProviderValue("content://system/info", "bt/hfp-curdev", "0");
    m_pStateListener->Initialize();
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_initialize_main)
{
    VR_ContentProvider  cp;
    cp.UpdateContentProviderValue("content://system/info", "bt/hfp-main", "disconnected");
    m_pStateListener->Initialize();
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_initialize_phonecall)
{
    VR_ContentProvider  cp;
    cp.UpdateContentProviderValue("content://system/info", "bt/phonecall", "idle");
    m_pStateListener->Initialize();
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_initialize_devnum)
{
    VR_ContentProvider  cp;
    cp.UpdateContentProviderValue("content://system/info", "bt/hfp-devnum", "0");
    m_pStateListener->Initialize();
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_initialize_pbk)
{
    VR_ContentProvider  cp;
    cp.UpdateContentProviderValue("content://system/info", "vehicle/pbk", "off");
    m_pStateListener->Initialize();
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_initialize_operestrict)
{
    VR_ContentProvider  cp;
    cp.UpdateContentProviderValue("content://system/info", "vehicle/operestrict", "off");
    m_pStateListener->Initialize();
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_initialize_pbmode)
{
    VR_ContentProvider  cp;
    cp.UpdateContentProviderValue("content://system/info", "bt/pbmode", "0");
    m_pStateListener->Initialize();
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_onNotifyUpdate_pbk)
{
    std::string strValue("testing");
    nutshell::NCUri testUrl(nutshell::NCString("content://system/info?vehicle/pbk"));
    m_pStateListener->onNotifyUpdate(testUrl, nutshell::NCVariant(strValue));
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_onNotifyUpdate_operestrict)
{
    std::string strValue("testing");
    nutshell::NCUri testUrl(nutshell::NCString("content://system/info?vehicle/operestrict"));
    m_pStateListener->onNotifyUpdate(testUrl, nutshell::NCVariant(strValue));
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_onNotifyUpdate_main)
{
    std::string strValue("disconnected");
    nutshell::NCUri testUrl(nutshell::NCString("content://system/info?bt/hfp-main"));
    m_pStateListener->onNotifyUpdate(testUrl, nutshell::NCVariant(strValue));
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_onNotifyUpdate_curdev)
{
    std::string strValue("0");
    nutshell::NCUri testUrl(nutshell::NCString("content://system/info?bt/hfp-curdev"));
    m_pStateListener->onNotifyUpdate(testUrl, nutshell::NCVariant(strValue));
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_onNotifyUpdate_devnum)
{
    std::string strValue("testing");
    nutshell::NCUri testUrl(nutshell::NCString("content://system/info?bt/hfp-devnum"));
    m_pStateListener->onNotifyUpdate(testUrl, nutshell::NCVariant(strValue));
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_onNotifyUpdate_rss)
{
    std::string strValue("testing");
    nutshell::NCUri testUrl(nutshell::NCString("content://system/info?bt/rss"));
    m_pStateListener->onNotifyUpdate(testUrl, nutshell::NCVariant(strValue));
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_onNotifyUpdate_phonecall)
{
    std::string strValue("incoming");
    nutshell::NCUri testUrl(nutshell::NCString("content://system/info?bt/phonecall"));
    m_pStateListener->onNotifyUpdate(testUrl, nutshell::NCVariant(strValue));
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_onNotifyUpdate_phonecall_idle)
{
    std::string strValue("incoming");
    nutshell::NCUri testUrl(nutshell::NCString("content://system/info?bt/phonecall"));
    m_pStateListener->onNotifyUpdate(testUrl, nutshell::NCVariant(strValue));
    strValue = "idle";
    m_pStateListener->onNotifyUpdate(testUrl, nutshell::NCVariant(strValue));
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_onNotifyUpdate_pbupdatestatus)
{
    std::string strValue("testing");
    nutshell::NCUri testUrl(nutshell::NCString("content://system/info?bt/pbupdatestatus"));
    m_pStateListener->onNotifyUpdate(testUrl, nutshell::NCVariant(strValue));
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_onNotifyUpdate_avp_dis)
{
    std::string strValue("disconnected");
    nutshell::NCUri testUrl(nutshell::NCString("content://system/info?bt/avp"));
    m_pStateListener->onNotifyUpdate(testUrl, nutshell::NCVariant(strValue));
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_onNotifyUpdate_avp)
{
    std::string strValue("connected");
    nutshell::NCUri testUrl(nutshell::NCString("content://system/info?bt/avp"));
    m_pStateListener->onNotifyUpdate(testUrl, nutshell::NCVariant(strValue));
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_onNotifyUpdate_pbmode)
{
    std::string strValue("testing");
    nutshell::NCUri testUrl(nutshell::NCString("content://system/info?bt/pbmode"));
    m_pStateListener->onNotifyUpdate(testUrl, nutshell::NCVariant(strValue));
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_onNotifyUpdate_other)
{
    std::string strValue("testingx");
    nutshell::NCUri testUrl(nutshell::NCString("content://system/info?bt/other"));
    m_pStateListener->onNotifyUpdate(testUrl, nutshell::NCVariant(strValue));
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_getPhoneState)
{
    m_pStateListener->getPhoneState();
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_get)
{
    nutshell::NCString deviceIdValue;
    m_pStateListener->get("bt/hfp-curdev", deviceIdValue);
    SUCCEED();
}

TEST_F(VR_PhoneBookStateListener_Test, test_sendStateMsgToFC)
{
    const VR_String  VR_PB_STATE_INCALL_TO_FC =
            "<action agent=\"phone\" op=\"phoneState\">"\
                "<phoneInCall>%1%</phoneInCall>"\
            "</action>";
    VR_String valueXml("disconnected");

    m_pStateListener->sendStateMsgToFC(VR_PB_STATE_INCALL_TO_FC, valueXml);
    SUCCEED();
}

/* EOF */
