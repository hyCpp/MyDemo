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
 * @file VR_VehicleInfoProxy.cpp
 * @brief Implementation of class VR_VehicleInfoProxy
 */


#include "stdafx.h"
#include "VR_VehicleInfoProxy.h"
#include "VR_VehicleInfoApi.h"
#include "VR_ConfigureIF.h"
#include "VR_Common_VrUnitAgent.h"
#include <evip/NEIpcServiceHelper.h>
#include <evip/NEIpcServiceProxy.h>
#include "MEM_list.h"

#define VR_NCVEHICLEINFO_SERVICENAME "nutshell.vehicleinfo"

// #ifndef VR_MUTITHREAD
// #define VR_MUTITHREAD
// #endif
#define VR_ACTION_TIME  1600

VR_VehicleInfoProxy* VR_VehicleInfoProxy::m_pInstance = NULL;

VR_VehicleTimer::VR_VehicleTimer() : BL_Timer(VR_ACTION_TIME, CL_FALSE)
{
    VR_LOGD_FUNC();
}

VR_VehicleTimer::~VR_VehicleTimer()
{
    VR_LOGD_FUNC();
}

VOID VR_VehicleTimer::OnTimer()
{
    VR_LOGI("Enter VR_VehicleTimer");
    VR_VehicleInfoProxy& VehicleProxy = VR_VehicleInfoProxy::CreateInstance();
    VehicleProxy.HandleDataError("time out");
    this->Stop();
}

VR_VehicleInfoProxy::VR_VehicleInfoProxy() : m_spProxy(NULL), m_spReceiver(NULL)
{
    VR_LOGD_FUNC();
    // m_spProxy = VR_new nutshell::NCVehicleInfo;
    // m_spReceiver = VR_new VR_VehicleInfoReply(this);
    m_propertyList.clear();
    m_reqList.clear();
    m_Timer.Start();
    m_Timer.Stop();
    // RegisterReplyReceiver();
}

VR_VehicleInfoProxy::~VR_VehicleInfoProxy()
{
    VR_LOGD_FUNC();
    Clear();
}

VR_VehicleInfoProxy& VR_VehicleInfoProxy::CreateInstance()
{
    VR_LOGD_FUNC();
    if (NULL == m_pInstance) {
        VR_LOGI("NULL == m_pInstance");
        m_pInstance = VR_new VR_VehicleInfoProxy;
    }
    return *m_pInstance;
}

void VR_VehicleInfoProxy::SubScribleProperty()
{
    VR_LOGD_FUNC();

    VR_ConfigureIF* pConfig = VR_ConfigureIF::Instance();
    if (NULL == pConfig) {
        VR_LOGD("**************pConfig is null, system error***************");
        return;
    }

    bool  bConnect = pConfig->getInnerAvcLanConnectStatus();
    if (bConnect) {
#ifdef VR_DM_TARGET_VRAGENT
        m_propertyList.push_back(vr_get_ac_status);
        m_propertyList.push_back(vr_get_dualsync_status);
        m_propertyList.push_back(vr_get_climateconcierge_status);
        m_propertyList.push_back(vr_get_wiperdeicer_status);
        m_propertyList.push_back(vr_get_rearauto_status);
        m_propertyList.push_back(vr_get_reardef_status);
        m_propertyList.push_back(vr_get_temp_status);
        m_propertyList.push_back(vr_get_outlet_mode);
        m_propertyList.push_back(vr_get_blower_level);
        m_propertyList.push_back(vr_get_blower_step);
        m_propertyList.push_back(vr_get_temp_unit);
        m_propertyList.push_back(vr_get_temprange);
        m_propertyList.push_back(vr_set_turn_climate);
        m_propertyList.push_back(vr_set_ac);
        m_propertyList.push_back(vr_set_dualmode);
        m_propertyList.push_back(vr_set_climateconcierge);
        m_propertyList.push_back(vr_set_wiperdeicer);
        m_propertyList.push_back(vr_set_rearaircon);
        m_propertyList.push_back(vr_set_reardefogger);
        m_propertyList.push_back(vr_set_temperature);
        m_propertyList.push_back(vr_set_blower);
        m_propertyList.push_back(vr_set_outlet);
        m_propertyList.push_back(vr_set_screenchange);
        m_propertyList.push_back(vr_set_cmdstatus);
        m_propertyList.push_back(vr_get_seat_screen);

        m_propertyList.push_back(vr_get_front_seat_ven_screen);
        m_propertyList.push_back(vr_get_steering_screen);
        m_propertyList.push_back(vr_get_rear_seat_ven_screen);
        m_propertyList.push_back(vr_get_rear_seat_screen);
        m_propertyList.push_back(vr_get_rear_screen);
        m_propertyList.push_back(vr_get_lexus_con_screen);
        m_propertyList.push_back(vr_get_wiperdeice_sw);
        m_propertyList.push_back(vr_get_rear_aircon_sw);
        m_propertyList.push_back(vr_get_climateconcierg_sw);
        m_propertyList.push_back(vr_get_signal_status);
        m_propertyList.push_back(vr_get_carsensor_ig);
        m_propertyList.push_back(vr_get_vr_version);
        m_propertyList.push_back(vr_get_dual_status);
#endif
#ifdef  VR_DM_TARGET_VRUNIT
        m_propertyList.push_back(vr_get_telephone_switch);
        m_propertyList.push_back(vr_get_china_hybird);
#endif
    }
    else {
        m_propertyList.push_back(vr_get_ac_status);
        m_propertyList.push_back(vr_get_dualsync_status);
        m_propertyList.push_back(vr_get_climateconcierge_status);
        m_propertyList.push_back(vr_get_wiperdeicer_status);
        m_propertyList.push_back(vr_get_rearauto_status);
        m_propertyList.push_back(vr_get_reardef_status);
        m_propertyList.push_back(vr_get_temp_status);
        m_propertyList.push_back(vr_get_outlet_mode);
        m_propertyList.push_back(vr_get_blower_level);
        m_propertyList.push_back(vr_get_blower_step);
        m_propertyList.push_back(vr_get_temp_unit);
        m_propertyList.push_back(vr_get_temprange);
        m_propertyList.push_back(vr_set_turn_climate);
        m_propertyList.push_back(vr_set_ac);
        m_propertyList.push_back(vr_set_dualmode);
        m_propertyList.push_back(vr_set_climateconcierge);
        m_propertyList.push_back(vr_set_wiperdeicer);
        m_propertyList.push_back(vr_set_rearaircon);
        m_propertyList.push_back(vr_set_reardefogger);
        m_propertyList.push_back(vr_set_temperature);
        m_propertyList.push_back(vr_set_blower);
        m_propertyList.push_back(vr_set_outlet);
        m_propertyList.push_back(vr_set_screenchange);
        m_propertyList.push_back(vr_set_cmdstatus);
        m_propertyList.push_back(vr_get_seat_screen);

        m_propertyList.push_back(vr_get_front_seat_ven_screen);
        m_propertyList.push_back(vr_get_steering_screen);
        m_propertyList.push_back(vr_get_rear_seat_ven_screen);
        m_propertyList.push_back(vr_get_rear_seat_screen);
        m_propertyList.push_back(vr_get_rear_screen);
        m_propertyList.push_back(vr_get_lexus_con_screen);
        m_propertyList.push_back(vr_get_wiperdeice_sw);
        m_propertyList.push_back(vr_get_rear_aircon_sw);
        m_propertyList.push_back(vr_get_climateconcierg_sw);
        m_propertyList.push_back(vr_get_signal_status);
        m_propertyList.push_back(vr_get_carsensor_ig);
        m_propertyList.push_back(vr_get_vr_version);
        m_propertyList.push_back(vr_get_dual_status);
    }

    if (m_spProxy->subscribeProperty(m_propertyList)) {
        VR_LOGI("[%s\n]", "VRVehicleInfoProxy subscrible success. ");
    }

    if (bConnect) {
#ifdef VR_DM_TARGET_VRAGENT
        GetPropertyAsync(vr_get_blower_step.c_str());
        GetPropertyAsync(vr_get_temprange.c_str());
        GetPropertyAsync(vr_get_carsensor_ig.c_str());
        GetPropertyAsync(vr_get_seat_screen.c_str());
#endif
#ifdef VR_DM_TARGET_VRUNIT
        GetPropertyAsync(vr_get_telephone_switch.c_str());
        GetPropertyAsync(vr_get_china_hybird.c_str());
#endif
    }
    else {
        GetPropertyAsync(vr_get_blower_step.c_str());
        GetPropertyAsync(vr_get_temprange.c_str());
        GetPropertyAsync(vr_get_carsensor_ig.c_str());
        GetPropertyAsync(vr_get_seat_screen.c_str());
        GetPropertyAsync(vr_get_telephone_switch.c_str());
    }
}

void VR_VehicleInfoProxy::RegisterReplyReceiver()
{
    nutshell::ncsp<nutshell::NEIpcServiceProxy>::sp ipc =
            nutshell::NEIpcServiceHelper::findService(VR_NCVEHICLEINFO_SERVICENAME, 0);

    if (NULL == ipc.get()) {
        VR_LOGI("[%s\n] ipc is NULL .", VR_NCVEHICLEINFO_SERVICENAME);
        return;
    }

    if (NULL == m_spProxy.get() || NULL == m_spReceiver.get()) {
        VR_LOGI("[%s\n]", "VRVehicleInfo proxy is null, or , VRVehicleReply is null.");
        return;
    }

    m_spProxy->setReplyReceiver(m_spReceiver);

    if (!m_spProxy->bindService(ipc)) {
        VR_LOGI("[%s\n]", "VRVehicleInfo proxy bindService error. ");
        return;
    }
    VR_LOGI("[%s\n]", "VR_VehicleInfoProxy register success!");
}

void VR_VehicleInfoProxy::Clear()
{
    VR_LOGD_FUNC();
    VoiceList<VR_VehicleData*>::iterator it;
    VR_VehicleData * pData(NULL);
    for (it = m_reqList.begin(); it != m_reqList.end();) {
        pData = *it;
        if (NULL != pData) {
            nutshell::NCVariant* pVar = pData->m_pncValue;
            if (NULL != pVar) {
                delete pVar;
                pVar = NULL;
            }
            std::string *pCommand = pData->m_pStrCommand;
            if (NULL != pCommand) {
                delete pCommand;
                pCommand = NULL;
            }
            delete pData;
            pData = NULL;
        }
        it = m_reqList.erase(it);
    }
    m_reqList.clear();
    m_propertyList.clear();
}

void VR_VehicleInfoProxy::Remove()
{
    VR_LOGD_FUNC();
    if (IsReqListEmpty()) {
        VR_LOGI("Remove() is empty.");
        return;
    }

    VR_VehicleData* pitem = m_reqList.front();
//    VehicleData* pitem = m_reqList.VoiceFront();
    if (NULL != pitem) {
        if (pitem->m_pncValue) {
            delete pitem->m_pncValue;
            pitem->m_pncValue = NULL;
        }
        if (NULL != pitem->m_pStrCommand) {
            delete pitem->m_pStrCommand;
            pitem->m_pStrCommand = NULL;
        }
        delete pitem;
        pitem = NULL;
    }
    m_reqList.pop_front();
//    m_reqList.VoicePop();
}

VR_VehicleData* VR_VehicleInfoProxy::Front()
{
    VR_LOGD_FUNC();
    VR_VehicleData * pData(NULL);
    if (IsReqListEmpty()) {
        return NULL;
    }
//    if (m_reqList.IsVoiceEmpty()) {
//        return NULL;
//    }

    pData = m_reqList.front();
//    pData = m_reqList.VoiceFront();
    return pData;
}

bool  VR_VehicleInfoProxy::IsReqListEmpty() const
{
    VR_LOGD_FUNC();
    return m_reqList.empty();
//    return m_reqList.IsVoiceEmpty();
}

bool  VR_VehicleInfoProxy::PushGetData(const VR_VehicleData &data)
{
    VR_LOGD_FUNC();
    VR_VehicleData* pData = VR_new VR_VehicleData;
    if (NULL == pData) {
        return false;
    }

    pData->m_nActionId = data.m_nActionId;
    pData->m_bVrCommand = data.m_bVrCommand;
    pData->m_pStrCommand = data.m_pStrCommand;
    pData->m_pGetCallFunc = data.m_pGetCallFunc;
    pData->m_pSendCallFunc = data.m_pSendCallFunc;

    bool IsReqEmpty = IsReqListEmpty();
    m_reqList.push_back(pData);
//    m_reqList.VoicePush(pData);
    if (IsReqEmpty && NULL != data.m_pGetCallFunc) {
        VR_LOGI("[request list is empty, request is : %s\n]", pData->m_pStrCommand ? pData->m_pStrCommand->c_str() : "no uri");
        (this->*data.m_pGetCallFunc)();
    }
    return true;
}

bool  VR_VehicleInfoProxy::PushSetData(const VR_VehicleData &data)
{
    VR_LOGD_FUNC();
    VR_VehicleData * pData = VR_new VR_VehicleData;
    if (NULL == pData) {
        return false;
    }

    pData->m_nActionId = data.m_nActionId;
    pData->m_bVrCommand = data.m_bVrCommand;
    pData->m_pStrCommand = data.m_pStrCommand;
    pData->m_pncValue = data.m_pncValue;
    pData->m_pSetCallFunc = data.m_pSetCallFunc;
    pData->m_pSendCallFunc = data.m_pSendCallFunc;

    bool IsReqEmpty = IsReqListEmpty();
    m_reqList.push_back(pData);
//    m_reqList.VoicePush(pData);
    if (IsReqEmpty && NULL != data.m_pSetCallFunc) {
        VR_LOGI("[request is empty, request is: %s\n]", pData->m_pStrCommand ? pData->m_pStrCommand->c_str() : "no url");
        (this->*data.m_pSetCallFunc)(*data.m_pncValue);
    }
    return true;
}

bool VR_VehicleInfoProxy::SetPropertyAsync(const nutshell::NCCHAR *property, const nutshell::NCVariant &var)
{
    VR_LOGD_FUNC();
    m_Timer.Restart();
    VR_LOGI("timer is restart now....");
    bool bRet = m_spProxy->setPropertyAsyn(property, var);
    if (!bRet) {
        VR_LOGI("m_spProxy->setPropertyAsyn() return false, go to handleError!");
        HandleDataError("set error");
        m_Timer.Stop();
        VR_LOGI("timer is stop... setpropertyasyn return false...");
    }
    return bRet;
}

bool VR_VehicleInfoProxy::GetPropertyAsync(const nutshell::NCCHAR *property)
{
    VR_LOGI("[GetPropertyAsync: %s\n]", property);
    bool bRet = m_spProxy->getPropertyAsyn(property);
    if (!bRet) {
        HandleDataError("get error");
    }
    return bRet;
}

bool VR_VehicleInfoProxy::GetShowScreenAsync(const nutshell::NCCHAR *perperty)
{
    return m_spProxy->getPropertyAsyn(perperty);
}

bool VR_VehicleInfoProxy::HandleDataError(const nutshell::NCCHAR *error)
{
    VR_LOGD_FUNC();
    VR_VehicleData* pData(NULL);
    pData = Front();
    if (NULL == pData) {
        VR_LOGI("[%s\n]", "HandleDataError front is NULL.");
        return false;
    }

    nutshell::NCString  ncString(error);
    nutshell::NCVariant ncValue(ncString);
    if (NULL != pData->m_pSendCallFunc) {
        (this->*pData->m_pSendCallFunc)(false, ncValue);
    }

    Remove();
    pData = Front();
    if (NULL == pData) {
        VR_LOGI("[%s\n]", "HandleDataError removed front is NULL.");
        return false;
    }

    if (NULL != pData->m_pGetCallFunc) {
        (this->*pData->m_pGetCallFunc)();
    }
    else if (NULL != pData->m_pSetCallFunc) {
        (this->*pData->m_pSetCallFunc)(*pData->m_pncValue);
    }
    else {

    }
    return true;
}

bool VR_VehicleInfoProxy::HandleData(const nutshell::NCVariant &var)
{
    VR_LOGD_FUNC();
    VR_VehicleData* pData  = Front();
    if (NULL == pData) {
        VR_LOGI("[%s\n]", "HandleData front is empty");
        return false;
    }
    if (NULL != pData->m_pSendCallFunc) {
        (this->*pData->m_pSendCallFunc)(true, var);
    }

    Remove();
    pData = Front();
    if (NULL == pData) {
        VR_LOGI("[%s\n]", "HandleData removed front is empty");
        return false;
    }

    if (NULL != pData->m_pGetCallFunc) {
        (this->*pData->m_pGetCallFunc)();
    }
    else if (NULL != pData->m_pSetCallFunc) {
        (this->*pData->m_pSetCallFunc)(*pData->m_pncValue);
    }
    else {

    }
    return true;
}

VR_VehicleTimer* VR_VehicleInfoProxy::GetVehicleTimer()
{
    VR_LOGD_FUNC();
    return  &m_Timer;
}

/* EOF */
