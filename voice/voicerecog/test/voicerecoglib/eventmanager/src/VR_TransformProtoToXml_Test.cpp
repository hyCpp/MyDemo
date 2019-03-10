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
#include <stdlib.h>
#include <sstream>

#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/assign/list_of.hpp>

#include "pugixml.hpp"
#include "VR_HAVCTypedefine.h"
#include "VR_TransformProtoToXml.h"
#include "VR_ActionInnerParameterDef.h"
#include "VR_ActionSenderDef.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrUnitStatus.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrMessageTrans.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActionNavi.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_TransformProtoToXml_Test.cpp
*
* The class is just for VR_TransformProtoToXml test.
*/
class  VR_TransformProtoToXml_Test : public testing::Test
{
protected:

    virtual   void  SetUp();
    virtual   void  TearDown();

    VR_TransformProtoToXml*     m_VR_TransformProtoToXml;
};


void
VR_TransformProtoToXml_Test::SetUp()
{
    m_VR_TransformProtoToXml = VR_new  VR_TransformProtoToXml();
}

void
VR_TransformProtoToXml_Test::TearDown()
{
    if (m_VR_TransformProtoToXml) {
        delete  m_VR_TransformProtoToXml;
        m_VR_TransformProtoToXml = NULL;
    }
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrMessageAcquisitionResp)
{
    VrMessageAcquisitionResp msgInfo;
    msgInfo.set_op_result(true);
    msgInfo.set_body("msg");
    m_VR_TransformProtoToXml->Transform(msgInfo);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrMessageAcquisitionResp_noOP)
{
    VrMessageAcquisitionResp msgInfo;
    msgInfo.set_body("msg");
    m_VR_TransformProtoToXml->Transform(msgInfo);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_MsgTypeEnumToXML_FAILED)
{
    m_VR_TransformProtoToXml->MsgTypeEnumToXML(-1);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrRecogResultConfirm_Failed)
{
    VrRecogResultConfirm msg;
    msg.set_action_id(10000);
    msg.set_processing_result(true);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrRecogResultConfirm_AudioOFF)
{
    VrRecogResultConfirm msg;
    msg.set_action_id(AudioOFF);
    msg.set_processing_result(false);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrRecogResultConfirm_AutoAirConditionerON)
{
    VrRecogResultConfirm msg;
    msg.set_action_id(AutoAirConditionerON);
    msg.set_processing_result(true);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrRecogResultConfirm_Call)
{
    VrRecogResultConfirm msg;
    msg.set_action_id(Call);
    msg.set_processing_result(true);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrRecogResultConfirm_StartApps)
{
    VrRecogResultConfirm msg;
    msg.set_action_id(StartApps);
    msg.set_processing_result(true);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrRecogResultConfirm_default)
{
    VrRecogResultConfirm msg;
    msg.set_action_id(-1);
    msg.set_processing_result(true);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_MsgNaviTypeEnumToXML_FAILED)
{
    m_VR_TransformProtoToXml->MsgNaviTypeEnumToXML(-1);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_MsgSpecialTypeEnumToXML_FAILED)
{
    m_VR_TransformProtoToXml->MsgSpecialTypeEnumToXML(-1);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_NAVI_queryDefaultInfo)
{
    m_VR_TransformProtoToXml->Transform(NAVI_ACTION_queryDefaultInfo, true);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_NAVI_default)
{
    m_VR_TransformProtoToXml->Transform(-1, true);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_SHOW_VOICE_SETTINGS)
{
    m_VR_TransformProtoToXml->TransformSpecialType(ACTION_TYPE_SHOW_VOICE_SETTINGS_UI, true);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_default)
{
    m_VR_TransformProtoToXml->TransformSpecialType(-1, false);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrAdditionalInfoRsp_USBDeviceInfo)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(USBDeviceInfo);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrAdditionalInfoRsp_RadioBandState)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(RadioBandState);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrAdditionalInfoRsp_RadioBandState0)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(RadioBandState);
    msg.mutable_radio_band_state_resp()->set_radio_band_state(0);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrAdditionalInfoRsp_RadioBandState1)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(RadioBandState);
    msg.mutable_radio_band_state_resp()->set_radio_band_state(1);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrAdditionalInfoRsp_CurFMChannelState)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(CurFMChannelState);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrAdditionalInfoRsp_SpecifiedFMState)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(SpecifiedFMState);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrAdditionalInfoRsp_PresetState)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(PresetState);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrAdditionalInfoRsp_SpecifiedPresetState)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(SpecifiedPresetState);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrAdditionalInfoRsp_RadioState)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(RadioState);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrAdditionalInfoRsp_RadioState0)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(RadioState);
    msg.mutable_radio_state_resp()->set_radio_band_state(0);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrAdditionalInfoRsp_RadioState1)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(RadioState);
    msg.mutable_radio_state_resp()->set_radio_band_state(1);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrAdditionalInfoRsp_GenreStationState)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(GenreStationState);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrAdditionalInfoRsp_AirConditionState)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(AirConditionState);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrAdditionalInfoRsp_Temperature)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(Temperature);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrAdditionalInfoRsp_TemperatureRange)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(TemperatureRange);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrAdditionalInfoRsp_FanSpeed)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(FanSpeed);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrAdditionalInfoRsp_BlowerMode)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(BlowerMode);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrAdditionalInfoRsp_default)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(-1);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrActiveInfoRsp_RadioActiveBand)
{
    VrActiveInfoRsp msg;
    msg.set_response_id(RadioActiveBand);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrActiveInfoRsp_ValidTemperature)
{
    VrActiveInfoRsp msg;
    msg.set_response_id(ValidTemperature);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrActiveInfoRsp_CheckTemperature)
{
    VrActiveInfoRsp msg;
    msg.set_response_id(CheckTemperature);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrActiveInfoRsp_ValidFanSpeed)
{
    VrActiveInfoRsp msg;
    msg.set_response_id(ValidFanSpeed);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrActiveInfoRsp_CheckFanSpeed)
{
    VrActiveInfoRsp msg;
    msg.set_response_id(CheckFanSpeed);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrActiveInfoRsp_RecentCallList)
{
    VrActiveInfoRsp msg;
    msg.set_response_id(RecentCallList);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrActiveInfoRsp_RecentIncomingCall)
{
    VrActiveInfoRsp msg;
    msg.set_response_id(RecentIncomingCall);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrActiveInfoRsp_RecentOutgoingCall)
{
    VrActiveInfoRsp msg;
    msg.set_response_id(RecentOutgoingCall);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrActiveInfoRsp_RecentMessageList)
{
    VrActiveInfoRsp msg;
    msg.set_response_id(RecentMessageList);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrActiveInfoRsp_RecentMessageListByType)
{
    VrActiveInfoRsp msg;
    msg.set_response_id(RecentMessageListByType);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrActiveInfoRsp_default)
{
    VrActiveInfoRsp msg;
    msg.set_response_id(GbookConnectComfirm);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformUSBDeviceInfo_NoUSBDeviceInfo)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(USBDeviceInfo);
    // msg.mutable_usb_device_info_resp();
    m_VR_TransformProtoToXml->TransformUSBDeviceInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformUSBDeviceInfo_GetCurrentAlbum_playing_hasSong)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(USBDeviceInfo);
    msg.mutable_usb_device_info_resp()->set_response_id(0);
    msg.mutable_usb_device_info_resp()->set_play_status(0);
    msg.mutable_usb_device_info_resp()->set_song_id("0");
    m_VR_TransformProtoToXml->TransformUSBDeviceInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformUSBDeviceInfo_GetCurrentAlbum_playing_noSong)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(USBDeviceInfo);
    msg.mutable_usb_device_info_resp()->set_response_id(0);
    msg.mutable_usb_device_info_resp()->set_play_status(0);
    m_VR_TransformProtoToXml->TransformUSBDeviceInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformUSBDeviceInfo_GetCurrentAlbum_notPlaying)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(USBDeviceInfo);
    msg.mutable_usb_device_info_resp()->set_response_id(0);
    msg.mutable_usb_device_info_resp()->set_play_status(1);
    m_VR_TransformProtoToXml->TransformUSBDeviceInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformUSBDeviceInfo_GetCurrentArtist_playing_hasSong)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(USBDeviceInfo);
    msg.mutable_usb_device_info_resp()->set_response_id(1);
    msg.mutable_usb_device_info_resp()->set_play_status(0);
    msg.mutable_usb_device_info_resp()->set_song_id("0");
    m_VR_TransformProtoToXml->TransformUSBDeviceInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformUSBDeviceInfo_GetCurrentArtist_playing_noSong)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(USBDeviceInfo);
    msg.mutable_usb_device_info_resp()->set_response_id(1);
    msg.mutable_usb_device_info_resp()->set_play_status(0);
    m_VR_TransformProtoToXml->TransformUSBDeviceInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformUSBDeviceInfo_GetCurrentArtist_notPlaying)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(USBDeviceInfo);
    msg.mutable_usb_device_info_resp()->set_response_id(1);
    msg.mutable_usb_device_info_resp()->set_play_status(1);
    m_VR_TransformProtoToXml->TransformUSBDeviceInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformUSBDeviceInfo_GetCurrentComposer_playing_hasSong)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(USBDeviceInfo);
    msg.mutable_usb_device_info_resp()->set_response_id(2);
    msg.mutable_usb_device_info_resp()->set_play_status(0);
    msg.mutable_usb_device_info_resp()->set_song_id("0");
    m_VR_TransformProtoToXml->TransformUSBDeviceInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformUSBDeviceInfo_GetCurrentComposer_playing_noSong)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(USBDeviceInfo);
    msg.mutable_usb_device_info_resp()->set_response_id(2);
    msg.mutable_usb_device_info_resp()->set_play_status(0);
    m_VR_TransformProtoToXml->TransformUSBDeviceInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformUSBDeviceInfo_GetCurrentComposer_notPlaying)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(USBDeviceInfo);
    msg.mutable_usb_device_info_resp()->set_response_id(2);
    msg.mutable_usb_device_info_resp()->set_play_status(1);
    m_VR_TransformProtoToXml->TransformUSBDeviceInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformUSBDeviceInfo_GetCurrentGenre_playing_hasSong)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(USBDeviceInfo);
    msg.mutable_usb_device_info_resp()->set_response_id(3);
    msg.mutable_usb_device_info_resp()->set_play_status(0);
    msg.mutable_usb_device_info_resp()->set_song_id("0");
    m_VR_TransformProtoToXml->TransformUSBDeviceInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformUSBDeviceInfo_GetCurrentGenre_playing_noSong)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(USBDeviceInfo);
    msg.mutable_usb_device_info_resp()->set_response_id(3);
    msg.mutable_usb_device_info_resp()->set_play_status(0);
    m_VR_TransformProtoToXml->TransformUSBDeviceInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformUSBDeviceInfo_GetCurrentGenre_notPlaying)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(USBDeviceInfo);
    msg.mutable_usb_device_info_resp()->set_response_id(3);
    msg.mutable_usb_device_info_resp()->set_play_status(1);
    m_VR_TransformProtoToXml->TransformUSBDeviceInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformUSBDeviceInfo_default)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(USBDeviceInfo);
    msg.mutable_usb_device_info_resp()->set_response_id(4);
    m_VR_TransformProtoToXml->TransformUSBDeviceInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, ChangeBand2Str_BandAM)
{
    int nBand = VR_TransformProtoToXml::BandAM;
    m_VR_TransformProtoToXml->ChangeBand2Str(nBand);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, ChangeBand2Str_BandFM)
{
    int nBand = VR_TransformProtoToXml::BandFM;
    m_VR_TransformProtoToXml->ChangeBand2Str(nBand);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, ChangeBand2Str_BandXM)
{
    int nBand = VR_TransformProtoToXml::BandXM;
    m_VR_TransformProtoToXml->ChangeBand2Str(nBand);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, ChangeBand2Str_BandDAB)
{
    int nBand = VR_TransformProtoToXml::BandDAB;
    m_VR_TransformProtoToXml->ChangeBand2Str(nBand);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, ChangeBand2Str_default)
{
    int nBand = -1;
    m_VR_TransformProtoToXml->ChangeBand2Str(nBand);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformRadioBandState_no_radio_band_state)
{
    VrAdditionalInfoRsp msg;
    m_VR_TransformProtoToXml->TransformRadioBandState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformRadioBandState_BandFM)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_radio_band_state_resp()->set_radio_band_kind(1);
    m_VR_TransformProtoToXml->TransformRadioBandState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformRadioBandState_BandAM)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_radio_band_state_resp()->set_radio_band_kind(0);
    m_VR_TransformProtoToXml->TransformRadioBandState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformRadioBandState_BandXM)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_radio_band_state_resp()->set_radio_band_kind(2);
    m_VR_TransformProtoToXml->TransformRadioBandState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformCurFMChannelState_no_cur_fm_channel_state)
{
    VrAdditionalInfoRsp msg;
    m_VR_TransformProtoToXml->TransformCurFMChannelState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformCurFMChannelState_has_cur_fm_channel_state_true)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_cur_fm_channel_state_resp()->set_multicast_channel_state(0);
    m_VR_TransformProtoToXml->TransformCurFMChannelState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformCurFMChannelState_has_cur_fm_channel_state_false)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_cur_fm_channel_state_resp()->set_multicast_channel_state(1);
    m_VR_TransformProtoToXml->TransformCurFMChannelState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformSpecifiedFMState_no_specified_fm_state)
{
    VrAdditionalInfoRsp msg;
    m_VR_TransformProtoToXml->TransformSpecifiedFMState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformSpecifiedFMState_has_specified_fm_state_true)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_specified_fm_state_resp()->set_radio_band_state(0);
    m_VR_TransformProtoToXml->TransformSpecifiedFMState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformSpecifiedFMState_has_specified_fm_state_false)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_specified_fm_state_resp()->set_radio_band_state(1);
    m_VR_TransformProtoToXml->TransformSpecifiedFMState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformPresetState_no_preset_state)
{
    VrAdditionalInfoRsp msg;
    m_VR_TransformProtoToXml->TransformPresetState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformPresetState_has_preset_state_true)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_preset_state_resp()->set_preset_state(0);
    m_VR_TransformProtoToXml->TransformPresetState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformPresetState_has_preset_state_false)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_preset_state_resp()->set_preset_state(1);
    m_VR_TransformProtoToXml->TransformPresetState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformSpecifiedPresetState_no_specified_preset_state)
{
    VrAdditionalInfoRsp msg;
    m_VR_TransformProtoToXml->TransformSpecifiedPresetState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformSpecifiedPresetState_has_specified_preset_state_true)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_specified_preset_state_resp()->set_preset_num_state(0);
    m_VR_TransformProtoToXml->TransformSpecifiedPresetState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformSpecifiedPresetState_has_specified_preset_state_false)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_specified_preset_state_resp()->set_preset_num_state(1);
    m_VR_TransformProtoToXml->TransformSpecifiedPresetState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformRadioState_no_specified_preset_state)
{
    VrAdditionalInfoRsp msg;
    m_VR_TransformProtoToXml->TransformRadioState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformRadioState_BandFM)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_radio_state_resp()->set_radio_band_kind(1);
    m_VR_TransformProtoToXml->TransformRadioState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformRadioState_BandAM)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_radio_state_resp()->set_radio_band_kind(0);
    m_VR_TransformProtoToXml->TransformRadioState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformRadioState_BandXM)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_radio_state_resp()->set_radio_band_kind(2);
    m_VR_TransformProtoToXml->TransformRadioState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformGenreStationState_no_genre_station_state)
{
    VrAdditionalInfoRsp msg;
    m_VR_TransformProtoToXml->TransformGenreStationState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformGenreStationState_has_genre_station_state_true)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_genre_station_state_resp()->set_genre_state(0);
    m_VR_TransformProtoToXml->TransformGenreStationState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformGenreStationState_has_genre_station_state_false)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_genre_station_state_resp()->set_genre_state(1);
    m_VR_TransformProtoToXml->TransformGenreStationState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformRadioActiveBandState_no_radio_active_band)
{
    VrActiveInfoRsp msg;
    m_VR_TransformProtoToXml->TransformRadioActiveBandState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformRadioActiveBandState_has_radio_active_band)
{
    VrActiveInfoRsp msg;
    msg.mutable_radio_active_band_resp();
    m_VR_TransformProtoToXml->TransformRadioActiveBandState(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransfromQueryStatus_MapEnd)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(RadioState);
    m_VR_TransformProtoToXml->TransfromQueryStatus(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransfromQueryStatus_AirConditionState)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(AirConditionState);
    m_VR_TransformProtoToXml->TransfromQueryStatus(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransfromQueryStatus_ACState)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(ACState);
    m_VR_TransformProtoToXml->TransfromQueryStatus(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransfromQueryStatus_DualMode)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(DualMode);
    m_VR_TransformProtoToXml->TransfromQueryStatus(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransfromQueryStatus_WiperDeicer)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(WiperDeicer);
    m_VR_TransformProtoToXml->TransfromQueryStatus(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransfromQueryStatus_RearDefogger)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(RearDefogger);
    m_VR_TransformProtoToXml->TransfromQueryStatus(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransfromQueryStatus_ClimateConciergeMode)
{
    VrAdditionalInfoRsp msg;
    msg.set_response_id(ClimateConciergeMode);
    m_VR_TransformProtoToXml->TransfromQueryStatus(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransfromTemperature_NoTemperature)
{
    VrAdditionalInfoRsp msg;
    m_VR_TransformProtoToXml->TransfromTemperature(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransfromTemperature_CEL_STEP_START)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_temperature_resp()->set_temperature_state(1);
    m_VR_TransformProtoToXml->TransfromTemperature(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransfromTemperature_FAH_STEP_START)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_temperature_resp()->set_temperature_state(0x24);
    m_VR_TransformProtoToXml->TransfromTemperature(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransfromTemperature_MIN)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_temperature_resp()->set_temperature_state(0);
    m_VR_TransformProtoToXml->TransfromTemperature(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransfromTemperature_MAX)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_temperature_resp()->set_temperature_state(0x3e);
    m_VR_TransformProtoToXml->TransfromTemperature(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransfromTemperature_INVALID)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_temperature_resp()->set_temperature_state(-1.0);
    m_VR_TransformProtoToXml->TransfromTemperature(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransfromTemperatureRang_no_temperature_range)
{
    VrAdditionalInfoRsp msg;
    m_VR_TransformProtoToXml->TransfromTemperatureRang(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransfromTemperatureRang_has_temperature_range)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_temperature_range_resp();
    m_VR_TransformProtoToXml->TransfromTemperatureRang(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransfromFanSpeed_no_fan_speed_state)
{
    VrAdditionalInfoRsp msg;
    m_VR_TransformProtoToXml->TransfromFanSpeed(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransfromFanSpeed_has_fan_speed_state)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_fan_speed_state_resp();
    m_VR_TransformProtoToXml->TransfromFanSpeed(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransfromFanMode_no_blower_mode)
{
    VrAdditionalInfoRsp msg;
    m_VR_TransformProtoToXml->TransfromFanMode(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransfromFanMode_FACE)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_blower_mode_resp()->set_blower_mode_state(0);
    m_VR_TransformProtoToXml->TransfromFanMode(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransfromFanMode_FOOT)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_blower_mode_resp()->set_blower_mode_state(1);
    m_VR_TransformProtoToXml->TransfromFanMode(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransfromFanMode_FACE_FOOT)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_blower_mode_resp()->set_blower_mode_state(2);
    m_VR_TransformProtoToXml->TransfromFanMode(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransfromFanMode_WIND_FOOT)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_blower_mode_resp()->set_blower_mode_state(3);
    m_VR_TransformProtoToXml->TransfromFanMode(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransfromFanMode_error)
{
    VrAdditionalInfoRsp msg;
    msg.mutable_blower_mode_resp()->set_blower_mode_state(4);
    m_VR_TransformProtoToXml->TransfromFanMode(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformGetRenctCallListInfo_no_phone_avaialble)
{
    VrActiveInfoRsp msg;
    m_VR_TransformProtoToXml->TransformGetRenctCallListInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformGetRenctCallListInfo_has_phone_avaialble)
{
    VrActiveInfoRsp msg;
    msg.mutable_recent_call_list_rsp()->set_phone_avaialble(1);
    msg.mutable_recent_call_list_rsp()->add_phone_info()->set_contact_id(1);
    msg.mutable_recent_call_list_rsp()->add_phone_info()->set_first_name("1");
    msg.mutable_recent_call_list_rsp()->add_phone_info()->set_last_name("1");
    msg.mutable_recent_call_list_rsp()->add_phone_info()->set_phone_type(1);
    msg.mutable_recent_call_list_rsp()->add_phone_info()->set_phone_number("123");
    msg.mutable_recent_call_list_rsp()->add_phone_info()->set_phone_kind(1);
    m_VR_TransformProtoToXml->TransformGetRenctCallListInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformGetInComingCallListInfo_no_phone_avaialble)
{
    VrActiveInfoRsp msg;
    m_VR_TransformProtoToXml->TransformGetInComingCallListInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformGetInComingCallListInfo_has_phone_avaialble)
{
    VrActiveInfoRsp msg;
    msg.mutable_incoming_call_list_rsp()->set_phone_avaialble(1);
    msg.mutable_incoming_call_list_rsp()->mutable_phone_info()->set_contact_id(1);
    msg.mutable_incoming_call_list_rsp()->mutable_phone_info()->set_first_name("1");
    msg.mutable_incoming_call_list_rsp()->mutable_phone_info()->set_last_name("1");
    msg.mutable_incoming_call_list_rsp()->mutable_phone_info()->set_phone_type(1);
    msg.mutable_incoming_call_list_rsp()->mutable_phone_info()->set_phone_number("123");
    msg.mutable_incoming_call_list_rsp()->mutable_phone_info()->set_phone_kind(1);
    m_VR_TransformProtoToXml->TransformGetInComingCallListInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformGetOutGoingCallListInfo_no_phone_avaialble)
{
    VrActiveInfoRsp msg;
    m_VR_TransformProtoToXml->TransformGetOutGoingCallListInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformGetOutGoingCallListInfo_has_phone_avaialble)
{
    VrActiveInfoRsp msg;
    msg.mutable_outgoing_call_list_rsp()->set_phone_avaialble(1);
    msg.mutable_outgoing_call_list_rsp()->mutable_phone_info()->set_contact_id(1);
    msg.mutable_outgoing_call_list_rsp()->mutable_phone_info()->set_first_name("1");
    msg.mutable_outgoing_call_list_rsp()->mutable_phone_info()->set_last_name("1");
    msg.mutable_outgoing_call_list_rsp()->mutable_phone_info()->set_phone_type(1);
    msg.mutable_outgoing_call_list_rsp()->mutable_phone_info()->set_phone_number("123");
    msg.mutable_outgoing_call_list_rsp()->mutable_phone_info()->set_phone_kind(1);
    m_VR_TransformProtoToXml->TransformGetOutGoingCallListInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformGetRenctMsgListInfo_no_msg_avaialble)
{
    VrActiveInfoRsp msg;
    m_VR_TransformProtoToXml->TransformGetRenctMsgListInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformGetRenctMsgListInfo_has_msg_avaialble)
{
    VrActiveInfoRsp msg;
    msg.mutable_recent_msg_list_rsp()->set_msg_avaialble(1);
    msg.mutable_recent_msg_list_rsp()->add_message_info()->set_instance_id(1);
    msg.mutable_recent_msg_list_rsp()->add_message_info()->set_message_id("1");
    msg.mutable_recent_msg_list_rsp()->add_message_info()->set_address("1");
    msg.mutable_recent_msg_list_rsp()->add_message_info()->set_phone_type(1);
    msg.mutable_recent_msg_list_rsp()->add_message_info()->set_sender_name("123");
    msg.mutable_recent_msg_list_rsp()->add_message_info()->set_time("1");
    msg.mutable_recent_msg_list_rsp()->add_message_info()->set_status(1);
    msg.mutable_recent_msg_list_rsp()->add_message_info()->set_message_type(1);
    msg.mutable_recent_msg_list_rsp()->add_message_info()->set_attachment(1);
    m_VR_TransformProtoToXml->TransformGetRenctMsgListInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformGetMsgListByTypeInfo_no_msg_avaialble)
{
    VrActiveInfoRsp msg;
    m_VR_TransformProtoToXml->TransformGetMsgListByTypeInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformGetMsgListByTypeInfo_has_msg_avaialble)
{
    VrActiveInfoRsp msg;
    msg.mutable_recent_type_msg_list_rsp()->set_msg_avaialble(1);
    msg.mutable_recent_type_msg_list_rsp()->add_message_info()->set_instance_id(1);
    msg.mutable_recent_type_msg_list_rsp()->add_message_info()->set_message_id("1");
    msg.mutable_recent_type_msg_list_rsp()->add_message_info()->set_address("1");
    msg.mutable_recent_type_msg_list_rsp()->add_message_info()->set_phone_type(1);
    msg.mutable_recent_type_msg_list_rsp()->add_message_info()->set_sender_name("123");
    msg.mutable_recent_type_msg_list_rsp()->add_message_info()->set_time("1");
    msg.mutable_recent_type_msg_list_rsp()->add_message_info()->set_status(1);
    msg.mutable_recent_type_msg_list_rsp()->add_message_info()->set_message_type(1);
    msg.mutable_recent_type_msg_list_rsp()->add_message_info()->set_attachment(1);
    m_VR_TransformProtoToXml->TransformGetMsgListByTypeInfo(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrMetMMStatus_Wait)
{
    VrMetMMStatus msg;
    msg.set_status(Wait);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrMetMMStatus_Operate)
{
    VrMetMMStatus msg;
    msg.set_status(Operate);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrMetMMStatus_Receive)
{
    VrMetMMStatus msg;
    msg.set_status(Receive);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrMetMMStatus_Process)
{
    VrMetMMStatus msg;
    msg.set_status(Process);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, Transform_VrMetMMStatus_default)
{
    VrMetMMStatus msg;
    msg.set_status((VrRecognizeStatus)4);
    m_VR_TransformProtoToXml->Transform(msg);
    SUCCEED();
}

TEST_F(VR_TransformProtoToXml_Test, TransformVrRecogResultConfirm)
{
    std::string op("");
    int type = 10000;
    std::string errorCode("0");
    m_VR_TransformProtoToXml->TransformVrRecogResultConfirm(op, type, errorCode);
    SUCCEED();
}

/* EOF */

