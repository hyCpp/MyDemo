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

#include   "stdafx.h"
#include   "gtest/gtest.h"
#include   "VR_DialogEngine_mock.h"
#include   "VR_EventSenderIF.h"
#include   "EV_EventContext.h"
#include   "VR_EngineEventSender.h"
#include   "eventmanger/VR_EngineEventSenderIF.h"

/**
* VR_EngineEventSender_Test.cpp
*
* The class is just for VR_EngineEventSender test.
*/
class  VR_EngineEventSender_Test : public testing::Test
{
protected:

    virtual   void  SetUp();
    virtual   void  TearDown();

    VR_EngineEventSender*     m_VR_EngineEventSender;
};


void
VR_EngineEventSender_Test::SetUp()
{
    m_VR_EngineEventSender = VR_new  VR_EngineEventSender();
}

void
VR_EngineEventSender_Test::TearDown()
{
    if (m_VR_EngineEventSender) {
        delete  m_VR_EngineEventSender;
        m_VR_EngineEventSender = NULL;
    }
}

TEST_F(VR_EngineEventSender_Test, SendEvent_display)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<display agent=\"climate\" op= turn>"
                                         "<type>AC</type>"
                                         "<status>ON</status>"
                                 "</display>";
    m_VR_EngineEventSender->SendEvent(nTaskId, nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendEvent_action)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<action agent=\"climate\" op= turn>"
                                         "<type>AC</type>"
                                         "<status>ON</status>"
                                 "</action>";
    m_VR_EngineEventSender->SendEvent(nTaskId, nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendEvent_grammar_result)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<grammar_result agent=\"climate\" op= turn>"
                                         "<type>AC</type>"
                                         "<status>ON</status>"
                                 "</grammar_result>";
    m_VR_EngineEventSender->SendEvent(nTaskId, nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendEvent_event_result)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<event-result name=\"climate\" op= turn>"
                                         "<type>AC</type>"
                                         "<status>ON</status>"
                                 "</event-result>";
    m_VR_EngineEventSender->SendEvent(nTaskId, nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendEvent_event_result_voiceTag)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<event-result name=\"addRecordVoiceTag\" op= turn>"
                                         "<type>AC</type>"
                                         "<status>ON</status>"
                                 "</event-result>";
    m_VR_EngineEventSender->SendEvent(nTaskId, nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendEvent_event_result_editRecordVoiceTag)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<event-result name=\"editRecordVoiceTag\" op= turn>"
                                         "<type>AC</type>"
                                         "<status>ON</status>"
                                 "</event-result>";
    m_VR_EngineEventSender->SendEvent(nTaskId, nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendEvent_event_result_saveVoiceTag)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<event-result name=\"saveVoiceTag\" op= turn>"
                                         "<type>AC</type>"
                                         "<status>ON</status>"
                                 "</event-result>";
    m_VR_EngineEventSender->SendEvent(nTaskId, nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendEvent_event_result_deleteVoiceTag)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<event-result name=\"deleteVoiceTag\" op= turn>"
                                         "<type>AC</type>"
                                         "<status>ON</status>"
                                 "</event-result>";
    m_VR_EngineEventSender->SendEvent(nTaskId, nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendEvent_event_result_playVoiceTag)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<event-result name=\"playVoiceTag\" op= turn>"
                                         "<type>AC</type>"
                                         "<status>ON</status>"
                                 "</event-result>";
    m_VR_EngineEventSender->SendEvent(nTaskId, nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendEvent_event_result_stopVoiceTag)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<event-result name=\"stopVoiceTag\" op= turn>"
                                         "<type>AC</type>"
                                         "<status>ON</status>"
                                 "</event-result>";
    m_VR_EngineEventSender->SendEvent(nTaskId, nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendEvent_event_result_cancelRecordVoiceTag)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<event-result name=\"cancelRecordVoiceTag\" op= turn>"
                                         "<type>AC</type>"
                                         "<status>ON</status>"
                                 "</event-result>";
    m_VR_EngineEventSender->SendEvent(nTaskId, nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendEvent_event_result_cancelSaveVoiceTag)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<event-result name=\"cancelSaveVoiceTag\" op= turn>"
                                         "<type>AC</type>"
                                         "<status>ON</status>"
                                 "</event-result>";
    m_VR_EngineEventSender->SendEvent(nTaskId, nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendEvent_other)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<event-result1 name=\"cancelSaveVoiceTag\" op= turn>"
                                         "<type>AC</type>"
                                         "<status>ON</status>"
                                 "</event-result1>";
    m_VR_EngineEventSender->SendEvent(nTaskId, nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendNotifyEvent_VRState_quit)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<display agent=\"Common\" content=\"VRState\">"
                                                        "<engineType>local</engineType>"
                                                        "<state>quit</state>"
                                                        "<tag>Voice Control</tag>"
                                                        "<prompt />"
                                                        "<result />"
                                                        "<meter>on</meter>"
                                                   "</display>";
    m_VR_EngineEventSender->SendNotifyEvent(nTaskId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendNotifyEvent_VRState_idle)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<display agent=\"Common\" content=\"VRState\">"
                                                        "<engineType>local</engineType>"
                                                        "<state>idle</state>"
                                                        "<tag>Voice Control</tag>"
                                                        "<prompt />"
                                                        "<result />"
                                                        "<meter>on</meter>"
                                                   "</display>";
    m_VR_EngineEventSender->SendNotifyEvent(nTaskId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendNotifyEvent_VRState_promptPlaying)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<display agent=\"Common\" content=\"VRState\">"
                                                        "<engineType>local</engineType>"
                                                        "<state>promptPlaying</state>"
                                                        "<tag>Voice Control</tag>"
                                                        "<prompt />"
                                                        "<result />"
                                                        "<meter>on</meter>"
                                                   "</display>";
    m_VR_EngineEventSender->SendNotifyEvent(nTaskId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendNotifyEvent_VRState_paused)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<display agent=\"Common\" content=\"VRState\">"
                                                        "<engineType>local</engineType>"
                                                        "<state>paused</state>"
                                                        "<tag>Voice Control</tag>"
                                                        "<prompt />"
                                                        "<result />"
                                                        "<meter>on</meter>"
                                                   "</display>";
    m_VR_EngineEventSender->SendNotifyEvent(nTaskId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendNotifyEvent_VRState_listening)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<display agent=\"Common\" content=\"VRState\">"
                                                        "<engineType>local</engineType>"
                                                        "<state>listening</state>"
                                                        "<tag>Voice Control</tag>"
                                                        "<prompt />"
                                                        "<result />"
                                                        "<meter>on</meter>"
                                                   "</display>";
    m_VR_EngineEventSender->SendNotifyEvent(nTaskId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendNotifyEvent_VRState_processing)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<display agent=\"Common\" content=\"VRState\">"
                                                        "<engineType>local</engineType>"
                                                        "<state>processing</state>"
                                                        "<tag>Voice Control</tag>"
                                                        "<prompt />"
                                                        "<result />"
                                                        "<meter>on</meter>"
                                                   "</display>";
    m_VR_EngineEventSender->SendNotifyEvent(nTaskId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendNotifyEvent_VRState_other)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<display agent=\"Common\" content=\"VRState\">"
                                                        "<engineType>local</engineType>"
                                                        "<state>111</state>"
                                                        "<tag>Voice Control</tag>"
                                                        "<prompt />"
                                                        "<result />"
                                                        "<meter>on</meter>"
                                                   "</display>";
    m_VR_EngineEventSender->SendNotifyEvent(nTaskId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendNotifyEvent_NotVRState)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<display agent=\"Common\" content=\"Volume\">"
                                                        "<type>asr</type>"
                                                        "<volume>55</volume>"
                                                   "</display>";
    m_VR_EngineEventSender->SendNotifyEvent(nTaskId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendReqEvent_Transform_NULL)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<action1 agent=\"prompt\" op=\"playTts\">"
                                                            "<language>en-us</language>"
                                                            "<text> After the beep, say a name.</text>"
                                                            "<phoneme />"
                                                       "</action1>";
    m_VR_EngineEventSender->SendReqEvent(nTaskId, nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendReqEvent_name)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<action name=\"111\">"
                                                        "<type>asr</type>"
                                                        "<volume>55</volume>"
                                                   "</action>";
    m_VR_EngineEventSender->SendReqEvent(nTaskId, nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendReqEvent__agent)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<action name=\"111\" agent=\"222\">"
                                                        "<type>asr</type>"
                                                        "<volume>55</volume>"
                                                   "</action>";
    m_VR_EngineEventSender->SendReqEvent(nTaskId, nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendReqEvent__op)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<action name=\"111\" op=\"222\">"
                                                        "<type>asr</type>"
                                                        "<volume>55</volume>"
                                                   "</action>";
    m_VR_EngineEventSender->SendReqEvent(nTaskId, nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendReqEvent_DE_DM)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<action agent=\"prompt\" op=\"playTts\">"
                                                            "<language>en-us</language>"
                                                            "<text> After the beep, say a name.</text>"
                                                            "<phoneme />"
                                                       "</action>";
    m_VR_EngineEventSender->SendReqEvent(nTaskId, nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendReqEvent_DE_FC)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<action agent=\"help\" op=\"showVoiceSettingsUI\">"
                                                    "</action>";
    m_VR_EngineEventSender->SendReqEvent(nTaskId, nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendReqEvent_NO_FIND_MSG)
{
    const int nTaskId = 101;
    const int nId     = 101;
    const VR_String  strAction = "<action agent=\"help\" op=\"showVoiceSettingsUI1\">"
                                                    "</action>";
    m_VR_EngineEventSender->SendReqEvent(nTaskId, nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendReplyEvent_NoEvent)
{
    const int nId     = 101;
    const VR_String  strAction = "<action agent=\"help\" op=\"showVoiceSettingsUI1\">"
                                                    "</action>";
    m_VR_EngineEventSender->SendReplyEvent(nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendReplyEvent)
{
//    const int nId     = 101;
//    std::string strAction = "<event-result name=\"startAgent\">"
//        "</event-result>";
//    m_VR_EngineEventSender->SendReplyEvent(nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendReplyVoiceTagEvent_NO_FIND_MSG)
{
    const int nId     = 101;
    const VR_String  strAction = "<event-result name=\"saveVoiceTag\" errcode=\"0\">"
                                                        "<actionId>39</actionId>"
                                                        "<voiceTagId>0</voiceTagId>"
                                                    "</event-result>";
    m_VR_EngineEventSender->SendReplyVoiceTagEvent(nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendReplyVoiceTagEvent_NO_FIND_MSG1)
{
    const int nId     = 101;
    const VR_String  strAction = "<event-result name=\"saveVoiceTag1\" errcode=\"0\">"
                                                        "<actionId>39</actionId>"
                                                        "<voiceTagId>0</voiceTagId>"
                                                    "</event-result>";
    m_VR_EngineEventSender->SendReplyVoiceTagEvent(nId, strAction);
    SUCCEED();
}

TEST_F(VR_EngineEventSender_Test, SendReplyVoiceTagEvent_DE_DM)
{
    const int nId     = 101;
    const VR_String  strAction = "<event name=\"cancelSaveVoiceTag\">"
                                                        "<actionId>39</actionId>"
                                                    "</event>";
    m_VR_EngineEventSender->SendReplyVoiceTagEvent(nId, strAction);
    SUCCEED();
}
/* EOF */
