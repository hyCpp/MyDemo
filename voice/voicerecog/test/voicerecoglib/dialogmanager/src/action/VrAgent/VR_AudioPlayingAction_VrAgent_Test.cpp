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

#include "VR_AvcLanSenderIF.h"
#include "VR_AudioPlayingAction_VrAgent.h"
#include "VR_ActionEventSender.h"
#include "VR_DialogEngine_mock.h"
#include "VR_AvcLanSender_mock.h"
#include "VR_AvcLanSenderIF.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_AudioPlayingAction_VrAgent_Test
*
* The class is just for VR_AudioPlayingAction_VrAgent test.
*/
class VR_AudioPlayingAction_VrAgent_Test : public testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

public:
    spVR_ActionEventSender m_sp_EventSender;
    VR_AudioPlayingAction_VrAgent *m_audioPlayAction;
};

void VR_AudioPlayingAction_VrAgent_Test::SetUp()
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrPlayAlbum> protoMsg(VrPlayAlbum::default_instance().New());
    protoMsg->set_source_type(1);
    protoMsg->set_source_id(1);
    protoMsg->set_shuffle_on(1);
    protoMsg->set_album_name("album");
    protoMsg->set_artist_name("artist");
    protoMsg->set_album_id(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioPlayAction = VR_new VR_AudioPlayingAction_VrAgent(m_sp_EventSender, 1, PlayAlbum, *protoMsg, avclanSender);
}

void VR_AudioPlayingAction_VrAgent_Test::TearDown()
{
    if (NULL != m_audioPlayAction) {
        delete m_audioPlayAction;
        m_audioPlayAction = NULL;
    }
}

TEST_F(VR_AudioPlayingAction_VrAgent_Test, Run_PlayAlbum_shuffle)
{
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrAgent_Test, Run_PlayAlbum)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrPlayAlbum> protoMsg(VrPlayAlbum::default_instance().New());
    protoMsg->set_source_type(1);
    protoMsg->set_source_id(1);
    protoMsg->set_shuffle_on(0);
    protoMsg->set_album_name("album");
    protoMsg->set_artist_name("artist");
    protoMsg->set_album_id(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioPlayAction = VR_new VR_AudioPlayingAction_VrAgent(m_sp_EventSender, 1, PlayAlbum, *protoMsg, avclanSender);
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrAgent_Test, Run_PlayArtist_shuffle)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrPlayArtist> protoMsg(VrPlayArtist::default_instance().New());
    protoMsg->set_source_type(1);
    protoMsg->set_source_id(1);
    protoMsg->set_shuffle_on(1);
    protoMsg->set_artist_name("artist");
    protoMsg->set_artist_id(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioPlayAction = VR_new VR_AudioPlayingAction_VrAgent(m_sp_EventSender, 1, PlayArtist, *protoMsg, avclanSender);
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrAgent_Test, Run_PlayArtist)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrPlayArtist> protoMsg(VrPlayArtist::default_instance().New());
    protoMsg->set_source_type(1);
    protoMsg->set_source_id(1);
    protoMsg->set_shuffle_on(0);
    protoMsg->set_artist_name("artist");
    protoMsg->set_artist_id(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioPlayAction = VR_new VR_AudioPlayingAction_VrAgent(m_sp_EventSender, 1, PlayArtist, *protoMsg, avclanSender);
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrAgent_Test, Run_PlayComposer_shuffle)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrPlayComposer> protoMsg(VrPlayComposer::default_instance().New());
    protoMsg->set_source_type(1);
    protoMsg->set_source_id(1);
    protoMsg->set_shuffle_on(1);
    protoMsg->set_composer_name("composer");
    protoMsg->set_composer_id(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioPlayAction = VR_new VR_AudioPlayingAction_VrAgent(m_sp_EventSender, 1, PlayComposer, *protoMsg, avclanSender);
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrAgent_Test, Run_PlayComposer)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrPlayComposer> protoMsg(VrPlayComposer::default_instance().New());
    protoMsg->set_source_type(1);
    protoMsg->set_source_id(1);
    protoMsg->set_shuffle_on(0);
    protoMsg->set_composer_name("composer");
    protoMsg->set_composer_id(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioPlayAction = VR_new VR_AudioPlayingAction_VrAgent(m_sp_EventSender, 1, PlayComposer, *protoMsg, avclanSender);
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrAgent_Test, Run_PlayPlaylist_shuffle)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrPlayPlaylist> protoMsg(VrPlayPlaylist::default_instance().New());
    protoMsg->set_source_type(1);
    protoMsg->set_source_id(1);
    protoMsg->set_shuffle_on(1);
    protoMsg->set_playlist_name("playlist");
    protoMsg->set_playlist_id(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioPlayAction = VR_new VR_AudioPlayingAction_VrAgent(m_sp_EventSender, 1, PlayPlaylist, *protoMsg, avclanSender);
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrAgent_Test, Run_PlayPlaylist)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrPlayPlaylist> protoMsg(VrPlayPlaylist::default_instance().New());
    protoMsg->set_source_type(1);
    protoMsg->set_source_id(1);
    protoMsg->set_shuffle_on(0);
    protoMsg->set_playlist_name("playlist");
    protoMsg->set_playlist_id(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioPlayAction = VR_new VR_AudioPlayingAction_VrAgent(m_sp_EventSender, 1, PlayPlaylist, *protoMsg, avclanSender);
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrAgent_Test, Run_PlayAll_shuffle)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrPlayAll> protoMsg(VrPlayAll::default_instance().New());
    protoMsg->set_source_type(1);
    protoMsg->set_source_id(1);
    protoMsg->set_shuffle_on(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioPlayAction = VR_new VR_AudioPlayingAction_VrAgent(m_sp_EventSender, 1, PlayAll, *protoMsg, avclanSender);
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrAgent_Test, Run_PlayAll)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrPlayAll> protoMsg(VrPlayAll::default_instance().New());
    protoMsg->set_source_type(1);
    protoMsg->set_source_id(1);
    protoMsg->set_shuffle_on(0);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioPlayAction = VR_new VR_AudioPlayingAction_VrAgent(m_sp_EventSender, 1, PlayAll, *protoMsg, avclanSender);
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrAgent_Test, Run_PlayPodcast)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrPlayPodcast> protoMsg(VrPlayPodcast::default_instance().New());
    protoMsg->set_source_type(1);
    protoMsg->set_source_id(1);
    protoMsg->set_podcast_name("podcast");
    protoMsg->set_podcast_id(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioPlayAction = VR_new VR_AudioPlayingAction_VrAgent(m_sp_EventSender, 1, PlayPodcast, *protoMsg, avclanSender);
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrAgent_Test, Run_PlayAudioBook)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrPlayAudioBook> protoMsg(VrPlayAudioBook::default_instance().New());
    protoMsg->set_source_type(1);
    protoMsg->set_source_id(1);
    protoMsg->set_audio_book_name("audio_book");
    protoMsg->set_audio_book_id(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioPlayAction = VR_new VR_AudioPlayingAction_VrAgent(m_sp_EventSender, 1, PlayAudioBook, *protoMsg, avclanSender);
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrAgent_Test, Run_PlayGenre)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrPlayGenre> protoMsg(VrPlayGenre::default_instance().New());
    protoMsg->set_source_type(1);
    protoMsg->set_source_id(1);
    protoMsg->set_shuffle_on(1);
    protoMsg->set_genre_name("genre");
    protoMsg->set_genre_id(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioPlayAction = VR_new VR_AudioPlayingAction_VrAgent(m_sp_EventSender, 1, PlayGenre, *protoMsg, avclanSender);
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrAgent_Test, Run_PlaySong)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrPlaySong> protoMsg(VrPlaySong::default_instance().New());
    protoMsg->set_source_type(1);
    protoMsg->set_source_id(1);
    protoMsg->set_song_id(1);
    protoMsg->set_song_name("song");
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioPlayAction = VR_new VR_AudioPlayingAction_VrAgent(m_sp_EventSender, 1, PlaySong, *protoMsg, avclanSender);
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrAgent_Test, Run_PlayPodcastItem)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrPlayPodcastItem> protoMsg(VrPlayPodcastItem::default_instance().New());
    protoMsg->set_source_type(1);
    protoMsg->set_source_id(1);
    protoMsg->set_podcast_name("podcast");
    protoMsg->set_episode_name("episode");
    protoMsg->set_podcast_id(1);
    protoMsg->set_episode_id(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioPlayAction = VR_new VR_AudioPlayingAction_VrAgent(m_sp_EventSender, 1, PlayPodcastItem, *protoMsg, avclanSender);
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrAgent_Test, Run_default)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrPlayArtist> protoMsg(VrPlayArtist::default_instance().New());
    protoMsg->set_source_type(1);
    protoMsg->set_source_id(1);
    protoMsg->set_shuffle_on(0);
    protoMsg->set_artist_name("artist");
    protoMsg->set_artist_id(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioPlayAction = VR_new VR_AudioPlayingAction_VrAgent(m_sp_EventSender, 1, AudioOFF, *protoMsg, avclanSender);
    m_audioPlayAction->Run();
    SUCCEED();
    SUCCEED();
}

TEST_F(VR_AudioPlayingAction_VrAgent_Test, OnReceive_Case)
{
    std::unique_ptr<VrRecogResultConfirm> protoMsg(VrRecogResultConfirm::default_instance().New());
    protoMsg->set_action_id(ChangeHDSubChannel);
    protoMsg->set_processing_result(true);

    m_audioPlayAction->OnReceive(*protoMsg);
    SUCCEED();
}

/* EOF */
