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
#include "stdafx.h"
#include "TTS_Controler.h"
#include "TTS_PlaylistInfo.h"
#include "TTS_ReceiverCb.h"
#include "TTS_PlaylistInfo.h"
#include "TTS_IF.h"
#include "TTS_Engine_mock.h"
#include "TTS_EngineManager.h"

using namespace std;
using ::testing::_;
using ::testing::Return;
using ::testing::ReturnPointee;

/**
* TTS_TextToSpeechImpl
*
* The class is just for TTS_PlaylistInfo test.
*/

class MockTTS_TextToSpeechImpl : public TTS_TextToSpeechImpl
{
public:
    MockTTS_TextToSpeechImpl(TTS_TextToSpeechImpl::PlaybackListener* listener) : TTS_TextToSpeechImpl(listener) 
    {
    }

    virtual ~MockTTS_TextToSpeechImpl()
    {
    }

    MOCK_METHOD0(GetDefaultLanguage, BL_String());
    MOCK_METHOD1(GetEngineByLang, TTS_Engine*(const BL_String& language));
    MOCK_CONST_METHOD0(GetEngineManager, TTS_EngineManager*());
    MOCK_METHOD2(GetSoundName, BL_String(const BL_String &sender, int soundID));
    MOCK_METHOD2(OnPlayEnd, void(TTS_PlaylistInfo* playlist, TTS_PLAYER_STATE status));
    MOCK_METHOD4(OnPlayState, void(int reqID, const BL_String &sender, TTS_PLAYER_STATE state, int playNum));

};

/**
* TestPalylistInfo
*
* The class is just for TTS_TextToSpeechImpl test.
*/
class TestPalylistInfo : public testing::Test
{
public:
    TestPalylistInfo() {}
    virtual ~TestPalylistInfo() {}

public:

};

TEST(TestPalylistInfo, AddText)
{
    MockTTS_TextToSpeechImpl text(NULL);
    TTS_EngineManager engine;

    EXPECT_CALL(text, GetEngineManager()).WillOnce(Return(&engine));

    TTS_PlaylistInfo playlist("sender", &text);
    playlist.AddText("test", "ENG", "male");
}

TEST(TestPalylistInfo, AddText_NoLanguage)
{
    MockTTS_TextToSpeechImpl text(NULL);
    TTS_EngineManager engine;

    EXPECT_CALL(text, GetEngineManager()).WillOnce(Return(&engine));
    EXPECT_CALL(text, GetDefaultLanguage()).WillOnce(Return("ENG"));

    TTS_PlaylistInfo playlist("sender", &text);
    playlist.AddText("test", "", "male");
}

TEST(TestPalylistInfo, AddText_engine_null)
{
    MockTTS_TextToSpeechImpl text(NULL);

    TTS_EngineManager *p = NULL;
    EXPECT_CALL(text, GetEngineManager()).WillOnce(Return(p));
    EXPECT_CALL(text, GetDefaultLanguage()).WillOnce(Return("ENG"));

    TTS_PlaylistInfo playlist("sender", &text);
    playlist.AddText("test", "", "male");
}

TEST(TestPalylistInfo, AddText_Own_null)
{
    TTS_PlaylistInfo playlist("sender", NULL);
    playlist.AddText("test", "", "male");
}

TEST(TestPalylistInfo, AddSound)
{
    TTS_PlaylistInfo playlist("sender", NULL);
    playlist.AddSound("test");
}

TEST(TestPalylistInfo, AddSound_ID)
{
    TTS_PlaylistInfo playlist("sender", NULL);
    playlist.AddSound(1);
}

TEST(TestPalylistInfo, AddSound_ID_Value)
{
    MockTTS_TextToSpeechImpl text(NULL);

    EXPECT_CALL(text, GetSoundName(_, _)).WillOnce(Return(""));

    TTS_PlaylistInfo playlist("sender", &text);
    playlist.AddSound(1);
}

TEST(TestPalylistInfo, AddSilence)
{
    TTS_PlaylistInfo playlist("sender", NULL);
    playlist.AddSilence(1);
}

TEST(TestPalylistInfo, SendPlayEnd_null)
{
    TTS_PlaylistInfo playlist("sender", NULL);
    playlist.SendPlayEnd(TTS_PLAYER_ABORT);
}

TEST(TestPalylistInfo, SendPlayEnd)
{

    MockTTS_TextToSpeechImpl text(NULL);
    EXPECT_CALL(text, OnPlayEnd(_, _)).Times(1);

    TTS_PlaylistInfo playlist("sender", &text);
    playlist.SendPlayEnd(TTS_PLAYER_ABORT);
}

TEST(TestPalylistInfo, SendPlayState_null)
{
    TTS_PlaylistInfo playlist("sender", NULL);
    playlist.SendPlayState(TTS_PLAYER_START);
}

TEST(TestPalylistInfo, SendPlayState)
{
    MockTTS_TextToSpeechImpl text(NULL);
    EXPECT_CALL(text, OnPlayState(_, _, _, _)).Times(1);

    TTS_PlaylistInfo playlist("sender", &text);
    playlist.SendPlayState(TTS_PLAYER_START);
}


/* EOF */
