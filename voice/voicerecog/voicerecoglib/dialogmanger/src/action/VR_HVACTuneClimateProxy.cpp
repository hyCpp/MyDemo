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
 * @file  VR_HVACTuneClimateProxy.cpp
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
#include "vehicle/NDAirconInfo.pb.h"
#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"

#include <iostream>

using namespace std;
using  namespace  navi::VoiceRecog;
using  namespace  nutshell::vehicle;

/**
 * @brief  temperature mode: warmer/a few warmer/cooler/a few cooler/temp max/temp min
 */
void VR_VehicleInfoProxy::SetTemperature(int nActionId, Temperature_Mode mode)
{
    VR_LOGI("nActionId=[%d],Temperature_Mode=[%d]", nActionId, mode);
    VR_VehicleData data;
    data.m_nActionId = nActionId;
    data.m_bVrCommand = true;
    data.m_pStrCommand = VR_new std::string(vr_set_temperature);
    data.m_pSetCallFunc = &VR_VehicleInfoProxy::SetTemperature;
    nutshell::NCVariant value;
    std::string  infoTempSetting;
    NDAirconVRTemperatureSetting  airconTempSetting;
    switch (mode) {
    case warmer:
        {
            data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleIncreaseTemp;
            airconTempSetting.set_temperaturemode(NDAirconTemperatureMode_Relative);
            airconTempSetting.set_value_relative(NDAirconTemperatureRelative_Warmer);
            airconTempSetting.SerializeToString(&infoTempSetting);
            value.setValue(infoTempSetting);
        }
        break;
    case a_few_warmer:
        {
            data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleAfewWarmer;
            airconTempSetting.set_temperaturemode(NDAirconTemperatureMode_Relative);
            airconTempSetting.set_value_relative(NDAirconTemperatureRelative_littleWarmer);
            airconTempSetting.SerializeToString(&infoTempSetting);
            value.setValue(infoTempSetting);
        }
        break;
    case cooler:
        {
            data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleDecreaseTemp;
            airconTempSetting.set_temperaturemode(NDAirconTemperatureMode_Relative);
            airconTempSetting.set_value_relative(NDAirconTemperatureRelative_Cooler);
            airconTempSetting.SerializeToString(&infoTempSetting);
            value.setValue(infoTempSetting);
        }
        break;
    case a_few_cooler:
        {
            data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleAfewCooler;
            airconTempSetting.set_temperaturemode(NDAirconTemperatureMode_Relative);
            airconTempSetting.set_value_relative(NDAirconTemperatureRelative_littleCooler);
            airconTempSetting.SerializeToString(&infoTempSetting);
            value.setValue(infoTempSetting);
        }
        break;
    case temperature_max:
        {
            data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleTempMax;
            airconTempSetting.set_temperaturemode(NDAirconTemperatureMode_Max);
            airconTempSetting.SerializeToString(&infoTempSetting);
            value.setValue(infoTempSetting);
        }
        break;
    case temperature_min:
        {
            data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleTempMin;
            airconTempSetting.set_temperaturemode(NDAirconTemperatureMode_Min);
            airconTempSetting.SerializeToString(&infoTempSetting);
            value.setValue(infoTempSetting);
        }
        break;
    default:
        break;
    }
    data.m_pncValue = VR_new nutshell::NCVariant(value);
    PushSetData(data);
}

void VR_VehicleInfoProxy::SetTemperature(const nutshell::NCVariant &value)
{
    VR_HVAC_SETCALLBACK_DEF(vr_set_temperature, value);
}

/**
 * @brief  Set the temperature to [fTempValue]  change temperature
*/
void VR_VehicleInfoProxy::ChangeCurrentTemperature(int nActionId, float fTempValue)
{
    VR_LOGI("nActionId=[%d],fTempValue=[%f]", nActionId, fTempValue);
    VR_VehicleData data;
    data.m_nActionId = nActionId;
    data.m_bVrCommand = true;
    data.m_pStrCommand = VR_new std::string(vr_set_temperature);
    data.m_pSetCallFunc =  &VR_VehicleInfoProxy::ChangeCurrentTemperature;
    data.m_pSendCallFunc = &VR_VehicleInfoProxy::HandleChangeTemp;
    nutshell::NCVariant value;
    std::string infoTempSetting;
    NDAirconVRTemperatureSetting  airconTempSetting;
    airconTempSetting.set_temperaturemode(NDAirconTemperatureMOde_Absolute);
    airconTempSetting.set_value_absolute(fTempValue);
    airconTempSetting.SerializeToString(&infoTempSetting);
    value.setValue(infoTempSetting);
    data.m_pncValue = VR_new nutshell::NCVariant(value);
    PushSetData(data);
}

void VR_VehicleInfoProxy::ChangeCurrentTemperature(const nutshell::NCVariant &value)
{
    VR_LOGD_FUNC();
    VR_HVAC_SETCALLBACK_DEF(vr_set_temperature, value);
}

void VR_VehicleInfoProxy::HandleDecreaseTemp(bool bSuccess, const nutshell::NCVariant&)
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
    vrResultConfirm->set_action_id(DecreaseTemperature);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleIncreaseTemp(bool bSuccess, const nutshell::NCVariant&)
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
    vrResultConfirm->set_action_id(IncreaseTemperature);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleAfewWarmer(bool bSuccess, const nutshell::NCVariant &)
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
    vrResultConfirm->set_action_id(AFewWarmer);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}



void VR_VehicleInfoProxy::HandleAfewCooler(bool bSuccess, const nutshell::NCVariant&)
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
    vrResultConfirm->set_action_id(AFewCooler);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleTempMax(bool bSuccess, const nutshell::NCVariant&)
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
    vrResultConfirm->set_action_id(TemperatureMax);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleTempMin(bool bSuccess, const nutshell::NCVariant &)
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
    vrResultConfirm->set_action_id(TemperatureMin);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}

void VR_VehicleInfoProxy::HandleChangeTemp(bool bSuccess, const nutshell::NCVariant &)
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
    vrResultConfirm->set_action_id(ChangeTemperature);
    vrResultConfirm->set_processing_result(true);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMsg.release());
}
/* EOF */
