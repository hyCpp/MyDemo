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
 *  @file   VR_GBookCheckErrorAction.cpp
 *  @brief  Implementation  class  of  VR_GBookCheckErrorAction
 */

#include   "stdafx.h"
#include   "boost/format.hpp"
#include   "VR_ActionEventSender.h"
#include   "VR_GBookStatusCheck.h"
#include   "VR_GBookCheckErrorAction.h"

VR_GBookCheckErrorAction::VR_GBookCheckErrorAction(spVR_ActionEventSender spSender, int nEngineId, int nActionType, const VR_ProtoMessageBase &proto)
    : VR_ActionBase(spSender)
    , m_nEngineId(nEngineId)
    , m_nActionType(nActionType)
    , m_ReqMsg(proto.New())
    , m_spGookStatusCheck(VR_new  VR_GBookStatusCheck())
{
    VR_LOGD("nEngineId = [%d], nActionType = [%d]", m_nEngineId, m_nActionType);

    m_ReqMsg->CheckTypeAndMergeFrom(proto);
}

VR_GBookCheckErrorAction::~VR_GBookCheckErrorAction()
{
    VR_LOGD_FUNC();
}

void   VR_GBookCheckErrorAction::Run()
{
    VR_LOGD_FUNC();

    VR_GBook_ErrorCode  code = m_spGookStatusCheck->GetCommunicationError();
    VR_LOGD("code = [%d]", code);

    static  VR_String  startXml =  "<action-result agent = \"communication\""
                                       " op = \"queryGBookStatus\" errcode = \"0\" >"
                                       "<code>%1%</code>"
                                   "</action-result>";
    std::string strErrorCode("NoError");
    switch (code) {
    case VR_GBK_None :
        {
            strErrorCode = "NoError";
            break;
        }
    case VR_GBK_ComLock :
        {
            strErrorCode = "CommunicationIsLocked";
            break;
        }
    case VR_GBK_ServiceFlag_Off :
        {
            strErrorCode = "ServiceIsNotInContract";
            break;
        }
    case VR_GBK_ServiceFlag_Node :
        {
            strErrorCode = "ServiceIsCurrentlyStopped";
            break;
        }
    case VR_GBK_TimeData_InCorrect :
        {
            strErrorCode = "CommunicationIsNotPossible";
            break;
        }
    case VR_GBK_DCMStatus_Error :
        {
            strErrorCode = "PreparingForCommunication";
            break;
        }
    case VR_GBK_BTNotSetting :
        {
            strErrorCode = "BluetoothMustBeSetUp";
            break;
        }
    case VR_GBK_PhoneNotSelect :
        {
            strErrorCode = "APhoneMustBeSelected";
            break;
        }
    case VR_GBK_PhoneNotSetting :
        {
            strErrorCode = "MobilePhoneCommunicationNoSetUp";
            break;
        }
    case VR_GBK_ComDeveiceNotSelect :
        {
            strErrorCode = "NoCommunicationsConnection";
            break;
        }
    case VR_GBK_ComSettingNotInit :
        {
            strErrorCode = "NeedToSetInitializeCommunication";
            break;
        }
    case VR_GBK_WifiSetting :
        {
            strErrorCode = "WifiIsNotConnected";
            break;
        }
    case VR_GBK_COM_ConnectConf :
        {
            strErrorCode = "ConnectComfirm";
            break;
        }
    case VR_GBOOK_Errror :
        {
            strErrorCode = "SystemError";  // Interprocess communication failure
            break;
        }
    default:
        {
            strErrorCode = "SystemError";  // Interprocess communication failure
            break;
        }
    }
    VR_String  xmlResult = (boost::format(startXml) % strErrorCode).str();
    VR_LOGD("strErrorCode = [%s], xmlResult = [%s]", strErrorCode.c_str(), xmlResult.c_str());
    m_sender->sendEngineReply(m_nEngineId, xmlResult);

    this->SetActionComplete();
}


void   VR_GBookCheckErrorAction::OnReceive(const VR_ProtoMessageBase &proto)
{
    VR_LOGD_FUNC();
}

/* EOF */
