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
* @file  VR_DictationStatusCheck.cpp
* @brief Declaration file of class VR_DictationStatusCheck.
*
* This file includes the declaration of class VR_DictationStatusCheck.
*
* @attention used for C++ only.
*/

#include "stdafx.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "VR_ConfigureIF.h"
#include "VR_EventSenderIF.h"
#include "VR_ContentProvider.h"
#include "system/NCEnvironment.h"
#include "VR_DictationStatusCheck.h"
#include "navi/Voice/VoiceRecog/RequestVrCommonProperty.pb.h"
#include "navi/Voice/VoiceRecog/RespVrCommonPropertyResult.pb.h"

using namespace navi::VoiceRecog;

VR_DictationStatusCheck::VR_DictationStatusCheck(spVR_EventSenderIF eventSender) : m_spVR_EventSender(eventSender)
{
    VR_LOGD_FUNC();
}

VR_DictationStatusCheck::~VR_DictationStatusCheck()
{
    VR_LOGD_FUNC();
}

void  VR_DictationStatusCheck::OnReqestCommonProperty(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    const RequestVrCommonProperty& reqCommonProperty = (const RequestVrCommonProperty&)msg;
    if (!reqCommonProperty.has_dictationstatus()) {
        VR_LOGI("has_dictationstatus = [%d]", reqCommonProperty.has_dictationstatus());
        return;
    }

    std::unique_ptr<RespVrCommonPropertyResult>  replyMsg(RespVrCommonPropertyResult::default_instance().New());
    VrReqDictationStatusResult*  dictationStatus = replyMsg->mutable_dictationstatusresult();
    bool bResult = GetDictationStatus();
    std::string strResult = "false";
    if (bResult) {
        strResult = "true";
    }
    dictationStatus->set_result(strResult);

    m_spVR_EventSender->Reply(context, replyMsg.release());
}

bool  VR_DictationStatusCheck::GetDictationStatus() const
{
    VR_LOGD_FUNC();

    bool bRegion = GetRegionCheck();
    if (!bRegion) {
        return false;
    }
    bool bProduct = GetProductCheck();
    if (!bProduct) {
        return false;
    }
    bool bStatus = GetUnitCheck();
    if (!bStatus) {
        return false;
    }
    bool bDcm = GetDcmCheck();
    if (!bDcm) {
        return false;
    }
    return true;
}

bool  VR_DictationStatusCheck::GetRegionCheck() const
{
    VR_LOGD_FUNC();

    nutshell::NCEnvironment  ncEnvironment;
    nutshell::NCString       ncCountry;
    ncEnvironment.getProductCountry(ncCountry);
    std::string strCountry = ncCountry.getString();
    VR_LOGI("strCountry = [%s]", strCountry.c_str());
    if ("America" == strCountry) {
        return true;
    }
    return false;
}

bool  VR_DictationStatusCheck::GetProductCheck()  const
{
    VR_LOGD_FUNC();

    nutshell::NCEnvironment ncEnvironment;
    nutshell::NCString      ncProduct;
    ncEnvironment.getProductName(ncProduct);
    std::string strProduct = ncProduct.getString();
    VR_LOGI("strProduct = [%s]", strProduct.c_str());
    if ("T2" == strProduct) {
        return true;
    }
    return false;
}

bool  VR_DictationStatusCheck::GetUnitCheck() const
{
    VR_LOGD_FUNC();

    VR_ConfigureIF* configIF = VR_ConfigureIF::Instance();
    if (configIF) {
       bool bStatus = configIF->getStartStatus();
       if (bStatus) {
           return true;
       }
    }
    return false;
}

bool  VR_DictationStatusCheck::GetDcmCheck()  const
{
    VR_LOGD_FUNC();

    VR_ContentProvider vr_cp;
    std::string strResult = vr_cp.GetContentProviderValue(VR_DCM_STATUS);
    VR_LOGI("strResult = [%s]", strResult.c_str());
    if ("connected" == strResult) {
        return true;
    }
    return false;
}
/* EOF */
