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
 * @file  VR_HVACStatusInfoProxy.cpp
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
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "vehicle/NDAirconInfo.pb.h"
#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"

using  namespace  navi::VoiceRecog;
using  namespace  nutshell::vehicle;

/**
 * @brief Get Auto air conditioner State
 */
void VR_VehicleInfoProxy::GetAutoStatus(int nActionId)
{
    VR_LOGI("nActionId=[%d]", nActionId);
    VR_VehicleData data;
    data.m_nActionId = nActionId;
    data.m_pGetCallFunc = &VR_VehicleInfoProxy::GetAutoStatus;
    data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleGetAutoACStatus;
    PushGetData(data);
}

void VR_VehicleInfoProxy::GetAutoStatus()
{
    VR_LOGD_FUNC();
    // VR_HVAC_GETCALLBACK_DEF(vr_get_auto_status);
    VR_HVAC_GETCALLBACK_DEF(vr_get_temp_status);
}

/**
 * @brief  Handle Get Auto AC Status callback
 */
void  VR_VehicleInfoProxy::HandleGetAutoACStatus(bool bSuccess, const nutshell::NCVariant &ncValue)
{
    VR_LOGI("bSuccess=[%d]", bSuccess);
    VR_VehicleData * pData = Front();
    if (NULL == pData) {
        VR_LOGI("Front() == NULL");
        return;
    }
    if (bSuccess) {
        bool bAutoAircon(true);
        std::string  infoTemperature;
        ncValue.value(infoTemperature);
        NDAirconTemperature  airconTemperature;
        airconTemperature.ParseFromString(infoTemperature);
        if (airconTemperature.has_type()) {
            VR_LOGD("airconTemperature type=[%d]", airconTemperature.type());
            switch (airconTemperature.type()) {
            case NDAirconTemperatureType_MaxCold:
                {
                }
                break;
            case NDAirconTemperatureType_MaxHot:
                {
                }
                break;
            case NDAirconTemperatureType_Celsius:
                {
                }
                break;
            case NDAirconTemperatureType_Fahrenheit:
                {
                }
                break;
            case NDAirconTemperatureType_Off:
                {
                    bAutoAircon = false;
                }
                break;
            case NDAirconTemperatureType_Unknown:
                {
                }
                break;
            default:
                break;
            }
        }
        std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
        vrResultAction->set_id(pData->m_nActionId);
        VR_LOGI("----------pData->m_nActionId----------------------------");
        vrResultAction->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_RSP);
        VrAdditionalInfoRsp* pAdditionInfoRsq = vrResultAction->mutable_additional_info_rsp();
        pAdditionInfoRsq->set_response_id(AirConditionState);
        VrAirConditionStateResp * pAutoState = pAdditionInfoRsq->mutable_air_condition_state_resp();
        pAutoState->set_air_condition_state(bAutoAircon);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
        VR_LOGD("Auto AC Status is : [%d]", bAutoAircon);
    }
    else {
        std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
        vrResultAction->set_id(pData->m_nActionId);
        vrResultAction->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_RSP);
        VrAdditionalInfoRsp* pAdditionInfoRsq = vrResultAction->mutable_additional_info_rsp();
        pAdditionInfoRsq->set_response_id(AirConditionState);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
        VR_LOGD("[Auto AC status is error ....]");
    }
    /*
    if (bSuccess) {
        bool bAutoACStatus(false);
        ncValue.value(bAutoACStatus);
        if (bAutoACStatus) {
            VR_LOGI("bAutoACStatus == 1");
        }
        else {
            VR_LOGI("bAutoACStatus == 0");
        }
        std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
        vrResultAction->set_id(pData->m_nActionId);
        VR_LOGI("----------pData->m_nActionId----------------------------");
        vrResultAction->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_RSP);
        VrAdditionalInfoRsp* pAdditionInfoRsq = vrResultAction->mutable_additional_info_rsp();
        pAdditionInfoRsq->set_response_id(AirConditionState);
        VrAirConditionStateResp * pAutoState = pAdditionInfoRsq->mutable_air_condition_state_resp();
        pAutoState->set_air_condition_state(bAutoACStatus);
        // vrResultAction->set_allocated_additional_info_rsp(pAdditionInfoRsq);   // this may case the pointer error...
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
        VR_LOGI("[Auto AC Status is : %d\n]", bAutoACStatus);
    }
    else {
        std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
        vrResultAction->set_id(pData->m_nActionId);
        vrResultAction->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_RSP);
        VrAdditionalInfoRsp* pAdditionInfoRsq = vrResultAction->mutable_additional_info_rsp();
        pAdditionInfoRsq->set_response_id(AirConditionState);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
        VR_LOGI("[Auto AC status is error ....]");
    } */
}

/**
 * @brief Get AC State
 */
void  VR_VehicleInfoProxy::GetACStatus(int nActionId)
{
    VR_LOGI("nActionId=[%d]", nActionId);
    VR_VehicleData data;
    data.m_nActionId = nActionId;
    data.m_pGetCallFunc = &VR_VehicleInfoProxy::GetACStatus;
    data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleGetACStatus;
    PushGetData(data);
}

void  VR_VehicleInfoProxy::GetACStatus()
{
    VR_LOGD_FUNC();
    VR_HVAC_GETCALLBACK_DEF(vr_get_ac_status);
}

/**
 * @brief Handle Get AC Status callback
 */
void VR_VehicleInfoProxy::HandleGetACStatus(bool bSuccess, const nutshell::NCVariant &ncValue)
{
    VR_LOGI("bSuccess=[%d]", bSuccess);
    VR_VehicleData *pData = Front();
    if (NULL == pData) {
        VR_LOGI("Front() == NULL");
        return;
    }
    if (bSuccess) {
        bool bACStatus(false);
        ncValue.value(bACStatus);
        if (bACStatus) {
            VR_LOGI("bACStatus == 1");
        }
        else {
            VR_LOGI("bACStatus == 0");
        }
        std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
        vrResultAction->set_id(pData->m_nActionId);
        VR_LOGI("----------------pData->m_nActionId-------------------------");
        vrResultAction->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_RSP);
        VrAdditionalInfoRsp *pAdditionInfo = vrResultAction->mutable_additional_info_rsp();
        pAdditionInfo->set_response_id(ACState);
        VrACStateResp *pACState = pAdditionInfo->mutable_ac_state_resp();
        pACState->set_ac_state(bACStatus);
        // vrResultAction->set_allocated_additional_info_rsp(pAdditionInfo);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
        VR_LOGI("bACStatus = [%d]", bACStatus);
    }
    else {
        std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
        vrResultAction->set_id(pData->m_nActionId);
        vrResultAction->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_RSP);
        VrAdditionalInfoRsp *pAdditionInfo = vrResultAction->mutable_additional_info_rsp();
        pAdditionInfo->set_response_id(ACState);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
        VR_LOGI("bACStatus is error...");
    }
}

/**
 * @brief Get Temperature State , IncreaseTemperature Request
 */
void VR_VehicleInfoProxy::GetTemperatureStatus(int nActionId)
{
    VR_LOGI("nActionId=[%d]", nActionId);
    VR_VehicleData data;
    data.m_nActionId = nActionId;
    data.m_pGetCallFunc = &VR_VehicleInfoProxy::GetTemperatureStatus;
    data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleGetTemperature;
    PushGetData(data);
}

void VR_VehicleInfoProxy::GetTemperatureStatus()
{
    VR_LOGD_FUNC();
    VR_HVAC_GETCALLBACK_DEF(vr_get_temp_status);
}

/**
 * @brief  Handle Get Temperature Status  Callback
 */
void  VR_VehicleInfoProxy::HandleGetTemperature(bool bSuccess, const nutshell::NCVariant &ncValue)
{
    VR_LOGI("bSuccess=[%d]", bSuccess);
    VR_VehicleData *pData = Front();
    if (NULL == pData) {
        VR_LOGI("Front() == NULL");
        return;
    }
    if (bSuccess) {
        std::string  infoTemperature;
        ncValue.value(infoTemperature);
        NDAirconTemperature  airconTemperature;
        airconTemperature.ParseFromString(infoTemperature);
        int nRsqTemperature(-1);
        float fCurrentTemp(VR_TEMP_INVALID);
        if (airconTemperature.has_type()) {
            VR_LOGI("airconTemperature type=[%d]", airconTemperature.type());
            switch (airconTemperature.type()) {
            case NDAirconTemperatureType_MaxCold:
                {
                    nRsqTemperature = VR_TEMP_MIN;
                }
                break;
            case NDAirconTemperatureType_MaxHot:
                {
                    nRsqTemperature = VR_TEMP_MAX;
                }
                break;
            case NDAirconTemperatureType_Celsius:
                {
                    fCurrentTemp = airconTemperature.value();
                    nRsqTemperature = VR_CEL_STEP_START + (int)((fCurrentTemp - VR_CELSIUS) * VR_CEL_REVERSE_RATE);
                }
                break;
            case NDAirconTemperatureType_Fahrenheit:
                {
                    fCurrentTemp = airconTemperature.value();
                    nRsqTemperature = VR_FAH_STEP_START + (int)(fCurrentTemp - VR_FAHRENHEIT);
                }
                break;
            case NDAirconTemperatureType_Off:
                {
                    nRsqTemperature = VR_TEMP_OFF;  // deal with spec case.....
                }
                break;
            case NDAirconTemperatureType_Unknown:
                {
                    nRsqTemperature = VR_TEMP_UNKOWN;
                }
                break;
            default:
                break;
            }
        }
        VR_LOGI("fCurrTemp = [%f]", fCurrentTemp);
        std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
        vrResultAction->set_id(pData->m_nActionId);
        VR_LOGI("-------------------pData->m_nActionId-------------------------");
        vrResultAction->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_RSP);
        VrAdditionalInfoRsp *pAdditionInfoRsq = vrResultAction->mutable_additional_info_rsp();
        pAdditionInfoRsq->set_response_id(Temperature);
        VrTemperatureResp *  pTemperatureResq = pAdditionInfoRsq->mutable_temperature_resp();
        pTemperatureResq->set_temperature_state(nRsqTemperature);
        // vrResultAction->set_allocated_additional_info_rsp(pAdditionInfoRsq);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
        VR_LOGI("[the temperature is : %d\n]", fCurrentTemp);
    }
    else {
        std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
        vrResultAction->set_id(pData->m_nActionId);
        vrResultAction->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_RSP);
        VrAdditionalInfoRsp *pAdditionInfoRsq = vrResultAction->mutable_additional_info_rsp();
        pAdditionInfoRsq->set_response_id(Temperature);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
        VR_LOGI("[Temperatue resq is error!\n]");
    }
}

/**
 * @brief Get Tempeature Range
 */
void VR_VehicleInfoProxy::GetTempRangleStatus(int nActionId)
{
    VR_LOGI("nActionId=[%d]", nActionId);
    VR_VehicleData data;
    data.m_nActionId = nActionId;
    data.m_pGetCallFunc = &VR_VehicleInfoProxy::GetTempRangleStatus;
    data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleGetTempRange;
    PushGetData(data);
}

void VR_VehicleInfoProxy::GetTempRangleStatus()
{
    VR_LOGD_FUNC();
    VR_HVAC_GETCALLBACK_DEF(vr_get_temprange);
}

void VR_VehicleInfoProxy::HandleGetTempRange(bool bSuccess, const nutshell::NCVariant &nvalue)
{
    VR_LOGI("bSuccess=[%d]", bSuccess);
    VR_VehicleData * pData = Front();
    if (NULL == pData) {
        VR_LOGI("Front() == NULL");
        return;
    }
    if (bSuccess) {
        int nTempUnit(0);
        // nvalue.value(nTempUnit);
        int nLow(0), nHigh(0);
        std::string  infoTempRange;
        nvalue.value(infoTempRange);
        NDAirconTemperatureRange  airconRange;
        airconRange.ParseFromString(infoTempRange);
        nLow = airconRange.value_lower();
        nHigh = airconRange.value_upper();
        nTempUnit = airconRange.unit();
        VR_LOGI("nTempUnit=[%d], nLow=[%d], nHigh=[%d]", nTempUnit, nLow, nHigh);
        std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
        vrResultAction->set_id(pData->m_nActionId);
        VR_LOGI("----------------------------pData------------------------");
        vrResultAction->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_RSP);
        VrAdditionalInfoRsp *pAdditionInfoRsq = vrResultAction->mutable_additional_info_rsp();
        pAdditionInfoRsq->set_response_id(TemperatureRange);
        VrTemperatureRangeResp * pTempRang = pAdditionInfoRsq->mutable_temperature_range_resp();
        pTempRang->set_min_temperature(nLow);
        pTempRang->set_max_temperature(nHigh);
        // vrResultAction->set_allocated_additional_info_rsp(pAdditionInfoRsq);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
        VR_LOGI("[the temperature range is : (%d, %d)\n]", nLow, nHigh);
    }
    else {
        std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
        vrResultAction->set_id(pData->m_nActionId);
        vrResultAction->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_RSP);
        VrAdditionalInfoRsp *pAdditionInfoRsq = vrResultAction->mutable_additional_info_rsp();
        pAdditionInfoRsq->set_response_id(TemperatureRange);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
        VR_LOGI("[Temperatue Range resq is error!\n]");
    }
}

/**
 * @brief Get Fan speed
 */
void VR_VehicleInfoProxy::GetFanSpeedStatus(int nActionId)
{
    VR_LOGI("nActionId=[%d]", nActionId);
    VR_VehicleData data;
    data.m_nActionId = nActionId;
    data.m_pGetCallFunc = &VR_VehicleInfoProxy::GetFanSpeedStatus;
    data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleGetFanSpeed;
    PushGetData(data);
}

void VR_VehicleInfoProxy::GetFanSpeedStatus()
{
    VR_LOGD_FUNC();
    VR_HVAC_GETCALLBACK_DEF(vr_get_blower_level);
}

/**
 * @brief  Handle Get Fan Speed Status Callback
 */
void VR_VehicleInfoProxy::HandleGetFanSpeed(bool bSuccess, const nutshell::NCVariant &ncValue)
{
    VR_LOGI("bSuccess=[%d]", bSuccess);
    VR_VehicleData *pData = Front();
    if (NULL == pData) {
        VR_LOGI("Front() == NULL");
        return;
    }
    if (bSuccess) {
        int nValue(-1);
        ncValue.value(nValue);
        switch (nValue) {
        VR_HVAC_SPEED_CASE(0x00, nValue, VR_FAN_SPEED_MIN);
        VR_HVAC_SPEED_CASE(0x01, nValue, VR_FAN_SPEED_1);
        VR_HVAC_SPEED_CASE(0x02, nValue, VR_FAN_SPEED_2);
        VR_HVAC_SPEED_CASE(0x03, nValue, VR_FAN_SPEED_3);
        VR_HVAC_SPEED_CASE(0x04, nValue, VR_FAN_SPEED_4);
        VR_HVAC_SPEED_CASE(0x05, nValue, VR_FAN_SPEED_5);
        VR_HVAC_SPEED_CASE(0x06, nValue, VR_FAN_SPEED_6);
        VR_HVAC_SPEED_CASE(0x07, nValue, VR_FAN_SPEED_7);
        VR_HVAC_SPEED_CASE(0x0f, nValue, VR_FAN_SPEED_MAX);
        default:
            break;
        }
        VR_LOGI("nValue = [%d]", nValue);
        std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
        vrResultAction->set_id(pData->m_nActionId);
        VR_LOGI("-------------------pData->m_nActionId------------------------");
        vrResultAction->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_RSP);
        VrAdditionalInfoRsp* pAdditionInfoRsq = vrResultAction->mutable_additional_info_rsp();
        pAdditionInfoRsq->set_response_id(FanSpeed);
        VrFanSpeedStateResp *pFanSpeedRsp = pAdditionInfoRsq->mutable_fan_speed_state_resp();
        pFanSpeedRsp->set_fan_speed_state(nValue);
        // vrResultAction->set_allocated_additional_info_rsp(pAdditionInfoRsq);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
        VR_LOGI("[the fan speed is : %d\n]", nValue);
    }
    else {
        std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
        vrResultAction->set_id(pData->m_nActionId);
        vrResultAction->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_RSP);
        VrAdditionalInfoRsp* pAdditionInfoRsq = vrResultAction->mutable_additional_info_rsp();
        pAdditionInfoRsq->set_response_id(FanSpeed);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
        VR_LOGI("[fan speed state resq is error!\n]");
    }
}

/**
 * @brief  Get DualModel State
 */
void VR_VehicleInfoProxy::GetDaulModeStatus(int nActionId)
{
    VR_LOGI("nActionId=[%d]", nActionId);
    VR_VehicleData data;
    data.m_nActionId = nActionId;
    data.m_pGetCallFunc = &VR_VehicleInfoProxy::GetDaulModeStatus;
    data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleGetDModeStatus;
    PushGetData(data);
}

void VR_VehicleInfoProxy::GetDaulModeStatus()
{
    VR_LOGD_FUNC();
    VR_HVAC_GETCALLBACK_DEF(vr_get_dualsync_status);
}

/**
 * @brief  Handle Get DaulMode Status Callback
 */
void VR_VehicleInfoProxy::HandleGetDModeStatus(bool bSuccess, const nutshell::NCVariant &ncValue)
{
    VR_LOGI("bSuccess=[%d]", bSuccess);
    VR_VehicleData *pData = Front();
    if (NULL == pData) {
        VR_LOGI("Front() == NULL");
        return;
    }
    if (bSuccess) {
        bool bDMState(false);
        ncValue.value(bDMState);
        if (bDMState) {
            VR_LOGI("bDMStatus == 1");
        }
        else {
            VR_LOGI("bDMStatus == 0");
        }
        std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
        vrResultAction->set_id(pData->m_nActionId);
        VR_LOGI("------------------pData->m_nActionId-------------------");
        vrResultAction->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_RSP);
        VrAdditionalInfoRsp *pAdditionInfo = vrResultAction->mutable_additional_info_rsp();
        pAdditionInfo->set_response_id(DualMode);
        VrDualModeResp *pDualModeRsp = pAdditionInfo->mutable_dual_mode_resp();
        pDualModeRsp->set_dual_mode_state(bDMState);
        // vrResultAction->set_allocated_additional_info_rsp(pAdditionInfo);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
        VR_LOGI("dual mode is : [%d]", bDMState);
    }
    else {
        std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
        vrResultAction->set_id(pData->m_nActionId);
        vrResultAction->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_RSP);
        VrAdditionalInfoRsp *pAdditionInfo = vrResultAction->mutable_additional_info_rsp();
        pAdditionInfo->set_response_id(DualMode);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
        VR_LOGI("daul mode is error... ");
    }
}

/**
 * @brief Get WiperDeicer State
 */
void VR_VehicleInfoProxy::GetWiperDeicerStatus(int nActionId)
{
    VR_LOGI("nActionId=[%d]", nActionId);
    VR_VehicleData data;
    data.m_nActionId = nActionId;
    data.m_pGetCallFunc = &VR_VehicleInfoProxy::GetWiperDeicerStatus;
    data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleGetWiperDeicerStatus;
    PushGetData(data);
}

void VR_VehicleInfoProxy::GetWiperDeicerStatus()
{
    VR_LOGD_FUNC();
    VR_HVAC_GETCALLBACK_DEF(vr_get_wiperdeicer_status);
}

/**
 * @brief Handle GetWiperDeicer Status Callback
 */
void VR_VehicleInfoProxy::HandleGetWiperDeicerStatus(bool bSuccess, const nutshell::NCVariant& ncValue)
{
    VR_LOGI("bSuccess=[%d]", bSuccess);
    VR_VehicleData *pData = Front();
    if (NULL == pData) {
        VR_LOGI("Front() == NULL");
        return;
    }
    if (bSuccess) {
        bool bWiperDState(false);
        ncValue.value(bWiperDState);
        if (bWiperDState) {
            VR_LOGI("bWiperDState == 1");
        }
        else {
            VR_LOGI("bWiperDState == 0");
        }
        std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
        vrResultAction->set_id(pData->m_nActionId);
        VR_LOGI("------------------pData->m_nActionId-------------------");
        vrResultAction->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_RSP);
        VrAdditionalInfoRsp *pAdditionInfo = vrResultAction->mutable_additional_info_rsp();
        pAdditionInfo->set_response_id(WiperDeicer);
        VrWiperDeicerResp *pWiperDResp = pAdditionInfo->mutable_wiper_deicer_resp();
        pWiperDResp->set_wiper_deicer_state(bWiperDState);
        // vrResultAction->set_allocated_additional_info_rsp(pAdditionInfo);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
        VR_LOGI("WiperD status is [%d]", bWiperDState);
    }
    else {
        std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
        vrResultAction->set_id(pData->m_nActionId);
        vrResultAction->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_RSP);
        VrAdditionalInfoRsp *pAdditionInfo = vrResultAction->mutable_additional_info_rsp();
        pAdditionInfo->set_response_id(WiperDeicer);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
        VR_LOGI("Wiper Status is error.....");
    }
}

/**
 * @brief Get rear defogger state
 */
void VR_VehicleInfoProxy::GetRearDefoggerStatus(int nActionId)
{
    VR_LOGI("nActionId=[%d]", nActionId);
    VR_VehicleData data;
    data.m_nActionId = nActionId;
    data.m_pGetCallFunc = &VR_VehicleInfoProxy::GetRearDefoggerStatus;
    data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleGetRearDefStatus;
    PushGetData(data);
}

void VR_VehicleInfoProxy::GetRearDefoggerStatus()
{
    VR_LOGD_FUNC();
    VR_HVAC_GETCALLBACK_DEF(vr_get_reardef_status);
}

/**
 * @brief Handel Get Rear Defogger Status Callback
 */
void VR_VehicleInfoProxy::HandleGetRearDefStatus(bool bSuccess, const nutshell::NCVariant &ncValue)
{
    VR_LOGI("bSuccess=[%d]", bSuccess);
    VR_VehicleData *pData = Front();
    if (NULL == pData) {
        VR_LOGI("Front() == NULL");
        return;
    }
    if (bSuccess) {
        bool bRearDef(false);
        ncValue.value(bRearDef);
        if (bRearDef) {
            VR_LOGI("bRearDef == 1");
        }
        else {
            VR_LOGI("bRearDef == 0");
        }
        std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
        vrResultAction->set_id(pData->m_nActionId);
        VR_LOGI("------------------pData->m_nActionId-------------------");
        vrResultAction->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_RSP);
        VrAdditionalInfoRsp *pAdditionInfo = vrResultAction->mutable_additional_info_rsp();
        pAdditionInfo->set_response_id(RearDefogger);
        VrRearDefoggerStateResp *pRearDefResp = pAdditionInfo->mutable_rear_defogger_state_resp();
        pRearDefResp->set_rear_defogger_state(bRearDef);
        // vrResultAction->set_allocated_additional_info_rsp(pAdditionInfo);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
        VR_LOGI("Rear Deaf is: [%d]", bRearDef);
    }
    else {
        std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
        vrResultAction->set_id(pData->m_nActionId);
        vrResultAction->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_RSP);
        VrAdditionalInfoRsp *pAdditionInfo = vrResultAction->mutable_additional_info_rsp();
        pAdditionInfo->set_response_id(RearDefogger);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
        VR_LOGI("Rear Deaf is error.....");
    }
}

/**
 * @brief Get BlowerModel State
 */
void VR_VehicleInfoProxy::GetBlowerModeStatus(int nActionId)
{
    VR_LOGI("nActionId=[%d]", nActionId);
    VR_VehicleData data;
    data.m_nActionId = nActionId;
    data.m_pGetCallFunc = &VR_VehicleInfoProxy::GetBlowerModeStatus;
    data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleGetBlowerModeStatus;
    PushGetData(data);
}

void VR_VehicleInfoProxy::GetBlowerModeStatus()
{
    VR_LOGD_FUNC();
    VR_HVAC_GETCALLBACK_DEF(vr_get_outlet_mode);
}

/**
 * @brief Handle Get Blower Mode Status Callback
 */
void VR_VehicleInfoProxy::HandleGetBlowerModeStatus(bool bSuccess, const nutshell::NCVariant &ncValue)
{
    VR_LOGI("bSuccess=[%d]", bSuccess);
    VR_VehicleData *pData = Front();
    if (NULL == pData) {
        VR_LOGI("Front() == NULL");
        return;
    }
    if (bSuccess) {
        int nValue(-1);
        ncValue.value(nValue);
        switch (nValue) {
        VR_HVAC_BLOWER_CASE(0x01, nValue, face);
        VR_HVAC_BLOWER_CASE(0x02, nValue, face_and_foot);
        VR_HVAC_BLOWER_CASE(0x03, nValue, foot);
        VR_HVAC_BLOWER_CASE(0X04, nValue, foot_and_window);
        default:
            break;
        }
        VR_LOGI("nValue=[%d]", nValue);
        std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
        vrResultAction->set_id(pData->m_nActionId);
        VR_LOGI("------------------pData->m_nActionId-------------------");
        vrResultAction->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_RSP);
        VrAdditionalInfoRsp* pAdditionInfoRsp = vrResultAction->mutable_additional_info_rsp();
        pAdditionInfoRsp->set_response_id(BlowerMode);
        VrBlowerModeResp *pBlowerModeRsp = pAdditionInfoRsp->mutable_blower_mode_resp();
        pBlowerModeRsp->set_blower_mode_state(nValue);
        // vrResultAction->set_allocated_additional_info_rsp(pAdditionInfoRsp);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
        VR_LOGI("Blower mode is [%d]", nValue);
    }
    else {
        std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
        vrResultAction->set_id(pData->m_nActionId);
        vrResultAction->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_RSP);
        VrAdditionalInfoRsp* pAdditionInfoRsp = vrResultAction->mutable_additional_info_rsp();
        pAdditionInfoRsp->set_response_id(BlowerMode);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
        VR_LOGI("Blower mode is error....");
    }
}
/**
 * @brief Get Climate Concierge Mode State
 */
void VR_VehicleInfoProxy::GetClimateConciergeModeStatus(int nActionId)
{
    VR_LOGI("nActionId=[%d]", nActionId);
    VR_VehicleData data;
    data.m_nActionId = nActionId;
    data.m_pGetCallFunc = &VR_VehicleInfoProxy::GetClimateConciergeModeStatus;
    data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleGetClimateConciergeMode;
    PushGetData(data);
}

void VR_VehicleInfoProxy::GetClimateConciergeModeStatus()
{
    VR_LOGD_FUNC();
    VR_HVAC_GETCALLBACK_DEF(vr_get_climateconcierge_status);
}

/**
 * @brief  Handle Get Climate Concierge Mode Status Callback
 */
void VR_VehicleInfoProxy::HandleGetClimateConciergeMode(bool bSuccess, const nutshell::NCVariant &ncValue)
{
    VR_LOGI("bSuccess=[%d]", bSuccess);
    VR_VehicleData *pData = Front();
    if (NULL == pData) {
        VR_LOGI("Front() == NULL");
        return;
    }
    if (bSuccess) {
        bool bCCMode(false);
        ncValue.value(bCCMode);
        if (bCCMode) {
            VR_LOGI("bCCMode == 1");
        }
        else {
            VR_LOGI("bCCMode == 0");
        }
        std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
        vrResultAction->set_id(pData->m_nActionId);
        VR_LOGI("------------------pData->m_nActionId-------------------");
        vrResultAction->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_RSP);
        VrAdditionalInfoRsp *pAdditionInfo = vrResultAction->mutable_additional_info_rsp();
        pAdditionInfo->set_response_id(ClimateConciergeMode);
        VrClimateConciergeModeResp *pCCModeResp = pAdditionInfo->mutable_climate_concierge_mode_resp();
        pCCModeResp->set_climate_concierge_mode(bCCMode);
        // vrResultAction->set_allocated_additional_info_rsp(pAdditionInfo);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
        VR_LOGI("bCCMode is [%d]", bCCMode);
    }
    else {
        std::unique_ptr<VrLoopBack> loopMsg(VrLoopBack::default_instance().New());
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *vrResultAction = loopMsg->mutable_m_fc2dm();
        vrResultAction->set_id(pData->m_nActionId);
        vrResultAction->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_RSP);
        VrAdditionalInfoRsp *pAdditionInfo = vrResultAction->mutable_additional_info_rsp();
        pAdditionInfo->set_response_id(ClimateConciergeMode);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
        VR_LOGI("bCCMode is error....");
    }
}

/* EOF */
