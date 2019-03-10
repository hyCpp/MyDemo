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
 * @file  VR_HVACFacilitySwitchProxy.cpp
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
 * @brief bAutoStatus is true: Auto air conditioner ON,
 * bAutoStatus is false: Auto air conditioner OFF
 */


void  VR_VehicleInfoProxy::SetAutoStatus(int nActionId, bool bAutoStatus)
{
    VR_LOGI("nActionId=[%d],bAutoStatus=[%d]", nActionId, bAutoStatus);
    VR_VehicleData data;
    data.m_nActionId = nActionId;
    data.m_bVrCommand = true;
    data.m_pStrCommand = VR_new std::string(vr_set_turn_climate);
    data.m_pSetCallFunc = &VR_VehicleInfoProxy::SetAutoStatus;
    if (bAutoStatus) {
        data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleAutoAcOn;
    }
    else {
        data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleAutoAcOff;
    }
    data.m_pncValue = VR_new nutshell::NCVariant(bAutoStatus);
    PushSetData(data);
}

void  VR_VehicleInfoProxy::SetAutoStatus(const nutshell::NCVariant& value)
{
    VR_LOGD_FUNC();
    VR_HVAC_SETCALLBACK_DEF(vr_set_turn_climate, value);
}

void VR_VehicleInfoProxy::HandleAutoAcOn(bool bSuccess, const nutshell::NCVariant&)
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
    vrResultConfirm->set_action_id(AutoAirConditionerON);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleAutoAcOff(bool bSuccess, const nutshell::NCVariant&)
{
    VR_LOGI("bSuccess=[%d]", bSuccess);
    VR_VehicleData * pData = Front();
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
    vrResultConfirm->set_action_id(AutoAirConditionerOFF);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}
/**
 * @brief bACStatus is true : AC ON
 * bACStatus is false: AC OFF
 */
void  VR_VehicleInfoProxy::SetACStatus(int nActionId, bool bACStatus)
{
    VR_LOGI("nActionId=[%d],bACStatus=[%d]", nActionId, bACStatus);
    VR_VehicleData data;
    data.m_nActionId = nActionId;
    data.m_bVrCommand = true;
    data.m_pStrCommand = VR_new std::string(vr_set_ac);
    data.m_pSetCallFunc = &VR_VehicleInfoProxy::SetACStatus;
    if (bACStatus) {
        data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleAcOn;
    }
    else {
        data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleAcOff;
    }
    data.m_pncValue = VR_new nutshell::NCVariant(bACStatus);
    PushSetData(data);
}

void  VR_VehicleInfoProxy::SetACStatus(const nutshell::NCVariant& value)
{
    VR_LOGD_FUNC();
    VR_HVAC_SETCALLBACK_DEF(vr_set_ac, value);
}

void VR_VehicleInfoProxy::HandleAcOn(bool bSuccess, const nutshell::NCVariant&)
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
    vrResultConfirm->set_action_id(ACON);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleAcOff(bool bSuccess, const nutshell::NCVariant&)
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
    vrResultConfirm->set_action_id(ACOFF);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}
/**
 * @brief bDualStatus is true: DualModel ON
 * bDualStatus is false: DualModle OFF
 */
void VR_VehicleInfoProxy::SetDaulModeStatus(int nActionId, bool bDualStatus)
{
    VR_LOGI("nActionId=[%d],bDualStatus=[%d]", nActionId, bDualStatus);
    VR_VehicleData data;
    data.m_nActionId = nActionId;
    data.m_bVrCommand = true;
    data.m_pStrCommand = VR_new std::string(vr_set_dualmode);
    data.m_pSetCallFunc = &VR_VehicleInfoProxy::SetDaulModeStatus;
    if (bDualStatus) {
        data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleDualModelOn;
    }
    else {
        data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleDualModeOff;
    }
    data.m_pncValue = VR_new nutshell::NCVariant(bDualStatus);
    PushSetData(data);
}

void VR_VehicleInfoProxy::SetDaulModeStatus(const nutshell::NCVariant& value)
{
    VR_LOGD_FUNC();
    VR_HVAC_SETCALLBACK_DEF(vr_set_dualmode, value);
}

void VR_VehicleInfoProxy::HandleDualModelOn(bool bSuccess, const nutshell::NCVariant&)
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
    vrResultConfirm->set_action_id(DualModeON);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleDualModeOff(bool bSuccess, const nutshell::NCVariant&)
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
    vrResultConfirm->set_action_id(DualModeOFF);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}
/**
 * @brief bWDStatus is true: WiperDeicer ON
 * bWDStatus is false: WiperDeicer OFF
 */
void VR_VehicleInfoProxy::SetWiperDeicerStatus(int nActionId, bool bWDStatus)
{
    VR_LOGI("nActionId=[%d],bWDStatus=[%d]", nActionId, bWDStatus);
    VR_VehicleData data;
    data.m_nActionId = nActionId;
    data.m_bVrCommand = true;
    data.m_pStrCommand = VR_new std::string(vr_set_wiperdeicer);
    data.m_pSetCallFunc = &VR_VehicleInfoProxy::SetWiperDeicerStatus;
    if (bWDStatus) {
        data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleWDeicerOn;
    }
    else {
        data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleWDeicerOff;
    }
    data.m_pncValue = VR_new nutshell::NCVariant(bWDStatus);
    PushSetData(data);
}

void VR_VehicleInfoProxy::SetWiperDeicerStatus(const nutshell::NCVariant& value)
{
    VR_LOGD_FUNC();
    VR_HVAC_SETCALLBACK_DEF(vr_set_wiperdeicer, value);
}

void VR_VehicleInfoProxy::HandleWDeicerOn(bool bSuccess, const nutshell::NCVariant&)
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
    vrResultConfirm->set_action_id(WiperDeicerON);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleWDeicerOff(bool bSuccess, const nutshell::NCVariant&)
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
    vrResultConfirm->set_action_id(WiperDeicerOFF);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}
/**
 * @brief bRSAStatus is true: Rear seat air conditioner ON
 * bRSAStatus is false: Rear seat air conditioner OFF
 */
void VR_VehicleInfoProxy::SetRearSeatAutoStatus(int nActionId, bool bRSAStatus)
{
    VR_LOGI("nActionId=[%d],bRSAStatus=[%d]", nActionId, bRSAStatus);
    VR_VehicleData data;
    data.m_nActionId = nActionId;
    data.m_bVrCommand = true;
    data.m_pStrCommand = VR_new std::string(vr_set_rearaircon);
    data.m_pSetCallFunc = &VR_VehicleInfoProxy::SetRearSeatAutoStatus;
    if (bRSAStatus) {
        data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleRearSCOn;
    }
    else {
        data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleRearSCOff;
    }
    data.m_pncValue = VR_new nutshell::NCVariant(bRSAStatus);
    PushSetData(data);
}

void VR_VehicleInfoProxy::SetRearSeatAutoStatus(const nutshell::NCVariant& value)
{
    VR_LOGD_FUNC();
    VR_HVAC_SETCALLBACK_DEF(vr_set_rearaircon, value);
}

void VR_VehicleInfoProxy::HandleRearSCOn(bool bSuccess, const nutshell::NCVariant&)
{
    VR_LOGI("bSuccess=[%d]", bSuccess);
    VR_VehicleData * pData = Front();
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
    vrResultConfirm->set_action_id(RearSeatAirConditionerON);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleRearSCOff(bool bSuccess, const nutshell::NCVariant&)
{
    VR_LOGI("bSuccess=[%d]", bSuccess);
    VR_VehicleData * pData = Front();
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
    vrResultConfirm->set_action_id(RearSeatAirConditionerOFF);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

/**
 * @brief  bRearDefStatus is true: Rear defogger ON
 *  bRearDefStatus is false: Rear defogger OFF
 */
void VR_VehicleInfoProxy::SetRearDefoggerStatus(int nActionId, bool bRearDefStatus)
{
    VR_LOGI("nActionId=[%d],bRearDefStatus=[%d]", nActionId, bRearDefStatus);
    VR_VehicleData data;
    data.m_nActionId = nActionId;
    data.m_bVrCommand = true;
    data.m_pStrCommand = VR_new std::string(vr_set_reardefogger);
    data.m_pSetCallFunc = &VR_VehicleInfoProxy::SetRearDefoggerStatus;
    if (bRearDefStatus) {
        data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleRDefoggerOn;
    }
    else {
        data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleRDefoggerOff;
    }
    data.m_pncValue = VR_new nutshell::NCVariant(bRearDefStatus);
    PushSetData(data);
}

void VR_VehicleInfoProxy::SetRearDefoggerStatus(const nutshell::NCVariant& value)
{
    VR_LOGD_FUNC();
    VR_HVAC_SETCALLBACK_DEF(vr_set_reardefogger, value);
}

void VR_VehicleInfoProxy::HandleRDefoggerOn(bool bSuccess, const nutshell::NCVariant&)
{
    VR_LOGI("bSuccess=[%d]", bSuccess);
    VR_VehicleData * pData = Front();
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
    vrResultConfirm->set_action_id(RearDefoggerON);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleRDefoggerOff(bool bSuccess, const nutshell::NCVariant&)
{
    VR_LOGI("bSuccess=[%d]", bSuccess);
    VR_VehicleData * pData = Front();
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
    vrResultConfirm->set_action_id(RearDefoggerOFF);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

/**
 * @brief  bRearDefStatus is true: Climate concierge mode ON
 *  bRearDefStatus is false: Climate concierge mode OFF
 */
void VR_VehicleInfoProxy::SetClimateConciergeStatus(int nActionId, bool bCConciergeStatus)
{
    VR_LOGI("nActionId=[%d],bCConciergeStatus=[%d]", nActionId, bCConciergeStatus);
    VR_VehicleData data;
    data.m_nActionId = nActionId;
    data.m_bVrCommand = true;
    data.m_pStrCommand = VR_new std::string(vr_set_climateconcierge);
    data.m_pSetCallFunc = &VR_VehicleInfoProxy::SetClimateConciergeStatus;
    if (bCConciergeStatus) {
        data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleCConiergerOn;
    }
    else {
        data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleCConiergerOn;
    }
    data.m_pncValue = VR_new nutshell::NCVariant(bCConciergeStatus);
    PushSetData(data);
}

void VR_VehicleInfoProxy::SetClimateConciergeStatus(const nutshell::NCVariant& value)
{
    VR_LOGD_FUNC();
    VR_HVAC_SETCALLBACK_DEF(vr_set_climateconcierge, value);
}

void VR_VehicleInfoProxy::HandleCConiergerOn(bool bSuccess, const nutshell::NCVariant&)
{
    VR_LOGI("bSuccess=[%d]", bSuccess);
    VR_VehicleData * pData = Front();
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
    vrResultConfirm->set_action_id(ClimateConciergeModeON);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

/* EOF */
