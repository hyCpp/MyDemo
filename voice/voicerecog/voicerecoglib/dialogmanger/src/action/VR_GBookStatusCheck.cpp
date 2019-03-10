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
 * @file VR_ActionTtsStop.cpp
 * @brief Declaration file of class VR_ActionTtsStop.
 *
 * This file includes the declaration of class VR_ActionTtsStop.
 *
 * @attention used for C++ only.
 */

#include "stdafx.h"
#include "VR_GBookStatusCheck.h"
#include "GBook/NDGBookService.pb.h"

using namespace nutshell::telema;

VR_GBookStatusCheck::VR_GBookStatusCheck()
{
    VR_LOGD_FUNC();
}

VR_GBookStatusCheck::~VR_GBookStatusCheck()
{
    VR_LOGD_FUNC();
}

bool  VR_GBookStatusCheck::GetContractFlag()
{
    VR_LOGD_FUNC();

    GBOOK_CONTRACT_STATUS gbStatus =  m_GBookProxy.getContractFlag();
    VR_LOGD("gbStatus = [%d]", gbStatus);
    if (GBOOK_CONTRACT_STATUS_ON == gbStatus) {
        return true;
    }
    return false;
}

bool  VR_GBookStatusCheck::GetServiceFlag()
{
    VR_LOGD_FUNC();

    GBOOK_SERVICEFLAG_TYPE serviceFlagType = GBOOK_SERVICEFLAG_TYPE_RECONGNITION;
    GBOOK_SERVICEFLAG_STATUS  serviceFlagStatus = GBOOK_SERVICEFLAG_STATUS_UNDEFINED;
    m_GBookProxy.getServiceFlag(serviceFlagType, serviceFlagStatus);
    VR_LOGD("serviceFlagStatus = [%d]", serviceFlagStatus);
    if (GBOOK_SERVICEFLAG_STATUS_ON == serviceFlagStatus) {
        return true;
    }
    return false;
}

std::string  VR_GBookStatusCheck::getUrl()
{
    VR_LOGD_FUNC();

    GBOOK_URL_KIND  gbUrl = GBOOK_URL_CENTER_VR;
    nutshell::NCString ncString = m_GBookProxy.getUrl(gbUrl);
    std::string strUrl = ncString.getString();
    VR_LOGD("strUrl = [%s]", strUrl.c_str());
    return  strUrl;
}

VR_GBook_ErrorCode   VR_GBookStatusCheck::GetCommunicationError()
{
    VR_LOGD_FUNC();

    GBK_VR_ONSID  VrOnsid = m_GBookProxy.canStartURLAccess(GBOOK_SERVICEFLAG_TYPE_RECONGNITION);
    VR_GBook_ErrorCode  errorCode(VR_GBOOK_Unkown);

    switch (VrOnsid) {
    case  GBK_VR_ONSID_None :
        {
            errorCode = VR_GBK_None;
            break;
        }
    case GBK_VR_ONSID_GBC_ComLock :
        {
            errorCode = VR_GBK_ComLock;
            break;
        }
    case GBK_VR_ONSID_GBC_ServiceFlag_Off :
        {
            errorCode = VR_GBK_ServiceFlag_Off;
            break;
        }
    case GBK_VR_ONSID_GBC_ServiceFlag_None :
        {
            errorCode = VR_GBK_ServiceFlag_Node;
            break;
        }
    case GBK_VR_ONSID_GBC_TimeData_Incorrect :
        {
            errorCode = VR_GBK_TimeData_InCorrect;
            break;
        }
    case GBK_VR_ONSID_GBC_DCMStatus_Error :
        {
            errorCode = VR_GBK_DCMStatus_Error;
            break;
        }
    case GBK_VR_ONSID_GBC_BTNotSetting :
        {
            errorCode = VR_GBK_BTNotSetting;
            break;
        }
    case GBK_VR_ONSID_GBC_PhoneNotSelect :
        {
            errorCode = VR_GBK_PhoneNotSelect;
            break;
        }
    case GBK_VR_ONSID_GBC_PhoneNotSetting :
        {
            errorCode = VR_GBK_PhoneNotSetting;
            break;
        }
    case GBK_VR_ONSID_GBC_ComDeviceNotSelect :
        {
            errorCode = VR_GBK_ComDeveiceNotSelect;
            break;
        }
    case GBK_VR_ONSID_GBC_ComSettingNotInit :
        {
            errorCode = VR_GBK_ComSettingNotInit;
            break;
        }
    case GBK_VR_ONSID_GIN_WifiSetting :
        {
            errorCode = VR_GBK_WifiSetting;
            break;
        }
    case GBK_VR_ONSID_COM_ConnectConf :
        {
            errorCode = VR_GBK_COM_ConnectConf;
            break;
        }
    case GBK_VR_ONSID_Error :
        {
            errorCode = VR_GBOOK_Errror;
            break;
        }
    default:
        break;
    }
    return errorCode;
}

/* EOF */
