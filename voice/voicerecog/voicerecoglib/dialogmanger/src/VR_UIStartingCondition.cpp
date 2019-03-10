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
 * @file VR_UIStartingCondition.cpp
 * @brief Declaration file of class VR_UIStartingCondition.
 *
 * This file includes the declaration of class VR_UIStartingCondition.
 *
 * @attention used for C++ only.
 */

#include "stdafx.h"
#include "VR_Configure.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "VR_EventSenderIF.h"
#include "VR_DialogEngineIF.h"
#include "VR_ContentProvider.h"
#include "VR_UIStartingCondition.h"
#include "VR_DailogEngineMessageBuilder.h"
#include <boost/format.hpp>

#include "navi/Voice/VoiceRecog/VrCommonDef.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyCommonProperty.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrCommonProperty.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMTaskMessage.pb.h"
#include "navi/Voice/VoiceRecog/RespVrCommonPropertyResult.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyRecogResult.pb.h"

using namespace navi::VoiceRecog;

VR_UIStartingCondition::VR_UIStartingCondition(spVR_DialogEngineIF spEngine, spVR_EventSenderIF spSender)
    : m_spEngineIF(spEngine)
    , m_spEventSenderIF(spSender)
    , m_bSearchFunc(false)
    , m_bDictionaryData(false)
    , m_bLanguageData(false)
    , m_bContactData(false)
    , m_bMediaVOCSData(false)
    , m_bPVRFlagData(false)
{
    VR_LOGD_FUNC();
}

VR_UIStartingCondition::~VR_UIStartingCondition()
{
    VR_LOGD_FUNC();
}

void VR_UIStartingCondition::OnReqestCommonProperty(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    const RequestVrCommonProperty& reqCommonProperty = (const RequestVrCommonProperty&)msg;
    if (!reqCommonProperty.has_reqstartcondition()) {
        VR_LOGE("has_reqstartconditin = [%d]", reqCommonProperty.has_reqstartcondition());
        return;
    }
    const VrReqStartConditions& startCondition = reqCommonProperty.reqstartcondition();
    if (!startCondition.has_code()) {
        VR_LOGE("has_code = [%d]", startCondition.has_code());
        return;
    }

    std::unique_ptr<RespVrCommonPropertyResult> replyMsg(RespVrCommonPropertyResult::default_instance().New());
    replyMsg->set_sender("dm");
    replyMsg->set_taskid(-1);
    VrStartConditionsResult* startConditionResult = replyMsg->mutable_startconditionsresult();
    startConditionResult->set_result(true);
    m_spEventSenderIF->Reply(context, replyMsg.release());

    VrStartCondition code = startCondition.code();
    switch (code) {
    case  VrStartCondition_SearchFunc:
        {
            break;
        }
    case VrStartCondition_DictionaryData:
        {
            break;
        }
    case VrStartCondition_LanguageData:
        {
            break;
        }
    case VrStartCondition_ContactData:
        {
            break;
        }
    case VrStartCondition_MediaVOCSData:
        {
            break;
        }
    case VrStartCondition_PVR:
        {
            NotifyPVRFlagData();
            break;
        }
    case VrStartCondition_All:
        {
            NotifyAllData();
            break;
        }
    default:
        break;
    }
}

void VR_UIStartingCondition::NotifySearchFunc()
{
    VR_LOGD_FUNC();

    std::unique_ptr<VrNotifyCommonProperty> notifyCommonProperty(VrNotifyCommonProperty::default_instance().New());
    notifyCommonProperty->set_sender("dm");
    notifyCommonProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyStartCondition);
    VrNotifyStartCondition* startCondition = notifyCommonProperty->mutable_startcondition();
    startCondition->set_code(VrStartCondition_SearchFunc);
    VR_ConfigureIF* configIF = VR_ConfigureIF::Instance();
    bool bHybridVRFlag = false;
    if (configIF) {
        bHybridVRFlag = configIF->getHybridVRFlag();
    }
    startCondition->set_result(bHybridVRFlag);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, notifyCommonProperty.release());
}

void VR_UIStartingCondition::NotifyDictionaryData()
{
    VR_LOGD_FUNC();

    std::unique_ptr<VrNotifyCommonProperty> notifyCommonProperty(VrNotifyCommonProperty::default_instance().New());
    notifyCommonProperty->set_sender("dm");
    notifyCommonProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyStartCondition);
    VrNotifyStartCondition* startCondition = notifyCommonProperty->mutable_startcondition();
    startCondition->set_code(VrStartCondition_DictionaryData);
    startCondition->set_result(m_bDictionaryData);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, notifyCommonProperty.release());
}

void VR_UIStartingCondition::NotifyLanguageData()
{
    VR_LOGD_FUNC();

    std::unique_ptr<VrNotifyCommonProperty> notifyCommonProperty(VrNotifyCommonProperty::default_instance().New());
    notifyCommonProperty->set_sender("dm");
    notifyCommonProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyStartCondition);
    VrNotifyStartCondition* startCondition = notifyCommonProperty->mutable_startcondition();
    startCondition->set_code(VrStartCondition_LanguageData);
    startCondition->set_result(m_bLanguageData);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, notifyCommonProperty.release());
}

void VR_UIStartingCondition::NotifyContactData()
{
    VR_LOGD_FUNC();

    std::unique_ptr<VrNotifyCommonProperty> notifyCommonProperty(VrNotifyCommonProperty::default_instance().New());
    notifyCommonProperty->set_sender("dm");
    notifyCommonProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyStartCondition);
    VrNotifyStartCondition* startCondition = notifyCommonProperty->mutable_startcondition();
    startCondition->set_code(VrStartCondition_ContactData);
    startCondition->set_result(m_bContactData);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, notifyCommonProperty.release());
}

void VR_UIStartingCondition::NotifyMedaiVOCSData()
{
    VR_LOGD_FUNC();

    std::unique_ptr<VrNotifyCommonProperty> notifyCommonProperty(VrNotifyCommonProperty::default_instance().New());
    notifyCommonProperty->set_sender("dm");
    notifyCommonProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyStartCondition);
    VrNotifyStartCondition* startCondition = notifyCommonProperty->mutable_startcondition();
    startCondition->set_code(VrStartCondition_MediaVOCSData);
    startCondition->set_result(m_bMediaVOCSData);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, notifyCommonProperty.release());
}

void VR_UIStartingCondition::NotifyPVRFlagData()
{
    VR_LOGD_FUNC();

    std::unique_ptr<VrNotifyCommonProperty> notifyCommonProperty(VrNotifyCommonProperty::default_instance().New());
    notifyCommonProperty->set_sender("dm");
    notifyCommonProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyInitialCondition);
    VrNotifyInitialCondition* startCondition = notifyCommonProperty->mutable_initialcondition();
    startCondition->set_code(VrStartCondition_PVR); // when ui crash , we need get new pvr flag, not InitializeData,,,
#ifdef HAVE_NUTSHELL_OS
    VR_ContentProvider  vr_cp;
    std::string  strPvr = vr_cp.GetContentProviderValue(VR_CP_PVR_FLAG);
    if ("On" == strPvr || "on" == strPvr) {
        m_bPVRFlagData = true;
    }
    else {
        m_bPVRFlagData = false;
    }
    VR_LOGI("strPvr = [%s], m_bPVRFlagData = [%d]", strPvr.c_str(), m_bPVRFlagData);
#endif
    startCondition->set_startconditionresult(m_bPVRFlagData);
    startCondition->set_hintsflag(true);
    VR_String hintsParam = VR_DialogEngineMessageBuilder::CreateGetHintsDialogMsg(0);
    const std::string strContent = m_spEngineIF->getHints(hintsParam);
    VR_LOGD("strContent = [%s]", strContent.c_str());
    startCondition->set_hints(strContent);

    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, notifyCommonProperty.release());
}

void  VR_UIStartingCondition::NotifyPhoneState()
{
    VR_LOGD_FUNC();
#ifdef HAVE_NUTSHELL_OS
    VR_ContentProvider vr_cp;
    std::string strPhoneState = vr_cp.GetContentProviderValue(VR_BT_PHONECALL);
    static const std::string  stateXml =     "<action agent=\"phone\" op=\"phoneState\">"
                                                  "<phoneInCall>%1%</phoneInCall>"
                                             "</action>";
    std::string resultXml = (boost::format(stateXml) % strPhoneState).str();
    VR_LOGD("PhoneState Xml To FC:[%s]", resultXml.c_str());

    std::unique_ptr<VrNotifyAction>  notifyAction(VrNotifyAction::default_instance().New());
    notifyAction->set_sender("dm");
    notifyAction->set_taskid(-1);    // -1 means invalid id
    notifyAction->set_actionid(-1);
    notifyAction->set_actioncontent(resultXml);

    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYACTION, notifyAction.release());
#endif
}


void VR_UIStartingCondition::NoitfyUnitState()
{
    VR_LOGD_FUNC();
#ifdef HAVE_NUTSHELL_OS
    VR_ContentProvider  vr_cp;
    std::string  strUnit = vr_cp.GetContentProviderValue(VR_CP_UNIT_OF_MEASUREMENT);
    VR_String  xmlResult  = "<display agent=\"Common\" content=\"distance_unit\">"
                                "<unit>%1%</unit>"
                            "</display>";
    VR_String  strResult = (boost::format(xmlResult) % strUnit).str();
    VR_LOGD("strUnit = [%s], strResult = [%s]", strUnit.c_str(), strResult.c_str());

    std::unique_ptr<VrNotifyRecogResult> notifyMessage(VrNotifyRecogResult::default_instance().New());
    notifyMessage->set_sender("dm");
    notifyMessage->set_taskid(-1);
    notifyMessage->set_recogresult(strResult);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYRECOGRESULT, notifyMessage.release());
#endif
}

void VR_UIStartingCondition::NotifyAllData()
{
    VR_LOGD_FUNC();

    NotifyPVRFlagData();
    NotifySearchFunc();
    VR_ConfigureIF *pConfig = VR_ConfigureIF::Instance();
    if (pConfig) {
        bool bConnect = pConfig->getInnerAvcLanConnectStatus();
        if (bConnect) {
            this->NoitfyUnitState();
        }
        else {
            this->NotifyPhoneState();
        }
    }
}

void VR_UIStartingCondition::InitializeData()
{
    VR_LOGD_FUNC();
#ifdef HAVE_NUTSHELL_OS
    VR_ContentProvider vr_cp;
    std::string strPvr = vr_cp.GetContentProviderValue(VR_CP_PVR_FLAG);
    VR_LOGI("strPvr = [%s]", strPvr.c_str());
    if (strPvr == "On" || strPvr == "on") {
        m_bPVRFlagData = true;
    }
    else {
        m_bPVRFlagData = false;
    }
#endif
}

void VR_UIStartingCondition::TransforLanguage()
{
    VR_LOGD_FUNC();

    std::unique_ptr<VrNotifyCommonProperty> notifyCommonProperty(VrNotifyCommonProperty::default_instance().New());
    notifyCommonProperty->set_sender("dm");
    notifyCommonProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyInitialCondition);
    VrNotifyInitialCondition* startCondition = notifyCommonProperty->mutable_initialcondition();

    startCondition->set_hintsflag(true);
    VR_String hintsParam = VR_DialogEngineMessageBuilder::CreateGetHintsDialogMsg(0);
    const std::string strContent = m_spEngineIF->getHints(hintsParam);
    VR_LOGD("strContent = [%s]", strContent.c_str());
    startCondition->set_hints(strContent);

    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, notifyCommonProperty.release());
}

void VR_UIStartingCondition::UpdateData(Update_Type type)
{
    VR_LOGD_FUNC();

    switch (type) {
    case startsearch_func:
        {
            break;
        }
    case startdictionary_data:
        {
            break;
        }
    case startlanguage_data:
        {
            break;
        }
    case startcontact_data:
        {
            break;
        }
    case startmediavocs:
        {
            break;
        }
    case startpvrflag:
        {
            NotifyPVRFlagData();
            break;
        }
    default:
        break;
    }
}

void VR_UIStartingCondition::OnMessage(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    const VrStartConditonComfirm& comfirm = (const VrStartConditonComfirm&)msg;
    if (!comfirm.has_code()) {
        VR_LOGE("has_code = [%d]", comfirm.has_code());
        return;
    }

    VrStartConditionType type = comfirm.code();
    int nResult = comfirm.result();
    bool bResult = true;
    if (0 == nResult) {
        bResult = false;
    }
    switch (type) {
    case VrStartType_SearchFunc :
    {
        if (bResult != m_bSearchFunc) {
            m_bSearchFunc = bResult;
            UpdateData(startsearch_func);
        }
        break;
    }
    case VrStartType_DictionaryData :
    {
        if (bResult != m_bDictionaryData) {
            m_bDictionaryData = bResult;
            UpdateData(startdictionary_data);
        }
        break;
    }
    case VrStartType_LanguageData :
    {
        if (bResult != m_bLanguageData) {
            m_bLanguageData = bResult;
            UpdateData(startlanguage_data);
        }
        break;
    }
    case VrStartType_ContactData :
    {
        if (bResult != m_bContactData) {
            m_bContactData = bResult;
            UpdateData(startcontact_data);
        }
        break;
    }
    case VrStartType_MediaVOCSData :
    {
        if (bResult != m_bMediaVOCSData) {
            m_bMediaVOCSData = bResult;
            UpdateData(startmediavocs);
        }
        break;
    }
    case VrStartCondition_PVR :
    {
        if (bResult != m_bPVRFlagData) {
            m_bPVRFlagData = bResult;
            UpdateData(startpvrflag);
        }
        break;
    }
    case VrStatrType_Dic_Language :
    {
        if (0 == nResult) {
            bResult = true;
            if (bResult != m_bDictionaryData) {
                m_bDictionaryData = bResult;
                UpdateData(startdictionary_data);
            }
            if (bResult != m_bLanguageData) {
                m_bLanguageData = bResult;
                UpdateData(startlanguage_data);
            }
        }
        else if (1 == nResult) {
            bResult = false;
            if (bResult != m_bDictionaryData) {
                m_bDictionaryData = bResult;
                UpdateData(startdictionary_data);
            }
        }
        else if (2 == nResult) {
            bResult = false;
            if (bResult != m_bLanguageData) {
                m_bLanguageData = bResult;
                UpdateData(startlanguage_data);
            }
        }
        else if (3 == nResult) {
            bResult = false;
            if (bResult != m_bDictionaryData) {
                m_bDictionaryData = bResult;
                UpdateData(startdictionary_data);
            }
            if (bResult != m_bLanguageData) {
                m_bLanguageData = bResult;
                UpdateData(startlanguage_data);
            }
        }
        break;
    }
    default :
        break;
    }
}
/* EOF */
