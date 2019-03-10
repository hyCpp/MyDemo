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

#include "stdafx.h"
#include "gtest/gtest.h"
#include "VR_DialogTaskBase.h"
#include "EV_EventContext.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_DialogTaskBase_Test.cpp
*
* The class is just for VR_DialogTaskBase test.
*/
class VR_DialogTaskBase_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_DialogTaskBase *m_Vr_DialogTaskBaseNull;
};

void
VR_DialogTaskBase_Test::SetUp()
{
    m_Vr_DialogTaskBaseNull = VR_new  VR_DialogTaskNull();
}

void
VR_DialogTaskBase_Test::TearDown()
{
    if (m_Vr_DialogTaskBaseNull) {
        delete m_Vr_DialogTaskBaseNull;
        m_Vr_DialogTaskBaseNull = NULL;
    }
}

TEST_F(VR_DialogTaskBase_Test, Start_01_Case)
{
    EV_EventContext  context;
    std::unique_ptr<VrRecogResultConfirm>  actionComfirm(VrRecogResultConfirm::default_instance().New());
    actionComfirm->set_action_id(HVACFrontScreen);
    actionComfirm->set_processing_result(true);
    m_Vr_DialogTaskBaseNull->VR_DialogTaskBase::Start(context, *actionComfirm);
    VR_String  strType = "test";
    m_Vr_DialogTaskBaseNull->VR_DialogTaskBase::UserOperation(context, strType);
    m_Vr_DialogTaskBaseNull->VR_DialogTaskBase::OnRequestVrTsl(context, *actionComfirm);
}

TEST_F(VR_DialogTaskBase_Test, Start_Case)
{
    m_Vr_DialogTaskBaseNull->Start();
    SUCCEED();
}

TEST_F(VR_DialogTaskBase_Test, Cancel_Case)
{
    EV_EventContext context;
    m_Vr_DialogTaskBaseNull->Cancel(context);
    SUCCEED();
}

TEST_F(VR_DialogTaskBase_Test, UserOperation_Case)
{
    EV_EventContext context;
    VR_String strType;
    m_Vr_DialogTaskBaseNull->UserOperation(context, strType);
    SUCCEED();
}

TEST_F(VR_DialogTaskBase_Test, OnMessage_Case)
{
    std::unique_ptr<VrRecogResultConfirm>  actionComfirm(VrRecogResultConfirm::default_instance().New());
    actionComfirm->set_action_id(HVACFrontScreen);
    actionComfirm->set_processing_result(true);
    m_Vr_DialogTaskBaseNull->OnMessage(*actionComfirm);
    SUCCEED();
}

TEST_F(VR_DialogTaskBase_Test, OnRequestVrTsl_Case)
{
    EV_EventContext context;
    std::unique_ptr<VrRecogResultConfirm>  actionComfirm(VrRecogResultConfirm::default_instance().New());
    actionComfirm->set_action_id(HVACFrontScreen);
    actionComfirm->set_processing_result(true);
    m_Vr_DialogTaskBaseNull->OnRequestVrTsl(context, *actionComfirm);
    SUCCEED();
}
/* EOF */
