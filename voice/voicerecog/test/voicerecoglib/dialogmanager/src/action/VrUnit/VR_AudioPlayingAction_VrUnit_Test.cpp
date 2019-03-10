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
#include "VR_AudioPlayingAction_VrUnit.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrTtsCommand.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_AudioPlayingAction_VrUnit_Test
*
* The class is just for VR_AudioPlayingAction_VrUnit test.
*/
class VR_AudioPlayingAction_VrUnit_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_AudioPlayingAction_VrUnit *m_action;
};

void
VR_AudioPlayingAction_VrUnit_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = PlayAlbum;
     std::unique_ptr<VrPlayAlbum> ProtoMsg(VrPlayAlbum::default_instance().New());
     ProtoMsg->set_source_type(1);
     ProtoMsg->set_source_id(1);
     ProtoMsg->set_shuffle_on(1);
     ProtoMsg->set_album_name("album");
     ProtoMsg->set_artist_name("artist");
     ProtoMsg->set_album_id(1);
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
     m_action = VR_new VR_AudioPlayingAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
}

void
VR_AudioPlayingAction_VrUnit_Test::TearDown()
{
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
}

TEST_F(VR_AudioPlayingAction_VrUnit_Test, Run_PlayAlbum)
{
    m_action->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrUnit_Test, Run_PlayAll)
{
    int nEngineId = 101;
    int nActionType = PlayAll;
    std::unique_ptr<VrPlayAll> ProtoMsg(VrPlayAll::default_instance().New());
    ProtoMsg->set_source_type(1);
    ProtoMsg->set_source_id(1);
    ProtoMsg->set_shuffle_on(1);
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_AudioPlayingAction_VrUnit *m_action1 = VR_new VR_AudioPlayingAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action1->Run();
    SUCCEED();
    if (NULL != m_action1) {
        delete m_action1;
        m_action1 = NULL;
    }
}

TEST_F(VR_AudioPlayingAction_VrUnit_Test, Run_PlayArtist)
{
    int nEngineId = 101;
    int nActionType = PlayArtist;
    std::unique_ptr<VrPlayArtist> ProtoMsg(VrPlayArtist::default_instance().New());
    ProtoMsg->set_source_type(1);
    ProtoMsg->set_source_id(1);
    ProtoMsg->set_shuffle_on(1);
    ProtoMsg->set_artist_name("artist");
    ProtoMsg->set_artist_id(1);
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_AudioPlayingAction_VrUnit *m_action1 = VR_new VR_AudioPlayingAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action1->Run();
    SUCCEED();
    if (NULL != m_action1) {
        delete m_action1;
        m_action1 = NULL;
    }
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrUnit_Test, Run_PlayAudioBook)
{
    int nEngineId = 101;
    int nActionType = PlayAudioBook;
    std::unique_ptr<VrPlayAudioBook> ProtoMsg(VrPlayAudioBook::default_instance().New());
    ProtoMsg->set_source_type(1);
    ProtoMsg->set_source_id(1);
    ProtoMsg->set_audio_book_name("audioBook");
    ProtoMsg->set_audio_book_id(1);
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_AudioPlayingAction_VrUnit *m_action1 = VR_new VR_AudioPlayingAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action1->Run();
    SUCCEED();
    if (NULL != m_action1) {
        delete m_action1;
        m_action1 = NULL;
    }
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrUnit_Test, Run_PlayComposer)
{
    int nEngineId = 101;
    int nActionType = PlayComposer;
    std::unique_ptr<VrPlayComposer> ProtoMsg(VrPlayComposer::default_instance().New());
    ProtoMsg->set_source_type(1);
    ProtoMsg->set_source_id(1);
    ProtoMsg->set_shuffle_on(1);
    ProtoMsg->set_composer_name("composer");
    ProtoMsg->set_composer_id(1);
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_AudioPlayingAction_VrUnit *m_action1 = VR_new VR_AudioPlayingAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action1->Run();
    SUCCEED();
    if (NULL != m_action1) {
        delete m_action1;
        m_action1 = NULL;
    }
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrUnit_Test, Run_PlayGenre)
{
    int nEngineId = 101;
    int nActionType = PlayGenre;
    std::unique_ptr<VrPlayGenre> ProtoMsg(VrPlayGenre::default_instance().New());
    ProtoMsg->set_source_type(1);
    ProtoMsg->set_source_id(1);
    ProtoMsg->set_shuffle_on(1);
    ProtoMsg->set_genre_name("genre");
    ProtoMsg->set_genre_id(1);
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_AudioPlayingAction_VrUnit *m_action1 = VR_new VR_AudioPlayingAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action1->Run();
    SUCCEED();
    if (NULL != m_action1) {
        delete m_action1;
        m_action1 = NULL;
    }
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrUnit_Test, Run_PlayPlaylist)
{
    int nEngineId = 101;
    int nActionType = PlayPlaylist;
    std::unique_ptr<VrPlayPlaylist> ProtoMsg(VrPlayPlaylist::default_instance().New());
    ProtoMsg->set_source_type(1);
    ProtoMsg->set_source_id(1);
    ProtoMsg->set_shuffle_on(1);
    ProtoMsg->set_playlist_name("playlist");
    ProtoMsg->set_playlist_id(1);
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_AudioPlayingAction_VrUnit *m_action1 = VR_new VR_AudioPlayingAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action1->Run();
    SUCCEED();
    if (NULL != m_action1) {
        delete m_action1;
        m_action1 = NULL;
    }
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrUnit_Test, Run_PlayPodcast)
{
    int nEngineId = 101;
    int nActionType = PlayPodcast;
    std::unique_ptr<VrPlayPodcast> ProtoMsg(VrPlayPodcast::default_instance().New());
    ProtoMsg->set_source_type(1);
    ProtoMsg->set_source_id(1);
    ProtoMsg->set_podcast_name("podcast");
    ProtoMsg->set_podcast_id(1);
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_AudioPlayingAction_VrUnit *m_action1 = VR_new VR_AudioPlayingAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action1->Run();
    SUCCEED();
    if (NULL != m_action1) {
        delete m_action1;
        m_action1 = NULL;
    }
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrUnit_Test, Run_PlayPodcastItem)
{
    int nEngineId = 101;
    int nActionType = PlayPodcastItem;
    std::unique_ptr<VrPlayPodcastItem> ProtoMsg(VrPlayPodcastItem::default_instance().New());
    ProtoMsg->set_source_type(1);
    ProtoMsg->set_source_id(1);
    ProtoMsg->set_podcast_name("podcast");
    ProtoMsg->set_episode_name("episode");
    ProtoMsg->set_podcast_id(1);
    ProtoMsg->set_episode_id(1);
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_AudioPlayingAction_VrUnit *m_action1 = VR_new VR_AudioPlayingAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action1->Run();
    SUCCEED();
    if (NULL != m_action1) {
        delete m_action1;
        m_action1 = NULL;
    }
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrUnit_Test, Run_PlaySong)
{
    int nEngineId = 101;
    int nActionType = PlaySong;
    std::unique_ptr<VrPlaySong> ProtoMsg(VrPlaySong::default_instance().New());
    ProtoMsg->set_source_type(1);
    ProtoMsg->set_source_id(1);
    ProtoMsg->set_song_id(1);
    ProtoMsg->set_song_name("song");
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_AudioPlayingAction_VrUnit *m_action1 = VR_new VR_AudioPlayingAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action1->Run();
    SUCCEED();
    if (NULL != m_action1) {
        delete m_action1;
        m_action1 = NULL;
    }
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrUnit_Test, Run_default)
{
    int nEngineId = 101;
    int nActionType = AudioOFF;
    std::unique_ptr<VrPlaySong> ProtoMsg(VrPlaySong::default_instance().New());
    ProtoMsg->set_source_type(1);
    ProtoMsg->set_source_id(1);
    ProtoMsg->set_song_id(1);
    ProtoMsg->set_song_name("song");
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_AudioPlayingAction_VrUnit *m_action1 = VR_new VR_AudioPlayingAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action1->Run();
    SUCCEED();
    if (NULL != m_action1) {
        delete m_action1;
        m_action1 = NULL;
    }
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrUnit_Test, OnReceive_Case)
{
    std::unique_ptr<VrRecogResultConfirm> protoMsg(VrRecogResultConfirm::default_instance().New());
    protoMsg->set_action_id(PlayAlbum);
    protoMsg->set_processing_result(true);

    m_action->OnReceive(*protoMsg);
    SUCCEED();
}
/* EOF */


