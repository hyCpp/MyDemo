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

#define private public
#define protected public

#include "VR_DEMessageBuilder.h"
#include "VR_DECommon_mock.h"
#include "Vr_Asr_Event_Notify_mock.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Vr_Asr_Engine.h"
using namespace N_Vr;
using namespace N_Asr;

using ::testing::Return;
using ::testing::_;
using ::testing::AtLeast;


/**
* VR_DEMessageBuilder_test.cpp
*
* The class for VR_DEMessageBuilder_test.
*/
class VR_DEMessageBuilder_test : public testing::Test
{
public:
    VR_DEMessageBuilder_test()
    {
        m_demessagebuilder = NULL;
        pDECommonIF = NULL;
    }

    virtual ~VR_DEMessageBuilder_test()
    {
    }

    virtual void SetUp()
    {
        pDECommonIF = new VR_DECommon_mock();
        m_demessagebuilder = new VR_DEMessageBuilder(pDECommonIF);
        m_c_event_notify = new C_Event_Notify_mock();
    }

    virtual void TearDown()
    {
        if (m_demessagebuilder != NULL) {
            delete m_demessagebuilder;
            m_demessagebuilder = NULL;
        }

        if (pDECommonIF != NULL) {
            delete pDECommonIF;
        }

        if (m_c_event_notify != NULL) {
            delete m_c_event_notify;
        }
    }

private:
    VR_DEMessageBuilder_test(const VR_DEMessageBuilder_test&);
    VR_DEMessageBuilder_test& operator= (const VR_DEMessageBuilder_test&);

    VR_ConfigureIF* m_pConfigure;
    VR_DECommon_mock* pDECommonIF;
    VR_DEMessageBuilder* m_demessagebuilder;
    C_Event_Notify_mock* m_c_event_notify;
};

// buildVolumeDisplay BEGIN  
TEST_F(VR_DEMessageBuilder_test, buildVolumeDisplay_00)
{
    std::string  outMsg = "";
    EXPECT_CALL(*m_c_event_notify, Get_Event_Notify_Type()).WillRepeatedly(
        Return(C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_Network_Disconnect));
    m_demessagebuilder->buildVolumeDisplay(*m_c_event_notify, outMsg);
    SUCCEED();
}

TEST_F(VR_DEMessageBuilder_test, buildVolumeDisplay_01)
{
    std::string  outMsg = "";
    EXPECT_CALL(*m_c_event_notify, Get_Event_Notify_Type()).WillRepeatedly(
        Return(C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_Too_Quiet));
    m_demessagebuilder->buildVolumeDisplay(*m_c_event_notify, outMsg);
    SUCCEED();
}

TEST_F(VR_DEMessageBuilder_test, buildVolumeDisplay_02)
{
    std::string  outMsg = "";
    EXPECT_CALL(*m_c_event_notify, Get_Event_Notify_Type()).WillRepeatedly(
        Return(C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_No_Signal));
    m_demessagebuilder->buildVolumeDisplay(*m_c_event_notify, outMsg);
    SUCCEED();
}

TEST_F(VR_DEMessageBuilder_test, buildVolumeDisplay_03)
{
    std::string  outMsg = "";
    EXPECT_CALL(*m_c_event_notify, Get_Event_Notify_Type()).WillRepeatedly(
        Return(C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_Overload));
    m_demessagebuilder->buildVolumeDisplay(*m_c_event_notify, outMsg);
    SUCCEED();
}

TEST_F(VR_DEMessageBuilder_test, buildVolumeDisplay_04)
{
    std::string  outMsg = "";
    EXPECT_CALL(*m_c_event_notify, Get_Event_Notify_Type()).WillRepeatedly(
        Return(C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_Too_Noisy));
    m_demessagebuilder->buildVolumeDisplay(*m_c_event_notify, outMsg);
    SUCCEED();
}
// buildVolumeDisplay END

// buildPlayTTSAction BEGIN
TEST_F(VR_DEMessageBuilder_test, buildPlayTTSAction_0)
{
    std::string txt = "";
    std::string outMsg = "";
    // EXPECT_FALSE(m_demessagebuilder->m_pDECommonIF->getVROverPrompt());
    m_demessagebuilder->buildPlayTTSAction(txt, outMsg);
    SUCCEED();
}

TEST_F(VR_DEMessageBuilder_test, buildPlayTTSAction_1)
{
    std::string txt = "";
    std::string outMsg = "";
    // EXPECT_TRUE(m_demessagebuilder->m_pDECommonIF->getVROverPrompt());
    m_demessagebuilder->buildPlayTTSAction(txt, outMsg);
    SUCCEED();
}
// buildPlayTTSAction END

TEST_F(VR_DEMessageBuilder_test, buildStopTTSAction)
{
    std::string outMsg;
    m_demessagebuilder->buildStopTTSAction(outMsg);
    SUCCEED();
}

// buildPlayBeepAction BEGIN
TEST_F(VR_DEMessageBuilder_test, buildPlayBeepAction_0)
{
    std::string beepPath = "";
    std::string outMsg = "";
    m_demessagebuilder->m_pDECommonIF = pDECommonIF;
    EXPECT_CALL(*pDECommonIF, getVROverBeep()).WillRepeatedly(Return(true));
    m_demessagebuilder->buildPlayBeepAction(beepPath, outMsg);
    SUCCEED();
}

TEST_F(VR_DEMessageBuilder_test, buildPlayBeepAction_1)
{
    std::string beepPath = "";
    std::string outMsg = "";
    m_demessagebuilder->m_pDECommonIF = pDECommonIF;
    EXPECT_CALL(*pDECommonIF, getVROverBeep()).WillRepeatedly(Return(false));
    m_demessagebuilder->buildPlayBeepAction(beepPath, outMsg);
    SUCCEED();
}
// buildPlayBeepAction END

TEST_F(VR_DEMessageBuilder_test, buildStopBeepAction)
{
    std::string outMsg;
    m_demessagebuilder->buildStopBeepAction(outMsg);
    SUCCEED();
}

// buildCloseSessionAction BEGIN
TEST_F(VR_DEMessageBuilder_test, buildCloseSessionAction)
{
    std::string outMsg;
    m_demessagebuilder->buildCloseSessionAction(outMsg);
    SUCCEED();
}
// buildCloseSessionAction END

/* EOF */
