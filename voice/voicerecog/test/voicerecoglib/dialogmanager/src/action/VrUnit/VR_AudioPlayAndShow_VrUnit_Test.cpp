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
#include "stdafx.h"
#include "VR_ReceiverCb.h"
#include "VR_ActionEventSender.h"
#include "VR_DialogEngine_mock.h"
#include "VR_AvcLanSender_mock.h"
#include "VR_AudioPlayAndShow_VrUnit.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrTtsCommand.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_AudioPlayAndShow_VrUnit_Test
*
* The class is just for VR_AudioPlayAndShow_VrUnit test.
*/
class VR_AudioPlayAndShow_VrUnit_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_String m_xml;
    VR_AudioPlayAndShow_VrUnit *m_action;
};

void
VR_AudioPlayAndShow_VrUnit_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = StartApps;
     std::unique_ptr<VrStartApps> ProtoMsg(VrStartApps::default_instance().New());
     ProtoMsg->set_application_id(1);
     ProtoMsg->set_data_size(0);
     m_xml = "<event name=\"cancel\">"
             "</event>";
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
     m_action = VR_new VR_AudioPlayAndShow_VrUnit(1, 1, m_xml, actionSender, spAvcSender);
}

void
VR_AudioPlayAndShow_VrUnit_Test::TearDown()
{
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
}

TEST_F(VR_AudioPlayAndShow_VrUnit_Test, Run_Case)
{
    m_action->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayAndShow_VrUnit_Test, OnReceive_Case)
{
    std::unique_ptr<VrRecogResultConfirm> protoMsg(VrRecogResultConfirm::default_instance().New());
    protoMsg->set_action_id(StartApps);
    protoMsg->set_processing_result(true);

    m_action->OnReceive(*protoMsg);
    SUCCEED();
}
/* EOF */


