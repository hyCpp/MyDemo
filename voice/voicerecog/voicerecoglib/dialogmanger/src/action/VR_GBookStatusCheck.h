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
 * @file   VR_GBookStatusCheck.h
 * @brief  Declaration class of VR_GBookStatusCheck
 */
#ifndef VR_GBOOKSTATUSCHECK_H
#define VR_GBOOKSTATUSCHECK_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilition (use a .cpp suffix)
#endif

#include "NCGBookProxy.h"

enum  VR_GBook_ErrorCode
{
    VR_GBK_None  =  0,
    VR_GBK_ComLock  = 1,
    VR_GBK_ServiceFlag_Off = 2,
    VR_GBK_ServiceFlag_Node = 3,
    VR_GBK_TimeData_InCorrect = 4,
    VR_GBK_DCMStatus_Error = 5,
    VR_GBK_BTNotSetting  = 6,
    VR_GBK_PhoneNotSelect = 7,
    VR_GBK_PhoneNotSetting = 8,
    VR_GBK_ComDeveiceNotSelect = 9,
    VR_GBK_ComSettingNotInit = 10,
    VR_GBK_WifiSetting   = 11,
    VR_GBK_COM_ConnectConf = 12,
    VR_GBOOK_Errror  = 13,
    VR_GBOOK_Unkown = 88
};

/**
 * @brief The VR_GBookStatusCheck class
 *
 * class declaration
 */
class VR_GBookStatusCheck
{
public:
    VR_GBookStatusCheck();
    virtual ~VR_GBookStatusCheck();

    bool     GetContractFlag();
    bool     GetServiceFlag();
    std::string  getUrl();
    VR_GBook_ErrorCode  GetCommunicationError();

private:
    nutshell::telema::NCGBookProxy    m_GBookProxy;
};

#endif // VR_GBOOKSTATUSCHECK_H
/* EOF */
