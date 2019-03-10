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
#include "VR_AudioPlayAndShow.h"
#include "VR_ActionEventSender.h"
#include "VR_DialogEngine_mock.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_AudioPlayAndShow_Test
*
* The class is just for VR_AudioPlayAndShow test.
*/
class VR_AudioPlayAndShow_Test : public testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

public:
    VR_String m_xml;
    spVR_ActionEventSender m_sp_EventSender;
    VR_AudioPlayAndShow *m_audioPlayAction;
};

void VR_AudioPlayAndShow_Test::SetUp()
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);

    m_xml = "<event name=\"cancel\">"
            "</event>";
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    m_audioPlayAction = VR_new VR_AudioPlayAndShow(1, 1, m_xml, m_sp_EventSender);
}

void VR_AudioPlayAndShow_Test::TearDown()
{
    if (NULL != m_audioPlayAction) {
        delete m_audioPlayAction;
        m_audioPlayAction = NULL;
    }
}

TEST_F(VR_AudioPlayAndShow_Test, Run_Case1)
{
    m_audioPlayAction->m_ReqMsg = "<action agent=\"media\" op =>"
                    "</action1>";
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayAndShow_Test, Run_Case2)
{
    m_audioPlayAction->m_ReqMsg = "<action agent=\"media\" op=\"playByFilter\">"
                        "<playType>PLAY_ALL_ARTIST</playType>"
                        "<filterGenre>none</filterGenre>"
                    "</action>";
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayAndShow_Test, Run_Case3)
{
    m_audioPlayAction->m_ReqMsg = "<action agent=\"media\" op=\"playByFilter\">"
                        "<playType>PLAY_ALL_ARTIST</playType>"
                        "<filterGenre>news</filterGenre>"
                    "</action>";
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayAndShow_Test, Run_Case4)
{
    m_audioPlayAction->m_ReqMsg = "<action agent=\"media\" op=\"playByFilter\">"
                        "<playType>none</playType>"
                        "<filterGenre>none</filterGenre>"
                        "<filterArtist>news</filterArtist>"
                    "</action>";
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayAndShow_Test, Run_Case5)
{
    m_audioPlayAction->m_ReqMsg = "<action agent=\"media\" op=\"playByFilter\">"
                        "<playType>none</playType>"
                        "<filterGenre>none</filterGenre>"
                        "<filterArtist>none</filterArtist>"
                    "</action>";
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayAndShow_Test, Run_Case6)
{
    m_audioPlayAction->m_ReqMsg = "<action agent=\"media\" op=\"playByFilter\">"
                        "<playType>none</playType>"
                        "<filterGenre>news</filterGenre>"
                        "<filterArtist>news</filterArtist>"
                    "</action>";
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayAndShow_Test, Run_Case7)
{
    m_audioPlayAction->m_ReqMsg = "<action agent=\"media\" op=\"playByFilter\">"
                        "<playType>none</playType>"
                        "<filterGenre>news</filterGenre>"
                        "<filterArtist>none</filterArtist>"
                    "</action>";
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayAndShow_Test, Run_Case8)
{
    m_audioPlayAction->m_ReqMsg = "<action agent=\"media\" op=\"showMediaScreen\">"
                        "<type>none</type>"
                    "</action>";
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayAndShow_Test, Run_Case9)
{
    m_audioPlayAction->m_ReqMsg = "<action agent=\"media\" op=\"showMediaScreen\">"
                        "<type>GENRE</type>"
                    "</action>";
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayAndShow_Test, Run_Case10)
{
    m_audioPlayAction->m_ReqMsg = "<action agent=\"media\" op=\"showMediaScreen\">"
                        "<type>ARTIST</type>"
                        "<filterGenre>news</filterGenre>"
                    "</action>";
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayAndShow_Test, Run_Case11)
{
    m_audioPlayAction->m_ReqMsg = "<action agent=\"media\" op=\"showMediaScreen\">"
                        "<type>ARTIST</type>"
                        "<filterGenre>none</filterGenre>"
                    "</action>";
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayAndShow_Test, Run_Case12)
{
    m_audioPlayAction->m_ReqMsg = "<action agent=\"media\" op=\"showMediaScreen\">"
                        "<type>ALBUM</type>"
                        "<filterGenre>news</filterGenre>"
                        "<filterArtist>jack</filterArtist>"
                    "</action>";
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayAndShow_Test, Run_Case13)
{
    m_audioPlayAction->m_ReqMsg = "<action agent=\"media\" op=\"showMediaScreen\">"
                        "<type>ALBUM</type>"
                        "<filterGenre>news</filterGenre>"
                        "<filterArtist>none</filterArtist>"
                    "</action>";
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayAndShow_Test, Run_Case14)
{
    m_audioPlayAction->m_ReqMsg = "<action agent=\"media\" op=\"showMediaScreen\">"
                        "<type>ALBUM</type>"
                        "<filterGenre>none</filterGenre>"
                        "<filterArtist>jack</filterArtist>"
                    "</action>";
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayAndShow_Test, Run_Case15)
{
    m_audioPlayAction->m_ReqMsg = "<action agent=\"media\" op=\"showMediaScreen\">"
                        "<type>ALBUM</type>"
                        "<filterGenre>none</filterGenre>"
                        "<filterArtist>none</filterArtist>"
                    "</action>";
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayAndShow_Test, Run_Case16)
{
    m_audioPlayAction->m_ReqMsg = "<action agent=\"media\" op=\"showMediaScreen\">"
                        "<type>SONG</type>"
                    "</action>";
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayAndShow_Test, Run_Case17)
{
    m_audioPlayAction->m_ReqMsg = "<action agent=\"media\" op=\"showMediaScreen\">"
                        "<type>PLAYLIST</type>"
                    "</action>";
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayAndShow_Test, Run_Case18)
{
    m_audioPlayAction->m_ReqMsg = "<action agent=\"media\" op=turn>"
                        "<type>PLAYLIST</type>"
                    "</action>";
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayAndShow_Test, Run_Case19)
{
    m_audioPlayAction->m_ReqMsg = "<action agent=\"media\" op=\"1playByFilter\">"
                        "<playType>PLAY_ALL_ARTIST</playType>"
                        "<filterGenre>none</filterGenre>"
                    "</action>";
    m_audioPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioPlayAndShow_Test, OnReceive_Case)
{
    std::unique_ptr<VrRecogResultConfirm> protoMsg(VrRecogResultConfirm::default_instance().New());
    protoMsg->set_action_id(ChangeHDSubChannel);
    protoMsg->set_processing_result(true);

    m_audioPlayAction->OnReceive(*protoMsg);
    SUCCEED();
}

/* EOF */
