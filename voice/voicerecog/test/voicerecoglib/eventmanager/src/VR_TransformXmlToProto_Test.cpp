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

#include "stdafx.h"
#include "gtest/gtest.h"
#include "pugixml.hpp"

#include "VR_TransformXmlToProto.h"

#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMTaskMessage.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyVoiceTagResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrMessageTrans.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActionNavi.pb.h"

using namespace navi::VoiceRecog;

/**
* MyXml_docment.cpp
*
* The class is just for MyXml_docment test.
*/
class MyXml_docment : public pugi::xml_document
{
public:
    MyXml_docment()
      : pugi::xml_document()
      , _back(0)
    {

    }

    void clear()
    {
        if (NULL != _root) {
            _back = _root;
            _root = NULL;
        }
    }

    void back()
    {
        if (NULL == _root) {
            _root = _back;
        }
    }

private:
    pugi::xml_node_struct *_back;
};

/**
* VR_TransformXmlToProto_Test.cpp
*
* The class is just for VR_TransformXmlToProto test.
*/
class  VR_TransformXmlToProto_Test : public testing::Test
{
protected:

    virtual   void  SetUp();
    virtual   void  TearDown();

    VR_TransformXmlToProto*     m_VR_TransformXmlToProto;
};


void
VR_TransformXmlToProto_Test::SetUp()
{
    m_VR_TransformXmlToProto = VR_new  VR_TransformXmlToProto();
}

void
VR_TransformXmlToProto_Test::TearDown()
{
    if (m_VR_TransformXmlToProto) {
        delete  m_VR_TransformXmlToProto;
        m_VR_TransformXmlToProto = NULL;
    }
}

TEST_F(VR_TransformXmlToProto_Test, Transform_Failed)
{
    std::string msg;
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PlayAndShow)
{
    std::string msg = "<action agent=\"media\" op=\"playByFilter\">"
            "<playType>PLAY_ALL_ARTIST</playType>"
            "<filterGenre>none</filterGenre>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_MusicPlayTransform_NoType)
{
    std::string msg = "<action agent=\"media\" op=\"play\">"
             "<sourceid>1</sourceid>"
            "<id>1</id>"
            "<subid>1</subid>"
            "<shuffle>false</shuffle>"
            "<playType>false</playType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_MusicPlayTransform_PlayAlbum)
{
    std::string msg = "<action agent=\"media\" op=\"play\">"
             "<sourceid>1</sourceid>"
            "<id>1</id>"
            "<subid>1</subid>"
            "<shuffle>true</shuffle>"
            "<playType>PLAY_ALBUM</playType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_MusicPlayTransform_PlayAll)
{
    std::string msg = "<action agent=\"media\" op=\"play\">"
             "<sourceid>1</sourceid>"
            "<id>1</id>"
            "<subid>1</subid>"
            "<shuffle>false</shuffle>"
            "<playType>PLAY_ALL</playType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_MusicPlayTransform_PlayArtist)
{
    std::string msg = "<action agent=\"media\" op=\"play\">"
             "<sourceid>1</sourceid>"
            "<id>1</id>"
            "<subid>1</subid>"
            "<shuffle>false</shuffle>"
            "<playType>PLAY_ARTIST</playType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_MusicPlayTransform_PlayAudioBook)
{
    std::string msg = "<action agent=\"media\" op=\"play\">"
             "<sourceid>1</sourceid>"
            "<id>1</id>"
            "<subid>1</subid>"
            "<shuffle>false</shuffle>"
            "<playType>PLAY_AUDIO_BOOK</playType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_MusicPlayTransform_PlayComposer)
{
    std::string msg = "<action agent=\"media\" op=\"play\">"
             "<sourceid>1</sourceid>"
            "<id>1</id>"
            "<subid>1</subid>"
            "<shuffle>false</shuffle>"
            "<playType>PLAY_COMPOSER</playType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_MusicPlayTransform_PlayGenre)
{
    std::string msg = "<action agent=\"media\" op=\"play\">"
             "<sourceid>1</sourceid>"
            "<id>1</id>"
            "<subid>1</subid>"
            "<shuffle>false</shuffle>"
            "<playType>PLAY_GENRE</playType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_MusicPlayTransform_PlayPlaylist)
{
    std::string msg = "<action agent=\"media\" op=\"play\">"
             "<sourceid>1</sourceid>"
            "<id>1</id>"
            "<subid>1</subid>"
            "<shuffle>false</shuffle>"
            "<playType>PLAY_PLAYLIST</playType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_MusicPlayTransform_PlayPodcast)
{
    std::string msg = "<action agent=\"media\" op=\"play\">"
             "<sourceid>1</sourceid>"
            "<id>1</id>"
            "<subid>1</subid>"
            "<shuffle>false</shuffle>"
            "<playType>PLAY_PODCAST</playType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_MusicPlayTransform_PlayPodcastItem)
{
    std::string msg = "<action agent=\"media\" op=\"play\">"
             "<sourceid>1</sourceid>"
            "<id>1</id>"
            "<subid>1</subid>"
            "<shuffle>false</shuffle>"
            "<playType>PLAY_PODCAST_ITEM</playType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_MusicPlayTransform_PlaySong)
{
    std::string msg = "<action agent=\"media\" op=\"play\">"
             "<sourceid>1</sourceid>"
            "<id>1</id>"
            "<subid>1</subid>"
            "<shuffle>false</shuffle>"
            "<playType>PLAY_SONG</playType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_MusicAudioOffTransform)
{
    std::string msg = "<action agent=\"media\" op=\"turnAudioOff\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_MusicAudioOnTransform)
{
    std::string msg = "<action agent=\"media\" op=\"turnAudioOn\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_MusicChangeByNameTransform)
{
    std::string msg = "<action agent=\"media\" op=\"changeSourceByName\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, MusicDictionaryResultTransform)
{
    std::string msg = "<grammar_result agent=\"media\">"
        "</grammar_result>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, PhoneBookContactResultTransform)
{
    std::string msg = "<grammar_result agent=\"phone\">"
        "</grammar_result>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_MusicGetAlbumTransform_NoSourceid)
{
    std::string msg = "<action agent=\"media\" op=\"getCurrentAlbum\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_MusicGetAlbumTransform)
{
    std::string msg = "<action agent=\"media\" op=\"getCurrentAlbum\">"
            "<sourceid>1</sourceid>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_MusicGetArtistTransform_NoSourceid)
{
    std::string msg = "<action agent=\"media\" op=\"getCurrentAlbum\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_MusicGetArtistTransform)
{
    std::string msg = "<action agent=\"media\" op=\"getCurrentArtist\">"
            "<sourceid>1</sourceid>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_MusicGetComposerTransform_NoSourceid)
{
    std::string msg = "<action agent=\"media\" op=\"getCurrentComposer\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_MusicGetComposerTransform)
{
    std::string msg = "<action agent=\"media\" op=\"getCurrentComposer\">"
            "<sourceid>1</sourceid>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_MusicGetGenreTransform_NoSourceid)
{
    std::string msg = "<action agent=\"media\" op=\"getCurrentGenre\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_MusicGetGenreTransform)
{
    std::string msg = "<action agent=\"media\" op=\"getCurrentGenre\">"
            "<sourceid>1</sourceid>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, ChangeBandToInt_AM)
{
    std::string band = "AM";
    m_VR_TransformXmlToProto->ChangeBandToInt(band);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, ChangeBandToInt_FM)
{
    std::string band = "FM";
    m_VR_TransformXmlToProto->ChangeBandToInt(band);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, ChangeBandToInt_XM)
{
    std::string band = "XM";
    m_VR_TransformXmlToProto->ChangeBandToInt(band);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, ChangeBandToInt_DAB)
{
    std::string band = "DAB";
    m_VR_TransformXmlToProto->ChangeBandToInt(band);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, ChangeBandToInt_Undef)
{
    std::string band = "";
    m_VR_TransformXmlToProto->ChangeBandToInt(band);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioChangeHDSubChannelTransform)
{
    std::string msg = "<action agent=\"media\" op=\"changeHDSubChannel\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioTuneFrequencyTransform_AM)
{
    std::string msg = "<action agent=\"media\" op=\"tuneFrequency\">"
            "<band>AM</band>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioTuneFrequencyTransform_FM)
{
    std::string msg = "<action agent=\"media\" op=\"tuneFrequency\">"
            "<band>FM</band>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioTuneFrequencyTransform_other)
{
    std::string msg = "<action agent=\"media\" op=\"tuneFrequency\">"
            "<band></band>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioTuneFMHDFrequencyTransform)
{
    std::string msg = "<action agent=\"media\" op=\"tuneFMHDFrequency\">"
            "<band>FM</band>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioTuneLastSatelliteTransform)
{
    std::string msg = "<action agent=\"media\" op=\"tuneLastSatellite\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioTuneSatelliteChannelNumberTransform)
{
    std::string msg = "<action agent=\"media\" op=\"tuneSatelliteChannelNumber\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioTuneLastAMTransform)
{
    std::string msg = "<action agent=\"media\" op=\"tuneLastAM\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioTuneLastFMTransform)
{
    std::string msg = "<action agent=\"media\" op=\"tuneLastFM\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioPlayByGenreTransform_FM)
{
    std::string msg = "<action agent=\"media\" op=\"playByGenre\">"
            "<band>FM</band>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioPlayByGenreTransform_XM)
{
    std::string msg = "<action agent=\"media\" op=\"playByGenre\">"
            "<band>XM</band>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioPlayByGenreTransform_other)
{
    std::string msg = "<action agent=\"media\" op=\"playByGenre\">"
            "<band></band>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioPlayByNameTransform_DAB)
{
    std::string msg = "<action agent=\"media\" op=\"playByName\">"
            "<band>DAB</band>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioPlayByNameTransform_XM)
{
    std::string msg = "<action agent=\"media\" op=\"playByName\">"
            "<band>XM</band>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioPlayByNameTransform_FM)
{
    std::string msg = "<action agent=\"media\" op=\"playByName\">"
            "<band>FM</band>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioPlayByNameTransform_other)
{
    std::string msg = "<action agent=\"media\" op=\"playByName\">"
            "<band></band>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioPlayByPresetTransform)
{
    std::string msg = "<action agent=\"media\" op=\"playByPreset\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioQueryActiveFrequencyBandTransform)
{
    std::string msg = "<action agent=\"media\" op=\"queryActiveFrequencyBand\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioQueryRadioStatusTransform)
{
    std::string msg = "<action agent=\"media\" op=\"queryRadioStatus\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioQueryFreqBandStatusTransform)
{
    std::string msg = "<action agent=\"media\" op=\"queryFreqBandStatus\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioQueryHDSubchannelAvailableTransform)
{
    std::string msg = "<action agent=\"media\" op=\"queryHDSubchannelAvailable\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioQueryCurrentHDSubchannelAvailableTransform)
{
    std::string msg = "<action agent=\"media\" op=\"queryCurrentHDSubchannelAvailable\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioQueryPresetsAvailableTransform)
{
    std::string msg = "<action agent=\"media\" op=\"queryPresetsAvailable\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioQueryPresetDefinedTransform)
{
    std::string msg = "<action agent=\"media\" op=\"queryPresetDefined\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_RadioQueryFMGenreAvailableTransform)
{
    std::string msg = "<action agent=\"media\" op=\"queryFMGenreAvailable\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_LaunchAppTransform_dataSize1)
{
    std::string msg = "<action agent=\"apps\" op=\"launchApp\">"
            "<dataSize>1</dataSize>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_LaunchAppTransform_dataSize0)
{
    std::string msg = "<action agent=\"apps\" op=\"launchApp\">"
            "<dataSize>0</dataSize>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_EventResultButtonPressedTransform)
{
    std::string msg = "<event-result name=\"buttonPressed\">"
            "<keycode value=\"select_three\">"
        "</event-result>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_EventResultStartAgentTransform)
{
    std::string msg = "<event-result name=\"startAgent\">"
        "</event-result>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_EventResultCancelTransform)
{
    std::string msg = "<event-result name=\"cancel\">"
        "</event-result>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PlayTtsTransform)
{
    std::string msg = "<action agent=\"prompt\" op=\"playTts\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_StopTtsTransform)
{
    std::string msg = "<action agent=\"prompt\" op=\"stopTts\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PlayBeepTransform)
{
    std::string msg = "<action agent=\"prompt\" op=\"playBeep\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_StopBeepTransform)
{
    std::string msg = "<action agent=\"prompt\" op=\"stopBeep\">"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ShowScreenTransform_HVACLexusConciergeScreen)
{
    std::string msg = "<action agent=\"climate\" op=\"showScreen\">"
            "<type>CONCIERGE</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ShowScreenTransform_HVACFrontScreen)
{
    std::string msg = "<action agent=\"climate\" op=\"showScreen\">"
            "<type>FRONT</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ShowScreenTransform_HVACFrontSeatScreen)
{
    std::string msg = "<action agent=\"climate\" op=\"showScreen\">"
            "<type>FRONT_SEAT</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ShowScreenTransform_FrontSeatVentilationScreen)
{
    std::string msg = "<action agent=\"climate\" op=\"showScreen\">"
            "<type>FRONT_SEAT_VENTILATION</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ShowScreenTransform_HVACRearScreen)
{
    std::string msg = "<action agent=\"climate\" op=\"showScreen\">"
            "<type>REAR</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ShowScreenTransform_HVACRearSeatScreen)
{
    std::string msg = "<action agent=\"climate\" op=\"showScreen\">"
            "<type>REAR_SEAT</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ShowScreenTransform_RearSeatVentilationScreen)
{
    std::string msg = "<action agent=\"climate\" op=\"showScreen\">"
            "<type>REAR_SEAT_VENTILATION</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ShowScreenTransform_HVACSeatOperationScreen)
{
    std::string msg = "<action agent=\"climate\" op=\"showScreen\">"
            "<type>SEAT_OPERATION</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ShowScreenTransform_HVACSteeringScreen)
{
    std::string msg = "<action agent=\"climate\" op=\"showScreen\">"
            "<type>STEERING</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ShowScreenTransform_other)
{
    std::string msg = "<action agent=\"climate\" op=\"showScreen\">"
            "<type></type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_QueryStatusTransform_AirConditionState)
{
    std::string msg = "<action agent=\"climate\" op=\"queryStatus\">"
            "<type>AUTO_AIRCONDITION</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_QueryStatusTransform_ACState)
{
    std::string msg = "<action agent=\"climate\" op=\"queryStatus\">"
            "<type>AC</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_QueryStatusTransform_DualMode)
{
    std::string msg = "<action agent=\"climate\" op=\"queryStatus\">"
            "<type>DUAL_MODE</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_QueryStatusTransform_WiperDeicer)
{
    std::string msg = "<action agent=\"climate\" op=\"queryStatus\">"
            "<type>WIPER_DEICER</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_QueryStatusTransform_RearDefogger)
{
    std::string msg = "<action agent=\"climate\" op=\"queryStatus\">"
            "<type>REAR_DEFOGGER</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_QueryStatusTransform_ClimateConciergeMode)
{
    std::string msg = "<action agent=\"climate\" op=\"queryStatus\">"
            "<type>CONCIERGE_MODE</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_QueryStatusTransform_other)
{
    std::string msg = "<action agent=\"climate\" op=\"queryStatus\">"
            "<type></type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_TemperatureTransform)
{
    std::string msg = "<action agent=\"climate\" op=\"queryTemperature\">"
            "<type></type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_TemperatureRangeTransform)
{
    std::string msg = "<action agent=\"climate\" op=\"queryTemperatureRange\">"
            "<type></type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_FanSpeedTransform)
{
    std::string msg = "<action agent=\"climate\" op=\"queryFanSpeed\">"
            "<type></type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_BlowerModeTransfrom)
{
    std::string msg = "<action agent=\"climate\" op=\"getFanMode\">"
            "<type></type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_TurnClimateTransform_AutoAirConditionerON)
{
    std::string msg = "<action agent=\"climate\" op=\"turn\">"
            "<status>ON</status>"
            "<type>AUTO_AIRCONDITION</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_TurnClimateTransform_ACON)
{
    std::string msg = "<action agent=\"climate\" op=\"turn\">"
            "<status>ON</status>"
            "<type>AC</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_TurnClimateTransform_DualModeON)
{
    std::string msg = "<action agent=\"climate\" op=\"turn\">"
            "<status>ON</status>"
            "<type>DUAL_MODE</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_TurnClimateTransform_WiperDeicerON)
{
    std::string msg = "<action agent=\"climate\" op=\"turn\">"
            "<status>ON</status>"
            "<type>WIPER_DEICER</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_TurnClimateTransform_RearDefoggerON)
{
    std::string msg = "<action agent=\"climate\" op=\"turn\">"
            "<status>ON</status>"
            "<type>REAR_DEFOGGER</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_TurnClimateTransform_RearSeatAirConditionerON)
{
    std::string msg = "<action agent=\"climate\" op=\"turn\">"
            "<status>ON</status>"
            "<type>REAR_AUTO_AIRCONDITION</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_TurnClimateTransform_ClimateConciergeModeON)
{
    std::string msg = "<action agent=\"climate\" op=\"turn\">"
            "<status>ON</status>"
            "<type>CONCIERGE_MODE</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_TurnClimateTransform_other)
{
    std::string msg = "<action agent=\"climate\" op=\"turn\">"
            "<status>ON</status>"
            "<type></type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_TurnClimateTransform_AutoAirConditionerOFF)
{
    std::string msg = "<action agent=\"climate\" op=\"turn\">"
            "<status>OFF</status>"
            "<type>AUTO_AIRCONDITION</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_TurnClimateTransform_ACOFF)
{
    std::string msg = "<action agent=\"climate\" op=\"turn\">"
            "<status>OFF</status>"
            "<type>AC</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_TurnClimateTransform_DualModeOFF)
{
    std::string msg = "<action agent=\"climate\" op=\"turn\">"
            "<status>OFF</status>"
            "<type>DUAL_MODE</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_TurnClimateTransform_WiperDeicerOFF)
{
    std::string msg = "<action agent=\"climate\" op=\"turn\">"
            "<status>OFF</status>"
            "<type>WIPER_DEICER</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_TurnClimateTransform_RearDefoggerOFF)
{
    std::string msg = "<action agent=\"climate\" op=\"turn\">"
            "<status>OFF</status>"
            "<type>REAR_DEFOGGER</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_TurnClimateTransform_RearSeatAirConditionerOFF)
{
    std::string msg = "<action agent=\"climate\" op=\"turn\">"
            "<status>OFF</status>"
            "<type>REAR_AUTO_AIRCONDITION</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_TurnClimateTransform_off_other)
{
    std::string msg = "<action agent=\"climate\" op=\"turn\">"
            "<status>OFF</status>"
            "<type></type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ControlTempTransform_IncreaseTemperature)
{
    std::string msg = "<action agent=\"climate\" op=\"controlTemperature\">"
            "<type>INC</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ControlTempTransform_DecreaseTemperature)
{
    std::string msg = "<action agent=\"climate\" op=\"controlTemperature\">"
            "<type>DEC</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ControlTempTransform_AFewWarmer)
{
    std::string msg = "<action agent=\"climate\" op=\"controlTemperature\">"
            "<type>INC_LITTLE</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ControlTempTransform_AFewCooler)
{
    std::string msg = "<action agent=\"climate\" op=\"controlTemperature\">"
            "<type>DEC_LITTLE</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ControlTempTransform_other)
{
    std::string msg = "<action agent=\"climate\" op=\"controlTemperature\">"
            "<type></type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeTemperatureTransform_max)
{
    std::string msg = "<action agent=\"climate\" op=\"changeTemperature\">"
            "<degree>max</degree>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeTemperatureTransform_MAX)
{
    std::string msg = "<action agent=\"climate\" op=\"changeTemperature\">"
            "<degree>MAX</degree>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeTemperatureTransform_min)
{
    std::string msg = "<action agent=\"climate\" op=\"changeTemperature\">"
            "<degree>min</degree>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeTemperatureTransform_MIN)
{
    std::string msg = "<action agent=\"climate\" op=\"changeTemperature\">"
            "<degree>MIN</degree>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeTemperatureTransform_CELSIUS_MAX)
{
    std::string msg = "<action agent=\"climate\" op=\"changeTemperature\">"
            "<degree>32</degree>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeTemperatureTransform_CELSIUS)
{
    std::string msg = "<action agent=\"climate\" op=\"changeTemperature\">"
            "<degree>14</degree>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeTemperatureTransform_FAHRENHEIT_MAX)
{
    std::string msg = "<action agent=\"climate\" op=\"changeTemperature\">"
            "<degree>85</degree>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeTemperatureTransform_FAHRENHEIT)
{
    std::string msg = "<action agent=\"climate\" op=\"changeTemperature\">"
            "<degree>59</degree>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeTemperatureTransform_other)
{
    std::string msg = "<action agent=\"climate\" op=\"changeTemperature\">"
            "<degree>86</degree>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeFanSpeedTransform_Max)
{
    std::string msg = "<action agent=\"climate\" op=\"changeFanSpeed\">"
            "<speed>Max</speed>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeFanSpeedTransform_MAX)
{
    std::string msg = "<action agent=\"climate\" op=\"changeFanSpeed\">"
            "<speed>MAX</speed>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeFanSpeedTransform_Min)
{
    std::string msg = "<action agent=\"climate\" op=\"changeFanSpeed\">"
            "<speed>Min</speed>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeFanSpeedTransform_MIN)
{
    std::string msg = "<action agent=\"climate\" op=\"changeFanSpeed\">"
            "<speed>MIN</speed>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeFanSpeedTransform_other)
{
    std::string msg = "<action agent=\"climate\" op=\"changeFanSpeed\">"
            "<speed>2</speed>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeFanModeTransform_BlowerModeFace)
{
    std::string msg = "<action agent=\"climate\" op=\"changeFanMode\">"
            "<mode>FACE</mode>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeFanModeTransform_BlowerModeFoot)
{
    std::string msg = "<action agent=\"climate\" op=\"changeFanMode\">"
            "<mode>FOOT</mode>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeFanModeTransform_BlowerModeFaceAndFoot)
{
    std::string msg = "<action agent=\"climate\" op=\"changeFanMode\">"
            "<mode>FACE_TO_FOOT</mode>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeFanModeTransform_BlowerModeWindowAndFoot)
{
    std::string msg = "<action agent=\"climate\" op=\"changeFanMode\">"
            "<mode>WINDOW_TO_FOOT</mode>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeFanModeTransform_other)
{
    std::string msg = "<action agent=\"climate\" op=\"changeFanMode\">"
            "<mode></mode>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_GetValidTemperature_increase)
{
    std::string msg = "<action agent=\"climate\" op=\"getValidTemperature\">"
            "<type>increase</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_GetValidTemperature_decrease)
{
    std::string msg = "<action agent=\"climate\" op=\"getValidTemperature\">"
            "<type>decrease</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_GetValidTemperature_max)
{
    std::string msg = "<action agent=\"climate\" op=\"getValidTemperature\">"
            "<type>max</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_GetValidTemperature_min)
{
    std::string msg = "<action agent=\"climate\" op=\"getValidTemperature\">"
            "<type>min</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_GetValidTemperature_other)
{
    std::string msg = "<action agent=\"climate\" op=\"getValidTemperature\">"
            "<type>other</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_CheckCurrTemperature)
{
    std::string msg = "<action agent=\"climate\" op=\"checkTemperature\">"
            "<type>other</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_GetValidFanSpeed_increase)
{
    std::string msg = "<action agent=\"climate\" op=\"getValidFanSpeed\">"
            "<type>increase</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_GetValidFanSpeed_decrease)
{
    std::string msg = "<action agent=\"climate\" op=\"getValidFanSpeed\">"
            "<type>decrease</type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_GetValidFanSpeed_other)
{
    std::string msg = "<action agent=\"climate\" op=\"getValidFanSpeed\">"
            "<type></type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_CheckCurrFanSpeed)
{
    std::string msg = "<action agent=\"climate\" op=\"checkFanSpeed\">"
            "<type></type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeVoiceTagResult_noErrorCode)
{
    std::string errorCode = "";
    int funcCode = VR_TransformXmlToProto::VOICETAG_OK;
    m_VR_TransformXmlToProto->ChangeVoiceTagResult(errorCode, funcCode);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeVoiceTagResult_Recorded)
{
    std::string errorCode = "0";
    int funcCode = VrVoiceTagFunc_AddNewRecord;
    m_VR_TransformXmlToProto->ChangeVoiceTagResult(errorCode, funcCode);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeVoiceTagResult_Registered)
{
    std::string errorCode = "0";
    int funcCode = VrVoiceTagFunc_Register;
    m_VR_TransformXmlToProto->ChangeVoiceTagResult(errorCode, funcCode);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeVoiceTagResult_Deleted)
{
    std::string errorCode = "0";
    int funcCode = VrVoiceTagFunc_Delete;
    m_VR_TransformXmlToProto->ChangeVoiceTagResult(errorCode, funcCode);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeVoiceTagResult_RecordCanceled)
{
    std::string errorCode = "0";
    int funcCode = VrVoiceTagFunc_CancelRecord;
    m_VR_TransformXmlToProto->ChangeVoiceTagResult(errorCode, funcCode);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeVoiceTagResult_RegisterCanceled)
{
    std::string errorCode = "0";
    int funcCode = VrVoiceTagFunc_CancelRegister;
    m_VR_TransformXmlToProto->ChangeVoiceTagResult(errorCode, funcCode);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeVoiceTagResult_0Failure)
{
    std::string errorCode = "0";
    int funcCode = VrVoiceTagFunc_Undefined;
    m_VR_TransformXmlToProto->ChangeVoiceTagResult(errorCode, funcCode);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeVoiceTagResult_Failure)
{
    std::string errorCode = "4";
    int funcCode = VrVoiceTagFunc_Undefined;
    m_VR_TransformXmlToProto->ChangeVoiceTagResult(errorCode, funcCode);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ChangeVoiceTagResult_default)
{
    std::string errorCode = "5";
    int funcCode = VrVoiceTagFunc_Undefined;
    m_VR_TransformXmlToProto->ChangeVoiceTagResult(errorCode, funcCode);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_EventResultAddVoiceTagTransform)
{
    std::string msg = "<event-result name=\"addRecordVoiceTag\">"
        "</event-result>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_EventResultEditVoiceTagTransform)
{
    std::string msg = "<event-result name=\"editRecordVoiceTag\">"
        "</event-result>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_EventResultSaveVoiceTagTransform)
{
    std::string msg = "<event-result name=\"saveVoiceTag\">"
        "</event-result>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_EventResultDeleteVoiceTagTransform_IDEmpty)
{
    std::string msg = "<event-result name=\"deleteVoiceTag\">"
        "</event-result>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_EventResultDeleteVoiceTagTransform)
{
    std::string msg = "<event-result name=\"deleteVoiceTag\">"
            "<voiceTagId>1</voiceTagId>"
        "</event-result>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_EventResultPlayVoiceTagTransform)
{
    std::string msg = "<event-result name=\"playVoiceTag\">"
        "</event-result>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_EventResultStopVoiceTagTransform)
{
    std::string msg = "<event-result name=\"stopVoiceTag\">"
        "</event-result>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_EventResultStopVoiceTagTransform_ErrorCode)
{
    std::string msg = "<event-result name=\"stopVoiceTag\" errcode=\"15\">"
        "</event-result>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_EventResultCancelRecoedVoiceTagTransform)
{
    std::string msg = "<event-result name=\"cancelRecordVoiceTag\">"
        "</event-result>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_EventResultCancelSaveVoiceTagTransform)
{
    std::string msg = "<event-result name=\"cancelSaveVoiceTag\">"
        "</event-result>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_EventResultSyncVoiceTagTransform)
{
    std::string msg = "<event-result name=\"syncVoiceTag\">"
            "<voiceTagIds>1,2</voiceTagIds>"
        "</event-result>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_EventResultSyncVoiceTagTransform_empty)
{
    std::string msg = "<event-result name=\"syncVoiceTag\">"
        "</event-result>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_EventResultTslTransform)
{
    std::string msg = "<event-result name=\"install\">"
            "<voiceTagIds>1,2</voiceTagIds>"
        "</event-result>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_ActionNotifyTslTransform)
{
    std::string msg = "<action name=\"notifyInstallProgress\">"
            "<voiceTagIds>1,2</voiceTagIds>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_CloseSessionTransform)
{
    std::string msg = "<action name=\"closeSession\">"
            "<voiceTagIds>1,2</voiceTagIds>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_EventResultStartServiceConditonTransform)
{
    std::string msg = "<event-result name=\"getResourceState\">"
            "<voiceTagIds>1,2</voiceTagIds>"
        "</event-result>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_EventResultChangeLanguageTransform)
{
    std::string msg = "<event-result name=\"changeLanguage\">"
            "<voiceTagIds>1,2</voiceTagIds>"
        "</event-result>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_EventResultSmartAgentTransform)
{
    std::string msg = "<event-reuslt name=\"smartagent\">"
        "</event-result>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PhoneGetMessageDetailTransform)
{
    std::string msg = "<action agent=\"phone\" op=\"getMessageDetail\">"
            "<type></type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PhoneAddVoiceTagTransform)
{
    std::string msg = "<action agent=\"phone\" op=\"addVoiceTag\">"
            "<type></type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PhoneShowRecentCallTransform)
{
    std::string msg = "<action agent=\"phone\" op=\"showRecentCallList\">"
            "<type></type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PhoneListAllContactTransform)
{
    std::string msg = "<action agent=\"phone\" op=\"showContactList\">"
            "<type></type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PhoneListFavourContactTransform)
{
    std::string msg = "<action agent=\"phone\" op=\"showFavoriteContactList\">"
            "<type></type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PhoneSendMessageTransform)
{
    std::string msg = "<action agent=\"phone\" op=\"sendMessage\">"
            "<type></type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PhoneSendDTMFToneTransform)
{
    std::string msg = "<action agent=\"phone\" op=\"sendDTMFTones\">"
            "<type></type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PhoneMuteOffTransform)
{
    std::string msg = "<action agent=\"phone\" op=\"muteOff\">"
            "<type></type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PhoneMuteOnTransform)
{
    std::string msg = "<action agent=\"phone\" op=\"muteCall\">"
            "<type></type>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PhoneDialTransform_Call)
{
    std::string msg = "<action agent=\"phone\" op=\"dial\">"
            "<dialType>CALL</dialType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PhoneDialTransform_Redial)
{
    std::string msg = "<action agent=\"phone\" op=\"dial\">"
            "<dialType>REDIA</dialType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PhoneDialTransform_CallWithNo)
{
    std::string msg = "<action agent=\"phone\" op=\"dial\">"
            "<dialType>CALL_NUMBER</dialType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PhoneDialTransform_CallRecent)
{
    std::string msg = "<action agent=\"phone\" op=\"dial\">"
            "<dialType>CALL_RECENT</dialType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PhoneDialTransform_CallShortRing)
{
    std::string msg = "<action agent=\"phone\" op=\"dial\">"
            "<dialType>CALL_SHORTRING</dialType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PhoneDialTransform_other)
{
    std::string msg = "<action agent=\"phone\" op=\"dial\">"
            "<dialType></dialType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PhoneGetMsgListByTypeTransform_EMAIL)
{
    std::string msg = "<action agent=\"phone\" op=\"queryRecentMessageByType\">"
            "<messageType>EMAIL</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PhoneGetMsgListByTypeTransform_SMS)
{
    std::string msg = "<action agent=\"phone\" op=\"queryRecentMessageByType\">"
            "<messageType>SMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PhoneGetMsgListByTypeTransform_MMS)
{
    std::string msg = "<action agent=\"phone\" op=\"queryRecentMessageByType\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PhoneGetRecentMsgListTransform)
{
    std::string msg = "<action agent=\"phone\" op=\"queryRecentMessage\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PhoneGetOutGoingCallListTransform)
{
    std::string msg = "<action agent=\"phone\" op=\"queryOutgoingCall\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PhoneGetInComingCallListTransform)
{
    std::string msg = "<action agent=\"phone\" op=\"queryIncomingCall\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PhoneGetRecentCallListTransform)
{
    std::string msg = "<action agent=\"phone\" op=\"queryRecentCallList\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_PhoneVehicleMontionTransform)
{
    std::string msg = "<action agent=\"phone\" op=\"queryVehicleinMotion\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_QueryVehicleMontionTransform)
{
    std::string msg = "<action agent=\"help\" op=\"queryVehicleinMotion\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_SmartAgentPromptLevelTransform)
{
    std::string msg = "<action agent=\"smartagent\" op=\"setPromptLevel\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_GBookCheckStatusTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"queryDestinationAssistAvailable\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_GBookCheckErrorTransform)
{
    std::string msg = "<action agent=\"communication\" op=\"queryGBookStatus\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviReqDfltInfoTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"requestDefaultInfo\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryStateInfoTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"queryStateInfo\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryStateInfoTransform_cnde)
{
    std::string msg = "<action agent=\"navi\" op=\"queryStateInfo\" detype=\"cnde\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryCityInfoTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"queryCityInfo\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryCityInfoTransform_cnde)
{
    std::string msg = "<action agent=\"navi\" op=\"queryCityInfo\" detype=\"cnde\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryStreetInfoTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"queryStreetInfo\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryStreetInfoTransform_cnde)
{
    std::string msg = "<action agent=\"navi\" op=\"queryStreetInfo\" detype=\"cnde\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryAddressInfoTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"queryAddressInfo\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryAddressInfoTransform_cnde)
{
    std::string msg = "<action agent=\"navi\" op=\"queryAddressInfo\" detype=\"cnde\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryOneShotInfoTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"queryOneShotInfo\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryDistrictInfoTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"queryDistrictInfo\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryShowPOIIconNoTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"queryShowingPOIIconNumber\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryPOINearbyTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOICollectionNearby\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryPOINearDestinationTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOICollectionNearDestination\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryPOIAlongRouteTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOICollectionAlongRoute\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryPointCollectionTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPointCollection\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryDestHistoryTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"queryDestinationHistory\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryRouteExistTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"queryRouteExist\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryAllDestListTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"queryAllDestinationList\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryUnpassedDestListTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"queryUnpassedDestinationList\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryPointTypeTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPointType\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryNextDestTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"queryNextDestination\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryPointDateAreaTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPointDataArea\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviShowPOIIconTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"showPOIIcon\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviHidePOIIconTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"hidePOIIcon\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviAddToRouteTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"addToRoute\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviDelDestinationTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"deleteDestination\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviCalcRouteTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"calculateRoute\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryCurrentPosTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"queryCurrentPosition\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, Transform_NaviQueryPOIByNameTransform)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">"
            "<messageType>MMS</messageType>"
        "</action>";
    m_VR_TransformXmlToProto->Transform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, MusicPlayAndShowTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->MusicPlayAndShowTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, EventResultButtonPressedTransform)
{
    pugi::xml_document doc;
    std::string msg = "<event name=\"buttonPressed\">""<keycode value=\"pause\"/>""</event>";
    if (doc.load(msg.c_str())) {
        m_VR_TransformXmlToProto->EventResultButtonPressedTransform(1, doc);
    }
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, EventResultSmartAgentTransform)
{
    pugi::xml_document doc;
    std::string msg = "<event-reuslt name=\"smartagent\">"
        "</event-result>";
    if (doc.load(msg.c_str())) {
        m_VR_TransformXmlToProto->EventResultSmartAgentTransform(1, doc);
    }
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviReqDfltInfoTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviReqDfltInfoTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviQueryStateInfoTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviQueryStateInfoTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviQueryCityInfoTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviQueryCityInfoTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviQueryStreetInfoTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviQueryStreetInfoTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviQueryAddressInfoTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviQueryAddressInfoTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviQueryOneShotInfoTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviQueryOneShotInfoTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviQueryDistrictInfoTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviQueryDistrictInfoTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviQueryShowPOIIconNoTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviQueryShowPOIIconNoTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviQueryPOINearbyTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviQueryPOINearbyTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviQueryPOINearDestinationTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviQueryPOINearDestinationTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviQueryPOIAlongRouteTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviQueryPOIAlongRouteTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviQueryPointCollectionTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviQueryPointCollectionTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviQueryDestHistoryTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviQueryDestHistoryTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviQueryRouteExistTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviQueryRouteExistTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviQueryAllDestListTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviQueryAllDestListTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviQueryUnpassedDestListTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviQueryUnpassedDestListTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviQueryPointTypeTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviQueryPointTypeTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviQueryNextDestTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviQueryNextDestTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviQueryPointDateAreaTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviQueryPointDateAreaTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviShowPOIIconTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviShowPOIIconTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviHidePOIIconTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviHidePOIIconTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviAddToRouteTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviAddToRouteTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviDelDestinationTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviDelDestinationTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviCalcRouteTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviCalcRouteTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviQueryCurrentPosTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviQueryCurrentPosTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, NaviQueryPOIByNameTransform_loadFailed)
{
    std::string msg = "<action agent=\"navi\" op=\"queryPOIByName\">";
    m_VR_TransformXmlToProto->NaviQueryPOIByNameTransform(1, msg);
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, RadioQueryActiveFrequencyBandTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->RadioQueryActiveFrequencyBandTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, EventResultButtonPressedTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->EventResultButtonPressedTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, EventResultStartAgentTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->EventResultStartAgentTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, PlayTtsTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->PlayTtsTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, StopTtsTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->StopTtsTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, PlayBeepTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->PlayBeepTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, StopBeepTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->StopBeepTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, ShowScreenTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->ShowScreenTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, QueryStatusTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->QueryStatusTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, TemperatureTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->TemperatureTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, TemperatureRangeTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->TemperatureRangeTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, FanSpeedTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->FanSpeedTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, BlowerModeTransfrom_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->BlowerModeTransfrom(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, TurnClimateTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->TurnClimateTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, ControlTempTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->ControlTempTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, ChangeTemperatureTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->ChangeTemperatureTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, ChangeFanSpeedTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->ChangeFanSpeedTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, ChangeFanModeTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->ChangeFanModeTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, GetValidTemperature_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->GetValidTemperature(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, CheckCurrTemperature_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->CheckCurrTemperature(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, GetValidFanSpeed_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->GetValidFanSpeed(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, CheckCurrFanSpeed_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->CheckCurrFanSpeed(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, EventResultAddVoiceTagTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->EventResultAddVoiceTagTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, EventResultEditVoiceTagTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->EventResultEditVoiceTagTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, EventResultSaveVoiceTagTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->EventResultSaveVoiceTagTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, EventResultDeleteVoiceTagTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->EventResultDeleteVoiceTagTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, EventResultPlayVoiceTagTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->EventResultPlayVoiceTagTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, EventResultStopVoiceTagTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->EventResultStopVoiceTagTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, EventResultCancelRecoedVoiceTagTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->EventResultCancelRecoedVoiceTagTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, EventResultCancelSaveVoiceTagTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->EventResultCancelSaveVoiceTagTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, EventResultSyncVoiceTagTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->EventResultSyncVoiceTagTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, EventResultTslTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->EventResultTslTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, ActionNotifyTslTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->ActionNotifyTslTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, CloseSessionTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->CloseSessionTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, EventResultStartServiceConditonTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->EventResultStartServiceConditonTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, EventResultChangeLanguageTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->EventResultChangeLanguageTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, EventResultSmartAgentTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->EventResultSmartAgentTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, PhoneGetMessageDetailTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->PhoneGetMessageDetailTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, PhoneAddVoiceTagTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->PhoneAddVoiceTagTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, PhoneShowRecentCallTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->PhoneShowRecentCallTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, PhoneListAllContactTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->PhoneListAllContactTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, PhoneListFavourContactTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->PhoneListFavourContactTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, PhoneSendMessageTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->PhoneSendMessageTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, PhoneSendDTMFToneTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->PhoneSendDTMFToneTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, PhoneMuteOffTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->PhoneMuteOffTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, PhoneMuteOnTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->PhoneMuteOnTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, PhoneDialTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->PhoneDialTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, PhoneGetMsgListByTypeTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->PhoneGetMsgListByTypeTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, PhoneGetRecentMsgListTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->PhoneGetRecentMsgListTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, PhoneGetOutGoingCallListTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->PhoneGetOutGoingCallListTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, PhoneGetInComingCallListTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->PhoneGetInComingCallListTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, PhoneGetRecentCallListTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->PhoneGetRecentCallListTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, PhoneVehicleMontionTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->PhoneVehicleMontionTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, QueryVehicleMontionTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->QueryVehicleMontionTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, SmartAgentPromptLevelTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->SmartAgentPromptLevelTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, GBookCheckStatusTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->GBookCheckStatusTransform(1, doc);
    doc.back();
    SUCCEED();
}

TEST_F(VR_TransformXmlToProto_Test, GBookCheckErrorTransform_loadFailed)
{
    MyXml_docment doc;
    doc.clear();
    m_VR_TransformXmlToProto->GBookCheckErrorTransform(1, doc);
    doc.back();
    SUCCEED();
}

/* EOF */

