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
#ifndef GTEST_INCLUDE_GTEST_GTEST_H_
#    include "gtest/gtest.h"
#endif

#ifndef GMOCK_INCLUDE_GMOCK_GMOCK_H_
#    include "gmock/gmock.h"
#endif

#include "MEM_list.h"
#include "EV_EventContext.h"
#include "navi/Voice/VoicePlay/VpPlaySoundId.pb.h"
#include "navi/Voice/VoicePlay/VpPlaySoundTts.pb.h"
#include "navi/Voice/VoicePlay/VpPlaySoundList.pb.h"
#include "navi/Voice/VoicePlay/VpStopPlay.pb.h"
#include "navi/Voice/VoicePlay/VpGetDuration.pb.h"
#include "navi/Voice/VoicePlay/VpDuration.pb.h"
#include "navi/Voice/TTS/TtsOnPlayEnd.pb.h"
#include "navi/Debug/RequestVoiceTest.pb.h"
#include "navi/Debug/ResponseVoiceTest.pb.h"
#include "navi/AvcProxy/GuidanceVoiceMuteSettingRequest.pb.h"
#include "navi/AvcProxy/GuidanceVoiceMuteSettingResponse.pb.h"
#include "navi/Setting/SettingChange.pb.h"
#include "VP_Request.h"
#include "VP_ChannelManager_mock.h"
#include "VP_DataManager_mock.h"
#include "VP_PhraseImp_mock.h"

using namespace ::navi::VoicePlay;
using namespace ::navi::TTS;
using namespace ::navi::Debug;
using namespace ::navi::AvcProxy;
using namespace ::navi::Setting;
using namespace std;
using ::testing::_;
using ::testing::Return;

/**
 * This is a test class for VP_SoundPlayId_Request_Test
 *
 * This class is used to execute a unit test for class 
 * VP_SoundPlayId_Request_Test 
 */
class VP_SoundPlayId_Request_Test : public testing::Test
{
public:
    virtual void SetUp()
    {
        msg = new(MEM_Voice) VpPlaySoundId();
        msg->set_sender("Guide");
        msg->set_userdata(0);
        msg->set_soundid(0x1c101102);
        m_pVP_Request = make_shared<VP_SoundPlayId_Request>(msg);

        dataMgr = make_shared<MockVP_DataManager>();
        shared_ptr<VP_VoicePlayControl> playControl = make_shared<VP_VoicePlayControl>(dataMgr, nullptr);
        m_channelMgr = make_shared<MockVP_ChannelManager>();
        playControl->m_channelMgr = m_channelMgr;
        m_pVP_Request->m_pControl = playControl;
        m_pVP_Request->m_pDataMgr = dataMgr;
    }

    virtual void TearDown() 
    {
        delete msg;
        msg = nullptr;
    }

private:
    VpPlaySoundId *msg; 
    shared_ptr<MockVP_DataManager> dataMgr;
    shared_ptr<MockVP_ChannelManager> m_channelMgr;
    shared_ptr<VP_SoundPlayId_Request> m_pVP_Request;
};

/**
 * This is a test class for VP_SoundPlayTts_Request_Test 
 *
 * This class is used to execute a unit test for class 
 * VP_SoundPlayTts_Request_Test 
 */
class VP_SoundPlayTts_Request_Test : public testing::Test
{
public:
    virtual void SetUp()
    {
        msg = new(MEM_Voice) VpPlaySoundTts();
        msg->set_sender("Guide");
        msg->set_userdata(0);
        msg->set_text("Hello");
        m_pVP_Request = make_shared<VP_SoundPlayTts_Request>(msg, "ENG");
        
        dataMgr = make_shared<MockVP_DataManager>();
        shared_ptr<VP_VoicePlayControl> playControl = make_shared<VP_VoicePlayControl>(dataMgr, nullptr);
        m_channelMgr = make_shared<MockVP_ChannelManager>();
        playControl->m_channelMgr = m_channelMgr;
        m_pVP_Request->m_pControl = playControl;
        m_pVP_Request->m_pDataMgr = dataMgr;
    }

    virtual void TearDown() 
    {
        delete msg;
        msg = nullptr;
    }

private:
    VpPlaySoundTts *msg;
    shared_ptr<MockVP_DataManager> dataMgr;
    shared_ptr<MockVP_ChannelManager> m_channelMgr;
    shared_ptr<VP_AbstractRequest> m_pVP_Request;
};

/**
 * This is a test class for VP_SoundPlayList_Request_Test 
 *
 * This class is used to execute a unit test for class 
 * VP_SoundPlayList_Request_Test 
 */
class VP_SoundPlayList_Request_Test : public testing::Test
{
public:
    virtual void SetUp()
    {
        soundList = new(MEM_Voice) VpPlaySoundList();

        VpPlaySoundList_VpPhrase* phrase = soundList->add_phrases();
        VpPlaySoundList_VpPhrase* ttsPhrase = soundList->add_phrases();

        // this two phrases are used to cover more condition
        VpPlaySoundList_VpPhrase* more_wavePhrase = soundList->add_phrases();
        VpPlaySoundList_VpPhrase* more_ttsPhrase = soundList->add_phrases();
 
        ttsPhrase->set_text("Test for playing TTS");
        ttsPhrase->set_ttstype(navi::VoicePlay::VpPlaySoundList_VpPhrase_VP_TTS_TYPE_VP_TTS_GENERAL);
        ttsPhrase->set_type(navi::VoicePlay::VpPlaySoundList_VpPhrase_VP_SOUND_TYPE_VP_SOUND_TTS);
 
        phrase->set_soundid(0x00301001);
        phrase->set_type(navi::VoicePlay::VpPlaySoundList_VpPhrase_VP_SOUND_TYPE_VP_SOUND_ID);

        more_ttsPhrase->set_text("More TTS Phrase");
        more_ttsPhrase->set_ttstype(navi::VoicePlay::VpPlaySoundList_VpPhrase_VP_TTS_TYPE_VP_TTS_GENERAL);
        more_ttsPhrase->set_type(navi::VoicePlay::VpPlaySoundList_VpPhrase_VP_SOUND_TYPE_VP_SOUND_TTS);
 
        more_wavePhrase->set_soundid(0x00301003);
        more_wavePhrase->set_type(navi::VoicePlay::VpPlaySoundList_VpPhrase_VP_SOUND_TYPE_VP_SOUND_ID);
 
        soundList->set_sender("Guide");
        soundList->set_userdata(0);

        m_pVP_Request = make_shared<VP_SoundPlayList_Request>(soundList, "ENG");

        dataMgr = make_shared<MockVP_DataManager>();
        shared_ptr<VP_VoicePlayControl> playControl = make_shared<VP_VoicePlayControl>(dataMgr, nullptr);
        m_channelMgr = make_shared<MockVP_ChannelManager>();
        playControl->m_channelMgr = m_channelMgr;
        m_pVP_Request->m_pControl = playControl;
        m_pVP_Request->m_pDataMgr = dataMgr;
    }

    virtual void TearDown() 
    {
        delete soundList;
        soundList = nullptr;
    }

private:
    VpPlaySoundList *soundList;
    shared_ptr<MockVP_DataManager> dataMgr;
    shared_ptr<MockVP_ChannelManager> m_channelMgr;
    shared_ptr<VP_SoundPlayList_Request> m_pVP_Request;
};

/**
 * This is a test class for VP_StopPlay_Request_Test 
 *
 * This class is used to execute a unit test for class 
 * VP_StopPlay_Request_Test 
 */
class VP_StopPlay_Request_Test : public testing::Test
{
public:
    virtual void SetUp()
    {
        msg = new(MEM_Voice) VpStopPlay();
        msg->set_sender("Guide");
        msg->set_userdata(0);
        m_pVP_Request = make_shared<VP_StopPlay_Request>(msg);

        dataMgr = make_shared<MockVP_DataManager>();
        shared_ptr<VP_VoicePlayControl> playControl = make_shared<VP_VoicePlayControl>(dataMgr, nullptr);
        m_channelMgr = make_shared<MockVP_ChannelManager>();
        playControl->m_channelMgr = m_channelMgr;
        m_pVP_Request->m_pControl = playControl;
        m_pVP_Request->m_pDataMgr = dataMgr;
    }

    virtual void TearDown() 
    {
        delete msg;
        msg = nullptr;
    }

private:
    VpStopPlay *msg;
    shared_ptr<MockVP_DataManager> dataMgr;
    shared_ptr<MockVP_ChannelManager> m_channelMgr;
    shared_ptr<VP_StopPlay_Request> m_pVP_Request;
};

/**
 * This is a test class for VP_GetDuration_Request_Test 
 *
 * This class is used to execute a unit test for class 
 * VP_GetDuration_Request_Test 
 */
class VP_GetDuration_Request_Test : public testing::Test
{
public:
    virtual void SetUp()
    {
        msg = new(MEM_Voice) VpGetDuration();
        msg->set_sender("VPTest");
        msg->set_userdata(0);
        m_pVP_Request = make_shared<VP_GetDuration_Request>(msg);
    }

    virtual void TearDown() 
    {
        delete msg;
        msg = nullptr;
    }

private:
    VpGetDuration *msg;
    shared_ptr<VP_GetDuration_Request> m_pVP_Request;
};

/**
 * This is a test class for VP_TtsPlayEnd_Request_Test 
 *
 * This class is used to execute a unit test for class 
 * VP_TtsPlayEnd_Request_Test 
 */
class VP_TtsPlayEnd_Request_Test : public testing::Test
{
public:
    virtual void SetUp()
    {
        msg = new(MEM_Voice) TtsOnPlayEnd();
        msg->set_sender("VPTest");
        msg->set_result(TtsOnPlayEnd::TTS_PLAY_RESULT_OK);
        msg->set_reqid(1);
        m_pVP_Request = make_shared<VP_TtsPlayEnd_Request>(msg);
    }

    virtual void TearDown() 
    {
        delete msg;
        msg = nullptr;
    }

private:
    TtsOnPlayEnd *msg;
    shared_ptr<VP_TtsPlayEnd_Request> m_pVP_Request;
};

/**
 * This is a test class for VP_TtsPlayEnd_Request_Test 
 *
 * This class is used to execute a unit test for class 
 * VP_TtsPlayEnd_Request_Test 
 */
class VP_VoiceTest_Request_Test : public testing::Test
{
public:
    virtual void SetUp()
    {
        msg = new(MEM_Voice) RequestVoiceTest();

        dataMgr = make_shared<MockVP_DataManager>();
        playControl = make_shared<VP_VoicePlayControl>(dataMgr, nullptr);
        m_channelMgr = make_shared<MockVP_ChannelManager>();
        playControl->m_channelMgr = m_channelMgr;
    }

    virtual void TearDown()
    {
        delete msg;
        msg = nullptr;
    }

private:
    EV_EventContext context;
    RequestVoiceTest *msg;
    shared_ptr<MockVP_DataManager> dataMgr;
    shared_ptr<MockVP_ChannelManager> m_channelMgr;
    shared_ptr<VP_VoiceTest_Request> m_pVP_Request;
    shared_ptr<VP_VoicePlayControl> playControl;
};

/**
 * This is a test class for VP_SetMute_Request 
 *
 * This class is used to execute a unit test for class 
 * VP_SetMute_Request 
 */
class VP_SetMute_Request_Test : public testing::Test
{
public:
    virtual void SetUp()
    {
        msg = new(MEM_Voice) GuidanceVoiceMuteSettingRequest();
    }

    virtual void TearDown()
    {
        delete msg;
        msg = nullptr;
    }

private:
    GuidanceVoiceMuteSettingRequest *msg;
    shared_ptr<VP_SetMute_Request> m_pVP_Request;
};

/**
 * This is a test class for VP_LanguageChange_Request
 *
 * This class is used to execute a unit test for class
 * VP_LanguageChange_Request
 */
class VP_LanguageChange_Request_Test : public testing::Test
{
public:
    virtual void SetUp()
    {
        msg = new(MEM_Voice) SettingChange();
        m_pVP_Request = make_shared<VP_LanguageChange_Request>(msg);
    }

    virtual void TearDown()
    {
        delete msg;
        msg = nullptr;
    }

private:
    SettingChange *msg;
    shared_ptr<VP_LanguageChange_Request> m_pVP_Request;
};

TEST_F(VP_SoundPlayId_Request_Test, commit)
{
    CL_ERROR err = m_pVP_Request->commit();
    EXPECT_EQ(err, APL_ERR_NO_ERROR);
}

TEST_F(VP_SoundPlayList_Request_Test, GetTtsPhonemePhrase)
{
    VpPlaySoundList_VpPhrase phrase;
    phrase.set_text("Test for playing TTS");
    phrase.set_ttstype(navi::VoicePlay::VpPlaySoundList_VpPhrase_VP_TTS_TYPE_VP_TTS_PHONEME);
    phrase.set_type(navi::VoicePlay::VpPlaySoundList_VpPhrase_VP_SOUND_TYPE_VP_SOUND_TTS);
    
    string result;
    m_pVP_Request->GetTtsPhrase(phrase, result);
    EXPECT_EQ(result, "<TTS_Phoneme>Test for playing TTS</TTS_Phoneme>");
}

TEST_F(VP_SoundPlayList_Request_Test, GetTtsAddressPhrase)
{
    VpPlaySoundList_VpPhrase phrase;
    phrase.set_text("Test for playing TTS");
    phrase.set_ttstype(navi::VoicePlay::VpPlaySoundList_VpPhrase_VP_TTS_TYPE_VP_TTS_ADDRESS);
    phrase.set_type(navi::VoicePlay::VpPlaySoundList_VpPhrase_VP_SOUND_TYPE_VP_SOUND_TTS);
    
    string result;
    m_pVP_Request->GetTtsPhrase(phrase, result);
    EXPECT_EQ(result, "<TTS_Address>Test for playing TTS</TTS_Address>");
}

TEST_F(VP_SoundPlayTts_Request_Test, commit)
{
    CL_ERROR err = m_pVP_Request->commit();
    EXPECT_EQ(err, APL_ERR_NO_ERROR);
}

TEST_F(VP_SoundPlayList_Request_Test, commit)
{
    CL_ERROR err = m_pVP_Request->commit();
    EXPECT_EQ(err, APL_ERR_NO_ERROR);
}

TEST_F(VP_StopPlay_Request_Test, commit)
{
    EXPECT_CALL(*m_channelMgr, getChannel(_)).WillOnce(Return(make_shared<VP_Channel>("VPTest", nullptr)));
    CL_ERROR err = m_pVP_Request->commit();
    EXPECT_EQ(err, APL_ERR_NO_ERROR);
}

TEST_F(VP_StopPlay_Request_Test, commit_with_null_channel)
{
    EXPECT_CALL(*m_channelMgr, getChannel(_)).WillOnce(Return(nullptr));
    CL_ERROR err = m_pVP_Request->commit();
    EXPECT_EQ(err, APL_ERR_VP_INVALID_PARAM);
}

TEST_F(VP_GetDuration_Request_Test, GetPhraseList)
{
    VpGetDuration *msg = new(MEM_Voice)VpGetDuration();
    VpGetDuration_VpPhrase *phrase = msg->add_phrases();
    VpGetDuration_VpPhrase *tts_phrase = msg->add_phrases();

    tts_phrase->set_text("Test for getting duration");
    tts_phrase->set_type(navi::VoicePlay::VpGetDuration_VpPhrase_VP_SOUND_TYPE_VP_SOUND_TTS);

    phrase->set_soundid(0x00301001);
    phrase->set_type(navi::VoicePlay::VpGetDuration_VpPhrase_VP_SOUND_TYPE_VP_SOUND_ID);

    m_pVP_Request->GetPhraseList(msg);
    delete msg;
}

TEST_F(VP_GetDuration_Request_Test, commit)
{
    CL_ERROR err = m_pVP_Request->commit();
}

TEST_F(VP_TtsPlayEnd_Request_Test, commit)
{
    CL_ERROR err = m_pVP_Request->commit();
    EXPECT_EQ(err, APL_ERR_NO_ERROR);
}

TEST_F(VP_VoiceTest_Request_Test, commit_init)
{
    msg->set_voicereqeusttype(RequestVoiceTest_VoiceReqeustType_VoiceRequestType_VoiceTestInit);
    m_pVP_Request = make_shared<VP_VoiceTest_Request>(msg);
    CL_ERROR error = m_pVP_Request->commit(context);
    EXPECT_EQ(error, APL_ERR_NO_ERROR);
}

TEST_F(VP_VoiceTest_Request_Test, commit_init_without_reply)
{
    msg->set_voicereqeusttype(RequestVoiceTest_VoiceReqeustType_VoiceRequestType_VoiceTestInit);
    m_pVP_Request = make_shared<VP_VoiceTest_Request>(msg);
    context.pReplyCBFunc = nullptr;
    CL_ERROR error = m_pVP_Request->commit(context);
    EXPECT_EQ(error, APL_ERR_NO_ERROR);
}

TEST_F(VP_VoiceTest_Request_Test, commit_start)
{
    CL_ERROR error;
    EXPECT_CALL(*m_channelMgr, getChannel(_)).WillRepeatedly(Return(make_shared<VP_Channel>("VPTest", nullptr)));
    EXPECT_CALL(*dataMgr, GetGuidePhrase(_, _, _)).WillRepeatedly(Return(APL_ERR_NO_ERROR));
    for (int ix = VP_VoiceTest_Request::MIN_VOICE_ID; ix <= VP_VoiceTest_Request::MAX_VOICE_ID + 1; ++ix) {
        msg->set_voicereqeusttype(RequestVoiceTest_VoiceReqeustType_VoiceRequestType_VoiceTestStart);
        msg->set_voiceid(ix);
        shared_ptr<VP_VoiceTest_Request> request = make_shared<VP_VoiceTest_Request>(msg);

        request->m_pControl = playControl;
        request->m_pDataMgr = dataMgr;
        error = request->commit(context);
        EXPECT_EQ(error, APL_ERR_NO_ERROR);
    }
}

TEST_F(VP_VoiceTest_Request_Test, commit_stop)
{
    msg->set_voicereqeusttype(RequestVoiceTest_VoiceReqeustType_VoiceRequestType_VoiceTestStop);
    m_pVP_Request = make_shared<VP_VoiceTest_Request>(msg);
    m_pVP_Request->m_pControl = playControl;
    EXPECT_CALL(*m_channelMgr, getChannel(_)).WillOnce(Return(make_shared<VP_Channel>("VPTest", nullptr)));

    CL_ERROR error = m_pVP_Request->commit(context);
    EXPECT_EQ(error, APL_ERR_NO_ERROR);
}

TEST_F(VP_VoiceTest_Request_Test, commit_stop_with_null_channel)
{

    msg->set_voicereqeusttype(RequestVoiceTest_VoiceReqeustType_VoiceRequestType_VoiceTestStop);
    m_pVP_Request = make_shared<VP_VoiceTest_Request>(msg);
    m_pVP_Request->m_pControl = playControl;
    EXPECT_CALL(*m_channelMgr, getChannel(_)).WillOnce(Return(nullptr));

    CL_ERROR error = m_pVP_Request->commit(context);
    EXPECT_EQ(error, APL_ERR_VP_INVALID_PARAM);
}

TEST_F(VP_VoiceTest_Request_Test, commit_uninit)
{
    msg->set_voicereqeusttype(RequestVoiceTest_VoiceReqeustType_VoiceRequestType_VoiceTestUnInit);
    m_pVP_Request = make_shared<VP_VoiceTest_Request>(msg);
    m_pVP_Request->m_pControl = playControl;
    EXPECT_CALL(*m_channelMgr, getChannel(_)).WillOnce(Return(make_shared<VP_Channel>("VPTest", nullptr)));

    CL_ERROR error = m_pVP_Request->commit(context);
    EXPECT_EQ(error, APL_ERR_NO_ERROR);
}

TEST_F(VP_SetMute_Request_Test, commit_muteChannels)
{
    msg->set_mutesetting_value(GuidanceVoiceMuteSettingRequest_MuteSettingValue_ON);
    m_pVP_Request = make_shared<VP_SetMute_Request>(msg);

    CL_ERROR error = m_pVP_Request->commit();
    EXPECT_EQ(error, APL_ERR_NO_ERROR);
}

TEST_F(VP_SetMute_Request_Test, commit_resetChannels)
{
    msg->set_mutesetting_value(GuidanceVoiceMuteSettingRequest_MuteSettingValue_OFF);
    m_pVP_Request = make_shared<VP_SetMute_Request>(msg);

    CL_ERROR error = m_pVP_Request->commit();
    EXPECT_EQ(error, APL_ERR_NO_ERROR);
}

TEST_F(VP_LanguageChange_Request_Test, commit)
{
    m_pVP_Request->commit();
}

/* EOF */
