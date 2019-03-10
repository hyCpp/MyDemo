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

/**
 * @file  VR_HVACTuneFanProxy.cpp
 * @brief Implementation of class VR_VehicleInfoProxy
 */

#include "stdafx.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "VR_VehicleInfoProxy.h"
#include "VR_VehicleInfoApi.h"
#include "VR_VehicleMacrodef.h"

#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"


using  namespace  navi::VoiceRecog;



/**
 * @brief  Change Fan Speed
 */
void VR_VehicleInfoProxy::ChangeCurrentFanSpeed(int nActionId, int nSpeed)
{
    VR_LOGI("nActionId=[%d],nSpeed=[%d]", nActionId, nSpeed);
    VR_VehicleData data;
    data.m_nActionId = nActionId;
    data.m_bVrCommand = true;
    data.m_pStrCommand = VR_new std::string(vr_set_blower);
    data.m_pSetCallFunc = &VR_VehicleInfoProxy::ChangeCurrentFanSpeed;
    switch (nSpeed) {
    VR_HVAC_SEND_FUNC_CASE(fan_min, HandleFanSpeedMin);
    VR_HVAC_SEND_FUNC_CASE(fan_max, HandleFanSpeedMax);
    default:
        {
            data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandChangeFanSpeed;
        }
        break;
    }
    data.m_pncValue = VR_new nutshell::NCVariant(nSpeed);
    PushSetData(data);
}

void VR_VehicleInfoProxy::ChangeCurrentFanSpeed(const nutshell::NCVariant& value)
{
    VR_LOGD_FUNC();
    VR_HVAC_SETCALLBACK_DEF(vr_set_blower, value);
}

/**
 * @brief Change Fan Mode
 */
void VR_VehicleInfoProxy::ChangeFanWindMode(int nActionId, int nWindMode)
{
    VR_LOGI("nActionId=[%d],nWindMode=[%d]", nActionId, nWindMode);
    VR_VehicleData  data;
    data.m_nActionId = nActionId;
    data.m_bVrCommand = true;
    data.m_pStrCommand = VR_new std::string(vr_set_outlet);
    data.m_pSetCallFunc = &VR_VehicleInfoProxy::ChangeFanWindMode;
    switch (nWindMode) {
    VR_HVAC_SEND_FUNC_CASE(face, HandleBMFace);
    VR_HVAC_SEND_FUNC_CASE(foot, HandleBMFoot);
    VR_HVAC_SEND_FUNC_CASE(face_and_foot, HandleFaceAndFoot);
    VR_HVAC_SEND_FUNC_CASE(foot_and_window, HandleWinAndFoot);
    default:
        break;
    }
    data.m_pncValue = VR_new nutshell::NCVariant(nWindMode);
    PushSetData(data);
}

void VR_VehicleInfoProxy::ChangeFanWindMode(const nutshell::NCVariant& value)
{
    VR_LOGD_FUNC();
    VR_HVAC_SETCALLBACK_DEF(vr_set_outlet, value);
}

void VR_VehicleInfoProxy::HandChangeFanSpeed(bool bSuccess, const nutshell::NCVariant&)
{
    VR_LOGI("bSuccess=[%d]", bSuccess);
    VR_VehicleData *pData = Front();
    if (NULL == pData) {
        VR_LOGI("Front() == NULL");
        return;
    }
    std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
    loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
    VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
    vrResultAction->set_id(pData->m_nActionId);
    vrResultAction->set_vr_dm_action_type(ActionType_RECOG_RESULT_CONFIRM);
    VrRecogResultConfirm* vrResultConfirm = vrResultAction->mutable_recog_result_confirm();
    vrResultConfirm->set_action_id(ChangeFanSpeed);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleFanSpeedMax(bool bSuccess, const nutshell::NCVariant &)
{
    VR_LOGI("bSuccess=[%d]", bSuccess);
    VR_VehicleData *pData = Front();
    if (NULL == pData) {
        VR_LOGI("Front() == NULL");
        return;
    }
    std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
    loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
    VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
    vrResultAction->set_id(pData->m_nActionId);
    vrResultAction->set_vr_dm_action_type(ActionType_RECOG_RESULT_CONFIRM);
    VrRecogResultConfirm* vrResultConfirm = vrResultAction->mutable_recog_result_confirm();
    vrResultConfirm->set_action_id(FanSpeedMax);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleFanSpeedMin(bool bSuccess, const nutshell::NCVariant &)
{
    VR_LOGI("bSuccess=[%d]", bSuccess);
    VR_VehicleData *pData = Front();
    if (NULL == pData) {
        VR_LOGI("Front() == NULL");
        return;
    }
    std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
    loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
    VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
    vrResultAction->set_id(pData->m_nActionId);
    vrResultAction->set_vr_dm_action_type(ActionType_RECOG_RESULT_CONFIRM);
    VrRecogResultConfirm* vrResultConfirm = vrResultAction->mutable_recog_result_confirm();
    vrResultConfirm->set_action_id(FanSpeedMin);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleBMFace(bool bSuccess, const nutshell::NCVariant &)
{
    VR_LOGI("bSuccess=[%d]", bSuccess);
    VR_VehicleData *pData = Front();
    if (NULL == pData) {
        VR_LOGI("Front() == NULL");
        return;
    }
    std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
    loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
    VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
    vrResultAction->set_id(pData->m_nActionId);
    vrResultAction->set_vr_dm_action_type(ActionType_RECOG_RESULT_CONFIRM);
    VrRecogResultConfirm* vrResultConfirm = vrResultAction->mutable_recog_result_confirm();
    vrResultConfirm->set_action_id(BlowerModeFace);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleBMFoot(bool bSuccess, const nutshell::NCVariant &)
{
    VR_LOGI("bSuccess=[%d]", bSuccess);
    VR_VehicleData *pData = Front();
    if (NULL == pData) {
        VR_LOGI("Front() == NULL");
        return;
    }
    std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
    loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
    VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
    vrResultAction->set_id(pData->m_nActionId);
    vrResultAction->set_vr_dm_action_type(ActionType_RECOG_RESULT_CONFIRM);
    VrRecogResultConfirm* vrResultConfirm = vrResultAction->mutable_recog_result_confirm();
    vrResultConfirm->set_action_id(BlowerModeFoot);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleFaceAndFoot(bool bSuccess, const nutshell::NCVariant &)
{
    VR_LOGI("bSuccess=[%d]", bSuccess);
    VR_VehicleData *pData = Front();
    if (NULL == pData) {
        VR_LOGI("Front() == NULL");
        return;
    }
    std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
    loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
    VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
    vrResultAction->set_id(pData->m_nActionId);
    vrResultAction->set_vr_dm_action_type(ActionType_RECOG_RESULT_CONFIRM);
    VrRecogResultConfirm* vrResultConfirm = vrResultAction->mutable_recog_result_confirm();
    vrResultConfirm->set_action_id(BlowerModeFaceAndFoot);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleWinAndFoot(bool bSuccess, const nutshell::NCVariant &)
{
    VR_LOGI("bSuccess=[%d]", bSuccess);
    VR_VehicleData *pData = Front();
    if (NULL == pData) {
        VR_LOGI("Front() == NULL");
        return;
    }
    std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
    loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
    VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
    vrResultAction->set_id(pData->m_nActionId);
    vrResultAction->set_vr_dm_action_type(ActionType_RECOG_RESULT_CONFIRM);
    VrRecogResultConfirm* vrResultConfirm = vrResultAction->mutable_recog_result_confirm();
    vrResultConfirm->set_action_id(BlowerModeWindowAndFoot);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}
/* EOF */
