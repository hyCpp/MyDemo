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
 * @file  VR_HVACShowScreenProxy.cpp
 * @brief Implementation of class VR_VehicleInfoProxy
 */

#include "stdafx.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "VR_VehicleInfoProxy.h"
#include "VR_VehicleMacrodef.h"
#include "VR_VehicleInfoApi.h"

#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"


using  namespace  navi::VoiceRecog;

/**
 * @brief Show HVAC Screen
 */
void VR_VehicleInfoProxy::ShowHVACScreen(int nActionId, int nShowMode)
{
    VR_LOGI("nActionId=[%d],nShowMode=[%d]", nActionId, nShowMode);
    VR_VehicleData data;
    data.m_nActionId = nActionId;
    data.m_bVrCommand = true;
    data.m_pStrCommand = VR_new std::string(vr_set_screenchange);
    data.m_pSetCallFunc = &VR_VehicleInfoProxy::ShowHVACScreen;
    switch (nShowMode) {
    VR_HVAC_SEND_FUNC_CASE(front_screen, HandleShowFrontScreen);
    VR_HVAC_SEND_FUNC_CASE(rear_screen, HandleShowRearScreen);
    VR_HVAC_SEND_FUNC_CASE(steering_screen, HandleShowSteeringScreen);
    VR_HVAC_SEND_FUNC_CASE(front_seat_heater_screen, HandleShowFrontSeatScreen);
    VR_HVAC_SEND_FUNC_CASE(front_seat_blower_screen, HandleFrontSeatVentilationScreen);
    VR_HVAC_SEND_FUNC_CASE(rear_seat_heater_screen, HandleShowRearSeatScreen);
    VR_HVAC_SEND_FUNC_CASE(rear_seat_blower_screen, HandleRearSeatVentilationScreen);
    VR_HVAC_SEND_FUNC_CASE(consierge_screen, HandleShowLexusConciergeScreen);
    VR_HVAC_SEND_FUNC_CASE(seat_operation_screen, HandleShowSeatOperationScreen);
    default:
        break;
    }
    data.m_pncValue = VR_new nutshell::NCVariant(nShowMode);
    PushSetData(data);
}

void VR_VehicleInfoProxy::ShowHVACScreen(const nutshell::NCVariant &value)
{
    VR_LOGD_FUNC();
    VR_HVAC_SETCALLBACK_DEF(vr_set_screenchange, value);
}

void VR_VehicleInfoProxy::HandleShowFrontScreen(bool bSuccess, const nutshell::NCVariant &)
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
    vrResultConfirm->set_action_id(HVACFrontScreen);
    vrResultConfirm->set_processing_result(bSuccess);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleShowSteeringScreen(bool bSuccess, const nutshell::NCVariant &)
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
    vrResultConfirm->set_action_id(HVACSteeringScreen);
    vrResultConfirm->set_processing_result(bSuccess);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleShowRearScreen(bool bSuccess, const nutshell::NCVariant&)
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
    vrResultConfirm->set_action_id(HVACRearScreen);
    vrResultConfirm->set_processing_result(bSuccess);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleShowFrontSeatScreen(bool bSuccess, const nutshell::NCVariant&)
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
    vrResultConfirm->set_action_id(HVACFrontSeatScreen);
    vrResultConfirm->set_processing_result(bSuccess);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleShowRearSeatScreen(bool bSuccess, const nutshell::NCVariant&)
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
    vrResultConfirm->set_action_id(HVACRearSeatScreen);
    vrResultConfirm->set_processing_result(bSuccess);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleShowLexusConciergeScreen(bool bSuccess, const nutshell::NCVariant&)
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
    vrResultConfirm->set_action_id(HVACLexusConciergeScreen);
    vrResultConfirm->set_processing_result(bSuccess);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleShowSeatOperationScreen(bool bSuccess, const nutshell::NCVariant&)
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
    vrResultConfirm->set_action_id(HVACSeatOperationScreen);
    vrResultConfirm->set_processing_result(bSuccess);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleFrontSeatVentilationScreen(bool bSuccess, const nutshell::NCVariant&)
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
    vrResultConfirm->set_action_id(FrontSeatVentilationScreen);
    vrResultConfirm->set_processing_result(bSuccess);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleRearSeatVentilationScreen(bool bSuccess, const nutshell::NCVariant&)
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
    vrResultConfirm->set_action_id(RearSeatVentilationScreen);
    vrResultConfirm->set_processing_result(bSuccess);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

/* EOF */
