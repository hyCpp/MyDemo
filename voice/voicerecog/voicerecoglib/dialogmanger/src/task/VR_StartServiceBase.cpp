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
* @file  VR_StartServiceBase.cpp
* @brief Declaration file of class VR_StartServiceBase.
*
* This file includes the declaration of class VR_StartServiceBase.
*
* @attention used for C++ only.
*/

#include "stdafx.h"
#include "VR_StartServiceBase.h"
#include "VR_ContentProvider.h"
#include "VR_ConfigureIF.h"
#include "system/NCEnvironment.h"
#include "MediaInfoProvider/NMMediaInfoHelper.h"
#include "MediaInfoProvider/NMMediaInfoDEF.h"
#include "cutils/properties.h"

#define VR_LANG_DICTION_VALID 0
#define VR_LANG_VALID_AND_DICTION_INVALID 1
#define VR_LANG_INVALID_AND_DICTION_VALID 2
#define VR_LANG_DICTION_INVALID           3

Start_Condition_Error VR_StartCondition::CheckConditionData() const
{
    VR_LOGD_FUNC();
    if (!m_bMicrophone) {   // Microphone  1
        return(Start_MicroPhone_Error);
    }
    else if (!m_bSearchFunc) { // Demo On & Search readying 2 3
        return(Start_SearchFun_Error);
    }
    else if (!m_bDictionData) {     // DE initing 4
        return(Start_DictionData_Error);
    }
    else if (!m_bLangData) {    // Busy 5
        return(Start_LangData_Error);
    }
    else if (!m_bContactData) {     // code invaild
        return(Start_ContactData_Error);
    }
    else if (!m_bMediaVOCSData) {   // VoCon Music Premium Update  6
        return(Start_MediaVOCS_Error);
    }
    else if (!m_bPTTSW) {  //  code invaild
        return(Start_PTTSW_Error);
    }
    else if (!m_bChinaLanguage) { // China Language error 7
        return(Start_ChinaLanguage_Error);
    }
    else if (!m_bPhoneIncoming) { // 8
        return(Start_PhoneIncoming_Error);
    }
    else if (!m_bPhoneDialing) { // 9
        return(Start_PhoneDialing_Error);
    }
    else if (!m_bHigherInterrupt) {  // 10
        return(Start_HigherInterput_Error);
    }
    else if (!m_bTslInstalling) {  // 10
        return(Start_TslInstalling_Error);
    }
    else if (!m_bPersonData) {
        return(Start_PersonData_Error);
    }
    else if (!m_bSmartAgent) {
        return(Start_SmartAgent_Error);
    }
    else {
        return(Start_Condition_Ok);
    }
}

VR_StartServiceBase::VR_StartServiceBase()
    : m_vrstart_condition()
    , m_errorCode(Start_Condition_Unkown)
    , m_RegionType(North_America_Region)
    , m_ProductType(Product_T0)
    , m_nPeerPID(0)
    , m_checkstart_reason()
    , m_checkstart_status()
    , m_cHintsString("")
    , m_bUICrashStatus(false)
{
    VR_LOGD_FUNC();
}

VR_StartServiceBase::~VR_StartServiceBase()
{
    VR_LOGD_FUNC();
}

bool VR_StartServiceBase::CheckPttSW()
{
    VR_LOGD_FUNC();
    return true;
}

bool VR_StartServiceBase::CheckMircophone()
{
    VR_LOGD_FUNC();
#ifdef HAVE_NUTSHELL_OS
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
#endif
    return m_vrstart_condition.m_bMicrophone;
}

bool VR_StartServiceBase::CheckSearchFunc()
{
    VR_LOGD_FUNC();
    m_vrstart_condition.m_bSearchFunc = true;
#ifdef HAVE_NUTSHELL_OS
    std::string keyName = "local.nutshell.navi";
    std::string strDefault = "ready";  // set the default value is ready for dcu, and no navi condition
    char keyValue[PROPERTY_VALUE_MAX] = { 0 };
    property_get(keyName.c_str(), keyValue, strDefault.c_str());
    VR_LOGI("VR_StartServiceBase keyValue is :[%s]", keyValue);
    std::string strValue = keyValue;
    if (strValue == strDefault) {
       m_vrstart_condition.m_bSearchFunc = true;
    }
    else {
        m_vrstart_condition.m_bSearchFunc = false;
    }
    //  navifulldata , vr start ok
    VR_ContentProvider  vr_cp;
    std::string  strMode = vr_cp.GetContentProviderValue(VR_FULLUPDATE_STATUS);
    if ("navifulldata" == strMode) {
        m_vrstart_condition.m_bSearchFunc = true;
    }
#endif
    // Demo Run , if Run , vr work not , otherwise , work
    VR_ConfigureIF* pConfig = VR_ConfigureIF::Instance();
    if (pConfig) {
        bool bDemoRun(false);
        nutshell::INT32 nWroadKind(0);
        nutshell::INT32 nLon(0);
        nutshell::INT32 nLat(0);
        pConfig->getLocateInfo(nWroadKind, nLon, nLat, bDemoRun);
        if (bDemoRun) {
            VR_LOGD("Navi demo is running: [%d]", bDemoRun);
            m_vrstart_condition.m_bSearchFunc = false;
        }
    }

    // navi status check
    if (pConfig) {
        unsigned int uiNaviStatus;
        uiNaviStatus = pConfig->getNaviStatus();
        if (3 != uiNaviStatus) { // EVIP_SERVICE_STATUS_READY != uiNaviStatus
            VR_LOGD("Navi status is: [%d]", uiNaviStatus);
            m_vrstart_condition.m_bSearchFunc = false;
        }
    }

    return(m_vrstart_condition.m_bSearchFunc);
}

bool VR_StartServiceBase::CheckHigerInterrupt()
{
    VR_LOGD_FUNC();
    return true;
}

bool VR_StartServiceBase::CheckChinaLanguage()
{
    VR_LOGD_FUNC();
    return true;
}

bool VR_StartServiceBase::CheckTslInstalling()
{
    VR_LOGD_FUNC();
    return true;
}

bool VR_StartServiceBase::CheckPhoneCall()
{
    VR_LOGD_FUNC();
#ifdef HAVE_NUTSHELL_OS
    VR_ContentProvider cp;
    std::string strPhoneState = cp.GetContentProviderValue(VR_BT_PHONECALL);
    VR_LOGI("strPhoneState = [%s]", strPhoneState.c_str());
    if ("incoming" == strPhoneState) {
        m_vrstart_condition.m_bPhoneIncoming = false;
        return(m_vrstart_condition.m_bPhoneIncoming);
    }
    else if (("dialing" == strPhoneState) || ("waiting" == strPhoneState)) {
        m_vrstart_condition.m_bPhoneDialing = false;
        return(m_vrstart_condition.m_bPhoneDialing);
    }
    else {
        m_vrstart_condition.m_bPhoneIncoming = true;
        m_vrstart_condition.m_bPhoneDialing = true;
        return(true);
    }
#else
    return false;
#endif
}

bool VR_StartServiceBase::CheckPersionData()
{
    VR_LOGD_FUNC();
    m_vrstart_condition.m_bPersonData = VR_ConfigureIF::Instance()->getPersonDataStatus();
    VR_LOGI("m_bPersonData=[%d]", m_vrstart_condition.m_bPersonData);

    return(m_vrstart_condition.m_bPersonData);
}

bool VR_StartServiceBase::CheckSmartAgent()
{
    VR_LOGD_FUNC();
    m_vrstart_condition.m_bSmartAgent = VR_ConfigureIF::Instance()->getSmartAgentStatus();
    VR_LOGI("m_bSmartAgent=[%d]", m_vrstart_condition.m_bSmartAgent);

    return(m_vrstart_condition.m_bSmartAgent);
}

bool VR_StartServiceBase::CheckMediaVOCS()
{
    VR_LOGD_FUNC();
    return true;
}

bool VR_StartServiceBase::CheckContactData()
{
    VR_LOGD_FUNC();
    return true;
}

void VR_StartServiceBase::CheckStartReason()
{
    if (m_checkstart_reason.m_bInInitializing) {
        m_vrstart_condition.m_bLangData = false;
    }
    else {
        m_vrstart_condition.m_bLangData = true;
    }

    if (m_checkstart_reason.m_bUpdatingDictionary) {
        m_vrstart_condition.m_bDictionData = false;
    }
    else {
        m_vrstart_condition.m_bDictionData = true;
    }

    if (m_checkstart_reason.m_bMapUpdateError || m_checkstart_reason.m_bNavigationDemo || m_checkstart_reason.m_bMapUpdateError) {
        m_vrstart_condition.m_bSearchFunc = false;
    }
    else {
        m_vrstart_condition.m_bSearchFunc = true;
    }
}

void VR_StartServiceBase::CheckStartStatus()
{
    VR_LOGD_FUNC();
}

void VR_StartServiceBase::CheckLangDicData()
{
    VR_LOGD_FUNC();
}

Start_Condition_Error  VR_StartServiceBase::CheckStartData()
{
    VR_LOGD_FUNC();

    VR_LOGI("----- sync check -----");
    CheckPttSW();
    CheckMircophone();
    CheckSearchFunc();
    CheckHigerInterrupt();
    CheckMediaVOCS();
    CheckContactData();
    CheckChinaLanguage();
    VR_LOGI("----- async check -------");
    m_errorCode = m_vrstart_condition.CheckConditionData();
    return m_errorCode;
}

void VR_StartServiceBase::OnStartMessage(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
}

void VR_StartServiceBase::SetInitState()
{
    VR_LOGD_FUNC();
}

void VR_StartServiceBase::InitializeData()
{
    VR_LOGD_FUNC();
#ifdef HAVE_NUTSHELL_OS
    VR_ContentProvider  vr_cp;
    std::string strValue = vr_cp.GetContentProviderValue(VR_MICROPHONE_URL);
    if ("valid" == strValue) {
        m_vrstart_condition.m_bMicrophone = true;
    }
#endif
}

void  VR_StartServiceBase::SetServiceData(int nValue, Condition_Data_Type dataType)
{
    VR_LOGD_FUNC();
}
/* EOF */
