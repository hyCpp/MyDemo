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
#include "gmock/gmock.h"
#include "stdafx.h"
#include <string>
#include "VP_ReceiverCb.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "EV_EventReceiver.h"
#include "EV_EventContext.h"
#include "boost/function.hpp"
#include "boost/bind.hpp"
#include "navi/Debug/RequestVoiceTest.pb.h"
#include "navi/Debug/ResponseVoiceTest.pb.h"
#include "navi/AvcProxy/GuidanceVoiceMuteSettingRequest.pb.h"
#include "navi/AvcProxy/GuidanceVoiceMuteSettingResponse.pb.h"
#include "navi/Voice/VoicePlay/VpPlaySoundList.pb.h"
#include "navi/Voice/VoicePlay/VpOnPlayEnd.pb.h"
#include "navi/Voice/VoicePlay/VpPlaySoundId.pb.h"
#include "navi/Voice/VoicePlay/VpPlaySoundTts.pb.h"
#include "navi/Voice/VoicePlay/VpStopPlay.pb.h"
#include "navi/Voice/VoicePlay/VpGetDuration.pb.h"
#include "navi/Voice/VoicePlay/VpDuration.pb.h"
#include "navi/Voice/VoicePlay/VpSetVrMode.pb.h"
#include "navi/Voice/VoicePlay/VpSetLanguage.pb.h"
#include "navi/Voice/VoicePlay/VpOnLanguageChangged.pb.h"
#include "VP_Define.h"
#include "navi/Voice/TTS/TtsOnPlayEnd.pb.h"
#include "VP_ChannelManager_mock.h"
#include "VP_DataManager_mock.h"

using namespace ::navi::VoicePlay;
using namespace ::navi::TTS;
using namespace ::navi::Debug;
using namespace ::navi::AvcProxy;
using namespace std;

using testing::Return;
using testing::AtLeast;
using testing::_;
using ::testing::Mock;


/**
 * Summary for class VP_ReceiverCb_Test
 * 
 * This is a detail description for class VP_ReceiverCb_Test
 *
 */
class VP_ReceiverCb_Test : public testing::Test
{
public:
    virtual void SetUp()
    {
        dataMgr = make_shared<MockVP_DataManager>();
        shared_ptr<VP_VoicePlayControl> playControl = make_shared<VP_VoicePlayControl>(dataMgr, nullptr);
        m_channelMgr = make_shared<MockVP_ChannelManager>();
        playControl->m_channelMgr = m_channelMgr;

        m_pVP_ReceiverCb = make_shared<VP_ReceiverCb>();
        VP_Factory::Instance()->m_pControl = nullptr;
        VP_Factory::Instance()->m_pControl = playControl;
        VP_Factory::Instance()->m_pDataMgr = nullptr;
        VP_Factory::Instance()->m_pDataMgr = dataMgr;
    }
        
    virtual void TearDown() 
    {
        VP_Factory::Instance()->m_pControl = nullptr;
        VP_Factory::Instance()->m_pDataMgr = nullptr; 
    }

public:
    shared_ptr<MockVP_ChannelManager> m_channelMgr;
    shared_ptr<VP_ReceiverCb> m_pVP_ReceiverCb;
    shared_ptr<MockVP_DataManager> dataMgr;
};

TEST_F(VP_ReceiverCb_Test, Register_NULL)
{
    EV_EventReceiver *er = nullptr;
    m_pVP_ReceiverCb->Register(er);
}

TEST_F(VP_ReceiverCb_Test, Register)
{
    EV_EventReceiver *er = new(MEM_Voice) EV_EventReceiver("VP");
    m_pVP_ReceiverCb->Register(er);
    delete er;
}

TEST_F(VP_ReceiverCb_Test, OnRecvSoundPlayID_NullCheck)
{
    void *msg = nullptr;
    m_pVP_ReceiverCb->OnRecvSoundPlayID(msg);
}

TEST_F(VP_ReceiverCb_Test, OnRecvSoundPlayID)
{
    VpPlaySoundId* playSoundMsg = new(MEM_Voice) VpPlaySoundId();
    playSoundMsg->set_soundid(VP_WAVE_ID_VOLUME_CHANGE_JP);
    playSoundMsg->set_needplayendnotificy(true);
    playSoundMsg->set_sender("Guide");

    m_pVP_ReceiverCb->OnRecvSoundPlayID(playSoundMsg);
}

TEST_F(VP_ReceiverCb_Test, OnRecvSoundPlayTTS)
{
    VpPlaySoundTts *playTtsMsg = new(MEM_Voice) VpPlaySoundTts();
    playTtsMsg->set_text("Hello");
    playTtsMsg->set_needplayendnotificy(true);
    playTtsMsg->set_sender("Guide");

    m_pVP_ReceiverCb->OnRecvSoundPlayTTS(playTtsMsg);
}

TEST_F(VP_ReceiverCb_Test, OnRecvSoundPlayTTS_NullCheck)
{
    void *msg = nullptr;
    m_pVP_ReceiverCb->OnRecvSoundPlayTTS(msg);
}

TEST_F(VP_ReceiverCb_Test, OnRecvSoundPlayList_NullCheck)
{
    void *msg = nullptr;
    m_pVP_ReceiverCb->OnRecvSoundPlayList(msg);
}

TEST_F(VP_ReceiverCb_Test, OnRecvSoundPlayList)
{
    VpPlaySoundList *playSoundList = new(MEM_Voice) VpPlaySoundList();
    playSoundList->set_sender("Guide");
    playSoundList->set_needplayendnotificy(true);

    m_pVP_ReceiverCb->OnRecvSoundPlayList(playSoundList);
}

TEST_F(VP_ReceiverCb_Test, setTtsLang)
{
    std::string ttsLang;
    shared_ptr<VP_DataManager> pDataMgr = VP_Factory::Instance()->m_pDataMgr;

    pDataMgr->m_language = "ENU";
    m_pVP_ReceiverCb->setTtsLang(BL_APLINFO_REGION_CHINA, EV_MODULE_GUIDE, ttsLang);
    EXPECT_EQ(ttsLang, "MNC");

    pDataMgr->m_language = "CAH";
    m_pVP_ReceiverCb->setTtsLang(BL_APLINFO_REGION_CHINA, EV_MODULE_GUIDE, ttsLang);
    EXPECT_EQ(ttsLang, "CAH");

    pDataMgr->m_language = "ENU";
    m_pVP_ReceiverCb->setTtsLang(BL_APLINFO_REGION_CHINA, EV_MODULE_POINT, ttsLang);
    EXPECT_EQ(ttsLang, "ENU");

    m_pVP_ReceiverCb->setTtsLang(BL_APLINFO_REGION_IN, EV_MODULE_GUIDE, ttsLang);
    EXPECT_EQ(ttsLang, "ENU");
}

TEST_F(VP_ReceiverCb_Test, OnRecvSoundStopPlay_NullCheck)
{
    void *msg = nullptr;
    m_pVP_ReceiverCb->OnRecvSoundStopPlay(msg);
}

TEST_F(VP_ReceiverCb_Test, OnRecvSoundStopPlay)
{
    EXPECT_CALL(*m_channelMgr, getChannel(_)).WillOnce(Return(make_shared<VP_Channel>("VPTest", nullptr)));
    VpStopPlay *msg = new(MEM_Voice) VpStopPlay;
    msg->set_sender("Guide");
    m_pVP_ReceiverCb->OnRecvSoundStopPlay(msg);
}

TEST_F(VP_ReceiverCb_Test, OnRecvGetDuration)
{
    VpGetDuration *duration = new(MEM_Voice) VpGetDuration();
    m_pVP_ReceiverCb->OnRecvGetDuration(duration);
}

TEST_F(VP_ReceiverCb_Test, OnRecvGetDuration_NullCheck)
{
    VpGetDuration *duration = nullptr;
    m_pVP_ReceiverCb->OnRecvGetDuration(duration);
}

TEST_F(VP_ReceiverCb_Test, OnRecvTtsPlayEnd)
{
    TtsOnPlayEnd *m = new(MEM_Voice) TtsOnPlayEnd();
    m->set_sender(::std::string(EV_MODULE_VOICEPLAY));
    m->set_result(TtsOnPlayEnd::TTS_PLAY_RESULT_OK);
    m_pVP_ReceiverCb->OnRecvTtsPlayEnd(m);
}

TEST_F(VP_ReceiverCb_Test, OnRecvTtsPlayEnd_NullCheck)
{
    void *m = nullptr;
    m_pVP_ReceiverCb->OnRecvTtsPlayEnd(m);
}

TEST_F(VP_ReceiverCb_Test, OnRecvVoiceTest)
{
    EV_EventContext context;
    RequestVoiceTest *m = new(MEM_Voice) RequestVoiceTest();
    m->set_voicereqeusttype(RequestVoiceTest_VoiceReqeustType_VoiceRequestType_VoiceTestInit);
    m->set_voiceid(1);
    m_pVP_ReceiverCb->OnRecvVoiceTest(context, m);
}

TEST_F(VP_ReceiverCb_Test, OnRecvVoiceTest_NullCheck)
{
    void *m = nullptr;
    EV_EventContext context;
    m_pVP_ReceiverCb->OnRecvVoiceTest(context, m);
}

TEST_F(VP_ReceiverCb_Test, OnRecvSetMute)
{
    GuidanceVoiceMuteSettingRequest *m = new(MEM_Voice) GuidanceVoiceMuteSettingRequest();
    m->set_mutesetting_value(GuidanceVoiceMuteSettingRequest_MuteSettingValue_OFF);
    m_pVP_ReceiverCb->OnRecvSetMute(m);
}

TEST_F(VP_ReceiverCb_Test, OnRecvSetMute_NullCheck)
{
    void *m = nullptr;
    m_pVP_ReceiverCb->OnRecvSetMute(m);
}

TEST_F(VP_ReceiverCb_Test, OnRecvTtsServiceStatus_NullCheck)
{
    void *m = nullptr;
    m_pVP_ReceiverCb->OnRecvTtsServiceReady(m);
}

TEST_F(VP_ReceiverCb_Test, OnRecvTtsServiceStatus)
{
    TtsRespServiceStatus *m = new(MEM_Voice) TtsRespServiceStatus();
    m->set_sender("TTS_MAIN");
    m_pVP_ReceiverCb->OnRecvTtsServiceReady(m);
}

TEST_F(VP_ReceiverCb_Test, OnRecvTtsServiceStatusWithErrorSender)
{
    TtsRespServiceStatus *m = new(MEM_Voice) TtsRespServiceStatus();
    m->set_sender("TTS_TEST");
    m_pVP_ReceiverCb->OnRecvTtsServiceReady(m);
}

/* EOF */
