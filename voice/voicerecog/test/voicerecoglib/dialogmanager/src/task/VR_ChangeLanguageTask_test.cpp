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

#include "VR_ChangeLanguageTask.h"
#include "stdafx.h"
#include <memory>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/assign/list_of.hpp>

#include "VR_EventSenderIF.h"

#include "VR_ChangeLanguageTask.h"
#include "VR_DialogEngineIF.h"
#include "VR_DailogEngineMessageBuilder.h"

#include "navi/Voice/VoiceRecog/RespVrCancelRecognitionResult.pb.h"
#include "navi/Voice/VoiceRecog/RespVrStartRecognitionResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMTaskMessage.pb.h"

class EV_EventContext;

/**
* VR_ChangeLanguageTask_Test
*
* The class is just for VR_ChangeLanguageTask test.
*/
class VR_ChangeLanguageTask_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_ChangeLanguageTask *m_vr_changelanguagetask;
};

void
VR_ChangeLanguageTask_Test::SetUp()
{
     const int type(1);
     const EV_EventContext *context;
     spVR_DialogEngineIF engine;
     m_vr_changelanguagetask = VR_new VR_ChangeLanguageTask(type, *context, engine);
}

void
VR_ChangeLanguageTask_Test::TearDown()
{
    if (NULL != m_vr_changelanguagetask) {
        delete m_vr_changelanguagetask;
        m_vr_changelanguagetask = NULL;
    }    
}

TEST_F(VR_ChangeLanguageTask_Test, Start_Case)
{
    m_vr_changelanguagetask->Start();
}

/* EOF */
