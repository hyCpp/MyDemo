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

/**
 * @file  VR_AvcLanSender_mock.h
 * @brief Mock class for VR_AvcLanSender.
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_AVCLANSENDER_MOCK_H
#define VR_AVCLANSENDER_MOCK_H

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "VR_AvcLanSenderIF.h"
#include "EV_EventContext.h"

/**
*  VR_AvcLanSender_mock
*
* The class is a mock class for VR_AvcLanSender_mock.
*/
class VR_AvcLanSender_mock : public  VR_AvcLanSenderIF
{
public:
    // Mock methods
    //  MOCK_METHOD0(Start, bool());
    MOCK_METHOD0(GetSenderType, VR_AvcLanSenderType());
    MOCK_METHOD3(Add, void(ReqestType type, int nProtoId, int nActionId));
    MOCK_METHOD2(Remove, void(ReqestType type, int nProtoId));
    MOCK_METHOD0(Start, void());
    MOCK_METHOD0(Stop, void());
    MOCK_METHOD0(GetVoiceTagID, int());
    MOCK_METHOD1(SetVoiceTagID, void(int nVoiceTagID));
    MOCK_METHOD0(GetMsgBodyID, int());
    MOCK_METHOD1(SetMsgBodyID, void(int nMsgBodyID));
    MOCK_CONST_METHOD0(GetNotifyFlag, bool());
    MOCK_CONST_METHOD0(GetAvcBindFlag, bool());

    // send vr dictation request proto to avc-lan
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrDictationReq& rMsg));

    // send vr dictation response proto to avc-lan
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrDictationResp& rMsg));

    // send vr dictation result proto to avc-lan
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrDictationResult& rMsg));

    // send vr tts proto to avc-lan
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrTTSCommandReq& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrTTSCommandResp& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrTTSCommandResult& rMsg));

    // send HVACValidFanSpeed proto to avc-lan
    MOCK_METHOD2(sendProto, bool(const navi::VoiceRecog::VrAdditionalInfoReq& rMsg, const int id));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrAdditionalInfoRsp& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrRecogResult& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrRecogResultConfirm& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrTransmitDataToTSLNotify& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrTransmitDataToVRNotify& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrAgentUsbConnected& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrMessageAcquisitionReq& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrMessageAcquisitionResp& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrVoiceTagRegisterReq& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrVoiceTagRegisterResp& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrVoiceTagRegisterResult& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrVroiceTagPlayBackReq& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrVoiceTagPlayBackResp& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrVoiceTagPlayBackResult& rMsg));

    // send vehicleinfo
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrAgentVehicleInfo& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrAgentVehicleInfoReq& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrAgentActivationConfirmReq& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrAgentActivationConfirmResp& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrAgentActivationReq& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrAgentActivationResp& rMsg));

    // meter
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrMetMMStatus& rMsg));

    // unit status
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrUnitStatusReq& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrUnitStatusRsp& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrUnitStatusNotification& rMsg));

    // vr file transport
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrFileUpdateRightAcquisitionRequest& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrFileUpdateRightReleaseRequest& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrFileUpdateRightAcquisitionResponse& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrFileUpdateRightReleaseResponse& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrAgentFileUpdateCompleteNotify& rMsg));

    // send outbox notify
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrAgentOutbox& rMsg));

    // web search engine
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrWebSearchEngineNotify& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrWebSearchEngineReq& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrWebSearchEngineResp& rMsg));

    // ServerVR
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrServerVrStartReq& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrServerVrStartResp& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrServerVrCancelReq& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrServerVrCancelResp& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrServerVrCancelNotify& rMsg));

    // magic string
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrMagicStringReq& rMsg));
    MOCK_METHOD1(sendProto, bool(const navi::VoiceRecog::VrMagicStringResp& rMsg));
};

#endif // VR_AVCLANSENDER_MOCK_H
/* EOF */
