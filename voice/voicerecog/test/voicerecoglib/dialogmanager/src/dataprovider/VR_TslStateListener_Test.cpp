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

#include "gtest/gtest.h"

#include "VR_DialogEngine_mock.h"
#include "stdafx.h"
#include "VR_ConfigureIF.h"
#include "VR_DialogEngineIF.h"
#include "VR_DataProviderComm.h"
#include "VR_TslStateListener.h"
#include <contentresolver/NCContentResolver.h>
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "navi/Voice/VoiceRecog/VrNotifyRecogResult.pb.h"

using namespace navi::VoiceRecog;
using namespace navi::dataprovider;
using namespace nutshell;

const VR_String ADD_RECORD_VOICETAG    = VR_String("addRecordVoiceTag");
const VR_String EDIT_RECORD_VOICETAG   = VR_String("editRecordVoiceTag");
const VR_String SAVE_VOICETAG          = VR_String("saveVoiceTag");
const VR_String DELETE_VOICETAG        = VR_String("deleteVoiceTag");
const VR_String DELETE_ALL_VOICETAG    = VR_String("deleteAllVoiceTag");
const VR_String PLAY_VOICETAG          = VR_String("playVoiceTag");
const VR_String STOP_VOICETAG          = VR_String("stopVoiceTag");
const VR_String CANCEL_RECORD_VOICETAG = VR_String("cancelRecordVoiceTag");
const VR_String CANCEL_SAVE_VOICETAG   = VR_String("cancelSaveVoiceTag");
const VR_INT32  INVALID_VALUE          = -1;

/**
* VR_TslStateListener_Test
*
* The class is just for VR_TslStateListener test.
*/
class VR_TslStateListener_Test : public testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

public:
    VR_TslStateListener *m_VR_TslStateListener;
};

void VR_TslStateListener_Test::SetUp()
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    m_VR_TslStateListener = VR_new VR_TslStateListener(spEngine);
}

void VR_TslStateListener_Test::TearDown()
{
    if (NULL != m_VR_TslStateListener) {
        delete m_VR_TslStateListener;
        m_VR_TslStateListener = NULL;
    }
}

TEST_F(VR_TslStateListener_Test, Initialize)
{
    m_VR_TslStateListener->Initialize();
    SUCCEED();
}

TEST_F(VR_TslStateListener_Test, GetInstance)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    m_VR_TslStateListener->GetInstance(spEngine);
    SUCCEED();
}

TEST_F(VR_TslStateListener_Test, GetInstance_other)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    m_VR_TslStateListener->GetInstance(spEngine);
    m_VR_TslStateListener->GetInstance(spEngine);
    SUCCEED();
}

TEST_F(VR_TslStateListener_Test, Dispose)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    m_VR_TslStateListener->GetInstance(spEngine);
    m_VR_TslStateListener->Dispose();
    SUCCEED();
}

TEST_F(VR_TslStateListener_Test, Dispose_other)
{
    m_VR_TslStateListener->m_instance =  NULL;
    m_VR_TslStateListener->Dispose();
    SUCCEED();
}

TEST_F(VR_TslStateListener_Test, onNotifyUpdate)
{
    NCUri uri(VR_TSL_STATUS_INFO_URI);
    NCVariant variant;
    variant.setValue(NCString("available"));
    m_VR_TslStateListener->m_tslStatus = 0;
    m_VR_TslStateListener->onNotifyUpdate(uri, variant);
    SUCCEED();
}

TEST_F(VR_TslStateListener_Test, onNotifyUpdate_1)
{
    NCUri uri(VR_TSL_STATUS_INFO_URI);
    NCVariant variant;
    variant.setValue(NCString("available"));
    m_VR_TslStateListener->m_tslStatus = 0;
    m_VR_TslStateListener->m_spDialogEngineIF = NULL;
    m_VR_TslStateListener->onNotifyUpdate(uri, variant);
    SUCCEED();
}

TEST_F(VR_TslStateListener_Test, onNotifyUpdate_2)
{
    NCUri uri(VR_TSL_STATUS_INFO_URI);
    NCVariant variant;
    variant.setValue(NCString("unavailable"));
    m_VR_TslStateListener->m_tslStatus = 0;
    m_VR_TslStateListener->onNotifyUpdate(uri, variant);
    SUCCEED();
}

TEST_F(VR_TslStateListener_Test, onNotifyUpdate_3)
{
    NCUri uri("111");
    NCVariant variant;
    variant.setValue(NCString("unavailable"));
    m_VR_TslStateListener->onNotifyUpdate(uri, variant);
    SUCCEED();
}

TEST_F(VR_TslStateListener_Test, get)
{
    NCString key("tsl/status");
    NCString value;
    m_VR_TslStateListener->get(key, value);
    m_VR_TslStateListener->get("key", value);
    SUCCEED();
}

TEST_F(VR_TslStateListener_Test, GetGlobalState)
{
    m_VR_TslStateListener->m_spDialogEngineIF = NULL;
    m_VR_TslStateListener->GetGlobalState();
    SUCCEED();
}

TEST_F(VR_TslStateListener_Test, sendStateMsgToFC)
{
    VR_String xml("aa");
    m_VR_TslStateListener->sendStateMsgToFC(xml);
    SUCCEED();
}

/* EOF */
