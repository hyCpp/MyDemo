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
 * @file  VR_VehicleInfoReply.cpp
 * @brief  Implementation of class VR_VehicleInfoReply
 * @attention  Use only for C++
 * @author  Yuky, Time: 14/05/2015 Thurs.
 */

#include "stdafx.h"

#include "VR_VehicleInfoReply.h"
#include "VR_VehicleInfoApi.h"
#include "VR_HVACXMLMessageBuilder.h"
#include "vehicle/NDAirconInfo.pb.h"

using namespace std;
using namespace nutshell::vehicle;

VR_VehicleInfoReply::VR_VehicleInfoReply(VR_VehicleInfoProxy *pInfoProxy) : m_pVehicleProxy(pInfoProxy)
{
    VR_LOGD_FUNC();
}

VR_VehicleInfoReply::~VR_VehicleInfoReply()
{
    VR_LOGD_FUNC();
}

VR_VehicleInfoReply::VR_VehicleInfoReply(const VR_VehicleInfoReply &other)
{

}

VR_VehicleInfoReply& VR_VehicleInfoReply::operator =(const VR_VehicleInfoReply& other)
{
    m_pVehicleProxy = other.m_pVehicleProxy;
    return *this;
}

void VR_VehicleInfoReply::onConnected()
{
    VR_LOGI("[%s\n]", "VRVehicleProxy connected success.");
    m_pVehicleProxy->SubScribleProperty();
}

void VR_VehicleInfoReply::onDisconnected()
{
    VR_LOGI("[%s\n]", "VRVehicleProxy disconnected. ");
}

void VR_VehicleInfoReply::onConnectError()
{
    VR_LOGI("[%s\n]", "VRVehicleProxy connectd error. ");
}

void VR_VehicleInfoReply::onGetPropertyAsyn(bool result, const char *error, const char *property, const nutshell::NCVariant &val)
{
    VR_LOGI("[result = %d, property = %s\n]", result, property);
    VR_VehicleData * pData = m_pVehicleProxy->Front();
    // in our case , there are two, one is action case, other is directly request...
    if (NULL == pData) {
        VR_LOGI("NULL == pData");
        if (result) {
            VR_HVACXMLMessageBuilder::GetMessageBuilder().HandleMessageData(property, val);
        }
        return;
    }
    if (result) {
        m_pVehicleProxy->HandleData(val);
    }
    else {
        m_pVehicleProxy->HandleDataError(error);
    }

}

void VR_VehicleInfoReply::onSetPropertyAsyn(bool result, const char *error, const char *property)
{
    VR_LOGI("[onSetPropertyAsyn : result = %d, property = %s\n]", result, property);
    if (!result) {
        VR_LOGI("onSetPropertyAsyn return error, go to handle error !");
        m_pVehicleProxy->GetVehicleTimer()->Stop();
        m_pVehicleProxy->HandleDataError(error);
        VR_LOGI("timer is stop... when onsetpropertyasyn is false...");
    }
}

void VR_VehicleInfoReply::onPropertyChanged(const char *property, const nutshell::NCVariant &val)
{
    VR_LOGD_FUNC();
    VR_LOGI("property = [%s]", property);
    std::string strProperty = property;
    if (vr_set_cmdstatus == strProperty) {
        VR_VehicleData *data = m_pVehicleProxy->Front();
        if (NULL == data) {
            VR_LOGI("NULL == data");
            return;
        }
        if (!data->m_bVrCommand) {
            VR_LOGI("false == data->m_bVrCommand");
            return;
        }
        std::string strInfoSetting;
        val.value(strInfoSetting);
        NDAirconVRSettingStatus vrSettingStatus;
        vrSettingStatus.ParseFromString(strInfoSetting);
        std::string strVrCommand = vrSettingStatus.vrcommand();
        VR_LOGI("strVrCommand = [%s]", strVrCommand.c_str());

        std::string strCommand = data->m_pStrCommand ? *data->m_pStrCommand : "";
        if (strVrCommand != strCommand) {
            VR_LOGI("strCommand = [%s]", strCommand.c_str());
            VR_LOGI("strVrCommand != strCommand");
            return;
        }
        bool bStatus(true);
        bStatus = vrSettingStatus.vrstatus();
        VR_LOGI("bStatus = [%d]", bStatus);
        if (!bStatus) {
            bStatus = !bStatus;
            nutshell::NCVariant resultTemp(bStatus);
            m_pVehicleProxy->GetVehicleTimer()->Stop();
            VR_LOGI("timer is stop... when onpropertychanged , action is done...");
            m_pVehicleProxy->HandleData(resultTemp);
        }
    }
    else {
        VR_HVACXMLMessageBuilder::GetMessageBuilder().HandleMessageData(property, val);
    }
}
/* EOF */
