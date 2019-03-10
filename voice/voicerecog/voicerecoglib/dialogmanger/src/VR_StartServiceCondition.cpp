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
* @file  VR_StartServiceCondition.cpp
* @brief Declaration file of class VR_StartServiceCondition.
*
* This file includes the declaration of class VR_StartServiceCondition.
*
* @attention used for C++ only.
*/

#include "stdafx.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "VR_EventSenderIF.h"
#include "VR_DialogEngineIF.h"
#include "VR_StartServiceCondition.h"
#include "system/NCEnvironment.h"
#include "VR_ContentProvider.h"
#include "MediaInfoProvider/NMMediaInfoHelper.h"
#include "MediaInfoProvider/NMMediaInfoDEF.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMTaskMessage.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrCommonProperty.pb.h"
#include "navi/Voice/VoiceRecog/RespVrCommonPropertyResult.pb.h"

using namespace navi::VoiceRecog;

// #ifndef VR_START_SERVICE_CONDITION_TEST
// #define VR_START_SERVICE_CONDITION_TEST
// #endif

#define VR_LANG_DICTION_VALID 0
#define VR_LANG_VALID_AND_DICTION_INVALID 1
#define VR_LANG_INVALID_AND_DICTION_VALID 2
#define VR_LANG_DICTION_INVALID           3

Start_Condition_Error VR_StartCondition::CheckConditionData() const
{
    VR_LOGD_FUNC();
    if (!m_bSearchFunc) {
        return Start_SearchFun_Error;
    }
    else if (!m_bDictionData) {
        return Start_DictionData_Error;
    }
    else if (!m_bLangData) {
        return Start_LangData_Error;
    }
    else if (!m_bContactData) {
        return Start_ContactData_Error;
    }
    else if (!m_bMediaVOCSData) {
        return Start_MediaVOCS_Error;
    }
    else if (!m_bMicrophone) {
        return Start_MicroPhone_Error;
    }
    else if (!m_bPTTSW) {
        return Start_PTTSW_Error;
    }
    else if (!m_bHigherInterrupt) {
        return Start_HigherInterput_Error;
    }
    else {
        return Start_Condition_Ok;
    }
}

VR_StartServiceCondition::VR_StartServiceCondition(spVR_DialogEngineIF spEngineIF, spVR_EventSenderIF spSenderIF) :
    m_spEngineIF(spEngineIF),
    m_spEventIF(spSenderIF),
    m_spEventContext(),
    m_vrstart_condition(),
    m_errorCode(Start_Condition_Unkown),
    m_RegionType(North_America_Region),
    m_ProductType(Product_T0)
{
    VR_LOGD_FUNC();
}

VR_StartServiceCondition::~VR_StartServiceCondition()
{
    VR_LOGD_FUNC();
}

void VR_StartServiceCondition::OnReqestCommonProperty(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const RequestVrCommonProperty& reqCommonProperty = (const RequestVrCommonProperty&)msg;
    if (!reqCommonProperty.has_reqservicestatus()) {
        VR_LOGI("has_reqservicestatus : [%d]", reqCommonProperty.has_reqservicestatus());
        return;
    }
    VR_LOGI("RequestVrCommonProperty,has_reqservicestatus:[%d]", reqCommonProperty.has_reqservicestatus());
    if (m_spEventContext.get()) {
        m_spEventContext.reset();
    }
    m_spEventContext = VR_EventSenderIF::CopyEventContext(&context);
    SetInitState();
    Start_Condition_Error errorCode = CheckStartData();
    if (Start_Condition_Ok == errorCode) {
        CheckLangDicData();
    }
    else {
        OnReplyCommonProperty();
    }
}

void VR_StartServiceCondition::OnReplyCommonProperty()
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespVrCommonPropertyResult>  replyMeg(RespVrCommonPropertyResult::default_instance().New());
    VrReqServiceStatusResult *startResult = replyMeg->mutable_reqservicestatusresult();
    VrReqServiceStatusResult_VrServiceStatus serviceStatus = VrReqServiceStatusResult_VrServiceStatus_Unknown;
    switch (m_errorCode) {
    case Start_SearchFun_Error:
        {
            VR_LOGI("Start_SearchFun_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_SearchFuncError;
            break;
        }
    case Start_DictionData_Error:
        {
            VR_LOGI("Start_DictionData_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_DictDataError;
            break;
        }
    case Start_LangData_Error:
        {
            VR_LOGI("Start_LangData_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_LangDataError;
            break;
        }
    case Start_ContactData_Error:
        {
            VR_LOGI("Start_ContactData_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_ContactDataError;
            break;
        }
    case Start_MediaVOCS_Error:
        {
            VR_LOGI("Start_MediaVOCS_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_MediaVOCSDataError;
            break;
        }
    case Start_MicroPhone_Error:
        {
            VR_LOGI("Start_MicroPhone_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_MicrophoneError;
            break;
        }
    case Start_PTTSW_Error:
        {
            VR_LOGI("Start_PTTSW_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_PTTSWError;
            break;
        }
    case Start_HigherInterput_Error:
        {
            VR_LOGI("Start_HigherInterput_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_HigherInterruptError;
            break;
        }
    case Start_Condition_Ok:
        {
            VR_LOGI("Start_Condition_Ok");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_Avaiable;
            break;
        }
    default:
        break;
    }

#ifdef VR_START_SERVICE_CONDITION_TEST
    serviceStatus = VrReqServiceStatusResult_VrServiceStatus_Avaiable;
#endif
    startResult->set_status(serviceStatus);
    m_spEventIF->Reply(*m_spEventContext.get(), replyMeg.release());
}

void VR_StartServiceCondition::OnMessage(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const VrStartServiceResult& startServiceResult = (const VrStartServiceResult&)msg;
    if (!startServiceResult.has_code()) {
        VR_LOGI("startServiceResult.has_code: [%d]", startServiceResult.has_code());
        return;
    }
    VrStartServiceConidtionType type = startServiceResult.code();
    int nValue = startServiceResult.result();
    switch (type) {
    case StartService_SearchFunc:
        {
            VR_LOGI("[Set Search Func data]");
            SetServiceData(nValue, Search_Func);
            break;
        }
    case StartService_DictionData:
        {
            VR_LOGI("[Set Diction data]");
            SetServiceData(nValue, Dic_Data);
            break;
        }
    case StartService_LangData:
        {
            VR_LOGI("[Set Lang data]");
            SetServiceData(nValue, Lang_Data);
            break;
        }
    case StartService_ContactData:
        {
            VR_LOGI("[Set Contact data]");
            SetServiceData(nValue, ContactData);
            break;
        }
    case StartService_MediaVOCS:
        {
            VR_LOGI("[Set Media VOCS]");
            SetServiceData(nValue, MediaVOCS);
            break;
        }
    case StartService_MicPhone:
        {
            VR_LOGI("[Set Mic Data]");
            SetServiceData(nValue, Micphone);
            break;
        }
    case StartService_PTTSW:
        {
            VR_LOGI("[Set PTT SW]");
            SetServiceData(nValue, Pttsw);
            break;
        }
    case StartService_Higher:
        {
            VR_LOGI("[Set Higher]");
            SetServiceData(nValue, Higher_Interupt);
            break;
        }
    case StartService_dic_lang:
        {
            VR_LOGI("[Set dic and lang]");
            SetServiceData(nValue, Dic_Lang_Data);
            m_errorCode = m_vrstart_condition.CheckConditionData();
            OnReplyCommonProperty();
            break;
        }
    default:
        break;
    }
}

void VR_StartServiceCondition::Initialize()
{
    VR_LOGD_FUNC();
    this->InitializeData();
}

void VR_StartServiceCondition::SetInitState()
{
    VR_LOGD_FUNC();
    VR_LOGI("-----set start value------");
    m_vrstart_condition.m_bPTTSW = true;
    m_vrstart_condition.m_bSearchFunc = true;
    m_vrstart_condition.m_bMicrophone = true;
    m_vrstart_condition.m_bMediaVOCSData = true;
    m_vrstart_condition.m_bLangData = true;
    m_vrstart_condition.m_bHigherInterrupt = true;
    m_vrstart_condition.m_bDictionData = true;
    m_vrstart_condition.m_bContactData = true;
    VR_LOGI("-----set start value end ---------");
}

bool VR_StartServiceCondition::CheckPttSW()
{
    VR_LOGD_FUNC();
    m_vrstart_condition.m_bPTTSW = true;
    return m_vrstart_condition.m_bPTTSW;
}

bool VR_StartServiceCondition::CheckMircophone()
{
    VR_LOGD_FUNC();
    VR_ContentProvider vr_cp;
    std::string strValue = vr_cp.GetContentProviderValue(VR_MICROPHONE_URL);
    VR_LOGI("strValue = [%s]", strValue.c_str());
    if ("valid" == strValue) {
        m_vrstart_condition.m_bMicrophone = true;
    }
    else if ("invalid" == strValue) {
        m_vrstart_condition.m_bMicrophone = false;
    }
    else {
        m_vrstart_condition.m_bMicrophone = true;
    }
    return m_vrstart_condition.m_bMicrophone;
}

bool VR_StartServiceCondition::CheckSearchFunc()
{
    VR_LOGD_FUNC();
    m_vrstart_condition.m_bSearchFunc = true;
    return m_vrstart_condition.m_bSearchFunc;
}

bool VR_StartServiceCondition::CheckHigerInterrupt()
{
    VR_LOGD_FUNC();
    m_vrstart_condition.m_bHigherInterrupt = true;
    return m_vrstart_condition.m_bHigherInterrupt;
}

bool VR_StartServiceCondition::CheckMediaVOCS()
{
    VR_LOGD_FUNC();
    m_vrstart_condition.m_bMediaVOCSData = true;
    nutshell::media::MediaInfo_GracenoteStatus graceStatus = { 0, 0 };
    bool bResult = nutshell::media::NMMediaInfoHelper::load(nutshell::media::MediaInfoKey_GracenoteStatus, &graceStatus, sizeof(graceStatus));
    if (bResult) {
        VR_LOGI("graceStatus.status = [%d]", graceStatus.status);
        if (1 == graceStatus.status) {
            m_vrstart_condition.m_bMediaVOCSData = false;
        }
    }
    return m_vrstart_condition.m_bMediaVOCSData;
}

bool VR_StartServiceCondition::CheckContactData()
{
    VR_LOGD_FUNC();
    VR_ContentProvider  cp;
    std::string strValue = cp.GetContentProviderValue(VR_BT_PB_MODE);
    VR_LOGI("strValue = [%s]", strValue.c_str());
    if ("1" == strValue || "2" == strValue || "3" == strValue || "4" == strValue) {
        m_vrstart_condition.m_bContactData = false;
    }
    else {
        m_vrstart_condition.m_bContactData = true;
    }
    return m_vrstart_condition.m_bContactData;
}

void VR_StartServiceCondition::CheckLangDicData()
{
    VR_LOGD_FUNC();
    static const VR_String requestXml = "<event name = \"getResourceState\">"
                                        "</event>";

    VR_LOGI("send meg to de:[%s]", requestXml.c_str());
    m_spEngineIF->SendMessage(requestXml);
}

Start_Condition_Error VR_StartServiceCondition::CheckStartData()
{
    VR_LOGD_FUNC();
    VR_LOGI("----- sync check -----");
    CheckPttSW();
    CheckMircophone();
    CheckSearchFunc();
    CheckHigerInterrupt();
    CheckMediaVOCS();
    CheckContactData();
    VR_LOGI("----- async check -------");
    m_errorCode = m_vrstart_condition.CheckConditionData();
    return m_errorCode;
}

void VR_StartServiceCondition::SetServiceData(int nValue, Condition_Data_Type dataType)
{
    VR_LOGD_FUNC();
    VR_LOGI("StartServiceCondition = [%d]", nValue);
    bool bValue(false);
    if (dataType != Dic_Lang_Data) {
        if (nValue) {
            bValue = true;
        }
    }
    else {
        if (nValue == VR_LANG_DICTION_VALID) {
            m_vrstart_condition.m_bLangData = true;
            m_vrstart_condition.m_bDictionData = true;
        }
        else if (nValue == VR_LANG_VALID_AND_DICTION_INVALID) {
            m_vrstart_condition.m_bLangData = true;
            m_vrstart_condition.m_bDictionData = false;
        }
        else if (nValue == VR_LANG_INVALID_AND_DICTION_VALID) {
            m_vrstart_condition.m_bLangData = false;
            m_vrstart_condition.m_bDictionData = true;
        }
        else {
            m_vrstart_condition.m_bLangData = false;
            m_vrstart_condition.m_bDictionData = false;
        }
        return;
    }
    switch (dataType) {
    case Search_Func:
        {
            m_vrstart_condition.m_bSearchFunc = bValue;
            break;
        }
    case Dic_Data:
        {
            m_vrstart_condition.m_bDictionData = bValue;
            break;
        }
    case Lang_Data:
        {
            m_vrstart_condition.m_bLangData = bValue;
            break;
        }
    case ContactData:
        {
            m_vrstart_condition.m_bContactData = bValue;
            break;
        }
    case MediaVOCS:
        {
            m_vrstart_condition.m_bMediaVOCSData = bValue;
            break;
        }
    case Micphone:
        {
            m_vrstart_condition.m_bMicrophone = bValue;
            break;
        }
    case Pttsw:
        {
            m_vrstart_condition.m_bPTTSW = bValue;
            break;
        }
    case Higher_Interupt:
        {
            m_vrstart_condition.m_bHigherInterrupt = bValue;
            break;
        }
    default:
        break;
    }
}

void VR_StartServiceCondition::InitializeData()
{
    VR_LOGD_FUNC();
    nutshell::NCEnvironment environment;
    nutshell::NCString ncContry;
    environment.getProductCountry(ncContry);
    std::string strContry = ncContry.getString();
    VR_LOGI("strContry = [%s]", strContry.c_str());
    if ("America" == strContry || "Canada" == strContry || "Mexico" == strContry) {
        m_RegionType = North_America_Region;
    }
    else if ("European" == strContry || "Russia" == strContry) {
        m_RegionType = Europen_Region;
    }
    else {
        m_RegionType = General_Region;
    }
    nutshell::NCString ncProduct;
    environment.getProductName(ncProduct);
    std::string strProduct = ncProduct.getString();
    VR_LOGI("strProduct = [%s]", strProduct.c_str());
    if ("T0" == strProduct) {
        m_ProductType = Product_T0;
    }
    else if ("T1" == strProduct) {
        m_ProductType = Product_T1;
    }
    else if ("T2" == strProduct) {
        m_ProductType = Product_T2;
    }
    else if ("T-EMV" == strProduct) {
        m_ProductType = Product_TEMV;
    }
    else if ("L1" == strProduct) {
        m_ProductType = Product_L1;
    }
    else if ("L1.5" == strProduct) {
        m_ProductType = Product_L1_5;
    }
    else if ("L2" == strProduct) {
        m_ProductType = Product_L2;
    }
    else {
    }
}

/* EOF */
