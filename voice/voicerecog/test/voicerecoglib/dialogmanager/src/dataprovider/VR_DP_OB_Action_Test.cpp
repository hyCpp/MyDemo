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
#include   "VR_DP_OB_Action.h"
#include   "VR_DM_CommandHandle.h"
#include   "VR_DP_OB_Action_Hfp.h"
#include   "VR_DialogEngine_mock.h"
// #include   "VR_AvcLanSender_mock.h"
#include   "VR_DP_OB_Action_Radio.h"
#include   "VR_DP_OB_Action_Music.h"
#include   "VR_DP_OB_Action_TslStatus.h"
#include   "VR_DP_OB_Action_AudioSource.h"
#include   "VR_DP_ThreadWorking.h"
// #include   "VR_MusicGracenoteListener.h"

// VR_DECLARE_CLASS_WITH_SMART_PTR(VR_AvcLanSender_Test);

using namespace navi::dataprovider;

/**
 *  @brief  The  VR_DP_OB_Action_Test  class
 *
 *
 *   this class is just for VR_DP_OB_Action test
 */
class VR_DP_OB_Action_Test : public testing::Test
{
protected:

    virtual   void  SetUp();
    virtual   void  TearDown();

    spVR_DialogEngineIF   m_spDialogEngine;
    spVR_CommandHandleIF  m_spCommandHandler;
};

void  VR_DP_OB_Action_Test::SetUp()
{
    m_spDialogEngine = spVR_DialogEngineIF(VR_new  VR_DialogEngine_mock());
    m_spCommandHandler = spVR_CommandHandleIF(VR_new  VR_DM_CommandHandle());
}

void  VR_DP_OB_Action_Test::TearDown()
{

}

TEST_F(VR_DP_OB_Action_Test, test_radio)
{
    nutshell::NCUri          InitialUrl("content://dataprovider/info?");
    nutshell::NCVariant      radioVariant;
    VR_DP_OB_Action_Radio    dpActionRadio(InitialUrl, radioVariant, m_spDialogEngine, m_spCommandHandler);
    dpActionRadio.setUriAndVariant(InitialUrl, radioVariant);

    dpActionRadio.DoUpdatePrepare();

    nutshell::NCUri          emptyUrl("");
    VR_DP_OB_Action_Radio    dpActionRadioUpdate(emptyUrl, radioVariant, m_spDialogEngine, m_spCommandHandler);
    dpActionRadioUpdate.setUriAndVariant(InitialUrl, radioVariant);
    dpActionRadioUpdate.DoUpdatePrepare();
    SUCCEED();
}

TEST_F(VR_DP_OB_Action_Test, test_music)
{
    nutshell::NCUri           emptyUrl("");
    nutshell::NCVariant       musicVariant;

    VR_DP_OB_Action_Music     dpActionMusic(emptyUrl, musicVariant, m_spDialogEngine, m_spCommandHandler, NULL);
    dpActionMusic.DoUpdatePrepare();
    spVR_DP_ThreadWorking dpHandle(VR_new VR_DP_ThreadWorking);
    SUCCEED();
}

TEST_F(VR_DP_OB_Action_Test, test_audiosource)
{
    nutshell::NCUri          InitialUrl("content://dataprovider/info?");
    nutshell::NCVariant      audiosourceVariant;
    VR_DP_OB_Action_AudioSource dpActionAudiosource(InitialUrl, audiosourceVariant, m_spDialogEngine, m_spCommandHandler);
    dpActionAudiosource.setUriAndVariant(InitialUrl, audiosourceVariant);

    dpActionAudiosource.DoUpdatePrepare();

    nutshell::NCUri             emptyUrl("");
    VR_DP_OB_Action_AudioSource dpActionAudiosourceUpdate(emptyUrl, audiosourceVariant, m_spDialogEngine, m_spCommandHandler);
    dpActionAudiosourceUpdate.setUriAndVariant(emptyUrl, audiosourceVariant);
    dpActionAudiosourceUpdate.DoUpdatePrepare();
    SUCCEED();
}

TEST_F(VR_DP_OB_Action_Test, test_hfd)
{
    nutshell::NCUri           emptyUrl("");
    nutshell::NCVariant       hfpVariant;
    VR_DP_OB_Action_Hfp       dpActionHfp(emptyUrl, hfpVariant, m_spDialogEngine, m_spCommandHandler);

    dpActionHfp.DoUpdatePrepare();

    SUCCEED();
}

TEST_F(VR_DP_OB_Action_Test, test_tslstatus)
{
    nutshell::NCUri           InitialUrl("content://dataprovider/info?");
    nutshell::NCVariant       tslStatusVariant;
    VR_DP_OB_Action_TslStatus dpActiontslStatus(InitialUrl, tslStatusVariant, m_spDialogEngine, m_spCommandHandler);
    dpActiontslStatus.setUriAndVariant(InitialUrl, tslStatusVariant);

    dpActiontslStatus.DoUpdatePrepare();

    nutshell::NCUri             emptyUrl("");
    VR_DP_OB_Action_TslStatus   dpActiontslStatusUpdate(emptyUrl, tslStatusVariant, m_spDialogEngine, m_spCommandHandler);
    dpActiontslStatusUpdate.setUriAndVariant(emptyUrl, tslStatusVariant);

    dpActiontslStatusUpdate.DoUpdatePrepare();

    SUCCEED();
}
/* EOF */
