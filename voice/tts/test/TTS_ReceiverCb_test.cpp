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

#include "ttslib_test.h"
#include "TTS_ReceiverCb.h"
#include "TTS_IF.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventReceiver.h"
#include "TTS_EngineInfo.h"
#include "TTS_TextToSpeechImpl.h"
#include "navi/Voice/TTS/TtsBase.pb.h"
#include "navi/Voice/TTS/TtsBasePlay.pb.h"
#include "navi/Voice/TTS/TtsGetParam.pb.h"
#include "navi/Voice/TTS/TtsRespGetParam.pb.h"
#include "navi/Voice/TTS/TtsOnPlayEnd.pb.h"
#include "navi/Voice/TTS/TtsPlayFile.pb.h"
#include "navi/Voice/TTS/TtsPlayId.pb.h"
#include "navi/Voice/TTS/TtsPlayList.pb.h"
#include "navi/Voice/TTS/TtsPlayText.pb.h"
#include "navi/Voice/TTS/TtsSetParam.pb.h"
#include "navi/Voice/TTS/TtsRespSetParam.pb.h"
#include "navi/Voice/TTS/TtsStop.pb.h"
#include "navi/Voice/TTS/TtsGetDefaultRole.pb.h"
#include "navi/Voice/TTS/TtsListParamRange.pb.h"
#include "navi/Voice/TTS/TtsListRole.pb.h"
#include "navi/Voice/TTS/TtsPlaySilence.pb.h"
#include "navi/Voice/TTS/TtsRegisterSound.pb.h"
#include "navi/Voice/TTS/TtsRespGetDefaultRole.pb.h"
#include "navi/Voice/TTS/TtsRespListParamRange.pb.h"
#include "navi/Voice/TTS/TtsRespListRole.pb.h"
#include "navi/Voice/TTS/TtsRespRegisterSound.pb.h"
#include "navi/Voice/TTS/TtsRespUnRegisterSound.pb.h"
#include "navi/Voice/TTS/TtsRoleInfo.pb.h"
#include "navi/Voice/TTS/TtsUnRegisterSound.pb.h"
#include "navi/Voice/TTS/TtsIsSpeaking.pb.h"
#include "navi/Voice/TTS/TtsRespIsSpeaking.pb.h"
#include "navi/Voice/TTS/TtsGetMaxInputLength.pb.h"
#include "navi/Voice/TTS/TtsRespGetMaxInputLength.pb.h"
#include "navi/Voice/TTS/TtsSetLanguage.pb.h"
#include "navi/Voice/TTS/TtsRespSetLanguage.pb.h"
#include "navi/Voice/TTS/TtsSetVrMode.pb.h"
#include "navi/Voice/TTS/TtsSynthesizeToFile.pb.h"
#include "navi/Voice/TTS/TtsServiceStatus.pb.h"
#include "navi/Voice/TTS/TtsPause.pb.h"
#include "navi/Voice/TTS/TtsResume.pb.h"
#include "navi/Voice/TTS/TtsTuningData.pb.h"
#include "navi/Voice/TTS/TtsRespRegisterTuningData.pb.h"
#include "TTS_PlaylistInfo.h"
#include "TTS_TextToSpeechImpl.h"

using namespace ::navi::TTS;
using namespace std;
using ::testing::_;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;

// using testing::Times;

class MockTextToSpeech : public TTS_TextToSpeechImpl
{
public:
    MockTextToSpeech(TTS_TextToSpeechImpl::PlaybackListener* listener) : TTS_TextToSpeechImpl(listener)
    {
    }

    virtual ~MockTextToSpeech()
    {
    }

    MOCK_METHOD0(PreInitialize, void());
    MOCK_METHOD0(Initialize, void());
    MOCK_METHOD0(Start, void());
    MOCK_METHOD0(Stop, void());
    MOCK_METHOD0(Destory, void());
    MOCK_METHOD0(LoadEngine, void());

    MOCK_METHOD1(ListRole, void(TtsEngineRoles& roles));
    // MOCK_METHOD1(GetDefaultRole, CL_BOOL(TTS_RoleInfo *role));
    MOCK_METHOD1(CreatePlayList, TTS_PlaylistInfo*(const BL_String &sender));
    // MOCK_METHOD2(ListParamRange, void(TTS_ParamRange** params, int* count));
    MOCK_METHOD3(GetParameter, bool(const std::string& sender, const std::string& language, TTS_EngineLanguage& param));
    MOCK_METHOD3(SetParameter, bool(const std::string& sender, const std::string& language, const TTS_EngineLanguage& param));
    MOCK_METHOD3(RegisterSound, CL_ERROR(const BL_String &sender, const BL_String& soundFileName, int* soundId));
    MOCK_METHOD2(UnRegisterSound, CL_ERROR(const BL_String &sender, int soundId));
    MOCK_METHOD0(IsSpeaking, CL_BOOL());
    MOCK_METHOD5(SynthesizeToFile, CL_ERROR(const BL_String &sender, const BL_String &text,
        const BL_String &filePath, const TTS_POLICY &policy, int *reqId));

    MOCK_METHOD3(Play, CL_ERROR(TTS_PlaylistInfo* playlist, const TTS_POLICY &policy, int *reqId));
    MOCK_METHOD2(Stop, CL_ERROR(const BL_String &, int reqId));
    MOCK_METHOD1(StopAll, CL_ERROR(const BL_String &sender));
    MOCK_CONST_METHOD1(GetSpeechMaxInputLength, int(const BL_String &sender));
    MOCK_METHOD2(SetLanguage, bool(const BL_String &sender, const BL_String& language));

    MOCK_METHOD2(Pause, CL_ERROR(int reqId, const BL_String &sender));
    MOCK_METHOD2(Resume, CL_ERROR(int reqId, const BL_String &sender));
};

/**
* MockTTS_PlaylistInfo
*
* The class is just for TTS_PlaylistInfo test.
*/
class MockTTS_PlaylistInfo : public TTS_PlaylistInfo
{
public:
    MockTTS_PlaylistInfo(const BL_String &sender, TTS_TextToSpeechImpl* owner) : TTS_PlaylistInfo(sender, owner)
    {
    }

    virtual ~MockTTS_PlaylistInfo() 
    {
    }

    MOCK_METHOD3(AddText, CL_ERROR(const BL_String& text, const BL_String& lang, const BL_String& speaker_sex));
    MOCK_METHOD1(AddSound, CL_ERROR(int soundId));
    MOCK_METHOD1(AddSound, CL_ERROR(const BL_String& soundFileName));
    MOCK_METHOD1(AddSilence, CL_ERROR(int millisecond));
};

TEST(TTS_ReceiverCb_test, PreInitialize)
{
    MockTextToSpeech* mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    EXPECT_CALL(*mocktts, PreInitialize()).Times(1);

    cb.PreInitialize();
}

TEST(TTS_ReceiverCb_test, PreInitialize_NULL)
{
    TTS_ReceiverCb cb;
    cb.m_tts = NULL;
    cb.PreInitialize();
}

TEST(TTS_ReceiverCb_test, Initialize)
{
    MockTextToSpeech* mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    EXPECT_CALL(*mocktts, Initialize()).Times(1);

    cb.Initialize();
}

TEST(TTS_ReceiverCb_test, Initialize_NULL)
{
    TTS_ReceiverCb cb;
    cb.m_tts = NULL;
    cb.Initialize();
}

TEST(TTS_ReceiverCb_test, LoadEngine)
{
    MockTextToSpeech* mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    EXPECT_CALL(*mocktts, LoadEngine()).Times(1);

    cb.LoadEngine();
}

TEST(TTS_ReceiverCb_test, LoadEngine_NULL)
{
    TTS_ReceiverCb cb;
    cb.m_tts = NULL;
    cb.LoadEngine();
}

TEST(TTS_ReceiverCb_test, Start)
{
    MockTextToSpeech* mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    EXPECT_CALL(*mocktts, Start()).Times(1);
    cb.Start();
}

TEST(TTS_ReceiverCb_test, Start_NULL)
{
    TTS_ReceiverCb cb;
    cb.m_tts = NULL;
    cb.Start();
}

TEST(TTS_ReceiverCb_test, Stop)
{
    TTS_ReceiverCb cb;
    MockTextToSpeech* mocktts = new MockTextToSpeech(NULL);
    cb.m_tts = mocktts;
    EXPECT_CALL(*mocktts, Stop()).Times(1);
    cb.Stop();
}

TEST(TTS_ReceiverCb_test, Stop_NULL)
{
    TTS_ReceiverCb cb;
    cb.m_tts = NULL;
    cb.Stop();
}

TEST(TTS_ReceiverCb_test, Destory)
{
    MockTextToSpeech* mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    EXPECT_CALL(*mocktts, Destory()).Times(1);
    cb.Destory();
}

TEST(TTS_ReceiverCb_test, Destory_NULL)
{
    TTS_ReceiverCb cb;
    cb.m_tts = NULL;
    cb.m_listener = NULL;
    cb.Destory();
}

TEST(TTS_ReceiverCb_test, Register)
{
    TTS_ReceiverCb cb;
    EV_EventReceiver* er = new(MEM_Voice) EV_EventReceiver("tst");
    cb.Register(er);
    delete er;
}

TEST(TTS_ReceiverCb_test, Register_null)
{
    TTS_ReceiverCb cb;
    cb.Register(NULL);
}

TEST(TTS_ReceiverCb_test, OnRecvPlayText)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    BL_String text = "text";
    TTS_PlaylistInfo *playInfo = new(MEM_Voice) TTS_PlaylistInfo(sender, NULL);
    EXPECT_CALL(*mocktts, CreatePlayList(_)).WillOnce(Return(playInfo));
    TtsPlayText *playText = new(MEM_Voice) TtsPlayText();
    playText->set_text(text);
    TtsBasePlay *basePlay = playText->mutable_play();
    TtsBase *base = basePlay->mutable_base();
    base->set_sender(sender);
    base->set_onresponse(CL_TRUE);   
    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(1);
    cb.OnRecvPlayText(playText);
    delete playText;
    delete playInfo;
}

TEST(TTS_ReceiverCb_test, OnRecvPlayText_CreatePlayList_Null)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    BL_String text = "text";
    TTS_PlaylistInfo *playInfo = NULL; 
    EXPECT_CALL(*mocktts, CreatePlayList(_)).WillOnce(Return(playInfo));
    TtsPlayText *playText = new(MEM_Voice) TtsPlayText();
    playText->set_text(text);
    TtsBasePlay *basePlay = playText->mutable_play();
    TtsBase *base = basePlay->mutable_base();
    base->set_sender(sender);
    base->set_onresponse(CL_TRUE);   
    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(0);
    cb.OnRecvPlayText(playText);
}

TEST(TTS_ReceiverCb_test, OnRecvPlayText_nosender)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "";
    BL_String text = "text";
    TtsPlayText *playText = new(MEM_Voice) TtsPlayText();
    playText->set_text(text);
    TtsBasePlay *basePlay = playText->mutable_play();
    TtsBase *base = basePlay->mutable_base();
    base->set_sender(sender);
    base->set_onresponse(CL_TRUE);   
    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(0);
    cb.OnRecvPlayText(playText);
    delete playText;
}

TEST(TTS_ReceiverCb_test, OnRecvPlayText_has_language)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    BL_String text = "text";
    TTS_PlaylistInfo *playInfo = new(MEM_Voice) TTS_PlaylistInfo(sender, NULL);
    EXPECT_CALL(*mocktts, CreatePlayList(_)).WillOnce(Return(playInfo));
    TtsPlayText *playText = new(MEM_Voice) TtsPlayText();
    playText->set_text(text);
    TtsBasePlay *basePlay = playText->mutable_play();
    TtsBase *base = basePlay->mutable_base();
    base->set_sender(sender);
    base->set_onresponse(CL_TRUE);   
    basePlay->set_language("ENG");
    basePlay->set_streamtype(TtsBasePlay_TTS_STREAM_TYPE_TTS_STREAM_NORMAL);
    basePlay->set_tokenid(1);
    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(1);
    cb.OnRecvPlayText(playText);
    delete playText;
    delete playInfo;
}

TEST(TTS_ReceiverCb_test, OnRecvPlayText_language_empty)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    BL_String text = "text";
    TTS_PlaylistInfo *playInfo = new(MEM_Voice) TTS_PlaylistInfo(sender, NULL);
    EXPECT_CALL(*mocktts, CreatePlayList(_)).WillOnce(Return(playInfo));
    TtsPlayText *playText = new(MEM_Voice) TtsPlayText();
    playText->set_text(text);
    TtsBasePlay *basePlay = playText->mutable_play();
    TtsBase *base = basePlay->mutable_base();
    base->set_sender(sender);
    base->set_onresponse(CL_TRUE);   
    basePlay->set_language("");
    basePlay->set_streamtype(TtsBasePlay_TTS_STREAM_TYPE_TTS_STREAM_NORMAL);
    basePlay->set_tokenid(1);
    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(1);
    cb.OnRecvPlayText(playText);
    delete playText;
    delete playInfo;
}

TEST(TTS_ReceiverCb_test, OnRecvPlayText_noresponse)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    BL_String text = "text";
    TTS_PlaylistInfo *playInfo = new(MEM_Voice) TTS_PlaylistInfo(sender, NULL);
    EXPECT_CALL(*mocktts, CreatePlayList(_)).WillOnce(Return(playInfo));
    TtsPlayText *playText = new(MEM_Voice) TtsPlayText();
    playText->set_text(text);
    TtsBasePlay *basePlay = playText->mutable_play();
    TtsBase *base = basePlay->mutable_base();
    base->set_sender(sender);
    base->set_onresponse(CL_TRUE);   
    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(1);
    cb.OnRecvPlayText(playText);
    delete playText;
    delete playInfo;
}

TEST(TTS_ReceiverCb_test, OnRecvPlayText_null)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(0);
    cb.OnRecvPlayText(NULL);
}

TEST(TTS_ReceiverCb_test, OnRecvPlaylist)
{
    TtsPlayList *playlist = new(MEM_Voice) TtsPlayList();
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    BL_String text = "text";
    TTS_PlaylistInfo *playInfo = new(MEM_Voice) TTS_PlaylistInfo(sender, NULL);
    EXPECT_CALL(*mocktts, CreatePlayList(_)).WillOnce(Return(playInfo));
    TtsPlayList_Phrase *phrase = playlist->add_phrases();
    phrase->set_type(TtsPlayList_Phrase::PHRASE_TEXT);
    phrase->set_text(text);
    TtsBasePlay *basePlay = playlist->mutable_base();
    TtsBase *base = basePlay->mutable_base();
    base->set_sender(sender);
    base->set_onresponse(CL_TRUE);
    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(1);
    cb.OnRecvPlayList(playlist);
    delete playlist;
    delete playInfo;
}

TEST(TTS_ReceiverCb_test, OnRecvPlaylist_on_phrase)
{
    TtsPlayList *playlist = new(MEM_Voice) TtsPlayList();
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    BL_String text = "text";
    TTS_PlaylistInfo *playInfo = new(MEM_Voice) TTS_PlaylistInfo(sender, NULL);
    EXPECT_CALL(*mocktts, CreatePlayList(_)).WillOnce(Return(playInfo));
    TtsBasePlay *basePlay = playlist->mutable_base();
    TtsBase *base = basePlay->mutable_base();
    base->set_sender(sender);
    base->set_onresponse(CL_TRUE);
    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(1);
    cb.OnRecvPlayList(playlist);
    delete playlist;
    delete playInfo;
}


TEST(TTS_ReceiverCb_test, OnRecvPlaylist_null)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(0);
    cb.OnRecvPlayList(NULL);
}

TEST(TTS_ReceiverCb_test, OnRecvPlaylist_noresponse)
{
    TtsPlayList *playlist = new(MEM_Voice) TtsPlayList();
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    BL_String text = "text";
    TTS_PlaylistInfo *playInfo = new(MEM_Voice) TTS_PlaylistInfo(sender, NULL);
    EXPECT_CALL(*mocktts, CreatePlayList(_)).WillOnce(Return(playInfo));
    TtsPlayList_Phrase *phrase = playlist->add_phrases();
    phrase->set_type(TtsPlayList_Phrase::PHRASE_TEXT);
    phrase->set_text(text);
    TtsBasePlay *basePlay = playlist->mutable_base();
    TtsBase *base = basePlay->mutable_base();
    base->set_sender(sender);
    base->set_onresponse(CL_FALSE);
    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(1);
    cb.OnRecvPlayList(playlist);
    delete playlist;
    delete playInfo;
}

TEST(TTS_ReceiverCb_test, OnRecvPlaylist_nosender)
{
    TtsPlayList *playlist = new(MEM_Voice) TtsPlayList();
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "";
    BL_String text = "text";
    TtsPlayList_Phrase *phrase = playlist->add_phrases();
    phrase->set_type(TtsPlayList_Phrase::PHRASE_TEXT);
    phrase->set_text(text);
    TtsBasePlay *basePlay = playlist->mutable_base();
    TtsBase *base = basePlay->mutable_base();
    base->set_sender(sender);
    base->set_onresponse(CL_FALSE);
    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(0);
    cb.OnRecvPlayList(playlist);
    delete playlist;
}

TEST(TTS_ReceiverCb_test, OnRecvPlaylist_has_language)
{
    TtsPlayList *playlist = new(MEM_Voice) TtsPlayList();
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    BL_String text = "text";

    TTS_PlaylistInfo *playInfo = new(MEM_Voice) TTS_PlaylistInfo(sender, NULL);
    EXPECT_CALL(*mocktts, CreatePlayList(_)).WillOnce(Return(playInfo));

    TtsPlayList_Phrase *phrase = playlist->add_phrases();
    phrase->set_type(TtsPlayList_Phrase::PHRASE_TEXT);
    phrase->set_text(text);
    TtsBasePlay *basePlay = playlist->mutable_base();
    TtsBase *base = basePlay->mutable_base();
    base->set_sender(sender);
    base->set_onresponse(CL_FALSE);
    basePlay->set_language("ENG");
    basePlay->set_streamtype(TtsBasePlay_TTS_STREAM_TYPE_TTS_STREAM_NORMAL);
    basePlay->set_tokenid(1);

    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(1);
    cb.OnRecvPlayList(playlist);
    delete playlist;
    delete playInfo;
}

TEST(TTS_ReceiverCb_test, OnRecvPlaylist_language_empty)
{
    TtsPlayList *playlist = new(MEM_Voice) TtsPlayList();
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    BL_String text = "text";

    TTS_PlaylistInfo *playInfo = new(MEM_Voice) TTS_PlaylistInfo(sender, NULL);
    EXPECT_CALL(*mocktts, CreatePlayList(_)).WillOnce(Return(playInfo));

    TtsPlayList_Phrase *phrase = playlist->add_phrases();
    phrase->set_type(TtsPlayList_Phrase::PHRASE_TEXT);
    phrase->set_text(text);
    TtsBasePlay *basePlay = playlist->mutable_base();
    TtsBase *base = basePlay->mutable_base();
    base->set_sender(sender);
    base->set_onresponse(CL_FALSE);
    basePlay->set_language("");
    basePlay->set_streamtype(TtsBasePlay_TTS_STREAM_TYPE_TTS_STREAM_NORMAL);
    basePlay->set_tokenid(1);

    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(1);
    cb.OnRecvPlayList(playlist);
    delete playlist;
    delete playInfo;
}

TEST(TTS_ReceiverCb_test, SetPlaylistElement_text)
{
    TTS_ReceiverCb cb;
    BL_String sender = "test";
    BL_String text = "text";
    BL_String lang = "ENG";
    BL_String speaker_sex = "male";
    TtsPlayList_Phrase phrase;
    phrase.set_type(TtsPlayList_Phrase::PHRASE_TEXT);
    phrase.set_text(text);
    MockTTS_PlaylistInfo playlist(sender, NULL);
    EXPECT_CALL(playlist, AddText(_, _, _)).Times(1);
    cb.SetPlaylistElement(&playlist, lang, speaker_sex, phrase);
}

TEST(TTS_ReceiverCb_test, SetPlaylistElement_soundid)
{
    TTS_ReceiverCb cb;
    BL_String sender = "test";
    BL_String lang = "ENG";
    BL_String speaker_sex = "male";
    TtsPlayList_Phrase phrase;
    phrase.set_type(TtsPlayList_Phrase::PHRASE_SOUNDID);
    phrase.set_soundid(1);
    MockTTS_PlaylistInfo playlist(sender, NULL);
    EXPECT_CALL(playlist, AddSound(1)).Times(1);
    cb.SetPlaylistElement(&playlist, lang, speaker_sex, phrase);
}

TEST(TTS_ReceiverCb_test, SetPlaylistElement_soundname)
{
    TTS_ReceiverCb cb;
    BL_String sender = "test";
    TtsPlayList_Phrase phrase;
    BL_String lang = "ENG";
    BL_String speaker_sex = "male";
    phrase.set_type(TtsPlayList_Phrase::PHRASE_SOUNDNAME);
    phrase.set_filename("zzz");
    MockTTS_PlaylistInfo playlist(sender, NULL);
    EXPECT_CALL(playlist, AddSound(BL_String("zzz"))).Times(1);
    cb.SetPlaylistElement(&playlist, lang, speaker_sex, phrase);
}

TEST(TTS_ReceiverCb_test, SetPlaylistElement_millisecond)
{
    TTS_ReceiverCb cb;
    BL_String sender = "test";
    BL_String lang = "ENG";
    BL_String speaker_sex = "male";
    TtsPlayList_Phrase phrase;
    phrase.set_type(TtsPlayList_Phrase::PHRASE_SILENCE);
    phrase.set_millisecond(1200);
    MockTTS_PlaylistInfo playlist(sender, NULL);
    EXPECT_CALL(playlist, AddSilence(1200)).Times(1);
    cb.SetPlaylistElement(&playlist, lang, speaker_sex, phrase);
}

TEST(TTS_ReceiverCb_test, SetPlaylistElement_error_type)
{
    TTS_ReceiverCb cb;
    BL_String sender = "test";
    BL_String lang = "ENG";
    BL_String speaker_sex = "male";
    TtsPlayList_Phrase phrase;
    phrase.set_millisecond(1200);
    MockTTS_PlaylistInfo playlist(sender, NULL);
    cb.SetPlaylistElement(&playlist, lang, speaker_sex, phrase);
}

TEST(TTS_ReceiverCb_test, OnRecvPlayFile)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    
    TtsPlayFile *playlist = new(MEM_Voice) TtsPlayFile();
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    BL_String filePath = "text.pcm";
    
    TTS_PlaylistInfo *playInfo = new(MEM_Voice) TTS_PlaylistInfo(sender, NULL);
    EXPECT_CALL(*mocktts, CreatePlayList(_)).WillOnce(Return(playInfo));

    TtsBasePlay *basePlay = playlist->mutable_play();
    TtsBase *base = basePlay->mutable_base();
    base->set_sender(sender);
    base->set_onresponse(CL_TRUE);
    playlist->set_filepath(filePath);
    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(1);
    cb.OnRecvPlayFile(playlist);
    delete playlist;
}

TEST(TTS_ReceiverCb_test, OnRecvPlayFile_CreatePlayList_Null)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    
    TtsPlayFile *playlist = new(MEM_Voice) TtsPlayFile();
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    BL_String filePath = "text.pcm";
    
    TTS_PlaylistInfo *playInfo = NULL; 
    EXPECT_CALL(*mocktts, CreatePlayList(_)).WillOnce(Return(playInfo));

    TtsBasePlay *basePlay = playlist->mutable_play();
    TtsBase *base = basePlay->mutable_base();
    base->set_sender(sender);
    base->set_onresponse(CL_TRUE);
    playlist->set_filepath(filePath);
    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(0);
    cb.OnRecvPlayFile(playlist);
}

TEST(TTS_ReceiverCb_test, OnRecvPlayFile_has_language)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    
    TtsPlayFile *playlist = new(MEM_Voice) TtsPlayFile();
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    BL_String filePath = "text.pcm";
    
    TTS_PlaylistInfo *playInfo = new(MEM_Voice) TTS_PlaylistInfo(sender, NULL);
    EXPECT_CALL(*mocktts, CreatePlayList(_)).WillOnce(Return(playInfo));

    TtsBasePlay *basePlay = playlist->mutable_play();
    TtsBase *base = basePlay->mutable_base();
    base->set_sender(sender);
    base->set_onresponse(CL_TRUE);
    playlist->set_filepath(filePath);

    basePlay->set_language("ENG");
    basePlay->set_streamtype(TtsBasePlay_TTS_STREAM_TYPE_TTS_STREAM_NORMAL);
    basePlay->set_tokenid(1);

    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(1);
    cb.OnRecvPlayFile(playlist);
    delete playlist;
}


TEST(TTS_ReceiverCb_test, OnRecvPlayFile_null)
{
    TTS_ReceiverCb cb;
    cb.OnRecvPlayFile(NULL);
}

TEST(TTS_ReceiverCb_test, OnRecvPlayFile_noresponse)
{
    TtsPlayFile *playlist = new(MEM_Voice) TtsPlayFile();
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    BL_String filePath = "text.pcm";

    TTS_PlaylistInfo *playInfo = new(MEM_Voice) TTS_PlaylistInfo(sender, NULL);
    EXPECT_CALL(*mocktts, CreatePlayList(_)).WillOnce(Return(playInfo));

    TtsBasePlay *basePlay = playlist->mutable_play();
    TtsBase *base = basePlay->mutable_base();
    base->set_sender(sender);
    base->set_onresponse(CL_FALSE);
    playlist->set_filepath(filePath);
    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(1);
    cb.OnRecvPlayFile(playlist);
    delete playlist;
}

TEST(TTS_ReceiverCb_test, OnRecvPlayFile_nosender)
{
    TtsPlayFile *playlist = new(MEM_Voice) TtsPlayFile();
    TTS_ReceiverCb cb;
    // BL_String sender = "test";
    BL_String filePath = "text.pcm";
    TtsBasePlay *basePlay = playlist->mutable_play();
    TtsBase *base = basePlay->mutable_base();
    // base->set_sender(sender);
    playlist->set_filepath(filePath);
    base->set_onresponse(CL_FALSE);
    cb.OnRecvPlayFile(playlist);
    delete playlist;
}

TEST(TTS_ReceiverCb_test, OnRecvPlayId)
{
    TtsPlayId *playlist = new(MEM_Voice) TtsPlayId();
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);

    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    int soundId = 1;
    
    TTS_PlaylistInfo *playInfo = new(MEM_Voice) TTS_PlaylistInfo(sender, NULL);
    EXPECT_CALL(*mocktts, CreatePlayList(_)).WillOnce(Return(playInfo));

    TtsBasePlay *basePlay = playlist->mutable_play();
    TtsBase *base = basePlay->mutable_base();
    base->set_sender(sender);
    base->set_onresponse(CL_TRUE);
    playlist->set_id(soundId);
    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(1);
    cb.OnRecvPlayId(playlist);
    delete playlist;
}

TEST(TTS_ReceiverCb_test, OnRecvPlayId_CreatePlaylist_Null)
{
    TtsPlayId *playlist = new(MEM_Voice) TtsPlayId();
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);

    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    int soundId = 1;
    
    TTS_PlaylistInfo *playInfo = NULL; 
    EXPECT_CALL(*mocktts, CreatePlayList(_)).WillOnce(Return(playInfo));

    TtsBasePlay *basePlay = playlist->mutable_play();
    TtsBase *base = basePlay->mutable_base();
    base->set_sender(sender);
    base->set_onresponse(CL_TRUE);
    playlist->set_id(soundId);
    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(0);
    cb.OnRecvPlayId(playlist);
}


TEST(TTS_ReceiverCb_test, OnRecvPlayId_has_language)
{
    TtsPlayId *playlist = new(MEM_Voice) TtsPlayId();
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);

    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    int soundId = 1;
    
    TTS_PlaylistInfo *playInfo = new(MEM_Voice) TTS_PlaylistInfo(sender, NULL);
    EXPECT_CALL(*mocktts, CreatePlayList(_)).WillOnce(Return(playInfo));

    TtsBasePlay *basePlay = playlist->mutable_play();
    TtsBase *base = basePlay->mutable_base();
    base->set_sender(sender);
    base->set_onresponse(CL_TRUE);
    playlist->set_id(soundId);
    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(1);

    basePlay->set_language("ENG");
    basePlay->set_streamtype(TtsBasePlay_TTS_STREAM_TYPE_TTS_STREAM_NORMAL);
    basePlay->set_tokenid(1);

    cb.OnRecvPlayId(playlist);
    delete playlist;
}


TEST(TTS_ReceiverCb_test, OnRecvPlayId_null)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(0);
    cb.OnRecvPlayId(NULL);
}

TEST(TTS_ReceiverCb_test, OnRecvPlayId_noresponse)
{
    TtsPlayId *playlist = new(MEM_Voice) TtsPlayId();
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    int soundId = 1;
   
    TTS_PlaylistInfo *playInfo = new(MEM_Voice) TTS_PlaylistInfo(sender, NULL);
    EXPECT_CALL(*mocktts, CreatePlayList(_)).WillOnce(Return(playInfo));

    TtsBasePlay *basePlay = playlist->mutable_play();
    TtsBase *base = basePlay->mutable_base();
    base->set_sender(sender);
    base->set_onresponse(CL_FALSE);
    playlist->set_id(soundId);
    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(1);
    cb.OnRecvPlayId(playlist);
    delete playlist;
}

TEST(TTS_ReceiverCb_test, OnRecvPlayId_nosender)
{
    TtsPlayId *playlist = new(MEM_Voice) TtsPlayId();
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    int soundId = 1;
    TtsBasePlay *basePlay = playlist->mutable_play();
    TtsBase *base = basePlay->mutable_base();
    base->set_onresponse(CL_FALSE);
    playlist->set_id(soundId);
    EXPECT_CALL(*mocktts, Play(_, _, _)).Times(0);
    cb.OnRecvPlayId(playlist);
    delete playlist;
}

TEST(TTS_ReceiverCb_test, OnRecvStop_stopall)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    EXPECT_CALL(*mocktts, StopAll(_)).Times(1);
    EXPECT_CALL(*mocktts, Stop(_, _)).Times(0);
    TtsStop *msg = new(MEM_Voice) TtsStop();
    TtsBase *base = msg->mutable_base();
    base->set_sender(sender);
    base->set_onresponse(CL_TRUE);
    msg->set_stopall(CL_TRUE);
    cb.OnRecvStop(msg);
}

TEST(TTS_ReceiverCb_test, OnRecvStop_stopid)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    EXPECT_CALL(*mocktts, Stop(_, 100)).Times(1);
    EXPECT_CALL(*mocktts, StopAll(_)).Times(0);
    TtsStop *msg = new(MEM_Voice) TtsStop();
    TtsBase *base = msg->mutable_base();
    base->set_sender(sender);
    base->set_onresponse(CL_TRUE);
    msg->set_stopall(CL_FALSE);
    msg->set_reqid(100);
    cb.OnRecvStop(msg);
    delete msg;
}

TEST(TTS_ReceiverCb_test, OnRecvStop_null)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    EXPECT_CALL(*mocktts, Stop(_, _)).Times(0);
    EXPECT_CALL(*mocktts, StopAll(_)).Times(0);
    cb.OnRecvStop(NULL);
}

TEST(TTS_ReceiverCb_test, OnRecvStop_nosender)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    EXPECT_CALL(*mocktts, StopAll(_)).Times(0);
    EXPECT_CALL(*mocktts, Stop(_, _)).Times(0);
    TtsStop *msg = new(MEM_Voice) TtsStop();
    TtsBase *base = msg->mutable_base();
    base->set_onresponse(CL_TRUE);
    msg->set_stopall(CL_TRUE);
    cb.OnRecvStop(msg);
    delete msg;
}

TEST(TTS_ReceiverCb_test, OnRecvSetParam)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    TTS_EngineLanguage param;
    EXPECT_CALL(*mocktts, GetParameter(_, _, _)).WillOnce(DoAll(SetArgReferee<2>(param), Return(true)));
    EXPECT_CALL(*mocktts, SetParameter(_, _, _)).WillOnce(Return(true));
    TtsSetParam *msg = new(MEM_Voice) TtsSetParam();
    TtsBase *base = msg->mutable_base();
    BL_String sender = "test";
    base->set_sender(sender);
    base->set_onresponse(CL_TRUE);
    msg->set_language("ENG");
    cb.OnRecvSetParam(msg);
    delete msg;
}

TEST(TTS_ReceiverCb_test, OnRecvSetParam_NULL)
{
    TTS_ReceiverCb cb;
    cb.OnRecvSetParam(NULL);
}

TEST(TTS_ReceiverCb_test, OnRecvSetParami_with_value)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    TTS_EngineLanguage param;
    EXPECT_CALL(*mocktts, GetParameter(_, _, _)).WillOnce(DoAll(SetArgReferee<2>(param), Return(true)));
    EXPECT_CALL(*mocktts, SetParameter(_, _, _)).WillOnce(Return(false));
    TtsSetParam *msg = new(MEM_Voice) TtsSetParam();
    TtsBase *base = msg->mutable_base();
    BL_String sender = "test";
    base->set_sender(sender);
    base->set_onresponse(CL_TRUE);
    msg->set_language("ENG");
    msg->set_role(1);
    msg->set_volume(1);
    msg->set_speed(1);
    msg->set_pitch(1);
    cb.OnRecvSetParam(msg);
    delete msg;
}

TEST(TTS_ReceiverCb_test, OnRecvSetParami_with_value_2)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    TTS_EngineLanguage param;
    EXPECT_CALL(*mocktts, GetParameter(_, _, _)).WillOnce(DoAll(SetArgReferee<2>(param), Return(false)));
    TtsSetParam *msg = new(MEM_Voice) TtsSetParam();
    TtsBase *base = msg->mutable_base();
    BL_String sender = "test";
    base->set_sender(sender);
    base->set_onresponse(CL_TRUE);
    msg->set_language("ENG");
    msg->set_role(1);
    msg->set_volume(1);
    msg->set_speed(1);
    msg->set_pitch(1);
    cb.OnRecvSetParam(msg);
    delete msg;
}

TEST(TTS_ReceiverCb_test, OnRecvSetParam_no_language)
{
    TTS_ReceiverCb cb;
    
    TtsSetParam *msg = new(MEM_Voice) TtsSetParam();
    TtsBase *base = msg->mutable_base();
    BL_String sender = "test";
    base->set_sender(sender);
    base->set_onresponse(CL_TRUE);
    msg->set_role(1);
    msg->set_volume(1);
    msg->set_speed(1);
    msg->set_pitch(1);
    cb.OnRecvSetParam(msg);
    delete msg;
}

TEST(TTS_ReceiverCb_test, OnRecvSetParam_nosender)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    EXPECT_CALL(*mocktts, SetParameter(_, _, _)).Times(0);
    TtsSetParam *msg = new(MEM_Voice) TtsSetParam();
    TtsBase *base = msg->mutable_base();
    // BL_String sender = "test";
    // base->set_sender(sender);
    base->set_onresponse(CL_TRUE);
    TtsBase_ParamPair *paramPair = msg->add_params();
    paramPair->set_type(TtsBase_ParamPair::PARAM_ROLE);
    paramPair->set_value(3);
    cb.OnRecvSetParam(msg);
    delete msg;
}

TEST(TTS_ReceiverCb_test, OnRecvGetParam_nosender)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    EXPECT_CALL(*mocktts, GetParameter(_, _, _)).Times(0);
    TtsGetParam *msg = new(MEM_Voice) TtsGetParam();
    TtsBase *base = msg->mutable_base();
    msg->set_language("ENG");
    // BL_String sender = "test";
    // base->set_sender(sender);
    base->set_onresponse(CL_TRUE);
    cb.OnRecvGetParam(msg);
    delete msg;
}

TEST(TTS_ReceiverCb_test, OnRecvGetParam)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    EXPECT_CALL(*mocktts, GetParameter(_, _, _)).WillOnce(Return(false));
    TtsGetParam *msg = new(MEM_Voice) TtsGetParam();
    TtsBase *base = msg->mutable_base();
    BL_String sender = "test";
    base->set_sender(sender);
    msg->set_language("ENG");
    base->set_onresponse(CL_TRUE);
    cb.OnRecvGetParam(msg);
    delete msg;
}

TEST(TTS_ReceiverCb_test, OnRecvGetParam_true)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    EXPECT_CALL(*mocktts, GetParameter(_, _, _)).WillOnce(Return(true));
    TtsGetParam *msg = new(MEM_Voice) TtsGetParam();
    TtsBase *base = msg->mutable_base();
    BL_String sender = "test";
    base->set_sender(sender);
    msg->set_language("ENG");
    base->set_onresponse(CL_TRUE);
    cb.OnRecvGetParam(msg);
    delete msg;
}

TEST(TTS_ReceiverCb_test, OnRecvGetParam_no_language)
{
    TTS_ReceiverCb cb;
    TtsGetParam *msg = new(MEM_Voice) TtsGetParam();
    TtsBase *base = msg->mutable_base();
    BL_String sender = "test";
    base->set_sender(sender);
    base->set_onresponse(CL_TRUE);
    cb.OnRecvGetParam(msg);
    delete msg;
}

TEST(TTS_ReceiverCb_test, OnRecvGetParam_NULL)
{
    TTS_ReceiverCb cb;
    cb.OnRecvGetParam(NULL);
}

TEST(TTS_ReceiverCb_test, OnRecvListRole)
{
    TtsListRole *msg = new(MEM_Voice) TtsListRole();
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;

    TtsEngineRoles roles;
    TTS_EngineRole role;
    roles.push_back(role);

    BL_String sender = "test";
    msg->set_sender(sender);
    EXPECT_CALL(*mocktts, ListRole(_)).WillOnce(SetArgReferee<0>(roles));
    cb.OnRecvListRole(msg);
    delete msg;
}

TEST(TTS_ReceiverCb_test, OnRecvListRole_noSender)
{
    TtsListRole *msg = new(MEM_Voice) TtsListRole();
    TTS_ReceiverCb cb;
    cb.OnRecvListRole(msg);
    delete msg;
}

TEST(TTS_ReceiverCb_test, OnRecvListRole_NULL)
{
    TTS_ReceiverCb cb;
    cb.OnRecvListRole(NULL);
}

#if 0
TEST(TTS_ReceiverCb_test, OnRecvListRole_null)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    EXPECT_CALL(*mocktts, ListRole(_, _)).Times(0);
    cb.OnRecvListRole(NULL);
}

TEST(TTS_ReceiverCb_test, OnRecvListRole_nosender)
{
    TtsListRole *msg = new(MEM_Voice) TtsListRole();
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    EXPECT_CALL(*mocktts, ListRole(_, _)).Times(1);
    cb.OnRecvListRole(msg);
    delete msg;
}

TEST(TTS_ReceiverCb_test, OnRecvGetDefaultRole)
{
    TtsGetDefaultRole *msg = new(MEM_Voice) TtsGetDefaultRole();
    TTS_ReceiverCb cb;
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    cb.m_tts = mocktts;
    BL_String sender = "test";
    msg->set_sender(sender);
    EXPECT_CALL(*mocktts, GetDefaultRole(_)).WillOnce(Return(CL_TRUE));
    cb.OnRecvGetDefaultRole(msg);
    delete msg;
}

TEST(TTS_ReceiverCb_test, OnRecvGetDefaultRole_null)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    EXPECT_CALL(*mocktts, GetDefaultRole(_)).Times(0);
    cb.OnRecvGetDefaultRole(NULL);
}

TEST(TTS_ReceiverCb_test, OnRecvListParamRange)
{
    TtsListParamRange *msg = new(MEM_Voice) TtsListParamRange();
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    msg->set_sender(sender);
    EXPECT_CALL(*mocktts, ListParamRange(_, _)).Times(1);
    cb.OnRecvListParamRange(msg);
    delete msg;
}

TEST(TTS_ReceiverCb_test, OnRecvListParamRange_with_value)
{
    TtsListParamRange *msg = new(MEM_Voice) TtsListParamRange();
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    msg->set_sender(sender);

    TTS_ParamRange *range = new TTS_ParamRange[2];

    EXPECT_CALL(*mocktts, ListParamRange(_, _)).WillOnce(DoAll(SetArgPointee<0>(range),
                      SetArgPointee<1>(2)));

    cb.OnRecvListParamRange(msg);
    delete msg;
}


TEST(TTS_ReceiverCb_test, OnRecvListParamRange_null)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    EXPECT_CALL(*mocktts, ListParamRange(_, _)).Times(0);
    cb.OnRecvListParamRange(NULL);
}

#endif

TEST(TTS_ReceiverCb_test, OnRecvRegisterSound)
{
    TtsRegisterSound *msg = new(MEM_Voice) TtsRegisterSound();
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    msg->set_sender(sender);
    BL_String fileName = "test.wav";
    msg->set_filename(fileName);
    EXPECT_CALL(*mocktts, RegisterSound(_, _, _)).WillOnce(Return(TTS_ERROR_NO_ERROR));
    cb.OnRecvRegisterSound(msg);
    delete msg;
}

TEST(TTS_ReceiverCb_test, OnRecvRegisterSound_false)
{
    TtsRegisterSound *msg = new(MEM_Voice) TtsRegisterSound();
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    msg->set_sender(sender);
    BL_String fileName = "test.wav";
    msg->set_filename(fileName);
    EXPECT_CALL(*mocktts, RegisterSound(_, _, _)).WillOnce(Return(TTS_ERROR_NO_RESOURCE));
    cb.OnRecvRegisterSound(msg);
    delete msg;
}


TEST(TTS_ReceiverCb_test, OnRecvRegisterSound_nosender)
{
    TtsRegisterSound *msg = new(MEM_Voice) TtsRegisterSound();
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String fileName = "test.wav";
    msg->set_filename(fileName);
    EXPECT_CALL(*mocktts, RegisterSound(_, _, _)).Times(0);
    cb.OnRecvRegisterSound(msg);
    delete msg;
}

TEST(TTS_ReceiverCb_test, OnRecvRegisterSound_null)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    EXPECT_CALL(*mocktts, RegisterSound(_, _, _)).Times(0);
    cb.OnRecvRegisterSound(NULL);
}

TEST(TTS_ReceiverCb_test, OnRecvUnRegisterSound)
{
    TtsUnRegisterSound *msg = new(MEM_Voice) TtsUnRegisterSound();
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    msg->set_sender(sender);
    int soundId = 1;
    msg->set_soundid(soundId);
    EXPECT_CALL(*mocktts, UnRegisterSound(_, _)).WillOnce(Return(TTS_ERROR_NO_ERROR));
    cb.OnRecvUnRegisterSound(msg);
    delete msg;
}

TEST(TTS_ReceiverCb_test, OnRecvUnRegisterSound_false)
{
    TtsUnRegisterSound *msg = new(MEM_Voice) TtsUnRegisterSound();
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    msg->set_sender(sender);
    int soundId = 1;
    msg->set_soundid(soundId);
    EXPECT_CALL(*mocktts, UnRegisterSound(_, _)).WillOnce(Return(TTS_ERROR_NO_RESOURCE));
    cb.OnRecvUnRegisterSound(msg);
    delete msg;
}

TEST(TTS_ReceiverCb_test, OnRecvUnRegisterSound_null)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    EXPECT_CALL(*mocktts, UnRegisterSound(_, _)).Times(0);
    cb.OnRecvUnRegisterSound(NULL);
}

TEST(TTS_ReceiverCb_test, OnRecvSynthesizeToFile_NULL)
{
    TTS_ReceiverCb cb;
    cb.OnRecvSynthesizeToFile(NULL);
}

TEST(TTS_ReceiverCb_test, OnRecvSynthesizeToFile)
{
    TtsSynthesizeToFile* playTextMsg = new(MEM_Voice) TtsSynthesizeToFile();
    TtsBasePlay *basePlay = playTextMsg->mutable_play();
    basePlay->set_priority(TtsBasePlay::OP_PRI_Navi);

    TtsBase *base = basePlay->mutable_base();
    base->set_sender(EV_MODULE_VOICEPLAY);
    base->set_onresponse(true);

    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    // EXPECT_CALL(*mocktts, SynthesizeToFile(_, _, _, _, _)).Times(1);
    cb.OnRecvSynthesizeToFile(playTextMsg);
    delete playTextMsg;
}

TEST(TTS_ReceiverCb_test, OnRecvSynthesizeToFile_no_sender)
{
    TtsSynthesizeToFile* playTextMsg = new(MEM_Voice) TtsSynthesizeToFile();
    TtsBasePlay *basePlay = playTextMsg->mutable_play();
    basePlay->set_priority(TtsBasePlay::OP_PRI_Navi);

    TtsBase *base = basePlay->mutable_base();
    base->set_onresponse(true);
    
    TTS_ReceiverCb cb;
    cb.OnRecvSynthesizeToFile(playTextMsg);
    delete playTextMsg;
}


TEST(TTS_ReceiverCb_test, OnRecvServiceStatus)
{
    TtsServiceStatus *msg = new(MEM_Voice) TtsServiceStatus();
    TTS_ReceiverCb cb;
    BL_String sender = "test";
    msg->set_sender(sender);
    cb.OnRecvServiceStatus(NULL);
    cb.OnRecvServiceStatus(msg);
    delete msg;
}

TEST(TTS_ReceiverCb_test, OnRecvPause)
{

    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TtsPause  *msg = new(MEM_Voice) TtsPause();
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    msg->set_sender(sender);
    msg->set_reqid(1);

    EXPECT_CALL(*mocktts, Pause(_, _)).Times(1);

    cb.OnRecvPause(msg);
    delete msg;
}

TEST(TTS_ReceiverCb_test, OnRecvPause_NULL)
{
    TTS_ReceiverCb cb;
    cb.OnRecvPause(NULL);
}

TEST(TTS_ReceiverCb_test, OnRecvResume)
{
    MockTextToSpeech *mocktts = new MockTextToSpeech(NULL);
    TtsResume *msg = new(MEM_Voice) TtsResume();
    TTS_ReceiverCb cb;
    cb.m_tts = mocktts;
    BL_String sender = "test";
    msg->set_sender(sender);
    msg->set_reqid(1);
    EXPECT_CALL(*mocktts, Resume(_, _)).Times(1);
    cb.OnRecvResume(msg);
    delete msg;
}

TEST(TTS_ReceiverCb_test, OnRecvResume_NULL)
{
    TTS_ReceiverCb cb;
    cb.OnRecvResume(NULL);
}

TEST(TTS_ReceiverCb_test, SendMessage_NULL)
{
    TTS_ReceiverCb cb;
    cb.SendMessage(EV_EVENT_TTS_RESP_SETPARAM, NULL);
}


/* EOF */
