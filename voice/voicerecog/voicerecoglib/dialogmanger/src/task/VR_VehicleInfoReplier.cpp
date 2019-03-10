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
 * @file  VR_VehicleInfoReplier.cpp
 * @brief  Implementation of class VR_VehicleInfoReplier
 * @attention  Use only for C++
 * @author  Yuky, Time: 14/05/2015 Thurs.
 */

#include "VR_Log.h"
#include "VR_VehicleInfoAgent.h"
#include "VR_VehicleInfoReplier.h"

using namespace std;

VR_VehicleInfoReplier::VR_VehicleInfoReplier(VR_VehicleInfoAgent *pInfoProxy)
    : m_pVehicleProxy(pInfoProxy)
{
    VR_LOGD_FUNC();
}

VR_VehicleInfoReplier::~VR_VehicleInfoReplier()
{
    VR_LOGD_FUNC();
}

void VR_VehicleInfoReplier::onConnected()
{
    VR_LOGD("connect success");
    m_pVehicleProxy->SubScribleProperty();
}

void VR_VehicleInfoReplier::onDisconnected()
{
    VR_LOGD_FUNC();
}

void VR_VehicleInfoReplier::onConnectError()
{
    VR_LOGD_FUNC();
}

void VR_VehicleInfoReplier::onSubScribeProperty(bool result, const char *error)
{
    if (!result) {
        VR_LOGD("subscribe error: %s", error);
        return;
    }
    m_pVehicleProxy->QueryListUriState();
    VR_LOGD("subscribe success");
}

void VR_VehicleInfoReplier::onUnsubScribeProperty(bool result, const char *error)
{
    if (!result) {
        VR_LOGD("unSubscribe error: %s", error);
        return;
    }
    VR_LOGD("unSubscribe success");
}

void VR_VehicleInfoReplier::onIsSupportPro(bool result, const char *error, const char *property)
{
    if (!result) {
        VR_LOGD("error: %s, prop: %s", error, property);
        return;
    }

    VR_LOGD("prop: %s", property);
}

void VR_VehicleInfoReplier::onSetPropertyAsyn(bool result,
                                              const char *error,
                                              const char *property)
{
    if (!result) {
        VR_LOGD("error: %s", error);
        return;
    }
    VR_LOGD("set propoerty: %s", property);
}

void VR_VehicleInfoReplier::onGetPropertyAsyn(bool result,
                                              const char *error,
                                              const char *property,
                                              const nutshell::NCVariant &val)
{
    if (!result) {
        VR_LOGD("error: %s. prop:%s", error, property);
        return;
    }
    m_pVehicleProxy->handleData(property, val);
    VR_LOGD("get property: %s", property);
}

void VR_VehicleInfoReplier::onPropertyChanged(const char *property,
                                              const nutshell::NCVariant &val)
{
    m_pVehicleProxy->handleData(property, val);
    VR_LOGD("changed property: %s", property);
}
/* EOF */
