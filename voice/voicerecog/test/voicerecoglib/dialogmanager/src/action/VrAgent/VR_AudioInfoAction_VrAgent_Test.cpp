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
#include "VR_AudioInfoAction_VrAgent.h"
#include "VR_ActionEventSender.h"
#include "VR_DialogEngine_mock.h"
#include "VR_AvcLanSender_mock.h"
#include "VR_AvcLanSenderIF.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "MediaInfoProvider/NMMediaInfoHelper.h"

using namespace navi::VoiceRecog;
using namespace nutshell::media;

/**
* VR_AudioInfoAction_VrAgent_Test
*
* The class is just for VR_AudioInfoAction_VrAgent test.
*/
class VR_AudioInfoAction_VrAgent_Test : public testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

public:
    spVR_ActionEventSender m_sp_EventSender;
    VR_AudioInfoAction_VrAgent *m_audioInfoAction;
};

void VR_AudioInfoAction_VrAgent_Test::SetUp()
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(0);
    protoMsg->set_source_type(0);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
}

void VR_AudioInfoAction_VrAgent_Test::TearDown()
{
    if (NULL != m_audioInfoAction) {
        delete m_audioInfoAction;
        m_audioInfoAction = NULL;
    }
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, OnReceive_Case)
{
    std::unique_ptr<VrRecogResultConfirm> protoMsg(VrRecogResultConfirm::default_instance().New());
    protoMsg->set_action_id(USBDeviceInfo);
    protoMsg->set_processing_result(true);

    m_audioInfoAction->OnReceive(*protoMsg);
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentAlbumRequest_ipod)
{
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentAlbumRequest_ipod_Play_hasIpod)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(0);
    protoMsg->set_source_type(0);
    struct MediaInfo_iPodPlayInfo iPodPlayInfo;
    iPodPlayInfo.state = MediaInfo_iPodPlayInfo::PlayState::PlayState_Play;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_iPodPlayInfo, &iPodPlayInfo, sizeof(iPodPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    struct MediaInfo_iPod ipodInfo;
    ipodInfo.songId = 1;
    bret = NMMediaInfoHelper::save(MediaInfoKey_iPod, &ipodInfo, sizeof(ipodInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentAlbumRequest_ipod_Play_hasNoIpod)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(0);
    protoMsg->set_source_type(0);
    struct MediaInfo_iPodPlayInfo iPodPlayInfo;
    iPodPlayInfo.state = MediaInfo_iPodPlayInfo::PlayState::PlayState_Play;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_iPodPlayInfo, &iPodPlayInfo, sizeof(iPodPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentAlbumRequest_ipod_Stop)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(0);
    protoMsg->set_source_type(0);
    struct MediaInfo_iPodPlayInfo iPodPlayInfo;
    iPodPlayInfo.state = MediaInfo_iPodPlayInfo::PlayState::PlayState_Stop;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_iPodPlayInfo, &iPodPlayInfo, sizeof(iPodPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentArtistRequest_ipod)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(1);
    protoMsg->set_source_type(0);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentArtistRequest_ipod_Play_hasIpod)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(1);
    protoMsg->set_source_type(0);
    struct MediaInfo_iPodPlayInfo iPodPlayInfo;
    iPodPlayInfo.state = MediaInfo_iPodPlayInfo::PlayState::PlayState_Play;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_iPodPlayInfo, &iPodPlayInfo, sizeof(iPodPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    struct MediaInfo_iPod ipodInfo;
    ipodInfo.songId = 1;
    bret = NMMediaInfoHelper::save(MediaInfoKey_iPod, &ipodInfo, sizeof(ipodInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentArtistRequest_ipod_Play_hasNoIpod)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(1);
    protoMsg->set_source_type(0);
    struct MediaInfo_iPodPlayInfo iPodPlayInfo;
    iPodPlayInfo.state = MediaInfo_iPodPlayInfo::PlayState::PlayState_Play;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_iPodPlayInfo, &iPodPlayInfo, sizeof(iPodPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentArtistRequest_ipod_Stop)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(1);
    protoMsg->set_source_type(0);
    struct MediaInfo_iPodPlayInfo iPodPlayInfo;
    iPodPlayInfo.state = MediaInfo_iPodPlayInfo::PlayState::PlayState_Stop;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_iPodPlayInfo, &iPodPlayInfo, sizeof(iPodPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentComposerRequest_ipod)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(2);
    protoMsg->set_source_type(0);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentComposerRequest_ipod_Play_hasIpod)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(2);
    protoMsg->set_source_type(0);
    struct MediaInfo_iPodPlayInfo iPodPlayInfo;
    iPodPlayInfo.state = MediaInfo_iPodPlayInfo::PlayState::PlayState_Play;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_iPodPlayInfo, &iPodPlayInfo, sizeof(iPodPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    struct MediaInfo_iPod ipodInfo;
    ipodInfo.songId = 1;
    bret = NMMediaInfoHelper::save(MediaInfoKey_iPod, &ipodInfo, sizeof(ipodInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentComposerRequest_ipod_Play_hasNoIpod)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(2);
    protoMsg->set_source_type(0);
    struct MediaInfo_iPodPlayInfo iPodPlayInfo;
    iPodPlayInfo.state = MediaInfo_iPodPlayInfo::PlayState::PlayState_Play;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_iPodPlayInfo, &iPodPlayInfo, sizeof(iPodPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentComposerRequest_ipod_Stop)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(2);
    protoMsg->set_source_type(0);
    struct MediaInfo_iPodPlayInfo iPodPlayInfo;
    iPodPlayInfo.state = MediaInfo_iPodPlayInfo::PlayState::PlayState_Stop;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_iPodPlayInfo, &iPodPlayInfo, sizeof(iPodPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentGenreRequest_ipod)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(3);
    protoMsg->set_source_type(0);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentGenreRequest_ipod_Play_hasIpod)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(3);
    protoMsg->set_source_type(0);
    struct MediaInfo_iPodPlayInfo iPodPlayInfo;
    iPodPlayInfo.state = MediaInfo_iPodPlayInfo::PlayState::PlayState_Play;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_iPodPlayInfo, &iPodPlayInfo, sizeof(iPodPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    struct MediaInfo_iPod ipodInfo;
    ipodInfo.songId = 1;
    bret = NMMediaInfoHelper::save(MediaInfoKey_iPod, &ipodInfo, sizeof(ipodInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentGenreRequest_ipod_Play_hasNoIpod)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(3);
    protoMsg->set_source_type(0);
    struct MediaInfo_iPodPlayInfo iPodPlayInfo;
    iPodPlayInfo.state = MediaInfo_iPodPlayInfo::PlayState::PlayState_Play;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_iPodPlayInfo, &iPodPlayInfo, sizeof(iPodPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentGenreRequest_ipod_Stop)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(3);
    protoMsg->set_source_type(0);
    struct MediaInfo_iPodPlayInfo iPodPlayInfo;
    iPodPlayInfo.state = MediaInfo_iPodPlayInfo::PlayState::PlayState_Stop;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_iPodPlayInfo, &iPodPlayInfo, sizeof(iPodPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentAlbumRequest_usb)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(0);
    protoMsg->set_source_type(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentAlbumRequest_usb_Play_hasIpod)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(0);
    protoMsg->set_source_type(1);
    struct MediaInfo_UsbPlayInfo USBPlayInfo;
    USBPlayInfo.state = MediaInfo_UsbPlayInfo::PlayState::PlayState_Play;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_UsbPlayInfo, &USBPlayInfo, sizeof(USBPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    struct MediaInfo_Usb usbInfo;
    usbInfo.songId = 1;
    bret = NMMediaInfoHelper::save(MediaInfoKey_Usb, &usbInfo, sizeof(usbInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentAlbumRequest_usb_Play_hasNoIpod)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(0);
    protoMsg->set_source_type(1);
    struct MediaInfo_UsbPlayInfo USBPlayInfo;
    USBPlayInfo.state = MediaInfo_UsbPlayInfo::PlayState::PlayState_Play;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_UsbPlayInfo, &USBPlayInfo, sizeof(USBPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentAlbumRequest_usb_Stop)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(0);
    protoMsg->set_source_type(1);
    struct MediaInfo_UsbPlayInfo USBPlayInfo;
    USBPlayInfo.state = MediaInfo_UsbPlayInfo::PlayState::PlayState_Stop;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_UsbPlayInfo, &USBPlayInfo, sizeof(USBPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentArtistRequest_usb)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(1);
    protoMsg->set_source_type(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentArtistRequest_usb_Play_hasIpod)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(1);
    protoMsg->set_source_type(1);
    struct MediaInfo_UsbPlayInfo USBPlayInfo;
    USBPlayInfo.state = MediaInfo_UsbPlayInfo::PlayState::PlayState_Play;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_UsbPlayInfo, &USBPlayInfo, sizeof(USBPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    struct MediaInfo_Usb usbInfo;
    usbInfo.songId = 1;
    bret = NMMediaInfoHelper::save(MediaInfoKey_Usb, &usbInfo, sizeof(usbInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentArtistRequest_usb_Play_hasNoIpod)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(1);
    protoMsg->set_source_type(1);
    struct MediaInfo_UsbPlayInfo USBPlayInfo;
    USBPlayInfo.state = MediaInfo_UsbPlayInfo::PlayState::PlayState_Play;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_UsbPlayInfo, &USBPlayInfo, sizeof(USBPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentArtistRequest_usb_Stop)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(1);
    protoMsg->set_source_type(1);
    struct MediaInfo_UsbPlayInfo USBPlayInfo;
    USBPlayInfo.state = MediaInfo_UsbPlayInfo::PlayState::PlayState_Stop;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_UsbPlayInfo, &USBPlayInfo, sizeof(USBPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentComposerRequest_usb)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(2);
    protoMsg->set_source_type(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentComposerRequest_usb_Play_hasIpod)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(2);
    protoMsg->set_source_type(1);
    struct MediaInfo_UsbPlayInfo USBPlayInfo;
    USBPlayInfo.state = MediaInfo_UsbPlayInfo::PlayState::PlayState_Play;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_UsbPlayInfo, &USBPlayInfo, sizeof(USBPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    struct MediaInfo_Usb usbInfo;
    usbInfo.songId = 1;
    bret = NMMediaInfoHelper::save(MediaInfoKey_Usb, &usbInfo, sizeof(usbInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentComposerRequest_usb_Play_hasNoIpod)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(2);
    protoMsg->set_source_type(1);
    struct MediaInfo_UsbPlayInfo USBPlayInfo;
    USBPlayInfo.state = MediaInfo_UsbPlayInfo::PlayState::PlayState_Play;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_UsbPlayInfo, &USBPlayInfo, sizeof(USBPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentComposerRequest_usb_Stop)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(2);
    protoMsg->set_source_type(1);
    struct MediaInfo_UsbPlayInfo USBPlayInfo;
    USBPlayInfo.state = MediaInfo_UsbPlayInfo::PlayState::PlayState_Stop;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_UsbPlayInfo, &USBPlayInfo, sizeof(USBPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentGenreRequest_usb)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(3);
    protoMsg->set_source_type(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentGenreRequest_usb_Play_hasIpod)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(3);
    protoMsg->set_source_type(1);
    struct MediaInfo_UsbPlayInfo USBPlayInfo;
    USBPlayInfo.state = MediaInfo_UsbPlayInfo::PlayState::PlayState_Play;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_UsbPlayInfo, &USBPlayInfo, sizeof(USBPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    struct MediaInfo_Usb usbInfo;
    usbInfo.songId = 1;
    bret = NMMediaInfoHelper::save(MediaInfoKey_Usb, &usbInfo, sizeof(usbInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentGenreRequest_usb_Play_hasNoIpod)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(3);
    protoMsg->set_source_type(1);
    struct MediaInfo_UsbPlayInfo USBPlayInfo;
    USBPlayInfo.state = MediaInfo_UsbPlayInfo::PlayState::PlayState_Play;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_UsbPlayInfo, &USBPlayInfo, sizeof(USBPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_getCurrentGenreRequest_usb_Stop)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(3);
    protoMsg->set_source_type(1);
    struct MediaInfo_UsbPlayInfo USBPlayInfo;
    USBPlayInfo.state = MediaInfo_UsbPlayInfo::PlayState::PlayState_Stop;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_UsbPlayInfo, &USBPlayInfo, sizeof(USBPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, Run_default)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(4);
    protoMsg->set_source_type(0);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, ClimateConciergeMode, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrAgent_Test, RunAction_default)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrUSBDeviceInfoReq> protoMsg(VrUSBDeviceInfoReq::default_instance().New());
    protoMsg->set_request_id(4);
    protoMsg->set_source_type(0);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_audioInfoAction = VR_new VR_AudioInfoAction_VrAgent(m_sp_EventSender, 1, USBDeviceInfo, *protoMsg, avclanSender);
    m_audioInfoAction->Run();
    SUCCEED();
}

/* EOF */
